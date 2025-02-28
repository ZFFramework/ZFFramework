/**
 * @file ZFUIViewFocus.h
 * @brief ZFUIView focus utility
 */

#ifndef _ZFI_ZFUIViewFocus_h_
#define _ZFI_ZFUIViewFocus_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief manually set the next focus target for the from view, set null to remove
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, void, ZFUIViewFocusNextSetup
        , ZFMP_IN(ZFUIView *, from)
        , ZFMP_IN(ZFUIView *, nextFocus)
        )

// ============================================================
/**
 * @brief filter to exclude certain view from being focused by #ZFUIViewFocusNextFind, empty by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIKit, ZFFilterForZFObject, ZFUIViewFocusNextFilter)

/**
 * @brief param for #ZFUIViewFocusNextFind and #ZFUIViewFocusNextMove
 */
zfclassLikePOD ZFLIB_ZFUIKit ZFUIViewFocusNextParam {
    ZFCORE_PARAM_DECLARE_SELF(ZFUIViewFocusNextParam)

    /**
     * @brief direction to find, #v_ZFUIOrientation::e_Right | #v_ZFUIOrientation::e_Bottom by default
     */
    ZFCORE_PARAM(ZFUIOrientationFlags, focusDirection, (ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom))
    /**
     * @brief whether find in loop mode, false by default
     */
    ZFCORE_PARAM(zfbool, focusLoopMode, zffalse)
    /**
     * @brief only find within this view, null to end with #ZFUIWindow, null by default
     */
    ZFCORE_PARAM(zfanyT<ZFUIView>, focusEndParent)
    /**
     * @brief whether to find internal views, true by default
     */
    ZFCORE_PARAM(zfbool, focusInternalViews, zftrue)

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIViewFocusNextParam &ref) const {
        return (zftrue
                && this->focusDirection() == ref.focusDirection()
                && this->focusLoopMode() == ref.focusLoopMode()
                && this->focusEndParent() == ref.focusEndParent()
                && this->focusInternalViews() == ref.focusInternalViews()
            );
    }
    zfbool operator != (ZF_IN const ZFUIViewFocusNextParam &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIViewFocusNextParam, ZFUIViewFocusNextParam)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFUIKit, ZFUIViewFocusNextParam, ZFUIViewFocusNextParam)
ZFOUTPUT_TYPE(ZFUIViewFocusNextParam, {v.objectInfoT(s);})

// ============================================================
/**
 * @brief find next focusable from view (excluding) with direction
 *
 * return first focusable view if found
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfanyT<ZFUIView>, ZFUIViewFocusNextFind
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam())
        )

/**
 * @brief move focus to next if available or do nothing otherwise, see #ZFUIViewFocusNextFind
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfanyT<ZFUIView>, ZFUIViewFocusNextMove
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam())
        )

// ============================================================
/**
 * @brief util method to resolve key event (such as up/down/left/right keys) and check to move focus, see #ZFUIViewFocusNextFind
 *
 * return true if the event has been resolved,
 * next focused view can be checked by the optional nextFocus param
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIKit, zfbool, ZFUIViewFocusResolveKeyEvent
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIKeyEvent *, keyEvent)
        , ZFMP_OUT_OPT(zfauto *, nextFocus, zfnull)
        , ZFMP_IN_OPT(ZFUIView *, endParent, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewFocus_h_

