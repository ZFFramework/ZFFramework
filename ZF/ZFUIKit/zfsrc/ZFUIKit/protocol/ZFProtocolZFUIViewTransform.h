/**
 * @file ZFProtocolZFUIViewTransform.h
 * @brief protocol for ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIViewTransform_h_
#define _ZFI_ZFProtocolZFUIViewTransform_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIViewTransform)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /** @brief see #ZFUIView::transformAvailable */
    virtual ZFUITransformFlags transformAvailable(void) zfpurevirtual;

    /** @brief see #ZFUIView::transformAvailable */
    virtual void viewTransform(ZF_IN ZFUIView *view) zfpurevirtual;

    /**
     * @brief called to reset transform,
     *   ensured called only when transform is not identity,
     *   before view's deallocation
     */
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, translateX), view)) {
            view->translateX(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, translateY), view)) {
            view->translateY(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, translateZ), view)) {
            view->translateZ(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, scaleX), view)) {
            view->scaleX(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, scaleY), view)) {
            view->scaleY(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, scaleZ), view)) {
            view->scaleZ(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, rotateX), view)) {
            view->rotateX(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, rotateY), view)) {
            view->rotateY(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, rotateZ), view)) {
            view->rotateZ(0);
        }
        this->viewTransform(view);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIViewTransform)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewTransform_h_

