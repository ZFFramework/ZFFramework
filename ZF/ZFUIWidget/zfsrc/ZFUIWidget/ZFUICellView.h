/**
 * @file ZFUICellView.h
 * @brief abstract cell content view
 */

#ifndef _ZFI_ZFUICellView_h_
#define _ZFI_ZFUICellView_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUICellView
/**
 * @brief abstract cell content view
 */
zfabstract ZFLIB_ZFUIWidget ZFUICellView : zfextend ZFUIView {
    ZFOBJECT_DECLARE_ABSTRACT(ZFUICellView, ZFUIView)

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    virtual zfanyT<ZFUIView> cellIconContainer(void) zfpurevirtual;
    /**
     * @brief container for center item, you may add your own custom view
     */
    virtual zfanyT<ZFUIView> cellCenterContainer(void) zfpurevirtual;
    /**
     * @brief container for accessory, you may add your own custom view
     */
    virtual zfanyT<ZFUIView> cellAccessoryContainer(void) zfpurevirtual;

protected:
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellView_h_

