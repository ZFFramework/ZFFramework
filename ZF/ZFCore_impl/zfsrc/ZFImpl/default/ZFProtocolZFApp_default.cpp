#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFThreadImpl_default_TaskData
{
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
    virtual void appRestart(ZF_IN zftimet delay)
    {
        _ZFP_ZFThreadImpl_default_TaskData *taskData = zfnew(_ZFP_ZFThreadImpl_default_TaskData);

        ZFLISTENER_2(runnable
                , _ZFP_ZFThreadImpl_default_TaskData *, taskData
                , zftimet, delay
                ) {
            ZFPROTOCOL_ACCESS(ZFThread)->sleep(delay);
            if(taskData->threadToken != zfnull)
            {
                ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThreadCleanup(taskData->threadToken);
                taskData->threadToken = zfnull;
            }

            ZFLISTENER_1(mainThreadRunnable
                    , _ZFP_ZFThreadImpl_default_TaskData *, taskData
                    ) {
                if(taskData->mainThreadToken != zfnull)
                {
                    ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThreadCleanup(taskData->mainThreadToken);
                    taskData->mainThreadToken = zfnull;
                }

                ZFCoreArray<zfstring> appParamsSaved;
                appParamsSaved.copyFrom(ZFApp::appParams());
                ZFFrameworkCleanup();
                ZFFrameworkInit();
                ZFMainExecute(appParamsSaved);

                zfdelete(taskData);
            } ZFLISTENER_END(mainThreadRunnable)
            taskData->mainThreadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInMainThread(mainThreadRunnable, zfnull, zfnull);
        } ZFLISTENER_END(runnable)

        ZFLISTENER(runnableCleanup) {
        } ZFLISTENER_END(runnableCleanup)

        taskData->threadToken = ZFPROTOCOL_ACCESS(ZFThread)->executeInNewThread(runnable, runnableCleanup, zfnull, zfnull);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAppImpl_default)

ZF_NAMESPACE_GLOBAL_END

