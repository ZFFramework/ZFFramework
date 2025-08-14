#include "ZFIO_base64.h"
#include "ZFMd5.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFIOToken_base64 : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_base64, ZFIOToken)
public:
    zfbool open(
            ZF_IN const ZFPathInfo &refPathInfo
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        this->ioClose();

        _refIOToken = ZFIOOpen(refPathInfo, flags, autoCreateParent);
        if(_refIOToken == zfnull) {
            return zffalse;
        }
        _refPathInfo = refPathInfo;
        _refOpenFlags = flags;
        _buf = zfobj<ZFIOBuffer>();

        if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
            if(!ZFBase64Decode(_buf->output(), ZFInputForPathInfoToken(_refIOToken))) {
                _refIOToken = zfnull;
                _refPathInfo = zfnull;
                return zffalse;
            }
            _buf->input().ioSeek(0);
        }
        else if(!ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(!ZFBase64Decode(_buf->output(), ZFInputForPathInfoToken(_refIOToken))) {
                _refIOToken = zfnull;
                _refPathInfo = zfnull;
                return zffalse;
            }
            _buf->input().ioSeek(0);
        }

        return zftrue;
    }

private:
    zfautoT<ZFIOToken> _refIOToken;
    ZFPathInfo _refPathInfo;
    ZFIOOpenOptionFlags _refOpenFlags;
    zfbool _refModified;
    zfautoT<ZFIOBuffer> _buf;

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _refModified = zffalse;
    }

private:
    ZFPathInfo _cachePath(void) {
        return ZFPathInfo(ZFPathType_cachePath(), ZFMd5(zfstr("ZFIO_base64/%s/%s", _refPathInfo, (const void *)this)));
    }

public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_base64();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return ZFPathInfoToString(_refPathInfo);
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return _refOpenFlags;
    }
protected:
    zfoverride
    virtual zfbool ioCloseImpl(void) {
        zfbool ret = zftrue;
        if(_refIOToken) {
            if(_refModified) {
                _buf->input().ioSeek(0);
                ret = ZFBase64Encode(
                        ZFOutputForPathInfoToken(_refIOToken)
                        , _buf->input()
                        , zfnull
                        , ZFBase64Table()
                        , ZFBase64Pad()
                        );
                _refModified = zffalse;
            }
            _refIOToken = zfnull;
        }
        _buf = zfnull;
        return ret;
    }
