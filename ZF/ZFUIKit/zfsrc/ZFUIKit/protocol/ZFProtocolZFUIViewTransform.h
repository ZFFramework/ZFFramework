/**
 * @file ZFProtocolZFUIViewTransform.h
 * @brief protocol for ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIViewTransform_h_
#define _ZFI_ZFProtocolZFUIViewTransform_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIViewTransform)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /** @brief see #ZFUIView::viewTransformAvailable */
    virtual ZFUITransformFlags viewTransformAvailable(void) zfpurevirtual;

    /** @brief see #ZFUIView::viewTransformAvailable */
    virtual void viewTransform(ZF_IN ZFUIView *view) zfpurevirtual;

    /**
     * @brief called to reset transform,
     *   ensured called only when transform is not identity,
     *   before view's deallocation
     */
    virtual void viewTransformReset(ZF_IN ZFUIView *view)
    {
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewTranslateX), view))
        {
            view->viewTranslateX(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewTranslateY), view))
        {
            view->viewTranslateY(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewTranslateZ), view))
        {
            view->viewTranslateZ(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewScaleX), view))
        {
            view->viewScaleX(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewScaleY), view))
        {
            view->viewScaleY(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewScaleZ), view))
        {
            view->viewScaleZ(1);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewRotateX), view))
        {
            view->viewRotateX(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewRotateY), view))
        {
            view->viewRotateY(0);
        }
        if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIView, viewRotateZ), view))
        {
            view->viewRotateZ(0);
        }
        this->viewTransform(view);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIViewTransform)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewTransform_h_

