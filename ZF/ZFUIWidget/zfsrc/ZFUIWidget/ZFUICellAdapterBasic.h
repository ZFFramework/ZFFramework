/**
 * @file ZFUICellAdapterBasic.h
 * @brief basic cell adapter that allow dynamic extension
 */

#ifndef _ZFI_ZFUICellAdapterBasic_h_
#define _ZFI_ZFUICellAdapterBasic_h_

#include "ZFUICellAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFUICellAdapterBasic::E_CellAt */
zfclass ZFLIB_ZFUIWidget ZFUICellAdapterBasicParam : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUICellAdapterBasicParam, ZFObject)

public:
    /** @brief see #ZFUICellAdapterBasic::E_CellAt */
    ZFPROPERTY_RETAIN(zfanyT<ZFUICell>, cell)
    /** @brief see #ZFUICellAdapterBasic::E_CellAt */
    ZFPROPERTY_ASSIGN(zfindex, cellIndex)
    /** @brief see #ZFUICellAdapterBasic::E_CellAt */
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
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.E_CellCount(), function(zfargs)
 *       zfargs:param0():zfv(yourCellCount());
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.E_CellAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourCellAt(param:cellIndex()));
 *   end);
 *
 *   -- optional
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.E_CellSizeAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cellSize(yourCellSizeAt(param:cellIndex(), param:cell()));
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.E_CellCacheOnAccess(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourCellCacheAt(param:cellIndex()));
 *   end);
 *   listView:cellAdapter():observerAdd(ZFUICellAdapterBasic.E_CellCacheOnRecycle(), function(zfargs)
 *       local cell = zfargs:param0();
 *       yourCellRecycle(cell);
 *   end);
 * @endcode
 */
zfclass ZFLIB_ZFUIWidget ZFUICellAdapterBasic : zfextend ZFStyle, zfimplement ZFUICellAdapter {
    ZFOBJECT_DECLARE(ZFUICellAdapterBasic, ZFStyle)
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
        this->observerNotify(zfself::E_CellCount(), ret);
        return ret->zfv;
    }
    zfoverride
    virtual zfautoT<ZFUICell> cellAt(ZF_IN zfindex index) {
        zfobj<ZFUICellAdapterBasicParam> param;
        param->cellIndex(index);
        this->observerNotify(zfself::E_CellAt(), param);
        return param->cell();
    }
    zfoverride
    virtual zffloat cellSizeAt(
            ZF_IN zfindex index
            , ZF_IN ZFUICell *cell
            ) {
        if(this->observerHasAdd(zfself::E_CellSizeAt())) {
            zfobj<ZFUICellAdapterBasicParam> param;
            param->cellIndex(index);
            param->cell(cell);
            this->observerNotify(zfself::E_CellSizeAt(), param);
            return param->cellSize();
        }
        else {
            return zfsuperI(ZFUICellAdapter)::cellSizeAt(index, cell);
        }
    }

protected:
    zfoverride
    virtual zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        if(this->observerHasAdd(zfself::E_CellCacheOnAccess())) {
            zfobj<ZFUICellAdapterBasicParam> param;
            param->cellIndex(index);
            this->observerNotify(zfself::E_CellCacheOnAccess(), param);
            return param->cell();
        }
        else {
            return zfsuperI(ZFUICellAdapter)::cellCacheOnAccess(index);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellAdapterBasic_h_

