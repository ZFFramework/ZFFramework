#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData
{
public:
    ZFUIOnScreenKeyboardState *state;
    ZFListener callback;
};

ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test)
{
    ZFLISTENER_1(sysWindowOnCreate
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFLISTENER(action) {
            zfLogTrimT() << "[ZFUIOnScreenKeyboardState] state changed:" << zfargs.sender();
        } ZFLISTENER_END()
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender()->toAny());
        state->observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), action);
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData task;
        task.state = state;
        task.callback = action;
        taskList.add(task);
    } ZFLISTENER_END()
    this->sysWindowOnCreateListener = sysWindowOnCreate;
    ZFGlobalObserver().observerAdd(ZFUISysWindow::EventSysWindowOnCreate(), this->sysWindowOnDestroyListener);

    ZFLISTENER_1(sysWindowOnDestroy
            , ZFCoreArray<_ZFP_ZFUIOnScreenKeyboardState_test_ObserverData>, taskList
            ) {
        ZFUIOnScreenKeyboardState *state = ZFUIOnScreenKeyboardState::instanceForSysWindow(zfargs.sender()->toAny());
        for(zfindex i = taskList.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
            if(task.state == state)
            {
                state->observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), task.callback);
                taskList.remove(i);
            }
        }
    } ZFLISTENER_END()
    this->sysWindowOnDestroyListener = sysWindowOnDestroy;
    ZFGlobalObserver().observerAdd(ZFUISysWindow::EventSysWindowOnDestroy(), this->sysWindowOnDestroyListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test)
{
    ZFGlobalObserver().observerRemove(ZFUISysWindow::EventSysWindowOnCreate(), this->sysWindowOnCreateListener);
    ZFGlobalObserver().observerRemove(ZFUISysWindow::EventSysWindowOnDestroy(), this->sysWindowOnDestroyListener);

    for(zfindex i = 0; i < taskList.count(); ++i)
    {
        _ZFP_ZFUIOnScreenKeyboardState_test_ObserverData const &task = taskList[i];
        task.state->observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), task.callback);
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

