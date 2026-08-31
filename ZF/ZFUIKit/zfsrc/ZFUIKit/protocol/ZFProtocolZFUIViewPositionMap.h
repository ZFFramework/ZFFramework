/**
 * @file ZFProtocolZFUIViewPositionMap.h
 * @brief protocol for #ZFUIViewPositionMap
 */

#ifndef _ZFI_ZFProtocolZFUIViewPositionMap_h_
#define _ZFI_ZFProtocolZFUIViewPositionMap_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIViewPositionMap
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIViewPositionMap)
public:
    /**
     * @brief see #ZFUIViewPositionMap
     */
    virtual void viewPositionMap(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN ZFUIView *view
            , ZF_IN_OPT const ZFUIPoint &localPos = ZFUIPointZero()
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIViewPositionMap)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewPositionMap_h_

