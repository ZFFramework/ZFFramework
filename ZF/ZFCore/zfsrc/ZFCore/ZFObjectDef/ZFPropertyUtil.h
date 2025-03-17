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

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyUtil_h_

