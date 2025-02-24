#include "ZFState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFState)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFState, instance, ZFLevelZFFrameworkEssential)

zfclassNotPOD _ZFP_ZFStateData {
public:
    zftimet cacheTime;
    zfstring key;
    zfstring value;
};

zfclassNotPOD _ZFP_ZFStatePrivate {
public:
    ZFCoreArray<ZFListener> *loadQueue; // null when ready
    ZFCoreOrderMap pending; // <key, _ZFP_ZFStateData>
    ZFCoreOrderMap m; // <key, _ZFP_ZFStateData>
    zfautoT<ZFTaskId> taskId; // taskId for load or save
    zfautoT<ZFTimer> delayId; // taskId for delay save

public:
    _ZFP_ZFStatePrivate(void)
    : loadQueue(zfpoolNew(ZFCoreArray<ZFListener>))
    , pending()
    , m()
    , taskId()
    , delayId()
    {
    }
    ~_ZFP_ZFStatePrivate(void) {
        this->taskCleanup();
        if(this->loadQueue) {
            zfpoolDelete(this->loadQueue);
        }
    }

public:
    zfbool trim(ZF_IN ZFState *owner) {
        zfsynchronize(owner);
        if(this->m.isEmpty()) {
            return zffalse;
        }
        zftimet curTime = ZFTime::currentTime();
        zfbool changed = zffalse;
        for(zfiter it = this->m.iter(); it; ++it) {
            _ZFP_ZFStateData *data = this->m.iterValue<_ZFP_ZFStateData *>(it);
            if(this->m.count() <= owner->cacheSize()
                    && curTime - data->cacheTime <= owner->cacheTime()
                    ) {
                break;
            }
            this->m.iterRemove(it);
            changed = zftrue;
        }
        return changed;
    }
    void loadCheck(ZF_IN ZFState *owner) {
        if(this->taskId || this->loadQueue == zfnull) {
            return;
        }
        ZFLISTENER_1(loadAsync
                , zfautoT<ZFState>, owner
                ) {
            if(!zfargs.param0()) {return;}
            ZFCoreOrderMap *mNew = zfpoolNew(ZFCoreOrderMap);
            zfobj<ZFValueHolder> holder((void *)mNew, ZFValueHolderTypePoolObject(ZFCoreOrderMap *));
            _loadAsyncImpl(*mNew, owner->stateFileFixed(), zfargs);
            zfargs.result(holder);
        } ZFLISTENER_END()
        ZFLISTENER_1(loadFinish
                , zfautoT<ZFState>, owner
                ) {
            ZFValueHolder *holder = zfargs.param0();
            ZFCoreOrderMap &mNew = holder->holdedDataRef<ZFCoreOrderMap &>();
            _ZFP_ZFStatePrivate *d = owner->d;
            {
                zfsynchronize(owner);
                d->m.swap(mNew);
            }
            zfbool changed = d->_resolvePending(owner);
            changed |= d->trim(owner);
            while(!d->loadQueue->isEmpty()) {
                ZFListener callback = d->loadQueue->removeAndGet(0);
                callback.execute();
            }
            zfpoolDelete(d->loadQueue);
            d->loadQueue = zfnull;
            d->taskId = zfnull;

            if(changed) {
                d->saveCheck(owner);
            }
        } ZFLISTENER_END()
        this->taskId = zfasync(loadAsync, loadFinish);
    }
    void saveCheck(ZF_IN ZFState *owner) {
        if(this->taskId || this->loadQueue != zfnull || this->delayId) {
            return;
        }
        ZFLISTENER_1(onSaveDelay
                , zfautoT<ZFState>, owner
                ) {
            _ZFP_ZFStatePrivate *d = owner->d;
            d->delayId = zfnull;
            ZFLISTENER_1(saveAsync
                    , zfautoT<ZFState>, owner
                    ) {
                _ZFP_ZFStatePrivate *d = owner->d;
                zfsynchronizeLock(owner);
                ZFCoreArray<ZFCorePointerForPoolObject<_ZFP_ZFStateData *> > l;
                for(zfiter it = d->m.iter(); it; ++it) {
                    l.add(*(const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> *)d->m.iterValue(it));
                }
                zfsynchronizeUnlock(owner);
                _saveAsyncImpl(l, owner->stateFileFixed(), zfargs);
            } ZFLISTENER_END()
            ZFLISTENER_1(saveFinish
                    , zfautoT<ZFState>, owner
                    ) {
                _ZFP_ZFStatePrivate *d = owner->d;
                d->taskId = zfnull;
            } ZFLISTENER_END()
            d->taskId = zfasync(saveAsync, saveFinish);
        } ZFLISTENER_END()
        this->delayId = ZFTimerOnce(3000, onSaveDelay);
    }
    void taskCleanup(void) {
        if(this->delayId) {
            this->delayId->stop();
            this->delayId = zfnull;
        }
        if(this->taskId) {
            this->taskId->stop();
            this->taskId = zfnull;
        }
    }

private:
    zfbool _resolvePending(ZF_IN ZFState *owner) {
        if(this->pending.isEmpty()) {
            return zffalse;
        }
        zfsynchronize(owner);
        for(zfiter it = this->pending.iter(); it; ++it) {
            const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> &item = *(const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> *)this->pending.iterValue(it);
            this->m.set(item->key, item);
        }
        this->pending.removeAll();
        return zftrue;
    }
    // file format:
    //     cacheTime:encode(key):encode(value)
    static void _loadAsyncImpl(
            ZF_OUT ZFCoreOrderMap &mNew
            , ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN const ZFArgs &zfargs
            ) {
        if(!zfargs.param0()) {return;}
        ZFInput input = ZFInputForPathInfo(pathInfo);
        if(!input) {return;}
        zfstring line;
        ZFCoreArray<ZFIndexRange> pos;
        for( ;
                ZFInputReadLine(line, input) != zfindexMax() && zfargs.param0();
                line.removeAll(), pos.removeAll()
                ) {
            zfstringSplitIndexT(pos, line, ":");
            if(pos.count() != 3) {
                continue;
            }
            ZFCorePointerForPoolObject<_ZFP_ZFStateData *> data = zfpoolNew(_ZFP_ZFStateData);
            if(!zftimetFromStringT(data->cacheTime, line + pos[0].start, pos[0].count)) {
                continue;
            }
            ZFCoreDataDecode(data->key, line + pos[1].start, pos[1].count);
            if(data->key.isEmpty()) {
                continue;
            }
            ZFCoreDataDecode(data->value, line + pos[2].start, pos[2].count);
            if(data->value.isEmpty()) {
                continue;
            }
            mNew.set(data->key, data);
        }
    }
    static void _saveAsyncImpl(
            ZF_IN const ZFCoreArray<ZFCorePointerForPoolObject<_ZFP_ZFStateData *> > &l
            , ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN const ZFArgs &zfargs
            ) {
        if(!zfargs.param0()) {return;}
        ZFOutput output = ZFOutputForPathInfo(pathInfo);
        if(!output) {return;}
        zfstring line;
        for(zfindex i = 0; i < l.count(); ++i) {
            const _ZFP_ZFStateData &data = *((l[i]).pointerValue());
            zftimetToStringT(line, data.cacheTime);
            line += ":";
            ZFCoreDataEncode(line, data.key);
            line += ":";
            ZFCoreDataEncode(line, data.value);
            output << line;
        }
    }
};

