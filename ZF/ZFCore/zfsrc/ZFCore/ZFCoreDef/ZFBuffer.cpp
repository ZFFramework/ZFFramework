#include "ZFBuffer.h"
#include "ZFCoreSPrintf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static inline void _ZFP_ZFBufferCapacityOptimize(ZF_IN_OUT zfindex &capacity)
{
    if(capacity < 256)
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

zfbool ZFBuffer::bufferMalloc(ZF_IN zfindex bufferCapacity)
{
    if(bufferCapacity == zfindexMax())
    {
        return zffalse;
    }
    if(bufferCapacity <= d->bufferCapacity && bufferCapacity * 4 >= d->bufferCapacity)
    {
        return zftrue;
    }
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    this->bufferFree();
    d->buffer = zfmalloc(bufferCapacity);
    if(d->buffer != zfnull)
    {
        zfmemset(d->buffer, 0, bufferCapacity);
        d->bufferCapacity = bufferCapacity;
        d->bufferAutoFree = zftrue;
        return zftrue;
    }
    else
    {
        d->bufferCapacity = 0;
        d->bufferAutoFree = zffalse;
        return zffalse;
    }
}
zfbool ZFBuffer::bufferRealloc(ZF_IN zfindex bufferCapacity)
{
    if(bufferCapacity == zfindexMax())
    {
        return zffalse;
    }
    if(bufferCapacity <= d->bufferCapacity && bufferCapacity * 4 >= d->bufferCapacity)
    {
        return zftrue;
    }
    _ZFP_ZFBufferCapacityOptimize(bufferCapacity);
    void *bufferOld = d->buffer;

    if(d->bufferAutoFree)
    {
        d->buffer = zfrealloc(d->buffer, bufferCapacity);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return zffalse;
        }
        if(bufferCapacity > d->bufferCapacity)
        {
            zfmemset((zfbyte *)d->buffer + d->bufferCapacity, 0, bufferCapacity - d->bufferCapacity);
        }
    }
    else
    {
        d->buffer = zfmalloc(bufferCapacity);
        if(d->buffer == zfnull)
        {
            d->buffer = bufferOld;
            return zffalse;
        }
        if(bufferCapacity > d->bufferCapacity)
        {
            zfmemcpy(d->buffer, bufferOld, d->bufferCapacity);
            zfmemset((zfbyte *)d->buffer + d->bufferCapacity, 0, bufferCapacity - d->bufferCapacity);
        }
        else
        {
            zfmemcpy(d->buffer, bufferOld, bufferCapacity);
        }
    }

    d->bufferCapacity = bufferCapacity;
    d->bufferAutoFree = zftrue;
    return zftrue;
}
void ZFBuffer::bufferFree(void)
{
    if(d->buffer != zfnull && d->bufferAutoFree)
    {
        zffree(d->buffer);
        d->buffer = zfnull;
        d->bufferCapacity = 0;
        d->bufferSize = 0;
        d->bufferAutoFree = zffalse;
    }
}

void ZFBuffer::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    zfstringAppend(ret, "<ZFBuffer %p(%zi/%zi)>", this->buffer(), this->bufferSize(), this->bufferCapacity());
}

ZF_NAMESPACE_GLOBAL_END

