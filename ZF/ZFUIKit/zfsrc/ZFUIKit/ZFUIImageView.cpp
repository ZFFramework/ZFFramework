#include "ZFUIImageView.h"
#include "protocol/ZFProtocolZFUIImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageView
ZFOBJECT_REGISTER(ZFUIImageView)
ZFSTYLE_DEFAULT_DEFINE(ZFUIImageView)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIImageView, zfbool, viewUIEnable) {
    propertyValue = zffalse;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIImageView, zfbool, viewUIEnableTree) {
    propertyValue = zffalse;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, zfanyT<ZFUIImage>, image) {
    if(this->image() != zfnull) {
        if(!this->imageStateOnUpdateListener) {
            ZFUIImageView *owner = this;
            ZFUISize sizeSaved = ZFUISizeZero();
            ZFLISTENER_2(imageStateOnUpdate
                    , ZFUIImageView *, owner
                    , ZFUISize, sizeSaved
                    ) {
                zfautoT<ZFUIImage> imageNew = owner->image() ? owner->image()->imageState() : zfnull;
                ZFUISize sizeNew = imageNew ? imageNew->imageSize() : ZFUISizeZero();
                ZFPROTOCOL_ACCESS(ZFUIImageView)->image(owner, imageNew);
                ZFUILayoutParam *lp = owner->layoutParam();
                if(lp && (zffalse
                            || (lp->sizeParam().width == ZFUISizeType::e_Wrap && sizeNew.width != sizeSaved.width)
                            || (lp->sizeParam().height == ZFUISizeType::e_Wrap && sizeNew.height != sizeSaved.height)
                            )) {
                    owner->layoutRequest();
                }
            } ZFLISTENER_END()
            this->imageStateOnUpdateListener = imageStateOnUpdate;
        }
        this->image()->imageStateAttach(this->imageStateOnUpdateListener);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIImageView)->image(this, zfnull);
    }
    this->layoutRequest();
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIImageView, zfanyT<ZFUIImage>, image) {
    if(this->image() != zfnull && this->imageStateOnUpdateListener) {
        this->image()->imageStateDetach(this->imageStateOnUpdateListener);
    }
}
ZFMETHOD_DEFINE_0(ZFUIImageView, zfautoT<ZFUIImage>, imageState) {
    return this->image() ? this->image()->imageState() : zfnull;
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIContentScaleTypeEnum, imageScaleType) {
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIMargin, imageMargin) {
    if(propertyValue != propertyValueOld) {
        this->nativeImplViewMarginUpdate();
    }
}

ZFMETHOD_DEFINE_1(ZFUIImageView, void, measureImageView
        , ZFMP_OUT(ZFUISize &, ret)
        ) {
    zfautoT<ZFUIImage> imageState = (this->image() ? this->image()->imageState() : zfnull);
    ret = (imageState ? imageState->imageSize() : ZFUISizeZero());
    ZFUISizeApplyMarginReversely(ret, ret, this->imageMargin());
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIImageView
        , ZFMP_IN(ZFUIImage *, image)
        ) {
    this->objectOnInit();
    this->image(image);
}
void ZFUIImageView::objectOnInit(void) {
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD NativeImplViewDestroy {
    public:
        static void action(
                ZF_IN zfanyT<ZFUIView> const &view
                , ZF_IN void *nativeImplView
                ) {
            ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewDestroy(view, nativeImplView);
        }
    };
    zfbool nativeImplViewRequireVirtualIndex = zftrue;
    void *nativeImplView = ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewCreate(this, nativeImplViewRequireVirtualIndex);
    this->nativeImplView(
        nativeImplView,
        NativeImplViewDestroy::action,
        nativeImplViewRequireVirtualIndex);
}
void ZFUIImageView::objectOnDealloc(void) {
    this->image(zfnull);
    zfsuper::objectOnDealloc();
}

void ZFUIImageView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    if(this->image() != zfnull) {
        ret += " ";
        this->image()->objectInfoT(ret);
    }
}

void ZFUIImageView::nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin) {
    zfsuper::nativeImplViewMarginImplUpdate(nativeImplViewMargin);
    ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, this->imageMargin());
}
void ZFUIImageView::nativeImplViewOnLayout(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const ZFUIRect &bounds
        , ZF_IN const ZFUIMargin &nativeImplViewMargin
        ) {
    ZFUIContentScaleTypeApply(
        ret,
        this->imageScaleType(),
        ZFUIRectApplyMargin(bounds, nativeImplViewMargin),
        this->image() ? this->image()->imageSize() : ZFUISizeZero());
}
void ZFUIImageView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    this->measureImageView(ret);
}

ZF_NAMESPACE_GLOBAL_END

