/**
 * @file zfiter.h
 * @brief iterator in ZFFramework
 */

#ifndef _ZFI_zfiter_h_
#define _ZFI_zfiter_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief iterator for ZFFramework
 *
 * zfiter is an abstract iterator holder,
 * which simply stores an void * pointer for implementations
 * to achieve iterator logic\n
 * \n
 * typically, you may use iterators like this:
 * @code
 *   for(zfiter it = someIterable.iter(); it; ++it) {
 *       const SomeType &value = someIterable.iterValue(it);
 *   }
 * @endcode
 * for key-value container,
 * access by iterator should return value,
 * and should supply key-value version of iterator access:
 * @code
 *   for(zfiter it = keyValueIterable.iterFind(key); it; ++it) {
 *       // access key and value
 *       KeyType key = keyValueIterable.iterKey(it);
 *       ValueType value = keyValueIterable.iterValue(it);
 *   }
 * @endcode
 * \n
 * for implementations,
 * you should construct a zfiter with proper zfiter::Impl
 */
zffinal zfclassLikePOD ZFLIB_ZFCore zfiter {

public:
    /** @brief whether valid */
    inline zfbool valid(void) const {
        return d && d->valid();
    }
    /** @brief move to next */
    inline void next(void) const {
        if(d) {
            d->next();
        }
    }
    /** @brief copy */
    inline zfiter copy(void) const {
        return zfiter(d ? d->copy() : zfnull);
    }
    /** @brief compare */
    inline zfbool isEqual(ZF_IN const zfiter &ref) const {
        return this->operator == (ref);
    }
    /** @cond ZFPrivateDoc */
    inline operator zfbool (void) const {
        return this->valid();
    }
    inline zfiter &operator ++ (void) {
        this->next();
        return *this;
    }
    inline zfiter operator ++ (int) {
        if(d) {
            zfiter ret(d->copy());
            d->next();
            return ret;
        }
        else {
            return zfnull;
        }
    }
    /** @endcond */

    // ============================================================
public:
    /** @brief see #zfiter */
    zfclassNotPOD ZFLIB_ZFCore Impl {
    public:
        virtual ~Impl(void) {}
    public:
        /** @brief see #zfiter */
        virtual zfbool valid(void) zfpurevirtual;
        /** @brief see #zfiter */
        virtual void next(void) zfpurevirtual;
        /** @brief see #zfiter */
        virtual Impl *copy(void) zfpurevirtual;
        /** @brief see #zfiter */
        virtual void destroy(void) zfpurevirtual;
        /** @brief see #zfiter */
        virtual zfbool isEqual(ZF_IN Impl *d) zfpurevirtual;
    };

public:
    /**
     * @brief create a dummy iterator
     */
    zfiter(void) : d(zfnull) {}
    /**
     * @brief implementations of iterables must use this to create an iterator,
     *   see #zfiter
     */
    zfiter(ZF_IN Impl *impl) : d(impl) {
    }
    /**
     * @brief implementations may use this method to access data passed from constructor,
     *   see #zfiter
     */
    inline Impl *impl(void) const {
        return d;
    }
    /**
     * @brief implementations may use this method to access data passed from constructor,
     *   see #zfiter
     */
    template<typename T_Impl>
    inline T_Impl impl(void) const {
        return (T_Impl)d;
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
    /** @cond ZFPrivateDoc */
    zfiter(ZF_IN const zfnullT &dummy) : d(zfnull) {}
    zfiter(ZF_IN const zfiter &ref) : d(ref.d ? ref.d->copy() : zfnull) {}
    ~zfiter(void) {
        if(d) {
            d->destroy();
        }
    }
    zfiter &operator = (ZF_IN const zfnullT &ref) {
        if(d) {
            Impl *dOld = d;
            d = zfnull;
            if(dOld) {
                dOld->destroy();
            }
        }
        return *this;
    }
    zfiter &operator = (ZF_IN const zfiter &ref) {
        if(this != &ref) {
            Impl *dOld = d;
            d = (ref.d ? ref.d->copy() : zfnull);
            if(dOld) {
                dOld->destroy();
            }
        }
        return *this;
    }
    inline zfbool operator == (ZF_IN const zfnullT &ref) const {return !this->valid();}
    inline zfbool operator != (ZF_IN const zfnullT &ref) const {return this->valid();}
    zfbool operator == (ZF_IN const zfiter &ref) const {
        if(d == ref.d) {
            return zftrue;
        }
        else if(d == zfnull || !d->valid()) {
            return ref.d == zfnull || !ref.d->valid();
        }
        else {
            return ref.d != zfnull && d->isEqual(ref.d);
        }
    }
    inline zfbool operator != (ZF_IN const zfiter &ref) const {return !this->operator == (ref);}
    /** @endcond */
private:
    Impl *d;
};
ZFOUTPUT_TYPE(zfiter, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfiter_h_

