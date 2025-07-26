#include "ZFIOBufferByMemory.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIOBufferByMemory {
public:
    zfuint refCount;
    zfstring ioBuf;
    zfindex inputIndex;
    zfindex outputIndex;
public:
    _ZFP_ZFIOBufferByMemory(void)
    : refCount(1)
    , ioBuf()
    , inputIndex(0)
    , outputIndex(0)
    {
    }
};

zfclass _ZFP_ZFIOBufferByMemory_input : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByMemory_input, ZFObject)
public:
    _ZFP_ZFIOBufferByMemory *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
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
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

zfclass _ZFP_ZFIOBufferByMemory_output : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByMemory_output, ZFObject)
public:
    _ZFP_ZFIOBufferByMemory *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
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
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

// ============================================================
ZFOBJECT_REGISTER(ZFIOBufferByMemory)

void ZFIOBufferByMemory::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFIOBufferByMemory);

    zfobj<_ZFP_ZFIOBufferByMemory_input> iOwner;
    iOwner->d = d;
    ++(d->refCount);
    this->_ZFP_input = ZFCallbackForMemberMethod(iOwner, ZFMethodAccess(_ZFP_ZFIOBufferByMemory_input, onInput));
    this->_ZFP_input.ioOwner(iOwner);

    zfobj<_ZFP_ZFIOBufferByMemory_output> oOwner;
    oOwner->d = d;
    ++(d->refCount);
    this->_ZFP_output = ZFCallbackForMemberMethod(oOwner, ZFMethodAccess(_ZFP_ZFIOBufferByMemory_output, onOutput));
    this->_ZFP_output.ioOwner(oOwner);
}
void ZFIOBufferByMemory::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    zfsuper::objectOnDealloc();
}

void ZFIOBufferByMemory::implRemoveAll(void) {
    d->ioBuf.removeAll();
    d->inputIndex = 0;
    d->outputIndex = 0;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_input, zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            ) {
    if(buf == zfnull) {
        return d->ioBuf.length() - d->inputIndex;
    }
    else {
        count = zfmMin(count, d->ioBuf.length() - d->inputIndex);
        zfmemcpy(buf, d->ioBuf.cString() + d->inputIndex, count);
        d->inputIndex += count;
        return count;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_input, zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            ) {
    d->inputIndex = ZFIOCallbackCalcSeek(0, d->ioBuf.length(), d->inputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_input, zfindex, ioTell) {
    return d->inputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_input, zfindex, ioSize) {
    return d->ioBuf.length();
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_output, zfindex, onOutput
            , ZFMP_IN(const void *, buf)
            , ZFMP_IN(zfindex, count)
            ) {
    if(count == zfindexMax()) {
        count = zfslen((const zfchar *)buf);
    }
    d->ioBuf.replace(d->outputIndex, zfmMin(count, d->ioBuf.length() - d->outputIndex), (const zfchar *)buf, count);
    d->outputIndex += count;
    return count;
}

ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_output, zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            ) {
    d->outputIndex = ZFIOCallbackCalcSeek(0, d->ioBuf.length(), d->outputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_output, zfindex, ioTell) {
    return d->outputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_output, zfindex, ioSize) {
    return d->ioBuf.length();
}

ZF_NAMESPACE_GLOBAL_END

