/**
 * @file ZFUIDialog.h
 * @brief abstract dialog
 */

#ifndef _ZFI_ZFUIDialog_h_
#define _ZFI_ZFUIDialog_h_

#include "ZFUIButton.h"
#include "ZFUIOnScreenKeyboardAutoResize.h"
#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief callback to create default layout param for #ZFUIDialog::dialogLayoutParam
 *
 * this value would be initialized to #ZFUIDialogDefaultLayoutParamCreatorDefault
 * during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow
 */
extern ZFLIB_ZFUIWidget ZFObjectCreator ZFUIDialogDefaultLayoutParamCreator;
/**
 * @brief see #ZFUIDialogDefaultLayoutParamCreator
 *
 * by default, the layout param would be:
 * -  have #ZFUIAlign::e_Center as #ZFUILayoutParam::layoutAlign
 * -  have #ZFUIGlobalStyle::itemMargin as #ZFUILayoutParam::layoutMargin
 */
extern ZFLIB_ZFUIWidget zfauto ZFUIDialogDefaultLayoutParamCreatorDefault(void);
/**
 * @brief util method to call #ZFUIDialogDefaultLayoutParamCreator
 */
inline zfauto ZFUIDialogDefaultLayoutParam(void) {
    return ((ZFUIDialogDefaultLayoutParamCreator != zfnull) ? ZFUIDialogDefaultLayoutParamCreator() : ZFUIDialogDefaultLayoutParamCreatorDefault());
}

/**
 * @brief callback to create default show animation for #ZFUIDialog
 *
 * this value would be initialized to #ZFUIDialogDefaultAniShowCreatorDefault
 * during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow
 */
extern ZFLIB_ZFUIWidget ZFObjectCreator ZFUIDialogDefaultAniShowCreator;
/**
 * @brief see #ZFUIDialogDefaultAniShowCreator
 *
 * by default, the animation would be #ZFAniForGeneric
 * with alpha change
 */
extern ZFLIB_ZFUIWidget zfauto ZFUIDialogDefaultAniShowCreatorDefault(void);
/**
 * @brief util method to call #ZFUIDialogDefaultAniShowCreator
 */
inline zfauto ZFUIDialogDefaultAniShow(void) {
    return ((ZFUIDialogDefaultAniShowCreator != zfnull) ? ZFUIDialogDefaultAniShowCreator() : ZFUIDialogDefaultAniShowCreatorDefault());
}

/**
 * @brief callback to create default hide animation for #ZFUIDialog
 *
 * this value would be initialized to #ZFUIDialogDefaultAniHideCreatorDefault
 * during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow
 */
extern ZFLIB_ZFUIWidget ZFObjectCreator ZFUIDialogDefaultAniHideCreator;
/**
 * @brief see #ZFUIDialogDefaultAniHideCreator
 *
 * by default, the animation would be #ZFAniForGeneric
 * with alpha change
 */
extern ZFLIB_ZFUIWidget zfauto ZFUIDialogDefaultAniHideCreatorDefault(void);
/**
 * @brief util method to call #ZFUIDialogDefaultAniHideCreator
 */
inline zfauto ZFUIDialogDefaultAniHide(void) {
    return ((ZFUIDialogDefaultAniHideCreator != zfnull) ? ZFUIDialogDefaultAniHideCreator() : ZFUIDialogDefaultAniHideCreatorDefault());
}

// ============================================================
// ZFUIDialog
zfclassFwd _ZFP_I_ZFUIDialogPrivate;
/**
 * @brief basic dialog
 *
 * note, all dialog would be forced to hide (by #ZFUIDialog::dialogHideForce)
 * during #ZFFrameworkCleanup's #ZFLevelZFFrameworkLow step
 * for safe resource cleanup
 */
