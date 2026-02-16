#include "ZFAppEntry.h"
#include "ZFAppRes.h"
#include "ZFAppLang.h"
#include "ZFAppSkin.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFAppEntry)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadState)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadRes)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadEntry)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadStop)

// ============================================================
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadState
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAddForOnce(zfself::E_OnLoadState(), callback);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadRes
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAddForOnce(zfself::E_OnLoadRes(), callback);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadEntry
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAddForOnce(zfself::E_OnLoadEntry(), callback);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadStop
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAddForOnce(zfself::E_OnLoadStop(), callback);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadState
        , ZFMP_IN(ZFTask *, task)
        ) {
    if(task == zfnull) {
        return;
    }
    zfautoT<ZFArray> list = this->objectTag("ZFAppEntryCustomTask_State");
    if(!list) {
        list = zfobj<ZFArray>();
        this->objectTag("ZFAppEntryCustomTask_State", list);
    }
    list->add(task);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadRes
        , ZFMP_IN(ZFTask *, task)
        ) {
    if(task == zfnull) {
        return;
    }
    zfautoT<ZFArray> list = this->objectTag("ZFAppEntryCustomTask_Res");
    if(!list) {
        list = zfobj<ZFArray>();
        this->objectTag("ZFAppEntryCustomTask_Res", list);
    }
    list->add(task);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadEntry
        , ZFMP_IN(ZFTask *, task)
        ) {
    if(task == zfnull) {
        return;
    }
    zfautoT<ZFArray> list = this->objectTag("ZFAppEntryCustomTask_Entry");
    if(!list) {
        list = zfobj<ZFArray>();
        this->objectTag("ZFAppEntryCustomTask_Entry", list);
    }
    list->add(task);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadStop
        , ZFMP_IN(ZFTask *, task)
        ) {
    if(task == zfnull) {
        return;
    }
    zfautoT<ZFArray> list = this->objectTag("ZFAppEntryCustomTask_Stop");
    if(!list) {
        list = zfobj<ZFArray>();
        this->objectTag("ZFAppEntryCustomTask_Stop", list);
    }
    list->add(task);
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFAppEntry, void, res
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        ) {
    ZFResExtPathAdd(pathInfo);
}
ZFMETHOD_DEFINE_2(ZFAppEntry, void, resPack
        , ZFMP_IN(const ZFPathInfo &, packagePathInfo)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        ) {
    ZFResExtPathAdd(ZFAppRes::pathInfoForPackage(packagePathInfo, packagePwd));
}
ZFMETHOD_DEFINE_2(ZFAppEntry, void, resPack
        , ZFMP_IN(const ZFListener &, packageGetter)
        , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
        ) {
    ZFAppRes::start(packageGetter, moduleName);
}
ZFMETHOD_DEFINE_3(ZFAppEntry, void, resPack
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageInfoList)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
        ) {
    ZFAppRes::start(packageInfoList, packagePwd, moduleName);
}

// ============================================================
static void _ZFP_ZFAppEntry_step(
        ZF_IN ZFAppEntry *owner
        , ZF_IN ZFTaskQueue *task
        , ZF_IN zfidentity eventId
        , ZF_IN const zfchar *customTaskKey
        ) {
    task->child(zfobj<ZFWaitTask>());

    zfautoT<ZFArray> list = owner->objectTag(customTaskKey);
    if(list) {
        for(zfindex i = 0; i < list->count(); ++i) {
            task->child(list->get(i));
        }
    }

    ZFLISTENER_2(loadImpl
            , zfautoT<ZFAppEntry>, owner
            , zfidentity, eventId
            ) {
        ZFTask *ownerTask = zfargs.sender();
        zfobj<v_zfwrap> extTaskHolder;
        owner->observerNotify(eventId, extTaskHolder);
        ZFTask *extTask = extTaskHolder;
        if(!extTask) {
            ownerTask->notifySuccess();
            return;
        }
        ZFLISTENER_1(extTaskOnStop
                , zfautoT<ZFTask>, ownerTask
                ) {
            ownerTask->notifySuccess();
        } ZFLISTENER_END()
        extTask->start(extTaskOnStop);
    } ZFLISTENER_END()
    task->child(loadImpl);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, start
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    ZFCoreAssert(this->objectTag("ZFAppEntryMainTask") == zfnull);
    zfobj<ZFTaskQueue> task;
    this->objectTag("ZFAppEntryMainTask", task);
    zfself *owner = this;

    { // state
        _ZFP_ZFAppEntry_step(owner, task, zfself::E_OnLoadState(), "ZFAppEntryCustomTask_State");
        ZFLISTENER_1(loadImpl
                , zfweakT<zfself>, owner
                ) {
            if(owner->stateEncryptKey()) {
                ZFState::instance()->stateFile(ZFPathInfoForEncrypt(ZFState::stateFileDefault(), owner->stateEncryptKey()));
            }

            ZFTask *ownerTask = zfargs.sender();
            ZFLISTENER_1(onFinish
                    , zfautoT<ZFTask>, ownerTask
                    ) {
                ownerTask->notifySuccess();
            } ZFLISTENER_END()
            ZFState::instance()->load(onFinish);
        } ZFLISTENER_END()
        task->child(loadImpl);
    }

    { // lang and skin
        zfobj<ZFTaskQueue> wrap;
        task->child(wrap);
        ZFLISTENER(onStart) {
            ZFStyleUpdateBegin();
        } ZFLISTENER_END()
        ZFLISTENER(onStop) {
            ZFStyleUpdateEnd();
        } ZFLISTENER_END()
        wrap->observerAdd(ZFTask::E_TaskOnStart(), onStart);
        wrap->observerAdd(ZFTask::E_TaskOnStop(), onStop);

        _ZFP_ZFAppEntry_step(owner, wrap, zfself::E_OnLoadRes(), "ZFAppEntryCustomTask_Res");

        wrap->child(ZFAppLangLoadTask());
        wrap->child(zfobj<ZFWaitTask>());

        wrap->child(ZFAppSkinLoadTask());
        wrap->child(zfobj<ZFWaitTask>());
    }

    { // entry
        _ZFP_ZFAppEntry_step(owner, task, zfself::E_OnLoadEntry(), "ZFAppEntryCustomTask_Entry");
        ZFLISTENER_1(loadImpl
                , zfautoT<zfself>, owner
                ) {
            ZFTask *ownerTask = zfargs.sender();
            ZFCoreArray<ZFPathInfo> entryList = owner->entryList();
            for(zfindex i = 0; i < entryList.count(); ++i) {
                zfauto ret = zfimport(ZFPathInfoToString(entryList[i]));
                v_zfbool *ck = ret;
                if(ck && ck->zfv) {
                    break;
                }
            }
            ownerTask->notifySuccess();
        } ZFLISTENER_END()
        task->child(loadImpl);
    }

    { // stop
        _ZFP_ZFAppEntry_step(owner, task, zfself::E_OnLoadStop(), "ZFAppEntryCustomTask_Stop");
    }
    ZFLISTENER_2(onStop
            , zfautoT<zfself>, owner
            , ZFListener, finishCallback
            ) {
        finishCallback.execute(ZFArgs()
                .sender(owner)
                );
        owner->objectTagRemove("ZFAppEntryMainTask");
    } ZFLISTENER_END()
    task->start(onStop);
}

ZF_NAMESPACE_GLOBAL_END

