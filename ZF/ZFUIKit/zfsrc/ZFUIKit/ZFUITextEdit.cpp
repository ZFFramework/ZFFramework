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
    zfbool editing;
    zfbool textUpdateByImplFlag;
    zfbool selectedRangeUpdateByImplFlag;

public:
    void updateSizeRelatedProperty(void) {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSize(this->pimplOwner, ZFUISizeApplyScale(this->pimplOwner->textSize(), this->pimplOwner->UIScaleFixed()));
    }

public:
    _ZFP_ZFUITextEditPrivate(void)
    : pimplOwner(zfnull)
    , editing(zffalse)
    , textUpdateByImplFlag(zffalse)
    , selectedRangeUpdateByImplFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUITextEdit
ZFOBJECT_REGISTER(ZFUITextEdit)

ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditBegin)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditEnd)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnUpdateCheck)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnUpdate)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnReturnClick)
ZFEVENT_REGISTER(ZFUITextEdit, TextOnEditConfirm)

ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, zfbool, focusable) {
    propertyValue = zftrue;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, ZFUISize, viewSizeMin) {
    propertyValue = ZFUISizeCreate(ZFUIGlobalStyle::DefaultStyle()->itemSizeControl());
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, editEnable) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->editEnable(this, this->editEnable());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfbool, editSecured) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textEditSecure(this, this->editSecured());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardType, keyboardType) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->keyboardType(this, this->keyboardType());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnType, keyboardReturnType) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->keyboardReturnType(this, this->keyboardReturnType());
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUITextEdit, zfanyT<ZFUITextView>, placeholder) {
    zfobj<ZFUITextView> placeholder;
    propertyValue = placeholder;
    placeholder->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorHint());
    placeholder->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
}
ZFMETHOD_DEFINE_2(ZFUITextEdit, void, placeholderProp
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, value)
        ) {
    if(this->placeholder()) {
        zfauto valueTmp;
        const ZFProperty *prop = this->placeholder()->classData()->propertyForName(propertyName);
        if(prop && zfconvT(valueTmp, prop->propertyTypeId(), value)) {
            prop->setterMethod()->methodInvoke(this->placeholder(), valueTmp);
        }
    }
}
ZFMETHOD_DEFINE_2(ZFUITextEdit, void, placeholderStyle
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const zfstring &, styleKey)
        ) {
    if(this->placeholder()) {
        this->placeholder()->propStyle(propertyName, styleKey);
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfanyT<ZFRegExp>, editFilter) {
    if(!this->text().isEmpty() && !this->textShouldUpdate(this->text())) {
        this->text("");
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUITextEdit, ZFIndexRange, selectedRange) {
    zfindex textLength = this->text().length();
    if(propertyValue.start >= textLength) {
        propertyValue = ZFIndexRangeZero();
    }
    else if(propertyValue.count > textLength - propertyValue.start) {
        propertyValue.count = textLength - propertyValue.start;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFIndexRange, selectedRange) {
    if(d->selectedRangeUpdateByImplFlag) {
        d->selectedRangeUpdateByImplFlag = zffalse;
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->selectedRange(this, this->selectedRange());
    }
}

ZFPROPERTY_ON_UPDATE_DEFINE(ZFUITextEdit, zfstring, text) {
    if(!this->textShouldUpdate(propertyValue)) {
        propertyValue = propertyValueOld;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zfstring, text) {
    if(d->textUpdateByImplFlag) {
        d->textUpdateByImplFlag = zffalse;
    }
    else {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->text(this, this->text());
    }
    if(propertyValueOld.compare(this->text()) != 0) {
        this->placeholder()->visible(this->text().isEmpty());
        this->layoutRequest();
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUITextAppearance, textAppearance) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAppearance(this, this->textAppearance());
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIAlignFlags, textAlign) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textAlign(this, this->textAlign());
    this->placeholder()->textAlign(this->textAlign());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, ZFUIColor, textColor) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textColor(this, this->textColor());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUITextEdit, zffloat, textSize) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->textSize(this, ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed()));
    if(propertyValue != propertyValueOld) {
        this->layoutRequest();
    }
}

