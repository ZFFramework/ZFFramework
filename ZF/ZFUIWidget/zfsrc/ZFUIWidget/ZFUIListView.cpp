#include "ZFUIListView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIListView)

// ============================================================
// _ZFP_ZFUIListViewPrivate
zfclassNotPOD _ZFP_ZFUIListViewPrivate {
public:
    ZFUIListView *pimplOwner;
    zfbool childOverrideFlag;
    zfbool scrollContentFrameOverrideFlag;
    ZFUICellAdapter *cellAdapter;
    ZFListener cellAdapterOnReloadListener;
    zfbool reloadRequested;
    zfbool listQuickReloadRequested;
    zfbool cellNeedUpdate;
    zfindex cellCount;
    ZFCoreArray<zffloat> cellSizeList;
    ZFCoreArray<ZFUICell *> visibleCells; // retain manually
    ZFIndexRange visibleCellRange;
    /*
     * left: left most cell's x
     * top: top most cell's y
     * right: right most cell's (x + width)
     * bottom: bottom most cell's (y + height)
     */
    zffloat visibleCellsOffset;
    zfbool visibleCellsOffsetNeedUpdate;
    zfbool reloadByUpdateListOrientation;

    /*
     * used by scrollCellToHead/Tail
     * if activating, we will recursively scrollByPoint until reach desired position
     * task would be canceled if content frame changed manually
     */
    zfindex scrollCellIndex; // zfindexMax() if not activating
    zffloat scrollCellOffset;
    zfbool scrollCellToHead;
    zfbool scrollCellAnimated;
    zffloat scrollCellDesiredPosSaved;

public:
    _ZFP_ZFUIListViewPrivate(void)
    : pimplOwner(zfnull)
    , childOverrideFlag(zffalse)
    , scrollContentFrameOverrideFlag(zffalse)
    , cellAdapter(zfnull)
    , cellAdapterOnReloadListener()
    , reloadRequested(zftrue)
    , listQuickReloadRequested(zftrue)
    , cellNeedUpdate(zftrue)
    , cellCount(0)
    , cellSizeList()
    , visibleCells()
    , visibleCellRange(ZFIndexRangeZero())
    , visibleCellsOffset(0)
    , visibleCellsOffsetNeedUpdate(zftrue)
    , reloadByUpdateListOrientation(zftrue)
    , scrollCellIndex(zfindexMax())
    , scrollCellOffset(0)
    , scrollCellToHead(zftrue)
    , scrollCellAnimated(zftrue)
    , scrollCellDesiredPosSaved(0)
    {
    }

public:
    static void cellAdapterOnReload(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFUIListView *listView
            ) {
        v_zfindex *atIndex = zfargs.param0();
        if(atIndex == zfnull || atIndex->zfv == zfindexMax()) {
            listView->reload();
        }
        else {
            listView->reloadCellAt(atIndex->zfv);
        }
    }
    void bounceableUpdate(void) {
        if(this->pimplOwner->bounceable()) {
            switch(this->pimplOwner->orientation()) {
                case v_ZFUIOrientation::e_Left:
                case v_ZFUIOrientation::e_Right:
                    this->pimplOwner->scrollBounceVerticalAlways(zffalse);
                    this->pimplOwner->scrollBounceHorizontalAlways(zftrue);
                    break;
                case v_ZFUIOrientation::e_Top:
                case v_ZFUIOrientation::e_Bottom:
                    this->pimplOwner->scrollBounceVerticalAlways(zftrue);
                    this->pimplOwner->scrollBounceHorizontalAlways(zffalse);
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return;
            }
        }
    }
    void cellAdapterNotifyUpdate(void) {
        if(this->cellAdapter != zfnull) {
            this->cellAdapter->notifyContainerUpdate(
                    this->pimplOwner->orientation()
                    , ZFUIRectGetSize(this->pimplOwner->scrollArea())
                    );
        }
    }
    void cellAdapterAfterAttach(void) {
        if(this->cellAdapter != zfnull) {
            if(!this->cellAdapterOnReloadListener) {
                ZFLISTENER_1(callback
                        , ZFUIListView *, pimplOwner
                        ) {
                    _ZFP_ZFUIListViewPrivate::cellAdapterOnReload(zfargs, pimplOwner);
                } ZFLISTENER_END()
                this->cellAdapterOnReloadListener = callback;
            }
            this->cellAdapter->toObject()->observerAdd(
                ZFUICellAdapter::E_CellAdapterOnReload(),
                this->cellAdapterOnReloadListener);
            this->cellAdapterNotifyUpdate();
        }
    }
    void cellAdapterBeforeDetach(void) {
        if(this->cellAdapter) {
            this->cellAdapter->toObject()->observerRemove(
                ZFUICellAdapter::E_CellAdapterOnReload(),
                this->cellAdapterOnReloadListener);
        }
    }

public:
    void child(
            ZF_IN ZFUIView *child
            , ZF_IN_OPT zfindex index = zfindexMax()
            ) {
        this->childOverrideFlag = zftrue;
        this->pimplOwner->child(child, index);
        this->childOverrideFlag = zffalse;
    }
    void childRemoveAt(ZF_IN zfindex index) {
        this->childOverrideFlag = zftrue;
        this->pimplOwner->childRemoveAt(index);
        this->childOverrideFlag = zffalse;
    }
    zfautoT<ZFUICell> cellLoadAt(ZF_IN zfindex index) {
        zfautoT<ZFUICell> ret = this->cellAdapter->notifyCellCacheAccess(index);
        if(ret != zfnull) {
            ZFCoreAssertWithMessage(ret != zfnull, "list cell %s not type of %s",
                ret->classData()->classNameFull(),
                ZFUICell::ClassData()->classNameFull());
            return ret;
        }
        ret = this->cellAdapter->cellAt(index);
        ZFCoreAssertWithMessage(ret != zfnull, "cellAt must return a %s", ZFUICell::ClassData()->classNameFull());
        ZFUICell *cell = ret;
        ZFCoreAssertWithMessage(cell != zfnull, "list cell %s not type of %s",
            ret->classData()->classNameFull(),
            ZFUICell::ClassData()->classNameFull());
        return ret;
    }
    void cellOnUpdate(
            ZF_IN zfindex index
            , ZF_IN ZFUICell *cell
            ) {
        if(!this->pimplOwner->cellUpdater()->isEmpty()) {
            ZFUICellUpdaterParam updateParam;
            updateParam.cell = cell;
            updateParam.cellIndex = index;
            updateParam.cellCount = this->cellCount;
            updateParam.orientation = this->cellAdapter->orientation();
            updateParam.containerSize = this->cellAdapter->containerSize();
            updateParam.cellSizeHint = this->cellAdapter->cellSizeHint();
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i) {
                ZFUICellUpdater *cellUpdater = this->pimplOwner->cellUpdater()->get(i);
                cellUpdater->notifyCellUpdate(updateParam);
            }
        }
        this->cellAdapter->notifyCellUpdate(index, cell);
    }
    void cellOnRecycle(ZF_IN ZFUICell *cell) {
        if(!this->pimplOwner->cellUpdater()->isEmpty()) {
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i) {
                ZFUICellUpdater *cellUpdater = this->pimplOwner->cellUpdater()->get(i);
                cellUpdater->notifyCellRecycle(cell);
            }
        }
        this->cellAdapter->notifyCellCacheRecycle(cell);
    }
    zffloat cellSizeAt(
            ZF_IN zfindex index
            , ZF_IN ZFUICell *cell
            ) {
        zffloat ret = this->cellAdapter->cellSizeAt(index, cell);
        if(ret < 0) {
            switch(this->pimplOwner->orientation()) {
                case v_ZFUIOrientation::e_Left:
                case v_ZFUIOrientation::e_Right:
                    ret = cell->layoutMeasure(
                            ZFUISizeCreate(-1, this->pimplOwner->scrollArea().height),
                            ZFUISizeParamWrapFill()
                        ).width;
                    break;
                case v_ZFUIOrientation::e_Top:
                case v_ZFUIOrientation::e_Bottom:
                    ret = cell->layoutMeasure(
                            ZFUISizeCreate(this->pimplOwner->scrollArea().width, -1),
                            ZFUISizeParamFillWrap()
                        ).height;
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
        }
        return ret;
    }
    void removeAll(void) {
        this->cellCount = 0;
        this->cellSizeList.removeAll();
        this->visibleCellRange = ZFIndexRangeZero();
        this->visibleCellsOffset = 0;
        if(!this->visibleCells.isEmpty()) {
            for(zfindex i = this->visibleCells.count() - 1; i != zfindexMax(); --i) {
                ZFUICell *cell = this->visibleCells[i];
                this->childRemoveAt(i);
                if(this->cellAdapter != zfnull) {
                    this->cellOnRecycle(cell);
                }
                this->pimplOwner->cellOnDetach(cell);
                zfRelease(cell);
            }
            this->visibleCells.removeAll();
        }
    }
    void cellRemoveBefore(ZF_IN zfindex index) {
        if(index < this->visibleCellRange.start || index == zfindexMax()) {
            return;
        }

        zfindex indexOfVisibleCell = index - this->visibleCellRange.start;
        for(zfindex i = indexOfVisibleCell; i != zfindexMax(); --i) {
            this->childRemoveAt(i);
            ZFUICell *cell = this->visibleCells[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->visibleCells.remove(0, indexOfVisibleCell + 1);
        this->visibleCellRange.start = index + 1;
        this->visibleCellRange.count = this->visibleCells.count();
        if(this->visibleCellRange.count == 0) {
            this->visibleCellRange.start = 0;
        }
    }
    void cellRemoveAfter(ZF_IN zfindex index) {
        if(index >= this->visibleCellRange.start + this->visibleCellRange.count) {
            return;
        }

        zfindex indexOfVisibleCell = index - this->visibleCellRange.start;
        for(zfindex i = this->visibleCellRange.count - 1; i != zfindexMax() && i >= indexOfVisibleCell; --i) {
            this->childRemoveAt(i);
            ZFUICell *cell = this->visibleCells[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->visibleCells.remove(indexOfVisibleCell, zfindexMax());
        this->visibleCellRange.count = this->visibleCells.count();
        if(this->visibleCellRange.count == 0) {
            this->visibleCellRange.start = 0;
        }
    }
    void updateHeadCellBeforeIndex(
            ZF_IN zfindex index
            , ZF_IN const ZFUIRect &cellFrame
            ) {
        this->scrollContentFrameOverrideFlag = zftrue;

        ZFUIMargin scrollMargin = ZFUIMarginInc(
            this->pimplOwner->nativeImplViewMargin(),
            this->pimplOwner->scrollAreaMargin());
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        const ZFUIRect &viewFrame = this->pimplOwner->viewFrame();

        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left: {
                zffloat offset = cellFrame.x + cellFrame.width;
                zffloat offsetEnd = zfmMax<zffloat>(
                    -scrollContentFrame.x - scrollMargin.left,
                    0);
                zffloat offsetBegin = zfmMin<zffloat>(
                    offsetEnd + viewFrame.width,
                    scrollContentFrame.width);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; --index) {
                    if(offset < offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset - this->cellSizeList[index] > offsetBegin; --index) {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax()) {
                    zffloat sizeDelta = 0;
                    for( ; index != zfindexMax() && offset >= offsetEnd; --index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew, 0);
                        this->visibleCellRange.start = index;
                        ++(this->visibleCellRange.count);
                        this->child(cellNew, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.x -= sizeDelta;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    if(index == zfindexMax()) {
                        this->visibleCellsOffset = 0;
                    }
                    else {
                        this->visibleCellsOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case v_ZFUIOrientation::e_Top: {
                zffloat offset = cellFrame.y + cellFrame.height;
                zffloat offsetEnd = zfmMax<zffloat>(
                    -scrollContentFrame.y - scrollMargin.top,
                    0);
                zffloat offsetBegin = zfmMin<zffloat>(
                    offsetEnd + viewFrame.height,
                    scrollContentFrame.height);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; --index) {
                    if(offset < offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset - this->cellSizeList[index] > offsetBegin; --index) {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax()) {
                    zffloat sizeDelta = 0;
                    for( ; index != zfindexMax() && offset >= offsetEnd; --index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew, 0);
                        this->visibleCellRange.start = index;
                        ++(this->visibleCellRange.count);
                        this->child(cellNew, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.y -= sizeDelta;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    if(index == zfindexMax()) {
                        this->visibleCellsOffset = 0;
                    }
                    else {
                        this->visibleCellsOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case v_ZFUIOrientation::e_Right: {
                zffloat offset = cellFrame.x;
                zffloat offsetEnd = zfmMin<zffloat>(
                    -scrollContentFrame.x + viewFrame.width - scrollMargin.left,
                    scrollContentFrame.width);
                zffloat offsetBegin = zfmMax<zffloat>(
                    offsetEnd - viewFrame.width,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; --index) {
                    if(offset > offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset + this->cellSizeList[index] < offsetBegin; --index) {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax()) {
                    zffloat sizeDelta = 0;
                    for( ; index != zfindexMax() && offset <= offsetEnd; --index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew, 0);
                        this->visibleCellRange.start = index;
                        ++(this->visibleCellRange.count);
                        this->child(cellNew, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    this->visibleCellsOffset = offset;
                }
            }
                break;
            case v_ZFUIOrientation::e_Bottom: {
                zffloat offset = cellFrame.y;
                zffloat offsetEnd = zfmMin<zffloat>(
                    -scrollContentFrame.y + viewFrame.height - scrollMargin.top,
                    scrollContentFrame.height);
                zffloat offsetBegin = zfmMax<zffloat>(
                    offsetEnd - viewFrame.height,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; --index) {
                    if(offset > offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset + this->cellSizeList[index] < offsetBegin; --index) {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax()) {
                    zffloat sizeDelta = 0;
                    for( ; index != zfindexMax() && offset <= offsetEnd; --index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew, 0);
                        this->visibleCellRange.start = index;
                        ++(this->visibleCellRange.count);
                        this->child(cellNew, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    this->visibleCellsOffset = offset;
                }
            }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }

        this->visibleCellsOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateTailCellAfterIndex(
            ZF_IN zfindex index
            , ZF_IN const ZFUIRect &cellFrame
            ) {
        this->scrollContentFrameOverrideFlag = zftrue;

        ZFUIMargin scrollMargin = ZFUIMarginInc(
            this->pimplOwner->nativeImplViewMargin(),
            this->pimplOwner->scrollAreaMargin());
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        const ZFUIRect &viewFrame = this->pimplOwner->viewFrame();

        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left: {
                zffloat offset = cellFrame.x;
                zffloat offsetEnd = zfmMin<zffloat>(
                    -scrollContentFrame.x + viewFrame.width - scrollMargin.left,
                    scrollContentFrame.width);
                zffloat offsetBegin = zfmMax<zffloat>(
                    offsetEnd - viewFrame.width,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; ++index) {
                    if(offset > offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index) {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount) {
                    zffloat sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew);
                        ++(this->visibleCellRange.count);
                        this->child(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->visibleCellRange.count == 1) {
                            this->visibleCellRange.start = index;
                            if(index == 0) {
                                this->visibleCellsOffset = 0;
                            }
                            else {
                                this->visibleCellsOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case v_ZFUIOrientation::e_Top: {
                zffloat offset = cellFrame.y;
                zffloat offsetEnd = zfmMin<zffloat>(
                    -scrollContentFrame.y + viewFrame.height - scrollMargin.top,
                    scrollContentFrame.height);
                zffloat offsetBegin = zfmMax<zffloat>(
                    offsetEnd - viewFrame.height,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; ++index) {
                    if(offset > offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index) {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount) {
                    zffloat sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew);
                        ++(this->visibleCellRange.count);
                        this->child(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->visibleCellRange.count == 1) {
                            this->visibleCellRange.start = index;
                            if(index == 0) {
                                this->visibleCellsOffset = 0;
                            }
                            else {
                                this->visibleCellsOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case v_ZFUIOrientation::e_Right: {
                zffloat offset = cellFrame.x + cellFrame.width;
                zffloat offsetEnd = zfmMax<zffloat>(
                    -scrollContentFrame.x - scrollMargin.left,
                    0);
                zffloat offsetBegin = zfmMin<zffloat>(
                    offsetEnd + viewFrame.width,
                    scrollContentFrame.width);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; ++index) {
                    if(offset < offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index) {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount) {
                    zffloat sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew);
                        ++(this->visibleCellRange.count);
                        this->child(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->visibleCellRange.count == 1) {
                            this->visibleCellsOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        this->visibleCellsOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.x -= sizeDelta;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case v_ZFUIOrientation::e_Bottom: {
                zffloat offset = cellFrame.y + cellFrame.height;
                zffloat offsetEnd = zfmMax<zffloat>(
                    -scrollContentFrame.y - scrollMargin.top,
                    0);
                zffloat offsetBegin = zfmMin<zffloat>(
                    offsetEnd + viewFrame.height,
                    scrollContentFrame.height);
                // remove cells exceeds visible range
                for( ; index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count; ++index) {
                    if(offset < offsetEnd) {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index) {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount) {
                    zffloat sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index) {
                        if(index >= this->visibleCellRange.start && index < this->visibleCellRange.start + this->visibleCellRange.count) {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUICell *cellNew = zfRetain(this->cellLoadAt(index));
                        zffloat cellSizeNew = this->cellSizeAt(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index]) {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->visibleCells.add(cellNew);
                        ++(this->visibleCellRange.count);
                        this->child(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->visibleCellRange.count == 1) {
                            this->visibleCellsOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0) {
                        this->visibleCellsOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.y -= sizeDelta;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }

        this->visibleCellsOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateFromFirstCell(void) {
        if(this->cellCount == 0) {
            return;
        }

        zffloat cellSize = this->cellSizeList[0];
        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left:
                this->updateTailCellAfterIndex(0, ZFUIRectCreate(
                    0,
                    0,
                    cellSize,
                    this->pimplOwner->scrollArea().height));
                break;
            case v_ZFUIOrientation::e_Top:
                this->updateTailCellAfterIndex(0, ZFUIRectCreate(
                    0,
                    0,
                    this->pimplOwner->scrollArea().width,
                    cellSize));
                break;
            case v_ZFUIOrientation::e_Right:
                this->updateTailCellAfterIndex(0, ZFUIRectCreate(
                    this->pimplOwner->scrollContentFrame().width - cellSize,
                    0,
                    cellSize,
                    this->pimplOwner->scrollArea().height));
                break;
            case v_ZFUIOrientation::e_Bottom:
                this->updateTailCellAfterIndex(0, ZFUIRectCreate(
                    0,
                    this->pimplOwner->scrollContentFrame().height - cellSize,
                    this->pimplOwner->scrollArea().width,
                    cellSize));
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }
    ZFUIRect visibleCellsFrame(ZF_IN zfindex cellIndex) {
        ZFUIRect ret = ZFUIRectZero();
        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left:
                ret.x = this->visibleCellsOffset;
                ret.height = this->pimplOwner->scrollArea().height;
                for(zfindex i = this->visibleCellRange.start; ; ++i) {
                    if(i == cellIndex) {
                        ret.width = this->cellSizeList[i];
                        break;
                    }
                    ret.x += this->cellSizeList[i];
                }
                break;
            case v_ZFUIOrientation::e_Top:
                ret.y = this->visibleCellsOffset;
                ret.width = this->pimplOwner->scrollArea().width;
                for(zfindex i = this->visibleCellRange.start; ; ++i) {
                    if(i == cellIndex) {
                        ret.height = this->cellSizeList[i];
                        break;
                    }
                    ret.y += this->cellSizeList[i];
                }
                break;
            case v_ZFUIOrientation::e_Right:
                ret.x = this->visibleCellsOffset;
                ret.height = this->pimplOwner->scrollArea().height;
                for(zfindex i = this->visibleCellRange.start; ; ++i) {
                    ret.x -= this->cellSizeList[i];
                    if(i == cellIndex) {
                        ret.width = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            case v_ZFUIOrientation::e_Bottom:
                ret.y = this->visibleCellsOffset;
                ret.width = this->pimplOwner->scrollArea().width;
                for(zfindex i = this->visibleCellRange.start; ; ++i) {
                    ret.y -= this->cellSizeList[i];
                    if(i == cellIndex) {
                        ret.height = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return ZFUIRectZero();
        }
        return ret;
    }
    void listCheckReload(void) {
        if(!this->reloadRequested) {
            if(this->listQuickReloadRequested) {
                this->listQuickReloadRequested = zffalse;
                ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
                switch(this->pimplOwner->orientation()) {
                    case v_ZFUIOrientation::e_Left:
                    case v_ZFUIOrientation::e_Right:
                        scrollContentFrame.height = this->pimplOwner->scrollArea().height;
                        break;
                    case v_ZFUIOrientation::e_Top:
                    case v_ZFUIOrientation::e_Bottom:
                        scrollContentFrame.width = this->pimplOwner->scrollArea().width;
                        break;
                    default:
                        ZFCoreCriticalShouldNotGoHere();
                        return;
                }
                if(scrollContentFrame != this->pimplOwner->scrollContentFrame()) {
                    this->scrollContentFrameOverrideFlag = zftrue;
                    this->pimplOwner->scrollContentFrameUpdate(scrollContentFrame);
                    this->scrollContentFrameOverrideFlag = zffalse;
                }
                if(!this->visibleCells.isEmpty()) {
                    zfindex cellIndex = this->visibleCellRange.start;
                    this->updateTailCellAfterIndex(
                        cellIndex,
                        this->visibleCellsFrame(cellIndex));
                }
                else {
                    this->updateFromFirstCell();
                }
            }
            return;
        }

        if(this->cellAdapter == zfnull) {
            this->removeAll();
            this->scrollContentFrameOverrideFlag = zftrue;
            this->pimplOwner->scrollContentFrame(ZFUIRectZero());
            this->scrollContentFrameOverrideFlag = zffalse;
            this->reloadRequested = zffalse;
            this->listQuickReloadRequested = zffalse;
            this->pimplOwner->visibleCellsOnUpdate();
            return;
        }

        this->cellNeedUpdate = zftrue;

        this->cellCount = this->cellAdapter->cellCount();
        if(this->cellSizeList.count() > this->cellCount) {
            this->cellSizeList.remove(this->cellCount, zfindexMax());
        }
        else {
            zffloat cellSizeHint = this->cellAdapter->cellSizeHint();
            if(this->cellAdapter->cellSizeFill()) {
                switch(this->cellAdapter->orientation()) {
                    case v_ZFUIOrientation::e_Left:
                    case v_ZFUIOrientation::e_Right:
                        cellSizeHint = this->cellAdapter->containerSize().width;
                        break;
                    case v_ZFUIOrientation::e_Top:
                    case v_ZFUIOrientation::e_Bottom:
                        cellSizeHint = this->cellAdapter->containerSize().height;
                        break;
                    default:
                        ZFCoreCriticalShouldNotGoHere();
                        break;
                }
            }
            else if(cellSizeHint < 0) {
                cellSizeHint = ZFUIGlobalStyle::DefaultStyle()->itemSizeCell();
            }
            this->cellSizeList.capacity(this->cellCount);
            for(zfindex i = this->cellSizeList.count(); i < this->cellCount; ++i) {
                this->cellSizeList.add(cellSizeHint);
            }
        }
        zffloat totalSize = 0;
        for(zfindex i = 0, iEnd = this->cellSizeList.count(); i < iEnd; ++i) {
            totalSize += this->cellSizeList[i];
        }
        this->scrollContentFrameOverrideFlag = zftrue;
        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left:
            case v_ZFUIOrientation::e_Right: {
                ZFUIRect scrollContentFrameNew = ZFUIRectCreate(
                    this->pimplOwner->scrollContentFrame().x,
                    0,
                    zfmMax(totalSize, this->pimplOwner->scrollArea().width),
                    this->pimplOwner->scrollArea().height);
                if(this->reloadByUpdateListOrientation) {
                    if(this->pimplOwner->orientation() == v_ZFUIOrientation::e_Left) {
                        scrollContentFrameNew.x = 0;
                    }
                    else {
                        scrollContentFrameNew.x = this->pimplOwner->scrollArea().width - scrollContentFrameNew.width;
                    }
                }
                this->pimplOwner->scrollContentFrameUpdate(scrollContentFrameNew);
            }
                break;
            case v_ZFUIOrientation::e_Top:
            case v_ZFUIOrientation::e_Bottom: {
                ZFUIRect scrollContentFrameNew = ZFUIRectCreate(
                    0,
                    this->pimplOwner->scrollContentFrame().y,
                    this->pimplOwner->scrollArea().width,
                    zfmMax(totalSize, this->pimplOwner->scrollArea().height));
                if(this->reloadByUpdateListOrientation) {
                    if(this->pimplOwner->orientation() == v_ZFUIOrientation::e_Top) {
                        scrollContentFrameNew.y = 0;
                    }
                    else {
                        scrollContentFrameNew.y = this->pimplOwner->scrollArea().height - scrollContentFrameNew.height;
                    }
                }
                this->pimplOwner->scrollContentFrameUpdate(scrollContentFrameNew);
            }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
        this->scrollContentFrameOverrideFlag = zffalse;

        zfindex cellIndex = zfindexMax();
        if(!this->visibleCells.isEmpty()) {
            cellIndex = this->visibleCellRange.start;
        }

        this->visibleCellRange = ZFIndexRangeZero();
        for(zfindex i = this->visibleCells.count() - 1; i != zfindexMax(); --i) {
            ZFUICell *cell = this->visibleCells[i];
            this->childRemoveAt(i);
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->visibleCells.removeAll();

        if(cellIndex != zfindexMax() && !this->visibleCellsOffsetNeedUpdate) {
            ZFUIRect cellFrame = this->visibleCellsFrame(cellIndex);
            this->updateHeadCellBeforeIndex(cellIndex, cellFrame);
            if(!this->visibleCells.isEmpty()) {
                cellIndex = this->visibleCellRange.start;
                cellFrame = this->visibleCellsFrame(cellIndex);
                this->updateTailCellAfterIndex(cellIndex, cellFrame);
            }
            else {
                this->updateFromFirstCell();
            }
        }
        else {
            this->updateFromFirstCell();
        }

        this->reloadRequested = zffalse;
        this->listQuickReloadRequested = zffalse;
        this->reloadByUpdateListOrientation = zffalse;
        this->pimplOwner->visibleCellsOnUpdate();

        // fix content range
        ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
        if(scrollContentFrame.x > 0) {
            scrollContentFrame.x = 0;
        }
        if(scrollContentFrame.y > 0) {
            scrollContentFrame.y = 0;
        }
        if(scrollContentFrame.width > this->pimplOwner->scrollArea().width
                && scrollContentFrame.x + scrollContentFrame.width < this->pimplOwner->scrollArea().width
                ) {
            scrollContentFrame.x = this->pimplOwner->scrollArea().width - scrollContentFrame.width;
        }
        if(scrollContentFrame.height > this->pimplOwner->scrollArea().height
                && scrollContentFrame.y + scrollContentFrame.height < this->pimplOwner->scrollArea().height
                ) {
            scrollContentFrame.y = this->pimplOwner->scrollArea().height - scrollContentFrame.height;
        }
        this->pimplOwner->scrollContentFrameUpdate(scrollContentFrame);

        this->pimplOwner->scrollToFitRange();
    }
    void updateCellLayout(void) {
        if(!this->cellNeedUpdate) {
            return;
        }
        this->cellNeedUpdate = zffalse;

        zffloat offset = 0;
        zffloat fillSize = 0;
        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left:
                offset = this->visibleCellsOffset;
                fillSize = this->pimplOwner->scrollArea().height;
                for(zfindex i = 0; i < this->visibleCells.count(); ++i) {
                    zffloat cellWidth = this->cellSizeList[this->visibleCellRange.start + i];
                    this->visibleCells[i]->viewFrame(ZFUIRectCreate(
                            offset,
                            0,
                            cellWidth,
                            fillSize
                        ));
                    offset += cellWidth;
                }
                break;
            case v_ZFUIOrientation::e_Top:
                offset = this->visibleCellsOffset;
                fillSize = this->pimplOwner->scrollArea().width;
                for(zfindex i = 0; i < this->visibleCells.count(); ++i) {
                    zffloat cellHeight = this->cellSizeList[this->visibleCellRange.start + i];
                    this->visibleCells[i]->viewFrame(ZFUIRectCreate(
                            0,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                    offset += cellHeight;
                }
                break;
            case v_ZFUIOrientation::e_Right:
                offset = this->visibleCellsOffset;
                fillSize = this->pimplOwner->scrollArea().height;
                for(zfindex i = 0; i < this->visibleCells.count(); ++i) {
                    zffloat cellWidth = this->cellSizeList[this->visibleCellRange.start + i];
                    offset -= cellWidth;
                    this->visibleCells[i]->viewFrame(ZFUIRectCreate(
                            offset,
                            0,
                            cellWidth,
                            fillSize
                        ));
                }
                break;
            case v_ZFUIOrientation::e_Bottom:
                offset = this->visibleCellsOffset;
                fillSize = this->pimplOwner->scrollArea().width;
                for(zfindex i = 0; i < this->visibleCells.count(); ++i) {
                    zffloat cellHeight = this->cellSizeList[this->visibleCellRange.start + i];
                    offset -= cellHeight;
                    this->visibleCells[i]->viewFrame(ZFUIRectCreate(
                            0,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }

public:
    void scrollCellCheckUpdate(void) {
        if(this->scrollCellIndex == zfindexMax()) {
            return;
        }

        zffloat desiredPos = this->scrollCellDesiredPosCalc();
        if(!this->scrollCellAnimated) {
            while(this->scrollCellIndex != zfindexMax() && desiredPos != this->scrollCellDesiredPosSaved) {
                this->scrollCellScrollToPos(desiredPos, zffalse);
            }
            this->scrollCellIndex = zfindexMax();
            return;
        }
        if(desiredPos != this->scrollCellDesiredPosSaved) {
            this->scrollCellScrollToPos(desiredPos, zftrue);
        }
        else {
            this->scrollCellIndex = zfindexMax();
        }
    }
private:
    zffloat scrollCellDesiredPosCalc(void) {
        zffloat offset = 0;
        for(zfindex i = 0, iEnd = this->cellSizeList.count(); i < iEnd; ++i) {
            offset += this->cellSizeList[i];
        }
        if(this->scrollCellToHead) {
            return this->scrollCellOffset - offset;
        }
        else {
            if(ZFUIOrientationIsHorizontal(this->pimplOwner->orientation())) {
                return this->pimplOwner->scrollArea().width - (offset + this->cellSizeList[this->scrollCellIndex] + this->scrollCellOffset);
            }
            else {
                return this->pimplOwner->scrollArea().height - (offset + this->cellSizeList[this->scrollCellIndex] + this->scrollCellOffset);
            }
        }
    }
    void scrollCellScrollToPos(
            ZF_IN zffloat pos
            , ZF_IN zfbool animated
            ) {
        this->scrollCellDesiredPosSaved = pos;
        this->pimplOwner->scrollOverride(zftrue);
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        zffloat posX = 0;
        zffloat posY = 0;
        switch(this->pimplOwner->orientation()) {
            case v_ZFUIOrientation::e_Left:
                posX = pos;
                posY = scrollContentFrame.y;
                break;
            case v_ZFUIOrientation::e_Top:
                posX = scrollContentFrame.x;
                posY = pos;
                break;
            case v_ZFUIOrientation::e_Right:
                posX = this->pimplOwner->scrollArea().width - pos - scrollContentFrame.width;
                posY = scrollContentFrame.y;
                break;
            case v_ZFUIOrientation::e_Bottom:
                posX = scrollContentFrame.x;
                posY = this->pimplOwner->scrollArea().height - pos - scrollContentFrame.height;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
        if(animated) {
            this->pimplOwner->scrollByPoint(posX, posY);
        }
        else {
            this->pimplOwner->scrollContentFrame(ZFUIRectCreate(posX, posY, scrollContentFrame.width, scrollContentFrame.height));
        }
        this->pimplOwner->scrollOverride(zffalse);
    }
};

// ============================================================
// ZFUIListView
ZFOBJECT_REGISTER(ZFUIListView)

ZFEVENT_REGISTER(ZFUIListView, CellOnAttach)
ZFEVENT_REGISTER(ZFUIListView, CellOnDetach)
ZFEVENT_REGISTER(ZFUIListView, ListVisibleCellOnUpdate)

#define _ZFP_ZFUIListView_cellAdapterHolderTag "_ZFP_ZFUIListView_cellAdapterHolderTag"
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListView, zfanyT<ZFUICellAdapter>, cellAdapter) {
    d->cellAdapter = this->cellAdapter();
    d->cellAdapterAfterAttach();

    if(propertyValue != propertyValueOld) {
        this->reload();
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIListView, zfanyT<ZFUICellAdapter>, cellAdapter) {
    d->cellAdapterBeforeDetach();
    d->cellAdapter = zfnull;
    this->objectTagRemove(_ZFP_ZFUIListView_cellAdapterHolderTag);
}
ZFMETHOD_DEFINE_1(ZFUIListView, void, cellAdapterAutoRetain
        , ZFMP_IN(ZFUICellAdapter *, cellAdapter)
        ) {
    zfRetain(cellAdapter);
    this->cellAdapter(cellAdapter);
    this->toObject()->objectTag(_ZFP_ZFUIListView_cellAdapterHolderTag, this->cellAdapter()->toObject());
    zfRelease(cellAdapter);
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListView, ZFUIOrientation, orientation) {
    if(propertyValue != propertyValueOld) {
        d->bounceableUpdate();
        d->reloadByUpdateListOrientation = zftrue;
        this->reload();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListView, zfbool, bounceable) {
    if(propertyValue != propertyValueOld) {
        d->bounceableUpdate();
        if(!this->bounceable()) {
            this->scrollBounceVerticalAlways(zffalse);
            this->scrollBounceHorizontalAlways(zffalse);
        }
    }
}

// ============================================================
void ZFUIListView::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIListViewPrivate);
    d->pimplOwner = this;
    d->bounceableUpdate();

    ZFLISTENER_1(layoutOnLayoutRequest
            , _ZFP_ZFUIListViewPrivate *, d
            ) {
        d->cellNeedUpdate = zftrue;
    } ZFLISTENER_END()
    this->observerAdd(ZFUIView::E_ViewLayoutOnLayoutRequest(), layoutOnLayoutRequest);
}
void ZFUIListView::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFUIListView::objectOnDeallocPrepare(void) {
    d->childOverrideFlag = zftrue;
    this->cellAdapter(zfnull);
    zfsuper::objectOnDeallocPrepare();
    d->childOverrideFlag = zffalse;
}

ZFSerializablePropertyType ZFUIListView::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property) {
    if(property == ZFPropertyAccess(ZFUIScrollView, scrollContentFrame)
            || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical)
            || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal)
            || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways)
            || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways)
            ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    else {
        return zfsuperI(ZFSerializable)::serializableOnCheckPropertyType(property);
    }
}

zfbool ZFUIListView::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->cellAdapter(zfnull);

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        zfstring category = ZFSerializableUtil::checkCategory(categoryData);
        if(!zfstringIsEqual(category, ZFSerializableKeyword_ZFUIListView_cellAdapter)) {continue;}

        zfauto element;
        if(!ZFObjectFromDataT(element, categoryData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(element != zfnull
                && !element->classData()->classIsTypeOf(ZFUICellAdapter::ClassData())
                ) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                element->objectInfoOfInstance(), ZFUICellAdapter::ClassData()->classNameFull());
            return zffalse;
        }

        this->cellAdapter(element);
        categoryData.resolveMark();
    }
    return zftrue;
}
zfbool ZFUIListView::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    if(!this->cellAdapterSerializable() || this->cellAdapter() == zfnull) {
        return zftrue;
    }

    ZFSerializableData categoryData;
    if(!ZFObjectToDataT(categoryData, this->cellAdapter()->toObject(), outErrorHint,
                ref ? zfcast(ZFSerializable *, ref->cellAdapter()) : zfnull
                )) {
        return zffalse;
    }

    if(!(
                ref != zfnull && ref->cellAdapter() != zfnull
                && this->cellAdapter()->classData()->classIsTypeOf(ref->cellAdapter()->classData())
                && categoryData.childCount() == 0 && categoryData.attrCount() == 0
                )) {
        categoryData.category(ZFSerializableKeyword_ZFUIListView_cellAdapter);
        serializableData.child(categoryData);
    }

    return zftrue;
}

void ZFUIListView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
    if(!d->listQuickReloadRequested && (
        this->viewFrame().width != this->viewFramePrev().width
        || this->viewFrame().height != this->viewFramePrev().height
    )) {
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
    }
    if(d->reloadRequested || d->listQuickReloadRequested) {
        d->cellAdapterNotifyUpdate();
    }
    zfsuper::layoutOnLayoutPrepare(bounds);
    d->listCheckReload();
}
void ZFUIListView::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    d->updateCellLayout();
}

void ZFUIListView::viewChildOnAdd(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayer layer
        ) {
    if(layer == v_ZFUIViewChildLayer::e_Normal) {
        ZFCoreAssertWithMessage(d->childOverrideFlag || layer != v_ZFUIViewChildLayer::e_Normal, "you must not add child to a list view");
    }
    zfsuper::viewChildOnAdd(child, layer);
}
void ZFUIListView::viewChildOnRemove(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayer layer
        ) {
    if(layer == v_ZFUIViewChildLayer::e_Normal) {
        ZFCoreAssertWithMessage(d->childOverrideFlag || layer != v_ZFUIViewChildLayer::e_Normal, "you must not remove child from a list view");
    }
    zfsuper::viewChildOnRemove(child, layer);
}

void ZFUIListView::scrollAreaOnUpdate(void) {
    zfsuper::scrollAreaOnUpdate();
    if(!d->listQuickReloadRequested) {
        d->listQuickReloadRequested = zftrue;
    }
    d->cellAdapterNotifyUpdate();
}
void ZFUIListView::scrollContentFrameOnUpdate(void) {
    zfsuper::scrollContentFrameOnUpdate();
    if(d->scrollContentFrameOverrideFlag) {
        return;
    }

    d->listCheckReload();

    if(d->cellAdapter == zfnull) {
        return;
    }

    if(!d->visibleCells.isEmpty() && !d->visibleCellsOffsetNeedUpdate) {
        zfindex cellIndex = d->visibleCellRange.start + d->visibleCellRange.count - 1;
        ZFUIRect cellFrame = d->visibleCellsFrame(cellIndex);
        d->updateHeadCellBeforeIndex(cellIndex, cellFrame);
        if(!d->visibleCells.isEmpty()) {
            cellIndex = d->visibleCellRange.start;
            cellFrame = d->visibleCellsFrame(cellIndex);
            d->updateTailCellAfterIndex(cellIndex, cellFrame);
        }
        else {
            d->updateFromFirstCell();
        }
    }
    else {
        d->updateFromFirstCell();
    }

    if(d->cellNeedUpdate) {
        this->visibleCellsOnUpdate();
    }
}
void ZFUIListView::scrollOnScrolledByUser(void) {
    zfsuper::scrollOnScrolledByUser();

    // cancel scrollCellToHead/Tail task
    d->scrollCellIndex = zfindexMax();
}
void ZFUIListView::scrollOnScrollEnd(void) {
    zfsuper::scrollOnScrollEnd();

    // cancel scrollCellToHead/Tail task
    d->scrollCellIndex = zfindexMax();
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIListView, void, reload) {
    if(!d->reloadRequested) {
        d->reloadRequested = zftrue;
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
        d->visibleCellsOffsetNeedUpdate = zftrue;
        this->layoutRequest();
    }
}
ZFMETHOD_DEFINE_0(ZFUIListView, zfbool, reloadRequested) {
    return d->reloadRequested;
}
ZFMETHOD_DEFINE_1(ZFUIListView, void, reloadCellAt
        , ZFMP_IN(zfindex, index)
        ) {
    if(d->reloadRequested || !ZFIndexRangeContain(d->visibleCellRange, index)) {
        return;
    }

    d->cellNeedUpdate = zftrue;

    zfindex indexOfVisibleCell = index - d->visibleCellRange.start;
    ZFUICell *cellOld = d->visibleCells[indexOfVisibleCell];
    ZFUIRect cellOldFrame = d->visibleCellsFrame(index);

    ZFUICell *cellNew = zfRetain(d->cellLoadAt(index));
    d->visibleCells[indexOfVisibleCell] = cellNew;

    this->childReplaceAt(indexOfVisibleCell, cellNew);

    d->cellOnRecycle(cellOld);
    this->cellOnDetach(cellOld);
    this->cellOnAttach(cellNew);
    d->cellOnUpdate(index, cellNew);

    // update cell size at index
    zffloat cellNewSize = d->cellSizeAt(index, cellNew);
    d->cellSizeList[index] = cellNewSize;

    // update all cells after the reloaded one
    switch(this->orientation()) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            cellOldFrame.width = cellNewSize;
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            cellOldFrame.height = cellNewSize;
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
    d->updateTailCellAfterIndex(index, cellOldFrame);

    // finally notify visible cell changed
    this->visibleCellsOnUpdate();

    zfRelease(cellOld);
}

ZFMETHOD_DEFINE_0(ZFUIListView, ZFCoreArray<ZFUICell *>, visibleCells) {
    return d->visibleCells;
}
ZFMETHOD_DEFINE_0(ZFUIListView, const ZFIndexRange &, visibleCellRange) {
    return d->visibleCellRange;
}

ZFMETHOD_DEFINE_3(ZFUIListView, void, scrollCellToHead
        , ZFMP_IN(zfindex, cellIndex)
        , ZFMP_IN_OPT(zffloat, offset, 0)
        , ZFMP_IN_OPT(zfbool, animated, zftrue)
        ) {
    if(cellIndex >= d->cellCount) {
        d->scrollCellIndex = zfindexMax();
    }
    else {
        d->scrollCellIndex = cellIndex;
    }
    d->scrollCellOffset = offset;
    d->scrollCellAnimated = animated;
    d->scrollCellToHead = zftrue;
    d->scrollCellDesiredPosSaved = 30000;
    d->scrollCellCheckUpdate();
}
ZFMETHOD_DEFINE_3(ZFUIListView, void, scrollCellToTail
        , ZFMP_IN(zfindex, cellIndex)
        , ZFMP_IN_OPT(zffloat, offset, 0)
        , ZFMP_IN_OPT(zfbool, animated, zftrue)
        ) {
    if(cellIndex >= d->cellCount) {
        d->scrollCellIndex = zfindexMax();
    }
    else {
        d->scrollCellIndex = cellIndex;
    }
    d->scrollCellOffset = offset;
    d->scrollCellAnimated = animated;
    d->scrollCellToHead = zffalse;
    d->scrollCellDesiredPosSaved = 30000;
    d->scrollCellCheckUpdate();
}

void ZFUIListView::visibleCellsOnUpdate(void) {
    this->observerNotify(ZFUIListView::E_ListVisibleCellOnUpdate());

    if(!this->scrollOverride()) {
        d->scrollCellCheckUpdate();
    }
}

ZF_NAMESPACE_GLOBAL_END

