#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIViewLayout)

void ZFUIViewLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ZFUILayoutParam::sizeHintApplyT(ret, this->viewSizeMin(), sizeHint, sizeParam);
    for(zfindex i = 0; i < this->childCount(); ++i) {
        ZFUIView *child = this->childAt(i);
        child->layoutMeasure(sizeHint, child->layoutParam()->sizeParam());
        ret.width = zfmMax<zffloat>(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->margin()));
        ret.height = zfmMax<zffloat>(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetHeight(child->layoutParam()->margin()));
    }
}

ZF_NAMESPACE_GLOBAL_END