public:
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        return _buf->input().execute(buf, maxByteSize);
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {
        if(!ZFBitTest(_refOpenFlags, 0
                    | v_ZFIOOpenOption::e_Write
                    | v_ZFIOOpenOption::e_Modify
                    )) {
            return 0;
        }
        _refModified = zftrue;
        return _buf->output().execute(src, maxByteSize);
    }
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        return _buf->input().ioSeek(byteSize, seekPos);
    }
    zfoverride
    virtual zfindex ioTell(void) {
        return _buf->input().ioTell();
    }
    zfoverride
    virtual zfindex ioSize(void) {
        return _buf->input().ioSize();
    }
};
zfclass _ZFP_I_ZFIOImpl_base64 : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_base64, ZFIOImpl)
private:
    zfautoT<ZFIOImpl> _refIOImpl(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        if(ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return ZFIOImplForPathType(refPathInfo.pathType());
        }
        else {
            return zfnull;
        }
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_base64();
    }

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        return ZFPathInfoFromStringT(refPathInfo, pathData)
            && ZFIOIsExist(refPathInfo);
    }
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        return ZFPathInfoFromStringT(refPathInfo, pathData)
            && ZFIOIsDir(refPathInfo);
    }
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        ZFPathInfo refPathInfo;
        if(!ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return zffalse;
        }
        if(&ret == &pathData) {
            zfstring tmp;
            if(!ZFIOToFileName(tmp, refPathInfo)) {
                return zffalse;
            }
            ret = tmp;
            return zftrue;
        }
        else {
            return ZFIOToFileName(ret, refPathInfo);
        }
    }
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {
        ZFPathInfo refPathInfo;
        if(!ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return zffalse;
        }
        if(&ret == &pathData) {
            zfstring tmp;
            if(!ZFIOToChild(tmp, refPathInfo, childName)) {
                return zffalse;
            }
            ret = tmp;
            return zftrue;
        }
        else {
            return ZFIOToChild(ret, refPathInfo, childName);
        }
    }
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        ZFPathInfo refPathInfo;
        if(!ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return zffalse;
        }
        if(&ret == &pathData) {
            zfstring tmp;
            if(!ZFIOToParent(tmp, refPathInfo)) {
                return zffalse;
            }
            ret = tmp;
            return zftrue;
        }
        else {
            return ZFIOToParent(ret, refPathInfo);
        }
    }
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        ZFPathInfo refPathInfo;
        return ZFPathInfoFromStringT(refPathInfo, pathData)
            && ZFIOPathCreate(refPathInfo, autoCreateParent);
    }
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfo;
        return ZFPathInfoFromStringT(refPathInfo, pathData)
            && ZFIORemove(refPathInfo, isRecursive, isForce);
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfoFrom;
        ZFPathInfo refPathInfoTo;
        return ZFPathInfoFromStringT(refPathInfoFrom, pathDataFrom)
            && ZFPathInfoFromStringT(refPathInfoTo, pathDataTo)
            && refPathInfoFrom.pathType() == refPathInfoTo.pathType()
            && ZFIOMove(refPathInfoFrom, refPathInfoTo.pathData(), isForce);
    }
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {
        ZFPathInfo refPathInfo;
        if(!ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return zffalse;
        }
        zfautoT<ZFIOImpl> refIOImpl = ZFIOImplForPathType(refPathInfo.pathType());
        if(refIOImpl == zfnull || !refIOImpl->ioFindFirst(fd, refPathInfo.pathData())) {
            return zffalse;
        }
        fd.implTag("ZFIO_base64_fd", refIOImpl);
        return zftrue;
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIO_base64_fd");
        return refIOImpl && refIOImpl->ioFindNext(fd);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIO_base64_fd");
        if(refIOImpl) {
            refIOImpl->ioFindClose(fd);
            fd.implTag("ZFIO_base64_fd", zfnull);
        }
    }
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        ZFPathInfo refPathInfo;
        if(!ZFPathInfoFromStringT(refPathInfo, pathData)) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_base64> ioToken;
        if(!ioToken->open(refPathInfo, flags, autoCreateParent)) {
            return zfnull;
        }
        return ioToken;
    }
};
ZFIO_DEFINE(base64, _ZFP_I_ZFIOImpl_base64)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFInput, ZFInputForBase64
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(
            ret
            , ZFPathInfo(ZFPathType_base64(), ZFPathInfoToString(refPathInfo))
            );
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForBase64
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(
            ret
            , ZFPathInfo(ZFPathType_base64(), ZFPathInfoToString(refPathInfo))
            );
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFInput, ZFInputForBase64
        , ZFMP_IN(const ZFInput &, refInput)
        ) {
    if(!refInput) {
        return zfnull;
    }
    ZFLISTENER(conv
            ) {
        ZFIOBuffer *buf = zfargs.sender();
        ZFInput refInput = zfargs.param0().to<v_ZFInput *>()->zfv;
        ZFBase64Decode(buf->output(), refInput);
        buf->input().ioSeek(0);
    } ZFLISTENER_END()
    ZFInput ret = ZFIOBufferInput(refInput, conv);
    if(refInput.pathInfo()) {
        ZFInputMarkSerializable(ret, ZFPathInfo(ZFPathType_base64(), ZFPathInfoToString(refInput.pathInfo())));
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForBase64
        , ZFMP_IN(const ZFOutput &, refOutput)
        ) {
    if(!refOutput) {
        return zfnull;
    }
    ZFLISTENER(conv
            ) {
        ZFIOBuffer *buf = zfargs.sender();
        buf->input().ioSeek(0);
        ZFOutput refOutput = zfargs.param0().to<v_ZFOutput *>()->zfv;
        ZFBase64Encode(refOutput, buf->input());
    } ZFLISTENER_END()
    ZFOutput ret = ZFIOBufferOutput(refOutput, conv);
    if(refOutput.pathInfo()) {
        ZFOutputMarkSerializable(ret, ZFPathInfo(ZFPathType_base64(), ZFPathInfoToString(refOutput.pathInfo())));
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

