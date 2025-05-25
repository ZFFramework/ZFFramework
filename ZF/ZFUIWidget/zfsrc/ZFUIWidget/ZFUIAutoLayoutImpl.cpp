#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef enum {
    _ZFP_ZFUIAutoLayoutRuleType_xAxis,
    _ZFP_ZFUIAutoLayoutRuleType_yAxis,
    _ZFP_ZFUIAutoLayoutRuleType_width,
    _ZFP_ZFUIAutoLayoutRuleType_height,
} _ZFP_ZFUIAutoLayoutRuleType;
zfclassPOD _ZFP_ZFUIAutoLayoutRuleStackData {
public:
    zfuint level;
    _ZFP_ZFUIAutoLayoutRuleType ruleType;
    zfindex childIndex;
};
ZFCORE_POD_DECLARE(_ZFP_ZFUIAutoLayoutRuleStackData)

zfclassNotPOD _ZFP_ZFUIAutoLayoutPrivate {
public:
    zfuint _childCount;
    zfuint _childStateCount;
    /*
     * ZFUIRect *_childFrame;
     * zfbool *_layoutingX;
     * zfbool *_layoutingY;
     * zfbool *_layoutingWidth;
     * zfbool *_layoutingHeight;
     * zfbool *_layoutFinishX;
     * zfbool *_layoutFinishY;
     * zfbool *_layoutFinishWidth;
     * zfbool *_layoutFinishHeight;
     */
    zfbyte *_childState;
public:
    _ZFP_ZFUIAutoLayoutPrivate(void)
    : _childCount(0)
    , _childStateCount(0)
    , _childState(zfnull)
    {
    }
    ~_ZFP_ZFUIAutoLayoutPrivate(void) {
        zffree(_childState);
    }
public:
    ZFUIRect &_childFrame(ZF_IN zfindex i) {
        return *(((ZFUIRect *)(_childState + (0
                            ) * _childCount)) + i);
    }
    zfbool &_layoutingX(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            ) * _childCount)) + i);
    }
    zfbool &_layoutingY(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            ) * _childCount)) + i);
    }
    zfbool &_layoutingWidth(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            ) * _childCount)) + i);
    }
    zfbool &_layoutingHeight(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            + sizeof(zfbool) // _layoutingWidth
                            ) * _childCount)) + i);
    }
    zfbool &_layoutFinishX(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            + sizeof(zfbool) // _layoutingWidth
                            + sizeof(zfbool) // _layoutingHeight
                            ) * _childCount)) + i);
    }
    zfbool &_layoutFinishY(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            + sizeof(zfbool) // _layoutingWidth
                            + sizeof(zfbool) // _layoutingHeight
                            + sizeof(zfbool) // _layoutFinishX
                            ) * _childCount)) + i);
    }
    zfbool &_layoutFinishWidth(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            + sizeof(zfbool) // _layoutingWidth
                            + sizeof(zfbool) // _layoutingHeight
                            + sizeof(zfbool) // _layoutFinishX
                            + sizeof(zfbool) // _layoutFinishY
                            ) * _childCount)) + i);
    }
    zfbool &_layoutFinishHeight(ZF_IN zfindex i) {
        return *(((zfbool *)(_childState + (0
                            + sizeof(ZFUIRect) // _childFrame
                            + sizeof(zfbool) // _layoutingX
                            + sizeof(zfbool) // _layoutingY
                            + sizeof(zfbool) // _layoutingWidth
                            + sizeof(zfbool) // _layoutingHeight
                            + sizeof(zfbool) // _layoutFinishX
                            + sizeof(zfbool) // _layoutFinishY
                            + sizeof(zfbool) // _layoutFinishWidth
                            ) * _childCount)) + i);
    }

public:
    void recursiveRuleAssert(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfbool layouting
            , ZF_IN zfuint level
            , ZF_IN _ZFP_ZFUIAutoLayoutRuleType ruleType
            , ZF_IN zfindex childIndex
            );

