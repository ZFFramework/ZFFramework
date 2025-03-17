#include "ZFUIViewBlink.h"
#include "ZFUIImageView.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkOn)
ZFEVENT_GLOBAL_REGISTER(ViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// #define _ZFP_ZFUIViewBlink_DEBUG_noAni 1
// #define _ZFP_ZFUIViewBlink_DEBUG_duration 1

#define _ZFP_ZFUIViewBlink_tag_ani "_ZFP_ZFUIViewBlink_tag_ani"
#define _ZFP_ZFUIViewBlink_tag_blinkView "_ZFP_ZFUIViewBlink_tag_blinkView"
#define _ZFP_ZFUIViewBlink_tag_delayTaskId "_ZFP_ZFUIViewBlink_tag_delayTaskId"
#define _ZFP_ZFUIViewBlink_tag_blinkCountLeft "_ZFP_ZFUIViewBlink_tag_blinkCountLeft"

zfclass _ZFP_ZFUIViewBlinkView : zfextend ZFUIImageView {
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewBlinkView, ZFUIImageView)
};

static void _ZFP_ZFUIViewBlinkDoOn(
        ZF_IN ZFUIView *view
        , ZF_IN const ZFUIViewBlinkParam &blinkParam
        );
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkDataHolder, ZFLevelZFFrameworkEssential) {
    ZFLISTENER(viewOnDealloc) {
        _ZFP_ZFUIViewBlinkDoOff(zfargs.sender());
    } ZFLISTENER_END()
    this->viewOnDeallocListener = viewOnDealloc;
}
public:
    ZFListener viewOnDeallocListener;
    ZFCoreArray<ZFUIView *> blinkingViews;
    ZFIdentityGenerator delayTaskIdGenerator;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkDataHolder)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIViewBlinkParam, ZFUIViewBlinkParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, zfautoT<ZFUIImage>, blinkImage)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, zftimet, blinkDuration)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, zfindex, blinkCount)

void ZFUIViewBlinkParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFUIViewBlinkParam blinkImage:%s blinkDuration:%s blinkCount:%s>"
            , this->blinkImage()
            , this->blinkDuration()
            , this->blinkCount()
            );
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewBlink
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewBlinkParam &, blinkParam, ZFUIViewBlinkParam())
        ) {
    if(blinkParam.blinkCount() <= 0) {
        return;
    }
    if(view) {
        _ZFP_ZFUIViewBlinkDoOn(view, blinkParam);
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_4(void, ZFUIViewBlink
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIImage *, blinkImage)
        , ZFMP_IN_OPT(zftimet, blinkDuration, 0)
        , ZFMP_IN_OPT(zfindex, blinkCount, 1)
        )

// ============================================================
// blink action
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view);
static void _ZFP_ZFUIViewBlinkDoOn(
        ZF_IN ZFUIView *view
        , ZF_IN const ZFUIViewBlinkParam &blinkParam
        ) {
    _ZFP_ZFUIViewBlinkView *blinkView = view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView != zfnull) {
        return;
    }

    {
        zfobj<_ZFP_ZFUIViewBlinkView> t;
        view->internalFgViewAdd(t);
        view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkView, t);
        t->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        blinkView = t;
    }
    blinkView->image((blinkParam.blinkImage() != zfnull)
        ? blinkParam.blinkImage()
        : ZFUIViewBlinkImageDefault());

