#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFUIAutoLayout_posAttach(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN ZFUIAutoLayoutPosEnum pos
        ) {
    ZFCoreAssertWithMessageTrim(lp->ownerChild() != zfnull, "[ZFUIAutoLayout] must add to parent before changing pos rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = lp->_ZFP_AL_d;
    if(d.posReset) {
        d.posReset = zffalse;
        zfmemset(d.posAttached, 0, sizeof(d.posAttached));
    }
    d.posAttached[pos] = zftrue;
}

static void _ZFP_ZFUIAutoLayout_targetAttach(
        ZF_IN ZFUIAutoLayoutParam *lp
        , ZF_IN ZFUIView *target
        , ZF_IN ZFUIAutoLayoutPosEnum targetPos
        ) {
    ZFCoreAssertWithMessageTrim(lp->ownerChild() != zfnull, "[ZFUIAutoLayout] must add to parent before changing target rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = lp->_ZFP_AL_d;
    ZFUIAutoLayout *parent = lp->ownerParent();
    if(target == zfnull) {
        target = parent;
    }
    zfbool posAttached = zffalse;
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        if(d.posAttached[i]) {
            posAttached = zftrue;

            ZFUIAutoLayoutRule &rule = d.ruleList[i];
            rule.target(target);
            rule.targetPos(targetPos == ZFUIAutoLayoutPos::e_None ? (ZFUIAutoLayoutPosEnum)i : targetPos);
        }
    }
    ZFCoreAssertWithMessageTrim(posAttached, "[ZFUIAutoLayout] pos rule (width/left/...) not set");
    d.posReset = zftrue;
    parent->layoutRequest();
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, const ZFUIAutoLayoutRule &, rule
        , ZFMP_IN(ZFUIAutoLayoutPosEnum, pos)
        ) {
    return _ZFP_AL_d.ruleList[pos];
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, ruleRemove
        , ZFMP_IN(ZFUIAutoLayoutPosEnum, pos)
        ) {
    ZFUIAutoLayout *parent = this->ownerParent();
    ZFUIAutoLayoutRule &rule = _ZFP_AL_d.ruleList[pos];
    if(rule.valid()) {
        if(parent != zfnull) {
            parent->layoutRequest();
        }
        rule.removeAll();
    }
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, ruleRemoveAll) {
    ZFUIAutoLayout *parent = this->ownerParent();
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
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
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, height) {
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, left) {
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, top) {
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, right) {
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, bottom) {
    _ZFP_ZFUIAutoLayout_posAttach(this, ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toWidth
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toHeight
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toLeft
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toTop
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toRight
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, toBottom
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentWidth) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Width);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentHeight) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Height);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentLeft) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Left);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentTop) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Top);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentRight) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Right);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParentBottom) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_Bottom);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, to
        , ZFMP_IN(ZFUIView *, target)
        ) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, target, ZFUIAutoLayoutPos::e_None);
}
ZFMETHOD_DEFINE_0(ZFUIAutoLayoutParam, void, toParent) {
    _ZFP_ZFUIAutoLayout_targetAttach(this, zfnull, ZFUIAutoLayoutPos::e_None);
}

ZFMETHOD_DEFINE_1(ZFUIAutoLayoutParam, void, offset
        , ZFMP_IN(zffloat, offset)
        ) {
    ZFCoreAssertWithMessageTrim(this->ownerChild() != zfnull, "[ZFUIAutoLayout] must add to parent before changing offset rule");
    ZFUIAutoLayoutParam::_ZFP_Data &d = _ZFP_AL_d;
    zfbool posAttached = zffalse;
    for(zfindex i = ZFUIAutoLayoutPos::e_None + 1; i < ZFUIAutoLayoutPos::ZFEnumCount; ++i) {
        if(d.posAttached[i]) {
            posAttached = zftrue;

            ZFUIAutoLayoutRule &rule = d.ruleList[i];
            rule.offset(offset);
        }
    }
    ZFCoreAssertWithMessageTrim(posAttached, "[ZFUIAutoLayout] pos rule (width/left/...) not set");
    d.posReset = zftrue;
    this->ownerParent()->layoutRequest();
}

ZF_NAMESPACE_GLOBAL_END

