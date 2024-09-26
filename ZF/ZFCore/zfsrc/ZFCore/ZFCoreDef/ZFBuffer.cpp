#include "ZFBuffer.h"
#include "zfstr.h"

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_ZFBufferPrivate::~_ZFP_ZFBufferPrivate(void) {
    if(this->autoFree && this->buffer) {
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

void ZFBuffer::capacity(ZF_IN zfindex capacity) {
    if(capacity == zfindexMax()) {
        return;
    }
    _ZFP_ZFBufferCapacityOptimize(capacity);
    if(capacity > d->capacity) {
        _capacityDoChange(capacity);
    }
}
void ZFBuffer::capacityTrim(void) {
    zfindex capacity = this->length();
    _ZFP_ZFBufferCapacityOptimize(capacity);
    if(capacity != this->capacity()) {
        _capacityDoChange(capacity);
    }
}
void ZFBuffer::_capacityDoChange(ZF_IN zfindex capacity) {
    if(capacity == 0) {
        if(d->autoFree) {
            zffree(d->buffer);
        }
        d->buffer = zfnull;
        d->capacity = 0;
        d->autoFree = zffalse;
        return;
    }

    if(d->autoFree) {
        d->buffer = zfrealloc(d->buffer, capacity + sizeof(zfchar));
    }
    else {
        void *bufferOld = d->buffer;
        d->buffer = zfmalloc(capacity + sizeof(zfchar));
        zfmemcpy(d->buffer, bufferOld, d->capacity);
    }
    *(zfchar *)((zfbyte *)d->buffer + d->capacity) = '\0';

    d->capacity = capacity;
    d->autoFree = zftrue;
}

void ZFBuffer::bufferFree(void) {
    if(d->buffer != zfnull && d->autoFree) {
        zffree(d->buffer);
    }
    d->buffer = zfnull;
    d->capacity = 0;
    d->length = 0;
    d->autoFree = zffalse;
}

void ZFBuffer::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFBuffer %s(%s/%s)>", this->buffer(), this->length(), this->capacity());
}

ZF_NAMESPACE_GLOBAL_END

