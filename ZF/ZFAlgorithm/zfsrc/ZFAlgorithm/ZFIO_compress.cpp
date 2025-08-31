#include "ZFIO_compress.h"
#include "ZFMd5.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCompressTokenCache, ZFLevelZFFrameworkLow) {
}
public:
    zfautoT<ZFCompressToken> obtain(
            ZF_IN const ZFPathInfo &refPathInfo
            , ZF_IN_OPT ZFIOOpenOptionFlags flags = v_ZFIOOpenOption::e_Read
            ) {
        if(flags != v_ZFIOOpenOption::e_Read) {
            return ZFCompressOpen(refPathInfo, flags);
        }
        else {
            return _accessReadonly(refPathInfo);
        }
    }
    void remove(ZF_IN const ZFPathInfo &refPathInfo) {
        ZFCoreMutexLocker();
        for(zfindex i = 0; i < _cache.count(); ++i) {
            if(refPathInfo == _cache[i]->refPathInfo) {
                _cache.remove(i);
            }
        }
    }
private:
    zfclassNotPOD CacheData {
    public:
        ZFPathInfo refPathInfo;
        zfautoT<ZFCompressToken> compressToken;
        zftimet lastAccessed;
    };
    ZFCoreArray<ZFCorePointerForObject<CacheData *> > _cache; // latest accessed at tail
private:
    zfautoT<ZFCompressToken> _accessReadonly(ZF_IN const ZFPathInfo &refPathInfo) {
        {
            ZFCoreMutexLocker();
            for(zfindex i = _cache.count() - 1; i != zfindexMax(); --i) {
                if(_cache[i]->refPathInfo == refPathInfo) {
                    ZFCorePointerForObject<CacheData *> cache = _cache[i];
                    cache->lastAccessed = ZFTime::currentTime();
                    if(i == _cache.count() - 1) {
                        _cacheTrim(cache->lastAccessed);
                        return cache->compressToken;
                    }
                    else {
                        _cache.remove(i);
                        _cache.add(cache);
                        _cacheTrim(cache->lastAccessed);
                        return cache->compressToken;
                    }
                }
            }
        }
        zfautoT<ZFCompressToken> ret = ZFCompressOpen(refPathInfo, v_ZFIOOpenOption::e_Read);
        if(!ret) {
            return zfnull;
        }
        ZFCoreMutexLocker();
        CacheData *t = zfnew(CacheData);
        t->refPathInfo = refPathInfo;
        t->compressToken = ret;
        t->lastAccessed = ZFTime::currentTime();
        _cache.add(ZFCorePointerForObject<CacheData *>(t));
        ZFLISTENER(onClose
                ) {
            zfautoT<ZFCompressToken> token = zfargs.sender();
            if(token) {
                ZFCoreMutexLocker();
                ZF_GLOBAL_INITIALIZER_CLASS(ZFCompressTokenCache) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache);
                for(zfindex i = 0; i < d->_cache.count(); ++i) {
                    if(token == d->_cache[i]->compressToken) {
                        d->_cache.remove(i);
                        break;
                    }
                }
            }
        } ZFLISTENER_END()
        ret->observerAddForOnce(ZFIOToken::E_IOCloseOnPrepare(), onClose);
        _cacheTrim(t->lastAccessed);
        return ret;
    }
    void _cacheTrim(ZF_IN zftimet curTime) {
        while(_cache.count() > 8
                || (!_cache.isEmpty()
                    && curTime - _cache[0]->lastAccessed >= 5 * 60 * 1000
                    )
                ) {
            _cache.remove(0);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFCompressTokenCache)

// ============================================================
zfclass _ZFP_I_ZFIOToken_compress : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_compress, ZFIOToken)
public:
    zfbool open(
            ZF_IN const ZFPathInfo &refPathInfo
            , ZF_IN const zfstring &itemPath
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        this->ioClose();

        _refIOToken = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->obtain(refPathInfo, flags);
        if(_refIOToken == zfnull) {
            return zffalse;
        }
        _refPathInfo = refPathInfo;
        _itemPath = itemPath;
        _refOpenFlags = flags;
        _buf = zfobj<ZFIOBuffer>();

        if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
            if(!_refIOToken->ioRead(_buf->output(), itemPath)) {
                _refIOToken = zfnull;
                _refPathInfo = zfnull;
                _itemPath = zfnull;
                return zffalse;
            }
            _buf->input().ioSeek(0);
        }
        else if(!ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(!_refIOToken->ioRead(_buf->output(), itemPath)) {
                _refIOToken = zfnull;
                _refPathInfo = zfnull;
                _itemPath = zfnull;
                return zffalse;
            }
            _buf->input().ioSeek(0);
        }

        return zftrue;
    }

private:
    zfautoT<ZFCompressToken> _refIOToken;
    ZFPathInfo _refPathInfo;
    zfstring _itemPath;
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
        return ZFPathInfo(ZFPathType_cachePath(), ZFMd5(zfstr("ZFIO_compress/%s/%s", _refPathInfo, (const void *)this)));
    }

