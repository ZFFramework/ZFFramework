#include "ZFIO_res_raw.h"

#include "protocol/ZFProtocolZFRes.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFIOToken_resraw : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_resraw, ZFIOToken)
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
        return ZFPathType_resraw();
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
        _pathData = zfnull;
        if(_ioToken) {
            void *ioToken = _ioToken;
            _ioToken = zfnull;
            return ZFResRaw::ZFResClose(ioToken);
        }
        else {
            return zffalse;
        }
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {return ZFResRaw::ZFResRead(_ioToken, buf, maxByteSize);}
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {return zfindexMax();}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {return ZFResRaw::ZFResSeek(_ioToken, byteSize, seekPos);}
    zfoverride
    virtual zfindex ioTell(void) {return ZFResRaw::ZFResTell(_ioToken);}
    zfoverride
    virtual zfindex ioSize(void) {return ZFResRaw::ZFResSize(_ioToken);}
};
zfclass _ZFP_I_ZFIOImpl_resraw : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_resraw, ZFIOImpl)
public:
    zfoverride
    virtual zfstring pathType(void) {return ZFPathType_resraw();}

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {return ZFResRaw::ZFResIsExist(pathData);}
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {return ZFResRaw::ZFResIsDir(pathData);}
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
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {return ZFResRaw::ZFResFindFirst(fd, pathData);}
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {return ZFResRaw::ZFResFindNext(fd);}
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {ZFResRaw::ZFResFindClose(fd);}
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        void *token = ZFResRaw::ZFResOpen(pathData);
        if(token == zfnull) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_resraw> ioToken;
        ioToken->_pathData = pathData;
        ioToken->_ioToken = token;
        return ioToken;
    }
};
ZFIO_DEFINE(resraw, _ZFP_I_ZFIOImpl_resraw)

// ============================================================
// ZFInputForResRaw
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForResRaw
        , ZFMP_IN(const zfstring &, resFilePath)
        )

ZF_NAMESPACE_BEGIN(ZFResRaw)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsExist(resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsDir
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsDir(resPath);
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resFindFirst(fd.impl(), resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    ZFPROTOCOL_ACCESS(ZFRes)->resFindClose(fd.impl());
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFResOpen
        , ZFMP_IN(const zfstring &, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resOpen(resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResClose
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resClose(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resTell(token);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resSeek(token, byteSize, seekPos);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    if(token == zfnull) {
        return 0;
    }
    if(buf == zfnull) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
        zfindex curPos = impl->resTell(token);
        impl->resSeek(token, 0, ZFSeekPosEnd);
        zfindex endPos = impl->resTell(token);
        impl->resSeek(token, curPos, ZFSeekPosBegin);
        return endPos - curPos;
    }
    if(maxByteSize == zfindexMax()) {
        return 0;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resRead(token, buf, maxByteSize);
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
    zfindex saved = impl->resTell(token);
    impl->resSeek(token, 0, ZFSeekPosEnd);
    zfindex size = impl->resTell(token);
    impl->resSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_END(ZFResRaw)
ZF_NAMESPACE_GLOBAL_END

