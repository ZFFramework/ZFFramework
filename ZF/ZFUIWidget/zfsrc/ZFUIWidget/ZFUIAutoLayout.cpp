#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// misc
ZFENUM_DEFINE(ZFUIAutoLayoutPos)
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, zfanyT<ZFUIView>, target)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIAutoLayoutRule, ZFUIAutoLayoutPos, targetPos)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutRule, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutRule, void, removeAll)

void ZFUIAutoLayoutRule::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFUIAutoLayoutRule target:%s targetPos:%s>"
            , this->target()
            , this->targetPos()
            );
}

// ============================================================
// ZFUIAutoLayoutParam
ZFOBJECT_REGISTER(ZFUIAutoLayoutParam)

/* ZFTAG_TRICKS: util for chained call to build view tree */
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIAutoLayoutParam, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIAutoLayoutParam, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIAutoLayout, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFUIAutoLayout, zfanyT<ZFUIAutoLayoutParam>, child
        , ZFMP_IN(ZFUILayoutParam *, layoutParam)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        )

static void _ZFP_ZFUIAutoLayoutAlignApply(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN const ZFUIAlignFlags &align
        ) {
    if(ZFBitTest(align, v_ZFUIAlign::e_Left)) {
        lp->c_left()->c_toParentLeft()->c_biasX(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Right);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_Right)) {
        lp->c_right()->c_toParentRight()->c_biasX(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Left);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_LeftEdge)) {
        lp->c_right()->c_toParentLeft()->c_biasX(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Left);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_RightEdge)) {
        lp->c_left()->c_toParentRight()->c_biasX(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Right);
    }
    else {
        lp
            ->c_left()->c_toParentLeft()
            ->c_right()->c_toParentRight()
            ->c_biasX(0.5f)
            ;
    }

    if(ZFBitTest(align, v_ZFUIAlign::e_Top)) {
        lp->c_top()->c_toParentTop()->c_biasY(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Bottom);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_Bottom)) {
        lp->c_bottom()->c_toParentBottom()->c_biasY(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Top);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_TopEdge)) {
        lp->c_bottom()->c_toParentTop()->c_biasY(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Top);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_BottomEdge)) {
        lp->c_top()->c_toParentBottom()->c_biasY(0.5f);
        lp->ruleRemove(v_ZFUIAutoLayoutPos::e_Bottom);
    }
    else {
        lp
            ->c_top()->c_toParentTop()
            ->c_bottom()->c_toParentBottom()
            ->c_biasY(0.5f)
            ;
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIAutoLayoutParam, ZFUIAlignFlags, align) {
    _ZFP_ZFUIAutoLayoutAlignApply(this, propertyValue);
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

// all printable chars (0x20 ~ 0x7E) except:
//   '%' : 0x25
//   ':' : 0x3A
//   '|' : 0x7C
#define _ZFP_ZFUIAutoLayoutRuleEscapeCharMap() const zfchar charMap[256] = { \
        /* 0x00 ~ 0x0F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x10 ~ 0x1F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x20 ~ 0x2F */ \
        1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x30 ~ 0x3F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, \
        /* 0x40 ~ 0x4F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x50 ~ 0x5F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x60 ~ 0x6F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x70 ~ 0x7F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, \
        /* 0x80 ~ 0x8F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x90 ~ 0x9F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xA0 ~ 0xAF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xB0 ~ 0xBF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xC0 ~ 0xCF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xD0 ~ 0xDF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xE0 ~ 0xEF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xF0 ~ 0xFF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    }

zfbool ZFUIAutoLayoutParam::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
    // remove all rule
    for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        _ZFP_AL_d.ruleList[i].removeAll();
    }

    zfstring ruleListStr = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFUIAutoLayoutParam_rule);
    ZFCoreArray<zfstring> ruleList = zfstringSplit(ruleListStr, "|");
    for(zfindex iRule = 0; iRule < ruleList.count(); ++iRule) {
        const zfstring &ruleStr = ruleList[iRule];
        ZFCoreArray<ZFIndexRange> itemPos = zfstringSplitIndex(ruleStr, ":");
        if(!(itemPos.count() >= 3 && itemPos.count() <= 4)) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData
                , "invalid rule: \"%s\""
                , ruleStr
                );
            return zffalse;
        }

        ZFUIAutoLayoutPos pos = v_ZFUIAutoLayoutPos::e_None;
        if(!ZFUIAutoLayoutPosFromStringT(pos, ruleStr + itemPos[0].start, itemPos[0].count)
                || pos == v_ZFUIAutoLayoutPos::e_None
                ) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData
                , "invalid pos rule \"%s\", declared in rule \"%s\""
                , zfstring(ruleStr + itemPos[0].start, itemPos[0].count)
                , ruleStr
                );
            return zffalse;
        }

        ZFUIAutoLayoutPos targetPos = v_ZFUIAutoLayoutPos::e_None;
        if(!ZFUIAutoLayoutPosFromStringT(targetPos, ruleStr + itemPos[1].start, itemPos[1].count)
                || pos == v_ZFUIAutoLayoutPos::e_None
                ) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData
                , "invalid targetPos rule \"%s\", declared in rule \"%s\""
                , zfstring(ruleStr + itemPos[1].start, itemPos[1].count)
                , ruleStr
                );
            return zffalse;
        }

        zfstring target;
        ZFCoreDataDecode(target, ruleStr + itemPos[2].start, itemPos[2].count);
        if(ruleStr[itemPos[2].start] == '@') {
            if(zftrue
                    && target != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent
                    && target != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self
                    ) {
                zfindex refIndex = zfindexMax();
                if(!zfindexFromStringT(refIndex, target + 1)) {
                    ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData
                            , "invalid target rule \"%s\", declared in rule \"%s\""
                            , target
                            , ruleStr
                            );
                    return zffalse;
                }
            }
        }

        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];
        rule.targetPos(targetPos);
        rule._ZFP_AL_targetId = target;
    }
    return zftrue;
}
zfbool ZFUIAutoLayoutParam::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);
    _ZFP_ZFUIAutoLayoutRuleEscapeCharMap();

    zfstring ruleStr;
    for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[i];
        if(ref != zfnull && ref->_ZFP_AL_d.ruleList[i] == rule) {
            continue;
        }
        if(!rule.valid()) {
            continue;
        }

        if(rule.targetPos() == v_ZFUIAutoLayoutPos::e_None) {
            ZFSerializableUtilErrorOccurred(outErrorHint
                    , "invalid targetPos"
                    );
            return zffalse;
        }
        _ZFP_ZFUIAutoLayout_targetIdUpdate(rule._ZFP_AL_targetId, rule, zfcast(ZFUIAutoLayout *, this->ownerParent()), this->owner());
        if(!rule._ZFP_AL_targetId) {
            ZFSerializableUtilErrorOccurred(outErrorHint
                    , "no target specified"
                    );
            return zffalse;
        }

        if(ruleStr) {
            ruleStr += "|";
        }
        ZFUIAutoLayoutPosToStringT(ruleStr, (ZFUIAutoLayoutPos)i);
        ruleStr += ":";
        ZFUIAutoLayoutPosToStringT(ruleStr, rule.targetPos());
        ruleStr += ":";
        ZFCoreDataEncode(ruleStr, rule._ZFP_AL_targetId, rule._ZFP_AL_targetId.length(), charMap);
    }
    if(ruleStr) {
        serializableData.attr(ZFSerializableKeyword_ZFUIAutoLayoutParam_rule, ruleStr);
    }
    return zftrue;
}

void ZFUIAutoLayoutParam::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    zfsuper::styleableOnCopyFrom(anotherStyleable);
    // copy ZFUIAutoLayoutParam does not copy rules,
    // it's done by ZFUIAutoLayout::styleableOnCopyFrom
}
ZFCompareResult ZFUIAutoLayoutParam::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}
    if(zfsuper::objectCompareValueImpl(anotherObj) != ZFCompareEqual) {
        return ZFCompareUncomparable;
    }
    for(zfindex i = 0; i < v_ZFUIAutoLayoutPos::EnumCount(); ++i) {
        ZFUIAutoLayoutPos pos = (ZFUIAutoLayoutPos)v_ZFUIAutoLayoutPos::EnumValueAt(i);
        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];
        ZFUIAutoLayoutRule &ruleRef = another->_ZFP_AL_d.ruleList[pos];
        if(rule.valid() != ruleRef.valid()
                || rule.targetPos() != ruleRef.targetPos()
                ) {
            return ZFCompareUncomparable;
        }
        zfstring targetId;
        zfstring targetIdRef;
        _ZFP_ZFUIAutoLayout_targetIdUpdate(targetId, rule, zfcast(ZFUIAutoLayout *, this->ownerParent()), this->owner());
        _ZFP_ZFUIAutoLayout_targetIdUpdate(targetIdRef, ruleRef, zfcast(ZFUIAutoLayout *, another->ownerParent()), another->owner());
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

void ZFUIAutoLayout::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
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
        for(zfindex iPos = 0; iPos < v_ZFUIAutoLayoutPos::EnumCount(); ++iPos) {
            ZFUIAutoLayoutPos pos = (ZFUIAutoLayoutPos)v_ZFUIAutoLayoutPos::EnumValueAt(iChild);
            ZFUIAutoLayoutRule &rule = lp->_ZFP_AL_d.ruleList[pos];
            ZFUIAutoLayoutRule &ruleRef = lpRef->_ZFP_AL_d.ruleList[pos];
            if(!ruleRef.valid()) {
                if(rule.valid()) {
                    rule.removeAll();
                }
            }
            else {
                rule.targetPos(ruleRef.targetPos());
                _ZFP_ZFUIAutoLayout_targetUpdate(ruleRef, another, childRef);
                if(ruleRef.target() == lpRef->ownerParent()) {
                    rule.target(this);
                }
                else if(ruleRef.target() == lpRef->owner()) {
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
        for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
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
        if(targetId == ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent) {
            rule.target(parent);
        }
        else if(targetId == ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self) {
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
    }
    else {
        rule.target(parent->childFindById(targetId, ZF_HINT("findRecursively")zffalse));
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
        if(&targetId != &(rule._ZFP_AL_targetId)) {
            targetId += rule._ZFP_AL_targetId;
        }
        return zftrue;
    }
    if(&targetId == &(rule._ZFP_AL_targetId)) {
        targetId.removeAll();
    }

    ZFUIView *target = rule.target();
    if(target == zfnull || (target != parent && rule.target()->parent() != parent)) {
        return zffalse;
    }
    if(target == parent) {
        targetId += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent;
    }
    else if(target == child) {
        targetId += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self;
    }
    else if(!target->viewId().isEmpty()) {
        targetId += target->viewId();
    }
    else {
        zfindex childIndexRef = parent->childFind(target);
        if(childIndexRef == zfindexMax()) {
            return zffalse;
        }
        else {
            targetId += '@';
            zfindexToStringT(targetId, childIndexRef);
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

