#include "ZFState.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstllist.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFState)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFState, instance, ZFLevelZFFrameworkEssential)

zfclassFwd _ZFP_ZFStateData;
typedef zfstlhashmap<zfstring, ZFCorePointerForPoolObject<_ZFP_ZFStateData *>, zfstring_zfstlHash, zfstring_zfstlEqual> _ZFP_ZFStateMapType;
typedef zfstllist<ZFCorePointerForPoolObject<_ZFP_ZFStateData *> > _ZFP_ZFStateListType;
zfclassNotPOD _ZFP_ZFStateData {
public:
    zftimet cacheTime;
    zfstring key;
    zfstring value;
    _ZFP_ZFStateListType::iterator listIt;
};

zfclassNotPOD _ZFP_ZFStatePrivate {
public:
    ZFCoreArray<ZFListener> *loadQueue; // null when ready
    _ZFP_ZFStateMapType pending;
    _ZFP_ZFStateMapType m;
    _ZFP_ZFStateListType l; // latest item at head
    zfautoT<ZFTaskId> taskId; // taskId for load or save
    zfautoT<ZFTimer> delayId; // taskId for delay save

public:
    _ZFP_ZFStatePrivate(void)
    : loadQueue(zfpoolNew(ZFCoreArray<ZFListener>))
    , pending()
    , m()
    , l()
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
    void trim(ZF_IN ZFState *owner) {
        zfsynchronize(owner);
        if(this->l.empty()) {
            return;
        }
        zftimet curTime = ZFTime::currentTime();
        _ZFP_ZFStateListType::iterator it = this->l.end();
        --it;
        while(it != this->l.end()) {
            if(this->m.size() <= owner->cacheSize()
                    && curTime - (*it)->cacheTime <= owner->cacheTime()
                    ) {
                break;
            }
            this->m.erase((*it)->key);
            this->l.erase(it--);
        }
    }
    void loadCheck(ZF_IN ZFState *owner) {
        if(this->taskId || this->loadQueue == zfnull) {
            return;
        }
        ZFLISTENER_1(loadAsync
                , zfautoT<ZFState>, owner
                ) {
            if(!zfargs.param0()) {return;}
            _ZFP_ZFStateListType *lNew = zfnew(_ZFP_ZFStateListType);
            zfobj<ZFValueHolder> holder((void *)lNew, ZFValueHolderTypeObject(_ZFP_ZFStateListType *));
            _loadAsyncImpl(*lNew, owner->stateFileFixed(), zfargs);
            zfargs.result(holder);
        } ZFLISTENER_END()
        ZFLISTENER_1(loadFinish
                , zfautoT<ZFState>, owner
                ) {
            ZFValueHolder *holder = zfargs.param0();
            _ZFP_ZFStateListType &lNew = holder->holdedDataRef<_ZFP_ZFStateListType &>();
            _ZFP_ZFStatePrivate *d = owner->d;
            {
                zfsynchronize(owner);
                for(_ZFP_ZFStateListType::iterator itNew = lNew.begin(); itNew != lNew.end(); ++itNew) {
                    ZFCorePointerForPoolObject<_ZFP_ZFStateData *> &data = *itNew;
                    _ZFP_ZFStateMapType::iterator it = d->m.find(data->key);
                    if(it == d->m.end()) {
                        d->m[data->key] = data;
                        d->l.push_back(data);
                        data->listIt = d->l.begin();
                    }
                    else if(data->cacheTime > it->second->cacheTime) {
                        it->second->cacheTime = data->cacheTime;
                        it->second->value = data->value;
                        // do not reorder by cacheTime for performance
                    }
                }
            }
            zfbool changed = d->_resolvePending(owner);
            d->trim(owner);
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
                _ZFP_ZFStateListType lTmp = d->l;
                zfsynchronizeUnlock(owner);
                _saveAsyncImpl(lTmp, owner->stateFileFixed(), zfargs);
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
            this->delayId->timerStop();
            this->delayId = zfnull;
        }
        if(this->taskId) {
            this->taskId->stop();
            this->taskId = zfnull;
        }
    }

private:
    zfbool _resolvePending(ZF_IN ZFState *owner) {
        zfsynchronize(owner);
        zfbool changed = zffalse;
        for(_ZFP_ZFStateMapType::iterator itNew = this->pending.begin(); itNew != this->pending.end(); ++itNew) {
            _ZFP_ZFStateMapType::iterator it = this->m.find(itNew->first);
            if(it == this->m.end()) {
                if(itNew->second->value) {
                    changed = zftrue;
                    this->m[itNew->first] = itNew->second;
                    this->l.push_front(itNew->second);
                    itNew->second->listIt = this->l.begin();
                }
            }
            else {
                changed = zftrue;
                if(itNew->second->value) {
                    it->second->cacheTime = itNew->second->cacheTime;
                    it->second->value = itNew->second->value;
                    this->l.erase(it->second->listIt);
                    this->l.push_front(it->second);
                    it->second->listIt = this->l.begin();
                }
                else {
                    this->l.erase(it->second->listIt);
                    this->m.erase(it);
                }
            }
        }
        return changed;
    }
    // file format:
    //     cacheTime:encode(key):encode(value)
    static void _loadAsyncImpl(
            ZF_OUT _ZFP_ZFStateListType &lNew
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
            lNew.push_back(data);
        }
    }
    static void _saveAsyncImpl(
            ZF_IN _ZFP_ZFStateListType &l
            , ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN const ZFArgs &zfargs
            ) {
        if(!zfargs.param0()) {return;}
        ZFOutput output = ZFOutputForPathInfo(pathInfo);
        if(!output) {return;}
        zfstring line;
        for(_ZFP_ZFStateListType::iterator it = l.begin(); it != l.end() && zfargs.param0(); ++it, line.removeAll()) {
            _ZFP_ZFStateData *data = *it;
            zftimetToStringT(line, data->cacheTime);
            line += ":";
            ZFCoreDataEncode(line, data->key);
            line += ":";
            ZFCoreDataEncode(line, data->value);
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
    _ZFP_ZFStatePrivate *d;
    ZFListener callback;
public:
    virtual void stop(void) {
        if(d && d->loadQueue && this->callback) {
            d->loadQueue->removeElement(this->callback);
            this->callback = zfnull;
        }
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
    ZFLISTENER_1(callbackWrap
            , zfautoT<_ZFP_I_ZFStateLoadTaskId>, taskId
            ) {
        taskId->d = zfnull;
        taskId->callback = zfnull;
    } ZFLISTENER_END()
    d->loadQueue->add(callbackWrap);
    return taskId;
}

ZFMETHOD_DEFINE_2(ZFState, void, set
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const zfstring &, value)
        ) {
    if(zfstringIsEmpty(key)) {
        return;
    }
    if(this->ready()) {
        {
            zfsynchronize(this);
            _ZFP_ZFStateMapType::iterator it = d->m.find(key);
            if(it != d->m.end()) {
                if(zfstringIsEmpty(value)) {
                    d->l.erase(it->second->listIt);
                    d->m.erase(it);
                }
                else {
                    it->second->cacheTime = ZFTime::currentTime();
                    it->second->value = value;
                    d->l.erase(it->second->listIt);
                    d->l.push_front(it->second);
                    it->second->listIt = d->l.begin();
                }
            }
            else {
                if(!zfstringIsEmpty(value)) {
                    ZFCorePointerForPoolObject<_ZFP_ZFStateData *> data = zfpoolNew(_ZFP_ZFStateData);
                    data->cacheTime = ZFTime::currentTime();
                    data->key = key;
                    data->value = value;
                    d->m[data->key] = data;
                    d->l.push_front(data);
                    data->listIt = d->l.begin();
                }
            }
        }
        d->trim(this);
        d->saveCheck(this);
    }
    else {
        {
            zfsynchronize(this);
            _ZFP_ZFStateMapType::iterator it = d->pending.find(key);
            if(it != d->pending.end()) {
                it->second->cacheTime = ZFTime::currentTime();
                it->second->value = value;
            }
            else {
                ZFCorePointerForPoolObject<_ZFP_ZFStateData *> data = zfpoolNew(_ZFP_ZFStateData);
                data->cacheTime = ZFTime::currentTime();
                data->key = key;
                data->value = value;
                d->pending[data->key] = data;
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

    ZFCoreArray<ZFCorePointerForPoolObject<_ZFP_ZFStateData *> > toRelease;
    if(!d->m.empty()) {
        toRelease.capacity((zfindex)d->m.size());
        zfsynchronize(this);
        for(_ZFP_ZFStateMapType::iterator it = d->m.begin(); it != d->m.end(); ++it) {
            toRelease.add(it->second);
        }
        for(_ZFP_ZFStateMapType::iterator it = d->pending.begin(); it != d->pending.end(); ++it) {
            toRelease.add(it->second);
        }
        d->m.clear();
        d->l.clear();
        d->pending.clear();
    }

    d->saveCheck(this);
}

ZFMETHOD_DEFINE_1(ZFState, zfstring, get
        , ZFMP_IN(const zfstring &, key)
        ) {
    if(zfstringIsEmpty(key)) {
        return zfnull;
    }
    zfsynchronize(this);
    _ZFP_ZFStateMapType::iterator it = d->m.find(key);
    if(it != d->m.end()) {
        return it->second->value;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFState, zfautoT<ZFTaskId>, getAsync
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(zfstringIsEmpty(key)) {
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
    for(_ZFP_ZFStateMapType::iterator it = d->m.begin(); it != d->m.end(); ++it) {
        ret.add(it->second->key);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

