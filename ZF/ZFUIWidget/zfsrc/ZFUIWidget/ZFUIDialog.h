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
 * @brief default layout param for #ZFUIDialog::layoutParam
 *
 * by default, the layout param would be:
 * -  have #v_ZFUIAlign::e_Center as #ZFUILayoutParam::align
 * -  have #ZFUIGlobalStyle::itemMargin as #ZFUILayoutParam::margin
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, ZFListener, ZFUIDialogLayoutParamCreator)
/**
 * @brief default animation for #ZFUIDialog::aniShow
 *
 * by default, the animation would be #ZFAniForGeneric
 * with alpha change
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, ZFListener, ZFUIDialogAniShowCreator)
/**
 * @brief default animation for #ZFUIDialog::aniHide
 *
 * by default, the animation would be #ZFAniForGeneric
 * with alpha change
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, ZFListener, ZFUIDialogAniHideCreator)
/**
 * @brief default #ZFUIWindow::windowLevel for #ZFUIDialog, 1000 by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIWidget, zfint, ZFUIDialogWindowLevel)

// ============================================================
// ZFUIDialog
zfclassFwd _ZFP_I_ZFUIDialogPrivate;
/**
 * @brief basic dialog
 *
 * note, all dialog would be forced to hide (by #ZFUIDialog::hideImmediately)
 * during #ZFFrameworkCleanup's #ZFLevelZFFrameworkLow step
 * for safe resource cleanup
 */
zfclass ZFLIB_ZFUIWidget ZFUIDialog : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFUIDialog, ZFStyle)
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
     * param0 is a #v_zfwrap to pass the result view to focus,
     * you may modify by #zfwrap::set
     */
    ZFEVENT(DialogFocusOnUpdate)

public:
    // ============================================================
    // properties
    /**
     * @brief whether this dialog update layout according to #ZFUIRootWindow::windowMargin,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, windowMarginShouldApply, zftrue)

    /**
     * @brief dialog's background color, #ZFUIGlobalStyle::colorMask by default
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, windowColor, ZFUIGlobalStyle::DefaultStyle()->colorMask())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIColor, windowColor)
    /**
     * @brief whether auto dim window background by an alpha change animation, true by default
     *
     * valid only if #windowColor is not transparent
     */
    ZFPROPERTY_ASSIGN(zfbool, windowAutoDim, zftrue)

    /**
     * @brief dialog's layout param
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUILayoutParam>, layoutParam, ZFObjectCreator(ZFUIDialogLayoutParamCreator()))

    /**
     * @brief dialog's show animation
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, aniShow, ZFObjectCreator(ZFUIDialogAniShowCreator()))

    /**
     * @brief dialog's hide animation
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFAnimation>, aniHide, ZFObjectCreator(ZFUIDialogAniHideCreator()))

    /**
     * @brief dialog's background image
     *
     * note that the #ZFUIImage::imageNinePatch would be used as dialog content's margin
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, bgImage, zfres("ZFUIWidget/ZFUIDialog_bg.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, bgImage)

    /**
     * @brief whether automatically focus to dialog content, true by default
     *
     * focus would be done by #dialogFocusOnUpdate
     */
    ZFPROPERTY_ASSIGN(zfbool, autoFocus, zftrue)

    /**
     * @brief whether hide when touched outside of the dialog, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, hideWhenTouchOutside)

    /**
     * @brief whether hide when clicked back or esc key, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, hideWhenClickBack)

    /**
     * @brief dialog content, null by default
     *
     * note that this content would be saved when serializing the dialog,
     * subclass should use #ZFUIDialog::dialogContainer to achieve additional logic,
     * which is internal view and won't be serialized
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIView>, content)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIView>, content)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIView>, content)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoResizeStart,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, windowAutoResize, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, windowAutoResize)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoFitLayout,
     *   false by default
     *
     * auto fit settings can be changed by #windowAutoFitLayout
     */
    ZFPROPERTY_ASSIGN(zfbool, windowAutoFit, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, windowAutoFit)
    /** @brief see #windowAutoFit */
    ZFMETHOD_DECLARE_0(ZFUIOnScreenKeyboardAutoFitLayout *, windowAutoFitLayout)

public:
    /**
     * @brief true if dialog is showing
     */
    ZFMETHOD_DECLARE_0(zfbool, showing)
    /**
     * @brief show the dialog
     */
    ZFMETHOD_DECLARE_0(void, show)
    /**
     * @brief hide the dialog
     */
    ZFMETHOD_DECLARE_0(void, hide)
    /**
     * @brief stop any animation and hide the dialog immediately,
     *   usually used to destroy a dialog immediately
     *
     * this method would be called for all showing dialog
     * during #ZFFrameworkCleanup, see #ZFUIDialog for more info
     */
    ZFMETHOD_DECLARE_0(void, hideImmediately)
    /**
     * @brief util method to attach click listener to specified button,
     *   which would simply hide the dialog
     */
    ZFMETHOD_DECLARE_1(void, autoHideForButton
            , ZFMP_IN(ZFUIButton *, button)
            )

public:
    /**
     * @brief directly access the window of this dialog
     *
     * by default, the dialog's window would have #ZFUIDialogWindowLevel as #ZFUIWindow::windowLevel
     */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, window)

protected:
    /**
     * @brief the dialog content container which hold the #ZFUIDialog::content
     *
     * the #ZFUIDialog::content is ensured te be added to this container as normal view\n
     * \n
     * for subclass to achieve additional logic,
     * you should add your own impl views to this container as normal view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, dialogContainer)
    /**
     * @brief the dialog background container which hold the #ZFUIDialog::bgImage
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, dialogBackgroundContainer)

protected:
    /** @brief see #E_DialogBeforeShow */
    virtual inline void dialogBeforeShow(void) {
        this->observerNotify(ZFUIDialog::E_DialogBeforeShow());
    }
    /** @brief see #E_DialogAfterShow */
    virtual inline void dialogAfterShow(void) {
        this->observerNotify(ZFUIDialog::E_DialogAfterShow());
    }
    /** @brief see #E_DialogBeforeHide */
    virtual inline void dialogBeforeHide(void) {
        this->observerNotify(ZFUIDialog::E_DialogBeforeHide());
    }
    /** @brief see #E_DialogAfterHide */
    virtual inline void dialogAfterHide(void) {
        this->observerNotify(ZFUIDialog::E_DialogAfterHide());
    }
    /**
     * @brief see #E_DialogFocusOnUpdate
     *
     * return the view to focus, or null if nothing need to be focused\n
     * by default, dialog would try to find first focusable view to focus by #ZFUIViewFocusNextMove\n
     * you may override this method to supply custom focus logic\n
     * note, this method won't be called if #autoFocus not set
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

