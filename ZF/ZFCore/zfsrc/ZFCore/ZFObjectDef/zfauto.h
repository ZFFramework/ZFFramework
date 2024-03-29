/**
 * @file zfauto.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfauto_h_
#define _ZFI_zfauto_h_

#include "ZFObjectRetain.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfauto
/** @cond ZFPrivateDoc */
template<typename T_ZFObject
        , typename T_Fix /* = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf */
        >
zfauto::zfauto(ZF_IN T_ZFObject *p)
: d(zfnull)
{
    ZFObject *obj = (p ? p->toObject() : zfnull);
    if(obj) {
        zfCoreMutexLock();
        d = zfpoolNew(_ZFP_zfautoPrivate,
            zfunsafe_zfRetain(obj));
        zfCoreMutexUnlock();
    }
}
template<typename T_ZFObject
        , typename T_Fix /* = typename zftEnableIf<zftIsZFObjectType(typename zftTraits<T_ZFObject>::TrType)>::EnableIf */
        >
zfauto::zfauto(ZF_IN T_ZFObject const &p)
: d(zfnull)
{
    ZFObject *obj = _ZFP_ZFAnyCast(T_ZFObject, p);
    if(obj) {
        zfCoreMutexLock();
        d = zfpoolNew(_ZFP_zfautoPrivate,
            zfunsafe_zfRetain(obj));
        zfCoreMutexUnlock();
    }
}

template<typename T_ZFObject
        , typename T_Fix /* = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf */
        >
zfauto &zfauto::operator = (ZF_IN T_ZFObject *p) {
    zfCoreMutexLock();
    this->zfunsafe_assign(p ? p->toObject() : zfnull);
    zfCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject
        , typename T_Fix /* = typename zftEnableIf<zftIsZFObjectType(typename zftTraits<T_ZFObject>::TrType)>::EnableIf */
        >
zfauto &zfauto::operator = (ZF_IN T_ZFObject const &p) {
    zfCoreMutexLock();
    this->zfunsafe_assign(_ZFP_ZFAnyCast(T_ZFObject, p));
    zfCoreMutexUnlock();
    return *this;
}
/** @endcond */

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
T_ZFObject zfautoT<T_ZFObject>::operator -> (void) const {
    return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
}
/** @endcond */

// ============================================================
template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, zfauto, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfauto const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, zfauto, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfauto c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, zfauto, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfauto const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, zfauto, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfauto c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
template<typename T_To, int T_ToType, typename T_From>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, zfautoT<T_From>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfautoT<T_From> const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_To, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, zfautoT<T_To>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfautoT<T_To> c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType, typename T_From>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, zfautoT<T_From>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfautoT<T_From> const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_To, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, zfautoT<T_To>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfautoT<T_To> c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
template<typename T_ZFObject
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfauto const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}

// ============================================================
template<typename T_ZFObject
, typename T_ZFObjectBase
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject
, typename T_ZFObjectBase
, typename T_Fix = typename zftEnableIf<zftIsZFObject(typename zftTraits<T_ZFObject>::TrType)>::EnableIf
>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfautoT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfauto_h_

