#include "ZFUIScrollLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScrollLayout
ZFOBJECT_REGISTER(ZFUIScrollLayout)
ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollLayout)

static void _ZFP_ZFUIScrollLayout_measure(
        ZF_OUT ZFUISize &ret
        , ZF_IN ZFUIScrollLayout *owner
        ) {
    ret = ZFUISizeZero();
    for(zfindex i = 0; i < owner->childCount(); ++i) {
        ZFUIView *child = owner->childAt(i);
        ZFUILayoutParam *lp = child->layoutParam();
        child->layoutMeasure(lp->sizeHint(), lp->sizeParam());
        ret.width = zfmMax<zffloat>(ret.width,
                child->layoutMeasuredSize().width + lp->margin().left + lp->margin().right
                );
        ret.height = zfmMax<zffloat>(ret.height,
                child->layoutMeasuredSize().height + lp->margin().top + lp->margin().bottom
                );
    }
}

void ZFUIScrollLayout::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    _ZFP_ZFUIScrollLayout_measure(ret, this);
}
void ZFUIScrollLayout::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
    zfsuper::layoutOnLayoutPrepare(bounds);
    ZFUISize ret = ZFUISizeZero();
    _ZFP_ZFUIScrollLayout_measure(ret, this);
    const ZFUIRect &orig = this->scrollContentFrame();
    this->scrollContentFrameUpdate(ZFUIRectCreate(
                orig.x
                , orig.y
                , ret.width
                , ret.height
                ));
}

ZF_NAMESPACE_GLOBAL_END

