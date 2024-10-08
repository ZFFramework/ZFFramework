/**
 * @file ZFUIListAdapter.h
 * @brief abstract list adpater to supply cells for list view
 */

#ifndef _ZFI_ZFUIListAdapter_h_
#define _ZFI_ZFUIListAdapter_h_

#include "ZFUIListCell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIListViewPrivate;
zfclassFwd ZFUIListView;
/**
 * @brief abstract list adpater to supply cells for list view
 */
zfinterface ZFLIB_ZFUIWidget ZFUIListAdapter : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFUIListAdapter, ZFInterface)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list adapter's content changed
     * and owner list view needs to be updated,
     * param0 is null to update entire list,
     * or a #v_zfindex shows which cell to reload\n
     * typically, this event would be fired by #listAdapterNotifyReload
     * which would be called by list adapter impl
     */
    ZFEVENT(ListAdapterOnReload)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when update list cell,
     * param0 is the #ZFUIListCell,
     * param1 is the index of the list cell (as #v_zfindex)
     */
    ZFEVENT(ListCellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when recycle list cell,
     * param0 is the list cell
     */
    ZFEVENT(ListCellCacheOnRecycle)

    // ============================================================
    // owner state
private:
    ZFUIOrientationEnum _ZFP_ZFUIListAdapter_orientation;
public:
    /**
     * @brief list's orientation
     */
    ZFMETHOD_DECLARE_0(ZFUIOrientationEnum, orientation)

private:
    ZFUISize _ZFP_ZFUIListAdapter_containerSize;
public:
    /**
     * @brief list container's size
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, containerSize)

public:
    /**
     * @brief list cell's hint size,
     *   use -1 to measure by list cell,
     *   #ZFUIGlobalStyle::itemSizeListCell by default
     *
     * setting #cellSizeFill would override this property,
     * and use #containerSize to layout each cell
     */
    ZFPROPERTY_ASSIGN(zffloat, cellSizeHint, ZFUIGlobalStyle::DefaultStyle()->itemSizeListCell())
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
    // basic list cell access
public:
    /**
     * @brief see #EventListAdapterOnReload
     */
    ZFMETHOD_DECLARE_1(void, listAdapterNotifyReload
            , ZFMP_IN_OPT(zfindex, atIndexOrMax, zfindexMax())
            )
    /**
     * @brief cell count
     */
    virtual zfindex cellCount(void) zfpurevirtual;
    /**
     * @brief access list cell at index, assert fail if return null object
     */
    virtual zfautoT<ZFUIListCell> cellAt(ZF_IN zfindex index) zfpurevirtual;
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
            , ZFMP_IN(ZFUIListCell *, cell)
            )

    // ============================================================
    // list update callback
protected:
    /**
     * @brief see #EventListAdapterOnReload
     */
    virtual inline void listAdapterOnReload(ZF_IN_OPT zfindex atIndexOrMax = zfindexMax()) {
        this->toObject()->observerNotify(zfself::EventListAdapterOnReload(),
            (atIndexOrMax == zfindexMax()) ? zfnull : zfobj<v_zfindex>(atIndexOrMax).toObject());
    }
    /**
     * @brief see #EventListCellOnUpdate
     */
    virtual inline void cellOnUpdate(
            ZF_IN zfindex atIndex
            , ZF_IN ZFUIListCell *cell
            ) {
        this->toObject()->observerNotify(zfself::EventListCellOnUpdate(), cell, zfobj<v_zfindex>(atIndex));
    }

    // ============================================================
    // cache logic
protected:
    /**
     * @brief try access cached list cell, return null means no cache available,
     *   by default, no cache logic support
     *
     * see #cellCacheOnRecycle for more info
     */
    virtual inline zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        return zfnull;
    }
    zffinal inline void _ZFP_ZFUIListAdapter_cellCacheOnRecycle(ZF_IN ZFUIListCell *cell) {
        this->cellCacheOnRecycle(cell);
        this->toObject()->observerNotify(zfself::EventListCellCacheOnRecycle(), cell);
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
    virtual inline void cellCacheOnRecycle(ZF_IN ZFUIListCell *cell) {
    }

public:
    /** @brief see #cellCacheOnRecycle */
    ZFMETHOD_DECLARE_1(zfautoT<ZFUIListCell>, cellCacheDefaultAccess
            , ZFMP_IN(const zfstring &, key)
            )
    /** @brief see #cellCacheOnRecycle */
    ZFMETHOD_DECLARE_2(void, cellCacheDefaultRecycle
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(ZFUIListCell *, cell)
            )

private:
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
    friend zfclassFwd ZFUIListView;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapter_h_

