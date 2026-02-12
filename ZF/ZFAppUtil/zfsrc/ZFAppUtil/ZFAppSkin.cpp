#include "ZFAppSkin.h"
#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAppSkinDataHolder, ZFLevelZFFrameworkNormal) {
}
public:
    zfautoT<ZFAppSkinData> data(
            ZF_IN const zfstring &skinId
            , ZF_IN ZFArray *dataList
            ) {
        for(zfindex i = 0; i < dataList->count(); ++i) {
            ZFAppSkinData *data = dataList->get(i);
            if(data->skinId() == skinId) {
                return data;
            }
        }
        return zfnull;
    }
    zfautoT<ZFArray> dataList(ZF_IN zfbool enableCache) {
        if(enableCache && _cache) {
            return _cache;
        }
        _cache = _load();
        return _cache;
    }
private:
    zfautoT<ZFArray> _cache; // ZFAppSkinData
private:
    zfautoT<ZFArray> _load(void) {
        ZFStyleUpdateBlock();
        zfobj<ZFArray> dataList;
        ZFLISTENER_1(impl
                , zfautoT<ZFArray>, dataList
                ) {
            v_ZFIOFindData *fd = zfargs.sender();
            v_ZFPathInfo *pathInfo = zfargs.param0();
            zfstring skinId = ZFPathOfWithoutAllExt(fd->zfv.name());
            if(!skinId) {
                return;
            }
            else if(skinId == "config") {
                ZFStyleLoad(pathInfo->zfv);
                return;
            }
            zfautoT<ZFAppSkinData> data;
            for(zfindex i = 0; i < dataList->count(); ++i) {
                ZFAppSkinData *tmp = dataList->get(i);
                if(tmp->skinId() == skinId) {
                    data = tmp;
                    break;
                }
            }
            if(data == zfnull) {
                data = zfobj<ZFAppSkinData>();
                dataList->add(data);
                data->skinId(skinId);
            }
            data->resList->add(pathInfo);
        } ZFLISTENER_END()
        ZFIOForEach(ZFPathInfo(ZFPathType_res(), "skin"), impl, zffalse);
        return dataList;
    }
ZF_GLOBAL_INITIALIZER_END(ZFAppSkinDataHolder)

ZFOBJECT_REGISTER(ZFAppSkinData)
ZFMETHOD_DEFINE_0(ZFAppSkinData, zfstring, skinName) {
    v_zfstring *ret = ZFStyleGet(zfstr("skinName_%s", this->skinId()));
    if(ret && ret->zfv) {
        return ret->zfv;
    }
    else {
        return this->skinId();
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFArray>, ZFAppSkinList
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder)->dataList(enableCache);
}

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFAppSkinLoad
        , ZFMP_IN_OPT(const zfstring &, skinId, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    zfstring skinIdCur;
    if(skinId == zfnull) {
        skinIdCur = ZFAppSkin();
    }
    else {
        skinIdCur = skinId;
    }
    ZFState::instance()->set("ZFAppSkin", skinIdCur);
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAppSkinDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppSkinDataHolder);
    zfautoT<ZFArray> dataList = d->dataList(enableCache);

    ZFStyleUpdateBlock();
    zfobj<ZFArray> taskList;
    zfautoT<ZFAppSkinData> data;

    data = d->data("default", dataList);
    if(data) {
        taskList->addFrom(data->resList);
    }

    if(skinIdCur != "default") {
        data = d->data(skinIdCur, dataList);
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
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFAppSkin) {
    return ZFState::instance()->get("ZFAppSkin");
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTask>, ZFAppSkinLoadTask
        , ZFMP_IN_OPT(const zfstring &, skinId, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    zfwrap implTaskId;
    ZFLISTENER_3(onStart
            , zfwrap, implTaskId
            , zfstring, skinId
            , zfbool, enableCache
            ) {
        zfweakT<ZFTask> ownerTask = zfargs.sender();
        ZFLISTENER_2(implOnStop
                , zfweakT<ZFTask>, ownerTask
                , zfwrap, implTaskId
                ) {
            ownerTask->notifySuccess();
            implTaskId = zfnull;
        } ZFLISTENER_END()
        implTaskId = ZFAppSkinLoad(skinId, implOnStop, enableCache);
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
    task->observerAdd(ZFTask::E_TaskOnStop(), onStop);
    return task;
}

ZF_NAMESPACE_GLOBAL_END

