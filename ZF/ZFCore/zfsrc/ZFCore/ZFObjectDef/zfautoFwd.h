/**
 * @file zfautoFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoFwd_h_
#define _ZFI_zfautoFwd_h_

#include "zfany.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfauto
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
    zfauto(void) : _ZFP_obj(zfnull) {}
    zfauto(ZF_IN zfauto const &obj);
    zfauto(ZF_IN ZFObject *obj);
    template<typename T_ZFObject>
    zfauto(ZF_IN T_ZFObject const &obj);
    zffinal ~zfauto(void);

public:
    zfauto &operator = (ZF_IN zfauto const &obj);
    template<typename T_ZFObject>
    zfauto &operator = (ZF_IN T_ZFObject const &obj);

public:
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() == _ZFP_zfanyCast(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() != _ZFP_zfanyCast(obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return (zfbool)_ZFP_obj;
    }
    inline operator ZFObject * (void) const {
        return this->toObject();
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const;

public:
    static const ZFClass *ClassData(void);
    /** @endcond */

public:
    /**
     * @brief access as #zfany
     */
    const zfany &asAny(void) const {
        return _ZFP_obj;
    }
    /**
     * @brief no lock version of object assign, for low level impl only
     */
    void zfunsafe_assign(ZF_IN ZFObject *obj);
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const;
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return _ZFP_obj;
    }
    /**
     * @brief cast by #zfcast
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const;

private:
    zfany _ZFP_obj;
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
    zfautoT(ZF_IN ZFObject *obj) : zfauto(obj) {}
    template<typename T_ZFObject>
    zfautoT(ZF_IN T_ZFObject const &obj) : zfauto(obj) {}

public:
    inline zfautoT<T_ZFObjectBase> &operator = (ZF_IN zfauto const &obj) {
        zfauto::operator = (obj);
        return *this;
    }
    inline zfautoT<T_ZFObjectBase> &operator = (ZF_IN zfautoT<T_ZFObjectBase> const &obj) {
        zfauto::operator = ((zfauto const &)obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfautoT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj) {
        zfauto::operator = (obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() == _ZFP_zfanyCast(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() != _ZFP_zfanyCast(obj);
    }

public:
    T_ZFObjectBase *operator -> (void) const;

public:
    inline ZFObject *toObject(void) const { // required for _ZFP_zfanyCast to work
        return zfauto::toObject();
    }

public:
    static const ZFClass *ClassData(void);
    /** @endcond */
};

/** @cond ZFPrivateDoc */
// ============================================================
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoFwd_h_

