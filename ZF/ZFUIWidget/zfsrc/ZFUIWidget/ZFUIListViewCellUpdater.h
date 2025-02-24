/**
 * @file ZFUIListViewCellUpdater.h
 * @brief common updater of #ZFUICellUpdater
 */

#ifndef _ZFI_ZFUIListViewCellUpdater_h_
#define _ZFI_ZFUIListViewCellUpdater_h_

#include "ZFUICellUpdater.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief basic updater that insert separator to list cell
 */
zfclass ZFLIB_ZFUIWidget ZFUIListViewCellUpdater : zfextend ZFStyle, zfimplement ZFUICellUpdater {
    ZFOBJECT_DECLARE(ZFUIListViewCellUpdater, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFUICellUpdater)

public:
    /**
     * @brief separator size, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, separatorSize, 1)

    /**
     * @brief separator color, #ZFUIGlobalStyle::colorSeparator by default
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, separatorColor, ZFUIGlobalStyle::DefaultStyle()->colorSeparator())

    /**
     * @brief separator margin, #ZFUIMarginZero by default
     *
     * note, for vertical list view, only left and right margin take effect,
     * and for horizontal list view, only top and bottom margin take effect
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, separatorMargin)

    /**
     * @brief whether add separator before first list cell, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, separatorIncludingHead)
    /**
     * @brief whether add separator after last list cell, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, separatorIncludingTail)

public:
    zfoverride
    virtual void cellOnUpdate(ZF_IN const ZFUICellUpdaterParam &updateParam);
    zfoverride
    virtual void cellOnRecycle(ZF_IN ZFUICell *cell);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListViewCellUpdater_h_

