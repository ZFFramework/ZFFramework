/**
 * @file ZFBuffer.h
 * @brief buffer util
 */

#ifndef _ZFI_ZFBuffer_h_
#define _ZFI_ZFBuffer_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFBufferPrivate
{
public:
    zfuint refCount;
    void *buffer;
    zfindex bufferCapacity;
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
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFBuffer
{
public:
    /**
     * @brief malloc, #buffer would be null if malloc failed
     *
     * newly memory is ensured to be filled with 0\n
     * note, for string type, you should ensure buffer size and append tail '\0' manually
     * @note the input bufferCapacity is the minimal capacity,
     *   the result capacity may or may not large than the requested one
     */
    zffinal zfbool bufferMalloc(ZF_IN zfindex bufferCapacity);
    /**
     * @brief realloc, #buffer won't be changed if realloc failed
     *
     * newly memory is ensured to be filled with 0\n
     * note, for string type, you should ensure buffer size and append tail '\0' manually
     * @note the input bufferCapacity is the minimal capacity,
     *   the result capacity may or may not large than the requested one
     */
    zffinal zfbool bufferRealloc(ZF_IN zfindex bufferCapacity);
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
     * @brief util method to change the internal buffer
     *
     * note, for string type, you should ensure buffer size and append tail '\0' manually
     */
    zffinal void bufferChange(ZF_IN void *buffer, ZF_IN zfindex bufferCapacity, ZF_IN zfindex bufferSize, ZF_IN zfbool bufferAutoFree)
    {
        this->bufferFree();
        d->buffer = buffer;
        d->bufferCapacity = bufferCapacity;
        d->bufferSize = bufferSize;
        d->bufferAutoFree = bufferAutoFree;
    }
    /**
     * @brief util method to malloc and copy buffer, ensured NULL terminated (not included in bufferSize)
     */
    zffinal zfbool bufferCopy(ZF_IN const void *buffer, ZF_IN zfindex bufferSize)
    {
        if(!this->bufferRealloc(bufferSize + sizeof(zfchar)))
        {
            return zffalse;
        }
        zfmemcpy(this->buffer(), buffer, bufferSize);
        *((zfchar *)((zfbyte *)this->buffer() + bufferSize)) = '\0';
        d->bufferSize = bufferSize;
        return zftrue;
    }
    /**
     * @brief util method to copy string
     */
    zffinal zfbool bufferCopy(ZF_IN const zfchar *s, ZF_IN_OPT zfindex length = zfindexMax())
    {
        return this->bufferCopy((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to copy string
     */
    zffinal zfbool bufferCopy(ZF_IN const zfstring &s)
    {
        return this->bufferCopy(s.cString(), s.length());
    }

    /**
     * @brief util method to realloc and append buffer, ensured NULL terminated (not included in bufferSize)
     */
    zffinal zfbool bufferAppend(ZF_IN const void *buffer, ZF_IN zfindex bufferSize)
    {
        if(!this->bufferRealloc(this->bufferSize() + bufferSize + sizeof(zfchar)))
        {
            return zffalse;
        }
        zfmemcpy((zfbyte *)this->buffer() + this->bufferSize(), buffer, bufferSize);
        *((zfchar *)((zfbyte *)this->buffer() + this->bufferSize() + bufferSize)) = '\0';
        d->bufferSize += bufferSize;
        return zftrue;
    }
    /**
     * @brief util method to append string
     */
    zffinal zfbool bufferAppend(ZF_IN const zfchar *s, ZF_IN_OPT zfindex length = zfindexMax())
    {
        return this->bufferAppend((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to append string
     */
    zffinal zfbool bufferAppend(ZF_IN const zfstring &s)
    {
        return this->bufferAppend(s.cString(), s.length());
    }

public:
    /**
     * @brief change buffer capacity, same as #bufferRealloc
     */
    zffinal void bufferCapacity(ZF_IN zfindex capacity)
    {
        this->bufferRealloc(capacity);
    }
    /**
     * @brief return buffer capacity
     */
    zffinal zfindex bufferCapacity(void) const
    {
        return d->bufferCapacity;
    }
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
    /**
     * @brief construct from exist buffer, see #bufferChange
     */
    ZFBuffer(ZF_IN void *buffer, ZF_IN zfindex bufferCapacity, ZF_IN zfindex bufferSize, ZF_IN zfbool bufferAutoFree)
    {
        d = zfnew(_ZFP_ZFBufferPrivate);
        d->buffer = buffer;
        d->bufferCapacity = bufferCapacity;
        d->bufferSize = bufferSize;
        d->bufferAutoFree = bufferAutoFree;
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
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFBuffer_h_

