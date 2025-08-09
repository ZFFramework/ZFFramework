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
            _buf->output().ioSeek(0);
        }
        else if(!ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(!ZFDecrypt(_buf->output(), ZFInputForPathInfoToken(_refIOToken), encryptKey)) {
                _refIOToken = zfnull;
                _encryptKey = zfnull;
                _refPathInfo = zfnull;
                return zffalse;
            }
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
        zfbool ret = zftrue;
        if(_refIOToken) {
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
        }
        _buf = zfnull;
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
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
            && ZFIOIsExist(refPathInfo);
    }
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
            && ZFIOIsDir(refPathInfo);
    }
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
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
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
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
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
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
        zfstring selfPathData;
        return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
            && ZFIOPathCreate(refPathInfo, autoCreateParent);
    }
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        return ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)
            && ZFIORemove(refPathInfo, isRecursive, isForce);
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfoFrom;
        zfstring selfPathDataFrom;
        ZFPathInfo refPathInfoTo;
        zfstring selfPathDataTo;
        return ZFPathInfoChainDecode(refPathInfoFrom, selfPathDataFrom, pathDataFrom)
            && ZFPathInfoChainDecode(refPathInfoTo, selfPathDataTo, pathDataTo)
            && refPathInfoFrom.pathType() == refPathInfoTo.pathType()
            && ZFIOMove(refPathInfoFrom, refPathInfoTo.pathData(), isForce);
    }
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        zfautoT<ZFIOImpl> refIOImpl = ZFIOImplForPathType(refPathInfo.pathType());
        if(refIOImpl == zfnull || !refIOImpl->ioFindFirst(fd, refPathInfo.pathData())) {
            return zffalse;
        }
        fd.implTag("ZFIO_encrypt_fd", refIOImpl);
        return zftrue;
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIO_encrypt_fd");
        return refIOImpl && refIOImpl->ioFindNext(fd);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFIOImpl> refIOImpl = fd.implTag("ZFIO_encrypt_fd");
        if(refIOImpl) {
            refIOImpl->ioFindClose(fd);
            fd.implTag("ZFIO_encrypt_fd", zfnull);
        }
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

