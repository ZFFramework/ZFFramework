#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData {
public:
    ZFUIOnScreenKeyboardState *state;
    ZFListener callback;
};

ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test) {
    ZFLISTENER_1(sysWindowOnCreate
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFLISTENER(action) {
            ZFLogTrim() << "[ZFUIOnScreenKeyboardState] state changed: " << zfargs.sender();
        } ZFLISTENER_END()
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender());
        state->observerAdd(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), action);
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData task;
        task.state = state;
        task.callback = action;
        taskList.add(task);
    } ZFLISTENER_END()
    this->sysWindowOnCreateListener = sysWindowOnCreate;
    ZFGlobalObserver().observerAdd(ZFUISysWindow::E_SysWindowOnCreate(), this->sysWindowOnDestroyListener);

    ZFLISTENER_1(sysWindowOnDestroy
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender());
        for(zfindex i = taskList.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
            if(task.state == state) {
                state->observerRemove(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), task.callback);
                taskList.remove(i);
            }
        }
    } ZFLISTENER_END()
    this->sysWindowOnDestroyListener = sysWindowOnDestroy;
    ZFGlobalObserver().observerAdd(ZFUISysWindow::E_SysWindowOnDestroy(), this->sysWindowOnDestroyListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test) {
    ZFGlobalObserver().observerRemove(ZFUISysWindow::E_SysWindowOnCreate(), this->sysWindowOnCreateListener);
    ZFGlobalObserver().observerRemove(ZFUISysWindow::E_SysWindowOnDestroy(), this->sysWindowOnDestroyListener);

    for(zfindex i = 0; i < taskList.count(); ++i) {
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
        task.state->observerRemove(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), task.callback);
        taskList.remove(i);
    }
    taskList.removeAll();
}
private:
    ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData> taskList;
    ZFListener sysWindowOnCreateListener;
    ZFListener sysWindowOnDestroyListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

