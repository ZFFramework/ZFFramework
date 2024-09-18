#include "ZFObjectAutoPtr.h"
#include "ZFObjectImpl.h"
#include "ZFDynamicInvoker.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

// #define _ZFP_ZFObjectPrivate_DEBUG 1

#if _ZFP_ZFObjectPrivate_DEBUG
#include "ZFCore/ZFTimer.h"
static zfindex _ZFP_ZFObjectAllocCount = 0;
static zfindex _ZFP_ZFObjectCount = 0;
static zfindex _ZFP_ZFObjectPrivateAllocCount = 0;
static zfindex _ZFP_ZFObjectPrivateCount = 0;
ZF_GLOBAL_INITIALIZER_INIT(ZFObjectCount) {
    ZFLISTENER(action) {
        ZFCoreLogTrim("[ZFObjectCount] %s/%s %s/%s"
                , _ZFP_ZFObjectCount
                , _ZFP_ZFObjectAllocCount
                , _ZFP_ZFObjectPrivateCount
                , _ZFP_ZFObjectPrivateAllocCount
                );
    } ZFLISTENER_END()
    ZFTimerOnce(3000, action);
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectCount)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFObjectPrivate
typedef zfstlmap<zfstring, zfauto> _ZFP_ZFObjectTagMapType;

zfclassNotPOD _ZFP_ZFObjectPrivate {
public:
    ZFObjectHolder *objectHolder;
    void *mutexImpl;
    _ZFP_ZFObjectTagMapType *objectTagMap;
    zfstlvector<const ZFProperty *> propertyAccessed;
    enum {
        stateFlag_objectIsInternal = 1 << 0,
        stateFlag_objectIsInternalPrivate = 1 << 1,
        stateFlag_observerHasAddFlag_objectBeforeAlloc = 1 << 2,
        stateFlag_observerHasAddFlag_objectAfterAlloc = 1 << 3,
        stateFlag_observerHasAddFlag_objectBeforeDealloc = 1 << 4,
        stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate = 1 << 5,
        stateFlag_ZFObjectInstanceStateOnInit = 1 << 6,
        stateFlag_ZFObjectInstanceStateOnInitFinish = 1 << 7,
        stateFlag_ZFObjectInstanceStateIdle = 1 << 8,
        stateFlag_ZFObjectInstanceStateOnDeallocPrepare = 1 << 9,
        stateFlag_ZFObjectInstanceStateOnDealloc = 1 << 10,
    };
    ZFObserver *observerHolder;
    _ZFP_zfAllocCacheReleaseCallback zfAllocCacheRelease;
    ZFObject *ZFImplementDynamicOwner;
    zfstlmap<const ZFClass *, zfauto> ZFImplementDynamicHolder;

public:
    _ZFP_ZFObjectPrivate(void)
    : objectHolder(zfnull)
    , mutexImpl(zfnull)
    , objectTagMap(zfnull)
    , propertyAccessed()
    , observerHolder(zfnull)
    , zfAllocCacheRelease(zfnull)
    , ZFImplementDynamicOwner(zfnull)
    , ZFImplementDynamicHolder()
    {
#if _ZFP_ZFObjectPrivate_DEBUG
        ++_ZFP_ZFObjectPrivateAllocCount;
        ++_ZFP_ZFObjectPrivateCount;
#endif
    }
#if _ZFP_ZFObjectPrivate_DEBUG
    ~_ZFP_ZFObjectPrivate(void) {
        --_ZFP_ZFObjectPrivateCount;
    }
#endif

public:
    ZFObserver &observerHolderCheck(ZF_IN ZFObject *owner) {
        if(this->observerHolder == zfnull) {
            this->observerHolder = zfpoolNew(ZFObserver);
            this->observerHolder->_ZFP_ZFObserver_observerOwner(owner);
        }
        return *this->observerHolder;
    }
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

ZFEVENT_REGISTER(ZFObject, ObjectBeforeAlloc)
ZFEVENT_REGISTER(ZFObject, ObjectAfterAlloc)
ZFEVENT_REGISTER(ZFObject, ObjectBeforeDealloc)
ZFEVENT_REGISTER(ZFObject, ObjectPropertyValueOnUpdate)

static zfuint _ZFP_ZFObject_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObject_stateFlags, ZFLevelZFFrameworkStatic) {
    _ZFP_ZFObject_stateFlags = 0;
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::EventObjectBeforeAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObject_stateFlags) {
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::EventObjectBeforeAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_END(ZFObject_stateFlags)

ZFObjectHolder *ZFObject::objectHolder(void) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    if(d->objectHolder == zfnull) {
        d->objectHolder = zfunsafe_zfAlloc(ZFObjectHolder);
        d->objectHolder->objectHolded(this);
    }
    return d->objectHolder;
}

