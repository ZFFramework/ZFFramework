#include "ZFState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFCoreArray<ZFState *> _ZFP_ZFStateList;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStateAutoSave, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStateAutoSave) {
    for(zfindex i = _ZFP_ZFStateList.count() - 1; i != zfindexMax(); --i) {
        _ZFP_ZFStateList[i]->saveImmediately();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFStateAutoSave)

ZFOBJECT_REGISTER(ZFState)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFState, instance, ZFLevelZFFrameworkEssential)

ZFEVENT_REGISTER(ZFState, StateFileUpdate)

zfclassNotPOD _ZFP_ZFStateData {
public:
    zftimet expireTime; // 0 for infinite
    zfstring key;
    zfstring value;
};

zfclassNotPOD _ZFP_ZFStatePrivate {
public:
    ZFCoreArray<ZFListener> *loadQueue; // null when ready
    ZFCoreArray<ZFListener> saveQueue;
    ZFCoreMap pending; // <key, _ZFP_ZFStateData>
    ZFCoreMap m; // <key, _ZFP_ZFStateData>
    zfautoT<ZFTaskId> taskId; // taskId for load or save
    zfautoT<ZFTimer> delayId; // taskId for delay save
    zfbool changed;

public:
    _ZFP_ZFStatePrivate(void)
    : loadQueue(zfpoolNew(ZFCoreArray<ZFListener>))
    , pending()
    , m()
    , taskId()
    , delayId()
    , changed(zffalse)
    {
    }
    ~_ZFP_ZFStatePrivate(void) {
        this->taskCleanup();
        if(this->loadQueue) {
            zfpoolDelete(this->loadQueue);
        }
    }

public:
    void loadCheck(ZF_IN ZFState *owner) {
        if(this->taskId || this->loadQueue == zfnull) {
            return;
        }
        ZFLISTENER_1(loadAsync
                , zfautoT<ZFState>, owner
                ) {
            if(!zfargs.param0()) {return;}
            ZFCoreMap *mNew = zfpoolNew(ZFCoreMap);
            zfobj<ZFValueHolder> holder((void *)mNew, ZFValueHolderTypePoolObject(ZFCoreMap *));
            _loadImpl(
                    *mNew
                    , _stateFileIO(zfargs, owner->stateFileFixed(), zftrue)
                    , zfargs
                    );
            zfargs.result(holder);
        } ZFLISTENER_END()
        ZFLISTENER_1(loadFinish
                , zfautoT<ZFState>, owner
                ) {
            ZFValueHolder *holder = zfargs.param0();
            ZFCoreMap &mNew = holder->valueRef<ZFCoreMap &>();
            _ZFP_ZFStatePrivate *d = owner->d;
            d->taskId = zfnull;
            {
                ZFObjectLocker(owner);
                d->m.swap(mNew);
            }
            d->_resolvePending(owner);

            ZFCoreArray<ZFListener> *loadQueueTmp = d->loadQueue;
            d->loadQueue = zfnull;
            for(zfindex i = 0; i < loadQueueTmp->count(); ++i) {
                loadQueueTmp->get(i).execute();
            }
            zfpoolDelete(loadQueueTmp);

            d->saveCheck(owner);
        } ZFLISTENER_END()
        this->taskId = zfasync(loadAsync, loadFinish);
    }
    void saveCheck(ZF_IN ZFState *owner) {
        if(this->taskId || this->loadQueue != zfnull || this->delayId || !this->changed) {
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
                _saveImpl(owner, zfargs);
            } ZFLISTENER_END()
            ZFLISTENER_1(saveFinish
                    , zfautoT<ZFState>, owner
                    ) {
                _saveFinishImpl(owner);
            } ZFLISTENER_END()
            d->taskId = zfasync(saveAsync, saveFinish);
        } ZFLISTENER_END()
        this->delayId = ZFTimerOnce(3000, onSaveDelay);
    }
    void saveImmediately(ZF_IN ZFState *owner) {
        if(this->loadQueue
                || this->taskId
                || !this->changed
                ) {
            return;
        }
        this->taskId = zfobj<ZFTaskIdBasic>();
        if(this->delayId) {
            this->delayId->stop();
            this->delayId = zfnull;
        }
        _saveImpl(owner, ZFArgs()
                .param0(this->taskId)
                );
        _saveFinishImpl(owner);
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
    void update(
            ZF_IN const zfstring &key
            , ZF_IN_OPT zftimet expire
            ) {
        const ZFCorePointer *item = this->m.get(key);
        if(item) {
            item->pointerValueT<_ZFP_ZFStateData *>()->expireTime = (expire > 0 ? ZFTime::currentTime() + expire : 0);
            this->m.set(key, *item);
        }
    }

private:
    void _resolvePending(ZF_IN ZFState *owner) {
        if(this->pending.isEmpty()) {
            return;
        }
        ZFObjectLocker(owner);
        for(zfiter it = this->pending.iter(); it; ++it) {
            const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> &item = *(const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> *)this->pending.iterValue(it);
            if(item->value) {
                this->m.set(item->key, item);
            }
            else {
                this->m.remove(item->key);
            }
        }
        this->pending.removeAll();
        this->changed = zftrue;
    }
    static zfautoT<ZFIOToken> _stateFileIO(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN zfbool readOnly
            ) {
        if(readOnly && !ZFIOIsExist(pathInfo)) {
            return zfnull;
        }
        // use modify to prevent concurrent R/W
        zfautoT<ZFIOToken> io = ZFIOOpen(pathInfo, v_ZFIOOpenOption::e_Modify);
        if(!io) {
            // other process may be writting, retry some times
            for(zfindex i = 0; i < 2; ++i) {
                if(!zfargs.param0()) {return zfnull;}
                ZFThread::sleep(200);
                if(!zfargs.param0()) {return zfnull;}
                io = ZFIOOpen(pathInfo, v_ZFIOOpenOption::e_Modify);
                if(io) {
                    return io;
                }
            }
        }
        return zfnull;
    }
    // file format:
    //     expireTime:encode(key):encode(value)
    static void _loadImpl(
            ZF_OUT ZFCoreMap &mNew
            , ZF_IN ZFIOToken *io
            , ZF_IN const ZFArgs &zfargs
            ) {
        if(!zfargs.param0()) {return;}
        ZFInput input = ZFInputForIOToken(io);
        if(!input) {return;}
        zftimet curTime = ZFTime::currentTime();
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
            if(!zftimetFromStringT(data->expireTime, line + pos[0].start, pos[0].count)) {
                continue;
            }
            if(data->expireTime != 0 && curTime >= data->expireTime) {
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
    static void _saveImpl(
            ZF_IN ZFState *owner
            , ZF_IN const ZFArgs &zfargs
            ) {

        // try open
        if(!zfargs.param0()) {return;}
        zfautoT<ZFIOToken> io = _stateFileIO(zfargs, owner->stateFileFixed(), zffalse);
        if(!io) {return;}
        _ZFP_ZFStatePrivate *d = owner->d;

        // load and merge, to prevent concurrent save/load across different process
        if(!zfargs.param0()) {return;}
        ZFCoreMap mNew;
        _loadImpl(mNew, io, zfargs);
        if(!zfargs.param0()) {return;}
        ZFObjectLock(owner);
        for(zfiter it = mNew.iter(); it; ++it) {
            zfiter itExist = d->m.iterFind(mNew.iterKey(it));
            if(itExist) {
                _ZFP_ZFStateData *item = mNew.iterValue<_ZFP_ZFStateData *>(it);
                _ZFP_ZFStateData *itemExist = d->m.iterValue<_ZFP_ZFStateData *>(itExist);
                if(zffalse
                        || (item->expireTime > itemExist->expireTime)
                        || (item->expireTime == 0 && item->value != itemExist->value)
                        ) {
                    d->m.iterValue(itExist, *(mNew.iterValue(it)));
                }
            }
        }
        ZFObjectUnlock(owner);

        // save
        if(!zfargs.param0()) {return;}
        ZFObjectLock(owner);
        ZFCoreArray<ZFCorePointerForPoolObject<_ZFP_ZFStateData *> > l;
        for(zfiter it = d->m.iter(); it; ++it) {
            l.add(*(const ZFCorePointerForPoolObject<_ZFP_ZFStateData *> *)d->m.iterValue(it));
        }
        ZFObjectUnlock(owner);

        ZFOutput output = ZFOutputForIOToken(io);
        if(!output) {return;}
        zftimet curTime = ZFTime::currentTime();
        zfstring line;
        for(zfindex i = 0; i < l.count(); ++i) {
            const _ZFP_ZFStateData &data = *((l[i]).pointerValue());
            if(data.expireTime != 0 && curTime >= data.expireTime) {
                continue;
            }
            zftimetToStringT(line, data.expireTime);
            line += ":";
            ZFCoreDataEncode(line, data.key);
            line += ":";
            ZFCoreDataEncode(line, data.value);
            output << line << "\n";
            line.removeAll();
        }
    }
    static void _saveFinishImpl(
            ZF_IN ZFState *owner
            ) {
        _ZFP_ZFStatePrivate *d = owner->d;
        d->taskId = zfnull;
        d->changed = zffalse;
        if(d->saveQueue.isEmpty()) {
            return;
        }
        ZFCoreArray<ZFListener> saveQueue;
        d->saveQueue.swap(saveQueue);
        for(zfindex i = 0; i < saveQueue.count(); ++i) {
            saveQueue[i].execute();
        }
    }
};

void ZFState::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFStatePrivate);
    _ZFP_ZFStateList.add(this);
}
void ZFState::objectOnDealloc(void) {
    _ZFP_ZFStateList.removeElement(this);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFState, ZFPathInfo, stateFile) {
    if(propertyValue != propertyValueOld) {
        d->saveCheck(this);
    }
}
ZFMETHOD_DEFINE_0(ZFState, ZFPathInfo, stateFileFixed) {
    zfobj<v_ZFPathInfo> tmp;
    if(this->stateFile().isEmpty()) {
        tmp->zfv.pathType(ZFPathType_settingPath());
        tmp->zfv.pathData("ZFState");
    }
    else {
        tmp->zfv.pathType(this->stateFile().pathType());
        tmp->zfv.pathData(this->stateFile().pathData());
    }
    this->observerNotify(zfself::E_StateFileUpdate(), tmp);
    return tmp->zfv;
}