public:
    void prepareLayout(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN const ZFUISize &sizeHint
            ) {
        _childCount = (zfuint)parent->childCount();
        zfindex size = (0
                + sizeof(ZFUIRect) // _childFrame
                + sizeof(zfbool) // _layoutingX
                + sizeof(zfbool) // _layoutingY
                + sizeof(zfbool) // _layoutingWidth
                + sizeof(zfbool) // _layoutingHeight
                + sizeof(zfbool) // _layoutFinishX
                + sizeof(zfbool) // _layoutFinishY
                + sizeof(zfbool) // _layoutFinishWidth
                + sizeof(zfbool) // _layoutFinishHeight
                ) * _childCount;
        if(_childStateCount < _childCount) {
            _childStateCount = _childCount;
            _childState = (zfbyte *)zfrealloc(_childState, size);
        }
        zfmemset(_childState, 0, size);

        for(zfindex i = (zfindex)_childCount - 1; i != zfindexMax(); --i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
            ZFUISize childSize = child->layoutMeasure(
                ZFUILayoutParam::sizeHintMerge(sizeHint, layoutParam->sizeHint()),
                ZFUISizeParamWrapWrap());
            _childFrame(i).width = childSize.width;
            _childFrame(i).height = childSize.height;
        }
    }
    void layoutChild(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfuint level
            , ZF_IN zffloat parentWidth
            , ZF_IN zffloat parentHeight
            , ZF_IN zfindex childIndex
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
private:
    void updateChildSize(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfuint level
            , ZF_IN zffloat parentWidth
            , ZF_IN zffloat parentHeight
            , ZF_IN zfindex childIndex
            , ZF_IN ZFUIAutoLayoutRule &rule
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
    void updateChain(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<zfindex> &chain
            , ZF_IN zfindex childIndex
            , ZF_IN ZFUIAutoLayoutRule *ruleList
            , ZF_IN ZFUIAutoLayoutPos posHead
            , ZF_IN ZFUIAutoLayoutPos posTail
            );
    void calcChildHead(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfuint level
            , ZF_IN zffloat parentWidth
            , ZF_IN zffloat parentHeight
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPos posHead
            , ZF_OUT zffloat &head
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
    void calcChildTail(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfuint level
            , ZF_IN zffloat parentWidth
            , ZF_IN zffloat parentHeight
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPos posTail
            , ZF_OUT zffloat &tail
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
    void layoutChildByChain(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
            , ZF_IN zfuint level
            , ZF_IN zffloat parentWidth
            , ZF_IN zffloat parentHeight
            , ZF_IN const ZFCoreArray<zfindex> &chain
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
    void layoutChildByRange(
            ZF_IN ZFUIAutoLayoutRule &ruleHead
            , ZF_IN ZFUIAutoLayoutRule &ruleTail
            , ZF_IN ZFUIAutoLayoutParam *layoutParam
            , ZF_IN zfindex childIndex
            , ZF_IN zffloat headPos
            , ZF_IN zffloat rangeSize
            , ZF_IN zfbool measureMode
            , ZF_IN zfbool xAxis
            );
};

// ============================================================
void ZFUIAutoLayout::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAutoLayoutPrivate);
}
void ZFUIAutoLayout::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIAutoLayout::viewChildOnRemove(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayer layer
        ) {
    if(layer == v_ZFUIViewChildLayer::e_Normal) {
        if(d->_childStateCount > this->childCount() * 4) {
            d->_childStateCount = 0;
        }
    }
    zfsuper::viewChildOnRemove(child, layer);
}

// ============================================================
void ZFUIAutoLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    if(this->childCount() == 0) {
        return;
    }

    d->prepareLayout(this, sizeHint);
    ZFUILayoutParam::sizeHintApplyT(ret, ZFUISizeZero(), sizeHint, sizeParam);
    ZFUISizeApplyRangeT(ret.width, ret.width, this->viewSizeMin().width, this->viewSizeMax().height);
    ZFUISizeApplyRangeT(ret.height, ret.height, this->viewSizeMin().height, this->viewSizeMax().height);

    // child rule may depends on parent size
    for(zfindex loop = 0; loop < 2; ++loop) {
        ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> ruleStack;
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
            d->layoutChild(this, ruleStack, 0, ret.width, ret.height, i, zftrue, zftrue);
            d->layoutChild(this, ruleStack, 0, ret.width, ret.height, i, zftrue, zffalse);
        }
        zffloat parentLeft = 0;
        zffloat parentTop = 0;
        zffloat parentRight = 0;
        zffloat parentBottom = 0;
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
            ZFUIRect const &childFrame = d->_childFrame(i);
            ZFUIMargin const &margin = this->childAt(i)->layoutParam()->margin();
            parentLeft = zfmMin<zffloat>(parentLeft, ZFUIRectGetLeft(childFrame) - margin.left);
            parentTop = zfmMin<zffloat>(parentTop, ZFUIRectGetTop(childFrame) - margin.top);
            parentRight = zfmMax<zffloat>(parentRight, ZFUIRectGetRight(childFrame) + margin.right);
            parentBottom = zfmMax<zffloat>(parentBottom, ZFUIRectGetBottom(childFrame) + margin.bottom);
        }
        parentRight = parentRight - parentLeft;
        parentBottom = parentBottom - parentTop;
        if(ret.width == parentRight && ret.height == parentBottom) {
            break;
        }
        else {
            ret.width = parentRight;
            ret.height = parentBottom;
            zfmemset(&(d->_layoutFinishX(0)), 0, this->childCount() * 4 * sizeof(zfbool));
        }
    }
}
void ZFUIAutoLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    d->prepareLayout(this, ZFUIRectGetSize(this->viewFrame()));
    ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> ruleStack;
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        d->layoutChild(this, ruleStack, 0, bounds.width, bounds.height, i, zffalse, zftrue);
        d->layoutChild(this, ruleStack, 0, bounds.width, bounds.height, i, zffalse, zffalse);
    }
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        this->childAt(i)->viewFrame(d->_childFrame(i));
    }
}

