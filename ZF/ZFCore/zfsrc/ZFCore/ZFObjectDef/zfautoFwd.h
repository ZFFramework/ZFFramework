/**
 * @file zfautoFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoFwd_h_
#define _ZFI_zfautoFwd_h_

#include "ZFAny.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfauto
zfclassNotPOD ZFLIB_ZFCore _ZFP_zfautoPrivate {
public:
    zfuint refCount;
    ZFAny obj;
public:
    _ZFP_zfautoPrivate(void) : refCount(1), obj(zfnull) {}
    _ZFP_zfautoPrivate(ZF_IN ZFObject *obj) : refCount(1), obj(obj) {}
};

/**
 * @brief a ZFObject holder which would release content object automatically when destroyed
 *
 * useful to hold a ZFObject as temp return value:
 * @code
 *   zfauto yourFunc(void) {
 *       // alloc an object for return
 *       zfobj<ZFObject> obj;
 *
 *       // use zfauto to wrap the returned object
 *       return zfauto(obj);
 *   }
 *
 *   {
 *       zfauto value = yourFunc();
 *       ZFObject *obj = value;
 *   } // content obj would be released when zfauto destroyed
 * @endcode
 */
zffinal zfclassLikePOD ZFLIB_ZFCore zfauto {
    /** @cond ZFPrivateDoc */
public:
    zfauto(void) : d(zfnull) {}
    zfauto(ZF_IN zfauto const &obj);
    zfauto(ZF_IN zfnullT const &) : d(zfnull) {}
    template<typename T_ZFObject>
    zfauto(ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfauto(ZF_IN T_ZFObject const &obj);
    zffinal ~zfauto(void);

public:
    zfauto &operator = (ZF_IN zfauto const &obj);
    zfauto &operator = (ZF_IN zfnullT const &);
    template<typename T_ZFObject>
    zfauto &operator = (ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfauto &operator = (ZF_IN T_ZFObject const &obj);

public:
    inline zfbool operator == (ZF_IN zfauto const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfauto const &obj) const {
        return this->toObject() != obj.toObject();
    }
    inline zfbool operator == (ZF_IN zfnullT const &) const {
        return this->toObject() == zfnull;
    }
    inline zfbool operator != (ZF_IN zfnullT const &) const {
        return this->toObject() != zfnull;
    }
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return this->toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return (this->toObject() != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
    }

public:
    const ZFAny &_ZFP_ZFAny(void) const {
        return d->obj;
    }

public:
    static const ZFClass *ClassData(void);
    /** @endcond */

public:
    /**
     * @brief no lock version of object assign, for low level impl only
     */
    void zfunsafe_assign(ZF_IN ZFObject *obj);
    /**
     * @brief no lock version of object assign, for low level impl only
     */
    void zfunsafe_assign(ZF_IN zfauto const &obj);
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const {
        return (zfindex)(d ? d->refCount : 0);
    }
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return (d ? d->obj : zfnull);
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }

private:
    _ZFP_zfautoPrivate *d;
};

// ============================================================
// zfautoT
/**
 * @brief type restrict version of #zfauto
 */
template<typename T_ZFObjectBase>
zffinal zfclassLikePOD zfautoT : zfextend zfauto {
    /** @cond ZFPrivateDoc */
public:
    zfautoT(void) : zfauto() {}
    zfautoT(ZF_IN zfauto const &obj) : zfauto(obj) {}
    zfautoT(ZF_IN zfautoT<T_ZFObjectBase> const &obj) : zfauto((zfauto const &)obj) {}
    zfautoT(ZF_IN zfnullT const &) : zfauto() {}
    template<typename T_ZFObject>
    zfautoT(ZF_IN T_ZFObject *obj) : zfauto(obj) {}
    template<typename T_ZFObject>
    zfautoT(ZF_IN T_ZFObject const &obj) : zfauto(obj) {}

public:
    inline zfbool operator == (ZF_IN zfautoT<T_ZFObjectBase> const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfautoT<T_ZFObjectBase> const &obj) const {
        return this->toObject() != obj.toObject();
    }
    inline zfbool operator == (ZF_IN zfnullT const &) const {
        return this->toObject() == zfnull;
    }
    inline zfbool operator != (ZF_IN zfnullT const &) const {
        return this->toObject() != zfnull;
    }
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return this->toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
    }

public:
    T_ZFObjectBase *operator -> (void) const;
    inline ZFObject *toObject(void) const { // required for _ZFP_ZFAnyCast to work
        return zfauto::toObject();
    }

public:
    static const ZFClass *ClassData(void);
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoFwd_h_