/* ZFMETHOD_MAX_PARAM */
zfauto ZFObject::invoke(
        ZF_IN const zfstring &methodName
        ) {
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfauto ret;
    if(ZFDI_invoke(ret, zfnull, this, methodName, 0, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFObject::invoke(
        ZF_IN const zfstring &methodName
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) {
    ZFCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == ZFMP_DEF()) {paramCount = 0; break;} else {paramList[0].zfunsafe_assign(param0);}
        if(param1 == ZFMP_DEF()) {paramCount = 1; break;} else {paramList[1].zfunsafe_assign(param1);}
        if(param2 == ZFMP_DEF()) {paramCount = 2; break;} else {paramList[2].zfunsafe_assign(param2);}
        if(param3 == ZFMP_DEF()) {paramCount = 3; break;} else {paramList[3].zfunsafe_assign(param3);}
        if(param4 == ZFMP_DEF()) {paramCount = 4; break;} else {paramList[4].zfunsafe_assign(param4);}
        if(param5 == ZFMP_DEF()) {paramCount = 5; break;} else {paramList[5].zfunsafe_assign(param5);}
        if(param6 == ZFMP_DEF()) {paramCount = 6; break;} else {paramList[6].zfunsafe_assign(param6);}
        if(param7 == ZFMP_DEF()) {paramCount = 7; break;} else {paramList[7].zfunsafe_assign(param7);}
    } while(zffalse);
    ZFCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_invoke(ret, zfnull, this, methodName, paramCount, paramList, zftrue)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFObject::invokeDetail(
        ZF_IN const zfstring &methodName
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = zfmMin((zfindex)ZFMETHOD_MAX_PARAM, params.count());
    for(zfindex i = 0; i < paramCount; ++i) {
        paramList[i].zfunsafe_assign(params[i]);
    }
    for(zfindex i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
        paramList[i].zfunsafe_assign(ZFMP_DEF());
    }
    ZFCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_invoke(ret, errorHint, this, methodName, paramCount, paramList, zftrue)) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

zfbool ZFObject::objectTagExist(void) {
    return d && d->objectTagMap && !(d->objectTagMap->empty());
}
void ZFObject::objectTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) {
    ZFCoreMutexLocker();
    if(tag != zfnull && this->objectDeallocRunning()) {
        ZFCoreCriticalMessageTrim("[ZFObject] you must not set tag while object is deallocating, class: %s, tag: %s",
            this->classData()->classNameFull(),
            key);
        return;
    }
    if(key == zfnull || (d == zfnull && tag == zfnull)) {
        return;
    }

    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    if(d->objectTagMap == zfnull) {
        d->objectTagMap = zfpoolNew(_ZFP_ZFObjectTagMapType);
    }
    _ZFP_ZFObjectTagMapType &m = *(d->objectTagMap);
    _ZFP_ZFObjectTagMapType::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag != zfnull) {
            m[key].zfunsafe_assign(tag);
        }
    }
    else {
        ZFObject *obj = zfunsafe_zfRetain(it->second);
        if(tag == zfnull) {
            m.erase(it);
        }
        else {
            it->second.zfunsafe_assign(tag);
        }
        zfunsafe_zfRelease(obj);
    }
}
zfany ZFObject::objectTag(ZF_IN const zfstring &key) {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        if(d && d->objectTagMap) {
            _ZFP_ZFObjectTagMapType::iterator it = d->objectTagMap->find(key);
            if(it != d->objectTagMap->end()) {
                return it->second;
            }
        }
    }
    return zfnull;
}
void ZFObject::objectTagGetAllKeyValue(
        ZF_IN_OUT ZFCoreArray<zfstring> &allKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
        ) {
    ZFCoreMutexLocker();
    if(d && d->objectTagMap) {
        _ZFP_ZFObjectTagMapType &m = *(d->objectTagMap);
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it) {
            allKey.add(it->first);
            allValue.add(it->second);
        }
    }
}
zfauto ZFObject::objectTagRemoveAndGet(ZF_IN const zfstring &key) {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        if(d && d->objectTagMap) {
            _ZFP_ZFObjectTagMapType::iterator it = d->objectTagMap->find(key);
            if(it != d->objectTagMap->end()) {
                zfauto ret;
                ret.zfunsafe_assign(it->second);
                d->objectTagMap->erase(it);
                return ret;
            }
        }
    }
    return zfnull;
}
void ZFObject::objectTagRemoveAll(void) {
    if(d && d->objectTagMap && !d->objectTagMap->empty()) {
        ZFCoreMutexLocker();
        _ZFP_ZFObjectTagMapType tmp;
        tmp.swap(*(d->objectTagMap));
    }
}

