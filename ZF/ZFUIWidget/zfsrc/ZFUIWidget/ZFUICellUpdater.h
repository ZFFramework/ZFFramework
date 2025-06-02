/**
 * @file ZFUICellUpdater.h
 * @brief utility to update #ZFUICell
 */

#ifndef _ZFI_ZFUICellUpdater_h_
#define _ZFI_ZFUICellUpdater_h_

#include "ZFUICell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief update param used by #ZFUICellUpdater::cellOnUpdate
 */
zfclassPOD ZFLIB_ZFUIWidget ZFUICellUpdaterParam {
public:
    zfanyT<ZFUICell> cell; /**< @brief the cell */
    zfindex cellIndex; /**< @brief cell's index */
    zfindex cellCount; /**< @brief cell's total count */
    ZFUIOrientation orientation; /**< @brief owner container's orientation */
    ZFUISize containerSize; /**< @brief owner container's size */
    zffloat cellSizeHint; /**< @brief cell's size hint */
};
ZFCORE_POD_DECLARE(ZFUICellUpdaterParam)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUIWidget, ZFUICellUpdaterParam, ZFUICellUpdaterParam)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFUIWidget, ZFUICellUpdaterParam, ZFUICellUpdaterParam)
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFUIWidget, ZFUICellUpdaterParam)

// ============================================================
/**
 * @brief abstract cell updater to update #ZFUICell
 */
zfinterface ZFLIB_ZFUIWidget ZFUICellUpdater : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUICellUpdater, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellOnUpdate,
     * param0 is #v_ZFUICellUpdaterParam
     */
    ZFEVENT(CellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellOnRecycle,
     * param0 is #ZFUICell
     */
    ZFEVENT(CellOnRecycle)

public:
    /** @brief for impl to notify cell update */
    zffinal void notifyCellUpdate(ZF_IN const ZFUICellUpdaterParam &updateParam) {
        this->cellOnUpdate(updateParam);
        this->toObject()->observerNotify(zfself::E_CellOnUpdate(), zfobj<v_ZFUICellUpdaterParam>(updateParam));
    }
    /** @brief for impl to notify cell recycle */
    zffinal void notifyCellRecycle(ZF_IN ZFUICell *cell) {
        this->cellOnRecycle(cell);
        this->toObject()->observerNotify(zfself::E_CellOnRecycle(), cell);
    }

protected:
    /**
     * @brief called to update cell style
     */
    virtual inline void cellOnUpdate(ZF_IN const ZFUICellUpdaterParam &updateParam) {
    }
    /**
     * @brief called to recycle cell
     */
    virtual inline void cellOnRecycle(ZF_IN ZFUICell *cell) {
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
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellUpdater_h_

