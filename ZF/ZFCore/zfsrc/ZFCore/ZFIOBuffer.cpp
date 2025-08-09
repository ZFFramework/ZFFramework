#include "ZFIOBuffer.h"
#include "ZFIO_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zfindex, ZFIOBufferSize, 256 * 1024)

// ============================================================
zfclassNotPOD _ZFP_ZFIOBufferPrivate {
public:
    zfuint refCount;
    zfbool cacheFileAutoRemove;
    zfindex contentIndex;
    zfindex contentSize;

    zfindex bufferSize;
    zfstring buf;
    zfautoT<ZFIOToken> cacheFile;
    ZFPathInfo cacheFilePath;
public:
    _ZFP_ZFIOBufferPrivate(void)
    : refCount(1)
    , cacheFileAutoRemove(zftrue)
    , contentIndex(0)
    , contentSize(0)
    , bufferSize(ZFIOBufferSize())
    , buf()
    , cacheFile()
    , cacheFilePath()
    {
    }
    ~_ZFP_ZFIOBufferPrivate(void) {
        if(this->cacheFile) {
            this->cacheFile->ioClose();
            this->cacheFile = zfnull;
        }
    }
public:
    void changeToCacheFile(void) {
        if(this->cacheFile) {
            return;
        }
        this->cacheFile = ZFIOOpenCache(
                this->cacheFilePath
                , v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write
                , this->cacheFileAutoRemove
                );
        if(!this->cacheFile) {
            this->cacheFile = ZFIOOpen(ZFPathInfo(ZFPathType_cachePath(), zfstr("ZFIOBuffer/%s_%s"
                            , zfidentityCalc(this)
                            , zfmRand(30000)
                            )), v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write);
        }
        if(!this->cacheFile) {
            return;
        }

        if(this->contentSize > 0) {
            if(this->cacheFile->ioWrite(this->buf.cString(), this->contentSize) != this->contentSize) {
                this->cacheFile = zfnull;
                return;
            }
            if(!this->cacheFile->ioSeek(this->contentIndex)) {
                this->cacheFile = zfnull;
                return;
            }
            this->buf = zfnull;
        }
    }
};

zfclass _ZFP_I_ZFIOBuffer_input : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOBuffer_input, ZFObject)
public:
    _ZFP_ZFIOBufferPrivate *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
        zfsuper::objectOnDealloc();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

zfclass _ZFP_I_ZFIOBuffer_output : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOBuffer_output, ZFObject)
public:
    _ZFP_ZFIOBufferPrivate *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
        zfsuper::objectOnDealloc();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

// ============================================================
ZFOBJECT_REGISTER(ZFIOBuffer)

void ZFIOBuffer::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFIOBufferPrivate);

    zfobj<_ZFP_I_ZFIOBuffer_input> iOwner;
    iOwner->d = d;
    ++(d->refCount);
    _ZFP_input = ZFCallbackForMemberMethod(iOwner, ZFMethodAccess(_ZFP_I_ZFIOBuffer_input, onInput));
    _ZFP_input.ioOwner(iOwner);

    zfobj<_ZFP_I_ZFIOBuffer_output> oOwner;
    oOwner->d = d;
    ++(d->refCount);
    _ZFP_output = ZFCallbackForMemberMethod(oOwner, ZFMethodAccess(_ZFP_I_ZFIOBuffer_output, onOutput));
    _ZFP_output.ioOwner(oOwner);
}
void ZFIOBuffer::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFIOBuffer, zfindex, bufferSize) {
    if(propertyValue <= d->bufferSize) {
        d->changeToCacheFile();
    }
}

