#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// misc
ZFENUM_DEFINE(ZFUIAutoLayoutPos)
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, ZFUIAutoLayoutPosEnum, pos)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zfanyT<ZFUIView>, target)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, ZFUIAutoLayoutPosEnum, targetPos)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zffloat, weight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zffloat, offset)

void ZFUIAutoLayoutRule::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFUIAutoLayoutRule pos:%s target:%s targetPos:%s weight:%s offset:%s>"
            , this->pos()
            , this->target()
            , this->targetPos()
            , this->weight()
            , this->offset()
            );
}

// ============================================================
// ZFUIAutoLayoutParam
ZFOBJECT_REGISTER(ZFUIAutoLayoutParam)

static void _ZFP_ZFUIAutoLayoutAlignApply(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN const ZFUIAlignFlags &layoutAlign
        , ZF_IN const ZFUIMargin &layoutMargin
        ) {
    if(ZFBitTest(layoutAlign, ZFUIAlign::e_Left)) {
        lp->c_left()->c_toParentLeft()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.left);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_Right)) {
        lp->c_right()->c_toParentRight()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.right);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_LeftEdge)) {
        lp->c_right()->c_toParentLeft()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.right);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_RightEdge)) {
        lp->c_left()->c_toParentRight()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.left);
    }
    else {
        lp
            ->c_left()->c_toParentLeft()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.left)
            ->c_right()->c_toParentRight()->c_weight(0)->c_biasX(0.5f)->c_offset(layoutMargin.right)
            ;
    }

    if(ZFBitTest(layoutAlign, ZFUIAlign::e_Top)) {
        lp->c_top()->c_toParentTop()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.top);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_Bottom)) {
        lp->c_bottom()->c_toParentBottom()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.bottom);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_TopEdge)) {
        lp->c_bottom()->c_toParentTop()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.bottom);
    }
    else if(ZFBitTest(layoutAlign, ZFUIAlign::e_BottomEdge)) {
        lp->c_top()->c_toParentBottom()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.top);
    }
    else {
        lp
            ->c_top()->c_toParentTop()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.top)
            ->c_bottom()->c_toParentBottom()->c_weight(0)->c_biasY(0.5f)->c_offset(layoutMargin.bottom)
            ;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIAutoLayoutParam, ZFUIAlignFlags, layoutAlign) {
    _ZFP_ZFUIAutoLayoutAlignApply(this, propertyValue, this->layoutMargin());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIAutoLayoutParam, ZFUIMargin, layoutMargin) {
    _ZFP_ZFUIAutoLayoutAlignApply(this, this->layoutAlign(), propertyValue);
}

ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, zfanyT<ZFUIAutoLayout>, ownerParent) {
    return _ZFP_AL_d.ownerParent;
}

ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, zfanyT<ZFUIView>, ownerChild) {
    if(_ZFP_AL_d.ownerChild != zfnull) {
        return _ZFP_AL_d.ownerChild->objectHolded();
    }
    else {
        return zfnull;
    }
}

ZFSerializablePropertyType ZFUIAutoLayoutParam::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property) {
    if(zffalse
        || property == ZFPropertyAccess(zfself, layoutAlign)
        || property == ZFPropertyAccess(zfself, layoutMargin)
    ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    return zfsuper::serializableOnCheckPropertyType(property);
}

ZFCompareResult ZFUIAutoLayoutParam::objectCompare(ZF_IN ZFObject *anotherObj) {
    zfself *ref = zfcast(zfself *, anotherObj);
    if(anotherObj == zfnull || zfsuper::objectCompare(anotherObj) != ZFCompareTheSame) {
        return ZFCompareUncomparable;
    }
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        if(_ZFP_AL_d.ruleList[i] != ref->_ZFP_AL_d.ruleList[i]) {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfbool ZFUIAutoLayoutParam::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // remove all rule
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        _ZFP_AL_d.ruleList[i].pos(ZFUIAutoLayoutPos::e_None);
    }

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &element = serializableData.childAt(i);
        if(element.resolved()
                || ZFSerializableUtil::checkItemClass(element, ZFSerializableKeyword_ZFUIAutoLayoutParam_rule) == zfnull
                ) {
            continue;
        }

        ZFUIAutoLayoutPosEnum pos = ZFUIAutoLayoutPos::e_None;

        // pos
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
                require, ZFSerializableKeyword_ZFUIAutoLayoutParam_pos, ZFUIAutoLayoutPos, pos, {
                    return zffalse;
                });
        if(pos == ZFUIAutoLayoutPos::e_None) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, element,
                "\"%s\" does not support \"%s\"",
                ZFSerializableKeyword_ZFUIAutoLayoutParam_pos,
                pos);
            return zffalse;
        }
        if(_ZFP_AL_d.ruleList[pos].pos() != ZFUIAutoLayoutPos::e_None) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, element,
                "\"%s\" rule already has been set",
                pos);
            return zffalse;
        }

        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];

        // target/targetPos
        {
            const zfchar *value = ZFSerializableUtil::requireAttribute(
                element, ZFSerializableKeyword_ZFUIAutoLayoutParam_target, outErrorHint, outErrorPos);
            if(value == zfnull) {
                return zffalse;
            }
            const zfchar *valueTmp = value;
            while(*valueTmp != '\0' && *valueTmp != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token) {++valueTmp;}
            ZFUIAutoLayoutPosEnum targetPos = ZFUIAutoLayoutPos::e_None;
            if(*valueTmp != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token
                    || !ZFUIAutoLayoutPosFromStringT(targetPos, value, valueTmp - value)
                    ) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, element,
                    "invalid target value: %s",
                    value);
                return zffalse;
            }
            rule.targetPos(targetPos);
            rule._ZFP_AL_targetId = valueTmp + 1;
        }

        // weight
        zffloat weight = 0;
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFUIAutoLayoutParam_weight, zffloat, weight, {
                    return zffalse;
                });
        rule.weight(weight);

        // offset
        zffloat offset = 0;
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, offset, {
                    return zffalse;
                });
        rule.offset(offset);

        // finish
        element.resolveMark();
        rule.pos(pos);
    }
    return zftrue;
}
zfbool ZFUIAutoLayoutParam::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        const ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[i];
        if(ref != zfnull && ref->_ZFP_AL_d.ruleList[i] == rule) {
            continue;
        }
        if(rule.pos() == ZFUIAutoLayoutPos::e_None) {
            continue;
        }
        ZFSerializableData element;

        // pos
        element.attr(ZFSerializableKeyword_ZFUIAutoLayoutParam_pos, ZFUIAutoLayoutPosToString(rule.pos()));

        // target
        {
            zfstring target;
            ZFUIAutoLayoutPosToStringT(target, rule.targetPos());
            target += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token;
            _ZFP_ZFUIAutoLayout_targetIdUpdate(target, rule, this->ownerParent(), this->ownerChild());
            element.attr(ZFSerializableKeyword_ZFUIAutoLayoutParam_target, target);
        }

        // weight
        ZFSerializableUtilSerializeAttributeToDataNoRef(element, outErrorHint,
                ZFSerializableKeyword_ZFUIAutoLayoutParam_weight, zffloat, rule.weight(), 0, {
                    return zffalse;
                });

        // offset
        ZFSerializableUtilSerializeAttributeToDataNoRef(element, outErrorHint,
                ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, rule.offset(), 0, {
                    return zffalse;
                });

        element.itemClass(ZFSerializableKeyword_ZFUIAutoLayoutParam_rule);
        serializableData.childAdd(element);
    }

    return zftrue;
}

