/**
 * @file ZFBuffer.h
 * @brief buffer util
 */

#ifndef _ZFI_ZFBuffer_h_
#define _ZFI_ZFBuffer_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFBufferPrivate
{
public:
    zfuint refCount;
    void *buffer;
    zfindex bufferCapacity; // capacity exclude tail '\0'
    zfindex bufferSize;
    zfbool bufferAutoFree;
public:
    _ZFP_ZFBufferPrivate(void)
    : refCount(1)
    , buffer(zfnull)
    , bufferCapacity(0)
    , bufferSize(0)
    , bufferAutoFree(zffalse)
    {
    }
    ~_ZFP_ZFBufferPrivate(void)
    {
        if(this->bufferAutoFree && this->buffer)
        {
            zffree(this->buffer);
        }
    }
};
/**
 * @brief buffer util
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFBuffer
{
public:
    /**
     * @brief free the internal buffer and reset all to zero
     */
    zffinal void bufferFree(void);
    /**
     * @brief give up the buffer's ownership and return the buffer, you should free it manually
     */
    zffinal void *bufferGiveUp(void)
    {
        void *ret = d->buffer;
        d->buffer = zfnull;
        d->bufferCapacity = 0;
        d->bufferSize = 0;
        d->bufferAutoFree = zffalse;
        return ret;
    }

