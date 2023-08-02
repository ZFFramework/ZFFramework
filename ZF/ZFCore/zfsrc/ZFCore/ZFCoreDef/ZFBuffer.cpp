#include "ZFBuffer.h"
#include "ZFCoreStringConvert.h"

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_ZFBufferPrivate::~_ZFP_ZFBufferPrivate(void) {
    if(this->bufferAutoFree && this->buffer) {
        zffree(this->buffer);
    }
}

static inline void _ZFP_ZFBufferCapacityOptimize(ZF_IN_OUT zfindex &capacity) {
    if(capacity == 0) {
        // nothing to do
    }
    else if(capacity < 256) {
        capacity = ((capacity / 64) + 1) * 64 - sizeof(zfchar);
    }
    else if(capacity < 1024) {
        capacity = ((capacity / 256) + 1) * 256 - sizeof(zfchar);
    }
    else {
        capacity = ((capacity / 1024) + 1) * 1024 - sizeof(zfchar);
    }
}

void ZFBuffer::bufferCapacity(ZF_IN zfindex bufferCapacity) {
    if(bufferCapacity == zfindexMax()) {
        return;
    }
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    if(bufferCapacity > d->bufferCapacity) {
        _bufferCapacityDoChange(bufferCapacity);
    }
}
void ZFBuffer::bufferCapacityTrim(void) {
    zfindex bufferCapacity = this->bufferSize();
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    if(bufferCapacity != this->bufferCapacity()) {
        _bufferCapacityDoChange(bufferCapacity);
    }
}
void ZFBuffer::_bufferCapacityDoChange(ZF_IN zfindex bufferCapacity) {
    if(bufferCapacity == 0) {
        if(d->bufferAutoFree) {
            zffree(d->buffer);
        }
        d->buffer = zfnull;
        d->bufferCapacity = 0;
        d->bufferAutoFree = zffalse;
        return;
    }

    if(d->bufferAutoFree) {
        d->buffer = zfrealloc(d->buffer, bufferCapacity + sizeof(zfchar));
    }
    else {
        void *bufferOld = d->buffer;
        d->buffer = zfmalloc(bufferCapacity + sizeof(zfchar));
        zfmemcpy(d->buffer, bufferOld, d->bufferCapacity);
    }
    *(zfchar *)((zfbyte *)d->buffer + d->bufferCapacity) = '\0';

    d->bufferCapacity = bufferCapacity;
    d->bufferAutoFree = zftrue;
}

void ZFBuffer::bufferFree(void) {
    if(d->buffer != zfnull && d->bufferAutoFree) {
        zffree(d->buffer);
    }
    d->buffer = zfnull;
    d->bufferCapacity = 0;
    d->bufferSize = 0;
    d->bufferAutoFree = zffalse;
}

void ZFBuffer::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    // "<ZFBuffer %p(%zi/%zi)>"
    ret += "<ZFBuffer ";
    zfsFromPointerT(ret, this->buffer());
    ret += "(";
    zfsFromIntT(ret, this->bufferSize());
    ret += "/";
    zfsFromIntT(ret, this->bufferCapacity());
    ret += ")>";
}

ZF_NAMESPACE_GLOBAL_END

