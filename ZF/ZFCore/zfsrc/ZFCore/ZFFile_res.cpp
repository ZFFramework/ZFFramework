#include "ZFFile.h"

#include "ZFPathType_res.h"
#include "ZFPathType_file.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes {
public:
    void *fd; // fd returned by ZFResOpen or ZFPathInfoOpen
    ZFPathInfo resExtPath; // true if fd created by ZFPathInfoOpen

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(zfnull)
    , resExtPath()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResExtPathDataHolder, ZFLevelZFFrameworkStatic) {
}
public:
    ZFCoreArray<ZFPathInfo> resExtPathList;
ZF_GLOBAL_INITIALIZER_END(ZFResExtPathDataHolder)
#define _ZFP_ZFResExtPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResExtPathDataHolder)->resExtPathList)

ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathAdd
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    if(pathInfo.pathType().isEmpty()) {
        return;
    }
    ZFCoreMutexLocker();
    _ZFP_ZFResExtPathList.add(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    if(pathInfo.pathType().isEmpty()) {
        return;
    }
    ZFCoreMutexLocker();
    _ZFP_ZFResExtPathList.removeElement(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFPathInfo>, ZFResExtPathList) {
    return _ZFP_ZFResExtPathList;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResExtPathCheck
        , ZFMP_OUT(ZFPathInfo &, resExtPath)
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    ZFCoreMutexLock();
    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i) {
        const ZFPathInfo t = l[i];
        ZFCoreMutexUnlock();

        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(t.pathType());
        if(impl) {
            if(impl->implIsExist(impl->implToChild(t.pathData(), resPath))) {
                ZFCoreMutexLocker();
                resExtPath = t;
                return zftrue;
            }
        }

        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
    return zffalse;
}
zfclassNotPOD _ZFP_ZFResExtKeyHash {
public:
    inline zfstlsize operator () (ZFPathInfo const &v) const {
        return (zfstlsize)zfidentityHash(
                zfidentityCalcString(v.pathType(), v.pathType().length())
                , zfidentityCalcString(v.pathData(), v.pathData().length())
                );
    }
    inline zfbool operator () (ZFPathInfo const &k1, ZFPathInfo const &k2) const {
        zfint t = k1.pathType().compare(k2.pathType());
        return (t < 0 || (t == 0 && k1.pathData().compare(k2.pathData()) < 0));
    }
};
typedef zfstlhashmap<ZFPathInfo, zfbool, _ZFP_ZFResExtKeyHash> _ZFP_ZFResExtMap;
static zfbool _ZFP_ZFResExtPathCheck(
        ZF_OUT ZFPathInfo &resExtPath
        , ZF_IN const zfchar *resPath
        , ZF_IN_OUT _ZFP_ZFResExtMap &m
        ) {
    ZFCoreMutexLock();
    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i) {
        if(m.find(l[i]) != m.end()) {
            continue;
        }
        m[l[i]] = zftrue;
        const ZFPathInfo t = l[i];
        ZFCoreMutexUnlock();

        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(t.pathType());
        if(impl) {
            if(impl->implIsExist(impl->implToChild(t.pathData(), resPath))) {
                ZFCoreMutexLocker();
                resExtPath = l[i];
                return zftrue;
            }
        }

        ZFCoreMutexLock();
    }
    ZFCoreMutexUnlock();
    return zffalse;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        return zftrue;
    }
    else {
        return ZFResRaw::IsExist(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsDir
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        resExtPath.pathData(ZFPathInfoToChild(resExtPath, resPath));
        return ZFPathInfoIsDir(resExtPath);
    }
    else {
        return ZFResRaw::IsDir(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFResCopy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(zfstring *, errPos, zfnull)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        ZFPathInfo errPosTmp;
        zfbool ret = ZFPathInfoCopy(
            ZFPathInfo(ZFPathType_res(), resPath),
            ZFPathInfo(ZFPathType_file(), dstPath),
            isRecursive,
            isForce,
            errPos != zfnull ? &errPosTmp : zfnull);
        if(errPos != zfnull && !ret) {
            *errPos += errPosTmp.pathData();
        }
        return ret;
    }
    else {
        return ZFResRaw::Copy(resPath, dstPath, isRecursive, isForce, errPos);
    }
}

#define _ZFP_ZFFileFindType_res "ZFResFindFirst"

zfclassNotPOD _ZFP_ZFResFindData {
public:
    zfstring resPathSaved;
    _ZFP_ZFResExtMap resExtResolved;
    zfstlhashmap<zfstring, zfbool> resExtItemResolved;
    ZFFileFindData resExtFd; // valid if resExtPath not empty
    /*
     * if not empty, the file is find from resExtPath
     * ensured end with file separator
     * ensured clear when find task ends from resExtPath
     */
    ZFPathInfo resExtPath;
    zfbool resFindFirstStarted;
public:
    _ZFP_ZFResFindData(void)
    : resPathSaved()
    , resExtResolved()
    , resExtItemResolved()
    , resExtFd()
    , resExtPath()
    , resFindFirstStarted(zffalse)
    {
    }
public:
    void copyToFd(ZF_OUT ZFFileFindData::Impl &fd) const {
        fd.name = this->resExtFd.impl().name;
        fd.isDir = this->resExtFd.impl().isDir;
    }
};

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    if(resPath == zfnull) {
        return zffalse;
    }
    _ZFP_ZFResFindData *implUserData = zfnew(_ZFP_ZFResFindData);
    fd.implAttach(_ZFP_ZFFileFindType_res, implUserData);
    implUserData->resPathSaved = resPath;

    if(_ZFP_ZFResExtPathCheck(implUserData->resExtPath, resPath, implUserData->resExtResolved)) {
        ZFPathInfo resPathTmp(implUserData->resExtPath.pathType(), ZFPathInfoToChild(implUserData->resExtPath, resPath));
        if(ZFPathInfoFindFirst(resPathTmp, implUserData->resExtFd)) {
            implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }
        else {
            implUserData->resExtPath.removeAll();
            if(!ZFResRaw::FindFirst(fd, resPath)) {
                fd.implDetach();
                zfdelete(implUserData);
                return zffalse;
            }
            implUserData->resFindFirstStarted = zftrue;
            return zftrue;
        }
    }
    else {
        if(!ZFResRaw::FindFirst(fd, resPath)) {
            fd.implDetach();
            zfdelete(implUserData);
            return zffalse;
        }
        implUserData->resFindFirstStarted = zftrue;
        return zftrue;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType().isEmpty()) {
        while(ZFPathInfoFindNext(implUserData->resExtPath, implUserData->resExtFd)) {
            if(implUserData->resExtItemResolved.find(implUserData->resExtFd.name()) != implUserData->resExtItemResolved.end()) {
                continue;
            }
            implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }
        ZFPathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);

        // try next resExtPath
        while(_ZFP_ZFResExtPathCheck(implUserData->resExtPath, implUserData->resPathSaved, implUserData->resExtResolved)) {
            ZFPathInfo resPathTmp(implUserData->resExtPath.pathType(), ZFPathInfoToChild(implUserData->resExtPath, implUserData->resPathSaved));
            if(ZFPathInfoFindFirst(resPathTmp, implUserData->resExtFd)) {
                do {
                    if(implUserData->resExtItemResolved.find(implUserData->resExtFd.name()) != implUserData->resExtItemResolved.end()) {
                        continue;
                    }
                    implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
                    implUserData->copyToFd(fd.impl());
                    return zftrue;
                } while(ZFPathInfoFindNext(resPathTmp, implUserData->resExtFd));
                ZFPathInfoFindClose(resPathTmp, implUserData->resExtFd);
            }
        }
        implUserData->resExtPath.removeAll();

        // finally fallback to res
        if(ZFResRaw::FindFirst(fd, implUserData->resPathSaved)) {
            implUserData->resFindFirstStarted = zftrue;
            do {
                if(implUserData->resExtItemResolved.find(fd.name()) != implUserData->resExtItemResolved.end()) {
                    continue;
                }
                return zftrue;
            } while(ZFResRaw::FindNext(fd));
        }
        return zffalse;
    }

    while(ZFResRaw::FindNext(fd)) {
        if(implUserData->resExtItemResolved.find(fd.name()) != implUserData->resExtItemResolved.end()) {
            continue;
        }
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType().isEmpty()) {
        ZFPathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);
        implUserData->resExtPath.removeAll();
    }
    if(implUserData->resFindFirstStarted) {
        ZFResRaw::FindClose(fd);
    }
    fd.implDetach();
    zfdelete(implUserData);
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFResOpen
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    if(resPath == zfnull) {
        return zfnull;
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    if(ZFResExtPathCheck(ret->resExtPath, resPath)) {
        ret->resExtPath.pathData(ZFPathInfoToChild(ret->resExtPath, resPath));
        ret->fd = ZFPathInfoOpen(ret->resExtPath, v_ZFFileOpenOption::e_Read);
    }
    else {
        ret->fd = ZFResRaw::Open(resPath);
    }
    if(ret->fd == zfnull) {
        zfdelete(ret);
        ret = zfnull;
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResClose
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    zfblockedDelete(resToken);
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::Close(resToken->fd);
    }
    else {
        return ZFPathInfoClose(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::Tell(resToken->fd);
    }
    else {
        return ZFPathInfoTell(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::Seek(resToken->fd, byteSize, position);
    }
    else {
        return ZFPathInfoSeek(resToken->resExtPath, resToken->fd, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    if(token == zfnull) {
        return 0;
    }
    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::Read(resToken->fd, buf, maxByteSize);
    }
    else {
        if(buf == zfnull) {
            zfindex curPos = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
            if(curPos == zfindexMax()) {
                return zfindexMax();
            }
            if(!ZFPathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd)) {
                return zfindexMax();
            }
            zfindex endPos = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
            ZFPathInfoSeek(resToken->resExtPath, resToken->fd, curPos, ZFSeekPosBegin);
            return endPos - curPos;
        }
        if(maxByteSize == zfindexMax()) {
            return 0;
        }
        return ZFPathInfoRead(resToken->resExtPath, resToken->fd, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsEof
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::IsEof(resToken->fd);
    }
    else {
        return ZFPathInfoIsEof(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsError
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::IsError(resToken->fd);
    }
    else {
        return ZFPathInfoIsError(resToken->resExtPath, resToken->fd);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::Size(resToken->fd);
    }
    else {
        zfindex saved = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
        if(saved == zfindexMax()) {
            return zfindexMax();
        }
        if(!ZFPathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd)) {
            return zfindexMax();
        }
        zfindex size = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
        ZFPathInfoSeek(resToken->resExtPath, resToken->fd, saved, ZFSeekPosBegin);
        return size;
    }
}

ZF_NAMESPACE_GLOBAL_END