void ZFUITextEdit::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextEditPrivate);
    d->pimplOwner = this;

    ZFCoreAssert(this->nativeImplView() == zfnull);
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

    ZFUIView *placeholderTmp = this->placeholder();
    if(placeholderTmp == zfnull) {
        if(this->placeholder() != zfnull) {
            ZFCoreCriticalClassNotTypeOf(this->placeholder()->classData(), ZFUIView::ClassData());
        }
        else {
            ZFCoreCriticalMessage("placeholder must not be null");
        }
        return;
    }
    this->internalImplViewAdd(placeholderTmp);
    placeholderTmp->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    placeholderTmp->serializableRefLayoutParam()->sizeParam(ZFUISizeParamFillFill());
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

void ZFUITextEdit::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);

    if(!this->placeholder()->text().isEmpty()) {
        zfstringAppend(ret, " (%s)", this->placeholder()->text());
    }

    if(this->editSecured()) {
        ret += " EditSecured";
    }
    else if(!this->text().isEmpty()) {
        zfstringAppend(ret, " \"%s\"", this->text());
    }

    if(!this->editEnable()) {
        ret += " EditDisabled";
    }
}

ZFMETHOD_DEFINE_2(ZFUITextEdit, void, measureTextEdit
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero())
        ) {
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFUISizeApplyScaleReverselyT(ret, ZFPROTOCOL_ACCESS(ZFUITextEdit)->measureNativeTextEdit(this,
        ZFUISizeApplyScale(ZFUILayoutParam::sizeHintOffset(sizeHint, ZFUISizeCreate(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->UIScaleFixed()),
        ZFUISizeApplyScale(this->textSize(), this->UIScaleFixed())),
        this->UIScaleFixed());
    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUITextEdit, placeholder), this)
            && !this->placeholder()->text().isEmpty()
            ) {
        ZFUISize hintSize = ZFUISizeZero();
        this->placeholder()->measureTextView(hintSize);
        if(ret.width < hintSize.width) {
            ret.width = hintSize.width;
        }
        if(ret.height < hintSize.height) {
            ret.height = hintSize.height;
        }
    }
    ZFUISizeApplyMarginReverselyT(ret, ret, nativeImplViewMargin);
}

