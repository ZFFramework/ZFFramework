/**
 * @file ZFBuffer.h
 * @brief buffer util
 */

#ifndef _ZFI_ZFBuffer_h_
#define _ZFI_ZFBuffer_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFBufferPrivate {
public:
    zfuint refCount;
    void *buffer;
    zfindex capacity; // capacity exclude tail '\0'
    zfindex length;
    zfbool autoFree;
public:
    _ZFP_ZFBufferPrivate(void)
    : refCount(1)
    , buffer(zfnull)
    , capacity(0)
    , length(0)
    , autoFree(zffalse)
    {
    }
    ~_ZFP_ZFBufferPrivate(void);
};
/**
 * @brief buffer util
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFBuffer {
public:
    /**
     * @brief free the internal buffer and reset all to zero
     */
    zffinal void bufferFree(void);
    /**
     * @brief give up the buffer's ownership and return the buffer, you should free it manually
     */
    zffinal void *bufferGiveUp(void) {
        void *ret = d->buffer;
        d->buffer = zfnull;
        d->capacity = 0;
        d->length = 0;
        d->autoFree = zffalse;
        return ret;
    }
    /**
     * @brief swap buffer
     */
    zffinal void swap(ZF_IN_OUT ZFBuffer &buf) {
        if(d == buf.d) {
            return;
        }
        void *buffer = d->buffer;
        zfindex capacity = d->capacity;
        zfindex length = d->length;
        zfindex autoFree = d->autoFree;

        d->buffer = buf.d->buffer;
        d->capacity = buf.d->capacity;
        d->length = buf.d->length;
        d->autoFree = buf.d->autoFree;

        buf.d->buffer = buffer;
        buf.d->capacity = capacity;
        buf.d->length = length;
        buf.d->autoFree = autoFree;
    }

public:
    /**
     * @brief change the internal buffer, usually for internal use only
     *
     * note the length <= capacity, the actual buffer capacity must at least (capacity + sizeof(zfchar))
     */
    zffinal void zfunsafe_bufferChange(
            ZF_IN void *buffer
            , ZF_IN zfindex capacity
            , ZF_IN zfindex length
            , ZF_IN zfbool autoFree
            ) {
        this->bufferFree();
        d->buffer = buffer;
        d->capacity = capacity;
        d->length = length;
        d->autoFree = autoFree;
    }
    /**
     * @brief util method to malloc and copy buffer, ensured NULL terminated (not included in length)
     * @note length must explicitly specified
     */
    zffinal void bufferCopy(
            ZF_IN const void *buffer
            , ZF_IN zfindex length
            ) {
        if(length == zfindexMax()) {
            length = zfslen((const zfchar *)buffer) * sizeof(zfchar);
        }
        this->capacity(length);
        if(length > 0) {
            zfmemcpy(this->buffer(), buffer, length);
            *((zfchar *)((zfbyte *)this->buffer() + length)) = '\0';
        }
        d->length = length;
    }
    /**
     * @brief util method to copy string
     */
    zffinal void bufferCopy(
            ZF_IN const zfchar *s
            , ZF_IN_OPT zfindex length = zfindexMax()
            ) {
        this->bufferCopy((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to copy string
     */
    zffinal void bufferCopy(ZF_IN const zfstring &s) {
        this->bufferCopy(s.cString(), s.length());
    }

    /**
     * @brief util method to realloc and append buffer, ensured NULL terminated (not included in length)
     */
    zffinal void append(
            ZF_IN const void *buffer
            , ZF_IN zfindex length
            ) {
        if(length == zfindexMax()) {
            length = zfslen((const zfchar *)buffer) * sizeof(zfchar);
        }
        if(length > 0) {
            this->capacity(this->length() + length);
            zfmemcpy((zfbyte *)this->buffer() + this->length(), buffer, length);
            *((zfchar *)((zfbyte *)this->buffer() + this->length() + length)) = '\0';
            d->length += length;
        }
    }
    /**
     * @brief util method to append string
     */
    zffinal void append(
            ZF_IN const zfchar *s
            , ZF_IN_OPT zfindex length = zfindexMax()
            ) {
        this->append((const void *)s, ((length == zfindexMax()) ? zfslen(s) : length) * sizeof(zfchar));
    }
    /**
     * @brief util method to append string
     */
    zffinal void append(ZF_IN const zfstring &s) {
        this->append(s.cString(), s.length());
    }

public:
    /**
     * @brief change buffer capacity
     *
     * this method do nothing if memory allocate fail\n
     * when success, for convenient, it's ensured the buffer can hold bytes up to
     * (buffer.capacity() + sizeof(zfchar))
     */
    zffinal void capacity(ZF_IN zfindex capacity);
    /**
     * @brief return buffer capacity
     */
    zffinal zfindex capacity(void) const {
        return d->capacity;
    }
    /**
     * @brief trim buffer capacity
     */
    zffinal void capacityTrim(void);
    /**
     * @brief change #length only, no memory change would be applied
     *
     * new size must be smaller than current #capacity\n
     * the length only stored for convenient,
     * the actual meaning depends on its user
     */
    zffinal void length(ZF_IN zfindex length) {
        if(length <= d->capacity) {
            d->length = length;
        }
    }
    /**
     * @brief return buffer content's size
     */
    zffinal zfindex length(void) const {
        return d->length;
    }

public:
    /**
     * @brief access the buffer
     */
    zffinal void *buffer(void) {
        return d->buffer;
    }
    /**
     * @brief access the buffer
     */
    zffinal const void *buffer(void) const {
        return d->buffer;
    }
    /**
     * @brief util method to access buffer as string type
     */
    zffinal zfchar *text(void) {
        return this->bufferT<zfchar *>();
    }
    /**
     * @brief util method to access buffer as string type
     */
    zffinal const zfchar *text(void) const {
        return this->bufferT<const zfchar *>();
    }
    /**
     * @brief util method to access buffer as string type
     */
    zfindex textLength(void) const {
        return this->length() / sizeof(zfchar);
    }
    /**
     * @brief access the buffer
     */
    template<typename T_Type>
    T_Type bufferT(void) {
        return (T_Type)this->buffer();
    }
    /**
     * @brief access the buffer
     */
    template<typename T_Type>
    T_Type bufferT(void) const {
        return (T_Type)this->buffer();
    }
    /**
     * @brief whether the buffer would be free-ed automatically
     */
    zffinal zfbool autoFree(void) const {
        return d->autoFree;
    }

public:
    /**
     * @brief construct an empty map
     */
    ZFBuffer(void) {
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
    zffinal ZFBuffer &operator = (ZF_IN const ZFBuffer &ref) {
        _ZFP_ZFBufferPrivate *dTmp = d;
        d = ref.d;
        ++(ref.d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfdelete(dTmp);
        }
        return *this;
    }
    zffinal zfbool operator == (ZF_IN const ZFBuffer &ref) const {
        return (d == ref.d);
    }
    inline zfbool operator != (ZF_IN const ZFBuffer &ref) const {return !this->operator == (ref);}
    /** @endcond */
    ~ZFBuffer(void) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
    }

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const {
        return d->refCount;
    }

private:
    _ZFP_ZFBufferPrivate *d;
private:
    void _capacityDoChange(ZF_IN zfindex capacity);
};
ZFOUTPUT_TYPE(ZFBuffer, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFBuffer_h_

