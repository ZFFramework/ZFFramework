#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData {
public:
    ZFUIOnScreenKeyboardState *state;
    ZFListener callback;
};

ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test) {
    ZFLISTENER_1(rootWindowOnCreate
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFLISTENER(action) {
            ZFLogTrim() << "[ZFUIOnScreenKeyboardState] state changed: " << zfargs.sender();
        } ZFLISTENER_END()
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForRootWindow(zfargs.sender());
        state->observerAdd(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), action);
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData task;
        task.state = state;
        task.callback = action;
        taskList.add(task);
    } ZFLISTENER_END()
    this->rootWindowOnCreateListener = rootWindowOnCreate;
    ZFGlobalObserver().observerAdd(ZFUIRootWindow::E_WindowOnCreate(), this->rootWindowOnCreateListener);

    ZFLISTENER_1(rootWindowOnDestroy
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForRootWindow(zfargs.sender());
        for(zfindex i = taskList.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
            if(task.state == state) {
                state->observerRemove(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), task.callback);
                taskList.remove(i);
            }
        }
    } ZFLISTENER_END()
    this->rootWindowOnDestroyListener = rootWindowOnDestroy;
    ZFGlobalObserver().observerAdd(ZFUIRootWindow::E_WindowOnDestroy(), this->rootWindowOnDestroyListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test) {
    ZFGlobalObserver().observerRemove(ZFUIRootWindow::E_WindowOnCreate(), this->rootWindowOnCreateListener);
    ZFGlobalObserver().observerRemove(ZFUIRootWindow::E_WindowOnDestroy(), this->rootWindowOnDestroyListener);

    for(zfindex i = 0; i < taskList.count(); ++i) {
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
        task.state->observerRemove(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), task.callback);
        taskList.remove(i);
    }
    taskList.removeAll();
}
private:
    ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData> taskList;
    ZFListener rootWindowOnCreateListener;
    ZFListener rootWindowOnDestroyListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

