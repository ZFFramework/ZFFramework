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
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIViewTransform)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief apply transform for view
     *
     * things you should impl:
     * -  #ZFUIView::viewTranslateX
     * -  #ZFUIView::viewTranslateY
     * -  #ZFUIView::viewScaleX
     * -  #ZFUIView::viewScaleY
     * -  #ZFUIView::viewRotate
     */
    virtual void viewTransform(ZF_IN ZFUIView *view) zfpurevirtual;

    /**
     * @brief called to reset transform,
     *   ensured called only when transform is not identity,
     *   before view's deallocation
     */
    virtual void viewTransformReset(ZF_IN ZFUIView *view)
    {
        if(!ZFPropertyIsInitValue(ZFPropertyAccess(ZFUIView, viewTranslateX), view))
        {
            view->viewTranslateX(0);
        }
        if(!ZFPropertyIsInitValue(ZFPropertyAccess(ZFUIView, viewTranslateY), view))
        {
            view->viewTranslateY(0);
        }
        if(!ZFPropertyIsInitValue(ZFPropertyAccess(ZFUIView, viewScaleX), view))
        {
            view->viewScaleX(1);
        }
        if(!ZFPropertyIsInitValue(ZFPropertyAccess(ZFUIView, viewScaleY), view))
        {
            view->viewScaleY(1);
        }
        if(!ZFPropertyIsInitValue(ZFPropertyAccess(ZFUIView, viewRotate), view))
        {
            view->viewRotate(0);
        }
        this->viewTransform(view);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIViewTransform)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewTransform_h_

