#include "ZFIOCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFIOCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFIOCache, instance, ZFLevelZFFrameworkNormal)

ZFMETHOD_DEFINE_0(ZFIOCache, ZFPathInfo, localCachePathInfoFixed) {
    return this->localCachePathInfo()
        ? this->localCachePathInfo()
        : ZFPathInfo(ZFPathType_cachePath(), "ZFIOCache")
        ;
}
ZFMETHOD_DEFINE_1(ZFIOCache, ZFPathInfo, localCachePathInfoForChild
        , ZFMP_IN(const zfchar *, childPath)
        ) {
    ZFPathInfo parent = this->localCachePathInfoFixed();
    zfstring pathData;
    ZFIOToChild(pathData, parent, childPath);
    return ZFPathInfo(parent.pathType(), pathData);
}

ZFPROPERTY_ON_INIT_DEFINE(ZFIOCache, zfindex, cacheMaxSize) {
    propertyValue = 20;
}

// ============================================================
zfclassLikePOD _ZFP_ZFIOCacheData {
public:
    ZFPathInfo pathInfo;
    zftimet cacheTime;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFIOCacheLoadTaskMap, ZFLevelZFFrameworkEssential) {
    this->localCacheReady = zffalse;
}
public:
    zfobj<ZFMap> taskMap; // <v_zfstring(callbackId), _ZFP_I_ZFIOCacheLoadTask>
    ZFCoreArray<_ZFP_ZFIOCacheData> localCacheList;
    zfbool localCacheReady;
    ZFCoreArray<ZFListener> localCacheCallback;
public:
    void localCacheLoad(void) {
        zfself *d = this;
        ZFLISTENER_1(run
                , zfself *, d
                ) {
            const ZFPathInfo pathInfo = ZFIOCache::instance()->localCachePathInfoFixed();
            zfautoT<ZFIOImpl> impl = ZFIOImplForPathType(pathInfo.pathType());
            if(impl == zfnull) {
                return;
            }
            zftimet cacheTimeMax = ZFIOCache::instance()->localCacheTime();
            zftimet curTime = ZFTime::currentTime();
            ZFIOFindData fd;
            zfobj<ZFRegExp> pattern("^[0-9a-fA-F]{32}_[0-9]+$");
            d->localCacheList.removeAll();
            if(impl->ioFindFirst(fd, pathInfo.pathData())) {
                do {
                    zfstring childPath;
                    impl->ioToChild(childPath, pathInfo.pathData(), fd.name());
                    if(!ZFRegExpMatch(fd.name(), pattern)) {
                        impl->ioRemove(childPath, zffalse, zftrue);
                        continue;
                    }
                    _ZFP_ZFIOCacheData cacheData;
                    zftimetFromStringT(cacheData.cacheTime, childPath + 32 + 1);
                    if(curTime - cacheData.cacheTime >= cacheTimeMax) {
                        impl->ioRemove(childPath, zffalse, zftrue);
                        continue;
                    }
                    cacheData.pathInfo.pathType(pathInfo.pathType());
                    cacheData.pathInfo.pathData(childPath);
                    d->localCacheList.add(cacheData);
                } while(impl->ioFindNext(fd));
                impl->ioFindClose(fd);
            }
        } ZFLISTENER_END()
        ZFLISTENER_1(finish
                , zfself *, d
                ) {
            d->localCacheReady = zftrue;
            for(zfindex i = 0; i < d->localCacheCallback.count(); ++i) {
                d->localCacheCallback[i].execute();
            }
            d->localCacheCallback.removeAll();
        } ZFLISTENER_END()
        zfasync(run, finish);
    }
ZF_GLOBAL_INITIALIZER_END(ZFIOCacheLoadTaskMap)

zfclass _ZFP_I_ZFIOCacheLoadTask : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOCacheLoadTask, ZFObject)

public:
    ZFInput src;
    ZFCoreArray<ZFListener> callbackList;
    ZFListener loadImpl;

public:
    void load(void) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFIOCacheLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap);
        _callbackId = this->src.callbackId();
        if(_callbackId) {
            d->taskMap->set(
                    zfobj<v_zfstring>(_callbackId)
                    , this
                    );
        }

        zfobjRetain(this);

        if(d->localCacheReady) {
            _loadAction();
        }
        else {
            zfself *owner = this;
            ZFLISTENER_1(tmp
                    , zfself *, owner
                    ) {
                owner->_loadAction();
            } ZFLISTENER_END()
            _localCacheCallback = tmp;
            ZFCoreMutexLock();
            d->localCacheCallback.add(_localCacheCallback);
            ZFCoreMutexUnlock();
            d->localCacheLoad();
        }
    }
    void loadCancel(void) {
        if(_localCacheCallback) {
            ZFCoreMutexLocker();
            ZF_GLOBAL_INITIALIZER_CLASS(ZFIOCacheLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap);
            d->localCacheCallback.removeElement(_localCacheCallback);
            _localCacheCallback = zfnull;
        }

        if(_implTask) {
            _implTask->stop();
            _implTask = zfnull;
        }
        _result = zfnull;
        _resultType = v_ZFResultType::e_Cancel;
        _implFinishAction();
    }
