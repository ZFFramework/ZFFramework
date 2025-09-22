#include "ZFProtocolZFApp.h"
#include "ZFProtocolZFMainEntry.h"
#include "ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFApp)

zfclassNotPOD _ZFP_ZFThreadImpl_default_TaskData {
public:
    void *threadToken;
    void *mainThreadToken;
public:
    _ZFP_ZFThreadImpl_default_TaskData(void)
    : threadToken(zfnull)
    , mainThreadToken(zfnull)
    {
    }
};

void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appExit(ZF_IN zfint appExitCode) {
    _ZFP_ZFThreadImpl_default_TaskData *taskData = zfnew(_ZFP_ZFThreadImpl_default_TaskData);

    ZFLISTENER_1(runnable
            , _ZFP_ZFThreadImpl_default_TaskData *, taskData
            ) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFThread) *threadImpl = ZFPROTOCOL_ACCESS(ZFThread);
        void *sleepToken = threadImpl->sleepTokenCreate();
        if(sleepToken) {
            threadImpl->sleep(sleepToken, 100);
            threadImpl->sleepTokenDestroy(sleepToken);
        }
        if(taskData->threadToken != zfnull) {
            threadImpl->executeInNewThreadCleanup(taskData->threadToken);
            taskData->threadToken = zfnull;
        }

        ZFLISTENER_1(mainThreadRunnable
                , _ZFP_ZFThreadImpl_default_TaskData *, taskData
                ) {
            if(taskData->mainThreadToken != zfnull) {
                ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(taskData->mainThreadToken);
                taskData->mainThreadToken = zfnull;
            }

            ZFFrameworkCleanup();
            zfdelete(taskData);
            exit(0);
        } ZFLISTENER_END()
        taskData->mainThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThread(mainThreadRunnable);
    } ZFLISTENER_END()

    taskData->threadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThread(runnable, zfnull);
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFApp)::appRestart(void) {
    _ZFP_ZFThreadImpl_default_TaskData *taskData = zfnew(_ZFP_ZFThreadImpl_default_TaskData);

    ZFLISTENER_1(runnable
            , _ZFP_ZFThreadImpl_default_TaskData *, taskData
            ) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFThread) *threadImpl = ZFPROTOCOL_ACCESS(ZFThread);
        void *sleepToken = threadImpl->sleepTokenCreate();
        if(sleepToken) {
            threadImpl->sleep(sleepToken, 100);
            threadImpl->sleepTokenDestroy(sleepToken);
        }
        if(taskData->threadToken != zfnull) {
            threadImpl->executeInNewThreadCleanup(taskData->threadToken);
            taskData->threadToken = zfnull;
        }

        ZFLISTENER_1(mainThreadRunnable
                , _ZFP_ZFThreadImpl_default_TaskData *, taskData
                ) {
            if(taskData->mainThreadToken != zfnull) {
                ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(taskData->mainThreadToken);
                taskData->mainThreadToken = zfnull;
            }

            ZFCoreArray<zfstring> appParamsSaved;
            appParamsSaved.copyFrom(ZFApp::appParams());
            ZFFrameworkCleanup();
            ZFFrameworkInit();
            ZFMainExecute(appParamsSaved);

            zfdelete(taskData);
        } ZFLISTENER_END()
        taskData->mainThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThread(mainThreadRunnable);
    } ZFLISTENER_END()

    taskData->threadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThread(runnable, zfnull);
}

ZF_NAMESPACE_GLOBAL_END

