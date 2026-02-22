/**
 * @file ZFCoreArray.h
 * @brief light weight array
 */

#ifndef _ZFI_ZFCoreArray_h_
#define _ZFI_ZFCoreArray_h_

#include "ZFCoreLog_CommonLog.h"
#include "ZFComparer.h"
#include "ZFCoreUtilMath.h"
#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Element, bool isPOD = zftIsPOD<T_Element>::Value>
zfclassNotPOD _ZFP_ZFCoreArrayW {
public:
    static void objCreate(
        ZF_IN T_Element *p
        , ZF_IN T_Element *pEnd
        ) {
        while(p != pEnd) {
            zfnewPlacement(p, T_Element);
            ++p;
        }
    }
    static void objCreate(
        ZF_IN T_Element *p
        , ZF_IN T_Element *pEnd
        , ZF_IN const T_Element *src
        ) {
        while(p != pEnd) {
            zfnewPlacement(p, T_Element, *src);
            ++p;
            ++src;
        }
    }
    static void objMove(
            ZF_IN T_Element *dst
            , ZF_IN const T_Element *src
            , ZF_IN zfindex count
            ) {
        zfmemmoveObject(dst, src, count);
    }
    static void objDestroy(
            ZF_IN T_Element *p
            , ZF_IN T_Element *pEnd
            ) {
        while(p != pEnd) {
            zfdeletePlacement(p);
            ++p;
        }
    }
};
template<typename T_Element>
zfclassNotPOD _ZFP_ZFCoreArrayW<T_Element, true> {
public:
    static void objCreate(
        ZF_IN T_Element *p
        , ZF_IN T_Element *pEnd
        ) {
    }
    static void objCreate(
        ZF_IN T_Element *p
        , ZF_IN T_Element *pEnd
        , ZF_IN const T_Element *src
        ) {
        zfmemcpy(p, src, (pEnd - p) * sizeof(T_Element));
    }
    static void objMove(
            ZF_IN T_Element *dst
            , ZF_IN const T_Element *src
            , ZF_IN zfindex count
            ) {
        zfmemmove(dst, src, count * sizeof(T_Element));
    }
    static void objDestroy(
            ZF_IN T_Element *p
            , ZF_IN T_Element *pEnd
            ) {
    }
};

template<typename T_Element>
zffinal zfclassNotPOD _ZFP_ZFCoreArrayPrivate {
public:
    zfuint refCount;
    T_Element *buf;
    zfuint capacity;
    zfuint count;
public:
    _ZFP_ZFCoreArrayPrivate(void)
    : refCount(1)
    , buf(zfnull)
    , capacity(0)
    , count(0)
    {
    }
    ~_ZFP_ZFCoreArrayPrivate(void) {
        _ZFP_ZFCoreArrayW<T_Element>::objDestroy(this->buf, this->buf + this->count);
        zffree(this->buf);
    }
};

template<int sizeFix>
zfclassNotPOD _ZFP_ZFCoreArrayCapacity {
public:
    // for size <= sizeof(void *)
    static void capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity == 0) {
        }
        else if(capacity < 64) {
            capacity = ((capacity / 16) + 1) * 16;
        }
        else if(capacity < 256) {
            capacity = ((capacity / 64) + 1) * 64;
        }
        else {
            capacity = ((capacity / 256) + 1) * 256;
        }
    }
};
template<>
zfclassNotPOD _ZFP_ZFCoreArrayCapacity<1> {
public:
    // for size <= 4 * sizeof(void *)
    static void capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity == 0) {
        }
        else if(capacity < 32) {
            capacity = ((capacity / 8) + 1) * 8;
        }
        else if(capacity < 128) {
            capacity = ((capacity / 32) + 1) * 32;
        }
        else {
            capacity = ((capacity / 64) + 1) * 64;
        }
    }
};
template<>
zfclassNotPOD _ZFP_ZFCoreArrayCapacity<2> {
public:
    // for size > 4 * sizeof(void *)
    static void capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity == 0) {
        }
        else if(capacity < 32) {
            capacity = ((capacity / 4) + 1) * 4;
        }
        else {
            capacity = ((capacity / 8) + 1) * 8;
        }
    }
};

