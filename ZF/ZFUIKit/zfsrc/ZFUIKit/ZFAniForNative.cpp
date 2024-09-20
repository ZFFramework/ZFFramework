#include "ZFAniForNative.h"
#include "protocol/ZFProtocolZFAniForNative.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFAniForNativeCurve)

// ============================================================
zfclassNotPOD _ZFP_ZFAniForNativePrivate {
public:
    void *nativeAni;
    zfbool aniTargetAutoDisableFlag;

public:
    _ZFP_ZFAniForNativePrivate(void)
    : nativeAni(zfnull)
    , aniTargetAutoDisableFlag(zffalse)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForNative)

void ZFAniForNative::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForNativePrivate);
    d->nativeAni = ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniCreate(this);
}
void ZFAniForNative::objectOnDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniDestroy(this, d->nativeAni);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFAniForNative::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    ret += "-";
    zfsFromPointerT(ret, this->nativeAnimation());
    ZFClassUtil::objectPropertyInfo(ret, this);
}
ZFCompareResult ZFAniForNative::objectValueCompare(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
        && this->aniCurve() == another->aniCurve()
        && this->aniAlphaFrom() == another->aniAlphaFrom()
        && this->aniAlphaTo() == another->aniAlphaTo()
        && this->aniScaleXFrom() == another->aniScaleXFrom()
        && this->aniScaleXTo() == another->aniScaleXTo()
        && this->aniScaleYFrom() == another->aniScaleYFrom()
        && this->aniScaleYTo() == another->aniScaleYTo()
        && this->aniTranslateXFrom() == another->aniTranslateXFrom()
        && this->aniTranslateXTo() == another->aniTranslateXTo()
        && this->aniTranslateYFrom() == another->aniTranslateYFrom()
        && this->aniTranslateYTo() == another->aniTranslateYTo()
        && this->aniRotateXFrom() == another->aniRotateXFrom()
        && this->aniRotateXTo() == another->aniRotateXTo()
        && this->aniRotateYFrom() == another->aniRotateYFrom()
        && this->aniRotateYTo() == another->aniRotateYTo()
        && this->aniRotateZFrom() == another->aniRotateZFrom()
        && this->aniRotateZTo() == another->aniRotateZTo()
        ) {
        return ZFCompareEqual;
    }
    return ZFCompareUncomparable;
}

ZFMETHOD_DEFINE_0(ZFAniForNative, void *, nativeAnimation) {
    return d->nativeAni;
}

// ============================================================
// start stop
zfbool ZFAniForNative::aniImplCheckValid(void) {
    return (zfsuper::aniImplCheckValid()
        && this->aniTarget() != zfnull && this->aniTarget()->classData()->classIsTypeOf(ZFUIView::ClassData())
    );
}

void ZFAniForNative::aniOnStart(void) {
    zfsuper::aniOnStart();
    ZFUIView *aniTarget = zfany(this->aniTarget());
    if(aniTarget == zfnull || !this->aniTargetAutoDisable()) {
        d->aniTargetAutoDisableFlag = zffalse;
    }
    else {
        d->aniTargetAutoDisableFlag = zftrue;
        aniTarget->viewUIEnableTree(zffalse);
    }
}
void ZFAniForNative::aniOnStop(ZF_IN ZFResultTypeEnum resultType) {
    ZFUIView *aniTarget = zfany(this->aniTarget());
    if(aniTarget != zfnull && d->aniTargetAutoDisableFlag) {
        aniTarget->viewUIEnableTree(zftrue);
    }
    zfsuper::aniOnStop(resultType);
}

void ZFAniForNative::aniImplStart(void) {
    zfsuper::aniImplStart();
    ZFUIView *aniTarget = this->aniTarget();
    ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniStart(this,
        aniTarget != zfnull ? aniTarget->UIScaleFixed() : (zffloat)1);
    zfanyT<ZFUIView> parent = aniTarget;
    while(parent->viewParent() != zfnull) {
        parent = parent->viewParent();
    }
    parent->layoutIfNeed();
}
void ZFAniForNative::aniImplStop(void) {
    ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniStop(this);
    zfsuper::aniImplStop();
}

ZF_NAMESPACE_GLOBAL_END

