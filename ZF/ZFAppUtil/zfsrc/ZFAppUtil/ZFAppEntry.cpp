#include "ZFAppEntry.h"
#include "ZFAppRes.h"
#include "ZFAppLang.h"
#include "ZFAppSkin.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFAppEntry)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadState)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadEntry)
ZFEVENT_REGISTER(ZFAppEntry, OnLoadStop)

ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadState
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAdd(zfself::E_OnLoadState(), callback);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadEntry
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAdd(zfself::E_OnLoadEntry(), callback);
}
ZFMETHOD_DEFINE_1(ZFAppEntry, void, onLoadStop
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    this->observerAdd(zfself::E_OnLoadStop(), callback);
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
ZFMETHOD_DEFINE_1(ZFAppEntry, void, task
        , ZFMP_IN(ZFTask *, task)
        ) {
    if(!task) {
        return;
    }
    zfautoT<ZFArray> holder = this->objectTag("ZFAppEntryCustomTask");
    if(!holder) {
        holder = zfobj<ZFArray>();
        this->objectTag("ZFAppEntryCustomTask", holder);
    }
    holder->add(task);
}

// ============================================================
static void _ZFP_ZFAppEntry_step(
        ZF_IN ZFTaskQueue *task
        , ZF_IN ZFAppEntry *owner
        , ZF_IN zfidentity eventId
        ) {
    task->child(zfobj<ZFWaitTask>());

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
        _ZFP_ZFAppEntry_step(task, owner, zfself::E_OnLoadState());
        ZFLISTENER(loadImpl) {
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

    { // ZFAppLangInit
        task->child(ZFAppLangInitTask());
        task->child(zfobj<ZFWaitTask>());
    }

    { // ZFAppSkinInit
        task->child(ZFAppSkinInitTask());
        task->child(zfobj<ZFWaitTask>());
    }

    { // custom task
        zfautoT<ZFArray> holder = this->objectTag("ZFAppEntryCustomTask");
        if(holder) {
            for(zfindex i = 0; i < holder->count(); ++i) {
                task->child(holder->get(i));
            }
        }
    }

    { // entry
        _ZFP_ZFAppEntry_step(task, owner, zfself::E_OnLoadEntry());
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
        _ZFP_ZFAppEntry_step(task, owner, zfself::E_OnLoadStop());
    }
    ZFLISTENER_2(onStop
            , zfautoT<zfself>, owner
            , ZFListener, finishCallback
            ) {
        finishCallback.execute(ZFArgs()
                .sender(owner)
                );
        owner->objectTagRemove("ZFAppEntryCustomTask");
        owner->objectTagRemove("ZFAppEntryMainTask");
    } ZFLISTENER_END()
    task->start(onStop);
}

ZF_NAMESPACE_GLOBAL_END

