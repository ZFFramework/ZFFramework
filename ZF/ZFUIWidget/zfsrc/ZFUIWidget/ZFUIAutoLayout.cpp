#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// misc
ZFENUM_DEFINE(ZFUIAutoLayoutPos)
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zfanyT<ZFUIView>, target)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, ZFUIAutoLayoutPosEnum, targetPos)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zffloat, offset)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutRule, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutRule, void, removeAll)

void ZFUIAutoLayoutRule::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFUIAutoLayoutRule target:%s targetPos:%s offset:%s>"
            , this->target()
            , this->targetPos()
            , this->offset()
            );
}

// ============================================================
// ZFUIAutoLayoutParam
ZFOBJECT_REGISTER(ZFUIAutoLayoutParam)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIAutoLayoutParam, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

static void _ZFP_ZFUIAutoLayoutAlignApply(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN const ZFUIAlignFlags &align
        , ZF_IN const ZFUIMargin &margin
        ) {
    if(ZFBitTest(align, ZFUIAlign::e_Left)) {
        lp->c_left()->c_toParentLeft()->c_biasX(0.5f)->c_offset(margin.left);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Right);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Right)) {
        lp->c_right()->c_toParentRight()->c_biasX(0.5f)->c_offset(margin.right);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Left);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_LeftEdge)) {
        lp->c_right()->c_toParentLeft()->c_biasX(0.5f)->c_offset(margin.right);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Left);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_RightEdge)) {
        lp->c_left()->c_toParentRight()->c_biasX(0.5f)->c_offset(margin.left);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Right);
    }
    else {
        lp
            ->c_left()->c_toParentLeft()->c_offset(margin.left)
            ->c_right()->c_toParentRight()->c_offset(margin.right)
            ->c_biasX(0.5f)
            ;
    }

    if(ZFBitTest(align, ZFUIAlign::e_Top)) {
        lp->c_top()->c_toParentTop()->c_biasY(0.5f)->c_offset(margin.top);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Bottom);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Bottom)) {
        lp->c_bottom()->c_toParentBottom()->c_biasY(0.5f)->c_offset(margin.bottom);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Top);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_TopEdge)) {
        lp->c_bottom()->c_toParentTop()->c_biasY(0.5f)->c_offset(margin.bottom);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Top);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_BottomEdge)) {
        lp->c_top()->c_toParentBottom()->c_biasY(0.5f)->c_offset(margin.top);
        lp->ruleRemove(ZFUIAutoLayoutPos::e_Bottom);
    }
    else {
        lp
            ->c_top()->c_toParentTop()->c_offset(margin.top)
            ->c_bottom()->c_toParentBottom()->c_offset(margin.bottom)
            ->c_biasY(0.5f)
            ;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIAutoLayoutParam, ZFUIAlignFlags, align) {
    _ZFP_ZFUIAutoLayoutAlignApply(this, propertyValue, this->margin());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIAutoLayoutParam, ZFUIMargin, margin) {
    _ZFP_ZFUIAutoLayoutAlignApply(this, this->align(), propertyValue);
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
        || property == ZFPropertyAccess(zfself, align)
        || property == ZFPropertyAccess(zfself, margin)
    ) {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    return zfsuper::serializableOnCheckPropertyType(property);
}

zfbool ZFUIAutoLayoutParam::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // remove all rule
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        _ZFP_AL_d.ruleList[i].removeAll();
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
        ZFSerializableUtilSerializeAttrFromData(element, outErrorHint, outErrorPos,
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
        if(_ZFP_AL_d.ruleList[pos].valid()) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, element,
                "\"%s\" rule already has been set",
                pos);
            return zffalse;
        }

        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];

        // target/targetPos
        {
            zfstring value = ZFSerializableUtil::requireAttr(
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

        // offset
        zffloat offset = 0;
        ZFSerializableUtilSerializeAttrFromData(element, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, offset, {
                    return zffalse;
                });
        rule.offset(offset);

        // finish
        element.resolveMark();
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
        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[i];
        if(ref != zfnull && ref->_ZFP_AL_d.ruleList[i] == rule) {
            continue;
        }
        if(!rule.valid()) {
            continue;
        }
        ZFSerializableData element;

        // pos
        element.attr(ZFSerializableKeyword_ZFUIAutoLayoutParam_pos, ZFUIAutoLayoutPosToString((ZFUIAutoLayoutPosEnum)i));

        // target
        {
            zfstring target;
            ZFUIAutoLayoutPosToStringT(target, rule.targetPos());
            target += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token;
            _ZFP_ZFUIAutoLayout_targetIdUpdate(target, rule, this->ownerParent(), this->ownerChild());
            element.attr(ZFSerializableKeyword_ZFUIAutoLayoutParam_target, target);
        }

        // offset
        ZFSerializableUtilSerializeAttrToDataNoRef(element, outErrorHint,
                ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, rule.offset(), 0, {
                    return zffalse;
                });

        element.itemClass(ZFSerializableKeyword_ZFUIAutoLayoutParam_rule);
        serializableData.child(element);
    }

    return zftrue;
}

