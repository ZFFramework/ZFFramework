/**
 * @file ZFProtocolZFUIViewCapture.h
 * @brief protocol for #ZFUIViewCapture
 */

#ifndef _ZFI_ZFProtocolZFUIViewCapture_h_
#define _ZFI_ZFProtocolZFUIViewCapture_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIViewCapture.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIViewCapture)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFUIViewCapture
     */
    virtual zfbool viewCapture(
            ZF_IN ZFUIView *view
            , ZF_IN_OUT ZFUIImage *image
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIViewCapture)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewCapture_h_