// ============================================================
/**
 * @brief dummy base for #ZFCoreArray
 */
zfclassLikePOD ZFLIB_ZFCore ZFCoreArrayBase {
public:
    virtual ~ZFCoreArrayBase(void) {}
    /**
     * @brief new reference
     */
    virtual ZFCoreArrayBase *refNew(void) const zfpurevirtual;
    /**
     * @brief delete reference
     */
    virtual void refDelete(void) {
        zfpoolDelete(this);
    }
    /**
     * @brief get the impl
     */
    virtual void *refImpl(void) const zfpurevirtual;

    /** @brief see #objectInfo */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const {
        this->objectInfoOfContentT(ret, 10);
    }
    /** @brief return object info */
    virtual zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    /** @brief return content info */
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) const zfpurevirtual;
    /** @brief return content info */
    virtual zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) const  {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

    /** @cond ZFPrivateDoc */
    virtual ZFCoreArrayBase &operator = (ZF_IN const ZFCoreArrayBase &ref) zfpurevirtual;
    virtual zfbool operator == (ZF_IN const ZFCoreArrayBase &ref) const zfpurevirtual;
    virtual zfbool operator != (ZF_IN const ZFCoreArrayBase &ref) const zfpurevirtual;
    /** @endcond */

public:
    /**
     * @brief change capacity to hold at least newCapacity
     *
     * do nothing if newCapacity not changed or less than current capacity
     */
    virtual void capacity(ZF_IN zfindex newCapacity) zfpurevirtual;
    /**
     * @brief trim current capacity
     *
     * do nothing if not necessary to trim
     */
    virtual void capacityTrim(void) zfpurevirtual;
    /**
     * @brief get capacity
     */
    virtual zfindex capacity(void) const zfpurevirtual;

    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    virtual void remove(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    virtual void remove(
            ZF_IN zfindex index
            , ZF_IN zfindex count
            ) zfpurevirtual;
    /**
     * @brief remove first, assert fail if out of range
     */
    virtual void removeFirst(void) {
        this->remove(0);
    }
    /**
     * @brief remove last, assert fail if out of range
     */
    virtual void removeLast(void) {
        this->remove(this->count() - 1);
    }
    /**
     * @brief remove all content
     */
    virtual void removeAll(void) {
        if(!this->isEmpty()) {
            this->remove(0, this->count());
        }
    }
    /**
     * @brief move element
     */
    virtual void move(
            ZF_IN zfindex fromIndex
            , ZF_IN zfindex toIndexOrIndexMax
            ) zfpurevirtual;
    /**
     * @brief element count of this array
     */
    virtual zfindex count(void) const zfpurevirtual;
    /**
     * @brief true if empty
     */
    virtual zfbool isEmpty(void) const {
        return this->count() != 0;
    }
    /**
     * @brief sort
     */
    virtual void sort(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) zfpurevirtual;
    /**
     * @brief sort reversely
     */
    virtual void sortReversely(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) zfpurevirtual;

public:
    /** @brief generic version */
    virtual void genericSwap(ZF_IN_OUT ZFCoreArrayBase &ref) zfpurevirtual;
    /** @brief generic version */
    virtual void genericCopyFrom(ZF_IN const ZFCoreArrayBase &ref) zfpurevirtual;
    /** @brief generic version */
    virtual zfindex genericFind(ZF_IN const void *e) zfpurevirtual;
    /** @brief generic version */
    virtual zfindex genericFindReversely(ZF_IN const void *e) zfpurevirtual;
    /** @brief generic version */
    virtual void genericAdd(ZF_IN const void *e) {
        this->genericAdd(e, zfindexMax());
    }
    /** @brief generic version */
    virtual void genericAdd(
            ZF_IN const void *e
            , ZF_IN zfindex index
            ) zfpurevirtual;
    /** @brief generic version */
    virtual void genericAddFrom(ZF_IN const ZFCoreArrayBase &ref) zfpurevirtual;
    /** @brief generic version */
    virtual void genericSet(
            ZF_IN zfindex index
            , ZF_IN const void *e
            ) zfpurevirtual;
    /** @brief generic version */
    virtual const void *genericGet(ZF_IN zfindex index) const zfpurevirtual;
};
ZFOUTPUT_TYPE(ZFCoreArrayBase, {v.objectInfoT(s);})

// ============================================================
/**
 * @brief light weight array
 *
 * @warning element must support:
 *   constructor with no argument,
 *   copy constructor to create from existing value,
 *   operator = to copy value
 *
 * REMARKS:
 * -  ZFCoreArray is designed to reduce dependency of STL or other third-party containers,
 *   aimed for light weight
 * -  use other containers such as ZFArray in your app,
 *   if performance is really a matter,
 *   choose any other containers you like,
 *   but try not to export them
 */
template<typename T_Element>
zffinal zfclassLikePOD ZFCoreArray : zfextend ZFCoreArrayBase {
public:
    /**
     * @brief main constructor
     */
    ZFCoreArray(void) : d(zfnull) {}
    /**
     * @brief construct from another array
     */
    ZFCoreArray(ZF_IN const ZFCoreArray<T_Element> &ref)
    : d(ref.d)
    {
        if(d) {
            ++(d->refCount);
        }
    }
    virtual ~ZFCoreArray(void) {
        if(d) {
            --(d->refCount);
            if(d->refCount == 0) {
                zfpoolDelete(d);
            }
        }
    }
    zfoverride
    virtual ZFCoreArrayBase *refNew(void) const {return zfpoolNew(ZFCoreArray<T_Element>, *this);}
    zfoverride
    virtual void *refImpl(void) const {return d;}
    /**
     * @brief retain the array, if you want to copy, use #copyFrom instead
     */
    ZFCoreArray<T_Element> &operator = (ZF_IN const ZFCoreArray<T_Element> &ref) {
        _ZFP_ZFCoreArrayPrivate<T_Element> *dTmp = d;
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp) {
            --(dTmp->refCount);
            if(dTmp->refCount == 0) {
                zfpoolDelete(dTmp);
            }
        }
        return *this;
    }
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFCoreArray<T_Element> &ref) const {return (d == ref.d);}
    inline zfbool operator != (ZF_IN const ZFCoreArray<T_Element> &ref) const {return !this->operator == (ref);}
    zfoverride
    virtual ZFCoreArrayBase &operator = (ZF_IN const ZFCoreArrayBase &ref) {
        return this->operator = ((const ZFCoreArray<T_Element> &)ref);
    }
    virtual zfbool operator == (ZF_IN const ZFCoreArrayBase &ref) const {
        return this->operator == ((const ZFCoreArray<T_Element> &)ref);
    }
    virtual zfbool operator != (ZF_IN const ZFCoreArrayBase &ref) const {
        return this->operator == ((const ZFCoreArray<T_Element> &)ref);
    }
    /** @endcond */

