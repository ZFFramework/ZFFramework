/**
 * @file ZFUIScrollLayout.h
 * @brief scroll view that update content frame automatically
 */

#ifndef _ZFI_ZFUIScrollLayout_h_
#define _ZFI_ZFUIScrollLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScrollLayoutParam
/**
 * @brief scroll view that update content frame automatically
 *
 * #ZFUIScrollView::scrollContentFrame would be updated automatically
 * according to max range of children during layoutOnLayoutPrepare,
 * #ZFUIView::layoutMeasure would also try to measure by children's range
 */
zfclass ZFLIB_ZFUIWidget ZFUIScrollLayout : zfextend ZFUIScrollView {
    ZFOBJECT_DECLARE(ZFUIScrollLayout, ZFUIScrollView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollLayout)

protected:
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollLayout_h_