// ============================================================
void _ZFP_ZFUIAutoLayoutPrivate::layoutChild(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfuint level
        , ZF_IN zffloat parentWidth
        , ZF_IN zffloat parentHeight
        , ZF_IN zfindex childIndex
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    zfbool &_layouting = (xAxis ? _layoutingX(childIndex) : _layoutingY(childIndex));
    zfbool &_layoutFinish = (xAxis ? _layoutFinishX(childIndex) : _layoutFinishY(childIndex));
    this->recursiveRuleAssert(
            parent
            , ruleStack
            , _layouting
            , level
            , xAxis ? _ZFP_ZFUIAutoLayoutRuleType_xAxis : _ZFP_ZFUIAutoLayoutRuleType_yAxis
            , childIndex
            );
    if(_layoutFinish) {
        return;
    }
    _layouting = zftrue;

    ZFCoreArray<zfindex> chain;
    ZFUIView *child = parent->childAt(childIndex);
    ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
    ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_AL_d.ruleList;
    ZFUIAutoLayoutPos posHead = xAxis ? v_ZFUIAutoLayoutPos::e_Left : v_ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPos posTail = xAxis ? v_ZFUIAutoLayoutPos::e_Right : v_ZFUIAutoLayoutPos::e_Bottom;

    this->updateChildSize(parent, ruleStack, level + 1, parentWidth, parentHeight, childIndex, ruleList[xAxis ? v_ZFUIAutoLayoutPos::e_Width : v_ZFUIAutoLayoutPos::e_Height], measureMode, xAxis);

    this->updateChain(parent, chain, childIndex, ruleList, posHead, posTail);
    if(!chain.isEmpty()) {
        this->layoutChildByChain(parent, ruleStack, level + 1, parentWidth, parentHeight, chain, measureMode, xAxis);
    }
    else {
        zffloat head = 0;
        zffloat tail = 0;
        this->calcChildHead(parent, ruleStack, level + 1, parentWidth, parentHeight, child, posHead, head, measureMode, xAxis);
        this->calcChildTail(parent, ruleStack, level + 1, parentWidth, parentHeight, child, posTail, tail, measureMode, xAxis);
        this->layoutChildByRange(ruleList[posHead], ruleList[posTail], layoutParam, childIndex, head, tail - head, measureMode, xAxis);
    }

    _layouting = zffalse;
    _layoutFinish = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChildSize(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfuint level
        , ZF_IN zffloat parentWidth
        , ZF_IN zffloat parentHeight
        , ZF_IN zfindex childIndex
        , ZF_IN ZFUIAutoLayoutRule &rule
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    if(!rule.valid()) {
        return;
    }

    zfbool &_layouting = (xAxis ? _layoutingWidth(childIndex) : _layoutingHeight(childIndex));
    zfbool &_layoutFinish = (xAxis ? _layoutFinishWidth(childIndex) : _layoutFinishHeight(childIndex));
    this->recursiveRuleAssert(
            parent
            , ruleStack
            , _layouting
            , level
            , xAxis ? _ZFP_ZFUIAutoLayoutRuleType_width : _ZFP_ZFUIAutoLayoutRuleType_height
            , childIndex
            );
    if(_layoutFinish) {
        return;
    }
    _layouting = zftrue;

    _ZFP_ZFUIAutoLayout_targetUpdate(rule, parent, parent->childAt(childIndex));
    ZFUIView *target = rule.target();

    zffloat &childSize = (xAxis ? _childFrame(childIndex).width : _childFrame(childIndex).height);
    if(rule.targetPos() == v_ZFUIAutoLayoutPos::e_Width) {
        if(target == parent) {
            childSize = zfmMax(childSize, parentWidth);
        }
        else {
            zfindex targetIndex = parent->childFind(target);
            this->layoutChild(parent, ruleStack, level + 1, parentWidth, parentHeight, targetIndex, measureMode, zftrue);
            childSize = zfmMax(childSize, _childFrame(targetIndex).width);
        }
    }
    else if(rule.targetPos() == v_ZFUIAutoLayoutPos::e_Height) {
        if(target == parent) {
            childSize = zfmMax(childSize, parentHeight);
        }
        else {
            zfindex targetIndex = parent->childFind(target);
            this->layoutChild(parent, ruleStack, level + 1, parentWidth, parentHeight, targetIndex, measureMode, zffalse);
            childSize = zfmMax(childSize, _childFrame(targetIndex).height);
        }
    }

    _layouting = zffalse;
    _layoutFinish = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChain(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<zfindex> &chain
        , ZF_IN zfindex childIndex
        , ZF_IN ZFUIAutoLayoutRule *ruleList
        , ZF_IN ZFUIAutoLayoutPos posHead
        , ZF_IN ZFUIAutoLayoutPos posTail
        ) {
    ZFUIAutoLayoutRule *ruleListCur = ruleList;
    ZFUIAutoLayoutRule *ruleListNext = ruleList;
    ZFUIView *childPrev = parent->childAt(childIndex);
    ZFUIView *childHead = zfnull;
    ZFUIView *childTail = zfnull;
    zfbool centerChildHasAdd = zffalse;

    // head
    do {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListCur[posHead], parent, childPrev);
        ZFUIView *target = ruleListCur[posHead].target();
        if(!ruleListCur[posHead].valid()
                || ruleListCur[posHead].targetPos() != posTail
                ) {
            break;
        }
        else if(target == parent) {
            childHead = target;
            break;
        }
        ruleListNext = target->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListNext[posTail], parent, target);
        if(ruleListNext[posTail].valid()
                && ruleListNext[posTail].target() == childPrev
                ) {
            if(!centerChildHasAdd) {
                centerChildHasAdd = zftrue;
                chain.add(parent->childFind(childPrev), 0);
            }
            childPrev = target;
            ruleListCur = ruleListNext;
            chain.add(parent->childFind(childPrev), 0);
            continue;
        }
        break;
    } while(zftrue);

    // tail
    ruleListCur = ruleList;
    childPrev = parent->childAt(childIndex);
    do {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListCur[posTail], parent, childPrev);
        ZFUIView *target = ruleListCur[posTail].target();
        if(!ruleListCur[posTail].valid()
                || ruleListCur[posTail].targetPos() != posHead
                ) {
            break;
        }
        else if(target == parent) {
            childTail = target;
            break;
        }
        ruleListNext = target->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListNext[posHead], parent, target);
        if(ruleListNext[posHead].valid()
                && ruleListNext[posHead].target() == childPrev
                ) {
            if(!centerChildHasAdd) {
                centerChildHasAdd = zftrue;
                chain.add(parent->childFind(childPrev));
            }
            childPrev = target;
            ruleListCur = ruleListNext;
            chain.add(parent->childFind(childPrev));
            continue;
        }
        break;
    } while(zftrue);

    if(!chain.isEmpty()) {
        if(childHead != zfnull) {
            chain.add(parent->childFind(childHead), 0);
        }
        if(childTail != zfnull) {
            chain.add(parent->childFind(childTail));
        }
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::calcChildHead(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfuint level
        , ZF_IN zffloat parentWidth
        , ZF_IN zffloat parentHeight
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUIAutoLayoutPos posHead
        , ZF_OUT zffloat &head
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    head = 0;
    ZFUIAutoLayoutRule *ruleList = child->layoutParam()->to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
    if(ruleList[posHead].valid()) {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleList[posHead], parent, child);
        ZFUIView *target = ruleList[posHead].target();
        if(target != parent) {
            zfindex targetIndex = parent->childFind(target);
            this->layoutChild(parent, ruleStack, level, parentWidth, parentHeight, targetIndex, measureMode, xAxis);
            if(ruleList[posHead].targetPos() == posHead) {
                head = xAxis ? ZFUIRectGetLeft(_childFrame(targetIndex)) : ZFUIRectGetTop(_childFrame(targetIndex));
            }
            else {
                head = xAxis ? ZFUIRectGetRight(_childFrame(targetIndex)) : ZFUIRectGetBottom(_childFrame(targetIndex));
            }
        }
    }
}
void _ZFP_ZFUIAutoLayoutPrivate::calcChildTail(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfuint level
        , ZF_IN zffloat parentWidth
        , ZF_IN zffloat parentHeight
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUIAutoLayoutPos posTail
        , ZF_OUT zffloat &tail
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    tail = xAxis ? parentWidth : parentHeight;
    ZFUIAutoLayoutRule *ruleList = child->layoutParam()->to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
    if(ruleList[posTail].valid()) {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleList[posTail], parent, child);
        ZFUIView *target = ruleList[posTail].target();
        if(target != parent) {
            zfindex targetIndex = parent->childFind(target);
            this->layoutChild(parent, ruleStack, level, parentWidth, parentHeight, targetIndex, measureMode, xAxis);
            if(ruleList[posTail].targetPos() == posTail) {
                tail = xAxis ? ZFUIRectGetRight(_childFrame(targetIndex)) : ZFUIRectGetBottom(_childFrame(targetIndex));
            }
            else {
                tail = xAxis ? ZFUIRectGetLeft(_childFrame(targetIndex)) : ZFUIRectGetTop(_childFrame(targetIndex));
            }
        }
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByChain(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfuint level
        , ZF_IN zffloat parentWidth
        , ZF_IN zffloat parentHeight
        , ZF_IN const ZFCoreArray<zfindex> &chain
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    ZFUIAutoLayoutPos posHead = xAxis ? v_ZFUIAutoLayoutPos::e_Left : v_ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPos posTail = xAxis ? v_ZFUIAutoLayoutPos::e_Right : v_ZFUIAutoLayoutPos::e_Bottom;
    zffloat head = 0;
    zffloat tail = 0;
    this->calcChildHead(parent, ruleStack, level, parentWidth, parentHeight, parent->childAt(chain.getFirst()), posHead, head, measureMode, xAxis);
    this->calcChildTail(parent, ruleStack, level, parentWidth, parentHeight, parent->childAt(chain.getLast()), posTail, tail, measureMode, xAxis);

    zffloat fixedSize = 0;
    zffloat *childSizeCache = (zffloat *)zfmalloc(sizeof(zffloat) * parent->childCount());
    zfblockedFree(childSizeCache);
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i) {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutRule *ruleList = parent->childAt(childIndex)->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        this->updateChildSize(parent, ruleStack, level, parentWidth, parentHeight, childIndex, ruleList[xAxis ? v_ZFUIAutoLayoutPos::e_Width : v_ZFUIAutoLayoutPos::e_Height], measureMode, xAxis);
        childSizeCache[childIndex] = xAxis
            ? _childFrame(childIndex).width + ZFUIMarginGetWidth(parent->childAt(childIndex)->layoutParam()->margin())
            : _childFrame(childIndex).height + ZFUIMarginGetHeight(parent->childAt(childIndex)->layoutParam()->margin())
            ;
        fixedSize += childSizeCache[childIndex];
    }

    if(measureMode) {
        tail = zfmMax<zffloat>(tail, head + fixedSize);
    }

    zffloat flexibleUnit = 0;
    flexibleUnit = (tail - head - fixedSize) / chain.count();

    zffloat tailTmp = tail;
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i) {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutParam *layoutParam = parent->childAt(childIndex)->layoutParam();
        ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_AL_d.ruleList;
        childSizeCache[childIndex] += flexibleUnit;
        tailTmp -= childSizeCache[childIndex];
        this->layoutChildByRange(ruleList[posHead], ruleList[posTail], layoutParam, childIndex, tailTmp, childSizeCache[childIndex], measureMode, xAxis);
        (xAxis ? _layoutFinishX(childIndex) : _layoutFinishY(childIndex)) = zftrue;
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByRange(
        ZF_IN ZFUIAutoLayoutRule &ruleHead
        , ZF_IN ZFUIAutoLayoutRule &ruleTail
        , ZF_IN ZFUIAutoLayoutParam *layoutParam
        , ZF_IN zfindex childIndex
        , ZF_IN zffloat headPos
        , ZF_IN zffloat rangeSize
        , ZF_IN zfbool measureMode
        , ZF_IN zfbool xAxis
        ) {
    typedef enum {
        _RuleTypeCenter,
        _RuleTypeHead,
        _RuleTypeTail,
    } _RuleType;
    _RuleType t = _RuleTypeCenter;
    if(!ruleHead.valid()) {
        if(!ruleTail.valid()) { // <none, none>
            t = _RuleTypeCenter;
        }
        else { // <none, fixed>
            t = _RuleTypeTail;
        }
    }
    else {
        if(!ruleTail.valid()) { // <fixed, none>
            t = _RuleTypeHead;
        }
        else { // <fixed, fixed>
            t = _RuleTypeCenter;
        }
    }
    zffloat &childHead = xAxis ? _childFrame(childIndex).x : _childFrame(childIndex).y;
    zffloat &childSize = xAxis ? _childFrame(childIndex).width : _childFrame(childIndex).height;
    ZFUISizeType sizeType = xAxis ? layoutParam->sizeParam().width : layoutParam->sizeParam().height;
    zffloat sizeHint = xAxis ? layoutParam->sizeHint().width : layoutParam->sizeHint().height;
    zffloat marginHead = xAxis ? layoutParam->margin().left : layoutParam->margin().top;
    zffloat marginTail = xAxis ? layoutParam->margin().right : layoutParam->margin().bottom;

    if(measureMode) {
        rangeSize = zfmMax<zffloat>(rangeSize, marginHead + childSize + marginTail);
    }

    switch(t) {
        case _RuleTypeCenter: {
            if(sizeType == v_ZFUISizeType::e_Fill) {
                ZFUISizeApplyRangeT(
                        childSize
                        , sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - marginHead - marginTail)
                        , xAxis ? layoutParam->ownerView()->viewSizeMin().width : layoutParam->ownerView()->viewSizeMin().height
                        , xAxis ? layoutParam->ownerView()->viewSizeMax().width : layoutParam->ownerView()->viewSizeMax().height
                        );
            }
            zffloat bias = xAxis ? layoutParam->biasX() : layoutParam->biasY();
            childHead = headPos + marginHead + (rangeSize - marginHead - marginTail - childSize) * bias;
            break;
        }
        case _RuleTypeHead:
            childHead = headPos + marginHead;
            if(sizeType == v_ZFUISizeType::e_Fill) {
                ZFUISizeApplyRangeT(
                        childSize
                        , sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - marginHead - marginTail)
                        , xAxis ? layoutParam->ownerView()->viewSizeMin().width : layoutParam->ownerView()->viewSizeMin().height
                        , xAxis ? layoutParam->ownerView()->viewSizeMax().width : layoutParam->ownerView()->viewSizeMax().height
                        );
            }
            break;
        case _RuleTypeTail:
            if(sizeType == v_ZFUISizeType::e_Fill) {
                ZFUISizeApplyRangeT(
                        childSize
                        , sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - marginHead - marginTail)
                        , xAxis ? layoutParam->ownerView()->viewSizeMin().width : layoutParam->ownerView()->viewSizeMin().height
                        , xAxis ? layoutParam->ownerView()->viewSizeMax().width : layoutParam->ownerView()->viewSizeMax().height
                        );
            }
            childHead = headPos + rangeSize - marginTail - childSize;
            break;
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::recursiveRuleAssert(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT ZFCoreArray<_ZFP_ZFUIAutoLayoutRuleStackData> &ruleStack
        , ZF_IN zfbool layouting
        , ZF_IN zfuint level
        , ZF_IN _ZFP_ZFUIAutoLayoutRuleType ruleType
        , ZF_IN zfindex childIndex
        ) {
    {
        _ZFP_ZFUIAutoLayoutRuleStackData ruleData;
        ruleData.level = level;
        ruleData.ruleType = ruleType;
        ruleData.childIndex = childIndex;
        ruleStack.add(ruleData);
    }
    if(!layouting) {
        return;
    }
    zfstring errorHint;
    errorHint += "[ZFUIAutoLayout] recursive rule detected, stack graph:";
    const zfchar *ruleTypeMap[] = {
        "xAxis",
        "yAxis",
        "width",
        "height",
    };
    for(zfindex i = 0; i < ruleStack.count(); ++i) {
        const _ZFP_ZFUIAutoLayoutRuleStackData &ruleData = ruleStack[i];
        ZFUIView *child = parent->childAt(ruleData.childIndex);
        errorHint += "\n";
        zfstringRepeatT(errorHint, "  ", ruleData.level + 1);
        zfstringAppend(errorHint, "[%s].%s : %s%s"
                , ruleData.childIndex
                , ruleTypeMap[(zfuint)ruleType]
                , child->viewId() ? zfstr("(%s) ", child->viewId()).cString() : ""
                , child->objectInfoOfInstance()
                );
    }
    ZFCoreCriticalMessageTrim(errorHint);
}

ZF_NAMESPACE_GLOBAL_END

