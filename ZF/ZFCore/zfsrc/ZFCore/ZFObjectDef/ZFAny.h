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
    ZFAny(void) : _obj(zfnull) {}
    ZFAny(ZF_IN ZFAny const &obj) : _obj(obj.toObject()) {}
    ZFAny(ZF_IN zfnullT const &) : _obj(zfnull) {}
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    ZFAny(ZF_IN T_ZFObject *obj) : _obj(obj ? obj->toObject() : zfnull) {}
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObjectType(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    ZFAny(ZF_IN T_ZFObject const &obj) : _obj(_ZFP_ZFAnyCast(T_ZFObject, obj)) {}

public:
    inline ZFAny &operator = (ZF_IN ZFAny const &obj) {
        this->_obj = obj.toObject();
        return *this;
    }
    inline ZFAny &operator = (ZF_IN zfnullT const &) {
        this->_obj = zfnull;
        return *this;
    }
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    inline ZFAny &operator = (ZF_IN T_ZFObject *obj) {
        this->_obj = (obj ? obj->toObject() : zfnull);
        return *this;
    }
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObjectType(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    inline ZFAny &operator = (ZF_IN T_ZFObject const &obj) {
        this->_obj = _ZFP_ZFAnyCast(T_ZFObject, obj);
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
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    inline zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return this->toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return (this->_obj != zfnull);
    }
    template<typename T_ZFObject
        , typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
        >
    inline operator T_ZFObject (void) const {
        return ZFCastZFObject(T_ZFObject, this->toObject());
    }
    /** @endcond */

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return this->_obj;
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->_obj);
    }

private:
    ZFObject *_obj;
};

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
template<typename T_ZFObject
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN ZFAny const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN ZFAny const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAny_h_

