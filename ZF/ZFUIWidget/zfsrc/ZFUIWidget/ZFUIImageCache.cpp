#include "ZFUIImageCache.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIImageCache)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFUIImageCache, instance, ZFLevelZFFrameworkNormal)

ZFMETHOD_DEFINE_0(ZFUIImageCache, ZFPathInfo, localCachePathInfoFixed) {
    return this->localCachePathInfo()
        ? this->localCachePathInfo()
        : ZFPathInfo(ZFPathType_cachePath(), "ZFUIImageCache")
        ;
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIImageCache, zfindex, cacheMaxSize) {
    propertyValue = 20;
}

// ============================================================
zfclassLikePOD _ZFP_ZFUIImageCacheData {
public:
    ZFPathInfo pathInfo;
    zftimet cacheTime;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageLoadTaskMap, ZFLevelZFFrameworkEssential) {
    this->localCacheReady = zffalse;
}
public:
    zfobj<ZFMap> taskMap; // <v_zfstring(callbackId), _ZFP_I_ZFUIImageLoadTask>
    ZFCoreArray<_ZFP_ZFUIImageCacheData> localCacheList;
    zfbool localCacheReady;
    ZFCoreArray<ZFListener> localCacheCallback;
public:
    void localCacheLoad(void) {
        zfself *d = this;
        ZFLISTENER_1(run
                , zfself *, d
                ) {
            ZFPathInfo pathInfo = ZFUIImageCache::instance()->localCachePathInfoFixed();
            const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType);
            if(impl == zfnull) {
                return;
            }
            zftimet cacheTimeMax = ZFUIImageCache::instance()->localCacheTime();
            zftimet curTime = ZFTime::currentTime();
            ZFFileFindData fd;
            zfobj<ZFRegExp> pattern("^[0-9a-fA-F]{32}_[0-9]+$");
            d->localCacheList.removeAll();
            if(impl->callbackFindFirst(fd, pathInfo.pathData)) {
                do {
                    zfstring childPath;
                    if(!impl->callbackToChild(pathInfo.pathData, childPath, fd.fileName())) {
                        continue;
                    }
                    if(ZFRegExpFind(fd.fileName(), pattern) == ZFIndexRangeZero()) {
                        impl->callbackRemove(childPath, zffalse, zftrue, zfnull);
                        continue;
                    }
                    _ZFP_ZFUIImageCacheData cacheData;
                    zftimetFromStringT(cacheData.cacheTime, childPath.cString() + 32 + 1);
                    if(curTime - cacheData.cacheTime >= cacheTimeMax) {
                        impl->callbackRemove(childPath, zffalse, zftrue, zfnull);
                        continue;
                    }
                    cacheData.pathInfo.pathType = pathInfo.pathType;
                    cacheData.pathInfo.pathData = childPath;
                    d->localCacheList.add(cacheData);
                } while(impl->callbackFindNext(fd));
                impl->callbackFindClose(fd);
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
ZF_GLOBAL_INITIALIZER_END(ZFUIImageLoadTaskMap)

zfclass _ZFP_I_ZFUIImageLoadTask : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIImageLoadTask, ZFObject)

public:
    ZFInput src;
    ZFCoreArray<ZFListener> callbackList;

public:
    void load(void) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap);
        _callbackId = this->src.callbackId();
        if(_callbackId) {
            d->taskMap->set(
                    zfobj<v_zfstring>(_callbackId)
                    , this
                    );
        }

        zfRetain(this);

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
            zfCoreMutexLock();
            d->localCacheCallback.add(_localCacheCallback);
            zfCoreMutexUnlock();
            d->localCacheLoad();
        }
    }
    void loadCancel(void) {
        if(_localCacheCallback) {
            zfCoreMutexLocker();
            ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap);
            d->localCacheCallback.removeElement(_localCacheCallback);
            _localCacheCallback = zfnull;
        }

        zfasyncCancel(_implTask);
        _implTask = zfnull;
        _result = zfnull;
        _resultType = ZFResultType::e_Cancel;
        _implFinishAction();
    }
private:
    zfstring _callbackId;
    zfauto _implTask;
    zfautoT<ZFUIImage> _result;
    ZFResultTypeEnum _resultType;
    ZFListener _localCacheCallback;
