#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
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
            , ZF_IN_OUT zffloat &parentWidth
            , ZF_IN_OUT zffloat &parentHeight
            , ZF_IN zfindex childIndex
            , ZF_IN zfbool xAxis
            );
private:
    void updateChildSize(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT zffloat &parentWidth
            , ZF_IN_OUT zffloat &parentHeight
            , ZF_IN zfindex childIndex
            , ZF_IN ZFUIAutoLayoutRule &rule
            , ZF_IN zfbool xAxis
            );
    void updateChain(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT ZFCoreArray<zfindex> &chain
            , ZF_IN zfindex childIndex
            , ZF_IN ZFUIAutoLayoutRule *ruleList
            , ZF_IN ZFUIAutoLayoutPosEnum posHead
            , ZF_IN ZFUIAutoLayoutPosEnum posTail
            );
    void calcChildHead(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT zffloat &parentWidth
            , ZF_IN_OUT zffloat &parentHeight
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPosEnum posHead
            , ZF_OUT zffloat &head
            , ZF_IN zfbool xAxis
            );
    void calcChildTail(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT zffloat &parentWidth
            , ZF_IN_OUT zffloat &parentHeight
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPosEnum posTail
            , ZF_OUT zffloat &tail
            , ZF_IN zfbool xAxis
            );
    void layoutChildByChain(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN_OUT zffloat &parentWidth
            , ZF_IN_OUT zffloat &parentHeight
            , ZF_IN const ZFCoreArray<zfindex> &chain
            , ZF_IN zfbool xAxis
            );
    void layoutChildByRange(
            ZF_IN ZFUIAutoLayoutRule &ruleHead
            , ZF_IN ZFUIAutoLayoutRule &ruleTail
            , ZF_IN ZFUIAutoLayoutParam *layoutParam
            , ZF_IN zfindex childIndex
            , ZF_IN zffloat headPos
            , ZF_IN zffloat rangeSize
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
        , ZF_IN ZFUIViewChildLayerEnum layer
        ) {
    if(layer == ZFUIViewChildLayer::e_Normal) {
        ZFUIAutoLayoutParam::_ZFP_Data &t = child->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d;
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
    d->prepareLayout(this, sizeHint);
    ZFUISizeApplyRange(ret.width, 0, this->viewSizeMin().width, this->viewSizeMax().height);
    ZFUISizeApplyRange(ret.height, 0, this->viewSizeMin().height, this->viewSizeMax().height);

    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        d->layoutChild(this, ret.width, ret.height, i, zftrue);
        d->layoutChild(this, ret.width, ret.height, i, zffalse);
    }
    zffloat parentLeft = 0;
    zffloat parentTop = 0;
    zffloat parentRight = 0;
    zffloat parentBottom = 0;
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        const ZFUIRect &childFrame = d->_childFrame(i);
        parentLeft = zfmMin(parentLeft, ZFUIRectGetLeft(childFrame));
        parentTop = zfmMin(parentTop, ZFUIRectGetTop(childFrame));
        parentRight = zfmMax(parentRight, ZFUIRectGetRight(childFrame));
        parentBottom = zfmMax(parentBottom, ZFUIRectGetBottom(childFrame));
    }
    ZFUISizeApplyRange(ret.width, parentRight - parentLeft, this->viewSizeMin().width, this->viewSizeMax().height);
    ZFUISizeApplyRange(ret.height, parentBottom - parentTop, this->viewSizeMin().height, this->viewSizeMax().height);
}
void ZFUIAutoLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    d->prepareLayout(this, ZFUIRectGetSize(this->viewFrame()));
    zffloat parentWidth = bounds.width;
    zffloat parentHeight = bounds.height;
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        d->layoutChild(this, parentWidth, parentHeight, i, zftrue);
        d->layoutChild(this, parentWidth, parentHeight, i, zffalse);
    }
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        this->childAt(i)->viewFrame(d->_childFrame(i));
    }
}

