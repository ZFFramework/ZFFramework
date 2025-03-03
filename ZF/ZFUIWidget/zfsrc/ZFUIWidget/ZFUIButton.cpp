#include "ZFUIButton.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// #define _ZFP_ZFUIButton_DEBUG 1

#if _ZFP_ZFUIButton_DEBUG
    #define _ZFP_ZFUIButton_DEBUG_LOG(fmt, ...) \
        ZFLogTrim() << "[ZFUIButton] " << zfstr(fmt, ##__VA_ARGS__) << " " << ZFLogHeader(ZFCallerInfoCreate());
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName) \
        ZFLogTrim() << "[ZFUIButton] " << ZFM_TOSTRING(actionName) << " " << ZFLogHeader(ZFCallerInfoCreate());
#else
    #define _ZFP_ZFUIButton_DEBUG_LOG(fmt, ...)
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName)
#endif

// ============================================================
ZFENUM_DEFINE(ZFUIButtonState)

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIButton)

// ============================================================
// _ZFP_ZFUIButtonPrivate
zfclassPOD _ZFP_ZFUIButtonMouseData {
public:
    zfidentity mouseId;
    ZFUIMouseAction mouseAction;
    ZFUIPoint mousePoint;
};
static inline _ZFP_ZFUIButtonMouseData _ZFP_ZFUIButtonMouseDataFromEvent(ZF_IN ZFUIMouseEvent *event) {
    _ZFP_ZFUIButtonMouseData ret = {
        event->mouseId,
        event->mouseAction,
        event->mousePoint,
    };
    return ret;
}
#define _ZFP_ZFUIButtonMouseDataToEvent(event, mouseData) \
    zfobj<ZFUIMouseEvent> event; \
    event->eventResolved(zffalse); \
    event->mouseId = mouseData.mouseId; \
    event->mouseAction = mouseData.mouseAction; \
    event->mousePoint = mouseData.mousePoint; \
    event->mouseButton = v_ZFUIMouseButton::e_Left

