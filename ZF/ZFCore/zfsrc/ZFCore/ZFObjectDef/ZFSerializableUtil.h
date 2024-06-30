/**
 * @file ZFSerializableUtil.h
 * @brief utilities for ZFSerializable's implementation
 */

#ifndef _ZFI_ZFSerializableUtil_h_
#define _ZFI_ZFSerializableUtil_h_

#include "ZFSerializable.h"
#include "ZFOutputDefault.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

/**
 * @brief util method to set error
 */
#define ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, errorPos, fmt, ...) \
        ZFSerializableUtil::_ZFP_errorOccurredAt(outErrorHint, outErrorPos, errorPos, zfstr(fmt, ##__VA_ARGS__))
/**
 * @brief util method to set error
 */
#define ZFSerializableUtilErrorOccurred(outErrorHint, fmt, ...) \
        ZFSerializableUtil::_ZFP_errorOccurred(outErrorHint, zfstr(fmt, ##__VA_ARGS__))

extern ZFLIB_ZFCore void _ZFP_errorOccurredAt(
        ZF_OUT_OPT zfstring *outErrorHint
        , ZF_OUT_OPT ZFSerializableData *outErrorPos
        , ZF_IN const ZFSerializableData &errorPos
        , ZF_IN const zfchar *text
        );
extern ZFLIB_ZFCore void _ZFP_errorOccurred(
        ZF_OUT_OPT zfstring *outErrorHint
        , ZF_IN const zfchar *text
        );

/**
 * @brief util to check whether the serializable data contains desired attribute
 *
 * use #ZFTypeId_none shows that any class name is allowed except empty\n
 * return null if no match
 */
extern ZFLIB_ZFCore const zfchar *checkItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredClass
        );
/**
 * @brief see #checkItemClass, output error hint if failed
 */
extern ZFLIB_ZFCore const zfchar *requireItemClass(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredClass
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired attribute,
 *   auto mark as resolved
 *
 * return null if not exist
 */
extern ZFLIB_ZFCore const zfchar *checkAttribute(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredAttribute
        );
/**
 * @brief see #checkAttribute, output error hint if failed,
 *   auto mark as resolved
 */
extern ZFLIB_ZFCore const zfchar *requireAttribute(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredAttribute
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto mark name as resolved
 */
extern ZFLIB_ZFCore const ZFSerializableData *checkElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementName
        );
/**
 * @brief see #checkElementByName, output error hint if failed,
 *   auto mark name as resolved
 */
extern ZFLIB_ZFCore const ZFSerializableData *requireElementByName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementName
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto mark category as resolved
 */
extern ZFLIB_ZFCore const ZFSerializableData *checkElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementCategory
        );
/**
 * @brief see #checkElementByCategory, output error hint if failed,
 *   auto mark category as resolved
 */
extern ZFLIB_ZFCore const ZFSerializableData *requireElementByCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_IN const zfchar *desiredElementCategory
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        );

/**
 * @brief util to check whether the serializable data contains property name attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyName(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_prop);
}
/**
 * @brief see #checkPropertyName, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyName(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_prop, outErrorHint, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains property value attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyValue(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_value);
}
/**
 * @brief see #checkPropertyValue, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyValue(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_value, outErrorHint, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains category,
 *   auto mark as resolved
 */
inline const zfchar *checkCategory(ZF_IN const ZFSerializableData &serializableData) {
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_category);
}
/**
 * @brief see #checkCategory, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requireCategory(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
        ) {
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_category, outErrorHint, outErrorPos);
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
#define _ZFP_ZFSerializableUtilSerializeFromData_check(outErrorHint, outErrorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData_require(outErrorHint, outErrorPos) , outErrorHint, outErrorPos
#define _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, outErrorHint, outErrorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_##check_or_require(outErrorHint, outErrorPos)
#define _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos) \
    _ZFP_ZFSerializableUtilSerializeFromData_(check_or_require, outErrorHint, outErrorPos)

/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos, \
    check_or_require, key, TypeName, value, failAction) \
    do { \
        const zfchar *valueString = ZFSerializableUtil::check_or_require##Attribute(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos)); \
        if(valueString != zfnull) { \
            if(!TypeName##FromStringT(value, valueString)) { \
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, \
                    "failed to convert from \"%s\"", valueString); \
                failAction \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref, \
    key, TypeName, thisValue, refData, defaultValue, failAction) \
    do { \
        if((ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) \
                || (ref != zfnull && ZFComparerDefault(thisValue, refData) != ZFCompareEqual) \
                ) { \
            zfstring valueString; \
            if(!TypeName##ToStringT(valueString, thisValue)) { \
                ZFSerializableUtilErrorOccurred(outErrorHint, \
                    "failed to convert %s to string", key); \
                failAction \
            } \
            else { \
                serializableData.attr(key, valueString); \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeAttributeToDataNoRef(serializableData, outErrorHint, \
    key, TypeName, thisValue, defaultValue, failAction) \
    do { \
        if(ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) { \
            zfstring valueString; \
            if(!TypeName##ToStringT(valueString, thisValue)) { \
                ZFSerializableUtilErrorOccurred(outErrorHint, \
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
#define ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos, \
    check_or_require, key, TypeName, value, failAction) \
    do { \
        const ZFSerializableData *valueData = ZFSerializableUtil::check_or_require##ElementByCategory(serializableData, key \
            _ZFP_ZFSerializableUtilSerializeFromData(check_or_require, outErrorHint, outErrorPos)); \
        if(valueData != zfnull) { \
            if(!TypeName##FromDataT(value, *valueData, outErrorHint, outErrorPos)) { \
                failAction \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHint, ref, \
    key, TypeName, thisValue, refData, defaultValue, failAction) \
    do { \
        if((ref == zfnull && ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) \
                || (ref != zfnull && ZFComparerDefault(thisValue, refData) != ZFCompareEqual) \
                ) { \
            ZFSerializableData categoryData; \
            if(!TypeName##ToDataT(categoryData, thisValue, outErrorHint)) { \
                failAction \
            } \
            else { \
                categoryData.category(key); \
                serializableData.childAdd(categoryData); \
            } \
        } \
    } while(zffalse)
/** @brief util macro to impl #ZFSerializable */
#define ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint, \
    key, TypeName, thisValue, defaultValue, failAction) \
    do { \
        if(ZFComparerDefault(thisValue, defaultValue) != ZFCompareEqual) { \
            ZFSerializableData categoryData; \
            if(!TypeName##ToDataT(categoryData, thisValue, outErrorHint)) { \
                failAction \
            } \
            else { \
                categoryData.category(key); \
                serializableData.childAdd(categoryData); \
            } \
        } \
    } while(zffalse)

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableUtil_h_

