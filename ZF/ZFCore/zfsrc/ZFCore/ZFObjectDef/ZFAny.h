/**
 * @file ZFAny.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_ZFAny_h_
#define _ZFI_ZFAny_h_

#include "ZFObjectCast.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFObjectHolder;

// ============================================================
// ZFAny
/**
 * @brief util method to cast ZFObject types freely
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAny any = obj;
 *
 *   // cast to any other object type, return null if not able to cast
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 *
 * note: ZFAny stores raw ZFObject pointer,
 * use #zfauto or #zfweak for most case,
 * use ZFAny only if necessary
 */
zfclassLikePOD ZFLIB_ZFCore ZFAny {
    /** @cond ZFPrivateDoc */
public:
    ZFAny(void) : _ZFP_obj(zfnull) {}
    ZFAny(ZF_IN ZFAny const &obj) : _ZFP_obj(obj.toObject()) {}
    ZFAny(ZF_IN zfnullT const &) : _ZFP_obj(zfnull) {}
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject *obj) : _ZFP_obj(obj ? obj->toObject() : zfnull) {}
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject const &obj) : _ZFP_obj(_ZFP_ZFAnyCast(T_ZFObject, obj)) {}

public:
    inline ZFAny &operator = (ZF_IN ZFAny const &obj) {
        _ZFP_obj = obj.toObject();
        return *this;
    }
    inline ZFAny &operator = (ZF_IN zfnullT const &) {
        _ZFP_obj = zfnull;
        return *this;
    }
    template<typename T_ZFObject>
    inline ZFAny &operator = (ZF_IN T_ZFObject *obj) {
        _ZFP_obj = (obj ? obj->toObject() : zfnull);
        return *this;
    }
    template<typename T_ZFObject>
    inline ZFAny &operator = (ZF_IN T_ZFObject const &obj) {
        _ZFP_obj = _ZFP_ZFAnyCast(T_ZFObject, obj);
        return *this;
    }

public:
    inline zfbool operator == (ZF_IN ZFAny const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN ZFAny const &obj) const {
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
        return (_ZFP_obj != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
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
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return ZFCastZFObjectUnchecked(T_ZFObject, _ZFP_obj);
    }

private:
    ZFObject *_ZFP_obj;
};

// ============================================================
/**
 * @brief see #ZFAny
 */
template<typename T_ZFObjectBase>
zfclassLikePOD ZFAnyT : zfextend ZFAny {
    /** @cond ZFPrivateDoc */
public:
    ZFAnyT(void) : ZFAny() {}
    ZFAnyT(ZF_IN zfnullT const &) : ZFAny() {}
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN T_ZFObject *obj) : ZFAny(obj) {}
    template<typename T_ZFObject>
    ZFAnyT(ZF_IN T_ZFObject const &obj) : ZFAny(obj) {}

public:
    inline zfbool operator == (ZF_IN ZFAnyT<T_ZFObjectBase> const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN ZFAnyT<T_ZFObjectBase> const &obj) const {
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
    inline T_ZFObjectBase *operator -> (void) const {
        return ZFCastZFObjectUnchecked(T_ZFObjectBase *, this->toObject());
    }
    inline ZFObject *toObject(void) const { // required for _ZFP_ZFAnyCast to work
        return ZFAny::toObject();
    }

public:
    static const ZFClass *ClassData(void) {
        return T_ZFObjectBase::ClassData();
    }
    /** @endcond */
};

// ============================================================
template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, ZFAny, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZFAny const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, ZFAny, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline ZFAny c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, ZFAny, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZFAny const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, ZFAny, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline ZFAny c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
template<typename T_ZFObject, typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, ZFAnyT<T_ZFObject>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZFAnyT<T_ZFObject> const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_ZFObject, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, ZFAnyT<T_ZFObject>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline ZFAnyT<T_ZFObject> c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_ZFObject, typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, ZFAnyT<T_ZFObject>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZFAnyT<T_ZFObject> const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_ZFObject, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, ZFAnyT<T_ZFObject>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline ZFAnyT<T_ZFObject> c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN ZFAny const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN ZFAny const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN ZFAnyT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN ZFAnyT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAny_h_