zfclassNotPOD _ZFP_ZFUIButtonPrivate {
public:
    ZFUIButton *pimplOwner;
    ZFUIButtonState buttonState;
    zfidentity processingMouseId;
    ZFUIPoint prevMousePoint;
    zfbool enableFlag;
    zfbool buttonHighlightedFlag;
    zftimet buttonLastClickTimestamp;

public:
    void enable(ZF_IN zfbool enable) {
        if(this->enableFlag == enable) {
            return;
        }
        this->enableFlag = enable;

        this->buttonStateUpdate(this->buttonHighlightedFlag);
    }

public:
    void buttonStateUpdate(ZF_IN zfbool highlighted) {
        ZFUIButtonState buttonStateOld = this->buttonState;
        this->buttonHighlightedFlag = highlighted;
        if(this->enableFlag) {
            if(this->buttonHighlightedFlag) {
                if(this->pimplOwner->checked()) {
                    this->buttonState = v_ZFUIButtonState::e_CheckedHighlighted;
                }
                else {
                    this->buttonState = v_ZFUIButtonState::e_Highlighted;
                }
                if(buttonStateOld != this->buttonState) {
                    if(this->pimplOwner->checked()) {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnCheckedHighlighted)
                    }
                    else {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnHighlighted)
                    }
                    this->pimplOwner->buttonStateOnUpdate();
                }
            }
            else {
                if(this->pimplOwner->checked()) {
                    this->buttonState = v_ZFUIButtonState::e_Checked;
                }
                else {
                    this->buttonState = v_ZFUIButtonState::e_Normal;
                }
                if(buttonStateOld != this->buttonState) {
                    if(this->pimplOwner->checked()) {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnChecked)
                    }
                    else {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                    }
                    this->pimplOwner->buttonStateOnUpdate();
                }
            }
        }
        else {
            if(buttonStateOld != this->buttonState) {
                this->buttonState = v_ZFUIButtonState::e_Disabled;
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnDisabled)
                this->pimplOwner->buttonStateOnUpdate();
            }
        }
    }
    void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent) {
        _ZFP_ZFUIButton_DEBUG_LOG("begin %s", mouseEvent)
        ZFUIMouseAction mouseAction = mouseEvent->mouseAction;
        switch(mouseAction) {
            case v_ZFUIMouseAction::e_Down:
                if(!this->enableFlag) {
                    _ZFP_ZFUIButton_DEBUG_LOG("      %s disabled", mouseEvent)
                    break;
                }
                this->processingMouseId = mouseEvent->mouseId;
                this->processMouse(mouseEvent);
                break;
            case v_ZFUIMouseAction::e_Move:
                if(mouseEvent->mouseId == this->processingMouseId) {
                    this->processMouse(mouseEvent);
                }
                break;
            case v_ZFUIMouseAction::e_Up:
            case v_ZFUIMouseAction::e_Cancel:
                if(mouseEvent->mouseId == this->processingMouseId) {
                    this->processMouse(mouseEvent);
                }
                break;
            case v_ZFUIMouseAction::e_HoverEnter:
            case v_ZFUIMouseAction::e_Hover:
            case v_ZFUIMouseAction::e_HoverExit:
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        mouseEvent->eventResolved(zftrue);
        _ZFP_ZFUIButton_DEBUG_LOG(" end  %s", mouseEvent)
    }

private:
    void processMouse(ZF_IN ZFUIMouseEvent *mouseEvent) {
        _ZFP_ZFUIButton_DEBUG_LOG("process begin %s", mouseEvent)
        switch(mouseEvent->mouseAction) {
            case v_ZFUIMouseAction::e_Down: {
                this->prevMousePoint = mouseEvent->mousePoint;
                this->buttonStateUpdate(zftrue);
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnDown)
                this->pimplOwner->buttonMouseOnDown(mouseEvent);
            }
                break;
            case v_ZFUIMouseAction::e_Move: {
                ZFUIRect bounds = ZFUIRectGetBounds(this->pimplOwner->viewFrame());
                zfbool mouseInside = this->buttonClickedInside(bounds, mouseEvent->mousePoint);
                zfbool mouseInsidePrev = this->buttonClickedInside(bounds, this->prevMousePoint);
                this->prevMousePoint = mouseEvent->mousePoint;

                if(mouseInside != mouseInsidePrev) {
                    this->buttonStateUpdate(mouseInside);
                    if(mouseInside) {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveEnter)
                        this->pimplOwner->buttonMouseOnMoveEnter(mouseEvent);
                    }
                    else {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveExit)
                        this->pimplOwner->buttonMouseOnMoveExit(mouseEvent);
                    }
                }
                if(mouseInside) {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveInside)
                    this->pimplOwner->buttonMouseOnMoveInside(mouseEvent);
                }
                else {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveOutside)
                    this->pimplOwner->buttonMouseOnMoveOutside(mouseEvent);
                }
            }
                break;
            case v_ZFUIMouseAction::e_Up: {
                zfbool mouseInside = this->buttonClickedInside(
                    ZFUIRectGetBounds(this->pimplOwner->viewFrame()),
                    mouseEvent->mousePoint);

                this->buttonHighlightedFlag = zffalse;
                if(mouseInside) {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUpInside)
                    this->pimplOwner->buttonMouseOnUpInside(mouseEvent);
                    if(this->pimplOwner->checkable()) {
                        this->pimplOwner->checked(!this->pimplOwner->checked());
                    }
                    else {
                        this->buttonStateUpdate(this->buttonHighlightedFlag);
                    }
                    this->buttonClicked(mouseEvent);
                }
                else {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                    this->pimplOwner->buttonMouseOnUp(mouseEvent);
                    this->buttonStateUpdate(this->buttonHighlightedFlag);
                }
            }
                break;
            case v_ZFUIMouseAction::e_Cancel: {
                this->buttonStateUpdate(zffalse);
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                this->pimplOwner->buttonMouseOnUp(mouseEvent);
            }
                break;
            case v_ZFUIMouseAction::e_HoverEnter:
            case v_ZFUIMouseAction::e_Hover:
            case v_ZFUIMouseAction::e_HoverExit:
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        _ZFP_ZFUIButton_DEBUG_LOG("process  end  %s", mouseEvent)
    }
    zfbool buttonClickedInside(
            ZF_IN const ZFUIRect &bounds
            , ZF_IN const ZFUIPoint &mousePoint
            ) {
        return ZFUIRectIsContainPoint(bounds, mousePoint, this->pimplOwner->clickTolerance());
    }
public:
    void buttonClicked(ZF_IN ZFUIEvent *event) {
        zftimet curTime = ZFTime::timestamp();
        if(curTime - this->buttonLastClickTimestamp >= this->pimplOwner->clickInterval()) {
            this->buttonLastClickTimestamp = curTime;

            _ZFP_ZFUIButton_DEBUG_EVENT(buttonOnClick)
            this->pimplOwner->buttonOnClick(event);
        }
        else {
            _ZFP_ZFUIButton_DEBUG_LOG("click ignored %s", event)
        }
    }

