#include "ZFUIDraw.h"
#include "protocol/ZFProtocolZFUIDraw.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIDrawableView)
ZFEVENT_REGISTER(ZFUIDrawableView, OnDraw)

ZFMETHOD_DEFINE_0(ZFUIDrawableView, void, drawRequest) {
    ZFPROTOCOL_ACCESS(ZFUIDrawForView)->drawRequest(this);
}

ZFMETHOD_DEFINE_0(ZFUIDrawableView, zfbool const &, drawing) {
    return this->_drawing;
}

void ZFUIDrawableView::objectOnInit(void) {
    zfsuper::objectOnInit();
    this->_drawing = zffalse;

    zfclassNotPOD NativeImplViewDestroy {
    public:
        static void action(
                ZF_IN zfanyT<ZFUIView> const &view
                , ZF_IN void *nativeImplView
                ) {
            ZFPROTOCOL_ACCESS(ZFUIDrawForView)->nativeDrawableViewDestroy(view, nativeImplView);
        }
    };
    zfbool nativeImplViewRequireVirtualIndex = zftrue;
    void *nativeImplView = ZFPROTOCOL_ACCESS(ZFUIDrawForView)->nativeDrawableViewCreate(this, nativeImplViewRequireVirtualIndex);
    this->nativeImplView(
        nativeImplView,
        NativeImplViewDestroy::action,
        nativeImplViewRequireVirtualIndex);
}

ZF_NAMESPACE_BEGIN(ZFUIDraw)

// ============================================================
// context
ZFMETHOD_FUNC_DEFINE_1(void *, beginForView
        , ZFMP_IN(ZFUIDrawableView *, view)
        ) {
    if(view == zfnull) {
        return zfnull;
    }
    ZFUIDrawToken *token = zfpoolNew(ZFUIDrawToken, ZFUIDrawToken::TypeView, view);
    if(!ZFPROTOCOL_ACCESS(ZFUIDrawForView)->beginForView(*token)) {
        zfpoolDelete(token);
        return zfnull;
    }
    return token;
}
ZFMETHOD_FUNC_DEFINE_1(void, endForView
        , ZFMP_IN(void *, context)
        ) {
    ZFUIDrawToken *token = (ZFUIDrawToken *)context;
    ZFCoreAssertWithMessage(token->type == ZFUIDrawToken::TypeView,
        "ZFUIDraw::endForView called with a mismatch context");
    ZFPROTOCOL_ACCESS(ZFUIDrawForView)->endForView(*token);
    zfpoolDelete(token);
}

ZFMETHOD_FUNC_DEFINE_1(void *, beginForImage
        , ZFMP_IN(const ZFUISize &, imageSize)
        ) {
    if(imageSize.width <= 0 || imageSize.height <= 0) {
        return zfnull;
    }
    ZFUISize imageSizePixel = ZFUISizeApplyScale(imageSize, ZFUIGlobalStyle::DefaultStyle()->imageScale());
    ZFUIDrawToken *token = zfpoolNew(ZFUIDrawToken, ZFUIDrawToken::TypeImage, ZFUIImage::ClassData()->newInstance());
    if(!ZFPROTOCOL_ACCESS(ZFUIDrawForImage)->beginForImage(*token, imageSizePixel)) {
        zfpoolDelete(token);
        return zfnull;
    }
    return token;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFUIImage>, endForImage
        , ZFMP_IN(void *, context)
        ) {
    ZFUIDrawToken *token = (ZFUIDrawToken *)context;
    ZFCoreAssertWithMessage(token->type == ZFUIDrawToken::TypeImage,
        "ZFUIDraw::endForImage called with a mismatch context");
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIDrawForImage)->endForImage(*token);
    zfautoT<ZFUIImage> ret = token->target;
    ret->nativeImage(nativeImage, zffalse);
    zfpoolDelete(token);
    if(nativeImage == zfnull) {
        return zfnull;
    }
    else {
        return ret;
    }
}

// ============================================================
// draw api
ZFMETHOD_FUNC_DEFINE_2(zfbool, drawClear
        , ZFMP_IN(void *, context)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        ) {
    return ZFPROTOCOL_ACCESS(ZFUIDraw)->drawClear(*(ZFUIDrawToken *)context
            , ZFUIRectApplyScale(targetFrame, ZFUIGlobalStyle::DefaultStyle()->imageScale())
            );
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, drawColor
        , ZFMP_IN(void *, context)
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        ) {
    return ZFPROTOCOL_ACCESS(ZFUIDraw)->drawColor(*(ZFUIDrawToken *)context
            , color
            , ZFUIRectApplyScale(targetFrame, ZFUIGlobalStyle::DefaultStyle()->imageScale())
            );
}

ZFMETHOD_FUNC_DEFINE_4(zfbool, drawImage
        , ZFMP_IN(void *, context)
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN_OPT(const ZFUIRect &, imageFrame, ZFUIRectZero())
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        ) {
    if(image == zfnull) {
        return zffalse;
    }
    zfautoT<ZFUIImage> imageState = image->imageStateUpdate();
    if(imageState == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFUIDraw)->drawImage(*(ZFUIDrawToken *)context
            , imageState
            , ZFUIRectApplyScale(imageFrame, imageState->imageScaleFixed())
            , ZFUIRectApplyScale(targetFrame, ZFUIGlobalStyle::DefaultStyle()->imageScale())
            );
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, drawText
        , ZFMP_IN(void *, context)
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUITextConfig *, config, zfnull)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        ) {
    return ZFPROTOCOL_ACCESS(ZFUIDraw)->drawText(*(ZFUIDrawToken *)context
            , text
            , config ? config : zfcast(ZFUITextConfig *, ZFUITextConfig::DefaultStyle())
            , ZFUIRectApplyScale(targetFrame, ZFUIGlobalStyle::DefaultStyle()->imageScale())
            );
}

ZF_NAMESPACE_END(ZFUIDraw)

ZF_NAMESPACE_GLOBAL_END

