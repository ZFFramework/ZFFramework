/**
 * @file ZFUIListCell.h
 * @brief abstract list cell to hold list content views
 */

#ifndef _ZFI_ZFUIListCell_h_
#define _ZFI_ZFUIListCell_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListCell
/**
 * @brief utility class to hold list cell
 *
 * a #ZFUIListView won't supply separators or margins or border radius directly,
 * it's achieved by a wrapper view, which is this #ZFUIListCell\n
 * typically, the extra features are implemented by utility methods described in #ZFUIListCellUpdater
 */
zfclass ZFLIB_ZFUIWidget ZFUIListCell : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIListCell, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListCell)

    /**
     * @brief the content view
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIView>, cellView)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIView>, cellView)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIView>, cellView)

    /**
     * @brief content view's layout param,
     *   usually would be updated by #ZFUIListCellUpdater
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
#endif // #ifndef _ZFI_ZFUIListCell_h_