public:
    /**
     * @brief swap internal data
     */
    void swap(ZF_IN_OUT ZFCoreArray<T_Element> &ref) {
        _ZFP_ZFCoreArrayPrivate<T_Element> *dTmp = d;
        d = ref.d;
        ref.d = dTmp;
    }

    /**
     * @brief copy all settings and contents from another array
     */
    void copyFrom(ZF_IN const ZFCoreArray<T_Element> &ref) {
        if(d != ref.d) {
            if(d && d->buf) {
                _ZFP_ZFCoreArrayW<T_Element>::objDestroy(d->buf, d->buf + d->count);
                d->count = 0;
            }
            if(ref.d) {
                _capacityRequire(ref.count());
                _ZFP_ZFCoreArrayW<T_Element>::objCreate(d->buf, d->buf + ref.count(), ref.arrayBuf());
                d->count = (zfuint)ref.count();
            }
        }
    }
    /**
     * @brief get retain count
     */
    zfindex objectRetainCount(void) const {return d ? d->refCount : 0;}
    /**
     * @brief compare by instance
     */
    ZFCompareResult objectCompare(ZF_IN const ZFCoreArray<T_Element> &ref) const {
        return d == ref.d ? ZFCompareEqual : ZFCompareUncomparable;
    }
    /**
     * @brief compare by content
     */
    ZFCompareResult objectCompareValue(
            ZF_IN const ZFCoreArray<T_Element> &ref
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) const {
        if(d == ref.d) {
            return ZFCompareEqual;
        }
        if(this->count() != ref.count()) {
            return ZFCompareUncomparable;
        }
        for(zfindex i = this->count() - 1; i != zfindexMax(); --i) {
            if(comparer(this->get(i), ref.get(i)) != ZFCompareEqual) {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareEqual;
    }

public:
    zfoverride
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) const {
        this->objectInfoOfContentT(ret, maxCount, token, zfnull);
    }
    zfoverride
    virtual zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            ) const {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token, zfnull);
        return ret;
    }

    /** @brief see #objectInfoOfContent */
    void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount
            , ZF_IN_OPT const ZFTokenForContainer &token
            , ZF_IN_OPT typename ZFCoreInfoGetter<T_Element>::InfoGetter infoGetter
            ) const {
        zfindex count = 0;
        ret += token.tokenLeft;
        for(; count < this->count() && count < maxCount; ++count) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }
            ret += token.tokenValueLeft;
            if(infoGetter != zfnull) {
                infoGetter(ret, this->get(count));
            }
            else {
                zftToStringT(ret, this->get(count));
            }
            ret += token.tokenValueRight;
        }
        if(count < this->count()) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }
            ret += token.tokenEtc;
        }
        ret += token.tokenRight;
    }
    /** @brief return content info */
    zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount
            , ZF_IN_OPT const ZFTokenForContainer &token
            , ZF_IN_OPT typename ZFCoreInfoGetter<T_Element>::InfoGetter infoGetter
            ) const {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token, infoGetter);
        return ret;
    }

