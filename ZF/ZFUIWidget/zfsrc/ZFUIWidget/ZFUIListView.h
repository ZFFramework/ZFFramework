/**
 * @file ZFUIListView.h
 * @brief list view
 */

#ifndef _ZFI_ZFUIListView_h_
#define _ZFI_ZFUIListView_h_

#include "ZFUIListAdapter.h"
#include "ZFUIListCellUpdater.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListView
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIListView_listAdapter "listAdapter"

zfclassFwd _ZFP_ZFUIListViewPrivate;
/**
 * @brief list view that layout children in linear by specified orientation
 *
 * a list layout is a scroll view, you may manage the scroll logic by #ZFUIScrollView's methods,
 * while scroll view's content size would be updated automatically
 * and you should not modify it manually
 * @warning you must not add or remove normal child to list view,
 *   only internal background or foreground view is allowed
 *
 * to use a list view, you must supply a list adapter which would supply all of list cells,
 * see #ZFUIListAdapter for more info\n
 * list view itself won't supplly any separator or margins or border radius,
 * you must supply by the list adapter,
 * see #ZFUIListCell for more info \n
 * \n
 * serializable data:
 * @code
 *   <ZFUIListView>
 *       <ListAdapterClass category="listAdapter" /> // used only if #listAdapterSerializable
 *       <ZFArray prop="cellUpdater"> // all cell updater
 *           ...
 *       </ZFArray>
 *   </ZFUIListView>
 * @endcode
 */
