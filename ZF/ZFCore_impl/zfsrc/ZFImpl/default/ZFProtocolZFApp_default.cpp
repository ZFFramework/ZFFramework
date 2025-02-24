#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

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

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_default, ZFApp, ZFProtocolLevel::e_Default)
public:
    virtual void appRestart(void) {
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

        ZFLISTENER(runnableCleanup) {
        } ZFLISTENER_END()

        taskData->threadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThread(runnable, runnableCleanup);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_default)

ZF_NAMESPACE_GLOBAL_END

