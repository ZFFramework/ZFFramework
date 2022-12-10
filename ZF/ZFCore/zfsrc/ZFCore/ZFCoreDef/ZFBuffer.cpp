#include "ZFBuffer.h"
#include "ZFCoreSPrintf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static inline void _ZFP_ZFBufferCapacityOptimize(ZF_IN_OUT zfindex &capacity)
{
    if(capacity == 0)
    {
        // nothing to do
    }
    else if(capacity < 256)
    {
        capacity = ((capacity / 64) + 1) * 64;
    }
    else if(capacity < 1024)
    {
        capacity = ((capacity / 256) + 1) * 256;
    }
    else
    {
        capacity = ((capacity / 1024) + 1) * 1024;
    }
}

void ZFBuffer::bufferCapacity(ZF_IN zfindex bufferCapacity)
{
    if(bufferCapacity == zfindexMax())
    {
        return;
    }
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    if(bufferCapacity >= d->bufferCapacity)
    {
        _bufferCapacityDoChange(bufferCapacity);
    }
}
void ZFBuffer::bufferCapacityTrim(void)
{
    zfindex bufferCapacity = this->bufferSize();
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    if(bufferCapacity - 1 != this->bufferCapacity())
    {
        _bufferCapacityDoChange(bufferCapacity);
    }
}
void ZFBuffer::_bufferCapacityDoChange(ZF_IN zfindex bufferCapacity)
{
    if(bufferCapacity == 0)
    {
        if(d->bufferAutoFree)
        {
            zffree(d->buffer);
        }
        d->buffer = zfnull;
        d->bufferCapacity = 0;
        d->bufferAutoFree = zffalse;
        return;
    }

    void *bufferOld = d->buffer;

    if(d->bufferAutoFree)
    {
        d->buffer = zfrealloc(d->buffer, bufferCapacity);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return ;
        }
        zfmemset((zfbyte *)d->buffer + d->bufferCapacity, 0, bufferCapacity - d->bufferCapacity);
    }
    else
    {
        d->buffer = zfmalloc(bufferCapacity);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return ;
        }
        zfmemcpy(d->buffer, bufferOld, d->bufferCapacity);
    }

    d->bufferCapacity = bufferCapacity - 1;
    d->bufferAutoFree = zftrue;
}

void ZFBuffer::bufferFree(void)
{
    if(d->buffer != zfnull && d->bufferAutoFree)
    {
        zffree(d->buffer);
    }
    d->buffer = zfnull;
    d->bufferCapacity = 0;
    d->bufferSize = 0;
    d->bufferAutoFree = zffalse;
}

void ZFBuffer::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    zfstringAppend(ret, "<ZFBuffer %p(%zi/%zi)>", this->buffer(), this->bufferSize(), this->bufferCapacity());
}

ZF_NAMESPACE_GLOBAL_END

