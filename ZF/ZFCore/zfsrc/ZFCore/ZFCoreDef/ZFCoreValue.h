/**
 * @file ZFCoreValue.h
 * @brief light weight value holder with ref count logic
 */
#ifndef _ZFI_ZFCoreValue_h_
#define _ZFI_ZFCoreValue_h_

#include "ZFCoreTypeDef.h"
#include "ZFMemPool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief light weight value holder with ref count logic
 */
template<typename T_Value>
zfclassLikePOD ZFCoreValue {
public:
    /**
     * @brief value type
     */
    typedef T_Value ValueType;

public:
    /** @brief access the value */
    T_Value &value(void) {
        if(d) {
            return d->v;
        }
        else {
            d = zfpoolNew(D);
            return d->v;
        }
    }
    /** @brief access the value */
    T_Value const &value(void) const {
        if(d) {
            return d->v;
        }
        else {
            static T_Value t;
            return t;
        }
    }
    /** @brief change the value, all reference would also be changed */
    ZFCoreValue<T_Value> &value(ZF_IN T_Value const &v) {
        if(d) {
            d->v = v;
        }
        else {
            d = zfpoolNew(D, v);
        }
        return *this;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFCoreValue(void) : d(zfnull) {}
    ZFCoreValue(ZF_IN T_Value const &v) : d(zfpoolNew(D, v)) {}
    ZFCoreValue(ZF_IN ZFCoreValue<T_Value> const &ref) : d(ref.d) {
        if(ref.d) {
            ++(ref.d->refCount);
        }
    }
    ~ZFCoreValue(void) {
        if(d && --(d->refCount) == 0) {
            zfpoolDelete(d);
        }
    }
    zfbool operator == (ZF_IN ZFCoreValue<T_Value> const &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN ZFCoreValue<T_Value> const &ref) const {return d != ref.d;}
    /** @endcond */

private:
    zfclassNotPOD D {
    public:
        zfuint refCount;
        T_Value v;
    public:
        D(void)
        : refCount(1)
        , v()
        {
        }
        D(ZF_IN T_Value const &v)
        : refCount(1)
        , v(v)
        {
        }
    };
private:
    D *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreValue_h_