void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyBeginEdit(void) {
    d->editing = zftrue;
    this->textOnEditBegin();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyEndEdit(void) {
    d->editing = zffalse;
    this->textOnEditEnd();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyUpdate(ZF_IN const zfstring &newText) {
    zfstring oldText = this->text();

    d->textUpdateByImplFlag = zftrue;
    this->text(newText);

    this->textOnUpdate(oldText);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_selectedRangeNotifyUpdate(void) {
    if(d->selectedRangeUpdateByImplFlag) {
        return;
    }

    ZFIndexRange tmp = ZFIndexRangeZero();
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->selectedRange(this, tmp);

    d->selectedRangeUpdateByImplFlag = zftrue;
    this->selectedRange(tmp);
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyReturnClicked(void) {
    this->textOnReturnClick();

    switch(this->keyboardReturnAction()) {
        case v_ZFUITextEditKeyboardReturnAction::e_None:
            break;
        case v_ZFUITextEditKeyboardReturnAction::e_Confirm:
            this->editConfirm();
            break;
        case v_ZFUITextEditKeyboardReturnAction::e_FocusNext:
            if(!this->focused() || !this->confirmWhenLostFocus()) {
                this->editConfirm();
            }
            if(this->focused()) {
                zfanyT<ZFUIView> next = ZFUIViewFocusNextFind(this);
                if(next == zfnull) {
                    this->editEnd();
                }
                else {
                    ZFUITextEdit *nextTmp = next;
                    if(nextTmp != zfnull) {
                        nextTmp->editBegin();
                    }
                    else {
                        next->focusRequest(zftrue);
                    }
                }
            }
            break;
        case v_ZFUITextEditKeyboardReturnAction::e_HideKeyboard: {
            this->editConfirm();
            ZFUIOnScreenKeyboardState *keyboardState = ZFUIOnScreenKeyboardState::instanceForView(this);
            if(keyboardState != zfnull && keyboardState->keyboardShowing() && this->editing()) {
                this->editEnd();
            }
        }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}
ZFMETHOD_DEFINE_1(ZFUITextEdit, zfbool, textShouldUpdate
        , ZFMP_IN(const zfstring &, newText)
        ) {
    zfbool shouldUpdate = zftrue;
    this->textOnUpdateCheck(newText, shouldUpdate);
    if(!shouldUpdate && newText != zfnull && *newText != '\0') {
        return zffalse;
    }
    else {
        return zftrue;
    }
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, editBegin) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->editBegin(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, editEnd) {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->editEnd(this);
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, zfbool, editing) {
    return d->editing;
}

void ZFUITextEdit::textOnEditBegin(void) {
    this->observerNotify(ZFUITextEdit::E_TextOnEditBegin());
}
void ZFUITextEdit::textOnEditEnd(void) {
    this->observerNotify(ZFUITextEdit::E_TextOnEditEnd());
}
void ZFUITextEdit::textOnUpdateCheck(
        ZF_IN const zfstring &newText
        , ZF_IN_OUT zfbool &shouldUpdate
        ) {
    shouldUpdate = zftrue;
    if(newText != zfnull && *newText != '\0' && this->editFilter() != zfnull) {
        ZFRegExpResult regexpResult;
        this->editFilter()->find(regexpResult, newText);
        if(!regexpResult.matched) {
            shouldUpdate = zffalse;
            return;
        }
    }

    if(this->observerHasAdd(ZFUITextEdit::E_TextOnUpdateCheck())) {
        zfobj<v_zfbool> t(shouldUpdate);
        this->observerNotify(ZFUITextEdit::E_TextOnUpdateCheck(), zfobj<v_zfstring>(newText), t);
        shouldUpdate = t->zfv;
    }
}
void ZFUITextEdit::textOnUpdate(ZF_IN const zfstring &oldText) {
    if(this->observerHasAdd(ZFUITextEdit::E_TextOnUpdate())) {
        zfobj<v_zfstring> oldTextTmp(oldText);
        this->observerNotify(ZFUITextEdit::E_TextOnUpdate(), oldTextTmp);
    }
}
void ZFUITextEdit::textOnReturnClick(void) {
    this->observerNotify(ZFUITextEdit::E_TextOnReturnClick());
}
void ZFUITextEdit::textOnEditConfirm(void) {
    this->observerNotify(ZFUITextEdit::E_TextOnEditConfirm());
}
ZFMETHOD_DEFINE_0(ZFUITextEdit, void, editConfirm) {
    this->textOnEditConfirm();
}

void ZFUITextEdit::UIScaleOnUpdate(void) {
    zfsuper::UIScaleOnUpdate();
    d->updateSizeRelatedProperty();
}
void ZFUITextEdit::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    this->measureTextEdit(ret, sizeHint);
    if(ret.width < 0) {
        ret.width = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
    if(ret.height < 0) {
        ret.height = ZFUIGlobalStyle::DefaultStyle()->itemSizeControl();
    }
}
void ZFUITextEdit::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent) {
    switch(keyEvent->keyCode) {
        case v_ZFUIKeyCode::e_kShift:
        case v_ZFUIKeyCode::e_kTab:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
        default:
            keyEvent->eventResolved(zftrue);
            break;
    }
}
void ZFUITextEdit::focusOnUpdate(void) {
    zfsuper::focusOnUpdate();
    if(!this->focused() && this->confirmWhenLostFocus()) {
        this->editConfirm();
    }
}

zfbool ZFUITextEdit::internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
    if(internalView == this->placeholder()) {
        return zffalse;
    }
    else {
        return zfsuper::internalViewShouldLayout(internalView);
    }
}
void ZFUITextEdit::internalViewOnLayout(ZF_IN const ZFUIRect &bounds) {
    zfsuper::internalViewOnLayout(bounds);

    ZFUITextView *placeholder = this->placeholder();
    placeholder->viewFrame(ZFUIRectApplyMargin(
            bounds,
            ZFUIMarginInc(this->nativeImplViewMargin(), placeholder->layoutParam()->margin())
        ));
}

ZF_NAMESPACE_GLOBAL_END

