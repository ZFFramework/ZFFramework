#include "ZFUINativeViewWrapper.h"
#include "protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUINativeViewWrapper)

ZFOBJECT_ON_INIT_DEFINE_1(ZFUINativeViewWrapper
        , ZFMP_IN(void *, nativeImplView)
        ) {
    this->objectOnInit();
    zfself::nativeImplView(nativeImplView, zfnull, zftrue);
}

void ZFUINativeViewWrapper::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    if(this->nativeImplView() != zfnull) {
        ret += " ";
        zfsFromPointerT(ret, this->nativeImplView());
    }
}

ZFMETHOD_DEFINE_2(ZFUINativeViewWrapper, void, measureNativeView
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

void ZFUINativeViewWrapper::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    this->measureNativeView(ret, sizeHint);
}

ZF_NAMESPACE_GLOBAL_END