private:
    zfstring _callbackId;
    zfautoT<ZFTaskId> _implTask;
    zfauto _result;
    ZFResultType _resultType;
    ZFListener _localCacheCallback;
public:
    void _loadAction(void) {
        _implTask = zfasync(
                ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, _implRun))
                , ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, _implFinish))
                );
    }
    void _implFinishAction(void) {
        if(_callbackId) {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap)->taskMap->remove(
                    zfobj<v_zfstring>(_callbackId)
                    );
        }
        ZFArgs zfargsTmp;
        zfargsTmp.param0(_result);
        zfargsTmp.param1(zfobj<v_ZFResultType>(_resultType));
        for(zfindex i = 0; i < this->callbackList.count(); ++i) {
            this->callbackList[i].execute(zfargsTmp);
        }
        this->src = zfnull;
        this->callbackList.removeAll();
        this->loadImpl = zfnull;
        _implTask = zfnull;
        _result = zfnull;

        zfobjRelease(this);
    }
    void _loadImplAction(ZF_IN const ZFInput &localSrc) {
        ZFArgs zfargsLoadImpl;
        zfargsLoadImpl
            .param0(zfobj<v_ZFInput>(localSrc))
            .param1(zfobj<v_ZFInput>(this->src))
            ;
        this->loadImpl.execute(zfargsLoadImpl);
        _result = zfargsLoadImpl.result();
        _resultType = (_result != zfnull ? v_ZFResultType::e_Success : v_ZFResultType::e_Fail);
    }
    ZFMETHOD_INLINE_1(void, _implRun
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(!_implTask) {
            return;
        }
        const ZFPathInfo srcPathInfo = this->src.pathInfo();
        zfbool localCache = (zftrue
                && srcPathInfo != zfnull
                && !ZFIOCache::instance()->localCacheExclude()->isContain(zfobj<v_zfstring>(srcPathInfo.pathType()))
                );
        if(_callbackId == zfnull || !localCache) {
            _loadImplAction(this->src);
            return;
        }
        if(_implTask == zfnull) {
            return;
        }
        zfstring localName = ZFMd5(_callbackId);
        zftimet curTime = ZFTime::currentTime();
        ZFPathInfo localPathInfo = ZFIOCache::instance()->localCachePathInfoForChild(zfstr("%s_%s", ZFMd5(_callbackId), curTime));

        // try load from local cache file
        {
            ZFInput cacheSrc = ZFInputForPathInfo(localPathInfo);
            if(cacheSrc) {
                cacheSrc.callbackInfoCopy(this->src);
                _loadImplAction(cacheSrc);
                if(_result) {
                    return;
                }
                else {
                    // cache file invalid, remove it
                    ZFIORemove(localPathInfo);
                }
            }
        }

        // read src and write to local cache file
        {
            ZFOutput tmpDst = ZFOutputForPathInfo(localPathInfo);
            zfbool success = zffalse;
            if(tmpDst) {
                const zfindex sizeToRead = 1024;
                do {
                    zfindex read = ZFInputRead(tmpDst, this->src, sizeToRead);
                    if(read == zfindexMax()) {
                        break;
                    }
                    else if(read < sizeToRead) {
                        success = zftrue;
                        break;
                    }
                } while(_implTask != zfnull);
            }
            if(!success) {
                tmpDst = zfnull;
                ZFIORemove(localPathInfo);
                return;
            }
        }

        // local from cache
        {
            ZFInput cacheSrc = ZFInputForPathInfo(localPathInfo);
            if(cacheSrc) {
                cacheSrc.callbackInfoCopy(this->src);
                _loadImplAction(cacheSrc);
                cacheSrc = zfnull;
            }
        }
        if(!_result) {
            ZFIORemove(localPathInfo);
            return;
        }

        // add cache and limit cache size
        {
            ZF_GLOBAL_INITIALIZER_CLASS(ZFIOCacheLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap);
            ZFCoreMutexLock();
            _ZFP_ZFIOCacheData cacheData;
            cacheData.pathInfo = localPathInfo;
            cacheData.cacheTime = curTime;
            d->localCacheList.add(cacheData);
            while(d->localCacheList.count() > ZFIOCache::instance()->localCacheMaxSize()) {
                _ZFP_ZFIOCacheData toRemove = d->localCacheList.removeAndGet(0);
                ZFCoreMutexUnlock();
                ZFIORemove(toRemove.pathInfo);
                ZFCoreMutexLock();
            }
            ZFCoreMutexUnlock();
        }
    }
    ZFMETHOD_INLINE_1(void, _implFinish
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(_result && _callbackId != zfnull) {
            ZFIOCache::instance()->cacheRemove(_callbackId);
            ZFIOCache::instance()->cacheAdd(_callbackId, _result);
        }
        _implFinishAction();
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFIOCacheLoadTaskAutoClean, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFIOCacheLoadTaskAutoClean) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFIOCacheLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap);
    while(!d->taskMap->isEmpty()) {
        zfautoT<_ZFP_I_ZFIOCacheLoadTask> task = d->taskMap->iterValue(d->taskMap->iter());
        task->loadCancel();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFIOCacheLoadTaskAutoClean)

