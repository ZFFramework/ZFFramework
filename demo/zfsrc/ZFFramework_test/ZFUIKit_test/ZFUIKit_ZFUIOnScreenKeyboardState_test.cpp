#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test)
{
    this->observerOwner = zflineAlloc(ZFArray);

    ZFLISTENER_1(sysWindowOnCreate
            , ZFArray *, observerOwner
            ) {
        ZFLISTENER(action) {
            zfLogTrimT() << "[ZFUIOnScreenKeyboardState] state changed:" << zfargs.sender();
        } ZFLISTENER_END(action)
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender()->toAny());
        state->observerAdd(ZFObserverAddParam()
            .eventId(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange())
            .observer(action)
            .owner(observerOwner)
            );
        observerOwner->add(state->objectHolder());
    } ZFLISTENER_END(sysWindowOnCreate)
    ZFGlobalObserver().observerAdd(ZFObserverAddParam()
            .eventId(ZFUISysWindow::EventSysWindowOnCreate())
            .observer(sysWindowOnCreate)
            .owner(this->observerOwner)
        );

    ZFLISTENER_1(sysWindowOnDestroy
            , ZFArray *, observerOwner
            ) {
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender()->toAny());
        state->observerRemoveByOwner(observerOwner);
        observerOwner->removeElement(state->objectHolder());
    } ZFLISTENER_END(sysWindowOnDestroy)
    ZFGlobalObserver().observerAdd(ZFObserverAddParam()
            .eventId(ZFUISysWindow::EventSysWindowOnDestroy())
            .observer(sysWindowOnDestroy)
            .owner(this->observerOwner)
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test)
{
    ZFGlobalObserver().observerRemoveByOwner(this->observerOwner);
    ZFArray *attached = this->observerOwner;
    for(zfindex i = 0; i < attached->count(); ++i)
    {
        ZFObjectHolder *stateHolder = attached->get(i)->toAny();
        stateHolder->objectHolded()->observerRemoveByOwner(this->observerOwner);
    }
}
zfautoObject observerOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

