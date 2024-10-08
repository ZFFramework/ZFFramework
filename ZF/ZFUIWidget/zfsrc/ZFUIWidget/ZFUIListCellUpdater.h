/**
 * @file ZFUIListCellUpdater.h
 * @brief utility to update #ZFUIListCell
 */

#ifndef _ZFI_ZFUIListCellUpdater_h_
#define _ZFI_ZFUIListCellUpdater_h_

#include "ZFUIListCell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief update param used by #ZFUIListCellUpdater::cellOnUpdate
 */
zfclassPOD ZFLIB_ZFUIWidget ZFUIListCellUpdaterParam {
public:
    zfanyT<ZFUIListCell> cell; /**< @brief the cell */
    zfindex cellIndex; /**< @brief cell's index */
    zfindex cellCount; /**< @brief cell's total count */
    ZFUIOrientationEnum orientation; /**< @brief owner list container's orientation */
    ZFUISize containerSize; /**< @brief owner list container's size */
    zffloat cellSizeHint; /**< @brief list cell's size hint */
};
ZFCORE_POD_DECLARE(ZFUIListCellUpdaterParam)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUIWidget, ZFUIListCellUpdaterParam, ZFUIListCellUpdaterParam)
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFUIWidget, ZFUIListCellUpdaterParam)
ZFCORE_POD_COMPARER_DECLARE(ZFUIListCellUpdaterParam)

// ============================================================
zfclassFwd ZFUIListView;
zfclassFwd _ZFP_ZFUIListViewPrivate;
/**
 * @brief abstract list cell updater to update #ZFUIListCell
 */
zfinterface ZFLIB_ZFUIWidget ZFUIListCellUpdater : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIListCellUpdater, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellOnUpdate,
     * param0 is #v_ZFUIListCellUpdaterParam
     */
    ZFEVENT(CellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellOnRecycle,
     * param0 is #ZFUIListCell
     */
    ZFEVENT(CellOnRecycle)

protected:
    /**
     * @brief called to update cell style
     */
    virtual inline void cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam) {
    }
    /**
     * @brief called to recycle cell
     */
    virtual inline void cellOnRecycle(ZF_IN ZFUIListCell *cell) {
    }

public:
    /**
     * @brief util method for impl to achieve cache logic
     */
    ZFMETHOD_DECLARE_1(zfauto, itemCacheAccess
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief see #itemCacheAccess */
    ZFMETHOD_DECLARE_2(void, itemCacheRecycle
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(ZFObject *, cache)
            )

private:
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
    friend zfclassFwd ZFUIListView;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellUpdater_h_

