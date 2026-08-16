/**
 * @file ZFSerializableUtil.h
 * @brief utilities for ZFSerializable's implementation
 */

#ifndef _ZFI_ZFSerializableUtil_h_
#define _ZFI_ZFSerializableUtil_h_

#include "ZFSerializable.h"
#include "ZFOutputDefault.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN_REGISTER(ZFSerializableUtil, ZF_NAMESPACE_GLOBAL)

/**
 * @brief util method to set error
 */
#define ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, pos, fmt, ...) \
        ZFSerializableUtil::_ZFP_errorOccurredAt(errorHint, errorPos, pos, zfstr(fmt, ##__VA_ARGS__))
/**
 * @brief util method to set error
 */
#define ZFSerializableUtilErrorOccurred(errorHint, fmt, ...) \
        ZFSerializableUtil::_ZFP_errorOccurred(errorHint, zfstr(fmt, ##__VA_ARGS__))

extern ZFLIB_ZFCore void _ZFP_errorOccurredAt(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_OUT_OPT ZFSerializableData *errorPos
        , ZF_IN const ZFSerializableData &pos
        , ZF_IN const zfstring &text
        );
extern ZFLIB_ZFCore void _ZFP_errorOccurred(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfstring &text
        );

/**
 * @brief util to check whether the serializable data contains desired attribute
 *
 * use #ZFTypeId_none shows that any class name is allowed except empty\n
 * return null if no match
 */
extern ZFLIB_ZFCore zfstring checkItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredClass
        );
/**
 * @brief see #checkItemClass, output error hint if failed
 */
extern ZFLIB_ZFCore zfstring requireItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredClass
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired attribute,
 *   auto mark as resolved
 *
 * return null if not exist
 */
extern ZFLIB_ZFCore zfstring checkAttr(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredAttr
        );
/**
 * @brief see #checkAttr, output error hint if failed,
 *   auto mark as resolved
 */
extern ZFLIB_ZFCore zfstring requireAttr(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredAttr
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto mark name as resolved
 */
extern ZFLIB_ZFCore ZFSerializableData checkElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementName
        );
/**
 * @brief see #checkElementByName, output error hint if failed,
 *   auto mark name as resolved
 */
extern ZFLIB_ZFCore ZFSerializableData requireElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementName
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto mark category as resolved
 */
extern ZFLIB_ZFCore ZFSerializableData checkElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementCategory
        );
/**
 * @brief see #checkElementByCategory, output error hint if failed,
 *   auto mark category as resolved
 */
extern ZFLIB_ZFCore ZFSerializableData requireElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfstring &desiredElementCategory
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains property name attribute,
 *   auto mark as resolved
 */
inline zfstring checkPropertyName(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_prop);
}
/**
 * @brief see #checkPropertyName, output error hint if failed,
 *   auto mark as resolved
 */
inline zfstring requirePropertyName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttr(serializableData, ZFSerializableKeyword_prop, errorHint, errorPos);
}

/**
 * @brief util to check whether the serializable data contains property value attribute,
 *   auto mark as resolved
 */
inline zfstring checkPropertyValue(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_value);
}
/**
 * @brief see #checkPropertyValue, output error hint if failed,
 *   auto mark as resolved
 */
inline zfstring requirePropertyValue(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttr(serializableData, ZFSerializableKeyword_value, errorHint, errorPos);
}

/**
 * @brief util to check whether the serializable data contains category,
 *   auto mark as resolved
 */
inline zfstring checkCategory(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_category);
}
/**
 * @brief see #checkCategory, output error hint if failed,
 *   auto mark as resolved
 */
inline zfstring requireCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *errorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttr(serializableData, ZFSerializableKeyword_category, errorHint, errorPos);
}

/**
 * @brief util method to print resolve status of ZFSerializableData, for debug use only,
 *   output nothing if all resolved
 *
 * return true if anything not resolved, or false if all resolved
 */
extern ZFLIB_ZFCore zfbool printResolveStatus(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN_OPT const ZFOutput &outputCallback = ZFOutputDefault()
        );

// ============================================================
// util macro for impl
#define _ZFP_ZFSerializableUtilSerializeFromData_check(errorHint, errorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData_require(errorHint, errorPos) , errorHint, errorPos
#define _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, errorHint, errorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_##check_or_require(errorHint, errorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, errorHint, errorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, errorHint, errorPos)

/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttrFromData(serializableData, errorHint, errorPos, \
    check_or_require, key, TypeName, value, failAction) \
    do { \
        zfstring valueString = ZFSerializableUtil::check_or_require##Attr(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, errorHint, errorPos)); \
        if(valueString != zfnull) { \
            if(!TypeName##FromStringT(value, valueString)) { \
                ZFSerializableUtilErrorOccurredAt(errorHint, errorPos, serializableData, \
                    "failed to convert from \"%s\"", valueString); \
                failAction \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttrToData(serializableData, errorHint, ref, \
    key, TypeName, thisValue, refData, defaultValue, failAction) \
    do { \
        if((ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) \
                || (ref != zfnull && ZFComparerDefault(thisValue, refData) != ZFCompareEqual) \
                ) { \
            zfstring valueString; \
            if(!TypeName##ToStringT(valueString, thisValue)) { \
                ZFSerializableUtilErrorOccurred(errorHint, \
                    "failed to convert %s to string", key); \
                failAction \
            } \
            else { \
                serializableData.attr(key, valueString); \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, errorHint, \
    key, TypeName, thisValue, defaultValue, failAction) \
    do { \
        if(ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) { \
            zfstring valueString; \
            if(!TypeName##ToStringT(valueString, thisValue)) { \
                ZFSerializableUtilErrorOccurred(errorHint, \
                    "failed to convert %s to string", key); \
                failAction \
            } \
            else { \
                serializableData.attr(key, valueString); \
            } \
        } \
    } while(zffalse)

// ============================================================
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryFromData(serializableData, errorHint, errorPos, \
    check_or_require, key, TypeName, value, failAction) \
    do { \
        ZFSerializableData valueData = ZFSerializableUtil::check_or_require##ElementByCategory(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, errorHint, errorPos)); \
        if(valueData != zfnull) { \
            if(!TypeName##FromDataT(value, valueData, errorHint, errorPos)) { \
                failAction \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryToData(serializableData, errorHint, ref, \
    key, TypeName, thisValue, refData, defaultValue, failAction) \
    do { \
        if((ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) \
                || (ref != zfnull && ZFComparerDefault(thisValue, refData) != ZFCompareEqual) \
                ) { \
            ZFSerializableData categoryData; \
            if(!TypeName##ToDataT(categoryData, thisValue, errorHint)) { \
                failAction \
            } \
            else { \
                categoryData.category(key); \
                serializableData.child(categoryData); \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, errorHint, \
    key, TypeName, thisValue, defaultValue, failAction) \
    do { \
        if(ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) { \
            ZFSerializableData categoryData; \
            if(!TypeName##ToDataT(categoryData, thisValue, errorHint)) { \
                failAction \
            } \
            else { \
                categoryData.category(key); \
                serializableData.child(categoryData); \
            } \
        } \
    } while(zffalse)

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableUtil_h_

