#include "ZFAniForNative.h"
#include "protocol/ZFProtocolZFAniForNative.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFAniForNativeCurve)

// ============================================================
zfclassNotPOD _ZFP_ZFAniForNativePrivate {
public:
    void *nativeAni;
    zfbool targetAutoDisableFlag;

public:
    _ZFP_ZFAniForNativePrivate(void)
    : nativeAni(zfnull)
    , targetAutoDisableFlag(zffalse)
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

void ZFAniForNative::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += "-";
    zfsFromPointerT(ret, this->nativeAnimation());
    ZFObjectPropertyInfoT(ret, this);
}
ZFCompareResult ZFAniForNative::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
        && this->nativeCurve() == another->nativeCurve()
        && this->alphaFrom() == another->alphaFrom()
        && this->alphaTo() == another->alphaTo()
        && this->scaleXFrom() == another->scaleXFrom()
        && this->scaleXTo() == another->scaleXTo()
        && this->scaleYFrom() == another->scaleYFrom()
        && this->scaleYTo() == another->scaleYTo()
        && this->translateXFrom() == another->translateXFrom()
        && this->translateXTo() == another->translateXTo()
        && this->translateYFrom() == another->translateYFrom()
        && this->translateYTo() == another->translateYTo()
        && this->rotateXFrom() == another->rotateXFrom()
        && this->rotateXTo() == another->rotateXTo()
        && this->rotateYFrom() == another->rotateYFrom()
        && this->rotateYTo() == another->rotateYTo()
        && this->rotateZFrom() == another->rotateZFrom()
        && this->rotateZTo() == another->rotateZTo()
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
        && this->target() != zfnull && this->target()->classData()->classIsTypeOf(ZFUIView::ClassData())
    );
}

void ZFAniForNative::aniOnStart(void) {
    zfsuper::aniOnStart();
    ZFUIView *target = zfany(this->target());
    if(target == zfnull || !this->targetAutoDisable()) {
        d->targetAutoDisableFlag = zffalse;
    }
    else {
        d->targetAutoDisableFlag = zftrue;
        target->viewUIEnableTree(zffalse);
    }
}
void ZFAniForNative::aniOnStop(ZF_IN ZFResultType resultType) {
    ZFUIView *target = zfany(this->target());
    if(target != zfnull && d->targetAutoDisableFlag) {
        target->viewUIEnableTree(zftrue);
    }
    zfsuper::aniOnStop(resultType);
}

void ZFAniForNative::aniImplStart(void) {
    zfsuper::aniImplStart();
    ZFUIView *target = this->target();
    ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniStart(this,
        target != zfnull ? target->UIScaleFixed() : (zffloat)1);
    zfanyT<ZFUIView> parent = target;
    while(parent->parent() != zfnull) {
        parent = parent->parent();
    }
    parent->layoutIfNeed();
}
void ZFAniForNative::aniImplStop(void) {
    ZFPROTOCOL_ACCESS(ZFAniForNative)->nativeAniStop(this);
    zfsuper::aniImplStop();
}

ZF_NAMESPACE_GLOBAL_END