void ZFState::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFStatePrivate);
}
void ZFState::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFState, ZFPathInfo, stateFile) {
    if(propertyValue != propertyValueOld) {
        d->saveCheck(this);
    }
}
ZFMETHOD_DEFINE_0(ZFState, ZFPathInfo, stateFileFixed) {
    if(this->stateFile().isEmpty()) {
        return ZFPathInfo(ZFPathType_settingPath(), "ZFState");
    }
    else {
        return this->stateFile();
    }
}

ZFMETHOD_DEFINE_0(ZFState, zfbool, ready) {
    return d->loadQueue == zfnull;
}

zfclass _ZFP_I_ZFStateLoadTaskId : zfextend ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_ZFStateLoadTaskId, ZFTaskId)
public:
    ZFListener callback;
public:
    virtual void stop(void) {
        this->callback = zfnull;
        zfsuper::stop();
    }
};
ZFMETHOD_DEFINE_1(ZFState, zfautoT<ZFTaskId>, load
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(!callback) {
        d->loadCheck(this);
        return zfnull;
    }
    else if(this->ready()) {
        callback.execute();
        return zfnull;
    }

    zfobj<_ZFP_I_ZFStateLoadTaskId> taskId;
    taskId->callback = callback;
    zfself *owner = this;
    ZFLISTENER_2(callbackWrap
            , zfweakT<zfself>, owner
            , zfautoT<_ZFP_I_ZFStateLoadTaskId>, taskId
            ) {
        if(taskId->callback) {
            taskId->callback.execute(ZFArgs()
                    .sender(owner)
                    );
            taskId->callback = zfnull;
        }
    } ZFLISTENER_END()
    d->loadQueue->add(callbackWrap);
    d->loadCheck(this);
    return taskId;
}

