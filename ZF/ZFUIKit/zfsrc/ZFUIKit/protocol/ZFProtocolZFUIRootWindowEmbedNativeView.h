/**
 * @file ZFProtocolZFUIRootWindowEmbedNativeView.h
 * @brief protocol for ZFUIRootWindow
 */

#ifndef _ZFI_ZFProtocolZFUIRootWindowEmbedNativeView_h_
#define _ZFI_ZFProtocolZFUIRootWindowEmbedNativeView_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIRootWindow.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIRootWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIRootWindowEmbedNativeView)
public:
    /**
     * @brief used to attach window's root view to native container
     *
     * the attached child must fill parent,
     * and change frame according to parent's size automatically
     */
    virtual void nativeViewAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) zfpurevirtual;
    /**
     * @brief remove native view, see #nativeViewAdd
     */
    virtual void nativeViewRemove(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN_OUT void *&parent
            , ZF_IN void *child
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIRootWindowEmbedNativeView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIRootWindowEmbedNativeView_h_

