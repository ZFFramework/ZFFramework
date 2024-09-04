/**
 * @file ZFUIButtonBasic.h
 * @brief basic button
 */

#ifndef _ZFI_ZFUIButtonBasic_h_
#define _ZFI_ZFUIButtonBasic_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonBasic
zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
/**
 * @brief basic button, see #ZFUIButton
 *
 * a basic button contains:
 * -  label: to show button's text
 * -  icon: to show button's icon
 * -  background: to show button's background
 *
 * to change the button's property,
 * you must use ZFUIButtonBasic's styleable property,
 * such as #ZFUIButtonBasic::labelNormal\n
 * \n
 * specially, those property's normal state's value would be used
 * if other state doesn't supply value:
 * -  label's #ZFUITextView::text
 * -  icon's #ZFUIImageView::image
 * -  background's #ZFUIImageView::image
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonBasic : zfextend ZFUIButton {
    ZFOBJECT_DECLARE(ZFUIButtonBasic, ZFUIButton)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonBasic)

public:
    // ============================================================
    // content layout
    /**
     * @brief button icon's orientation (relative to label), #ZFUIOrientation::e_Left by default
     */
    ZFPROPERTY_ASSIGN(ZFUIOrientationEnum, iconPosition, ZFUIOrientation::e_Left)
    /**
     * @brief button content(icon, label)'s layout align, #ZFUIAlign::e_Center by default
     */
    ZFPROPERTY_ASSIGN(ZFUIAlignFlags, contentAlign, ZFUIAlign::e_Center)
    /**
     * @brief button content(icon, label)'s layout margin, #ZFUIGlobalStyle::itemMargin by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, contentMargin, ZFUIMarginCreate(ZFUIGlobalStyle::DefaultStyle()->itemMargin()))
    /**
     * @brief button icon and label's space, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN(zffloat, contentSpace, ZFUIGlobalStyle::DefaultStyle()->itemSpace())

    /**
     * @brief button background's layout margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, backgroundMargin, ZFUIMarginZero())

    // ============================================================
    // label
    /** @brief label style in normal state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, labelNormal, zfobj<ZFUITextView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, labelNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, labelNormal)

    /** @brief label style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, labelHighlighted, zfobj<ZFUITextView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, labelHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, labelHighlighted)

    /** @brief label style in checked state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, labelChecked, zfobj<ZFUITextView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, labelChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, labelChecked)

    /** @brief label style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, labelCheckedHighlighted, zfobj<ZFUITextView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, labelCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, labelCheckedHighlighted)

    /** @brief label style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, labelDisabled, zfobj<ZFUITextView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, labelDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, labelDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, labelDisabled)

    // ============================================================
    // icon
    /** @brief icon style in normal state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, iconNormal, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, iconNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, iconNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, iconNormal)

    /** @brief icon style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, iconHighlighted, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, iconHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, iconHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, iconHighlighted)

    /** @brief icon style in checked state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, iconChecked, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, iconChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, iconChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, iconChecked)

    /** @brief icon style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, iconCheckedHighlighted, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, iconCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, iconCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, iconCheckedHighlighted)

    /** @brief icon style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, iconDisabled, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, iconDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, iconDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, iconDisabled)

    // ============================================================
    // background
    /** @brief background style in normal state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, backgroundNormal, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, backgroundNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, backgroundNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, backgroundNormal)

    /** @brief background style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, backgroundHighlighted, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, backgroundHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, backgroundHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, backgroundHighlighted)

    /** @brief background style in checked state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, backgroundChecked, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, backgroundChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, backgroundChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, backgroundChecked)

    /** @brief background style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, backgroundCheckedHighlighted, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, backgroundCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, backgroundCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, backgroundCheckedHighlighted)

    /** @brief background style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, backgroundDisabled, zfobj<ZFUIImageView>())
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, backgroundDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, backgroundDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, backgroundDisabled)

    // ============================================================
    // util method
public:
    /**
     * @brief util method to get button label's style
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFUITextView>, label
            , ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal)
            )
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFUIImageView>, icon
            , ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal)
            )
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFUIImageView>, background
            , ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

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

    zfoverride
    virtual void buttonStateOnUpdate(void);

protected:
    /**
     * @brief must be type of #ZFUITextView, by default it's #ZFUITextView
     */
    virtual const ZFClass *buttonLabelClass(void);
    /**
     * @brief must be type of #ZFUIImageView, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonIconClass(void);
    /**
     * @brief must be type of #ZFUIImageView, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonBackgroundClass(void);

public:
    /**
     * @brief button's label view, style of this view must be changed by #ZFUIButtonBasic::labelNormal, etc
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUITextView>, labelView)
    /**
     * @brief button's icon view, style of this view must be changed by #ZFUIButtonBasic::iconNormal, etc
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIImageView>, iconView)
    /**
     * @brief button's background view, style of this view must be changed by #ZFUIButtonBasic::backgroundNormal, etc
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIImageView>, backgroundView)

private:
    _ZFP_ZFUIButtonBasicPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonBasic_h_