ZFMETHOD_DEFINE_2(ZFIOBuffer, void, cacheFilePath
        , ZFMP_IN(const ZFPathInfo &, value)
        , ZFMP_IN_OPT(zfbool, autoRemove, zftrue)
        ) {
    if(!d->cacheFile) {
        d->cacheFilePath = value;
        d->cacheFileAutoRemove = zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFPathInfo, cacheFilePath) {
    return d->cacheFilePath;
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, zfbool, cacheFileAutoRemove) {
    return d->cacheFileAutoRemove;
}

ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFPathInfo, pathInfo) {
    if(d->cacheFile) {
        return d->cacheFile->pathInfo();
    }
    else {
        return ZFPathInfo(ZFPathType_text(), d->buf);
    }
}

ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFInput, input) {
    return _ZFP_input;
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFOutput, output) {
    return _ZFP_output;
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, void, removeAll) {
    d->contentIndex = 0;
    d->contentSize = 0;
    if(d->cacheFile) {
        d->cacheFile->ioSeek(0);
    }
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBuffer_input, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(buf == zfnull) {
        return 0;
    }
    count = zfmMin<zfindex>(count, d->contentSize - d->contentIndex);
    if(d->cacheFile) {
        count = d->cacheFile->ioRead(buf, count);
    }
    else {
        zfmemcpy(buf, d->buf.cString(), count);
    }
    d->contentIndex += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBuffer_input, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    d->contentIndex = ZFIOCallbackCalcSeek(0, d->contentSize, d->contentIndex, byteSize, seekPos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBuffer_input, zfindex, ioTell) {
    return d->contentIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBuffer_input, zfindex, ioSize) {
    return d->contentSize;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBuffer_output, zfindex, onOutput
        , ZFMP_IN(const void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(d->cacheFile) {
        count = d->cacheFile->ioWrite(buf, count);
        d->contentIndex += count;
        d->contentSize = zfmMax(d->contentSize, d->contentIndex);
        return count;
    }
    else {
        if(count == zfindexMax()) {
            count = zfslen((const zfchar *)buf);
        }
        if(d->contentIndex + count >= d->bufferSize) {
            d->changeToCacheFile();
            if(!d->cacheFile) {
                return 0;
            }
            count = d->cacheFile->ioWrite(buf, count);
            d->contentIndex += count;
            d->contentSize = zfmMax(d->contentSize, d->contentIndex);
            return count;
        }
        else {
            d->buf.capacity(d->contentIndex + count);
            zfmemcpy(d->buf.zfunsafe_buffer() + d->contentIndex, buf, count);
            d->contentIndex += count;
            d->contentSize = zfmMax(d->contentSize, d->contentIndex);
            return count;
        }
    }
}

ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBuffer_output, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    d->contentIndex = ZFIOCallbackCalcSeek(0, d->contentSize, d->contentIndex, byteSize, seekPos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBuffer_output, zfindex, ioTell) {
    return d->contentIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBuffer_output, zfindex, ioSize) {
    return d->contentSize;
}

// ============================================================
zfclass _ZFP_I_ZFIOBufferInputOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOBufferInputOwner, ZFObject)
public:
    zfobj<ZFIOBuffer> buf;
    ZFInput refInput;
    ZFListener convCallback;
private:
    void _prepare(void) {
        if(this->refInput) {
            if(this->convCallback) {
                this->convCallback.execute(ZFArgs()
                        .sender(this->buf)
                        .param0(zfobj<v_ZFInput>(this->refInput))
                        );
            }
            else {
                ZFInputRead(this->buf->output(), this->refInput);
            }
            this->refInput = zfnull;
        }
    }
public:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBufferInputOwner, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    _prepare();
    return this->buf->input().execute(buf, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBufferInputOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    _prepare();
    return this->buf->input().ioSeek(byteSize, seekPos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBufferInputOwner, zfindex, ioTell) {
    _prepare();
    return this->buf->input().ioTell();
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBufferInputOwner, zfindex, ioSize) {
    _prepare();
    return this->buf->input().ioSize();
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFIOBufferInput
        , ZFMP_IN(const ZFInput &, refInput)
        , ZFMP_IN_OPT(const ZFListener &, convCallback, zfnull)
        ) {
    zfobj<_ZFP_I_ZFIOBufferInputOwner> ioOwner;
    ioOwner->refInput = refInput;
    ioOwner->convCallback = convCallback;
    ZFInput ret = ZFCallbackForMemberMethod(ioOwner, ZFMethodAccess(_ZFP_I_ZFIOBufferInputOwner, onInput));
    ret.ioOwner(ioOwner);
    return ret;
}

// ============================================================
zfclass _ZFP_I_ZFIOBufferOutputOwner : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOBufferOutputOwner, ZFObject)
public:
    zfobj<ZFIOBuffer> buf;
    ZFOutput refOutput;
    ZFListener convCallback;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->convCallback) {
            this->convCallback.execute(ZFArgs()
                    .sender(this->buf)
                    .param0(zfobj<v_ZFOutput>(this->refOutput))
                    );
        }
        else {
            ZFInputRead(this->refOutput, this->buf->input());
        }
        zfsuper::objectOnDeallocPrepare();
    }
public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, seekPos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBufferOutputOwner, zfindex, onOutput
        , ZFMP_IN(const void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    return this->buf->output().execute(buf, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFIOBufferOutputOwner, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, seekPos)
        ) {
    return this->buf->output().ioSeek(byteSize, seekPos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBufferOutputOwner, zfindex, ioTell) {
    return this->buf->output().ioTell();
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFIOBufferOutputOwner, zfindex, ioSize) {
    return this->buf->output().ioSize();
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFIOBufferOutput
        , ZFMP_IN(const ZFOutput &, refOutput)
        , ZFMP_IN_OPT(const ZFListener &, convCallback, zfnull)
        ) {
    zfobj<_ZFP_I_ZFIOBufferOutputOwner> ioOwner;
    ioOwner->refOutput = refOutput;
    ioOwner->convCallback = convCallback;
    ZFOutput ret = ZFCallbackForMemberMethod(ioOwner, ZFMethodAccess(_ZFP_I_ZFIOBufferOutputOwner, onOutput));
    ret.ioOwner(ioOwner);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

