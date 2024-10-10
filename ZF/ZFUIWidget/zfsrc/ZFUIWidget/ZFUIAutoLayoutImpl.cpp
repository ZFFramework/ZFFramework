#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIAutoLayoutPrivate {
public:
    zffloat _parentWidth;
    zffloat _parentHeight;
    zfindex _childCountCache;
    ZFUIRect *_childFrame;
    zffloat *_childSizeCacheX;
    zffloat *_childSizeCacheY;
    zfbool *_layoutingX;
    zfbool *_layoutingY;
    zfbool *_layoutingWidth;
    zfbool *_layoutingHeight;
    zfbool *_layoutFinishX;
    zfbool *_layoutFinishY;
    zfbool *_layoutFinishWidth;
    zfbool *_layoutFinishHeight;
public:
    _ZFP_ZFUIAutoLayoutPrivate(void)
    : _parentWidth(0)
    , _parentHeight(0)
    , _childCountCache(0)
    , _childFrame(zfnull)
    , _childSizeCacheX(zfnull)
    , _childSizeCacheY(zfnull)
    , _layoutingX(zfnull)
    , _layoutingY(zfnull)
    , _layoutingWidth(zfnull)
    , _layoutingHeight(zfnull)
    , _layoutFinishX(zfnull)
    , _layoutFinishY(zfnull)
    , _layoutFinishWidth(zfnull)
    , _layoutFinishHeight(zfnull)
    {
    }
    ~_ZFP_ZFUIAutoLayoutPrivate(void) {
        zffree(_childFrame);
    }

public:
    void prepareLayout(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN const ZFUISize &sizeHint
            ) {
        zfindex childCount = parent->childCount();
        if(_childCountCache < childCount) {
            _childCountCache = childCount;
            zfbyte *buf = (zfbyte *)zfrealloc(_childFrame, 0
                    + sizeof(ZFUIRect) * childCount // _childFrame
                    + sizeof(zffloat) * childCount // _childSizeCacheX
                    + sizeof(zffloat) * childCount // _childSizeCacheY
                    + sizeof(zfbool) * childCount // _layoutingX
                    + sizeof(zfbool) * childCount // _layoutingY
                    + sizeof(zfbool) * childCount // _layoutingWidth
                    + sizeof(zfbool) * childCount // _layoutingHeight
                    + sizeof(zfbool) * childCount // _layoutFinishX
                    + sizeof(zfbool) * childCount // _layoutFinishY
                    + sizeof(zfbool) * childCount // _layoutFinishWidth
                    + sizeof(zfbool) * childCount // _layoutFinishHeight
                );
            _childFrame = (ZFUIRect *)buf; buf += sizeof(ZFUIRect) * childCount;
            _childSizeCacheX = (zffloat *)buf; buf += sizeof(zffloat) * childCount;
            _childSizeCacheY = (zffloat *)buf; buf += sizeof(zffloat) * childCount;
            _layoutingX = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutingY = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutingWidth = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutingHeight = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutFinishX = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutFinishY = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutFinishWidth = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
            _layoutFinishHeight = (zfbool *)buf; buf += sizeof(zfbool) * childCount;
        }
        zfmemset(_layoutingX, 0, sizeof(zfbool) * childCount * 8);

        for(zfindex i = childCount - 1; i != zfindexMax(); --i) {
            ZFUIView *child = parent->childAt(i);
            ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
            ZFUISize childSize = child->layoutMeasure(
                ZFUILayoutParam::sizeHintMerge(sizeHint, layoutParam->sizeHint()),
                ZFUISizeParamWrapWrap());
            _childFrame[i].width = childSize.width;
            _childFrame[i].height = childSize.height;
        }
    }
    void layoutChild(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN zfindex childIndex
            , ZF_IN zfbool xAxis
            );
