#include "ZFUITextEdit.h"
#include "protocol/ZFProtocolZFUITextEdit.h"

#include "ZFUIViewFocus.h"
#include "ZFUIOnScreenKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUITextEditKeyboardType)

ZFENUM_DEFINE(ZFUITextEditKeyboardReturnType)
ZFENUM_DEFINE(ZFUITextEditKeyboardReturnAction)

ZFSTYLE_DEFAULT_DEFINE(ZFUITextEdit)

// ============================================================
// _ZFP_ZFUITextEditPrivate
zfclassNotPOD _ZFP_ZFUITextEditPrivate {
public:
    ZFUITextEdit *pimplOwner;
    zfbool textEditing;
    zfbool textChangedByImplFlag;
    zfbool textSelectRangeChangedByImplFlag;

public:
    void updateSizeRelatedProperty(void) {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSize(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSize(), this->pimplOwner->UIScaleFixed()));
    }

public:
    _ZFP_ZFUITextEditPrivate(void)
    : pimplOwner(zfnull)
    , textEditing(zffalse)
    , textChangedByImplFlag(zffalse)
    , textSelectRangeChangedByImplFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUITextEdit
ZFOBJECT_REGISTER(ZFUITextEdit)

ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditBegin)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditEnd)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnChangeCheck)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnChange)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnReturnClick)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditConfirm)

ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, zfbool, viewFocusable) {
    propertyValue = zftrue;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, ZFUISize, viewSizeMin) {
    propertyValue = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeControl());
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, textEditEnable) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditEnable(this, this->textEditEnable());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, textEditSecured) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditSecure(this, this->textEditSecured());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardTypeEnum, textEditKeyboardType) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditKeyboardType(this, this->textEditKeyboardType());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditKeyboardReturnType(this, this->textEditKeyboardReturnType());
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, zfanyT<ZFUITextView>, textPlaceHolder) {
    zfobj<ZFUITextView> textPlaceHolder;
    propertyValue = textPlaceHolder;
    textPlaceHolder->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorHint());
    textPlaceHolder->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfanyT<ZFRegExp>, textEditFilter) {
    if(!this->text().isEmpty() && !this->textShouldChange(this->text())) {
        this->text("");
    }
}
ZFPROPERTY_ON_VERIFY_DEFINE(ZFUITextEdit, ZFIndexRange, textSelectRange) {
    zfindex textLength = this->text().length();
    if(propertyValue.start >= textLength) {
        propertyValue = ZFIndexRangeZero();
    }
    else if(propertyValue.count > textLength - propertyValue.start) {
        propertyValue.count = textLength - propertyValue.start;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFIndexRange, textSelectRange) {
    if(d->textSelectRangeChangedByImplFlag) {
        d->textSelectRangeChangedByImplFlag = zffalse;
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSelectRange(this, this->textSelectRange());
    }
}

ZFPROPERTY_ON_VERIFY_DEFINE(ZFUITextEdit, zfstring, text) {
    if(!this->textShouldChange(propertyValue)) {
        propertyValue = propertyValueOld;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfstring, text) {
    if(d->textChangedByImplFlag) {
        d->textChangedByImplFlag = zffalse;
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->text(this, this->text());
    }
    if(propertyValueOld.compare(this->text()) != 0) {
        this->textPlaceHolder()->viewVisible(this->text().isEmpty());
        this->layoutRequest();
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextAppearanceEnum, textAppearance) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAppearance(this, this->textAppearance());
    if(this->textAppearance() != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIAlignFlags, textAlign) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAlign(this, this->textAlign());
    this->textPlaceHolder()->textAlign(this->textAlign());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIColor, textColor) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textColor(this, this->textColor());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zffloat, textSize) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSize(this, ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed()));
    if(this->textSize() != propertyValueOld) {
        this->layoutRequest();
    }
}

