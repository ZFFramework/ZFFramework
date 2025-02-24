/**
 * @file ZFUIRootView.h
 * @brief root view for holding a ZFUIView's tree
 */

#ifndef _ZFI_ZFUIRootView_h_
#define _ZFI_ZFUIRootView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUISysWindow;
zfclassFwd ZFUIWindow;
/**
 * @brief root view for holding a ZFUIView's tree
 *
 * internally used to manage view's,
 * usually you should not use this class
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIRootView : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIRootView, ZFUIView)
    ZFOBJECT_PRIVATE_ALLOC("can only be created by owner ZFUISysWindow")

public:
    /**
     * @brief owner #ZFUISysWindow
     */
    zffinal zfanyT<ZFUISysWindow> const &rootViewOwnerSysWindow(void) {
        return this->_ZFP_ZFUIRootView_rootViewOwnerSysWindow;
    }

    /**
     * @brief all #ZFUIWindow attached to the root view
     */
    ZFMETHOD_DECLARE_0(const ZFCoreArray<zfautoT<ZFUIWindow> > &, windowList)

protected:
    zfoverride
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    zfoverride
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

protected:
    /**
     * @brief for a root view, this method would always return sizeHint
     */
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

public:
    zfanyT<ZFUISysWindow> _ZFP_ZFUIRootView_rootViewOwnerSysWindow;
    ZFCoreArray<zfautoT<ZFUIWindow> > _ZFP_ZFUIRootView_windowList;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIRootView_h_

