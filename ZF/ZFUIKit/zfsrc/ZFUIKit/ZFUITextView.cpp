#include "ZFUITextView.h"
#include "protocol/ZFProtocolZFUITextView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFUITextView_updateSizeRelatedProperty(ZF_IN ZFUITextView *owner) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl = ZFPROTOCOL_ACCESS(ZFUITextView);
    impl->textSize(owner, ZFUISizeApplyScale(owner->textSize(), owner->UIScaleFixed()));
    impl->textSizeAutoMin(owner, ZFUISizeApplyScale(owner->textSizeAutoMin(), owner->UIScaleFixed()));
    impl->textSizeAutoMax(owner, ZFUISizeApplyScale(owner->textSizeAutoMax(), owner->UIScaleFixed()));
}

// ============================================================
// ZFUITextView
ZFOBJECT_REGISTER(ZFUITextView)
ZFSTYLE_DEFAULT_DEFINE(ZFUITextView)

ZFPROPERTY_ON_INIT_DEFINE(ZFUITextView, zfbool, viewUIEnable) {
    propertyValue = zffalse;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextView, zfbool, viewUIEnableTree) {
    propertyValue = zffalse;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, zfstring, text) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->text(this, this->text());
    if(propertyValueOld.compare(this->text()) != 0) {
        this->layoutRequest();
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, ZFUITextAppearance, textAppearance) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textAppearance(this, this->textAppearance());
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, ZFUIAlignFlags, textAlign) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textAlign(this, this->textAlign());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, ZFUIColor, textColor) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textColor(this, this->textColor());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, zffloat, textSize) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSize(this, ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed()));
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, zffloat, textSizeAutoMin) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeAutoMin(this, ZFUISizeApplyScale(this->textSizeAutoMin(), this->UIScaleFixed()));
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, zffloat, textSizeAutoMax) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeAutoMax(this, ZFUISizeApplyScale(this->textSizeAutoMax(), this->UIScaleFixed()));
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, zfbool, singleLine) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->singleLine(this, this->singleLine());
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextView, ZFUITextTruncateMode, textTruncateMode) {
    ZFPROTOCOL_ACCESS(ZFUITextView)->textTruncateMode(this, this->textTruncateMode());
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}

void ZFUITextView::objectOnInit(void) {
    zfsuper::objectOnInit();

    ZFCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD NativeImplViewDestroy {
    public:
        static void action(
                ZF_IN zfanyT<ZFUIView> const &view
                , ZF_IN void *nativeImplView
                ) {
            ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewDestroy(view, nativeImplView);
        }
    };
    this->nativeImplView(
            ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewCreate(this)
            , NativeImplViewDestroy::action
            );
}
void ZFUITextView::objectOnDealloc(void) {
    zfsuper::objectOnDealloc();
}
void ZFUITextView::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    _ZFP_ZFUITextView_updateSizeRelatedProperty(this);
}

void ZFUITextView::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);

    if(!this->text().isEmpty()) {
        ret += " \"";
        ret += this->text();
        ret += "\"";
    }
}

ZFMETHOD_DEFINE_2(ZFUITextView, void, measureTextView
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero())
        ) {
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFUISizeApplyScaleReverselyT(ret, ZFPROTOCOL_ACCESS(ZFUITextView)->measureNativeTextView(this,
        ZFUISizeApplyScale(ZFUILayoutParam::sizeHintOffset(sizeHint, ZFUISizeCreate(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->UIScaleFixed()),
        ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed())),
        this->UIScaleFixed());
    ZFUISizeApplyMarginReverselyT(ret, ret, nativeImplViewMargin);
}

ZFMETHOD_DEFINE_0(ZFUITextView, zffloat, textSizeCurrent) {
    return ZFUISizeApplyScaleReversely(ZFPROTOCOL_ACCESS(ZFUITextView)->textSizeCurrent(this), this->UIScaleFixed());
}

void ZFUITextView::UIScaleOnUpdate(void) {
    zfsuper::UIScaleOnUpdate();
    _ZFP_ZFUITextView_updateSizeRelatedProperty(this);
}
void ZFUITextView::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    this->measureTextView(ret, sizeHint);
}
void ZFUITextView::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);
    ZFPROTOCOL_ACCESS(ZFUITextView)->layoutNativeTextView(this,
        ZFUISizeApplyScale(ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), this->nativeImplViewMargin()), this->UIScaleFixed()));
}

ZF_NAMESPACE_GLOBAL_END

