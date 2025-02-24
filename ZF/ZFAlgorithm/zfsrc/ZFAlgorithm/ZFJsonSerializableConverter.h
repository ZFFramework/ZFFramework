/**
 * @file ZFJsonSerializableConverter.h
 * @brief serializable convertion between json and serializalbe
 */

#ifndef _ZFI_ZFJsonSerializableConverter_h_
#define _ZFI_ZFJsonSerializableConverter_h_

#include "ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief parse json formated data to serializable
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataFromJson
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, outErrorPos, zfnull)
        )
/**
 * @brief see #ZFSerializableDataFromJson
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFJson &, jsonObject)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFJson *, outErrorPos, zfnull)
        )
/**
 * @brief print serializable to json formated data
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataToJson
        , ZFMP_OUT(ZFJson &, jsonObject)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
/**
 * @brief see #ZFSerializableDataToJson
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFJson, ZFSerializableDataToJson
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )

// ============================================================
/**
 * @brief util method to parse serializable data from input with json format
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataFromJson
        , ZFMP_OUT(ZFSerializableData &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/** @brief see #ZFSerializableDataFromJson */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFSerializableData, ZFSerializableDataFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/**
 * @brief util method to print serializable data in json format, usually for debug use only
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        )

// ============================================================
/**
 * @brief util method to parse object from input with json format
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFObjectFromJson
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/** @brief see #ZFObjectFromJson */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfauto, ZFObjectFromJson
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/**
 * @brief util method to convert serializable object to json format
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFObjectToJson
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJsonSerializableConverter_h_