zfclass ZFLIB_ZFUIWidget ZFUIDialog : zfextend ZFStyleableObject {
    ZFOBJECT_DECLARE(ZFUIDialog, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialog)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog show,
     */
    ZFEVENT(DialogBeforeShow)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog show,
     */
    ZFEVENT(DialogAfterShow)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog hide
     */
    ZFEVENT(DialogBeforeHide)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog hide
     */
    ZFEVENT(DialogAfterHide)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called after dialog show, to update focus\n
     * param0 is a #ZFObjectHolder to store the result view to focus
     */
    ZFEVENT(DialogFocusOnUpdate)

public:
    // ============================================================
    // properties
    /**
     * @brief whether this dialog update layout according to #ZFUISysWindow::sysWindowMargin,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, sysWindowMarginShouldApply, zftrue)

    /**
     * @brief dialog's background color, #ZFUIGlobalStyle::colorMask by default
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, dialogWindowColor, ZFUIGlobalStyle::DefaultStyle()->colorMask())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIColor, dialogWindowColor)
    /**
     * @brief whether auto dim window background by an alpha change animation, true by default
     *
     * valid only if #dialogWindowColor is not transparent
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogWindowAutoDim, zftrue)

    /**
     * @brief dialog's layout param, created by #ZFUIDialogDefaultLayoutParamCreator by default
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUILayoutParam>, dialogLayoutParam, ZFUIDialogDefaultLayoutParamCreator())

    /**
     * @brief dialog's show animation, #ZFUIDialogDefaultAniShowCreator by default
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, dialogAniShow, ZFUIDialogDefaultAniShowCreator())

    /**
     * @brief dialog's hide animation, #ZFUIDialogDefaultAniHideCreator by default
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, dialogAniHide, ZFUIDialogDefaultAniHideCreator())

    /**
     * @brief dialog's background image
     *
     * note that the #ZFUIImage::imageNinePatch would be used as dialog content's margin
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, dialogBackgroundImage, zfRes("ZFUIWidget/ZFUIDialog_background.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, dialogBackgroundImage)

    /**
     * @brief whether automatically focus to dialog content, true by default
     *
     * focus would be done by #dialogFocusOnUpdate
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogFocusAutomatically, zftrue)

    /**
     * @brief whether hide when touched outside of the dialog, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogHideWhenTouchOutside)

    /**
     * @brief whether hide when clicked back or esc key, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogHideWhenClickBack)

    /**
     * @brief dialog content, null by default
     *
     * note that this content would be saved when serializing the dialog,
     * subclass should use #ZFUIDialog::dialogInternalContainer to achieve additional logic,
     * which is internal view and won't be serialized
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIView>, dialogView)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIView>, dialogView)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIView>, dialogView)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoResizeStart,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogWindowAutoResize, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, dialogWindowAutoResize)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoFitLayout,
     *   false by default
     *
     * auto fit settings can be changed by #dialogWindowAutoFitLayout
     */
    ZFPROPERTY_ASSIGN(zfbool, dialogWindowAutoFit, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, dialogWindowAutoFit)
    /** @brief see #dialogWindowAutoFit */
    ZFMETHOD_DECLARE_0(ZFUIOnScreenKeyboardAutoFitLayout *, dialogWindowAutoFitLayout)

public:
    /**
     * @brief true if dialog is showing
     */
    ZFMETHOD_DECLARE_0(zfbool, dialogShowing)
    /**
     * @brief show the dialog
     */
    ZFMETHOD_DECLARE_0(void, dialogShow)
    /**
     * @brief hide the dialog
     */
    ZFMETHOD_DECLARE_0(void, dialogHide)
    /**
     * @brief stop any animation and hide the dialog immediately,
     *   usually used to destroy a dialog immediately
     *
     * this method would be called for all showing dialog
     * during #ZFFrameworkCleanup, see #ZFUIDialog for more info
     */
    ZFMETHOD_DECLARE_0(void, dialogHideForce)
    /**
     * @brief util method to attach click listener to specified button,
     *   which would simply hide the dialog
     */
    ZFMETHOD_DECLARE_1(void, dialogApplyAutoHide
            , ZFMP_IN(ZFUIButton *, button)
            )

public:
    /**
     * @brief directly access the window of this dialog
     *
     * by default, the dialog's window would have #ZFUIWindowLevel::e_AppHigh as #ZFUIWindow::windowLevel
     */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, dialogWindow)

protected:
    /**
     * @brief the dialog content container which hold the #ZFUIDialog::dialogView
     *
     * the #ZFUIDialog::dialogView is ensured te be added to this container as normal view\n
     * \n
     * for subclass to achieve additional logic,
     * you should add your own impl views to this container as normal view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, dialogInternalContainer)
    /**
     * @brief the dialog background container which hold the #ZFUIDialog::dialogBackgroundImage
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, dialogInternalBackgroundContainer)

protected:
    /** @brief see #EventDialogBeforeShow */
    virtual inline void dialogBeforeShow(void) {
        this->observerNotify(ZFUIDialog::EventDialogBeforeShow());
    }
    /** @brief see #EventDialogAfterShow */
    virtual inline void dialogAfterShow(void) {
        this->observerNotify(ZFUIDialog::EventDialogAfterShow());
    }
    /** @brief see #EventDialogBeforeHide */
    virtual inline void dialogBeforeHide(void) {
        this->observerNotify(ZFUIDialog::EventDialogBeforeHide());
    }
    /** @brief see #EventDialogAfterHide */
    virtual inline void dialogAfterHide(void) {
        this->observerNotify(ZFUIDialog::EventDialogAfterHide());
    }
    /**
     * @brief see #EventDialogFocusOnUpdate
     *
     * return the view to focus, or null if nothing need to be focused\n
     * by default, dialog would try to find first focusable view to focus by #ZFUIViewFocusNextMove\n
     * you may override this method to supply custom focus logic\n
     * note, this method won't be called if #dialogFocusAutomatically not set
     */
    virtual zfanyT<ZFUIView> dialogFocusOnUpdate(void);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_I_ZFUIDialogPrivate *d;
    friend zfclassFwd _ZFP_I_ZFUIDialogPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialog_h_