ZFMETHOD_DEFINE_0(ZFState, zfbool, ready) {
    return d->loadQueue == zfnull;
}

zfclass _ZFP_I_ZFStateLoadTaskId : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_I_ZFStateLoadTaskId, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    ZFListener callback;
public:
    virtual void stop(void) {
        this->callback = zfnull;
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
    ZFLISTENER_1(callbackWrap
            , zfautoT<_ZFP_I_ZFStateLoadTaskId>, taskId
            ) {
        if(taskId->callback) {
            taskId->callback.execute();
            taskId->callback = zfnull;
        }
    } ZFLISTENER_END()
    d->loadQueue->add(callbackWrap);
    d->loadCheck(this);
    return taskId;
}

ZFMETHOD_DEFINE_1(ZFState, zfautoT<ZFTaskId>, save
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(!callback) {
        return zfnull;
    }
    if(!d->taskId && !d->delayId) {
        callback.execute();
        return zfnull;
    }

    zfobj<_ZFP_I_ZFStateLoadTaskId> taskId;
    taskId->callback = callback;
    ZFLISTENER_1(callbackWrap
            , zfautoT<_ZFP_I_ZFStateLoadTaskId>, taskId
            ) {
        if(taskId->callback) {
            taskId->callback.execute();
            taskId->callback = zfnull;
        }
    } ZFLISTENER_END()
    d->saveQueue.add(callbackWrap);
    d->saveCheck(this);
    return taskId;
}