void ZFUIAutoLayoutParam::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable) {
    zfsuper::styleableOnCopyFrom(anotherStyleable);
    // copy ZFUIAutoLayoutParam does not copy rules,
    // it's done by ZFUIAutoLayout::styleableOnCopyFrom
}
ZFCompareResult ZFUIAutoLayoutParam::objectCompareValue(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}
    if(zfsuper::objectCompareValue(anotherObj) != ZFCompareEqual) {
        return ZFCompareUncomparable;
    }
    for(zfindex i = 0; i < ZFUIAutoLayoutPos::EnumCount(); ++i) {
        ZFUIAutoLayoutPosEnum pos = (ZFUIAutoLayoutPosEnum)ZFUIAutoLayoutPos::EnumValueAt(i);
        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];
        ZFUIAutoLayoutRule &ruleRef = another->_ZFP_AL_d.ruleList[pos];
        if(rule.valid() != ruleRef.valid()
                || rule.targetPos() != ruleRef.targetPos()
                || rule.offset() != ruleRef.offset()
                ) {
            return ZFCompareUncomparable;
        }
        zfstring targetId;
        zfstring targetIdRef;
        _ZFP_ZFUIAutoLayout_targetIdUpdate(targetId, rule, this->ownerParent(), this->ownerChild());
        _ZFP_ZFUIAutoLayout_targetIdUpdate(targetIdRef, ruleRef, another->ownerParent(), another->ownerChild());
        if(targetId != targetIdRef) {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareEqual;
}

// ============================================================
// ZFUIAutoLayout
ZFSTYLE_DEFAULT_DEFINE(ZFUIAutoLayout)
ZFOBJECT_REGISTER(ZFUIAutoLayout)

ZFMETHOD_DEFINE_2(ZFUIAutoLayout, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    return this->childWithParam(view, zfnull, atIndex);
}

void ZFUIAutoLayout::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable) {
    zfsuper::styleableOnCopyFrom(anotherStyleable);
    zfself *another = zfcast(zfself *, anotherStyleable);
    if(another == zfnull
            || this->childCount() != another->childCount()
            ) {
        return;
    }
    // copy all rules
    for(zfindex iChild = 0; iChild < this->childCount(); ++iChild) {
        ZFUIView *childRef = another->childAt(iChild);
        ZFUIAutoLayoutParam *lp = this->childAt(iChild)->layoutParam();
        ZFUIAutoLayoutParam *lpRef = childRef->layoutParam();
        for(zfindex iPos = 0; iPos < ZFUIAutoLayoutPos::EnumCount(); ++iPos) {
            ZFUIAutoLayoutPosEnum pos = (ZFUIAutoLayoutPosEnum)ZFUIAutoLayoutPos::EnumValueAt(iChild);
            ZFUIAutoLayoutRule &rule = lp->_ZFP_AL_d.ruleList[pos];
            ZFUIAutoLayoutRule &ruleRef = lpRef->_ZFP_AL_d.ruleList[pos];
            if(!ruleRef.valid()) {
                if(rule.valid()) {
                    rule.removeAll();
                }
            }
            else {
                rule.targetPos(ruleRef.targetPos());
                rule.offset(ruleRef.offset());
                _ZFP_ZFUIAutoLayout_targetUpdate(ruleRef, another, childRef);
                if(ruleRef.target() == lpRef->ownerParent()) {
                    rule.target(this);
                }
                else if(ruleRef.target() == lpRef->ownerChild()) {
                    rule.target(this->childAt(iChild));
                }
                else {
                    zfindex childIndex = another->childFind(ruleRef.target());
                    ZFCoreAssert(childIndex < another->childCount());
                    rule.target(this->childAt(childIndex));
                }
            }
        }
    }
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
            if(!_ZFP_ZFUIAutoLayout_targetUpdate(rule, this, child)) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "%s invalid target \"%s\" for %s",
                    this,
                    rule._ZFP_AL_targetId,
                    child);
                rule.target(zfnull);
                rule._ZFP_AL_targetId.removeAll();
                return zffalse;
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
        ) {
    if(!rule._ZFP_AL_targetId) {
        return zftrue;
    }
    zfstring &targetId = rule._ZFP_AL_targetId;
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
        else {
            rule.target(zfnull);
        }
    }

    if(rule.target()) {
        targetId = zfnull;
        return zftrue;
    }
    else {
        rule.target(parent);
        return zffalse;
    }
}
zfbool _ZFP_ZFUIAutoLayout_targetIdUpdate(
        ZF_OUT zfstring &targetId
        , ZF_IN ZFUIAutoLayoutRule &rule
        , ZF_IN ZFUIAutoLayout *parent
        , ZF_IN ZFUIView *child
        ) {
    if(rule._ZFP_AL_targetId) {
        targetId += rule._ZFP_AL_targetId;
        return zftrue;
    }
    ZFUIView *target = rule.target();
    if(target == zfnull || (target != parent && rule.target()->parent() != parent)) {
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