#if _ZFP_ZFUIViewBlink_DEBUG_noAni
    view->observerAdd(ZFObject::E_ObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

    ZFGlobalObserver().observerNotifyWithSender(view, ZFGlobalEvent::E_ViewBlinkOn());
    zfidentity delayTaskId = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idAcquire();
    zfobj<v_zfidentity> delayTaskIdTag(delayTaskId);
    view->objectTag(_ZFP_ZFUIViewBlink_tag_delayTaskId, delayTaskIdTag);

    ZFLISTENER_2(blinkDelayOnFinish
            , zfauto, delayTaskIdTag
            , zfautoT<ZFUIView>, view
            ) {
        v_zfidentity *delayTaskId = delayTaskIdTag;
        v_zfidentity *delayTaskIdCur = view->objectTag(_ZFP_ZFUIViewBlink_tag_delayTaskId);
        if(delayTaskId != delayTaskIdCur) {
            return;
        }

        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idRelease(delayTaskId->zfv);
        _ZFP_ZFUIViewBlink_noAni_doOff(view);
    } ZFLISTENER_END()
    ZFTimerOnce(
        #if _ZFP_ZFUIViewBlink_DEBUG_duration
            (zftimet)5000
        #else
            (zftimet)(blinkParam.blinkDuration() * blinkParam.blinkCount())
        #endif
        ,
        blinkDelayOnFinish);
#else
    if(blinkParam.blinkCount() > 1) {
        view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkCountLeft, zfobj<v_zfindex>(blinkParam.blinkCount() - 1));
    }

    zfautoT<ZFAnimation> ani = ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0));
    view->objectTag(_ZFP_ZFUIViewBlink_tag_ani, ani);
    #if _ZFP_ZFUIViewBlink_DEBUG_duration
        ani->duration(5000);
    #else
        ani->duration(blinkParam.blinkDuration());
    #endif

    ZFLISTENER_1(aniOnStopListener
            , zfautoT<ZFUIView>, view
            ) {
        ZFAnimation *ani = zfargs.sender();
        ZFUIView *blinkView = ani->target();

        v_zfindex *blinkCountLeft = view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
        if(blinkCountLeft != zfnull) {
            if(blinkCountLeft->zfv <= 1) {
                view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
            }
            else {
                blinkCountLeft->zfv = blinkCountLeft->zfv - 1;
            }

            ani->start();
            return;
        }

        view->observerRemove(ZFObject::E_ObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

        view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_ani);
        view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
        blinkView->removeFromParent();
        ani->target(zfnull);

        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
        ZFGlobalObserver().observerNotifyWithSender(view, ZFGlobalEvent::E_ViewBlinkOff());
    } ZFLISTENER_END()
    view->observerAdd(ZFObject::E_ObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);
    ani->observerAdd(ZFAnimation::E_AniOnStop(), aniOnStopListener);
    ani->target(blinkView);

    ZFGlobalObserver().observerNotifyWithSender(view, ZFGlobalEvent::E_ViewBlinkOn());
    ani->start();
#endif // #if _ZFP_ZFUIViewBlink_DEBUG_noAni
}
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view) {
    ZFUIView *blinkView = view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkView);

    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
    blinkView->removeFromParent();

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_delayTaskId);
    ZFGlobalObserver().observerNotifyWithSender(view, ZFGlobalEvent::E_ViewBlinkOff());
}
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view) {
    _ZFP_ZFUIViewBlinkView *blinkView = view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView == zfnull) {
        return;
    }

    ZFAnimation *ani = view->objectTag(_ZFP_ZFUIViewBlink_tag_ani);
    if(ani != zfnull) {
        view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
        ani->stop();
    }
    else {
        _ZFP_ZFUIViewBlink_noAni_doOff(view);
    }
}

// ============================================================
// other
ZFEXPORT_VAR_DEFINE(zfautoT<ZFUIImage>, ZFUIViewBlinkImageDefault, zfnull)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkInitSetting, ZFLevelZFFrameworkNormal) {
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    if(ZFPROTOCOL_IS_AVAILABLE(ZFUIImage)) {
        ZFUIViewBlinkImageDefault(zfres("ZFUIKit/ZFUIViewBlinkImage.xml"));
    }
    else if(ZFPROTOCOL_IS_AVAILABLE(ZFUIImageIO)) {
        ZFUIViewBlinkImageDefault(ZFUIImageFromColor(ZFUIColorCreate(1, 0, 0)));
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkInitSetting) {
    ZFUIViewBlinkImageDefault(zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkInitSetting)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkCleanup, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkCleanup) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    while(!d->blinkingViews.isEmpty()) {
        _ZFP_ZFUIViewBlinkDoOff(d->blinkingViews[d->blinkingViews.count() - 1]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkCleanup)

ZF_NAMESPACE_GLOBAL_END

