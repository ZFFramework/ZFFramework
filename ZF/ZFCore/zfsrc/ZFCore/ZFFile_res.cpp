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

    // store pathData of `res:xxx`, which would cause recursive search
    // any access to this path would result to fail for safety
    zfstlhashmap<zfstring, zfuint> recursiveMap;
ZF_GLOBAL_INITIALIZER_END(ZFResExtPathDataHolder)
#define _ZFP_ZFResExtPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResExtPathDataHolder)->resExtPathList)
#define _ZFP_ZFResExtRecursiveMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResExtPathDataHolder)->recursiveMap)

ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathAdd
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    if(pathInfo.pathType().isEmpty()) {
        return;
    }
    ZFCoreMutexLocker();
    if(zfstringIsEqual(pathInfo.pathType(), ZFPathType_res())) {
        ++(_ZFP_ZFResExtRecursiveMap[pathInfo.pathData()]);
    }
    _ZFP_ZFResExtPathList.add(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathRemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    if(pathInfo.pathType().isEmpty()) {
        return;
    }
    ZFCoreMutexLocker();
    if(zfstringIsEqual(pathInfo.pathType(), ZFPathType_res())) {
        zfstlhashmap<zfstring, zfuint> &m = _ZFP_ZFResExtRecursiveMap;
        zfstlhashmap<zfstring, zfuint>::iterator it = m.find(pathInfo.pathData());
        if(it != m.end()) {
            if(it->second == 1) {
                m.erase(it);
            }
            else {
                --(it->second);
            }
        }
    }
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
    zfstlhashmap<zfstring, zfuint> &m = _ZFP_ZFResExtRecursiveMap;
    zfindex resPathLen = zfslen(resPath);
    for(zfstlhashmap<zfstring, zfuint>::iterator it = m.begin(); it != m.end(); ++it) {
        if(zfstringBeginWith(resPath, resPathLen, it->first.cString(), it->first.length())
                && resPath[it->first.length()] == '/'
                && zfstringBeginWith(resPath + it->first.length() + 1, resPathLen - (it->first.length() + 1), it->first.cString(), it->first.length())
                ) {
            ZFCoreMutexUnlock();
            return zffalse;
        }
    }

    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i) {
        const ZFPathInfo &t = l[i];
        ZFCoreMutexUnlock();

        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(t.pathType());
        if(impl) {
            zfstring toCheck;
            impl->implToChild(toCheck, t.pathData(), resPath);
            if(impl->implIsExist(toCheck)) {
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
        , ZF_IN_OUT _ZFP_ZFResExtMap &ck
        ) {
    ZFCoreMutexLock();
    zfstlhashmap<zfstring, zfuint> &m = _ZFP_ZFResExtRecursiveMap;
    zfindex resPathLen = zfslen(resPath);
    for(zfstlhashmap<zfstring, zfuint>::iterator it = m.begin(); it != m.end(); ++it) {
        if(zfstringBeginWith(resPath, resPathLen, it->first.cString(), it->first.length())
                && resPath[it->first.length()] == '/'
                && zfstringBeginWith(resPath + it->first.length() + 1, resPathLen - (it->first.length() + 1), it->first.cString(), it->first.length())
                ) {
            ZFCoreMutexUnlock();
            return zffalse;
        }
    }

    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i) {
        const ZFPathInfo &t = l[i];
        if(ck.find(t) != ck.end()) {
            continue;
        }
        ck[t] = zftrue;
        ZFCoreMutexUnlock();

        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(t.pathType());
        if(impl) {
            zfstring toCheck;
            impl->implToChild(toCheck, t.pathData(), resPath);
            if(impl->implIsExist(toCheck)) {
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

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        return zftrue;
    }
    else {
        return ZFResRaw::ZFResIsExist(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsDir
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        zfstring tmp;
        ZFPathInfoToChild(tmp, resExtPath, resPath);
        resExtPath.pathData(tmp);
        return ZFPathInfoIsDir(resExtPath);
    }
    else {
        return ZFResRaw::ZFResIsDir(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFResCopy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        zfbool ret = ZFPathInfoCopy(
            ZFPathInfo(ZFPathType_res(), resPath)
            , ZFPathInfo(ZFPathType_file(), dstPath)
            , isRecursive
            , isForce
            );
        return ret;
    }
    else {
        return ZFResRaw::ZFResCopy(resPath, dstPath, isRecursive, isForce);
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
        zfstring resPathTmp;
        ZFPathInfoToChild(resPathTmp, implUserData->resExtPath, resPath);
        if(ZFPathInfoFindFirst(ZFPathInfo(implUserData->resExtPath.pathType(), resPathTmp), implUserData->resExtFd)) {
            implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
            fd.implCopy(implUserData->resExtFd);
            return zftrue;
        }
        else {
            implUserData->resExtPath.removeAll();
            if(!ZFResRaw::ZFResFindFirst(fd, resPath)) {
                fd.implDetach();
                zfdelete(implUserData);
                return zffalse;
            }
            implUserData->resFindFirstStarted = zftrue;
            return zftrue;
        }
    }
    else {
        if(!ZFResRaw::ZFResFindFirst(fd, resPath)) {
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
            fd.implCopy(implUserData->resExtFd);
            return zftrue;
        }
        ZFPathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);

        // try next resExtPath
        while(_ZFP_ZFResExtPathCheck(implUserData->resExtPath, implUserData->resPathSaved, implUserData->resExtResolved)) {
            zfstring resPathDataTmp;
            ZFPathInfoToChild(resPathDataTmp, implUserData->resExtPath, implUserData->resPathSaved);
            ZFPathInfo resPathTmp(implUserData->resExtPath.pathType(), resPathDataTmp);
            if(ZFPathInfoFindFirst(resPathTmp, implUserData->resExtFd)) {
                do {
                    if(implUserData->resExtItemResolved.find(implUserData->resExtFd.name()) != implUserData->resExtItemResolved.end()) {
                        continue;
                    }
                    implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
                    fd.implCopy(implUserData->resExtFd);
                    return zftrue;
                } while(ZFPathInfoFindNext(resPathTmp, implUserData->resExtFd));
                ZFPathInfoFindClose(resPathTmp, implUserData->resExtFd);
            }
        }
        implUserData->resExtPath.removeAll();

        // finally fallback to res
        if(ZFResRaw::ZFResFindFirst(fd, implUserData->resPathSaved)) {
            implUserData->resFindFirstStarted = zftrue;
            do {
                if(implUserData->resExtItemResolved.find(fd.name()) != implUserData->resExtItemResolved.end()) {
                    continue;
                }
                return zftrue;
            } while(ZFResRaw::ZFResFindNext(fd));
        }
        return zffalse;
    }

    while(ZFResRaw::ZFResFindNext(fd)) {
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
        ZFResRaw::ZFResFindClose(fd);
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
        zfstring tmp;
        ZFPathInfoToChild(tmp, ret->resExtPath, resPath);
        ret->resExtPath.pathData(tmp);
        ret->fd = ZFPathInfoOpen(ret->resExtPath, v_ZFFileOpenOption::e_Read);
    }
    else {
        ret->fd = ZFResRaw::ZFResOpen(resPath);
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
        return ZFResRaw::ZFResClose(resToken->fd);
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
        return ZFResRaw::ZFResTell(resToken->fd);
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
        return ZFResRaw::ZFResSeek(resToken->fd, byteSize, position);
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
        return ZFResRaw::ZFResRead(resToken->fd, buf, maxByteSize);
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

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType().isEmpty()) {
        return ZFResRaw::ZFResSize(resToken->fd);
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

