/**
 * @file ZFProtocolZFUIImageView.h
 * @brief protocol for ZFUIImageView
 */

#ifndef _ZFI_ZFProtocolZFUIImageView_h_
#define _ZFI_ZFProtocolZFUIImageView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIImageView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIImageView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIImageView)
public:
    /**
     * @brief create native image view
     */
    virtual void *nativeImageViewCreate(
            ZF_IN ZFUIImageView *imageView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) zfpurevirtual;
    /**
     * @brief destroy native image view
     */
    virtual void nativeImageViewDestroy(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN void *nativeImageView
            ) zfpurevirtual;

    /**
     * @brief set the image
     *
     * implementations must not retain the image object,
     * it's ensured set null when owner image view deallocated\n
     * note that implementation must implement the nine patch logic
     */
    virtual void image(
            ZF_IN ZFUIImageView *imageView
            , ZF_IN ZFUIImage *image
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIImageView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIImageView_h_

