/**
 * @file zfany.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_zfany_h_
#define _ZFI_zfany_h_

#include "zfcast.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;

// ============================================================
// zfany
/**
 * @brief util method to cast ZFObject types freely
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   zfany any = obj;
 *
 *   // cast to any other object type, return null if not able to cast
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 *
 * note: zfany stores raw ZFObject pointer,
 * use #zfauto or #zfweak for most case,
 * use zfany only if necessary
 */
zfclassPOD ZFLIB_ZFCore zfany {
    /** @cond ZFPrivateDoc */
public:
    zfany(void) : _ZFP_obj(zfnull) {}
    zfany(ZF_IN zfany const &obj) : _ZFP_obj(obj._ZFP_obj) {}
    zfany(ZF_IN ZFObject *obj) : _ZFP_obj(obj) {}
    template<typename T_ZFObject>
    zfany(ZF_IN T_ZFObject const &obj) : _ZFP_obj(_ZFP_zfanyCast(obj)) {}

public:
    inline zfany &operator = (ZF_IN zfany const &obj) {
        _ZFP_obj = obj._ZFP_obj;
        return *this;
    }
    inline zfany &operator = (ZF_IN ZFObject *obj) {
        _ZFP_obj = obj;
        return *this;
    }
    template<typename T_ZFObject>
    inline zfany &operator = (ZF_IN T_ZFObject const &obj) {
        _ZFP_obj = _ZFP_zfanyCast(obj);
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
    inline ZFObject *operator -> (void) const {
        return _ZFP_obj;
    }
    inline operator zfbool (void) const {
        return _ZFP_obj != zfnull;
    }
    inline operator ZFObject * (void) const {
        return _ZFP_obj;
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return zfcast(T_ZFObject *, _ZFP_obj);
    }

public:
    static const ZFClass *ClassData(void);
    /** @endcond */

public:
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
    inline T_ZFObject to(void) const {
        return zfcast(T_ZFObject, _ZFP_obj);
    }

private:
    ZFObject *_ZFP_obj;
};
ZFCORE_POD_DECLARE_NO_COMPARER(zfany)

// ============================================================
/**
 * @brief see #zfany
 */
template<typename T_ZFObjectBase>
zfclassPOD zfanyT : zfextend zfany {
    /** @cond ZFPrivateDoc */
public:
    zfanyT(void) : zfany() {}
    zfanyT(ZF_IN zfany const &obj) : zfany(obj) {}
    zfanyT(ZF_IN zfanyT<T_ZFObjectBase> const &obj) : zfany((zfany const &)obj) {}
    zfanyT(ZF_IN ZFObject *obj) : zfany(obj) {}
    template<typename T_ZFObject>
    zfanyT(ZF_IN T_ZFObject const &obj) : zfany(obj) {}

public:
    inline zfanyT<T_ZFObjectBase> &operator = (ZF_IN zfany const &obj) {
        zfany::operator = (obj);
        return *this;
    }
    inline zfanyT<T_ZFObjectBase> &operator = (ZF_IN zfanyT<T_ZFObjectBase> const &obj) {
        zfany::operator = ((zfany const &)obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfanyT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj) {
        zfany::operator = (obj);
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
    inline T_ZFObjectBase *operator -> (void) const {
        return zfcast(T_ZFObjectBase *, this->toObject());
    }

public:
    inline ZFObject *toObject(void) const { // required for _ZFP_zfanyCast to work
        return zfany::toObject();
    }

public:
    static const ZFClass *ClassData(void) {
        return T_ZFObjectBase::ClassData();
    }
    /** @endcond */
};
ZFCORE_POD_DECLARE_TEMPLATE_NO_COMPARER(typename T_Type, zfanyT<T_Type>)

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfany const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfany const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfanyT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfanyT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfany_h_