void ZFUITextEdit::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextEditPrivate);
    d->pimplOwner = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD NativeImplViewDestroy {
    public:
        static void action(
                ZF_IN zfanyT<ZFUIView> const &view
                , ZF_IN void *nativeImplView
                ) {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditDestroy(view, nativeImplView);
        }
    };
    zfbool nativeImplViewRequireVirtualIndex = zftrue;
    void *nativeImplView = ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditCreate(this, nativeImplViewRequireVirtualIndex);
    this->nativeImplView(
        nativeImplView,
        NativeImplViewDestroy::action,
        nativeImplViewRequireVirtualIndex);

    ZFUIView *textPlaceHolderTmp = this->textPlaceHolder();
    if(textPlaceHolderTmp == zfnull) {
        if(this->textPlaceHolder() != zfnull) {
            zfCoreCriticalClassNotTypeOf(this->textPlaceHolder()->classData(), ZFUIView::ClassData());
        }
        else {
            zfCoreCriticalMessage("textPlaceHolder must not be null");
        }
        return;
    }
    this->internalImplViewAdd(textPlaceHolderTmp);
    textPlaceHolderTmp->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    textPlaceHolderTmp->serializableRefLayoutParam()->sizeParam(ZFUISizeParamFillFill());
}
void ZFUITextEdit::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextEdit::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextEdit::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);

    if(!this->textPlaceHolder()->text().isEmpty()) {
        zfstringAppend(ret, " (%s)", this->textPlaceHolder()->text());
    }

    if(this->textEditSecured()) {
        ret += " EditSecured";
    }
    else if(!this->text().isEmpty()) {
        zfstringAppend(ret, " \"%s\"", this->text());
    }

    if(!this->textEditEnable()) {
        ret += " EditDisabled";
    }
}

ZFMETHOD_DEFINE_2(ZFUITextEdit, void, measureTextEdit
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero())
        ) {
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFUISizeApplyScaleReversely(ret, ZFPROTOCOL_ACCESS(ZFUITextEdit)->measureNativeTextEdit(this,
        ZFUISizeApplyScale(ZFUILayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->UIScaleFixed()),
        ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed())),
        this->UIScaleFixed());
    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUITextEdit, textPlaceHolder), this)
            && !this->textPlaceHolder()->text().isEmpty()
            ) {
        ZFUISize hintSize = ZFUISizeZero();
        this->textPlaceHolder()->measureTextView(hintSize);
        if(ret.width < hintSize.width) {
            ret.width = hintSize.width;
        }
        if(ret.height < hintSize.height) {
            ret.height = hintSize.height;
        }
    }
    ZFUISizeApplyMarginReversely(ret, ret, nativeImplViewMargin);
}