public:
    /**
     * @brief change the internal buffer, usually for internal use only
     *
     * note the bufferSize <= bufferCapacity, the actual buffer capacity must at least (bufferCapacity + sizeof(zfchar))
     */
    zffinal void zfunsafe_bufferChange(ZF_IN void *buffer, ZF_IN zfindex bufferCapacity, ZF_IN zfindex bufferSize, ZF_IN zfbool bufferAutoFree)
    {
        this->bufferFree();
        d->buffer = buffer;
        d->bufferCapacity = bufferCapacity;
        d->bufferSize = bufferSize;
        d->bufferAutoFree = bufferAutoFree;
    }
    /**
     * @brief util method to malloc and copy buffer, ensured NULL terminated (not included in bufferSize)
     * @note bufferSize must explicitly specified
     */
    zffinal void bufferCopy(ZF_IN const void *buffer, ZF_IN zfindex bufferSize)
    {
        if(bufferSize == zfindexMax())
        {
            bufferSize = zfslen((const zfchar *)buffer) * sizeof(zfchar);
        }
        this->bufferCapacity(bufferSize);
        if(bufferSize > 0)
        {
            zfmemcpy(this->buffer(), buffer, bufferSize);
            *((zfchar *)((zfbyte *)this->buffer() + bufferSize)) = '\0';
        }
        d->bufferSize = bufferSize;
    }
    /**
     * @brief util method to copy string
     */
    zffinal void bufferCopy(ZF_IN const zfchar *s, ZF_IN_OPT zfindex length = zfindexMax())
    {
        this->bufferCopy((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to copy string
     */
    zffinal void bufferCopy(ZF_IN const zfstring &s)
    {
        this->bufferCopy(s.cString(), s.length());
    }

    /**
     * @brief util method to realloc and append buffer, ensured NULL terminated (not included in bufferSize)
     */
    zffinal void bufferAppend(ZF_IN const void *buffer, ZF_IN zfindex bufferSize)
    {
        if(bufferSize == zfindexMax())
        {
            bufferSize = zfslen((const zfchar *)buffer) * sizeof(zfchar);
        }
        if(bufferSize > 0)
        {
            this->bufferCapacity(this->bufferSize() + bufferSize);
            zfmemcpy((zfbyte *)this->buffer() + this->bufferSize(), buffer, bufferSize);
            *((zfchar *)((zfbyte *)this->buffer() + this->bufferSize() + bufferSize)) = '\0';
            d->bufferSize += bufferSize;
        }
    }
    /**
     * @brief util method to append string
     */
    zffinal void bufferAppend(ZF_IN const zfchar *s, ZF_IN_OPT zfindex length = zfindexMax())
    {
        this->bufferAppend((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to append string
     */
    zffinal void bufferAppend(ZF_IN const zfstring &s)
    {
        this->bufferAppend(s.cString(), s.length());
    }

public:
    /**
     * @brief change buffer capacity
     *
     * this method do nothing if memory allocate fail\n
     * when success, for convenient, it's ensured the buffer can hold bytes up to
     * (buffer.bufferCapacity() + sizeof(zfchar))
     */
    zffinal void bufferCapacity(ZF_IN zfindex bufferCapacity);
    /**
     * @brief return buffer capacity
     */
    zffinal zfindex bufferCapacity(void) const
    {
        return d->bufferCapacity;
    }
    /**
     * @brief trim buffer capacity
     */
    zffinal void bufferCapacityTrim(void);
    /**
     * @brief change #bufferSize only, no memory change would be applied
     *
     * new size must be smaller than current #bufferCapacity\n
     * the bufferSize only stored for convenient,
     * the actual meaning depends on its user
     */
    zffinal void bufferSize(ZF_IN zfindex bufferSize)
    {
        if(bufferSize <= d->bufferCapacity)
        {
            d->bufferSize = bufferSize;
        }
    }
    /**
     * @brief return buffer content's size
     */
    zffinal zfindex bufferSize(void) const
    {
        return d->bufferSize;
    }

public:
    /**
     * @brief access the buffer
     */
    zffinal void *buffer(void)
    {
        return d->buffer;
    }
    /**
     * @brief access the buffer
     */
    zffinal const void *buffer(void) const
    {
        return d->buffer;
    }
    /**
     * @brief util method to access buffer as string type
     */
    zffinal zfchar *text(void)
    {
        return this->bufferT<zfchar *>();
    }
    /**
     * @brief util method to access buffer as string type
     */
    zffinal const zfchar *text(void) const
    {
        return this->bufferT<const zfchar *>();
    }
    /**
     * @brief util method to access buffer as string type
     */
    zfindex textLength(void) const
    {
        return this->bufferSize() / sizeof(zfchar);
    }
    /**
     * @brief access the buffer
     */
    template<typename T_Type>
    T_Type bufferT(void)
    {
        return ZFCastStatic(T_Type, this->buffer());
    }
    /**
     * @brief access the buffer
     */
    template<typename T_Type>
    T_Type bufferT(void) const
    {
        return ZFCastStatic(T_Type, this->buffer());
    }
    /**
     * @brief whether the buffer would be free-ed automatically
     */
    zffinal zfbool bufferAutoFree(void) const
    {
        return d->bufferAutoFree;
    }

public:
    /**
     * @brief construct an empty map
     */
    ZFBuffer(void)
    {
        d = zfnew(_ZFP_ZFBufferPrivate);
    }
    /**
     * @brief retain the ref, to copy, use #bufferCopy
     */
    ZFBuffer(ZF_IN const ZFBuffer &ref)
    : d(ref.d)
    {
        ++(d->refCount);
    }
    /** @cond ZFPrivateDoc */
    zffinal ZFBuffer &operator = (ZF_IN const ZFBuffer &ref)
    {
        _ZFP_ZFBufferPrivate *dTmp = d;
        d = ref.d;
        ++(ref.d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
        return *this;
    }
    zffinal zfbool operator == (ZF_IN const ZFBuffer &ref) const
    {
        return (d == ref.d);
    }
    inline zfbool operator != (ZF_IN const ZFBuffer &ref) const {return !this->operator == (ref);}
    /** @endcond */
    ~ZFBuffer(void)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const
    {
        return d->refCount;
    }

private:
    _ZFP_ZFBufferPrivate *d;
private:
    void _bufferCapacityDoChange(ZF_IN zfindex bufferCapacity);
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFBuffer_h_