void ZFObject::observerAdd(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    return this->observerHolder().observerAdd(
        eventId,
        observer,
        observerLevel);
}
void ZFObject::observerAddForOnce(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    return this->observerHolder().observerAddForOnce(
        eventId,
        observer,
        observerLevel);
}
void ZFObject::observerRemove(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &callback
        ) {
    if(d && d->observerHolder) {
        d->observerHolder->observerRemove(eventId, callback);
    }
}
void ZFObject::observerRemoveAll(ZF_IN zfidentity eventId) {
    if(d && d->observerHolder) {
        d->observerHolder->observerRemoveAll(eventId);
    }
}
void ZFObject::observerRemoveAll(void) {
    if(d && d->observerHolder) {
        d->observerHolder->observerRemoveAll();
    }
}
zfbool ZFObject::observerHasAdd(void) {
    if(d && d->observerHolder) {
        return d->observerHolder->observerHasAdd();
    }
    else {
        return zffalse;
    }
}
zfbool ZFObject::observerHasAdd(ZF_IN zfidentity eventId) {
    if(d && d->observerHolder) {
        return d->observerHolder->observerHasAdd(eventId);
    }
    else {
        return ZFGlobalObserver().observerHasAdd(eventId);
    }
}
void ZFObject::observerNotify(
        ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) {
    if(d && d->observerHolder) {
        d->observerHolder->observerNotify(eventId, param0, param1);
    }
    else {
        ZFGlobalObserver().observerNotifyWithSender(this, eventId, param0, param1);
    }
}
void ZFObject::observerNotifyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) {
    if(d && d->observerHolder) {
        d->observerHolder->observerNotifyWithSender(customSender, eventId, param0, param1);
    }
    else {
        ZFGlobalObserver().observerNotifyWithSender(customSender, eventId, param0, param1);
    }
}
ZFObserver &ZFObject::observerHolder(void) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    if(d->observerHolder == zfnull) {
        d->observerHolder = zfpoolNew(ZFObserver);
        d->observerHolder->_ZFP_ZFObserver_observerOwner(this);
    }
    return *d->observerHolder;
}

