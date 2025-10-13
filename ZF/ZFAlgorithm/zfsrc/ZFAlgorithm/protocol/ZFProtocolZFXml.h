/**
 * @file ZFProtocolZFXml.h
 * @brief protocol for ZFXml
 */

#ifndef _ZFI_ZFProtocolZFXml_h_
#define _ZFI_ZFProtocolZFXml_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFXml
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFAlgorithm, ZFXml)
public:
    /**
     * @brief see #ZFXmlEscapeCharEncode
     */
    virtual void xmlEscapeCharEncode(
            ZF_OUT const ZFOutput &dst
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            );
    /**
     * @brief see #ZFXmlEscapeCharEncode
     */
    virtual void xmlEscapeCharDecode(
            ZF_OUT const ZFOutput &dst
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            );

public:
    /**
     * @brief parse xml document
     *
     * you must parse the DOM tree and save to document,
     * which including:
     * -  name for each xml item
     * -  value for each xml item
     * -  CDATA for text node
     */
    virtual ZFXml xmlParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief parse xml document
     */
    virtual ZFXml xmlParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFXml)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFXml_h_