zfclass _ZFP_I_ZFIOCacheLoadTaskId : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOCacheLoadTaskId, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    zfautoT<_ZFP_I_ZFIOCacheLoadTask> owner;
    ZFListener callback;
public:
    zfoverride
    virtual void stop(void) {
        if(this->owner) {
            this->owner->callbackList.removeElement(this->callback);
        }
        this->owner = zfnull;
        this->callback = zfnull;
    }
};

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFIOCacheLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, loadImpl, zfnull)
        ) {
    if(!src) {
        if(callback) {
            callback.execute(ZFArgs()
                    .param0(zfnull)
                    .param1(zfobj<v_ZFResultType>(v_ZFResultType::e_Fail))
                    );
        }
        return zfnull;
    }
    if(src.callbackId()) {
        zfauto ret = ZFIOCache::instance()->cacheCheck(src.callbackId());
        if(ret) {
            if(callback) {
                callback.execute(ZFArgs()
                        .param0(ret)
                        .param1(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                        );
            }
            return zfnull;
        }
    }

    zfobj<_ZFP_I_ZFIOCacheLoadTaskId> taskId;
    taskId->callback = callback;
    if(src.callbackId()) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFIOCacheLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFIOCacheLoadTaskMap);
        taskId->owner = d->taskMap->get(zfobj<v_zfstring>(src.callbackId()));
    }
    if(taskId->owner == zfnull) {
        zfobj<_ZFP_I_ZFIOCacheLoadTask> task;
        task->src = src;
        if(loadImpl) {
            task->loadImpl = loadImpl;
        }
        else {
            ZFLISTENER_0(loadImplWrap) {
                zfargs.result(ZFObjectIOLoad(zfargs.param0().to<v_ZFInput *>()->zfv));
            } ZFLISTENER_END()
            task->loadImpl = loadImplWrap;
        }
        if(callback) {
            task->callbackList.add(callback);
        }
        taskId->owner = task;
        task->load();
    }
    else {
        taskId->owner->callbackList.add(callback);
    }
    return taskId;
}

// ============================================================
ZFOBJECT_REGISTER(ZFIOCacheLoadTask)
ZFOBJECT_ON_INIT_DEFINE_2(ZFIOCacheLoadTask
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN_OPT(const ZFListener &, loadImpl, zfnull)
        ) {
    this->input(input);
    this->loadImpl(loadImpl);
}

void ZFIOCacheLoadTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->input()) {
        zfweakT<zfself> owner = this;
        ZFLISTENER_1(implOnStop
                , zfweakT<zfself>, owner
                ) {
            owner->_implTaskId = zfnull;
            v_ZFResultType *resultType = zfargs.param1();
            if(resultType->enumValue() == v_ZFResultType::e_Success) {
                owner->notifySuccess(zfargs.param0());
            }
            else {
                owner->notifyFail(zfstr("io cache load fail: %s"
                            , owner->input()
                            ), zfargs.param0());
            }
        } ZFLISTENER_END()
        this->_implTaskId = ZFIOCacheLoad(this->input(), implOnStop, this->loadImpl());
    }
    else {
        this->notifySuccess();
    }
}
void ZFIOCacheLoadTask::taskOnStop(void) {
    if(this->_implTaskId) {
        this->_implTaskId->stop();
        this->_implTaskId = zfnull;
    }
    zfsuper::taskOnStop();
}

ZF_NAMESPACE_GLOBAL_END

