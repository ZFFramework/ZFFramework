#include "ZFUINativeView.h"
#include "protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUINativeView)

ZFOBJECT_ON_INIT_DEFINE_1(ZFUINativeView
        , ZFMP_IN(void *, nativeImplView)
        ) {
    zfself::nativeImplView(nativeImplView, zfnull);
}

void ZFUINativeView::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    if(this->nativeImplView() != zfnull) {
        ret += " ";
        zfsFromPointerT(ret, this->nativeImplView());
    }
}

ZFMETHOD_DEFINE_2(ZFUINativeView, void, measureNativeView
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeInvalid())
        ) {
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFPROTOCOL_ACCESS(ZFUIView)->measureNativeView(
        ret,
        this->nativeImplView(),
        ZFUISizeApplyScale(
            ZFUILayoutParam::sizeHintOffset(sizeHint, ZFUISizeCreate(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->UIScaleFixed()));
    ZFUISizeApplyScaleReverselyT(ret, ret, this->UIScaleFixed());
    ZFUISizeApplyMarginReverselyT(ret, ret, nativeImplViewMargin);
}

void ZFUINativeView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    this->measureNativeView(ret, sizeHint);
}

ZF_NAMESPACE_GLOBAL_END

