#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFUIAutoLayout_posAttach(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN ZFUIAutoLayoutPos pos
        ) {
    ZFCoreAssertWithMessageTrim(lp->owner() != zfnull, "[ZFUIAutoLayout] must add to parent before changing pos rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = lp->_ZFP_AL_d;
    if(d.posReset) {
        d.posReset = zffalse;
        zfmemset(d.posAttached, 0, sizeof(d.posAttached));
    }
    d.posAttached[pos] = zftrue;
}

static zfbool _ZFP_ZFUIAutoLayout_posValid(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN ZFUIAutoLayoutPos pos
        , ZF_IN ZFUIAutoLayoutPos targetPos
        ) {
    switch(pos) {
        case v_ZFUIAutoLayoutPos::e_Left:
        case v_ZFUIAutoLayoutPos::e_Right:
            switch(targetPos) {
                case v_ZFUIAutoLayoutPos::e_Left:
                case v_ZFUIAutoLayoutPos::e_Right:
                    return zftrue;
                default:
                    break;
            }
            break;
        case v_ZFUIAutoLayoutPos::e_Top:
        case v_ZFUIAutoLayoutPos::e_Bottom:
            switch(targetPos) {
                case v_ZFUIAutoLayoutPos::e_Top:
                case v_ZFUIAutoLayoutPos::e_Bottom:
                    return zftrue;
                default:
                    break;
            }
            break;
        case v_ZFUIAutoLayoutPos::e_Width:
        case v_ZFUIAutoLayoutPos::e_Height:
            switch(targetPos) {
                case v_ZFUIAutoLayoutPos::e_Width:
                case v_ZFUIAutoLayoutPos::e_Height:
                    return zftrue;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    ZFCoreCriticalMessageTrim("[ZFUIAutoLayout] invalid pos rule \"%s => %s\" for view: %s"
            , pos
            , targetPos
            , lp->owner()
            );
    return zffalse;
}

static void _ZFP_ZFUIAutoLayout_targetAttach(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN const zfstring &viewId
        , ZF_IN ZFUIAutoLayoutPos targetPos
        ) {
    ZFCoreAssertWithMessageTrim(lp->owner() != zfnull, "[ZFUIAutoLayout] must add to parent before changing target rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = lp->_ZFP_AL_d;
    ZFUIAutoLayout *parent = zfcast(ZFUIAutoLayout *, lp->ownerParent());
    zfbool posAttached = zffalse;
    for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        if(d.posAttached[i] && _ZFP_ZFUIAutoLayout_posValid(
                    lp
                    , (ZFUIAutoLayoutPos)i
                    , targetPos == v_ZFUIAutoLayoutPos::e_None ? (ZFUIAutoLayoutPos)i : targetPos
                    )) {
            posAttached = zftrue;

            ZFUIAutoLayoutRule &rule = d.ruleList[i];
            if(!viewId) {
                rule._ZFP_AL_targetId = ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent;
            }
            else {
                rule._ZFP_AL_targetId = viewId;
            }
            rule.target(zfnull);
            rule.targetPos(targetPos == v_ZFUIAutoLayoutPos::e_None ? (ZFUIAutoLayoutPos)i : targetPos);
        }
    }
    ZFCoreAssertWithMessageTrim(posAttached, "[ZFUIAutoLayout] pos rule (width/left/...) not set");
    d.posReset = zftrue;
    parent->layoutRequest();
}
static void _ZFP_ZFUIAutoLayout_targetAttach(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN ZFUIView *target
        , ZF_IN ZFUIAutoLayoutPos targetPos
        ) {
    ZFCoreAssertWithMessageTrim(lp->owner() != zfnull, "[ZFUIAutoLayout] must add to parent before changing target rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = lp->_ZFP_AL_d;
    ZFUIAutoLayout *parent = zfcast(ZFUIAutoLayout *, lp->ownerParent());
    if(target == zfnull) {
        target = parent;
    }
    zfbool posAttached = zffalse;
    for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        if(d.posAttached[i] && _ZFP_ZFUIAutoLayout_posValid(
                    lp
                    , (ZFUIAutoLayoutPos)i
                    , targetPos == v_ZFUIAutoLayoutPos::e_None ? (ZFUIAutoLayoutPos)i : targetPos
                    )) {
            posAttached = zftrue;

            ZFUIAutoLayoutRule &rule = d.ruleList[i];
            rule.target(target);
            rule.targetPos(targetPos == v_ZFUIAutoLayoutPos::e_None ? (ZFUIAutoLayoutPos)i : targetPos);
        }
    }
    ZFCoreAssertWithMessageTrim(posAttached, "[ZFUIAutoLayout] pos rule (width/left/...) not set");
    d.posReset = zftrue;
    parent->layoutRequest();
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, const ZFUIAutoLayoutRule &, rule
        , ZFMP_IN(ZFUIAutoLayoutPos, pos)
        ) {
    return _ZFP_AL_d.ruleList[pos];
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, ruleRemove
        , ZFMP_IN(ZFUIAutoLayoutPos, pos)
        ) {
    ZFUIAutoLayout *parent = zfcast(ZFUIAutoLayout *, this->ownerParent());
    ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];
    if(rule.valid()) {
        if(parent != zfnull) {
            parent->layoutRequest();
        }
        rule.removeAll();
    }
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, ruleRemoveAll) {
    ZFUIAutoLayout *parent = zfcast(ZFUIAutoLayout *, this->ownerParent());
    for(zfindex i = v_ZFUIAutoLayoutPos::e_None + 1; i < v_ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[i];
        if(rule.valid()) {
            if(parent != zfnull) {
                parent->layoutRequest();
            }
            rule.removeAll();
        }
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, width) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, height) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, left) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, top) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, right) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, bottom) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, edges) {
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Left);
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Top);
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Right);
    _ZFP_ZFUIAutoLayout_posAttach(this, v_ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toWidth
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toHeight
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toLeft
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toTop
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toRight
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toBottom
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toWidth
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toHeight
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toLeft
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toTop
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toRight
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toBottom
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentWidth) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentHeight) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentLeft) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentTop) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentRight) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentBottom) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, to
        , ZFMP_IN(const zfstring &, viewId)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, viewId, v_ZFUIAutoLayoutPos::e_None);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, to
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, v_ZFUIAutoLayoutPos::e_None);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParent) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, (ZFUIView *)zfnull, v_ZFUIAutoLayoutPos::e_None);
}

ZF_NAMESPACE_GLOBAL_END

