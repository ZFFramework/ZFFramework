/**
 * @file ZFObjectUtil.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFObjectUtil_h_
#define _ZFI_ZFObjectUtil_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// compare
/**
 * @brief util method to compare two ZFObject,
 *   return #ZFCompareTheSame if both null,
 *   return #ZFCompareUncomparable if one null but the other is not null,
 *   return compare result by #ZFObject::objectCompare if both not null
 */
inline ZFCompareResult ZFObjectCompare(
        ZF_IN ZFObject *const &e0
        , ZF_IN ZFObject *const &e1
        ) {
    if(e0 == zfnull) {
        if(e1 == zfnull) {
            return ZFCompareTheSame;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
    else {
        if(e1 == zfnull) {
            return ZFCompareUncomparable;
        }
        else {
            return e0->toObject()->objectCompare(e1->toObject());
        }
    }
}

// ============================================================
/** @brief see #ZFObjectInfoOfInstance */
inline void ZFObjectInfoOfInstanceT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        ) {
    if(obj == zfnull) {
        ret += ZFTOKEN_zfnull;
    }
    else {
        obj->objectInfoOfInstanceT(ret);
    }
}
/**
 * @brief util method to get info of object
 *   or return ZFTOKEN_zfnull if obj is null
 * @see ZFObject::objectInfoOfInstance
 */
inline zfstring ZFObjectInfoOfInstance(ZF_IN ZFObject *obj) {
    return ((obj == zfnull) ? zfstring(ZFTOKEN_zfnull) : obj->objectInfoOfInstance());
}
/** @brief see #ZFObjectInfo */
inline void ZFObjectInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        ) {
    if(obj == zfnull) {
        ret += ZFTOKEN_zfnull;
    }
    else {
        obj->objectInfoT(ret);
    }
}
/**
 * @brief util method to get info of object
 *   or return ZFTOKEN_zfnull if obj is null
 * @see ZFObject::objectInfo
 */
inline zfstring ZFObjectInfo(ZF_IN ZFObject *obj) {
    return ((obj == zfnull) ? zfstring(ZFTOKEN_zfnull) : obj->objectInfo());
}

/**
 * @brief util method to calculate hash of object,
 *   or return #zfidentityInvalid if null
 */
inline zfidentity ZFObjectHash(ZF_IN ZFObject *obj) {
    return ((obj == zfnull) ? zfidentityInvalid() : obj->objectHash());
}

// ============================================================
// special fix for ZFObject types
/** @cond ZFPrivateDoc */
template<typename T0, typename T1>
zfclassNotPOD ZFComparerDefaultHolder<T0, T1
    , void, void
    , typename zftEnableIf<zftIsZFObject(typename zftTraits<T0>::TrType)>::EnableIf
    , typename zftEnableIf<zftIsZFObject(typename zftTraits<T1>::TrType)>::EnableIf
    > {
public:
    static ZFCompareResult comparer(
            ZF_IN T0 const &e0
            , ZF_IN T1 const &e1
            ) {
        return ZFObjectCompare(ZFCastZFObject(ZFObject *, e0), ZFCastZFObject(ZFObject *, e1));
    }
};
/** @endcond */

// ============================================================
ZFCOMPARER_DEFAULT_DECLARE(zfauto, zfauto, {
        return ZFObjectCompare(v0.toObject(), v1.toObject());
    })

/** @cond ZFPrivateDoc */
template<typename T_ZFObject0, typename T_ZFObject1>
zfclassNotPOD ZFComparerDefaultHolder<zfautoT<T_ZFObject0>, zfautoT<T_ZFObject1> > {
public:
    static ZFCompareResult comparer(
            ZF_IN zfautoT<T_ZFObject0> const &v0
            , ZF_IN zfautoT<T_ZFObject1> const &v1
            ) {
        return ZFObjectCompare(v0.toObject(), v1.toObject());
    }
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectUtil_h_