void ZFObject::observerOnAdd(ZF_IN zfidentity eventId) {
    if(zffalse) {
    }
    else if(eventId == ZFObject::EventObjectBeforeAlloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}
void ZFObject::observerOnRemove(ZF_IN zfidentity eventId) {
    if(zffalse) {
    }
    else if(eventId == ZFObject::EventObjectBeforeAlloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}

void *ZFObject::_ZFP_ZFObjectMutexImpl(void) {
    return d ? d->mutexImpl : zfnull;
}
void ZFObject::_ZFP_ZFObjectLock(void) {
    if(d && d->mutexImpl) {
        _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
    }
    else {
        ZFCoreMutexLock();
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFObjectPrivate);
        }
        if(_ZFP_ZFObjectMutexImplInit) {
            if(d->mutexImpl == zfnull) {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        ZFCoreMutexUnlock();
        if(d->mutexImpl) {
            _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
        }
    }
}
void ZFObject::_ZFP_ZFObjectUnlock(void) {
    if(d && d->mutexImpl) {
        _ZFP_ZFObjectMutexImplUnlock(d->mutexImpl);
    }
}
zfbool ZFObject::_ZFP_ZFObjectTryLock(void) {
    if(d && d->mutexImpl) {
        return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
    }
    else {
        ZFCoreMutexLock();
        if(_ZFP_ZFObjectMutexImplInit) {
            if(d->mutexImpl == zfnull) {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        ZFCoreMutexUnlock();
        if(d->mutexImpl) {
            return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
        }
        else {
            return zffalse;
        }
    }
}

ZFObject *ZFObject::_ZFP_ZFObjectCheckOnInit(void) {
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
    this->classData()->_ZFP_ZFClass_propertyAutoInitAction(this);
    this->_ZFP_ObjI_onInitIvk();
    if(!this->objectIsInternalPrivate()) {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc)
                ) {
            this->observerNotify(ZFObject::EventObjectBeforeAlloc());
        }
    }
    this->objectOnInitFinish();
    ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInit);
    ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateIdle);

    if(!this->objectIsInternalPrivate()) {
        if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                ) {
            this->observerNotify(ZFObject::EventObjectAfterAlloc());
        }
    }

    return this;
}
void ZFObject::_ZFP_ZFObjectCheckRelease(void) {
    if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
            ) {
        if(_objectRetainCount == 1) {
            this->observerNotify(ZFObject::EventObjectBeforeDealloc());
            if(_objectRetainCount > 1) {
                this->objectOnRelease();
                this->observerRemoveAll(ZFObject::EventObjectBeforeDealloc());
                return;
            }
        }
    }

    if(d && d->zfAllocCacheRelease && _objectRetainCount == 1) { // check to save cache
        this->observerRemoveAll();
        d->zfAllocCacheRelease(this);
        return;
    }

    this->objectOnRelease();
    if(_objectRetainCount > 0) {
        return;
    }

    ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateIdle);
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDeallocPrepare);
    this->objectOnDeallocPrepare();
    this->_ZFP_ObjI_onDeallocIvk();
    if(d) {
        for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i) {
            const ZFProperty *property = d->propertyAccessed[i];
            if(property->_ZFP_ZFProperty_callbackDealloc) {
                property->_ZFP_ZFProperty_callbackDealloc(property, this);
            }
        }
    }
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDealloc);
    this->objectOnDealloc();
    this->classData()->_ZFP_ZFClass_objectDesctuct(this);
}

void ZFObject::objectOnInit(void) {
    if(this->classData()->classIsInternal()) {
        ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
    }
    if(this->classData()->classIsInternalPrivate()) {
        ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternalPrivate);
    }
#if _ZFP_ZFObjectPrivate_DEBUG
        ++_ZFP_ZFObjectAllocCount;
        ++_ZFP_ZFObjectCount;
#endif
}
void ZFObject::objectOnDealloc(void) {
#if _ZFP_ZFObjectPrivate_DEBUG
        --_ZFP_ZFObjectCount;
#endif
    if(_ZFP_ZFObject_classDynamic) {
        _ZFP_ZFObject_classDynamic->_ZFP_classDynamicRegisterObjectInstanceDetach(this);
    }

    if(d) {
        if(d->mutexImpl) {
            _ZFP_ZFObjectMutexImplDealloc(d->mutexImpl);
            d->mutexImpl = zfnull;
        }

        if(this->_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf() == this) {
            if(d->objectTagMap) {
                zfpoolDelete(d->objectTagMap);
            }
            if(d->observerHolder) {
                zfpoolDelete(d->observerHolder);
            }
            if(d->objectHolder) {
                d->objectHolder->objectHolded(zfnull);
                zfRelease(d->objectHolder);
            }
        }
        else {
            if(d->objectHolder) {
                zfRelease(d->objectHolder);
            }
        }

        zfpoolDelete(d);
    }
}

zfbool ZFObject::objectInstanceStateCheck(ZF_IN ZFObjectInstanceState state) {
    switch(state) {
        case ZFObjectInstanceStateOnInit:
            return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInit);
        case ZFObjectInstanceStateOnInitFinish:
            return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
        case ZFObjectInstanceStateIdle:
            return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateIdle);
        case ZFObjectInstanceStateOnDeallocPrepare:
            return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDeallocPrepare);
        case ZFObjectInstanceStateOnDealloc:
            return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDealloc);
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zffalse;
    }
}
zfbool ZFObject::objectInitRunning(void) {
    return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInit)
        || ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
}
zfbool ZFObject::objectDeallocRunning(void) {
    return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDeallocPrepare)
        || ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDealloc);
}

zfbool ZFObject::objectIsInternal(void) {
    return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal)
        || ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternalPrivate);
}
zfbool ZFObject::objectIsInternalPrivate(void) {
    return ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternalPrivate);
}

