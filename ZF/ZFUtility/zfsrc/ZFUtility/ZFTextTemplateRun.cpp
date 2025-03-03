#include "ZFTextTemplateRun.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFTextTemplateRunParam, ZFTextTemplateRunParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateRunParam, ZFFilterForString, dirNameFilter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateRunParam, ZFFilterForString, dirContentFilter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateRunParam, ZFFilterForString, fileNameFilter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateRunParam, ZFFilterForString, fileContentFilter)

ZFTextTemplateRunParam &_ZFP_ZFTextTemplateRunParamDefault(void) {
    static ZFTextTemplateRunParam d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunParamCleaner, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTextTemplateRunParamCleaner) {
    ZFTextTemplateRunParam &d = ZFTextTemplateRunParamDefault();
    d.dirNameFilter = ZFFilterForString();
    d.dirContentFilter = ZFFilterForString();
    d.fileNameFilter = ZFFilterForString();
    d.fileContentFilter = ZFFilterForString();
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunParamCleaner)

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFFilterForString &nameFilter
        , ZF_OUT zfstring *outErrorHint
        );
static zfbool _ZFP_ZFTextTemplateRun_applyDir(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFTextTemplateRunParam &runParam
        , ZF_OUT zfstring *outErrorHint
        );
static zfbool _ZFP_ZFTextTemplateRun_applyFile(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFTextTemplateRunParam &runParam
        , ZF_OUT zfstring *outErrorHint
        );
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFTextTemplateRun
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN(const ZFTextTemplateParam &, textTemplateParam)
        , ZFMP_IN_OPT(const ZFTextTemplateRunParam &, runParam, ZFTextTemplateRunParam())
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        ) {
    zfstring pathTmp = ZFPathFormat(path);
    if(!ZFFileIsExist(pathTmp)) {
        zfstringAppend(outErrorHint, "path not exist: \"%s\"", path);
        return zffalse;
    }

    if(ZFFileIsDir(pathTmp)) {
        return _ZFP_ZFTextTemplateRun_applyDir(pathTmp, textTemplateParam, runParam, outErrorHint);
    }
    else {
        return _ZFP_ZFTextTemplateRun_applyFile(pathTmp, textTemplateParam, runParam, outErrorHint);
    }
}

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFFilterForString &nameFilter
        , ZF_OUT zfstring *outErrorHint
        ) {
    if(!nameFilter.filterPassed(path)) {
        return zftrue;
    }

    zfstring fileName;
    ZFFileNameOf(fileName, path);
    zfstring fileNameNew;
    ZFTextTemplateApply(textTemplateParam, ZFOutputForString(fileNameNew), fileName);
    if(fileName.compare(fileNameNew) == 0) {
        return zftrue;
    }
    if(fileNameNew.isEmpty()) {
        ZFFileRemove(path);
        path.removeAll();
        return zftrue;
    }

    zfstring pathNew;
    if(ZFPathParentOf(pathNew, path)) {
        pathNew += '/';
    }
    pathNew += fileNameNew;

    if(!ZFFileMove(path, pathNew)) {
        zfstringAppend(outErrorHint,
            "failed to move from \"%s\" to \"%s\"",
            path, pathNew);
        return zffalse;
    }

    path = pathNew;
    return zftrue;
}
static zfbool _ZFP_ZFTextTemplateRun_applyDir(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFTextTemplateRunParam &runParam
        , ZF_OUT zfstring *outErrorHint
        ) {
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.dirNameFilter, outErrorHint)) {
        return zffalse;
    }
    if(path.isEmpty()) {
        return zftrue;
    }
    if(!runParam.dirContentFilter.filterPassed(path)) {
        return zftrue;
    }

    zfbool ret = zftrue;
    ZFFileFindData fd;
    if(ZFFileFindFirst(fd, path)) {
        do {
            zfstring fullPath = path;
            fullPath += '/';
            fullPath += fd.name();
            if(fd.isDir()) {
                if(!_ZFP_ZFTextTemplateRun_applyDir(fullPath, textTemplateParam, runParam, outErrorHint)) {
                    ret = zffalse;
                    break;
                }
            }
            else {
                if(!_ZFP_ZFTextTemplateRun_applyFile(fullPath, textTemplateParam, runParam, outErrorHint)) {
                    ret = zffalse;
                    break;
                }
            }
        } while(ZFFileFindNext(fd));
        ZFFileFindClose(fd);
    }
    return ret;
}
static zfbool _ZFP_ZFTextTemplateRun_applyFile(
        ZF_IN_OUT zfstring &path
        , ZF_IN const ZFTextTemplateParam &textTemplateParam
        , ZF_IN const ZFTextTemplateRunParam &runParam
        , ZF_OUT zfstring *outErrorHint
        ) {
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.fileNameFilter, outErrorHint)) {
        return zffalse;
    }
    if(path.isEmpty()) {
        return zftrue;
    }
    if(!runParam.fileContentFilter.filterPassed(path)) {
        return zftrue;
    }

    // read entire file to buffer
    zfchar *buf = zfnull;
    zfchar *bufEnd = zfnull;
    {
        void *token = ZFFileOpen(path, v_ZFFileOpenOption::e_Read);
        if(token == zfnull) {
            zfstringAppend(outErrorHint, "failed to open file %s", path);
            return zffalse;
        }
        ZFFileCloseHolder(token);

        zfindex fileSize = ZFFileSize(token);
        if(fileSize == 0) {
            return zftrue;
        }

        buf = (zfchar *)zfmalloc(fileSize);
        if(buf == zfnull) {
            zfstringAppend(outErrorHint, "failed to malloc buffer for size %s", fileSize);
            return zffalse;
        }

        if(ZFFileRead(token, buf, fileSize) != fileSize) {
            zfstringAppend(outErrorHint, "failed to read file %s", path);
            zffree(buf);
            return zffalse;
        }
        bufEnd = buf + fileSize;
    }
    zfblockedFree(buf);

    if(ZFTextTemplateApply(textTemplateParam, ZFOutputForFile(path), buf, bufEnd - buf) == zfindexMax()) {
        zfstringAppend(outErrorHint, "failed to update template for %s", path);
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

