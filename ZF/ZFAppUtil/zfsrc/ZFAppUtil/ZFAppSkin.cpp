#include "ZFAppSkin.h"
#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAppSkinDataHolder, ZFLevelZFFrameworkNormal) {
}
public:
    zfstring skinIdCur;
public:
    zfautoT<ZFAppSkinData> dataForSkin(ZF_IN const zfstring &skinId) {
        zfautoT<ZFArray> dataList = this->dataList();
        for(zfindex i = 0; i < dataList->count(); ++i) {
            zfautoT<ZFAppSkinData> data = dataList->get(i);
            if(data->skinName() == skinId) {
                return data;
            }
        }
        return zfnull;
    }
    zfautoT<ZFArray> dataList(void) {
        if(!_dataList) {
            _init();
        }
        return _dataList;
    }
private:
    zfautoT<ZFArray> _dataList; // ZFAppSkinData
private:
    void _init(void) {
        _dataList = zfobj<ZFArray>();
        zfself *owner = this;
        ZFLISTENER_1(impl
                , zfself *, owner
                ) {
            v_ZFIOFindData *fd = zfargs.sender();
            v_ZFPathInfo *pathInfo = zfargs.param0();
            zfstring skinId = ZFPathOfWithoutAllExt(fd->zfv.name());
            if(skinId == "config") {
                ZFStyleLoad(pathInfo->zfv);
                return;
            }
            zfautoT<ZFAppSkinData> data = owner->_attach(skinId);
            data->resList->add(pathInfo);
        } ZFLISTENER_END()
        ZFIOForEach(ZFPathInfo(ZFPathType_res(), "skin"), impl, zffalse);
    }
    zfautoT<ZFAppSkinData> _attach(ZF_IN const zfstring &skinId) {
        zfautoT<ZFAppSkinData> data = this->dataForSkin(skinId);
        if(data) {
            return data;
        }
        else {
            data = zfobj<ZFAppSkinData>();
            data->skinId(skinId);
            _dataList->add(data);
            return data;
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFAppSkinDataHolder)

ZFOBJECT_REGISTER(ZFAppSkinData)
ZFMETHOD_DEFINE_0(ZFAppSkinData, zfstring, skinName) {
    v_zfstring *ret = ZFStyleGet(zfstr("skin_%s", this->skinId()));
    if(ret && ret->zfv) {
        return ret->zfv;
    }
    else {
        return this->skinId();
    }
}

ZFMETHOD_FUNC_DEFINE_0(zfautoT<ZFArray>, ZFAppSkinDataList) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder)->dataList();
}

static zfautoT<ZFTaskId> _ZFP_ZFAppSkinLoad(
        ZF_IN const zfstring &skinId
        , ZF_IN const ZFListener &finishCallback
        , ZF_IN zfbool forceLoad
        ) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAppSkinDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder);
    if(skinId == "default" || skinId == zfnull) {
        if(!forceLoad && d->skinIdCur == zfnull) {
            finishCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                    );
            return zfnull;
        }
        d->skinIdCur = zfnull;
    }
    else {
        if(!forceLoad && d->skinIdCur == skinId) {
            finishCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                    );
            return zfnull;
        }
        d->skinIdCur = skinId;
    }
    ZFState::instance()->set("ZFAppSkin", d->skinIdCur);

    ZFStyleUpdateBlock();
    zfautoT<ZFAppSkinData> data;
    zfobj<ZFArray> taskList;

    data = d->dataForSkin("default");
    if(data) {
        taskList->addFrom(data->resList);
    }

    if(skinId != "default") {
        data = d->dataForSkin(skinId);
        if(data) {
            taskList->addFrom(data->resList);
        }
    }
    zfobj<ZFStyleLoadAsyncTask> task(taskList);
    ZFLISTENER_1(onStop
            , ZFListener, finishCallback
            ) {
        ZFTask *task = zfargs.sender();
        finishCallback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(task->resultType()))
                );
    } ZFLISTENER_END()
    task->start(onStop);
    if(task->started()) {
        return task;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFTaskId>, ZFAppSkinInit
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder)->dataList();
    return _ZFP_ZFAppSkinLoad(ZFState::instance()->get("ZFAppSkin"), finishCallback, zftrue);
}
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFAppSkinLoad
        , ZFMP_IN(const zfstring &, skinId)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    return _ZFP_ZFAppSkinLoad(skinId, finishCallback, zffalse);
}
ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFAppSkinCur) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder)->skinIdCur;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(zfautoT<ZFTask>, ZFAppSkinInitTask) {
    zfwrap implTaskId;
    ZFLISTENER_1(onStart
            , zfwrap, implTaskId
            ) {
        zfweakT<ZFTask> ownerTask = zfargs.sender();
        ZFLISTENER_2(implOnStop
                , zfweakT<ZFTask>, ownerTask
                , zfwrap, implTaskId
                ) {
            ownerTask->notifySuccess();
            implTaskId = zfnull;
        } ZFLISTENER_END()
        implTaskId = ZFAppSkinInit(implOnStop);
    } ZFLISTENER_END()
    ZFLISTENER_1(onStop
            , zfwrap, implTaskId
            ) {
        if(implTaskId) {
            implTaskId.to<ZFTask *>()->stop();
        }
    } ZFLISTENER_END()
    zfobj<ZFTask> task;
    task->observerAdd(ZFTask::E_TaskOnStart(), onStart);
    task->observerAdd(ZFTask::E_TaskOnStart(), onStop);
    return task;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFTask>, ZFAppSkinLoadTask
        , ZFMP_IN(const zfstring &, langId)
        ) {
    zfwrap implTaskId;
    ZFLISTENER_2(onStart
            , zfwrap, implTaskId
            , zfstring, langId
            ) {
        zfweakT<ZFTask> ownerTask = zfargs.sender();
        ZFLISTENER_2(implOnStop
                , zfweakT<ZFTask>, ownerTask
                , zfwrap, implTaskId
                ) {
            ownerTask->notifySuccess();
            implTaskId = zfnull;
        } ZFLISTENER_END()
        implTaskId = ZFAppSkinLoad(langId, implOnStop);
    } ZFLISTENER_END()
    ZFLISTENER_1(onStop
            , zfwrap, implTaskId
            ) {
        if(implTaskId) {
            implTaskId.to<ZFTask *>()->stop();
        }
    } ZFLISTENER_END()
    zfobj<ZFTask> task;
    task->observerAdd(ZFTask::E_TaskOnStart(), onStart);
    task->observerAdd(ZFTask::E_TaskOnStart(), onStop);
    return task;
}

ZF_NAMESPACE_GLOBAL_END