public:
    zfoverride
    virtual void capacity(ZF_IN zfindex newCapacity) {
        _capacityRequire(newCapacity);
    }
    zfoverride
    virtual void capacityTrim(void) {
        zfindex capacity = this->count();
        _capacityOptimize(capacity);
        if(capacity != this->capacity()) {
            _capacityDoChange(capacity);
        }
    }
    zfoverride
    virtual zfindex capacity(void) const {
        return (zfindex)(d ? d->capacity : 0);
    }

public:
    /**
     * @brief add element
     */
    void add(ZF_IN T_Element const &e) {
        _capacityRequire(this->count() + 1);
        _ZFP_ZFCoreArrayW<T_Element>::objCreate(d->buf + d->count, d->buf + d->count + 1, &e);
        ++(d->count);
    }
    /**
     * @brief add element at index
     */
    void add(
            ZF_IN T_Element const &e
            , ZF_IN zfindex index
            ) {
        if(index == zfindexMax()) {
            index = this->count();
        }
        else {
            ZFCoreAssertIndexRange(index, this->count() + 1);
        }
        _capacityRequire(this->count() + 1);
        _ZFP_ZFCoreArrayW<T_Element>::objCreate(d->buf + d->count, d->buf + d->count + 1);
        T_Element *pos = d->buf + index;
        _ZFP_ZFCoreArrayW<T_Element>::objMove(pos + 1, pos, this->count() - index);
        ++(d->count);
        *pos = e;
    }
    /**
     * @brief add elements, src can be part of this array's buffer
     */
    void addFrom(
            ZF_IN const T_Element *src
            , ZF_IN zfindex count
            ) {
        if(src == zfnull || count == 0) {
            return;
        }
        if(d == zfnull || src < d->buf || src >= d->buf + d->capacity) {
            _capacityRequire(this->count() + count);
            _ZFP_ZFCoreArrayW<T_Element>::objCreate(d->buf + d->count, d->buf + d->count + count, src);
            d->count += (zfuint)count;
        }
        else {
            ZFCoreArray<T_Element> tmp;
            tmp.capacity(count);
            tmp.addFrom(src, count);
            this->addFrom(tmp.arrayBuf(), count);
        }
    }

    /**
     * @brief add from another array
     */
    void addFrom(ZF_IN const ZFCoreArray<T_Element> &ref) {
        if(d != ref.d) {
            this->addFrom(ref.arrayBuf(), ref.count());
        }
    }

    /**
     * @brief find element
     */
    zfindex find(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) const {
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element reversely
     */
    zfindex findReversely(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) const {
        if(d && d->buf) {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element
     */
    template<typename T_Another>
    zfindex find(
            ZF_IN T_Another const &e
            , ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer
            ) const {
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element reversely
     */
    template<typename T_Another>
    zfindex findReversely(
            ZF_IN T_Another const &e
            , ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer
            ) const {
        if(d && d->buf) {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }

    /**
     * @brief remove first matched element, return whether the element removed
     */
    zfbool removeElement(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) {
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove first matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElement(
            ZF_IN T_Another const &e
            , ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer
            ) {
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove last matched element, return whether the element removed
     */
    zfbool removeElementReversely(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) {
        if(d && d->buf) {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove last matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElementReversely(
            ZF_IN T_Another const &e
            , ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer
            ) {
        if(d && d->buf) {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove all matched element, return number of removed element
     */
    zfindex removeElementAll(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) {
        zfindex removedCount = 0;
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    ++removedCount;
                    this->remove(p - d->buf);
                    --p;
                }
            }
        }
        return removedCount;
    }
    /**
     * @brief remove all matched element, return number of removed element
     */
    template<typename T_Another>
    zfindex removeElementAll(
            ZF_IN T_Another const &e
            , ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer
            ) {
        zfindex removedCount = 0;
        if(d) {
            for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p) {
                if(comparer(*p, e) == ZFCompareEqual) {
                    ++removedCount;
                    this->remove(p - d->buf);
                    --p;
                }
            }
        }
        return removedCount;
    }

    zfoverride
    virtual void remove(ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, this->count());
        _ZFP_ZFCoreArrayW<T_Element>::objMove(d->buf + index, d->buf + index + 1, this->count() - index - 1);
        _ZFP_ZFCoreArrayW<T_Element>::objDestroy(d->buf + d->count - 1, d->buf + d->count);
        --(d->count);
    }
    zfoverride
    virtual void remove(
            ZF_IN zfindex index
            , ZF_IN zfindex count
            ) {
        ZFCoreAssertIndexRange(index, this->count());
        if(count > this->count() - index) {
            count = this->count() - index;
        }
        _ZFP_ZFCoreArrayW<T_Element>::objMove(d->buf + index, d->buf + index + count, this->count() - (index + count));
        _ZFP_ZFCoreArrayW<T_Element>::objDestroy(d->buf + d->count - count, d->buf + d->count);
        d->count -= (zfuint)count;
    }
    /**
     * @brief remove and return the removed value
     */
    T_Element removeAndGet(ZF_IN zfindex index) {
        T_Element t = this->get(index);
        this->remove(index);
        return t;
    }
    /**
     * @brief remove first and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeFirstAndGet(void) {
        T_Element t = this->getFirst();
        this->removeFirst();
        return t;
    }
    /**
     * @brief remove last and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeLastAndGet(void) {
        T_Element t = this->getLast();
        this->removeLast();
        return t;
    }

    zfoverride
    virtual void move(
            ZF_IN zfindex fromIndex
            , ZF_IN zfindex toIndexOrIndexMax
            ) {
        ZFCoreAssertIndexRange(fromIndex, this->count());
        if(toIndexOrIndexMax == zfindexMax()) {
            toIndexOrIndexMax = this->count() - 1;
        }
        else {
            ZFCoreAssertIndexRange(toIndexOrIndexMax, this->count());
        }
        if(fromIndex == toIndexOrIndexMax) {
            return;
        }
        T_Element t = d->buf[fromIndex];
        if(fromIndex < toIndexOrIndexMax) {
            _ZFP_ZFCoreArrayW<T_Element>::objMove(d->buf + fromIndex, d->buf + fromIndex + 1, toIndexOrIndexMax - fromIndex);
        }
        else {
            _ZFP_ZFCoreArrayW<T_Element>::objMove(d->buf + toIndexOrIndexMax + 1, d->buf + toIndexOrIndexMax, fromIndex - toIndexOrIndexMax);
        }
        d->buf[toIndexOrIndexMax] = t;
    }

public:
    /**
     * @brief set element at index, or assert fail if index out of range
     */
    void set(
            ZF_IN zfindex index
            , ZF_IN T_Element const &e
            ) {
        ZFCoreAssertIndexRange(index, this->count());
        d->buf[index] = e;
    }

public:
    /**
     * @brief get element's reference at index
     */
    T_Element &get(ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, this->count());
        return d->buf[index];
    }
    /**
     * @brief get element's const reference at index
     */
    T_Element const &get(ZF_IN zfindex index) const {
        ZFCoreAssertIndexRange(index, this->count());
        return d->buf[index];
    }
    /**
     * @brief get element's reference at index
     */
    T_Element &operator [] (ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, this->count());
        return d->buf[index];
    }
    /**
     * @brief get element's const reference at index
     */
    T_Element const &operator [] (ZF_IN zfindex index) const {
        ZFCoreAssertIndexRange(index, this->count());
        return d->buf[index];
    }
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getFirst(void) const {
        ZFCoreAssertIndexRange(0, this->count());
        return *(d->buf);
    }
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getLast(void) const {
        ZFCoreAssertIndexRange(0, this->count());
        return *(d->buf + d->count - 1);
    }

    /**
     * @brief directly access the array
     *
     * @warning the returned pointer may changed after modify the array
     * @warning return null if current capacity is 0
     */
    T_Element *arrayBuf(void) {return d ? d->buf : zfnull;}
    /**
     * @brief see #arrayBuf
     */
    const T_Element *arrayBuf(void) const {return d ? d->buf : zfnull;}

    zfoverride
    virtual zfindex count(void) const {return (zfindex)(d ? d->count : 0);}
    zfoverride
    virtual zfbool isEmpty(void) const {return (d == zfnull || d->count == 0);}
    /** @brief true if contains element */
    virtual zfbool isContain(
            ZF_IN T_Element const &e
            , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
            ) const {
        return this->find(e, comparer) != zfindexMax();
    }

public:
    zfoverride
    virtual void sort(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        this->sort(start, count, ZFComparerDefault);
    }
    zfoverride
    virtual void sortReversely(
            ZF_IN_OPT zfindex start = 0
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        this->sortReversely(start, count, ZFComparerDefault);
    }
    /**
     * @brief sort element
     */
    void sort(
            ZF_IN zfindex start
            , ZF_IN zfindex count
            , ZF_IN typename ZFComparer<T_Element>::Comparer comparer
            ) {
        if(!this->isEmpty() && start + 1 < this->count() && count > 1) {
            zfmSort<T_Element>(
                    d->buf
                    , start
                    , (count > this->count() - start) ? (this->count() - 1) : (start + count - 1)
                    , comparer
                    );
        }
    }
    /**
     * @brief sort element
     */
    void sortReversely(
            ZF_IN zfindex start
            , ZF_IN zfindex count
            , ZF_IN typename ZFComparer<T_Element>::Comparer comparer
            ) {
        if(!this->isEmpty() && start + 1 < this->count() && count > 1) {
            zfmSortReversely<T_Element>(
                    d->buf
                    , start
                    , (count > this->count() - start) ? (this->count() - 1) : (start + count - 1)
                    , comparer
                    );
        }
    }

public:
    zfoverride
    virtual void genericSwap(ZF_IN_OUT ZFCoreArrayBase &ref) {this->swap((ZFCoreArray<T_Element> &)ref);}
    zfoverride
    virtual void genericCopyFrom(ZF_IN const ZFCoreArrayBase &ref) {this->copyFrom((const ZFCoreArray<T_Element> &)ref);}
    zfoverride
    virtual zfindex genericFind(ZF_IN const void *e) {return this->find(*(const T_Element *)e);}
    zfoverride
    virtual zfindex genericFindReversely(ZF_IN const void *e) {return this->findReversely(*(const T_Element *)e);}
    zfoverride
    virtual void genericAdd(
            ZF_IN const void *e
            , ZF_IN zfindex index
            ) {this->add(*(const T_Element *)e, index);}
    zfoverride
    virtual void genericAddFrom(ZF_IN const ZFCoreArrayBase &ref) {this->addFrom((const ZFCoreArray<T_Element> &)ref);}
    zfoverride
    virtual void genericSet(
            ZF_IN zfindex index
            , ZF_IN const void *e
            ) {this->set(index, *(const T_Element *)e);}
    zfoverride
    virtual const void *genericGet(ZF_IN zfindex index) const {return &(this->get(index));}

private:
    _ZFP_ZFCoreArrayPrivate<T_Element> *d;
private:
    inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        _ZFP_ZFCoreArrayCapacity<
                sizeof(T_Element) <= sizeof(void *)
                ? 0
                : (sizeof(T_Element) <= 4 * sizeof(void *)
                    ? 1
                    : 2
                )
            >::capacityOptimize(capacity);
    }
    inline void _capacityRequire(ZF_IN zfindex capacity) {
        _capacityOptimize(capacity);
        if(capacity > this->capacity()) {
            _capacityDoChange(capacity);
        }
    }
    void _capacityDoChange(ZF_IN zfindex capacity) {
        if(capacity == 0) {
            if(d) {
                _ZFP_ZFCoreArrayW<T_Element>::objDestroy(d->buf, d->buf + d->count);
                zffree(d->buf);
                d->buf = zfnull;
                d->capacity = 0;
                d->count = 0;
            }
        }
        else {
            if(d == zfnull) {
                d = zfpoolNew(_ZFP_ZFCoreArrayPrivate<T_Element>);
            }

            T_Element *oldBuf = d->buf;
            zfuint oldCount = d->count;

            T_Element *newBuf = (T_Element *)zfmalloc(capacity * sizeof(T_Element));
            _ZFP_ZFCoreArrayW<T_Element>::objCreate(newBuf, newBuf + oldCount, oldBuf);

            d->buf = newBuf;
            d->capacity = (zfuint)capacity;
            d->count = oldCount;

            _ZFP_ZFCoreArrayW<T_Element>::objDestroy(oldBuf, oldBuf + oldCount);
            zffree(oldBuf);
        }
    }
};
ZFOUTPUT_TYPE_TEMPLATE(typename T_Element, ZFCoreArray<T_Element>, {v.objectInfoT(s);})

template<typename T_Element>
zfclassLikePOD _ZFP_ZFCoreArrayCreate {
public:
    inline _ZFP_ZFCoreArrayCreate<T_Element> &add(ZF_IN T_Element const &v) {
        this->v.add(v);
        return *this;
    }

public:
    ZFCoreArray<T_Element> v;
};
#define _ZFP_ZFCoreArrayCreate_action_expand(value) .add(value)
#define _ZFP_ZFCoreArrayCreate_action(CreatorType, values, ...) \
    CreatorType() ZFM_FIX_PARAM(_ZFP_ZFCoreArrayCreate_action_expand, ZFM_EMPTY, values, ##__VA_ARGS__) .v
/**
 * @brief util macro to create a temp array for short
 *
 * usage:
 * @code
 *   ZFCoreArray<YourType> array = ZFCoreArrayCreate(YourType, v0, v1, ...); // any element count
 * @endcode
 */
#define ZFCoreArrayCreate(ElementType, values, ...) _ZFP_ZFCoreArrayCreate_action(_ZFP_ZFCoreArrayCreate<ElementType>, values, ##__VA_ARGS__)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArray_h_