// ============================================================
void _ZFP_ZFUIAutoLayoutPrivate::layoutChild(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT zffloat &parentWidth
        , ZF_IN_OUT zffloat &parentHeight
        , ZF_IN zfindex childIndex
        , ZF_IN zfbool xAxis
        ) {
    zfbool &_layouting = (xAxis ? _layoutingX(childIndex) : _layoutingY(childIndex));
    zfbool &_layoutFinish = (xAxis ? _layoutFinishX(childIndex) : _layoutFinishY(childIndex));
    ZFCoreAssertWithMessageTrim(!_layouting,
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAt(childIndex)->objectInfoOfInstance(),
        childIndex);
    if(_layoutFinish) {
        return;
    }
    _layouting = zftrue;

    ZFCoreArray<zfindex> chain;
    ZFUIView *child = parent->childAt(childIndex);
    ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
    ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_AL_d.ruleList;
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;

    this->updateChildSize(parent, parentWidth, parentHeight, childIndex, ruleList[xAxis ? ZFUIAutoLayoutPos::e_Width : ZFUIAutoLayoutPos::e_Height], xAxis);

    this->updateChain(parent, chain, childIndex, ruleList, posHead, posTail);
    if(!chain.isEmpty()) {
        this->layoutChildByChain(parent, parentWidth, parentHeight, chain, xAxis);
    }
    else {
        zffloat head = 0;
        zffloat tail = 0;
        this->calcChildHead(parent, parentWidth, parentHeight, child, posHead, head, xAxis);
        this->calcChildTail(parent, parentWidth, parentHeight, child, posTail, tail, xAxis);
        this->layoutChildByRange(
                ruleList[posHead]
                , ruleList[posTail]
                , layoutParam
                , childIndex
                , head
                , tail - head
                , xAxis
                );
    }

    _layouting = zffalse;
    _layoutFinish = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChildSize(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT zffloat &parentWidth
        , ZF_IN_OUT zffloat &parentHeight
        , ZF_IN zfindex childIndex
        , ZF_IN ZFUIAutoLayoutRule &rule
        , ZF_IN zfbool xAxis
        ) {
    if(!rule.valid()) {
        return;
    }

    zfbool &_layouting = (xAxis ? _layoutingWidth(childIndex) : _layoutingHeight(childIndex));
    zfbool &_layoutFinish = (xAxis ? _layoutFinishWidth(childIndex) : _layoutFinishHeight(childIndex));
    ZFCoreAssertWithMessageTrim(!_layouting,
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAt(childIndex)->objectInfoOfInstance(),
        childIndex);
    if(_layoutFinish) {
        return;
    }
    _layouting = zftrue;

    _ZFP_ZFUIAutoLayout_targetUpdate(rule, parent, parent->childAt(childIndex));
    ZFUIView *target = rule.target();

    zffloat &_childSize = (xAxis ? _childFrame(childIndex).width : _childFrame(childIndex).height);
    if(rule.targetPos() == ZFUIAutoLayoutPos::e_Width) {
        if(target == parent) {
            _childSize = zfmMax(_childSize, (zffloat)(parentWidth + rule.offset()));
        }
        else {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, parentWidth, parentHeight, targetIndex, zftrue);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame(targetIndex).width + rule.offset()));
        }
    }
    else if(rule.targetPos() == ZFUIAutoLayoutPos::e_Height) {
        if(target == parent) {
            _childSize = zfmMax(_childSize, (zffloat)(parentHeight + rule.offset()));
        }
        else {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, parentWidth, parentHeight, targetIndex, zffalse);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame(targetIndex).height + rule.offset()));
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
        , ZF_IN ZFUIAutoLayoutPosEnum posHead
        , ZF_IN ZFUIAutoLayoutPosEnum posTail
        ) {
    ZFUIAutoLayoutRule *ruleListCur = ruleList;
    ZFUIAutoLayoutRule *ruleListNext = ruleList;
    ZFUIView *childPrev = parent->childAt(childIndex);
    zfbool centerChildHasAdd = zffalse;

    // head
    do {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListCur[posHead], parent, childPrev);
        ZFUIView *target = ruleListCur[posHead].target();
        if(!ruleListCur[posHead].valid() || target == parent) {
            break;
        }
        ruleListNext = target->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        if(ruleListNext[posTail].valid()) {
            _ZFP_ZFUIAutoLayout_targetUpdate(ruleListNext[posTail], parent, target);
            if(ruleListNext[posTail].target() == childPrev) {
                if(!centerChildHasAdd) {
                    centerChildHasAdd = zftrue;
                    chain.add(parent->childArray().find(childPrev), 0);
                }
                childPrev = target;
                ruleListCur = ruleListNext;
                chain.add(parent->childArray().find(childPrev), 0);
                continue;
            }
        }
        break;
    } while(zftrue);

    // tail
    ruleListCur = ruleList;
    childPrev = parent->childAt(childIndex);
    do {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleListCur[posTail], parent, childPrev);
        ZFUIView *target = ruleListCur[posTail].target();
        if(!ruleListCur[posTail].valid() || target == parent) {
            break;
        }
        ruleListNext = target->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        if(ruleListNext[posHead].valid()) {
            _ZFP_ZFUIAutoLayout_targetUpdate(ruleListNext[posHead], parent, target);
            if(ruleListNext[posHead].target() == childPrev) {
                if(!centerChildHasAdd) {
                    centerChildHasAdd = zftrue;
                    chain.add(parent->childArray().find(childPrev));
                }
                childPrev = target;
                ruleListCur = ruleListNext;
                chain.add(parent->childArray().find(childPrev));
                continue;
            }
        }
        break;
    } while(zftrue);
}