ZFMETHOD_DEFINE_2(ZFState, void, set
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        ) {
    if(!key) {
        return;
    }
    if(this->ready()) {
        {
            zfsynchronize(this);
            _ZFP_ZFStateData *data = zfpoolNew(_ZFP_ZFStateData);
            data->cacheTime = ZFTime::currentTime();
            data->key = key;
            data->value = value;
            d->m.set(key, ZFCorePointerForPoolObject<_ZFP_ZFStateData *>(data));
        }
        d->trim(this);
        d->saveCheck(this);
    }
    else {
        {
            zfsynchronize(this);
            _ZFP_ZFStateData *exist = d->pending.update<_ZFP_ZFStateData *>(key);
            if(exist != zfnull) {
                exist->cacheTime = ZFTime::currentTime();
            }
            else {
                ZFCorePointerForPoolObject<_ZFP_ZFStateData *> data = zfpoolNew(_ZFP_ZFStateData);
                data->cacheTime = ZFTime::currentTime();
                data->key = key;
                data->value = value;
                d->pending.set(key, data);
            }
        }
        d->loadCheck(this);
    }
}
ZFMETHOD_DEFINE_1(ZFState, void, remove
        , ZFMP_IN(const zfstring &, key)
        ) {
    return this->set(key, zfnull);
}
ZFMETHOD_DEFINE_0(ZFState, void, removeAll) {
    d->taskCleanup();

    {
        zfsynchronize(this);
        d->pending.removeAll();
        d->m.removeAll();
    }

    d->saveCheck(this);
}

ZFMETHOD_DEFINE_1(ZFState, zfstring, get
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(!key) {
        return zfnull;
    }
    zfsynchronize(this);
    const _ZFP_ZFStateData *data = d->m.get<const _ZFP_ZFStateData *>(key);
    if(data != zfnull) {
        return data->value;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFState, zfautoT<ZFTaskId>, getAsync
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(!key) {
        callback.execute(ZFArgs()
                .sender(this)
                .param0(zfnull)
                );
        return zfnull;
    }
    if(this->ready()) {
        callback.execute(ZFArgs()
                .sender(this)
                .param0(zfobj<v_zfstring>(this->get(key)))
                );
        return zfnull;
    }
    zfweakT<ZFState> weakSelf = this;
    ZFLISTENER_3(action
            , zfweakT<ZFState>, weakSelf
            , zfstring, key
            , ZFListener, callback
            ) {
        callback.execute(ZFArgs()
                .sender(weakSelf)
                .param0(zfobj<v_zfstring>(weakSelf->get(key)))
                );
    } ZFLISTENER_END()
    return this->load(action);
}

ZFMETHOD_DEFINE_0(ZFState, ZFCoreArray<zfstring>, allKey) {
    zfsynchronize(this);
    ZFCoreArray<zfstring> ret;
    for(zfiter it = d->m.iter(); it; ++it) {
        ret.add(d->m.iterKey(it));
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

