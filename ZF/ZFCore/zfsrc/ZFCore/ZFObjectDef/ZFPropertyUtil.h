/**
 * @file ZFPropertyUtil.h
 * @brief utility for ZFProperty
 */

#ifndef _ZFI_ZFPropertyUtil_h_
#define _ZFI_ZFPropertyUtil_h_

#include "ZFObjectUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPropertyIsValueAccessed
/**
 * @brief util for #ZFPropertyCallbackIsValueAccessed
 */
inline zfbool ZFPropertyIsValueAccessed(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN zfany const &ownerObj
        ) {
    return propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
}

// ============================================================
// ZFPropertyIsInitValue
/**
 * @brief util for #ZFPropertyCallbackIsInitValue
 */
inline zfbool ZFPropertyIsInitValue(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN zfany const &ownerObj
        ) {
    return propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, zfnull);
}

// ============================================================
// ZFPropertyValueReset
/**
 * @brief util for #ZFPropertyCallbackValueReset
 */
inline void ZFPropertyValueReset(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN zfany const &ownerObj
        ) {
    propertyInfo->callbackValueReset(propertyInfo, ownerObj);
}

// ============================================================
// ZFPropertyCompare
/**
 * @brief util to compare property value
 */
inline ZFCompareResult ZFPropertyCompare(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        ) {
    return ZFObjectCompare(
        propertyInfo->getterMethod()->methodInvoke(obj0),
        propertyInfo->getterMethod()->methodInvoke(obj1));
}

// ============================================================
// ZFPropertyCompareValue
/**
 * @brief util to compare property value
 */
inline ZFCompareResult ZFPropertyCompareValue(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        ) {
    return ZFObjectCompareValue(
        propertyInfo->getterMethod()->methodInvoke(obj0),
        propertyInfo->getterMethod()->methodInvoke(obj1));
}

// ============================================================
// ZFPropertyCopy
/**
 * @brief util to copy property between object
 */
inline void ZFPropertyCopy(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN ZFObject *dstObj
        , ZF_IN ZFObject *srcObj
        ) {
    propertyInfo->setterMethod()->methodInvoke(dstObj, propertyInfo->getterMethod()->methodInvoke(srcObj));
}

// ============================================================
// ZFPropertyCopyAll
/**
 * @brief copy all properties from srcObj to dstObj, filtered by filter
 *
 * this function would go through all properties in dstObj,
 * then search whether there are same property in srcObj,
 * if there are, copy them from srcObj to dstObj by the reflectable getter and setter\n
 * search is done by property's name compare and type compare,
 * and type is compared by ZFProperty::propertyTypeId\n
 * it's your responsibility to make sure the copy is valid
 * @note this function may cost much time if there are many properties in the inherit tree,
 *   you may want to use #ZFPropertyCopy directly for performance
 */
extern ZFLIB_ZFCore void ZFPropertyCopyAll(
        ZF_IN ZFObject *dstObj
        , ZF_IN ZFObject *srcObj
        , ZF_IN_OPT const ZFFilterForZFProperty *filter = zfnull
        , ZF_OUT_OPT ZFCoreArray<const ZFProperty *> *copiedProperties = zfnull
        );

// ============================================================
// ZFPropertyGetInfo
/**
 * @brief util to get property value info
 */
inline void ZFPropertyGetInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const ZFProperty *propertyInfo
        , ZF_IN ZFObject *ownerObject
        ) {
    ZFObjectInfoT(ret, propertyInfo->getterMethod()->methodInvoke(ownerObject));
}
/** @brief see #ZFPropertyGetInfo */
inline zfstring ZFPropertyGetInfo(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN ZFObject *ownerObject
        ) {
    zfstring ret;
    ZFPropertyGetInfo(ret, propertyInfo, ownerObject);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyUtil_h_

