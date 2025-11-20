#include "ZFIO_res.h"
#include "ZFIO_res_raw.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFIOToken_res : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_res, ZFIOToken)
public:
    zfstring _pathData;
    void *_ioToken;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _ioToken = zfnull;
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_res();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return _pathData;
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return v_ZFIOOpenOption::e_Read;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_ioToken) {
            return zffalse;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        _pathData = zfnull;
        void *ioToken = _ioToken;
        _ioToken = zfnull;
        ZFResClose(ioToken);
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {return ZFResRead(_ioToken, buf, maxByteSize);}
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {return 0;}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {return ZFResSeek(_ioToken, byteSize, seekPos);}
    zfoverride
    virtual zfindex ioTell(void) {return ZFResTell(_ioToken);}
    zfoverride
    virtual zfindex ioSize(void) {return ZFResSize(_ioToken);}
};
zfclass _ZFP_I_ZFIOImpl_res : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_res, ZFIOImpl)
public:
    zfoverride
    virtual zfstring pathType(void) {return ZFPathType_res();}

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {return ZFResIsExist(pathData);}
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {return ZFResIsDir(pathData);}
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return ioToFileNameDefault(ret, pathData);}
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {return ioToChildDefault(ret, pathData, childName);}
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return ioToParentDefault(ret, pathData);}
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataTo
            , ZF_IN const zfstring &pathDataFrom
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {return ZFResFindFirst(fd, pathData);}
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {return ZFResFindNext(fd);}
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {ZFResFindClose(fd);}
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        void *token = ZFResOpen(pathData);
        if(token == zfnull) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_res> ioToken;
        ioToken->_pathData = pathData;
        ioToken->_ioToken = token;
        return ioToken;
    }
};
ZFIO_DEFINE(res, _ZFP_I_ZFIOImpl_res)

