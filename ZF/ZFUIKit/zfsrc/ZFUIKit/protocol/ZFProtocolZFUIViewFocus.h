/**
 * @file ZFProtocolZFUIViewFocus.h
 * @brief protocol for #ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIViewFocus_h_
#define _ZFI_ZFProtocolZFUIViewFocus_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIViewFocus)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief change view's focusable setting
     */
    virtual void focusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focusable
            ) zfpurevirtual;
    /**
     * @brief see #ZFUIView::focused
     */
    virtual zfbool focused(ZF_IN ZFUIView *view) zfpurevirtual;
    /**
     * @brief see #ZFUIView::focusFind
     */
    virtual ZFUIView *focusFind(ZF_IN ZFUIView *view) {
        if(view->focused()) {
            return view;
        }
        ZFUIView *ret = zfnull;
        ZFCoreArray<zfautoT<ZFUIView> > children = view->childArray();
        for(zfindex i = 0; i < children.count(); ++i) {
            ret = this->focusFind(children[i]);
            if(ret != zfnull) {
                return ret;
            }
        }
        children = view->internalFgViewArray();
        for(zfindex i = 0; i < children.count(); ++i) {
            ret = this->focusFind(children[i]);
            if(ret != zfnull) {
                return ret;
            }
        }
        children = view->internalBgViewArray();
        for(zfindex i = 0; i < children.count(); ++i) {
            ret = this->focusFind(children[i]);
            if(ret != zfnull) {
                return ret;
            }
        }
        children = view->internalImplViewArray();
        for(zfindex i = 0; i < children.count(); ++i) {
            ret = this->focusFind(children[i]);
            if(ret != zfnull) {
                return ret;
            }
        }
        return zfnull;
    }
    /**
     * @brief see #ZFUIView::focusRequest
     */
    virtual void focusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focus
            ) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when view's focus state changed (both obtain or resign)
     */
    zffinal void notifyViewFocusUpdate(ZF_IN ZFUIView *view) {
        view->_ZFP_ZFUIView_focusOnUpdate();
    }
ZFPROTOCOL_INTERFACE_END(ZFUIViewFocus)

/**
 * @brief convenient method to call notifyViewFocusUpdate of optional protocol ZFUIViewFocus
 */
inline void ZFProtocolZFUIViewFocus_notifyViewFocusUpdate(ZF_IN ZFUIView *view) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull) {
        impl->notifyViewFocusUpdate(view);
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewFocus_h_