void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyBeginEdit(void) {
    d->textEditing = zftrue;
    this->textOnEditBegin();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyEndEdit(void) {
    d->textEditing = zffalse;
    this->textOnEditEnd();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyChange(ZF_IN const zfstring &newText) {
    zfstring oldText = this->text();

    d->textChangedByImplFlag = zftrue;
    this->text(newText);

    this->textOnChange(oldText);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textSelectRangeNotifyChange(void) {
    if(d->textSelectRangeChangedByImplFlag) {
        return;
    }

    ZFIndexRange tmp = ZFIndexRangeZero();
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSelectRange(this, tmp);

    d->textSelectRangeChangedByImplFlag = zftrue;
    this->textSelectRange(tmp);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyReturnClicked(void) {
    this->textOnReturnClick();

    switch(this->textEditKeyboardReturnAction()) {
        case ZFUITextEditKeyboardReturnAction::e_None:
            break;
        case ZFUITextEditKeyboardReturnAction::e_Confirm:
            this->textEditNotifyConfirm();
            break;
        case ZFUITextEditKeyboardReturnAction::e_FocusNext:
            if(!this->viewFocused() || !this->textEditConfirmWhenLostFocus()) {
                this->textEditNotifyConfirm();
            }
            if(this->viewFocused()) {
                zfanyT<ZFUIView> next = ZFUIViewFocusNextFind(this);
                if(next == zfnull) {
                    this->textEditEnd();
                }
                else {
                    ZFUITextEdit *nextTmp = next;
                    if(nextTmp != zfnull) {
                        nextTmp->textEditBegin();
                    }
                    else {
                        next->viewFocusRequest(zftrue);
                    }
                }
            }
            break;
        case ZFUITextEditKeyboardReturnAction::e_HideKeyboard: {
            this->textEditNotifyConfirm();
            ZFUIOnScreenKeyboardState *keyboardState = ZFUIOnScreenKeyboardState::instanceForView(this);
            if(keyboardState != zfnull && keyboardState->keyboardShowing() && this->textEditing()) {
                this->textEditEnd();
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return;
    }
}
ZFMETHOD_DEFINE_1(ZFUITextEdit, zfbool, textShouldChange
        , ZFMP_IN(const zfstring &, newText)
        ) {
    zfbool shouldChange = zftrue;
    this->textOnChangeCheck(newText, shouldChange);
    if(!shouldChange && newText != zfnull && *newText != '\0') {
        return zffalse;
    }
    else {
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, textEditBegin) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditBegin(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, textEditEnd) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditEnd(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, zfbool, textEditing) {
    return d->textEditing;
}

void ZFUITextEdit::textOnEditBegin(void) {
    this->observerNotify(ZFUITextEdit::EventTextOnEditBegin());
}
void ZFUITextEdit::textOnEditEnd(void) {
    this->observerNotify(ZFUITextEdit::EventTextOnEditEnd());
}
void ZFUITextEdit::textOnChangeCheck(
        ZF_IN const zfstring &newText
        , ZF_IN_OUT zfbool &shouldChange
        ) {
    shouldChange = zftrue;
    if(newText != zfnull && *newText != '\0' && this->textEditFilter() != zfnull) {
        ZFRegExpResult regexpResult;
        this->textEditFilter()->find(regexpResult, newText);
        if(!regexpResult.matched) {
            shouldChange = zffalse;
            return;
        }
    }

    if(this->observerHasAdd(ZFUITextEdit::EventTextOnChangeCheck())) {
        zfobj<v_zfbool> t(shouldChange);
        this->observerNotify(ZFUITextEdit::EventTextOnChangeCheck(), zfobj<v_zfstring>(newText), t);
        shouldChange = t->zfv;
    }
}
void ZFUITextEdit::textOnChange(ZF_IN const zfstring &oldText) {
    if(this->observerHasAdd(ZFUITextEdit::EventTextOnChange())) {
        zfobj<v_zfstring> oldTextTmp(oldText);
        this->observerNotify(ZFUITextEdit::EventTextOnChange(), oldTextTmp);
    }
}
void ZFUITextEdit::textOnReturnClick(void) {
    this->observerNotify(ZFUITextEdit::EventTextOnReturnClick());
}
void ZFUITextEdit::textOnEditConfirm(void) {
    this->observerNotify(ZFUITextEdit::EventTextOnEditConfirm());
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, textEditNotifyConfirm) {
    this->textOnEditConfirm();
}

void ZFUITextEdit::UIScaleOnChange(void) {
    zfsuper::UIScaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextEdit::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    ret = this->viewSizePrefer();
    ZFUISize tmp = ZFUISizeZero();
    this->measureTextEdit(tmp, sizeHint);
    if(ret.width < tmp.width) {
        ret.width = tmp.width;
    }
    if(ret.height < 0) {
        ret.height = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
}
void ZFUITextEdit::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent) {
    switch(keyEvent->keyCode) {
        case ZFUIKeyCode::e_kShift:
        case ZFUIKeyCode::e_kTab:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
        default:
            keyEvent->eventResolved(zftrue);
            break;
    }
}
void ZFUITextEdit::viewFocusOnChange(void) {
    zfsuper::viewFocusOnChange();
    if(!this->viewFocused() && this->textEditConfirmWhenLostFocus()) {
        this->textEditNotifyConfirm();
    }
}

zfbool ZFUITextEdit::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if(internalView == this->textPlaceHolder()) {
        return zffalse;
    }
    else {
        return zfsuper::internalViewShouldLayout(internalView);
    }
}
void ZFUITextEdit::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUITextView *textPlaceHolder = this->textPlaceHolder();
    textPlaceHolder->viewFrame(ZFUIRectApplyMargin(
            bounds,
            ZFUIMarginInc(this->nativeImplViewMargin(), textPlaceHolder->layoutParam()->layoutMargin())
        ));
}

ZF_NAMESPACE_GLOBAL_END

