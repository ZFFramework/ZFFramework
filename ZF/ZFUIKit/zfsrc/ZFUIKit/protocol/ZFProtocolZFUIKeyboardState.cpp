#include "ZFProtocolZFUIKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIKeyboardState)

// ============================================================
static zfbool _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zffalse;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIKeyboardStateBuiltinImpl_DataHolder, ZFLevelZFFrameworkNormal) {
    _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zftrue;
    this->implRegisterFlag = zffalse;

    ZFLISTENER(viewOnEvent) {
        ZFUIKeyEvent *event = zfargs.param0();
        if(event != zfnull) {
            ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(event);
        }
    } ZFLISTENER_END()
    this->viewOnEventListener = viewOnEvent;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardStateBuiltinImpl_DataHolder) {
    this->implUnregister();
    _ZFP_ZFUIKeyboardStateBuiltinImpl_available = zffalse;
}
public:
    ZFCoreArray<ZFUIKeyCode> keyPressed;
    ZFCoreArray<zfflags> keyPressedRaw;
    ZFListener viewOnEventListener;

    zfbool implRegisterFlag;
    void implRegister(void) {
        if(this->implRegisterFlag) {
            return;
        }
        this->implRegisterFlag = zftrue;

        ZFGlobalObserver().observerAdd(
                ZFUIView::E_ViewOnEvent(),
                this->viewOnEventListener,
                ZFLevelZFFrameworkEssential
            );
    }
    void implUnregister(void) {
        if(!this->implRegisterFlag) {
            return;
        }
        this->implRegisterFlag = zffalse;

        ZFGlobalObserver().observerRemove(
            ZFUIView::E_ViewOnEvent(),
            this->viewOnEventListener);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardStateBuiltinImpl_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIKeyboardStateBuiltinImpl, ZFUIKeyboardState, ZFProtocolLevel::e_Default)
public:
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCode keyCode) {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressed.find(keyCode) != zfindexMax());
    }
    virtual zfbool keyPressedRaw(ZF_IN zfflags keyCodeRaw) {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressedRaw.find(keyCodeRaw) != zfindexMax());
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIKeyboardStateBuiltinImpl)

// ============================================================
void ZFUIKeyboardStateBuiltinImplRegister(void) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->implRegister();
}
void ZFUIKeyboardStateBuiltinImplUnregister(void) {
    if(_ZFP_ZFUIKeyboardStateBuiltinImpl_available) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->implUnregister();
    }
}

void ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIKeyboardStateBuiltinImpl_DataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder);
    switch(event->keyAction) {
        case ZFUIKeyAction::e_Down:
        case ZFUIKeyAction::e_Repeat: {
            d->keyPressed.add(event->keyCode);
            d->keyPressedRaw.add(event->keyCodeRaw);
        }
            break;
        case ZFUIKeyAction::e_Up:
        case ZFUIKeyAction::e_Cancel: {
            zfindex index = 0;
            for( ; index < d->keyPressed.count(); ++index) {
                if(d->keyPressed[index] == event->keyCode
                        && d->keyPressedRaw[index] == event->keyCodeRaw) {
                    break;
                }
            }
            if(index < d->keyPressed.count()) {
                d->keyPressed.remove(index);
                d->keyPressedRaw.remove(index);
            }
        }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull) {
        impl->notifyKeyPressedOnUpdate();
    }
}

ZF_NAMESPACE_GLOBAL_END