// ============================================================
// ZFUIAutoLayout
ZFSTYLE_DEFAULT_DEFINE(ZFUIAutoLayout)
ZFOBJECT_REGISTER(ZFUIAutoLayout)

ZFMETHOD_DEFINE_2(ZFUIAutoLayout, zfanyT<ZFUIAutoLayoutParam>, childAdd
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    return this->childAddWithParam(view, zfnull, atIndex);
}

zfbool ZFUIAutoLayout::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {
        return zffalse;
    }

    for(zfindex iChild = this->childCount() - 1; iChild != zfindexMax(); --iChild) {
        ZFUIView *child = this->childAt(iChild);
        ZFUIAutoLayoutParam *lp = child->layoutParam();
        for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
            ZFUIAutoLayoutRule &rule = lp->_ZFP_AL_d.ruleList[i];
            if(!rule._ZFP_AL_targetId.isEmpty()) {
                if(!_ZFP_ZFUIAutoLayout_targetUpdate(rule, this, child, rule._ZFP_AL_targetId)) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                        "%s invalid target \"%s\" for %s",
                        this,
                        rule._ZFP_AL_targetId,
                        child);
                    return zffalse;
                }
                rule._ZFP_AL_targetId.removeAll();
            }
        }
    }

    return zftrue;
}

// ============================================================
zfbool _ZFP_ZFUIAutoLayout_targetUpdate(
        ZF_IN_OUT ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        , ZF_IN const zfchar *targetId
        ) {
    if(rule.pos() == ZFUIAutoLayoutPos::e_None) {
        return zftrue;
    }
    rule.target(zfnull);
    if(!zfstringIsEmpty(targetId)) {
        if(targetId[0] == '@') {
            rule.target(parent->childFindById(targetId + 1, zfHint("findRecursively")zffalse));
        }
        else if(zfstringIsEqual(targetId, ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent)) {
            rule.target(parent);
        }
        else if(zfstringIsEqual(targetId, ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self)) {
            rule.target(child);
        }
        else {
            zfindex childIndexRef = zfindexMax();
            if(zfindexFromStringT(childIndexRef, targetId) && childIndexRef < parent->childCount()) {
                rule.target(parent->childAt(childIndexRef));
            }
        }
    }
    return (rule.target() != zfnull);
}
zfbool _ZFP_ZFUIAutoLayout_targetIdUpdate(
        ZF_OUT zfstring &targetId
        , ZF_IN const ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        ) {
    ZFUIView *target = rule.target();
    if(target == zfnull || (target != parent && rule.target()->viewParent() != parent)) {
        return zffalse;
    }
    if(target == parent) {
        targetId += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent;
    }
    else if(!target->viewId().isEmpty()) {
        targetId += target->viewId();
    }
    else if(target == child) {
        targetId += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self;
    }
    else {
        zfindex childIndexRef = parent->childFind(target);
        if(childIndexRef == zfindexMax()) {
            return zffalse;
        }
        else {
            zfindexToStringT(targetId, childIndexRef);
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

