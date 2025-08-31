#include "ZFIO_encrypt.h"
#include "ZFMd5.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFIOToken_encrypt : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_encrypt, ZFIOToken)
public:
    zfbool open(
            ZF_IN const ZFPathInfo &refPathInfo
            , ZF_IN const zfstring &encryptKey
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        this->ioClose();

        _refIOToken = ZFIOOpen(refPathInfo, flags, autoCreateParent);
        if(_refIOToken == zfnull) {
            return zffalse;
        }
        _encryptKey = encryptKey;
        _refPathInfo = refPathInfo;
        _refOpenFlags = flags;
        _buf = zfobj<ZFIOBuffer>();

        if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
            if(!ZFDecrypt(_buf->output(), ZFInputForPathInfoToken(_refIOToken), encryptKey)) {
                _refIOToken = zfnull;
                _encryptKey = zfnull;
                _refPathInfo = zfnull;
                return zffalse;
            }
            _buf->input().ioSeek(0);
        }
        else if(!ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(!ZFDecrypt(_buf->output(), ZFInputForPathInfoToken(_refIOToken), encryptKey)) {
                _refIOToken = zfnull;
                _encryptKey = zfnull;
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
    zfstring _encryptKey;
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
        return ZFPathInfo(ZFPathType_cachePath(), ZFMd5(zfstr("ZFIO_encrypt/%s/%s", _refPathInfo, (const void *)this)));
    }

public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_encrypt();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return ZFPathInfoChainEncode(_refPathInfo, _encryptKey);
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return _refOpenFlags;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_refIOToken) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        zfbool ret = zftrue;
        if(_refModified) {
            _buf->input().ioSeek(0);
            ret = ZFEncrypt(
                    ZFOutputForPathInfoToken(_refIOToken)
                    , _buf->input()
                    , _encryptKey
                    );
            _refModified = zffalse;
        }
        _refIOToken = zfnull;
        _buf = zfnull;
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return ret;
    }
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
zfclass _ZFP_I_ZFIOImpl_encrypt : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_encrypt, ZFIOImpl)
private:
    zfautoT<ZFIOImpl> _refIOImpl(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return ZFIOImplForPathType(refPathInfo.pathType());
        }
        else {
            return zfnull;
        }
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_encrypt();
    }

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {
        return ZFIOImpl::ioIsExistForChained(pathData);
    }
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {
        return ZFIOImpl::ioIsDirForChained(pathData);
    }
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        return ZFIOImpl::ioToFileNameForChained(ret, pathData);
    }
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {
        return ZFIOImpl::ioToChildForChained(ret, pathData, childName);
    }
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        return ZFIOImpl::ioToParentForChained(ret, pathData);
    }
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        return ZFIOImpl::ioPathCreateForChained(pathData, autoCreateParent);
    }
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return ZFIOImpl::ioRemoveForChained(pathData, isRecursive, isForce);
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return ZFIOImpl::ioMoveForChained(pathDataFrom, pathDataTo, isForce);
    }
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {
        return ZFIOImpl::ioFindFirstForChained(fd, pathData);
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        return ZFIOImpl::ioFindNextForChained(fd);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        return ZFIOImpl::ioFindCloseForChained(fd);
    }
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_encrypt> ioToken;
        if(!ioToken->open(refPathInfo, selfPathData, flags, autoCreateParent)) {
            return zfnull;
        }
        return ioToken;
    }
};
ZFIO_DEFINE(encrypt, _ZFP_I_ZFIOImpl_encrypt)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfstring &, encryptKey)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(
            ret
            , ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(refPathInfo, encryptKey))
            );
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfstring &, encryptKey)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(
            ret
            , ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(refPathInfo, encryptKey))
            );
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForEncrypt
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN(const zfstring &, encryptKey)
        ) {
    if(!refInput) {
        return zfnull;
    }
    ZFLISTENER_1(conv
            , zfstring, encryptKey
            ) {
        ZFIOBuffer *buf = zfargs.sender();
        ZFInput refInput = zfargs.param0().to<v_ZFInput *>()->zfv;
        ZFDecrypt(buf->output(), refInput, encryptKey);
        buf->input().ioSeek(0);
    } ZFLISTENER_END()
    ZFInput ret = ZFIOBufferInput(refInput, conv);
    if(refInput.pathInfo()) {
        ZFInputMarkSerializable(ret, ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(refInput.pathInfo(), encryptKey)));
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForEncrypt
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN(const zfstring &, encryptKey)
        ) {
    if(!refOutput) {
        return zfnull;
    }
    ZFLISTENER_1(conv
            , zfstring, encryptKey
            ) {
        ZFIOBuffer *buf = zfargs.sender();
        buf->input().ioSeek(0);
        ZFOutput refOutput = zfargs.param0().to<v_ZFOutput *>()->zfv;
        ZFEncrypt(refOutput, buf->input(), encryptKey);
    } ZFLISTENER_END()
    ZFOutput ret = ZFIOBufferOutput(refOutput, conv);
    if(refOutput.pathInfo()) {
        ZFOutputMarkSerializable(ret, ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(refOutput.pathInfo(), encryptKey)));
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

