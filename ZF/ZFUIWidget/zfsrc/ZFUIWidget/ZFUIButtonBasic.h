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
zfclass ZFLIB_ZFUIWidget ZFUIButtonBasic : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(ZFUIButtonBasic, ZFUIButton)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonBasic)

public:
    // ============================================================
    // content layout
    /**
     * @brief button icon's orientation (relative to label), #ZFUIOrientation::e_Left by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIOrientationEnum, iconPosition, ZFUIOrientation::e_Left)
    /**
     * @brief button content(icon, label)'s layout align, #ZFUIAlign::e_Center by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, contentAlign, ZFUIAlign::e_Center)
    /**
     * @brief button content(icon, label)'s layout margin, #ZFUIGlobalStyle::itemMargin by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, contentMargin, ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()))
    /**
     * @brief button icon and label's space, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, contentSpace, ZFUIGlobalStyle::DefaultStyle()->itemSpace())

    /**
     * @brief button background's layout margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, backgroundMargin, ZFUIMarginZero())

    // ============================================================
    // label
    /** @brief label style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, labelNormal,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, labelNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, labelNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, labelNormal)

    /** @brief label style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, labelHighlighted,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, labelHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, labelHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, labelHighlighted)

    /** @brief label style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, labelChecked,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, labelChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, labelChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, labelChecked)

    /** @brief label style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, labelCheckedHighlighted,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, labelCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, labelCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, labelCheckedHighlighted)

    /** @brief label style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, labelDisabled,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, labelDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, labelDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, labelDisabled)

    // ============================================================
    // icon
    /** @brief icon style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, iconNormal,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, iconNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, iconNormal)

    /** @brief icon style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, iconHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, iconHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, iconHighlighted)

    /** @brief icon style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, iconChecked,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, iconChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, iconChecked)

    /** @brief icon style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, iconCheckedHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, iconCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, iconCheckedHighlighted)

    /** @brief icon style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, iconDisabled,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, iconDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, iconDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, iconDisabled)

    // ============================================================
    // background
    /** @brief background style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, backgroundNormal,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, backgroundNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, backgroundNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, backgroundNormal)

    /** @brief background style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, backgroundHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, backgroundHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, backgroundHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, backgroundHighlighted)

    /** @brief background style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, backgroundChecked,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, backgroundChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, backgroundChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, backgroundChecked)

    /** @brief background style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, backgroundCheckedHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, backgroundCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, backgroundCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, backgroundCheckedHighlighted)

    /** @brief background style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, backgroundDisabled,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, backgroundDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, backgroundDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, backgroundDisabled)

    // ============================================================
    // util method
public:
    /**
     * @brief util method to get button label's style
     */
    ZFMETHOD_DECLARE_1(ZFUITextView *, label,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(ZFUIImageView *, icon,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(ZFUIImageView *, background,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
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
    ZFMETHOD_DECLARE_0(ZFUITextView *, labelView)
    /**
     * @brief button's icon view, style of this view must be changed by #ZFUIButtonBasic::iconNormal, etc
     */
    ZFMETHOD_DECLARE_0(ZFUIImageView *, iconView)
    /**
     * @brief button's background view, style of this view must be changed by #ZFUIButtonBasic::backgroundNormal, etc
     */
    ZFMETHOD_DECLARE_0(ZFUIImageView *, backgroundView)

private:
    _ZFP_ZFUIButtonBasicPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonBasic_h_