zfclass ZFLIB_ZFUIWidget ZFUIListView : zfextend ZFUIScrollView {
    ZFOBJECT_DECLARE(ZFUIListView, ZFUIScrollView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list cell attached to this list view,
     * param0 is the list cell
     * @note you must not access #visibleCells or #visibleCellRange
     *   during this event,
     *   you can only access them during #EventListVisibleCellOnUpdate
     */
    ZFEVENT(ListCellOnAttach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list cell detached from this list view,
     * param0 is the list cell
     * @note you must not access #visibleCells or #visibleCellRange
     *   during this event,
     *   you can only access them during #EventListVisibleCellOnUpdate
     */
    ZFEVENT(ListCellOnDetach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when list visible cell changed
     */
    ZFEVENT(ListVisibleCellOnUpdate)

public:
    // ============================================================
    // properties
    /**
     * @brief list adapter
     *
     * no auto retain by default, use #listAdapterAutoRetain if necessary\n
     * by default, the list adapter won't be serialized during the list view's serialization,
     * you can enable it by #listAdapterSerializable
     */
    ZFPROPERTY_ASSIGN(zfanyT<ZFUIListAdapter>, listAdapter)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIListAdapter>, listAdapter)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIListAdapter>, listAdapter)
    /**
     * @brief set and retain the list adapter
     *
     * by default, list adapter won't be retained by this list view to prevent recursive retain,
     * you may use this method to retain it automatically
     */
    ZFMETHOD_DECLARE_1(void, listAdapterAutoRetain
            , ZFMP_IN(ZFUIListAdapter *, listAdapter)
            )

    /**
     * @brief whether the #listAdapter is serializable, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, listAdapterSerializable)

    /**
     * @brief list updater to update list cells, holds #ZFUIListCellUpdater
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, cellUpdater, zfobj<ZFArray>())

    /**
     * @brief direction to layout children, #ZFUIOrientation::e_Top by default
     *
     * #ZFUIOrientation::e_Left means layout children from left to right,
     * while #ZFUIOrientation::e_Bottom means layout children from bottom to top
     */
    ZFPROPERTY_ASSIGN(ZFUIOrientationEnum, orientation, ZFUIOrientation::e_Top)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIOrientationEnum, orientation)

    /**
     * @brief whether auto update #ZFUIScrollView::scrollBounceVertical series according to #orientation,
     *   true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, bounceable, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, bounceable)

    // ============================================================
    // override
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /**
     * @brief see #ZFSerializable::serializableOnCheckPropertyType
     *
     * we will ignore:
     * -  #ZFUIScrollView::scrollContentFrame
     * -  #ZFUIScrollView::scrollBounceVertical
     * -  #ZFUIScrollView::scrollBounceHorizontal
     * -  #ZFUIScrollView::scrollBounceVerticalAlways
     * -  #ZFUIScrollView::scrollBounceHorizontalAlways
     */
    zfoverride
    virtual ZFSerializablePropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    /**
     * @brief all child views would be ignored from serializable
     */
    zfoverride
    virtual zfbool serializableOnCheckNeedSerializeChildren(void) {
        return zffalse;
    }
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

    zfoverride
    virtual void viewChildOnAdd(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayerEnum layer
            );
    zfoverride
    virtual void viewChildOnRemove(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayerEnum layer
            );

    zfoverride
    virtual void scrollAreaOnUpdate(void);
    zfoverride
    virtual void scrollContentFrameOnUpdate(void);

protected:
    zfoverride
    virtual void scrollOnScrolledByUser(void);
    zfoverride
    virtual void scrollOnScrollEnd(void);

    // ============================================================
    // list control
public:
    /**
     * @brief reload entire list cells
     *
     * @note this method would only schedule an update step,
     * when it would be reloaded is not ensured\n
     * @note this method would reload the entire list,
     *   use #reloadCellAt for performance
     */
    ZFMETHOD_DECLARE_0(void, reload)
    /**
     * @brief true if list need reload
     */
    ZFMETHOD_DECLARE_0(zfbool, reloadRequested)
    /**
     * @brief reload cell at index immediately, do nothing if index not in visible range
     *   or #reloadRequested
     *
     * this method would have higher performance if you want to update specified cell only
     */
    ZFMETHOD_DECLARE_1(void, reloadCellAt
            , ZFMP_IN(zfindex, index)
            )

public:
    /**
     * @brief return a list of current visible cells, valid only if #reloadRequested is not true
     *
     * returned value should not be stored,
     * since visible cell may change frequently
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<ZFUIListCell *>, visibleCells)
    /**
     * @brief return first visible cell's index, valid only if #reloadRequested is not true
     *
     * index is ordered by #ZFUIListAdapter,
     * so first cell may positioned at bottom according to layout orientation
     */
    ZFMETHOD_DECLARE_0(const ZFIndexRange &, visibleCellRange)

public:
    /**
     * @brief scroll cell to top, do nothing if invalid or no need to scroll
     *
     * if animated, scroll task would be canceled if scrolled by user (#ZFUIScrollView::scrollOnScrolledByUser)
     * @note since list cell may load dynamically,
     *   this method would recursively scroll until reached desired position,
     *   you should prevent doing other load logic during scroll events,
     *   otherwise, dead loop may occurred
     */
    ZFMETHOD_DECLARE_3(void, scrollCellToHead
            , ZFMP_IN(zfindex, cellIndex)
            , ZFMP_IN_OPT(zffloat, offset, 0)
            , ZFMP_IN_OPT(zfbool, animated, zftrue)
            )
    /**
     * @brief scroll cell to bottom, see #scrollCellToHead
     */
    ZFMETHOD_DECLARE_3(void, scrollCellToTail
            , ZFMP_IN(zfindex, cellIndex)
            , ZFMP_IN_OPT(zffloat, offset, 0)
            , ZFMP_IN_OPT(zfbool, animated, zftrue)
            )

    // ============================================================
    // events
protected:
    /** @brief see #EventListCellOnAttach */
    virtual inline void cellOnAttach(ZF_IN ZFUIListCell *cell) {
        this->observerNotify(ZFUIListView::EventListCellOnAttach(), cell);
    }
    /** @brief see #EventListCellOnDetach */
    virtual inline void cellOnDetach(ZF_IN ZFUIListCell *cell) {
        this->observerNotify(ZFUIListView::EventListCellOnDetach(), cell);
    }
    /** @brief see #EventListVisibleCellOnUpdate */
    virtual void visibleCellsOnUpdate(void);

private:
    _ZFP_ZFUIListViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListView_h_