private:
    void updateChildSize(
            ZF_IN ZFUIAutoLayout *parent
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
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPosEnum posHead
            , ZF_OUT zffloat &head
            , ZF_IN zfbool xAxis
            );
    void calcChildTail(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN ZFUIView *child
            , ZF_IN ZFUIAutoLayoutPosEnum posTail
            , ZF_OUT zffloat &tail
            , ZF_IN zfbool xAxis
            );
    void layoutChildByChain(
            ZF_IN ZFUIAutoLayout *parent
            , ZF_IN const ZFCoreArray<zfindex> &chain
            , ZF_IN zfbool xAxis
            );
    void layoutChildByRange(
            ZF_IN ZFUIAutoLayoutRule &ruleHead
            , ZF_IN ZFUIAutoLayoutRule &ruleTail
            , ZF_IN zffloat headPos
            , ZF_IN zffloat childSizeCache
            , ZF_IN ZFUISizeTypeEnum sizeType
            , ZF_IN zffloat bias
            , ZF_IN_OUT zffloat &childHead
            , ZF_IN_OUT zffloat &childSize
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

void ZFUIAutoLayout::viewChildOnAdd(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayerEnum layer
        ) {
    zfsuper::viewChildOnAdd(child, layer);
    if(layer == ZFUIViewChildLayer::e_Normal) {
        ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
        ZFCoreAssertWithMessageTrim(
            layoutParam->_ZFP_AL_d.ownerParent == zfnull,
            "[ZFUIAutoLayout] layout param %s already attached to %s",
            child->layoutParam(),
            layoutParam->_ZFP_AL_d.ownerParent);
        layoutParam->_ZFP_AL_d.ownerParent = this;
        layoutParam->_ZFP_AL_d.ownerChild = child->objectHolder();
    }
}
void ZFUIAutoLayout::viewChildOnRemove(
        ZF_IN ZFUIView *child
        , ZF_IN ZFUIViewChildLayerEnum layer
        ) {
    if(layer == ZFUIViewChildLayer::e_Normal) {
        ZFUIAutoLayoutParam::_ZFP_Data &t = child->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d;
        t.ownerParent = zfnull;
        t.ownerChild = zfnull;
        if(d->_childCountCache > this->childCount() * 4) {
            d->_childCountCache = 0;
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
    d->_parentWidth = 0;
    d->_parentHeight = 0;

    // we need to calculate twice
    // because some rule may depends on parent's size
    for(zfindex i = 0; i < 2; ++i) {
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
            d->layoutChild(this, i, zftrue);
            d->layoutChild(this, i, zffalse);
        }

        zffloat parentLeft = 0;
        zffloat parentTop = 0;
        zffloat parentRight = 0;
        zffloat parentBottom = 0;
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
            const ZFUIRect &childFrame = d->_childFrame[i];
            parentLeft = zfmMin(parentLeft, ZFUIRectGetLeft(childFrame));
            parentTop = zfmMin(parentTop, ZFUIRectGetTop(childFrame));
            parentRight = zfmMax(parentRight, ZFUIRectGetRight(childFrame));
            parentBottom = zfmMax(parentBottom, ZFUIRectGetBottom(childFrame));
        }
        d->_parentWidth = parentRight - parentLeft;
        d->_parentHeight = parentBottom - parentTop;
    }
    ret.width = d->_parentWidth;
    ret.height = d->_parentHeight;
}
void ZFUIAutoLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
    d->prepareLayout(this, ZFUIRectGetSize(this->viewFrame()));
    d->_parentWidth = bounds.width;
    d->_parentHeight = bounds.height;
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        d->layoutChild(this, i, zftrue);
        d->layoutChild(this, i, zffalse);
    }
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i) {
        this->childAt(i)->viewFrame(d->_childFrame[i]);
    }
}