// ============================================================
// ZFInputForRes
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForRes
        , ZFMP_IN(const zfstring &, resFilePath)
        )

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes {
public:
    void *fd; // fd returned by ZFResOpen
    zfautoT<ZFIOImpl> resExtImpl; // valid if opened by ZFIOOpen
    zfautoT<ZFIOToken> resExtToken; // valid if opened by ZFIOOpen

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(zfnull)
    , resExtImpl()
    , resExtToken()
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

zfclassNotPOD _ZFP_ZFResExtKeyHash {
public:
    inline zfstlsize operator () (ZFPathInfo const &v) const {
        return (zfstlsize)zfidentityHash(
                zfidentityCalcString(v.pathType(), v.pathType().length())
                , zfidentityCalcString(v.pathData(), v.pathData().length())
                );
    }
};
typedef zfstlhashmap<ZFPathInfo, zfbool, _ZFP_ZFResExtKeyHash> _ZFP_ZFResExtMap;
typedef zfstlhashmap<zfstring, _ZFP_ZFResExtMap> _ZFP_ZFResExtCkMap;
static zfbool _ZFP_ZFResExtPathCheck(
        ZF_OUT ZFPathInfo &resExtPath
        , ZF_IN const zfstring &resPath
        , ZF_IN_OUT _ZFP_ZFResExtCkMap &ck
        ) {
    ZFCoreMutexLock();

    _ZFP_ZFResExtMap &ckTmp = ck[resPath];
    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i) {
        const ZFPathInfo &t = l[i];
        if(t.pathType() == ZFPathType_res() && (zffalse
                    || (t.pathData().length() == resPath.length() && t.pathData() == resPath)
                    || (t.pathData().length() < resPath.length() && resPath[t.pathData().length()] == '/' && zfstringBeginWith(resPath, t.pathData()))
                    )) {
            continue;
        }
        if(ckTmp.find(t) != ckTmp.end()) {
            continue;
        }
        ckTmp[t] = zftrue;
        ZFCoreMutexUnlock();

        zfautoT<ZFIOImpl> impl = ZFIOImplForPathType(t.pathType());
        if(impl) {
            zfstring toCheck;
            impl->ioToChild(toCheck, t.pathData(), resPath);
            if(impl->ioIsExist(toCheck)) {
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
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResExtPathCheck
        , ZFMP_OUT(ZFPathInfo &, resExtPath)
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    ZFCoreMutexLocker();
    static _ZFP_ZFResExtCkMap ck;
    static zfuint ckCount = 0;
    ++ckCount;
    zfbool result = _ZFP_ZFResExtPathCheck(resExtPath, resPath, ck);
    --ckCount;
    if(ckCount == 0) {
        ck.clear();
    }
    return result;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist
        , ZFMP_IN(const zfstring &, resPath)
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
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(resExtPath.pathType());
        if(ioImpl) {
            zfstring tmp;
            return ioImpl->ioToChild(tmp, resExtPath.pathData(), resPath)
                && ioImpl->ioIsDir(tmp);
        }
    }
    return ZFResRaw::ZFResIsDir(resPath);
}

#define _ZFP_ZFIOFindType_res "ZFResFindFirst"

zfclassNotPOD _ZFP_ZFResFindData {
public:
    zfstring resPathSaved;
    _ZFP_ZFResExtCkMap resExtResolved;
    zfstlhashmap<zfstring, zfbool> resExtItemResolved;
    ZFIOFindData resExtFd; // valid if resExtImpl not empty
    /*
     * if not empty, the file is find from resExtPath
     * ensured end with file separator
     * ensured clear when find task ends from resExtPath
     */
    zfautoT<ZFIOImpl> resExtImpl;
    ZFPathInfo resExtPath;
    zfbool resFindFirstStarted;
public:
    _ZFP_ZFResFindData(void)
    : resPathSaved()
    , resExtResolved()
    , resExtItemResolved()
    , resExtFd()
    , resExtImpl()
    , resExtPath()
    , resFindFirstStarted(zffalse)
    {
    }
};

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    _ZFP_ZFResFindData *implUserData = zfnew(_ZFP_ZFResFindData);
    fd.implAttach(_ZFP_ZFIOFindType_res, implUserData);
    implUserData->resPathSaved = resPath;

    if(_ZFP_ZFResExtPathCheck(implUserData->resExtPath, resPath, implUserData->resExtResolved)
            && (implUserData->resExtImpl = ZFIOImplForPathType(implUserData->resExtPath.pathType())) != zfnull
            ) {
        zfstring resPathTmp;
        implUserData->resExtImpl->ioToChild(resPathTmp, implUserData->resExtPath.pathData(), resPath);
        if(implUserData->resExtImpl->ioFindFirst(implUserData->resExtFd, resPathTmp)) {
            implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
            fd.implCopy(implUserData->resExtFd);
            return zftrue;
        }
        else {
            implUserData->resExtPath = zfnull;
            implUserData->resExtImpl = zfnull;
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
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFIOFindType_res);

    if(implUserData->resExtImpl != zfnull) {
        while(implUserData->resExtImpl->ioFindNext(implUserData->resExtFd)) {
            if(implUserData->resExtItemResolved.find(implUserData->resExtFd.name()) != implUserData->resExtItemResolved.end()) {
                continue;
            }
            implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
            fd.implCopy(implUserData->resExtFd);
            return zftrue;
        }
        implUserData->resExtImpl->ioFindClose(implUserData->resExtFd);
        implUserData->resExtPath = zfnull;
        implUserData->resExtImpl = zfnull;

        // try next resExtPath
        while(_ZFP_ZFResExtPathCheck(implUserData->resExtPath, implUserData->resPathSaved, implUserData->resExtResolved)
                && (implUserData->resExtImpl = ZFIOImplForPathType(implUserData->resExtPath.pathType())) != zfnull
                ) {
            zfstring resPathDataTmp;
            implUserData->resExtImpl->ioToChild(resPathDataTmp, implUserData->resExtPath.pathData(), implUserData->resPathSaved);
            if(implUserData->resExtImpl->ioFindFirst(implUserData->resExtFd, resPathDataTmp)) {
                do {
                    if(implUserData->resExtItemResolved.find(implUserData->resExtFd.name()) != implUserData->resExtItemResolved.end()) {
                        continue;
                    }
                    implUserData->resExtItemResolved[implUserData->resExtFd.name()] = zftrue;
                    fd.implCopy(implUserData->resExtFd);
                    return zftrue;
                } while(implUserData->resExtImpl->ioFindNext(implUserData->resExtFd));
                implUserData->resExtImpl->ioFindClose(implUserData->resExtFd);
            }
        }
        implUserData->resExtPath = zfnull;
        implUserData->resExtImpl = zfnull;

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
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFIOFindType_res);

    if(implUserData->resExtImpl != zfnull) {
        implUserData->resExtImpl->ioFindClose(implUserData->resExtFd);
        implUserData->resExtPath = zfnull;
        implUserData->resExtImpl = zfnull;
    }
    if(implUserData->resFindFirstStarted) {
        ZFResRaw::ZFResFindClose(fd);
    }
    fd.implDetach();
    zfdelete(implUserData);
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFResOpen
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    if(resPath == zfnull) {
        return zfnull;
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    ZFPathInfo resExtPath;
    if(ZFResExtPathCheck(resExtPath, resPath)) {
        ret->resExtImpl = ZFIOImplForPathType(resExtPath.pathType());
        if(ret->resExtImpl) {
            zfstring tmp;
            if(ret->resExtImpl->ioToChild(tmp, resExtPath.pathData(), resPath)) {
                ret->resExtToken = ret->resExtImpl->ioOpen(tmp, v_ZFIOOpenOption::e_Read);
            }
        }
        if(ret->resExtToken == zfnull) {
            zfdelete(ret);
            ret = zfnull;
        }
    }
    else {
        ret->fd = ZFResRaw::ZFResOpen(resPath);
        if(ret->fd == zfnull) {
            zfdelete(ret);
            ret = zfnull;
        }
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
    zfscopeDelete(resToken);
    if(resToken->resExtToken != zfnull) {
        zfbool ret = resToken->resExtToken->ioClose();;
        resToken->resExtToken = zfnull;
        resToken->resExtImpl = zfnull;
        return ret;
    }
    else {
        zfbool ret = ZFResRaw::ZFResClose(resToken->fd);
        resToken->fd = zfnull;
        return ret;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtToken != zfnull) {
        return resToken->resExtToken->ioTell();
    }
    else {
        return ZFResRaw::ZFResTell(resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        ) {
    if(token == zfnull) {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtToken != zfnull) {
        return resToken->resExtToken->ioSeek(byteSize, seekPos);
    }
    else {
        return ZFResRaw::ZFResSeek(resToken->fd, byteSize, seekPos);
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
    if(resToken->resExtToken != zfnull) {
        return resToken->resExtToken->ioRead(buf, maxByteSize);
    }
    else {
        return ZFResRaw::ZFResRead(resToken->fd, buf, maxByteSize);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtToken != zfnull) {
        return resToken->resExtToken->ioSize();
    }
    else {
        return ZFResRaw::ZFResSize(resToken->fd);
    }
}

ZF_NAMESPACE_GLOBAL_END

