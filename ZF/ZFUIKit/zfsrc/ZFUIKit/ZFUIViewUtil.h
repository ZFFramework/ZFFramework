/**
 * @file ZFUIViewUtil.h
 * @brief utils for ZFUIView
 */

#ifndef _ZFI_ZFUIViewUtil_h_
#define _ZFI_ZFUIViewUtil_h_

#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN_REGISTER(ZFUIViewUtil, ZF_NAMESPACE_GLOBAL)

/**
 * @brief recursive find the root ZFUIView parent of this view,
 *   may return self if no parent
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, zfanyT<ZFUIView>, viewRoot
        , ZFMP_IN(ZFUIView *, view)
        )

/**
 * @brief check whether this view is child of specified view (recursively)
 *
 * true if view equals to this view
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfbool, isChildOf
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIView *, parentToCheck)
        )

/**
 * @brief recursively check child at position (relative to the view)
 *
 * may be invalid if layout step not finished
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIKit, zfanyT<ZFUIView>, viewAtPos
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(const ZFUIPoint &, pos)
        , ZFMP_IN_OPT(zfbool, filterDisabledView, zffalse)
        , ZFMP_IN_OPT(zfbool, filterInternalView, zftrue)
        )

/**
 * @brief check rect of view relative to parent,
 *   return #ZFUIRectZero if not child of parent or invalid
 *
 * result value would be invalid until whole layout step finished\n
 * unlike #ZFUIViewPositionOnScreen,
 * this method does not depends on impl,
 * however, result may be invalid if view's parent contains #ZFUINativeView\n
 * this method would calculate rect depends on #ZFUIView::layoutChildOffset recursively
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, void, viewRectToParentT
        , ZFMP_OUT(ZFUIRect &, rect)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(ZFUIView *, parent, zfnull)
        )
/** @brief see #viewRectToParentT */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, viewRectToParent
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(ZFUIView *, parent, zfnull)
        )

ZF_NAMESPACE_END(ZFUIViewUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewUtil_h_

