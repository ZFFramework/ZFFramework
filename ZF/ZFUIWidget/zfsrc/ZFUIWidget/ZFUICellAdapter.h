/**
 * @file ZFUICellAdapter.h
 * @brief abstract cell adpater to supply cells for container view
 */

#ifndef _ZFI_ZFUICellAdapter_h_
#define _ZFI_ZFUICellAdapter_h_

#include "ZFUICell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief abstract cell adpater to supply cells for container view
 */
zfinterface ZFLIB_ZFUIWidget ZFUICellAdapter : zfextend ZFInterface {
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUICellAdapter, ZFInterface)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when cell adapter's content changed
     * and owner container view needs to be updated,
     * param0 is null to update entire container,
     * or a #v_zfindex shows which cell to reload\n
     * typically, this event would be fired by #cellAdapterNotifyReload
     * which would be called by cell adapter impl
     */
    ZFEVENT(CellAdapterOnReload)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when update cell,
     * param0 is the #ZFUICell,
     * param1 is the index of the cell (as #v_zfindex)
     */
    ZFEVENT(CellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when recycle cell,
     * param0 is the cell
     */
    ZFEVENT(CellCacheOnRecycle)

    // ============================================================
    // owner state
private:
    ZFUIOrientation _ZFP_ZFUICellAdapter_orientation;
public:
    /**
     * @brief owner view's orientation
     */
    ZFMETHOD_DECLARE_0(ZFUIOrientation, orientation)

private:
    ZFUISize _ZFP_ZFUICellAdapter_containerSize;
public:
    /**
     * @brief container's size
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, containerSize)

public:
    /**
     * @brief cell's hint size,
     *   use -1 to measure by cell,
     *   #ZFUIGlobalStyle::itemSizeCell by default
     *
     * setting #cellSizeFill would override this property,
     * and use #containerSize to layout each cell
     */
    ZFPROPERTY_ASSIGN(zffloat, cellSizeHint, ZFUIGlobalStyle::DefaultStyle()->itemSizeCell())
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, cellSizeHint)

    /**
     * @brief if #cellSizeHint not set, whether fill cell size to #containerSize,
     *   false by default
     *
     * this property is useful when combined with #ZFUIScrollView::scrollAlignToPageHorizontal series
     */
    ZFPROPERTY_ASSIGN(zfbool, cellSizeFill, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, cellSizeFill)

    // ============================================================
    // basic cell access
public:
    /**
     * @brief see #E_CellAdapterOnReload
     */
    ZFMETHOD_DECLARE_1(void, cellAdapterNotifyReload
            , ZFMP_IN_OPT(zfindex, atIndexOrMax, zfindexMax())
            )
    /**
     * @brief cell count
     */
    virtual zfindex cellCount(void) zfpurevirtual;
    /**
     * @brief access cell at index, assert fail if return null object
     */
    virtual zfautoT<ZFUICell> cellAt(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief check cell size at index
     *
     * for performance, this method is ensured to be called for visible cells only,
     * to check the cell's actual size\n
     * while cell is not visible,
     * a predicted hint size (or previous size) would be used
     * util it's coming to visible\n
     * return a -1 size to measure the cell automatically,
     * otherwise, the cell's size is fixed\n
     * return #cellSizeHint according to #cellSizeFill by default
     */
    ZFMETHOD_DECLARE_2(zffloat, cellSizeAt
            , ZFMP_IN(zfindex, index)
            , ZFMP_IN(ZFUICell *, cell)
            )

public:
    /** @brief for impl to notify state update */
    zffinal void notifyContainerUpdate(
            ZF_IN ZFUIOrientation orientation
            , ZF_IN const ZFUISize &containerSize
            ) {
        _ZFP_ZFUICellAdapter_orientation = orientation;
        _ZFP_ZFUICellAdapter_containerSize = containerSize;
    }
    /** @brief for impl to notify reload */
    zffinal void notifyCellAdapterReload(ZF_IN_OPT zfindex atIndexOrMax = zfindexMax()) {
        this->cellAdapterOnReload(atIndexOrMax);
    }
    /** @brief for impl to notify cell update */
    zffinal void notifyCellUpdate(
            ZF_IN zfindex atIndex
            , ZF_IN ZFUICell *cell
            ) {
        this->cellOnUpdate(atIndex, cell);
    }
    /** @brief for impl to notify cell cache access */
    zffinal zfauto notifyCellCacheAccess(ZF_IN zfindex index) {
        return this->cellCacheOnAccess(index);
    }
    /** @brief for impl to notify cell cache access */
    zffinal void notifyCellCacheRecycle(ZF_IN ZFUICell *cell) {
        this->cellCacheOnRecycle(cell);
        this->toObject()->observerNotify(zfself::E_CellCacheOnRecycle(), cell);
    }

    // ============================================================
    // cell update callback
protected:
    /**
     * @brief see #E_CellAdapterOnReload
     */
    virtual inline void cellAdapterOnReload(ZF_IN_OPT zfindex atIndexOrMax = zfindexMax()) {
        this->toObject()->observerNotify(zfself::E_CellAdapterOnReload(),
            (atIndexOrMax == zfindexMax()) ? zfnull : zfobj<v_zfindex>(atIndexOrMax).toObject());
    }
    /**
     * @brief see #E_CellOnUpdate
     */
    virtual inline void cellOnUpdate(
            ZF_IN zfindex atIndex
            , ZF_IN ZFUICell *cell
            ) {
        this->toObject()->observerNotify(zfself::E_CellOnUpdate(), cell, zfobj<v_zfindex>(atIndex));
    }

    // ============================================================
    // cache logic
protected:
    /**
     * @brief try access cached cell, return null means no cache available,
     *   by default, no cache logic support
     *
     * see #cellCacheOnRecycle for more info
     */
    virtual inline zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        return zfnull;
    }
    /**
     * @brief recycle cell or do nothing if you don't need recycle logic
     *
     * to achieve recycle logic, you should:
     * -# override #cellCacheOnRecycle and store the cell manually
     * -# during #cellCacheOnAccess, return your cached cell if exist
     *
     * by default, we have #cellCacheDefaultAccess and #cellCacheDefaultRecycle
     * that would suit for most case
     */
    virtual inline void cellCacheOnRecycle(ZF_IN ZFUICell *cell) {
    }

public:
    /** @brief see #cellCacheOnRecycle */
    ZFMETHOD_DECLARE_1(zfautoT<ZFUICell>, cellCacheDefaultAccess
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief see #cellCacheOnRecycle */
    ZFMETHOD_DECLARE_2(void, cellCacheDefaultRecycle
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(ZFUICell *, cell)
            )

protected:
    /** @cond ZFPrivateDoc */
    ZFUICellAdapter(void)
    : _ZFP_ZFUICellAdapter_orientation(ZFUIOrientation::e_Top)
    , _ZFP_ZFUICellAdapter_containerSize(ZFUISizeZero())
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellAdapter_h_