ZFMETHOD_DEFINE_0(ZFState, void, saveImmediately) {
    d->saveImmediately(this);
}

ZFMETHOD_DEFINE_3(ZFState, void, set
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        , ZFMP_IN_OPT(zftimet, expire, 0)
        ) {
    if(!key) {
        return;
    }
    d->changed = zftrue;
    if(this->ready()) {
        {
            ZFObjectLocker(this);
            if(value) {
                _ZFP_ZFStateData *data = zfpoolNew(_ZFP_ZFStateData);
                data->expireTime = (expire > 0 ? ZFTime::currentTime() + expire : 0);
                data->key = key;
                data->value = value;
                d->m.set(key, ZFCorePointerForPoolObject<_ZFP_ZFStateData *>(data));
            }
            else {
                d->m.remove(key);
            }
        }
        d->saveCheck(this);
    }
    else {
        {
            ZFObjectLocker(this);
            _ZFP_ZFStateData *exist = d->pending.get<_ZFP_ZFStateData *>(key);
            if(exist != zfnull) {
                exist->expireTime = (expire > 0 ? ZFTime::currentTime() + expire : 0);
            }
            else {
                ZFCorePointerForPoolObject<_ZFP_ZFStateData *> data = zfpoolNew(_ZFP_ZFStateData);
                data->expireTime = (expire > 0 ? ZFTime::currentTime() + expire : 0);
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
    ZFCoreArray<ZFListener> *loadQueueTmp = d->loadQueue;
    d->loadQueue = zfnull;

    {
        ZFObjectLocker(this);
        d->changed = (!d->pending.isEmpty() || !d->m.isEmpty());
        d->pending.removeAll();
        d->m.removeAll();
    }

    if(loadQueueTmp) {
        for(zfindex i = 0; i < loadQueueTmp->count(); ++i) {
            loadQueueTmp->get(i).execute();
        }
        zfpoolDelete(loadQueueTmp);
    }

    d->saveCheck(this);
}

ZFMETHOD_DEFINE_1(ZFState, zfstring, get
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(!key) {
        return zfnull;
    }
    ZFObjectLocker(this);
    const _ZFP_ZFStateData *data = d->m.get<const _ZFP_ZFStateData *>(key);
    if(data != zfnull) {
        if(data->expireTime != 0 && ZFTime::currentTime() >= data->expireTime) {
            d->m.remove(key);
            return zfnull;
        }
        else {
            return data->value;
        }
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

ZFMETHOD_DEFINE_2(ZFState, void, update
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(zftimet, expire)
        ) {
    if(!key) {
        return;
    }
    if(this->ready()) {
        d->update(key, expire);
    }
    else {
        zfweakT<ZFState> weakSelf = this;
        ZFLISTENER_3(action
                , zftimet, expire
                , zfweakT<ZFState>, weakSelf
                , zfstring, key
                ) {
            weakSelf->d->update(key, expire);
        } ZFLISTENER_END()
        this->load(action);
    }
}

ZFMETHOD_DEFINE_0(ZFState, zfautoT<ZFMap>, getAll) {
    ZFObjectLocker(this);
    zfobj<ZFMap> ret;
    for(zfiter it = d->m.iter(); it; ++it) {
        ret->set(
                zfobj<v_zfstring>(d->m.iterKey(it))
                , zfobj<v_zfstring>(d->m.iterValue<_ZFP_ZFStateData *>(it)->value)
                );
    }
    return ret;
}
ZFMETHOD_DEFINE_2(ZFState, void, getAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, keys)
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, values)
        ) {
    ZFObjectLocker(this);
    for(zfiter it = d->m.iter(); it; ++it) {
        keys.add(d->m.iterKey(it));
        values.add(d->m.iterValue<_ZFP_ZFStateData *>(it)->value);
    }
}

ZF_NAMESPACE_GLOBAL_END

