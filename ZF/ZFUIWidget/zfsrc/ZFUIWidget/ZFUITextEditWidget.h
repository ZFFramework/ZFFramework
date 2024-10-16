/**
 * @file ZFUITextEditWidget.h
 * @brief basic text edit widget
 */

#ifndef _ZFI_ZFUITextEditWidget_h_
#define _ZFI_ZFUITextEditWidget_h_

#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditWidget
/**
 * @brief basic text edit widget
 */
zfclass ZFLIB_ZFUIWidget ZFUITextEditWidget : zfextend ZFUITextEdit {
    ZFOBJECT_DECLARE(ZFUITextEditWidget, ZFUITextEdit)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextEditWidget)

public:
    // ============================================================
    // properties
    /**
     * @brief the background view
     *
     * note, image's nine patch would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, bgView, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, bgView)

    /**
     * @brief the clear button
     *
     * note, the clear button's size would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIButtonBasic>, clearButton, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIButtonBasic>, clearButton)

    /**
     * @brief whether automatically show or hide clear button, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, clearButtonAutoEnable, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, clearButtonAutoEnable)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void textOnUpdate(ZF_IN const zfstring &oldText);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextEditWidget_h_