public:
    void _loadAction(void) {
        _implTask = zfasync(
                ZFListener(this, ZFMethodAccess(zfself, _implRun))
                , ZFListener(this, ZFMethodAccess(zfself, _implFinish))
                );
    }
    void _implFinishAction(void) {
        if(_callbackId) {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap)->taskMap->remove(
                    zfobj<v_zfstring>(_callbackId)
                    );
        }
        ZFArgs zfargsTmp;
        zfargsTmp.param0(_result);
        zfargsTmp.param1(zfobj<ZFResultType>(_resultType));
        for(zfindex i = 0; i < this->callbackList.count(); ++i) {
            this->callbackList[i].execute(zfargsTmp);
        }
        this->src = zfnull;
        this->callbackList.removeAll();
        _implTask = zfnull;
        _result = zfnull;

        zfRelease(this);
    }
    ZFMETHOD_INLINE_1(void, _implRun
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(!_implTask) {
            return;
        }
        const ZFPathInfo *srcPathInfo = this->src.pathInfo();
        zfbool localCache = (zftrue
                && srcPathInfo != zfnull
                && !ZFUIImageCache::instance()->localCacheExclude()->isContain(zfobj<v_zfstring>(srcPathInfo->pathType))
                );
        if(_callbackId == zfnull || !localCache) {
            _result = ZFUIImageFromInput(this->src);
            _resultType = (_result != zfnull ? ZFResultType::e_Success : ZFResultType::e_Fail);
            return;
        }
        if(_implTask == zfnull) {
            return;
        }
        zfstring localName = ZFMd5(_callbackId);
        zftimet curTime = ZFTime::currentTime();
        ZFPathInfo localPathInfo = ZFUIImageCache::instance()->localCachePathInfoFixed();
        ZFPathInfoToChild(localPathInfo, zfstr("%s_%s", ZFMd5(_callbackId), curTime));

        // try load from local cache file
        {
            ZFInput cacheSrc = ZFInputForPathInfo(localPathInfo);
            if(cacheSrc) {
                _result = ZFUIImageFromInput(cacheSrc);
                _resultType = (_result != zfnull ? ZFResultType::e_Success : ZFResultType::e_Fail);
                if(_result) {
                    return;
                }
                else {
                    // cache file invalid, remove it
                    ZFPathInfoRemove(localPathInfo);
                }
            }
        }

        // read src and write to local cache file
        ZFPathInfo tmpPathInfo = ZFUIImageCache::instance()->localCachePathInfoFixed();
        ZFPathInfoToChild(tmpPathInfo, zfstr(".%s", ZFMd5(_callbackId)));
        {
            ZFOutput tmpDst = ZFOutputForPathInfo(tmpPathInfo);
            zfbool success = zffalse;
            if(tmpDst) {
                const zfindex sizeToRead = 1024;
                do {
                    if(ZFInputRead(tmpDst, this->src, sizeToRead) != sizeToRead) {
                        success = zftrue;
                        break;
                    }
                } while(_implTask != zfnull);
            }
            if(!success) {
                tmpDst = zfnull;
                ZFPathInfoRemove(tmpPathInfo);
                return;
            }
        }

        // local from cache
        {
            ZFInput cacheSrc = ZFInputForPathInfo(tmpPathInfo);
            _result = ZFUIImageFromInput(cacheSrc);
            cacheSrc = zfnull;
            _resultType = (_result != zfnull ? ZFResultType::e_Success : ZFResultType::e_Fail);
        }
        if(!_result) {
            return;
        }

        // load success, move to cache
        if(!ZFPathInfoMove(tmpPathInfo, localPathInfo.pathData)) {
            ZFPathInfoCopy(tmpPathInfo, localPathInfo);
            ZFPathInfoRemove(tmpPathInfo);
        }

        // add cache and limit cache size
        {
            ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap);
            zfCoreMutexLock();
            _ZFP_ZFUIImageCacheData cacheData;
            cacheData.pathInfo = localPathInfo;
            cacheData.cacheTime = curTime;
            d->localCacheList.add(cacheData);
            while(d->localCacheList.count() > ZFUIImageCache::instance()->localCacheMaxSize()) {
                _ZFP_ZFUIImageCacheData toRemove = d->localCacheList.removeAndGet(0);
                zfCoreMutexUnlock();
                ZFPathInfoRemove(toRemove.pathInfo);
                zfCoreMutexLock();
            }
            zfCoreMutexUnlock();
        }
    }
    ZFMETHOD_INLINE_1(void, _implFinish
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        if(_result && _callbackId != zfnull) {
            ZFUIImageCache::instance()->cacheRemove(_callbackId);
            ZFUIImageCache::instance()->cacheAdd(_callbackId, _result);
        }
        _implFinishAction();
    }
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageLoadTaskAutoClean, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIImageLoadTaskAutoClean) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap);
    while(!d->taskMap->isEmpty()) {
        zfautoT<_ZFP_I_ZFUIImageLoadTask> task = d->taskMap->iterValue(d->taskMap->iter());
        task->loadCancel();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIImageLoadTaskAutoClean)

zfclass _ZFP_I_ZFUIImageLoadTaskId : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIImageLoadTaskId, ZFObject)
public:
    zfautoT<_ZFP_I_ZFUIImageLoadTask> owner;
    ZFListener callback;
};

ZFMETHOD_FUNC_DEFINE_2(zfauto, ZFUIImageLoad
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(!src) {
        if(callback) {
            callback.execute(ZFArgs()
                    .param0(zfnull)
                    .param1(zfobj<ZFResultType>(ZFResultType::e_Fail))
                    );
        }
        return zfnull;
    }
    if(src.callbackId()) {
        zfauto ret = ZFUIImageCache::instance()->cacheCheck(src.callbackId());
        if(ret) {
            if(callback) {
                callback.execute(ZFArgs()
                        .param0(ret)
                        .param1(zfobj<ZFResultType>(ZFResultType::e_Success))
                        );
            }
            return zfnull;
        }
    }

    zfobj<_ZFP_I_ZFUIImageLoadTaskId> taskId;
    taskId->callback = callback;
    if(src.callbackId()) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageLoadTaskMap) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageLoadTaskMap);
        taskId->owner = d->taskMap->get(zfobj<v_zfstring>(src.callbackId()));
    }
    if(taskId->owner == zfnull) {
        zfobj<_ZFP_I_ZFUIImageLoadTask> task;
        task->src = src;
        if(callback) {
            task->callbackList.add(callback);
        }
        taskId->owner = task;
        task->load();
    }
    return taskId;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIImageLoadCancel
        , ZFMP_IN(ZFObject *, taskId)
        ) {
    _ZFP_I_ZFUIImageLoadTaskId *taskIdTmp = zfcast(_ZFP_I_ZFUIImageLoadTaskId *, taskId);
    if(taskIdTmp) {
        if(taskIdTmp->owner) {
            taskIdTmp->owner->callbackList.removeElement(taskIdTmp->callback);
        }
        taskIdTmp->owner = zfnull;
        taskIdTmp->callback = zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