public:
    _ZFP_ZFUIButtonPrivate(void)
    : pimplOwner(zfnull)
    , buttonState(v_ZFUIButtonState::e_Normal)
    , processingMouseId(zfidentityInvalid())
    , prevMousePoint()
    , enableFlag(zftrue)
    , buttonHighlightedFlag(zffalse)
    , buttonLastClickTimestamp(0)
    {
    }
};

// ============================================================
// ZFUIButton
ZFOBJECT_REGISTER(ZFUIButton)

ZFEVENT_REGISTER(ZFUIButton, ButtonOnClick)
ZFEVENT_REGISTER(ZFUIButton, ButtonStateOnUpdate)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnDown)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveEnter)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveExit)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveInside)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveOutside)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnUpInside)
ZFEVENT_REGISTER(ZFUIButton, ButtonMouseOnUp)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButton, zfbool, focusable) {
    propertyValue = zftrue;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButton, ZFUISize, viewSizeMin) {
    propertyValue = ZFUISizeCreate(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButton, zfbool, enable) {
    d->enable(this->enable());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButton, zfbool, checkable) {
    if(!this->checkable() && this->checked()) {
        this->checked(zffalse);
    }
}
ZFPROPERTY_ON_VERIFY_DEFINE(ZFUIButton, zfbool, checked) {
    if(!this->checkable()) {
        propertyValue = zffalse;
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButton, zfbool, checked) {
    d->buttonStateUpdate(d->buttonHighlightedFlag);
}

void ZFUIButton::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonPrivate);
    d->pimplOwner = this;
}
void ZFUIButton::objectOnInitFinish(void) {
    zfsuper::objectOnInitFinish();
    this->buttonStateOnUpdate();
}
void ZFUIButton::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFUIButton, void, clickIntervalReset) {
    d->buttonLastClickTimestamp = 0;
}

ZFMETHOD_DEFINE_1(ZFUIButton, void, simulateClick
        , ZFMP_IN_OPT(ZFUIEvent *, event, zfnull)
        ) {
    ZFCoreAssert(ZFThread::currentThread() == ZFThread::mainThread());
    d->buttonHighlightedFlag = zffalse;
    if(this->checkable()) {
        this->checked(!this->checked());
    }
    else {
        d->buttonStateUpdate(d->buttonHighlightedFlag);
    }
    d->buttonClicked(event);
}

ZFMETHOD_DEFINE_2(ZFUIButton, void, onClick
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        ) {
    this->observerAdd(ZFUIButton::E_ButtonOnClick(), observer, observerLevel);
}
ZFMETHOD_DEFINE_2(ZFUIButton, void, onClickForOnce
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        ) {
    this->observerAddForOnce(ZFUIButton::E_ButtonOnClick(), observer, observerLevel);
}

void ZFUIButton::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent) {
    zfsuper::viewEventOnMouseEvent(mouseEvent);

    if(mouseEvent->mouseButton == v_ZFUIMouseButton::e_Left) {
        d->viewEventOnMouseEvent(mouseEvent);
    }
}
void ZFUIButton::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent) {
    zfsuper::viewEventOnKeyEvent(keyEvent);
    if(keyEvent->eventResolved()) {
        return;
    }

    switch(keyEvent->keyCode) {
        case v_ZFUIKeyCode::e_kEnter:
        case v_ZFUIKeyCode::e_kSpace:
            switch(keyEvent->keyAction) {
                case v_ZFUIKeyAction::e_Down:
                    d->buttonStateUpdate(zftrue);
                    break;
                case v_ZFUIKeyAction::e_Repeat:
                    break;
                case v_ZFUIKeyAction::e_Up:
                    d->buttonHighlightedFlag = zffalse;
                    if(this->checkable()) {
                        this->checked(!this->checked());
                    }
                    else {
                        d->buttonStateUpdate(d->buttonHighlightedFlag);
                    }
                    d->buttonClicked(keyEvent);
                    break;
                case v_ZFUIKeyAction::e_Cancel:
                    d->buttonStateUpdate(zffalse);
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
            keyEvent->eventResolved(zftrue);
            break;
        default:
            break;
    }
}

ZFMETHOD_DEFINE_0(ZFUIButton, ZFUIButtonState, buttonState) {
    return d->buttonState;
}

ZF_NAMESPACE_GLOBAL_END