// ============================================================
void _ZFP_ZFUIAutoLayoutPrivate::layoutChild(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN zfindex childIndex
        , ZF_IN zfbool xAxis
        ) {
    zfbool *&_layouting = (xAxis ? _layoutingX : _layoutingY);
    zfbool *&_layoutFinish = (xAxis ? _layoutFinishX : _layoutFinishY);
    ZFCoreAssertWithMessageTrim(!_layouting[childIndex],
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAt(childIndex)->objectInfoOfInstance(),
        childIndex);
    if(_layoutFinish[childIndex]) {
        return;
    }
    _layouting[childIndex] = zftrue;

    ZFCoreArray<zfindex> chain;
    ZFUIView *child = parent->childAt(childIndex);
    ZFUIAutoLayoutParam *layoutParam = child->layoutParam();
    ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_AL_d.ruleList;
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;

    this->updateChildSize(parent, childIndex, ruleList[xAxis ? ZFUIAutoLayoutPos::e_Width : ZFUIAutoLayoutPos::e_Height], xAxis);

    this->updateChain(parent, chain, childIndex, ruleList, posHead, posTail);
    if(!chain.isEmpty()) {
        this->layoutChildByChain(parent, chain, xAxis);
    }
    else {
        zffloat head = 0;
        zffloat tail = 0;
        this->calcChildHead(parent, child, posHead, head, xAxis);
        this->calcChildTail(parent, child, posTail, tail, xAxis);
        this->layoutChildByRange(
            ruleList[posHead],
            ruleList[posTail],
            head,
            tail - head,
            xAxis ? layoutParam->sizeParam().width : layoutParam->sizeParam().height,
            xAxis ? layoutParam->biasX() : layoutParam->biasY(),
            xAxis ? _childFrame[childIndex].x : _childFrame[childIndex].y,
            xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height);
    }

    _layouting[childIndex] = zffalse;
    _layoutFinish[childIndex] = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChildSize(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN zfindex childIndex
        , ZF_IN ZFUIAutoLayoutRule &rule
        , ZF_IN zfbool xAxis
        ) {
    if(!rule.valid()) {
        return;
    }

    zfbool *&_layouting = xAxis ? _layoutingWidth : _layoutingHeight;
    zfbool *&_layoutFinish = xAxis ? _layoutFinishWidth : _layoutFinishHeight;
    ZFCoreAssertWithMessageTrim(!_layouting[childIndex],
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAt(childIndex)->objectInfoOfInstance(),
        childIndex);
    if(_layoutFinish[childIndex]) {
        return;
    }
    _layouting[childIndex] = zftrue;

    _ZFP_ZFUIAutoLayout_targetUpdate(rule, parent, parent->childAt(childIndex));
    ZFUIView *target = rule.target();

    zffloat &_childSize = xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height;
    if(rule.targetPos() == ZFUIAutoLayoutPos::e_Width) {
        if(target == parent) {
            _childSize = zfmMax(_childSize, (zffloat)(_parentWidth + rule.offset()));
        }
        else {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, targetIndex, xAxis);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame[targetIndex].width + rule.offset()));
        }
    }
    else if(rule.targetPos() == ZFUIAutoLayoutPos::e_Height) {
        if(target == parent) {
            _childSize = zfmMax(_childSize, (zffloat)(_parentHeight + rule.offset()));
        }
        else {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, targetIndex, xAxis);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame[targetIndex].height + rule.offset()));
        }
    }

    _layouting[childIndex] = zftrue;
    _layoutFinish[childIndex] = zftrue;
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
            this->layoutChild(parent, targetIndex, xAxis);
            if(ruleList[posHead].targetPos() == posHead) {
                head = xAxis ? ZFUIRectGetLeft(_childFrame[targetIndex]) : ZFUIRectGetTop(_childFrame[targetIndex]);
            }
            else {
                head = xAxis ? ZFUIRectGetRight(_childFrame[targetIndex]) : ZFUIRectGetBottom(_childFrame[targetIndex]);
            }
        }
    }
}
void _ZFP_ZFUIAutoLayoutPrivate::calcChildTail(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        , ZF_IN ZFUIAutoLayoutPosEnum posTail
        , ZF_OUT zffloat &tail
        , ZF_IN zfbool xAxis
        ) {
    tail = xAxis ? _parentWidth : _parentHeight;
    ZFUIAutoLayoutRule *ruleList = child->layoutParam()->to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
    if(ruleList[posTail].valid()) {
        _ZFP_ZFUIAutoLayout_targetUpdate(ruleList[posTail], parent, child);
        ZFUIView *target = ruleList[posTail].target();
        if(target != parent) {
            zfindex targetIndex = parent->childArray().find(target);
            this->layoutChild(parent, targetIndex, xAxis);
            if(ruleList[posTail].targetPos() == posTail) {
                tail = xAxis ? ZFUIRectGetRight(_childFrame[targetIndex]) : ZFUIRectGetBottom(_childFrame[targetIndex]);
            }
            else {
                tail = xAxis ? ZFUIRectGetLeft(_childFrame[targetIndex]) : ZFUIRectGetTop(_childFrame[targetIndex]);
            }
        }
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByChain(
        ZF_IN ZFUIAutoLayout *parent
        , ZF_IN const ZFCoreArray<zfindex> &chain
        , ZF_IN zfbool xAxis
        ) {
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;
    zffloat head = 0;
    zffloat tail = 0;
    this->calcChildHead(parent,
        parent->childAt(chain.getFirst()),
        posHead, head, xAxis);
    this->calcChildTail(parent,
        parent->childAt(chain.getLast()),
        posTail, tail, xAxis);

    zffloat fixedSize = 0;
    zffloat *&_childSizeCache = xAxis ? _childSizeCacheX : _childSizeCacheY;
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i) {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutRule *ruleList = parent->childAt(childIndex)->layoutParam().to<ZFUIAutoLayoutParam *>()->_ZFP_AL_d.ruleList;
        _childSizeCache[childIndex] = xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height;
        if(ruleList[posHead].valid()) {
            _childSizeCache[childIndex] += ruleList[posHead].offset();
        }
        if(ruleList[posTail].valid()) {
            _childSizeCache[childIndex] += ruleList[posTail].offset();
        }
        fixedSize += _childSizeCache[childIndex];
    }
    zffloat flexibleUnit = 0;
    flexibleUnit = (tail - head - fixedSize) / chain.count();

    zffloat tailTmp = tail;
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i) {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutParam *layoutParam = parent->childAt(childIndex)->layoutParam();
        ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_AL_d.ruleList;
        _childSizeCache[childIndex] += flexibleUnit;
        tailTmp -= _childSizeCache[childIndex];
        this->layoutChildByRange(
            ruleList[posHead],
            ruleList[posTail],
            tailTmp,
            _childSizeCache[childIndex],
            xAxis ? layoutParam->sizeParam().width : layoutParam->sizeParam().height,
            xAxis ? layoutParam->biasX() : layoutParam->biasY(),
            xAxis ? _childFrame[childIndex].x : _childFrame[childIndex].y,
            xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height);
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByRange(
        ZF_IN ZFUIAutoLayoutRule &ruleHead
        , ZF_IN ZFUIAutoLayoutRule &ruleTail
        , ZF_IN zffloat headPos
        , ZF_IN zffloat childSizeCache
        , ZF_IN ZFUISizeTypeEnum sizeType
        , ZF_IN zffloat bias
        , ZF_IN_OUT zffloat &childHead
        , ZF_IN_OUT zffloat &childSize
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
    switch(t) {
        case _RuleTypeCenter:
            if(sizeType == ZFUISizeType::e_Wrap) {
                childHead = headPos + ruleHead.offset() + (childSizeCache - ruleHead.offset() - ruleTail.offset() - childSize) * bias;
            }
            else {
                childHead = headPos + ruleHead.offset();
                childSize = childSizeCache - ruleHead.offset() - ruleTail.offset();
            }
            break;
        case _RuleTypeHead:
            childHead = headPos + ruleHead.offset();
            break;
        case _RuleTypeTail:
            childHead = headPos + childSizeCache - ruleTail.offset() - childSize;
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

