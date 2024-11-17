/**
 * @file ZFUICell.h
 * @brief abstract cell to hold content views
 */

#ifndef _ZFI_ZFUICell_h_
#define _ZFI_ZFUICell_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUICell
/**
 * @brief utility class to hold cell
 *
 * a #ZFUIListView won't supply separators or margins or border radius directly,
 * it's achieved by a wrapper view, which is this #ZFUICell\n
 * typically, the extra features are implemented by utility methods described in #ZFUICellUpdater
 */
zfclass ZFLIB_ZFUIWidget ZFUICell : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUICell, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUICell)

    /**
     * @brief the content view
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIView>, cellView)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIView>, cellView)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIView>, cellView)

    /**
     * @brief content view's layout param,
     *   usually would be updated by #ZFUICellUpdater
     *   to achieve additional features,
     *   fill parent and align center by default
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUILayoutParam>, cellViewLayoutParam, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUILayoutParam>, cellViewLayoutParam)

protected:
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICell_h_