void _ZFP_ZFUIAutoLayoutPrivate::calcChildHead(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN_OUT zffloat &parentWidth
        , ZF_IN_OUT zffloat &parentHeight
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUIAutoLayoutPosEnum posHead
        , ZF_OUT zffloat &head
        , ZF_IN zfbool xAxis
        ) {
    head = 0;
    ZFUIAutoLayoutRule *ruleList = child->layoutParam()->to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
    if(ruleList[posHead].valid()) {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleList[posHead], parent, child);
        ZFUIView *target = ruleList[posHead].target();
        if(target != parent) {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, parentWidth, parentHeight, targetIndex, xAxis);
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
        , ZF_IN_OUT zffloat &parentWidth
        , ZF_IN_OUT zffloat &parentHeight
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUIAutoLayoutPosEnum posTail
        , ZF_OUT zffloat &tail
        , ZF_IN zfbool xAxis
        ) {
    tail = xAxis ? parentWidth : parentHeight;
    ZFUIAutoLayoutRule *ruleList = child->layoutParam()->to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
    if(ruleList[posTail].valid()) {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleList[posTail], parent, child);
        ZFUIView *target = ruleList[posTail].target();
        if(target != parent) {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, parentWidth, parentHeight, targetIndex, xAxis);
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
        , ZF_IN_OUT zffloat &parentWidth
        , ZF_IN_OUT zffloat &parentHeight
        , ZF_IN const ZFCoreArray<zfindex> &chain
        , ZF_IN zfbool xAxis
        ) {
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;
    zffloat head = 0;
    zffloat tail = 0;
    this->calcChildHead(parent, parentWidth, parentHeight
            , parent->childAt(chain.getFirst())
            , posHead, head, xAxis
            );
    this->calcChildTail(parent, parentWidth, parentHeight
            , parent->childAt(chain.getLast())
            , posTail, tail, xAxis
            );

    zffloat fixedSize = 0;
    zffloat *childSizeCache = (zffloat *)zfmalloc(sizeof(zffloat) * parent->childCount());
    zfblockedFree(childSizeCache);
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i) {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutRule *ruleList = parent->childAt(childIndex)->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        childSizeCache[childIndex] = xAxis ? _childFrame(childIndex).width : _childFrame(childIndex).height;
        if(ruleList[posHead].valid()) {
            childSizeCache[childIndex] += ruleList[posHead].offset();
        }
        if(ruleList[posTail].valid()) {
            childSizeCache[childIndex] += ruleList[posTail].offset();
        }
        fixedSize += childSizeCache[childIndex];
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
        this->layoutChildByRange(
                ruleList[posHead]
                , ruleList[posTail]
                , layoutParam
                , childIndex
                , tailTmp
                , childSizeCache[childIndex]
                , xAxis
                );
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByRange(
        ZF_IN ZFUIAutoLayoutRule &ruleHead
        , ZF_IN ZFUIAutoLayoutRule &ruleTail
        , ZF_IN ZFUIAutoLayoutParam *layoutParam
        , ZF_IN zfindex childIndex
        , ZF_IN zffloat headPos
        , ZF_IN zffloat rangeSize
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
    ZFUISizeTypeEnum sizeType = xAxis ? layoutParam->sizeParam().width : layoutParam->sizeParam().height;
    zffloat sizeHint = xAxis ? layoutParam->sizeHint().width : layoutParam->sizeHint().height;
    switch(t) {
        case _RuleTypeCenter: {
            if(sizeType == ZFUISizeType::e_Wrap) {
                childSize = ZFUISizeApplyRange(childSize
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            else {
                childSize = ZFUISizeApplyRange(
                        sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - ruleHead.offset() - ruleTail.offset())
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            zffloat bias = xAxis ? layoutParam->biasX() : layoutParam->biasY();
            childHead = headPos + ruleHead.offset() + (rangeSize - ruleHead.offset() - ruleTail.offset() - childSize) * bias;
            break;
        }
        case _RuleTypeHead:
            childHead = headPos + ruleHead.offset();
            if(sizeType == ZFUISizeType::e_Wrap) {
                childSize = ZFUISizeApplyRange(childSize
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            else {
                childSize = ZFUISizeApplyRange(
                        sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - ruleHead.offset() - ruleTail.offset())
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            break;
        case _RuleTypeTail:
            if(sizeType == ZFUISizeType::e_Wrap) {
                childSize = ZFUISizeApplyRange(childSize
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            else {
                childSize = ZFUISizeApplyRange(
                        sizeHint >= 0 ? sizeHint : zfmMax<zffloat>(childSize, rangeSize - ruleHead.offset() - ruleTail.offset())
                        , xAxis ? layoutParam->owner()->viewSizeMin().width : layoutParam->owner()->viewSizeMin().height
                        , xAxis ? layoutParam->owner()->viewSizeMax().width : layoutParam->owner()->viewSizeMax().height
                        );
            }
            childHead = headPos + rangeSize - ruleTail.offset() - childSize;
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

