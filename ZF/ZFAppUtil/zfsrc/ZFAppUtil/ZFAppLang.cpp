#include "ZFAppLang.h"
#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAppLangDataHolder, ZFLevelZFFrameworkNormal) {
}
public:
    zfautoT<ZFAppLangData> data(
            ZF_IN const zfstring &langId
            , ZF_IN ZFArray *dataList
            ) {
        for(zfindex i = 0; i < dataList->count(); ++i) {
            ZFAppLangData *data = dataList->get(i);
            if(data->langId() == langId) {
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
    zfautoT<ZFArray> _cache; // ZFAppLangData
private:
    zfautoT<ZFArray> _load(void) {
        ZFStyleUpdateBlock();
        zfobj<ZFArray> dataList;
        ZFLISTENER_1(impl
                , zfautoT<ZFArray>, dataList
                ) {
            v_ZFIOFindData *fd = zfargs.sender();
            v_ZFPathInfo *pathInfo = zfargs.param0();
            zfstring langId = ZFPathOfWithoutAllExt(fd->zfv.name());
            if(!langId) {
                return;
            }
            else if(langId == "config") {
                ZFStyleLoad(pathInfo->zfv);
                return;
            }
            zfautoT<ZFAppLangData> data;
            for(zfindex i = 0; i < dataList->count(); ++i) {
                ZFAppLangData *tmp = dataList->get(i);
                if(tmp->langId() == langId) {
                    data = tmp;
                    break;
                }
            }
            if(data == zfnull) {
                data = zfobj<ZFAppLangData>();
                dataList->add(data);
                data->langId(langId);
            }
            data->resList->add(pathInfo);
        } ZFLISTENER_END()
        ZFIOForEach(ZFPathInfo(ZFPathType_res(), "lang"), impl, zffalse);
        return dataList;
    }
ZF_GLOBAL_INITIALIZER_END(ZFAppLangDataHolder)

ZFOBJECT_REGISTER(ZFAppLangData)
ZFMETHOD_DEFINE_0(ZFAppLangData, zfstring, langName) {
    v_zfstring *ret = ZFStyleGet(zfstr("langName_%s", this->langId()));
    if(ret && ret->zfv) {
        return ret->zfv;
    }
    else {
        return this->langId();
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFArray>, ZFAppLangList
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppLangDataHolder)->dataList(enableCache);
}

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFAppLangLoad
        , ZFMP_IN_OPT(const zfstring &, langId, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    zfstring langIdCur;
    if(langId == zfnull) {
        langIdCur = ZFAppLang();
    }
    else {
        langIdCur = langId;
    }
    ZFState::instance()->set("ZFAppLang", langIdCur);
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAppLangDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppLangDataHolder);
    zfautoT<ZFArray> dataList = d->dataList(enableCache);

    ZFStyleUpdateBlock();
    zfobj<ZFArray> taskList;
    zfautoT<ZFAppLangData> data;

    data = d->data("default", dataList);
    if(data) {
        taskList->addFrom(data->resList);
    }

    if(langIdCur != "default") {
        data = d->data(langIdCur, dataList);
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
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFAppLang) {
    zfstring ret = ZFState::instance()->get("ZFAppLang");
    if(ret) {
        return ret;
    }
    else {
        return ZFEnvInfo::localeLangId();
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTask>, ZFAppLangLoadTask
        , ZFMP_IN_OPT(const zfstring &, langId, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        ) {
    zfwrap implTaskId;
    ZFLISTENER_3(onStart
            , zfwrap, implTaskId
            , zfstring, langId
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
        implTaskId = ZFAppLangLoad(langId, implOnStop, enableCache);
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

