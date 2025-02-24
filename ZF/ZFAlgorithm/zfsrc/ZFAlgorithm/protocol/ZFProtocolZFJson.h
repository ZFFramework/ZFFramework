/**
 * @file ZFProtocolZFJson.h
 * @brief protocol for ZFJson
 */

#ifndef _ZFI_ZFProtocolZFJson_h_
#define _ZFI_ZFProtocolZFJson_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFJson
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFAlgorithm, ZFJson)
public:
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharEncode(
            ZF_OUT const ZFOutput &dst
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            );
    /**
     * @brief see #ZFJsonEscapeCharEncode
     */
    virtual void jsonEscapeCharDecode(
            ZF_OUT const ZFOutput &dst
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            );

public:
    /**
     * @brief parse json
     */
    virtual ZFJson jsonParse(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex size = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief parse json
     */
    virtual ZFJson jsonParse(
            ZF_IN const ZFInput &inputCallback
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFJson)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFJson_h_