void ZFObject::_ZFP_ZFObject_objectPropertyValueAttach(ZF_IN const ZFProperty *property) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    d->propertyAccessed.push_back(property);
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueDetach(ZF_IN const ZFProperty *property) {
    for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i) {
        if(d->propertyAccessed[i] == property) {
            d->propertyAccessed.erase(d->propertyAccessed.begin() + i);
        }
    }
}

void ZFObject::objectPropertyValueOnUpdate(
        ZF_IN const ZFProperty *property
        , ZF_IN const void *oldValue
        ) {
    if(!this->objectIsInternalPrivate()
            && (ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate))
                ) {
        v_ZFProperty *param0 = zfunsafe_zfAlloc(v_ZFProperty);
        param0->zfv = property;
        v_zfptr *param1 = zfunsafe_zfAlloc(v_zfptr);
        param1->zfv = oldValue;
        this->observerNotify(ZFObject::EventObjectPropertyValueOnUpdate(), param0, param1);
        zfunsafe_zfRelease(param0);
        zfunsafe_zfRelease(param1);
    }
}

// ============================================================
void ZFObject::_ZFP_ZFObject_zfAllocCacheRelease(ZF_IN _ZFP_zfAllocCacheReleaseCallback callback) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    d->zfAllocCacheRelease = callback;
}
_ZFP_zfAllocCacheReleaseCallback ZFObject::_ZFP_ZFObject_zfAllocCacheRelease(void) {
    return d ? d->zfAllocCacheRelease : zfnull;
}

ZFObject *ZFObject::_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf(void) {
    return d && d->ZFImplementDynamicOwner ? d->ZFImplementDynamicOwner : this;
}
ZFObject *ZFObject::_ZFP_ZFObject_ZFImplementDynamicHolder(ZF_IN const ZFClass *clsToImplement) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    zfstlmap<const ZFClass *, zfauto>::iterator it = d->ZFImplementDynamicHolder.find(clsToImplement);
    if(it != d->ZFImplementDynamicHolder.end()) {
        return it->second;
    }
    else {
        _ZFP_ZFObjectPrivate *dObj = zfpoolNew(_ZFP_ZFObjectPrivate);
        dObj->ZFImplementDynamicOwner = this;

        // alias internal data to owner
        dObj->objectHolder = zfRetain(this->objectHolder());
        dObj->observerHolder = zfpoolNew(ZFObserver, this->observerHolder());
        if(d->objectTagMap == zfnull) {
            d->objectTagMap = zfpoolNew(_ZFP_ZFObjectTagMapType);
        }
        if(dObj->objectTagMap != zfnull) {
            for(_ZFP_ZFObjectTagMapType::iterator it = dObj->objectTagMap->begin(); it != dObj->objectTagMap->end(); ++it) {
                (*(d->objectTagMap))[it->first] = it->second;
            }
            _ZFP_ZFObjectTagMapType *tmp = dObj->objectTagMap;
            dObj->objectTagMap = d->objectTagMap;
            zfpoolDelete(tmp);
        }
        else {
            dObj->objectTagMap = d->objectTagMap;
        }

        zfauto holder = clsToImplement->_ZFP_ZFClass_newInstance(dObj);
        d->ZFImplementDynamicHolder[clsToImplement] = holder;
        return holder;
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFClass *, classData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObjectHolder *, objectHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoOfInstanceT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfoOfInstance)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfidentity, objectHash)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectCompare
        , ZFMP_IN(ZFObject *, anotherObj)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectValueCompare
        , ZFMP_IN(ZFObject *, anotherObj)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfbool, equalTo
        , ZFMP_IN(ZFObject *, anotherObj)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfauto, invoke
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ZFObject, zfauto, invoke
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        // ZFMETHOD_MAX_PARAM
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, zfauto, invokeDetail
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, params)
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectTagExist)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, objectTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfany, objectTag
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, objectTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectTagRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfauto, objectTagRemoveAndGet
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, objectTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerAdd
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerAddForOnce
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, observerRemove
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveAll
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfbool, observerHasAdd
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerNotify
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerNotifyWithSender
        , ZFMP_IN(ZFObject *, customSender)
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObserver &, observerHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfbool, objectInstanceStateCheck
        , ZFMP_IN(ZFObjectInstanceState, state)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectInitRunning)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectDeallocRunning)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsInternal)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsInternalPrivate)

ZF_NAMESPACE_GLOBAL_END
#endif