public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_compress();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return ZFPathInfoChainEncode(_refPathInfo, _itemPath);
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
            ret = _refIOToken->ioWrite(_itemPath, _buf->input());
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
zfclass _ZFP_I_ZFIOImpl_compress : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_compress, ZFIOImpl)
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
        return ZFPathType_compress();
    }

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        zfautoT<ZFCompressToken> token = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->obtain(refPathInfo);
        return token && token->ioIsExist(selfPathData);
    }
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        zfautoT<ZFCompressToken> token = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->obtain(refPathInfo);
        return token && token->ioIsDir(selfPathData);
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
        return ZFFileNameOfT(ret, selfPathData);
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
        if(!ZFIOImpl::ioToChildDefault(selfPathData, selfPathData, childName)) {
            return zffalse;
        }
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
        return zftrue;
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
        if(!ZFIOImpl::ioToParentDefault(selfPathData, selfPathData)) {
            return zffalse;
        }
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
        return zftrue;
    }
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->remove(refPathInfo);
        zfautoT<ZFCompressToken> token = ZFCompressOpen(refPathInfo, v_ZFIOOpenOption::e_Modify);
        if(!token) {
            return zffalse;
        }
        return token->ioPathCreate(selfPathData);
    }
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->remove(refPathInfo);
        zfautoT<ZFCompressToken> token = ZFCompressOpen(refPathInfo, v_ZFIOOpenOption::e_Modify);
        if(!token) {
            return zffalse;
        }
        return token->ioRemove(selfPathData);
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        ZFPathInfo refPathInfoFrom;
        zfstring selfPathDataFrom;
        if(!ZFPathInfoChainDecode(refPathInfoFrom, selfPathDataFrom, pathDataFrom)) {
            return zffalse;
        }
        ZFPathInfo refPathInfoTo;
        zfstring selfPathDataTo;
        if(!ZFPathInfoChainDecode(refPathInfoTo, selfPathDataTo, pathDataTo)) {
            return zffalse;
        }
        if(refPathInfoFrom != refPathInfoTo) {
            return zffalse;
        }
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->remove(refPathInfoFrom);
        zfautoT<ZFCompressToken> token = ZFCompressOpen(refPathInfoFrom, v_ZFIOOpenOption::e_Modify);
        if(!token) {
            return zffalse;
        }
        return token->ioMove(selfPathDataFrom, selfPathDataTo);
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
        zfautoT<ZFCompressToken> token = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCompressTokenCache)->obtain(refPathInfo);
        if(!token) {
            return zffalse;
        }
        if(!token->ioFindFirst(fd, selfPathData)) {
            return zffalse;
        }
        fd.implTag("_ZFP_ZFIO_compress_ioFindOwner", token);
        return zftrue;
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFCompressToken> token = fd.implTag("_ZFP_ZFIO_compress_ioFindOwner");
        return token && token->ioFindNext(fd);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        zfautoT<ZFCompressToken> token = fd.implTag("_ZFP_ZFIO_compress_ioFindOwner");
        if(token) {
            fd.implTag("_ZFP_ZFIO_compress_ioFindOwner", zfnull);
            token->ioFindClose(fd);
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
        zfobj<_ZFP_I_ZFIOToken_compress> ioToken;
        if(!ioToken->open(refPathInfo, selfPathData, flags, autoCreateParent)) {
            return zfnull;
        }
        return ioToken;
    }
};
ZFIO_DEFINE(compress, _ZFP_I_ZFIOImpl_compress)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        ) {
    ZFInput ret;
    ZFInputForPathInfoTokenT(
            ret
            , ZFIOOpen(ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(refPathInfo, itemPath)), v_ZFIOOpenOption::e_Read)
            );
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Modify)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoTokenT(
            ret
            , ZFIOOpen(ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(refPathInfo, itemPath)), flags)
            );
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        ) {
    if(!refInput) {
        return zfnull;
    }
    if(refInput.pathInfo()) {
        return ZFInputForCompress(refInput.pathInfo(), itemPath);
    }

    zfobj<ZFIOBuffer> buf;
    buf->bufferSize(0);
    if(ZFInputRead(buf->output(), refInput) == zfindexMax()) {
        return zfnull;
    }

    ZFInput ret;
    ret.callbackSerializeDisable(zftrue);
    ZFInputForPathInfoT(ret, buf->pathInfo());
    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN_OPT(const zfstring &, itemPath, zftext("content"))
        ) {
    if(!refOutput) {
        return zfnull;
    }
    ZFLISTENER_1(conv
            , zfstring, itemPath
            ) {
        ZFIOBuffer *buf = zfargs.sender();
        buf->input().ioSeek(0);
        ZFOutput refOutput = zfargs.param0().to<v_ZFOutput *>()->zfv;

        ZFPathInfo cachePath = ZFIO_cachePathGen();
        zfautoT<ZFCompressToken> compress = ZFCompressOpen(cachePath, v_ZFIOOpenOption::e_Write);
        compress->ioWrite(itemPath, buf->input());
        compress = zfnull;

        ZFInputRead(refOutput, ZFInputForPathInfo(cachePath));
        ZFIORemove(cachePath);
    } ZFLISTENER_END()
    return ZFIOBufferOutput(refOutput, conv);
}

ZF_NAMESPACE_GLOBAL_END

