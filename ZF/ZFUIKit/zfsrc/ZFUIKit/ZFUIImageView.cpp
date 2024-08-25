#include "ZFUIImageView.h"
#include "protocol/ZFProtocolZFUIView.h"
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
        if(!this->imageUpdateListener) {
            ZFUIImageView *owner = this;
            ZFLISTENER_1(imageOnUpdate
                    , ZFUIImageView *, owner
                    ) {
                if(owner->image() != zfnull) {
                    ZFPROTOCOL_ACCESS(ZFUIImageView)->image(owner, owner->image()->imageState(owner));
                }
                else {
                    ZFPROTOCOL_ACCESS(ZFUIImageView)->image(owner, zfnull);
                }
            } ZFLISTENER_END()
            this->imageUpdateListener = imageOnUpdate;
        }
        this->image()->observerAdd(ZFUIImage::EventImageStateOnUpdate(), this->imageUpdateListener);
        ZFPROTOCOL_ACCESS(ZFUIImageView)->image(this, this->image()->imageState(this));
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUIImageView)->image(this, zfnull);
    }
    this->layoutRequest();
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIImageView, zfanyT<ZFUIImage>, image) {
    if(this->image() != zfnull) {
        if(this->imageUpdateListener) {
            this->image()->observerRemove(ZFUIImage::EventImageStateOnUpdate(), this->imageUpdateListener);
        }
    }
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
    ret = ((this->image() != zfnull) ? this->image()->imageSize() : ZFUISizeZero());
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

