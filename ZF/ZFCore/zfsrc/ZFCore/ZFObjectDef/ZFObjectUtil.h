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
 *   return #ZFCompareEqual if both null,
 *   return #ZFCompareUncomparable if one null but the other is not null,
 *   return compare result by #ZFObject::objectCompare if both not null
 */
inline ZFCompareResult ZFObjectCompare(
        ZF_IN ZFObject *const &e0
        , ZF_IN ZFObject *const &e1
        ) {
    if(e0 == zfnull) {
        if(e1 == zfnull) {
            return ZFCompareEqual;
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
            return e0->objectCompare(e1);
        }
    }
}
/**
 * @brief util method to #ZFObject::objectCompareValue
 */
inline ZFCompareResult ZFObjectCompareValue(
        ZF_IN ZFObject *const &e0
        , ZF_IN ZFObject *const &e1
        ) {
    if(e0 == zfnull) {
        if(e1 == zfnull) {
            return ZFCompareEqual;
        }
        else {
            switch(e1->objectCompareValue(e0)) {
                case ZFCompareEqual:
                    return ZFCompareEqual;
                case ZFCompareSmaller:
                    return ZFCompareGreater;
                case ZFCompareGreater:
                    return ZFCompareSmaller;
                case ZFCompareUncomparable:
                default:
                    return ZFCompareUncomparable;
            }
        }
    }
    else {
        return e0->objectCompareValue(e1);
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
    , typename zftEnableIf<zftIsZFObject(typename zftTraits<T0>::TrType)>::Value
    , typename zftEnableIf<zftIsZFObject(typename zftTraits<T1>::TrType)>::Value
    > {
public:
    static ZFCompareResult comparer(
            ZF_IN T0 const &e0
            , ZF_IN T1 const &e1
            ) {
        return ZFObjectCompare(zfcast(ZFObject *, e0), zfcast(ZFObject *, e1));
    }
};
/** @endcond */

// ============================================================
ZFCOMPARER_DEFAULT_DECLARE(zfany, zfany, {
        return ZFObjectCompare(v0.toObject(), v1.toObject());
    })
ZFCOMPARER_DEFAULT_DECLARE(zfauto, zfauto, {
        return ZFObjectCompare(v0.toObject(), v1.toObject());
    })

// ============================================================
/**
 * @brief util to create object from custom impl
 */
inline zfauto ZFObjectCreator(
        ZF_IN const ZFListener &impl
        , ZF_IN_OPT const ZFArgs &zfargs = ZFArgs()
        ) {
    impl.execute(zfargs);
    return zfargs.result();
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectUtil_h_

