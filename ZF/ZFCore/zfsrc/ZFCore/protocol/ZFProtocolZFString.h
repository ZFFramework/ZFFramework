/**
 * @file ZFProtocolZFString.h
 * @brief protocol for zfstringW
 */

#ifndef _ZFI_ZFProtocolZFString_h_
#define _ZFI_ZFProtocolZFString_h_

#include "ZFCore/ZFProtocol.h"
#include "../zfstringW.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for zfstringW
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFCore, ZFString)
public:
    /**
     * @brief see #zfstringToUTF8
     */
    virtual zfbool toUTF8(
            ZF_OUT zfstring &result
            , ZF_IN const void *s
            , ZF_IN ZFStringEncoding srcEncoding
            ) zfpurevirtual;
    /**
     * @brief see #zfstringToUTF16
     */
    virtual zfbool toUTF16(
            ZF_OUT zfstringW &result
            , ZF_IN const void *s
            , ZF_IN ZFStringEncoding srcEncoding
            ) zfpurevirtual;
    /**
     * @brief see #zfstringToUTF16BE
     */
    virtual zfbool toUTF16BE(
            ZF_OUT zfstringW &result
            , ZF_IN const void *s
            , ZF_IN ZFStringEncoding srcEncoding
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFString_h_

