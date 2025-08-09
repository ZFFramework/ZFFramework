#include "ZFTextTemplateRun.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFArray *nameFilter
        , ZF_OUT zfstring *outErrorHint
        );
static zfbool _ZFP_ZFTextTemplateRun_applyDir(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFTextTemplateRunFilter *runFilter
        , ZF_OUT zfstring *outErrorHint
        );
static zfbool _ZFP_ZFTextTemplateRun_applyFile(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFTextTemplateRunFilter *runFilter
        , ZF_OUT zfstring *outErrorHint
        );
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFTextTemplateRun
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFTextTemplateParam &, textTemplateParam)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        zfstringAppend(outErrorHint, "IO impl not available: \"%s\"", pathInfo.pathType());
        return zffalse;
    }
    if(!ioImpl->ioIsExist(pathInfo.pathData())) {
        zfstringAppend(outErrorHint, "path not exist: \"%s\"", pathInfo);
        return zffalse;
    }

    zfstring path = pathInfo.pathData();
    if(ioImpl->ioIsDir(pathInfo.pathData())) {
        return _ZFP_ZFTextTemplateRun_applyDir(ioImpl, path, textTemplateParam, ZFTextTemplateRunFilter::instance(), outErrorHint);
    }
    else {
        return _ZFP_ZFTextTemplateRun_applyFile(ioImpl, path, textTemplateParam, ZFTextTemplateRunFilter::instance(), outErrorHint);
    }
}

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFArray *nameFilter
        , ZF_OUT zfstring *outErrorHint
        ) {
    for(zfindex i = 0; i < nameFilter->count(); ++i) {
        ZFRegExp *filter = nameFilter->get(i);
        if(filter && ZFRegExpMatch(path, filter)) {
            return zftrue;
        }
    }

    zfstring fileName;
    ioImpl->ioToFileName(fileName, path);
    zfstring fileNameNew;
    ZFTextTemplateApply(textTemplateParam, ZFOutputForString(fileNameNew), fileName);
    if(fileName.compare(fileNameNew) == 0) {
        return zftrue;
    }
    if(fileNameNew.isEmpty()) {
        ioImpl->ioRemove(path);
        path.removeAll();
        return zftrue;
    }

    zfstring pathNew;
    if(ZFPathParentOfT(pathNew, path)) {
        pathNew += '/';
    }
    pathNew += fileNameNew;

    if(!ioImpl->ioMove(path, pathNew)) {
        zfstringAppend(outErrorHint,
            "failed to move from \"%s\" to \"%s\"",
            path, pathNew);
        return zffalse;
    }

    path = pathNew;
    return zftrue;
}
static zfbool _ZFP_ZFTextTemplateRun_applyDir(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFTextTemplateRunFilter *runFilter
        , ZF_OUT zfstring *outErrorHint
        ) {
    if(!_ZFP_ZFTextTemplateRun_applyName(ioImpl, path, textTemplateParam, runFilter->dirNameFilter(), outErrorHint)) {
        return zffalse;
    }
    if(path.isEmpty()) {
        return zftrue;
    }
    for(zfindex i = 0; i < runFilter->dirContentFilter()->count(); ++i) {
        ZFRegExp *filter = runFilter->dirContentFilter()->get(i);
        if(filter && ZFRegExpMatch(path, filter)) {
            return zftrue;
        }
    }

    zfbool ret = zftrue;
    ZFIOFindData fd;
    if(ioImpl->ioFindFirst(fd, path)) {
        do {
            zfstring fullPath;
            ioImpl->ioToChild(fullPath, path, fd.name());
            if(fd.isDir()) {
                if(!_ZFP_ZFTextTemplateRun_applyDir(ioImpl, fullPath, textTemplateParam, runFilter, outErrorHint)) {
                    ret = zffalse;
                    break;
                }
            }
            else {
                if(!_ZFP_ZFTextTemplateRun_applyFile(ioImpl, fullPath, textTemplateParam, runFilter, outErrorHint)) {
                    ret = zffalse;
                    break;
                }
            }
        } while(ioImpl->ioFindNext(fd));
        ioImpl->ioFindClose(fd);
    }
    return ret;
}
static zfbool _ZFP_ZFTextTemplateRun_applyFile(
        ZF_IN ZFIOImpl *ioImpl
        , ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN ZFTextTemplateRunFilter *runFilter
        , ZF_OUT zfstring *outErrorHint
        ) {
    if(!_ZFP_ZFTextTemplateRun_applyName(ioImpl, path, textTemplateParam, runFilter->fileNameFilter(), outErrorHint)) {
        return zffalse;
    }
    if(path.isEmpty()) {
        return zftrue;
    }
    for(zfindex i = 0; i < runFilter->fileContentFilter()->count(); ++i) {
        ZFRegExp *filter = runFilter->fileContentFilter()->get(i);
        if(filter && ZFRegExpMatch(path, filter)) {
            return zftrue;
        }
    }

    // read entire file to buffer
    zfstring buf;
    {
        zfautoT<ZFIOToken> ioToken = ioImpl->ioOpen(path, v_ZFIOOpenOption::e_Read);
        if(ioToken == zfnull) {
            zfstringAppend(outErrorHint, "failed to open file %s", path);
            return zffalse;
        }

        zfindex fileSize = ioToken->ioSize();
        if(fileSize == 0) {
            return zftrue;
        }
        buf.capacity(fileSize);

        if(buf.capacity() < fileSize || ioToken->ioRead(buf.zfunsafe_buffer(), fileSize) != fileSize) {
            zfstringAppend(outErrorHint, "failed to read file %s", path);
            return zffalse;
        }
        buf.zfunsafe_length(fileSize);
        buf.zfunsafe_buffer()[fileSize] = '\0';
    }

    if(ZFTextTemplateApply(textTemplateParam, ZFOutputForFile(path), buf, buf.length()) == zfindexMax()) {
        zfstringAppend(outErrorHint, "failed to update template for %s", path);
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

