/**
 * @file zfany.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_zfany_h_
#define _ZFI_zfany_h_

#include "ZFObjectCast.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFObjectHolder;

/**
 * @brief a helper class to convert any #ZFTypeIdWrapper to its original type
 *
 * this class is not intend to be used by user,
 * it's a convenient wrapper for api design,
 * typical usage:
 * @code
 *   zfobj<v_zfstring> obj;
 *   zfstring const &v = obj.zfv();
 *   const zfchar *v2 = (zfstring const &)obj.zfv();
 * @endcode
 */
zfclassNotPOD ZFLIB_ZFCore ZFV {
    /** @cond ZFPrivateDoc */
public:
    ZFV(ZF_IN ZFObject *obj) : _ZFP_obj(obj) {}
public:
    template<typename T_Type>
    operator T_Type &(void) const;
    /** @endcond */
private:
    ZFObject *_ZFP_obj;
};

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
    zfany(ZF_IN zfany const &obj) : _ZFP_obj(obj.toObject()) {}
    zfany(ZF_IN zfnullT const &) : _ZFP_obj(zfnull) {}
    template<typename T_ZFObject>
    zfany(ZF_IN T_ZFObject *obj) : _ZFP_obj(obj ? obj->toObject() : zfnull) {}
    template<typename T_ZFObject>
    zfany(ZF_IN T_ZFObject const &obj) : _ZFP_obj(_ZFP_zfanyCast(T_ZFObject, obj)) {}

public:
    inline zfany &operator = (ZF_IN zfany const &obj) {
        _ZFP_obj = obj.toObject();
        return *this;
    }
    inline zfany &operator = (ZF_IN zfnullT const &) {
        _ZFP_obj = zfnull;
        return *this;
    }
    template<typename T_ZFObject>
    inline zfany &operator = (ZF_IN T_ZFObject *obj) {
        _ZFP_obj = (obj ? obj->toObject() : zfnull);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfany &operator = (ZF_IN T_ZFObject const &obj) {
        _ZFP_obj = _ZFP_zfanyCast(T_ZFObject, obj);
        return *this;
    }

public:
    inline zfbool operator == (ZF_IN zfany const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfany const &obj) const {
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
        return this->toObject() == _ZFP_zfanyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_zfanyCast(ZFObject *, obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return (_ZFP_obj != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return zfcast(T_ZFObject *, this->toObject());
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

    /**
     * @brief see #ZFV
     */
    inline ZFV zfv(void) const {
        return ZFV(this->toObject());
    }

private:
    ZFObject *_ZFP_obj;
};
ZFCORE_POD_DECLARE(zfany)

// ============================================================
/**
 * @brief see #zfany
 */
template<typename T_ZFObjectBase>
zfclassPOD zfanyT : zfextend zfany {
    /** @cond ZFPrivateDoc */
public:
    zfanyT(void) : zfany() {}
    zfanyT(ZF_IN zfnullT const &) : zfany() {}
    template<typename T_ZFObject>
    zfanyT(ZF_IN T_ZFObject *obj) : zfany(obj) {}
    template<typename T_ZFObject>
    zfanyT(ZF_IN T_ZFObject const &obj) : zfany(obj) {}

public:
    inline zfbool operator == (ZF_IN zfanyT<T_ZFObjectBase> const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfanyT<T_ZFObjectBase> const &obj) const {
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
        return this->toObject() == _ZFP_zfanyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_zfanyCast(ZFObject *, obj);
    }

public:
    inline T_ZFObjectBase *operator -> (void) const {
        return zfcast(T_ZFObjectBase *, this->toObject());
    }
    inline ZFObject *toObject(void) const { // required for _ZFP_zfanyCast to work
        return zfany::toObject();
    }

public:
    static const ZFClass *ClassData(void) {
        return T_ZFObjectBase::ClassData();
    }
    /** @endcond */
};
ZFCORE_POD_DECLARE_TEMPLATE(typename T_Type, zfanyT<T_Type>)

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfany const &e) {
    return e.toObject() == _ZFP_zfanyCast(ZFObject *, obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfany const &e) {
    return e.toObject() != _ZFP_zfanyCast(ZFObject *, obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfanyT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_zfanyCast(ZFObject *, obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfanyT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_zfanyCast(ZFObject *, obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfany_h_

