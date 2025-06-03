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

/**
 * @brief util for #ZFPropertyCallbackIsInitValue
 */
inline zfauto ZFPropertyGetInitValue(
        ZF_IN const ZFProperty *propertyInfo
        , ZF_IN zfany const &ownerObj
        ) {
    zfauto v;
    propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, &v);
    return v;
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
/**
 * @brief copy all properties from srcObj to dstObj
 *
 * this function would go through all properties in dstObj,
 * then search whether there are same property in srcObj,
 * if there are, copy them from srcObj to dstObj by the reflectable getter and setter\n
 * search is done by property's name compare and type compare,
 * and type is compared by ZFProperty::propertyTypeId\n
 * it's your responsibility to make sure the copy is valid\n
 * \n
 * by default, these properties would be ignored:
 * -  setter or getter is not public
 * -  property name start with '_'
 *
 * @note this function may cost much time if there are many properties in the inherit tree,
 *   you may want to use #ZFPropertyCopy directly for performance
 */
extern ZFLIB_ZFCore void ZFPropertyCopyAll(
        ZF_IN ZFObject *dstObj
        , ZF_IN ZFObject *srcObj
        );

// ============================================================
/**
 * @brief compare all properties of obj0 with obj1's property by reflect
 *
 * if both object is null, return true\n
 * two object have no need to be same class,
 * but must be ensured that all property are safe to be compared,
 * i.e. each property's type must match
 */
extern ZFLIB_ZFCore zfbool ZFPropertyAllEqual(
        ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        );

/**
 * @brief use reflect to print all property if the property is not #ZFPropertyIsInitValue
 *
 * result looks like:
 * @code
 *   {prop1=v1,prop2=v2}
 * @endcode
 */
extern ZFLIB_ZFCore void ZFObjectPropertyInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        );
/** @brief see #ZFObjectPropertyInfoT */
inline zfstring ZFObjectPropertyInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        ) {
    zfstring ret;
    ZFObjectPropertyInfoT(ret, obj, maxCount, token);
    return ret;
}

/**
 * @brief print verbose info about the object, using #ZFObjectPropertyInfoT
 *
 * result looks like:
 * @code
 *   <NS.Obj(0x12345678){prop1=v1,prop2=v2}>
 * @endcode
 */
extern ZFLIB_ZFCore void ZFObjectVerboseInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        );
/** @brief see #ZFObjectVerboseInfoT */
inline zfstring ZFObjectVerboseInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        ) {
    zfstring ret;
    ZFObjectVerboseInfoT(ret, obj, maxCount, token);
    return ret;
}

/**
 * @brief print pretty info about the object, using #ZFObjectPropertyInfoT
 *
 * result looks like:
 * @code
 *   <Obj prop1=v1 prop2=v2>
 * @endcode
 */
extern ZFLIB_ZFCore void ZFObjectPrettyInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        );
/** @brief see #ZFObjectPrettyInfoT */
inline zfstring ZFObjectPrettyInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        ) {
    zfstring ret;
    ZFObjectPrettyInfoT(ret, obj, maxCount);
    return ret;
}

/**
 * @brief print short info about the object, using #ZFObjectPropertyInfoT
 *
 * result looks like:
 * @code
 *   (v1, v2)
 * @endcode
 */
extern ZFLIB_ZFCore void ZFObjectShortInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        );
/** @brief see #ZFObjectShortInfoT */
inline zfstring ZFObjectShortInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        ) {
    zfstring ret;
    ZFObjectShortInfoT(ret, obj, maxCount);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyUtil_h_

