/**
 * @file ZFXmlSerializableConverter.h
 * @brief serializable convertion between xml and serializalbe
 */

#ifndef _ZFI_ZFXmlSerializableConverter_h_
#define _ZFI_ZFXmlSerializableConverter_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief parse xml formated data to serializable
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataFromXml
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const ZFXml &, xmlElement)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFXml *, outErrorPos, zfnull)
        )
/**
 * @brief see #ZFSerializableDataFromXml
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFSerializableData, ZFSerializableDataFromXml
        , ZFMP_IN(const ZFXml &, xmlElement)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFXml *, outErrorPos, zfnull)
        )
/**
 * @brief print serializable to xml formated data
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataToXml
        , ZFMP_OUT(ZFXml &, xmlElement)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
/**
 * @brief see #ZFSerializableDataToXml
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFXml, ZFSerializableDataToXml
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )

// ============================================================
/**
 * @brief util method to parse serializable data from input with xml format
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataFromXml
        , ZFMP_OUT(ZFSerializableData &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/** @brief see #ZFSerializableDataFromXml */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFSerializableData, ZFSerializableDataFromXml
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/**
 * @brief util method to convert serializable object to xml format
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFSerializableDataToXml
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        )

// ============================================================
/**
 * @brief util method to parse object from input with xml format
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFObjectFromXmlT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/** @brief see #ZFObjectFromXmlT */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfauto, ZFObjectFromXml
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
/**
 * @brief util method to convert serializable object to xml format
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfbool, ZFObjectToXml
        , ZFMP_IN(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXmlSerializableConverter_h_

