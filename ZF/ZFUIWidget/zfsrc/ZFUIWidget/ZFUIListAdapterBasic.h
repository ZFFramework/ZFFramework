/**
 * @file ZFUIListAdapterBasic.h
 * @brief basic list adapter that allow dynamic extension
 */

#ifndef _ZFI_ZFUIListAdapterBasic_h_
#define _ZFI_ZFUIListAdapterBasic_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFUIListAdapterBasic::EventListCellAt */
zfclass ZFLIB_ZFUIWidget ZFUIListAdapterBasicParam : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIListAdapterBasicParam, ZFObject)

public:
    /** @brief see #ZFUIListAdapterBasic::EventListCellAt */
    ZFPROPERTY_RETAIN(ZFUIListCell *, cell)
    /** @brief see #ZFUIListAdapterBasic::EventListCellAt */
    ZFPROPERTY_ASSIGN(zfindex, cellIndex)
    /** @brief see #ZFUIListAdapterBasic::EventListCellAt */
    ZFPROPERTY_ASSIGN(zffloat, cellSize, -1)
};

// ============================================================
/**
 * @brief basic list adapter that allow dynamic extension
 *
 * this class is designed for script file to implement list adapter dynamically,
 * typical usage in lua:
 * @code
 *   listView:listAdapterAutoRetain(ZFUIListAdapterBasic());
 *
 *   -- required
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCount(), function(zfargs)
 *       zfargs:param0():zfv(yourListCellCount());
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourListCellAt(param:cellIndex()));
 *   end);
 *
 *   -- optional
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellSizeAt(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cellSize(yourListCellSizeAt(param:cellIndex(), param:cell()));
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCacheOnAccess(), function(zfargs)
 *       local param = zfargs:param0();
 *       param:cell(yourListCellCacheAt(param:cellIndex()));
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCacheOnRecycle(), function(zfargs)
 *       local cell = zfargs:param0();
 *       yourListCellRecycle(cell);
 *   end);
 * @endcode
 */
zfclass ZFLIB_ZFUIWidget ZFUIListAdapterBasic : zfextend ZFStyleableObject, zfimplement ZFUIListAdapter {
    ZFOBJECT_DECLARE(ZFUIListAdapterBasic, ZFStyleableObject)
    ZFIMPLEMENT_DECLARE(ZFUIListAdapter)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCount,
     * param0 is #v_zfindex that holds the cell count
     */
    ZFOBSERVER_EVENT(ListCellCount)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellAt,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellAt)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellSizeAt,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellSizeAt)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCacheOnAccess,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellCacheOnAccess)

public:
    zfoverride
    virtual zfindex cellCount(void) {
        zfblockedAlloc(v_zfindex, ret);
        this->observerNotify(zfself::EventListCellCount(), ret);
        return ret->zfv;
    }
    zfoverride
    virtual zfauto cellAt(ZF_IN zfindex index) {
        zfblockedAlloc(ZFUIListAdapterBasicParam, param);
        param->cellIndex(index);
        this->observerNotify(zfself::EventListCellAt(), param);
        return param->cell();
    }
    zfoverride
    virtual zffloat cellSizeAt(
            ZF_IN zfindex index
            , ZF_IN ZFUIListCell *cell
            ) {
        if(this->observerHasAdd(zfself::EventListCellSizeAt())) {
            zfblockedAlloc(ZFUIListAdapterBasicParam, param);
            param->cellIndex(index);
            param->cell(cell);
            this->observerNotify(zfself::EventListCellSizeAt(), param);
            return param->cellSize();
        }
        else {
            return zfsuperI(ZFUIListAdapter)::cellSizeAt(index, cell);
        }
    }

protected:
    zfoverride
    virtual zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        if(this->observerHasAdd(zfself::EventListCellCacheOnAccess())) {
            zfblockedAlloc(ZFUIListAdapterBasicParam, param);
            param->cellIndex(index);
            this->observerNotify(zfself::EventListCellCacheOnAccess(), param);
            return param->cell();
        }
        else {
            return zfsuperI(ZFUIListAdapter)::cellCacheOnAccess(index);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterBasic_h_

