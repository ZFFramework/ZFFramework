/**
 * @file zfcast.h
 * @brief zfcast declarations
 */

#ifndef _ZFI_zfcast_h_
#define _ZFI_zfcast_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
zfclassFwd ZFObject;
zfclassFwd ZFInterface;

// ============================================================
/**
 * @brief safely cast ZFObject types, return null if not desired type
 *
 * types can only be ZFObject or ZFInterface types\n
 * \n
 * ADVANCED:\n
 * -  cast between ZFInterface or it's children types may take much time for dynamic checking,
 *   use with caution
 * -  try not to cast from and to the base ZFInterface * type
 */
#define zfcast(T_To, obj) _ZFP_ObjCast<T_To>::c(obj)

#define _ZFP_ObjCastInternal(T_To, obj) \
    static_cast<T_To>(obj)


// ============================================================
// cast any to ZFObject
template<typename T_Type, int isZFObject>
zfclassNotPOD _ZFP_zfanyCastH {};
template<>
zfclassNotPOD _ZFP_zfanyCastH<zfnullT, 0> {
public:
    static inline ZFObject *c(ZF_IN zfnullT const &obj) {
        return zfnull;
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_zfanyCastH<T_Type *, 1> {
public:
    static inline ZFObject *c(ZF_IN T_Type *const &obj) {
        return obj ? obj->toObject() : zfnull;
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_zfanyCastH<T_Type, 1> {
public:
    static inline ZFObject *c(ZF_IN T_Type const &obj) {
        return obj.toObject();
    }
};
template<typename T_Type>
inline ZFObject *_ZFP_zfanyCast(ZF_IN T_Type const &obj) {
    typedef typename zftTraits<T_Type>::TrType T_TypeTmp;
    return _ZFP_zfanyCastH<T_Type, zftIsZFObject(T_TypeTmp) || zftIsZFObjectType(T_TypeTmp)>::c(obj);
}
inline ZFObject *_ZFP_zfanyCast(ZF_IN ZFObject * const &obj) {
    return obj;
}

// ============================================================
// for impl
/** @cond ZFPrivateDoc */
template<typename T_ZFObjectTo, typename T_ZFObjectFrom>
inline T_ZFObjectTo _ZFP_ObjCastImpl(T_ZFObjectFrom const &obj) {
    return static_cast<T_ZFObjectTo>(obj->classData()->_ZFP_ZFClass_objectCast(obj, zftTraits<T_ZFObjectTo>::TrType::ClassData()));
}
/** @endcond */


// ============================================================
#define _ZFP_ObjCastTypeZFObject 1
#define _ZFP_ObjCastTypeZFInterface 2
#define _ZFP_ObjCastTypeUnknown 3

#define _ZFP_ObjCastType(TrType) \
    (zftIsZFObject(TrType) \
        ? (zftIsTypeOf<TrType, ZFObject>::Value \
                ? _ZFP_ObjCastTypeZFObject \
                : _ZFP_ObjCastTypeZFInterface \
            ) \
        : _ZFP_ObjCastTypeUnknown)


// ============================================================
// zfcast
template<typename T_To>
zfclassNotPOD _ZFP_ObjCast {
public:
    template<typename T_From>
    static inline T_To c(ZF_IN T_From const &obj);
};
template<int T_CanCastDirectly, typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD _ZFP_ObjCastH {
};

// can cast directly
template<typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD _ZFP_ObjCastH<1, T_To, T_From, T_ToType, T_FromType> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return static_cast<T_To>(obj);
    }
};
// object to object
template<typename T_To, typename T_From>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFObject> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return (obj
            ? _ZFP_ObjCastImpl<T_To>(obj)
            : zfnull);
    }
};
// object to interface
template<typename T_To, typename T_From>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFObject> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj,
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};
// interface to object
template<typename T_To, typename T_From>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFInterface> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return (obj
            ? _ZFP_ObjCastImpl<T_To>(obj->toObject())
            : zfnull);
    }
};
// interface to interface
template<typename T_To, typename T_From>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFInterface> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj->toObject(),
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};
// unknown to any
template<typename T_To, typename T_From, int T_ToType>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return zfcast(T_To, _ZFP_zfanyCast(obj));
    }
};
// any to unknown
template<typename T_To, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return T_To(obj);
    }
};
// unknown to unknown
template<typename T_To, typename T_From>
zfclassNotPOD _ZFP_ObjCastH<0, T_To, T_From, _ZFP_ObjCastTypeUnknown, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(ZF_IN T_From const &obj) {
        return T_To(_ZFP_zfanyCast(obj));
    }
};

// invoker
template<typename T_To>
template<typename T_From>
inline T_To _ZFP_ObjCast<T_To>::c(ZF_IN T_From const &obj) {
    typedef typename zftTraits<T_To>::TrType T_ToTmp;
    typedef typename zftTraits<T_From>::TrType T_FromTmp;
    return _ZFP_ObjCastH<
            zftIsTypeOf<typename zftTraits<T_From>::TrType, typename zftTraits<T_To>::TrType>::Value,
            T_To, T_From,
            _ZFP_ObjCastType(T_ToTmp), _ZFP_ObjCastType(T_FromTmp)
        >::c(obj);
}
// direct cast to ZFInterface is not allowed
template<>
zfclassNotPOD _ZFP_ObjCast<ZFInterface *> {};
// spec for const void *
template<>
zfclassNotPOD _ZFP_ObjCast<const void *> {
public:
    template<typename T_From>
    static inline const void *c(ZF_IN T_From const &obj) {
        return (const void *)zfcast(ZFObject *, obj);
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfcast_h_

