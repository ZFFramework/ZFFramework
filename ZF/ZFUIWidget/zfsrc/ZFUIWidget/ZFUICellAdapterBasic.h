/**
 * @file ZFUICellAdapterBasic.h
 * @brief basic cell adapter that allow dynamic extension
 */

#ifndef _ZFI_ZFUICellAdapterBasic_h_
#define _ZFI_ZFUICellAdapterBasic_h_

#include "ZFUICellAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFUICellAdapterBasic::EventCellAt */
zfclass ZFLIB_ZFUIWidget ZFUICellAdapterBasicParam : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUICellAdapterBasicParam, ZFObject)

public:
    /** @brief see #ZFUICellAdapterBasic::EventCellAt */
    ZFPROPERTY_RETAIN(zfanyT<ZFUICell>, cell)
    /** @brief see #ZFUICellAdapterBasic::EventCellAt */
    ZFPROPERTY_ASSIGN(zfindex, cellIndex)
    /** @brief see #ZFUICellAdapterBasic::EventCellAt */
    ZFPROPERTY_ASSIGN(zffloat, cellSize, -1)
};

// ============================================================
/**
 * @brief basic cell adapter that allow dynamic extension
 *
 * this class is designed for script file to implement cell adapter dynamically,
 * typical usage in lua:
 * @code
 *   listView:cellAdapterAutoRetain(ZFUICellAdapterBasic());
 *
 *   -- required
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.EventCellCount(), function(zfargs)
 *       zfargs:param0():zfv(yourCellCount());
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.EventCellAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourCellAt(param:cellIndex()));
 *   end);
 *
 *   -- optional
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.EventCellSizeAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cellSize(yourCellSizeAt(param:cellIndex(), param:cell()));
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.EventCellCacheOnAccess(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourCellCacheAt(param:cellIndex()));
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.EventCellCacheOnRecycle(), function(zfargs)
 *       local cell = zfargs:param0();
 *       yourCellRecycle(cell);
 *   end);
 * @endcode
 */
zfclass ZFLIB_ZFUIWidget ZFUICellAdapterBasic : zfextend ZFStyleableObject, zfimplement ZFUICellAdapter {
    ZFOBJECT_DECLARE(ZFUICellAdapterBasic, ZFStyleableObject)
    ZFIMPLEMENT_DECLARE(ZFUICellAdapter)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCount,
     * param0 is #v_zfindex that holds the cell count
     */
    ZFEVENT(CellCount)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellAt,
     * param0 is #ZFUICellAdapterBasicParam
     */
    ZFEVENT(CellAt)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellSizeAt,
     * param0 is #ZFUICellAdapterBasicParam
     */
    ZFEVENT(CellSizeAt)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCacheOnAccess,
     * param0 is #ZFUICellAdapterBasicParam
     */
    ZFEVENT(CellCacheOnAccess)

public:
    zfoverride
    virtual zfindex cellCount(void) {
        zfobj<v_zfindex> ret;
        this->observerNotify(zfself::EventCellCount(), ret);
        return ret->zfv;
    }
    zfoverride
    virtual zfautoT<ZFUICell> cellAt(ZF_IN zfindex index) {
        zfobj<ZFUICellAdapterBasicParam> param;
        param->cellIndex(index);
        this->observerNotify(zfself::EventCellAt(), param);
        return param->cell();
    }
    zfoverride
    virtual zffloat cellSizeAt(
            ZF_IN zfindex index
            , ZF_IN ZFUICell *cell
            ) {
        if(this->observerHasAdd(zfself::EventCellSizeAt())) {
            zfobj<ZFUICellAdapterBasicParam> param;
            param->cellIndex(index);
            param->cell(cell);
            this->observerNotify(zfself::EventCellSizeAt(), param);
            return param->cellSize();
        }
        else {
            return zfsuperI(ZFUICellAdapter)::cellSizeAt(index, cell);
        }
    }

protected:
    zfoverride
    virtual zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        if(this->observerHasAdd(zfself::EventCellCacheOnAccess())) {
            zfobj<ZFUICellAdapterBasicParam> param;
            param->cellIndex(index);
            this->observerNotify(zfself::EventCellCacheOnAccess(), param);
            return param->cell();
        }
        else {
            return zfsuperI(ZFUICellAdapter)::cellCacheOnAccess(index);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellAdapterBasic_h_

