/**
 * @file ZFClassUtil.h
 * @brief util for ZFClass
 */

#ifndef _ZFI_ZFClassUtil_h_
#define _ZFI_ZFClassUtil_h_

#include "ZFObjectCore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;

ZF_NAMESPACE_BEGIN_REGISTER(ZFClassUtil, ZF_NAMESPACE_GLOBAL)

/**
 * @brief compare all properties of obj0 with obj1's property by reflect
 *
 * if both object is null, return true\n
 * two object have no need to be same class,
 * but must be ensured that all property are safe to be compared,
 * i.e. each property's type must match
 */
extern ZFLIB_ZFCore zfbool allPropertyIsEqual(
        ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        );

/**
 * @brief use reflect to print all property if the property is not #ZFPropertyIsInitValue
 */
extern ZFLIB_ZFCore void objectPropertyInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        );
/** @brief see #objectPropertyInfo */
inline zfstring objectPropertyInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        ) {
    zfstring ret;
    ZFClassUtil::objectPropertyInfo(ret, obj, maxCount, token);
    return ret;
}

/** @brief see #objectInfo */
extern ZFLIB_ZFCore void objectInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        );
/**
 * @brief get short info of object, see #objectPropertyInfo
 */
inline zfstring objectInfo(
        ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()
        ) {
    zfstring ret;
    ZFClassUtil::objectInfoT(ret, obj, maxCount, token);
    return ret;
}

ZF_NAMESPACE_END(ZFClassUtil)
ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFClassUtil_h_

