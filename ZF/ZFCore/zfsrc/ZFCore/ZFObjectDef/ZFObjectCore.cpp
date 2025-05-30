#include "ZFObjectAutoPtr.h"
#include "ZFObjectImpl.h"
#include "ZFDynamicInvoker.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"

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
typedef zfstlhashmap<zfstring, zfauto> _ZFP_ZFObjectTagMapType;

zfclassNotPOD _ZFP_ZFObjectPrivate {
public:
    _ZFP_I_zfweak *weakHolder;
    void *mutexImpl;
    _ZFP_ZFObjectTagMapType *objectTagMap;
    zfstldeque<const ZFProperty *> propertyAccessed;
    enum {
        stateFlag_objectIsInternal = 1 << 0,
        stateFlag_objectIsInternalPrivate = 1 << 1,
        stateFlag_observerHasAddFlag_objectBeforeAlloc = 1 << 2,
        stateFlag_observerHasAddFlag_objectAfterAlloc = 1 << 3,
        stateFlag_observerHasAddFlag_objectBeforeDealloc = 1 << 4,
        stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate = 1 << 5,
        stateFlag_observerHasAddFlag_objectPropertyValueOnReset = 1 << 6,
        stateFlag_ZFObjectInstanceStateOnInit = 1 << 7,
        stateFlag_ZFObjectInstanceStateOnInitFinish = 1 << 8,
        stateFlag_ZFObjectInstanceStateIdle = 1 << 9,
        stateFlag_ZFObjectInstanceStateOnDeallocPrepare = 1 << 10,
        stateFlag_ZFObjectInstanceStateOnDealloc = 1 << 11,
    };
    ZFObserver *observerHolder;
    _ZFP_zfAllocCacheReleaseCallback zfAllocCacheRelease;
    ZFObject *ZFImplementDynamicOwner;
    zfstlhashmap<const ZFClass *, zfauto> ZFImplementDynamicHolder;

public:
    _ZFP_ZFObjectPrivate(void)
    : weakHolder(zfnull)
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
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

ZFEVENT_REGISTER(ZFObject, ObjectBeforeAlloc)
ZFEVENT_REGISTER(ZFObject, ObjectAfterAlloc)
ZFEVENT_REGISTER(ZFObject, ObjectBeforeDealloc)
ZFEVENT_REGISTER(ZFObject, ObjectPropertyValueOnUpdate)
ZFEVENT_REGISTER(ZFObject, ObjectPropertyValueOnReset)

static zfuint _ZFP_ZFObject_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObject_stateFlags, ZFLevelZFFrameworkStatic) {
    _ZFP_ZFObject_stateFlags = 0;
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::E_ObjectBeforeAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::E_ObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::E_ObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::E_ObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    ZFGlobalObserver().observerHasAddStateAttach(ZFObject::E_ObjectPropertyValueOnReset(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObject_stateFlags) {
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::E_ObjectBeforeAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::E_ObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::E_ObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::E_ObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    ZFGlobalObserver().observerHasAddStateDetach(ZFObject::E_ObjectPropertyValueOnReset(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset);
}
ZF_GLOBAL_INITIALIZER_END(ZFObject_stateFlags)

_ZFP_I_zfweak *ZFObject::_ZFP_ZFObject_weakHolder(void) {
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFObjectPrivate);
    }
    if(d->weakHolder == zfnull) {
        d->weakHolder = zfunsafe_zfAlloc(_ZFP_I_zfweak, this);
    }
    return d->weakHolder;
}

void ZFObject::objectInfoT(ZF_IN_OUT zfstring &ret) {
    if(this->_ZFP_ZFObject_classDynamic) {
        const ZFMethod *m = this->classData()->methodForName("objectInfoImpl");
        if(m && m->ownerClass() != ZFObject::ClassData()) {
            zfobj<v_zfstring> retHolder;
            m->methodInvoke(this, retHolder);
            ret += retHolder->zfv;
            return;
        }
    }
    this->objectInfoImpl(ret);
}

zfidentity ZFObject::objectHash(void) {
    if(this->_ZFP_ZFObject_classDynamic) {
        const ZFMethod *m = this->classData()->methodForName("objectHashImpl");
        if(m && m->ownerClass() != ZFObject::ClassData()) {
            return m->methodInvoke(this).to<v_zfidentity *>()->zfv;
        }
    }
    return this->objectHashImpl();
}
ZFCompareResult ZFObject::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(this->_ZFP_ZFObject_classDynamic) {
        const ZFMethod *m = this->classData()->methodForName("objectCompareImpl");
        if(m && m->ownerClass() != ZFObject::ClassData()) {
            return m->methodInvoke(this, anotherObj).to<v_ZFCompareResult *>()->zfv;
        }
    }
    return this->objectCompareImpl(anotherObj);
}
ZFCompareResult ZFObject::objectCompareValue(ZF_IN ZFObject *anotherObj) {
    if(this->_ZFP_ZFObject_classDynamic) {
        const ZFMethod *m = this->classData()->methodForName("objectCompareValueImpl");
        if(m && m->ownerClass() != ZFObject::ClassData()) {
            return m->methodInvoke(this, anotherObj).to<v_ZFCompareResult *>()->zfv;
        }
        else {
            m = this->classData()->methodForName("objectCompareImpl");
            if(m && m->ownerClass() != ZFObject::ClassData()) {
                return m->methodInvoke(this, anotherObj).to<v_ZFCompareResult *>()->zfv;
            }
        }
    }
    return this->objectCompareValueImpl(anotherObj);
}

/* ZFMETHOD_MAX_PARAM */
zfauto ZFObject::invoke(
        ZF_IN const zfstring &methodName
        ) {
    ZFArgs zfargs;
    zfargs
        .sender(this)
        .paramInit()
        ;
    ZFDI_invoke(zfargs, methodName, zftrue);
    return zfargs.result();
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
    ZFArgs zfargs;
    zfargs
        .sender(this)
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        ;
    ZFDI_invoke(zfargs, methodName, zftrue);
    return zfargs.result();
}
zfbool ZFObject::invokeT(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT ZFObject *param0 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) {
    ZFArgs zfargs;
    zfargs
        .sender(this)
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    ZFDI_invoke(zfargs, methodName, zftrue);
    ret = zfargs.result();
    if(errorHint != zfnull) {
        *errorHint += zfargs.errorHint();
    }
    return zfargs.success();
}
zfauto ZFObject::invokeDetail(
        ZF_IN const zfstring &methodName
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFArgs zfargs;
    zfargs
        .sender(this)
        .paramInit(params)
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    ZFDI_invoke(zfargs, methodName, zftrue);
    if(success != zfnull) {
        *success = zfargs.success();
    }
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
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
void ZFObject::observerNotifyReversely(
        ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) {
    if(d && d->observerHolder) {
        d->observerHolder->observerNotifyReversely(eventId, param0, param1);
    }
    else {
        ZFGlobalObserver().observerNotifyReverselyWithSender(this, eventId, param0, param1);
    }
}
void ZFObject::observerNotifyReverselyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) {
    if(d && d->observerHolder) {
        d->observerHolder->observerNotifyReverselyWithSender(customSender, eventId, param0, param1);
    }
    else {
        ZFGlobalObserver().observerNotifyReverselyWithSender(customSender, eventId, param0, param1);
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

void ZFObject::on(
        ZF_IN const zfstring &eventName
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    zfidentity eventId = ZFIdMapIdForName(eventName);
    if(eventId != zfidentityInvalid()) {
        return this->observerAdd(eventId, observer, observerLevel);
    }
    eventId = ZFIdMapIdForName(zfstr("%s.E_%s", this->classData()->classNameFull(), eventName));
    if(eventId != zfidentityInvalid()) {
        return this->observerAdd(eventId, observer, observerLevel);
    }
    ZFCoreArray<const ZFClass *> allParent = this->classData()->parentGetAll();
    for(zfindex i = 0; i < allParent.count(); ++i) {
        eventId = ZFIdMapIdForName(zfstr("%s.E_%s", this->classData()->classNameFull(), eventName));
        if(eventId != zfidentityInvalid()) {
            return this->observerAdd(eventId, observer, observerLevel);
        }
    }
    ZFCoreLogTrim("no such event \"%s\" for class: %s", eventName, this->classData()->classNameFull());
}

void ZFObject::observerOnAdd(ZF_IN zfidentity eventId) {
    if(zffalse) {
    }
    else if(eventId == ZFObject::E_ObjectBeforeAlloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    }
    else if(eventId == ZFObject::E_ObjectAfterAlloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::E_ObjectBeforeDealloc()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::E_ObjectPropertyValueOnUpdate()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
    else if(eventId == ZFObject::E_ObjectPropertyValueOnReset()) {
        ZFBitSet(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset);
    }
}
void ZFObject::observerOnRemove(ZF_IN zfidentity eventId) {
    if(zffalse) {
    }
    else if(eventId == ZFObject::E_ObjectBeforeAlloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc);
    }
    else if(eventId == ZFObject::E_ObjectAfterAlloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::E_ObjectBeforeDealloc()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::E_ObjectPropertyValueOnUpdate()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
    else if(eventId == ZFObject::E_ObjectPropertyValueOnReset()) {
        ZFBitUnset(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset);
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

void ZFObject::_ZFP_ZFObject_objectOnInit(void) {
    ZFCoreMutexLock();
    this->objectOnInit();
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
    this->classData()->_ZFP_ZFClass_propertyAutoInitAction(this);
    this->_ZFP_ObjI_onInitIvk();
    ZFCoreMutexUnlock();

    if(!this->objectIsInternalPrivate()) {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeAlloc)
                ) {
            this->observerNotify(ZFObject::E_ObjectBeforeAlloc());
        }
    }
}
void ZFObject::_ZFP_ZFObject_objectOnInitFinish(void) {
    this->objectOnInitFinish();
    ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInit);
    ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnInitFinish);
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateIdle);

    if(!this->objectIsInternalPrivate()) {
        if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                ) {
            this->observerNotify(ZFObject::E_ObjectAfterAlloc());
        }
    }
}
void ZFObject::_ZFP_ZFObject_objectOnInitFromCache(void) {
    if(!this->objectIsInternalPrivate()) {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
                ) {
            this->observerNotify(ZFObject::E_ObjectAfterAlloc());
        }
    }
}

void ZFObject::_ZFP_ZFObjectCheckRelease(void) {
    if(ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
            ) {
        if(_objectRetainCount == 1) {
            this->observerNotifyReversely(ZFObject::E_ObjectBeforeDealloc());
            if(_objectRetainCount > 1) {
                this->objectOnRelease();
                this->observerRemoveAll(ZFObject::E_ObjectBeforeDealloc());
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
    if(_objectRetainCount > 0) {
        ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateIdle);
        ZFBitUnset(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDeallocPrepare);
        return;
    }

    this->observerRemoveAll();
    this->_ZFP_ObjI_onDeallocIvk();
    ZFBitSet(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDealloc);
    if(d) {
        for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i) {
            const ZFProperty *property = d->propertyAccessed[i];
            if(property->_ZFP_ZFProperty_callbackDealloc) {
                property->_ZFP_ZFProperty_callbackDealloc(property, this);
            }
        }
    }
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
            if(d->weakHolder) {
                d->weakHolder->set(zfnull);
                zfRelease(d->weakHolder);
            }
        }
        else {
            if(d->weakHolder) {
                zfRelease(d->weakHolder);
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
    if(ZFBitTest(this->_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_ZFObjectInstanceStateOnDealloc)) {
        ZFCoreLogTrim("access property during object deallocation may cause logical problem, property: %s, object: %s"
                , property->propertyName()
                , this->objectInfoOfInstance()
                );
    }
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
        ZFObject *param1 = zfnull;
        if(oldValue) {
            if(property->isRetainProperty()) {
                param1 = *(const zfauto *)oldValue;
                zfunsafe_zfRetain(param1);
            }
            else {
                const ZFTypeInfo *typeInfo = ZFTypeInfoForName(property->propertyTypeId());
                if(typeInfo != zfnull) {
                    zfautoT<ZFTypeIdWrapper> v = typeInfo->typeIdClass()->newInstance();
                    v->wrappedValue(oldValue);
                    param1 = zfunsafe_zfRetain(v);
                }
                else {
                    ZFCoreLogTrim("unknown property type: %s, for object: %s", property, this->objectInfoOfInstance());
                    param1 = zfunsafe_zfRetain(ZFNull());
                }
            }
        }
        else {
            param1 = zfunsafe_zfRetain(ZFNull());
        }
        this->observerNotify(ZFObject::E_ObjectPropertyValueOnUpdate(), param0, param1);
        zfunsafe_zfRelease(param0);
        zfunsafe_zfRelease(param1);
    }
}
void ZFObject::objectPropertyValueOnReset(
        ZF_IN const ZFProperty *property
        ) {
    if(!this->objectIsInternalPrivate()
            && (ZFBitTest(_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnReset))
                ) {
        v_ZFProperty *param0 = zfunsafe_zfAlloc(v_ZFProperty);
        param0->zfv = property;
        this->observerNotify(ZFObject::E_ObjectPropertyValueOnReset(), param0);
        zfunsafe_zfRelease(param0);
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
    zfstlhashmap<const ZFClass *, zfauto>::iterator it = d->ZFImplementDynamicHolder.find(clsToImplement);
    if(it != d->ZFImplementDynamicHolder.end()) {
        return it->second;
    }
    else {
        _ZFP_ZFObjectPrivate *dObj = zfpoolNew(_ZFP_ZFObjectPrivate);
        dObj->ZFImplementDynamicOwner = this;

        // alias internal data to owner
        dObj->weakHolder = zfRetain(this->_ZFP_ZFObject_weakHolder());
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

// ============================================================
zfany ZFObjectCast(ZF_IN const ZFClass *cls, ZF_IN const zfany &obj) {
    if(cls == zfnull || obj == zfnull) {
        return zfnull;
    }
    else {
        return obj->classData()->_ZFP_ZFClass_objectCast(obj, cls);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFClass *, classData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoOfInstanceT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfoOfInstance)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_DETAIL_1({
    invokerObject->_ZFP_ZFObject_objectInfoImpl(ret);
}, ZFObject, protected, ZFMethodTypeVirtual, _, void, objectInfoImpl
, ZFMP_IN_OUT(zfstring &, ret)
)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfidentity, objectHash)
ZFMETHOD_USER_REGISTER_DETAIL_0({
    return invokerObject->_ZFP_ZFObject_objectHashImpl();
}, ZFObject, protected, ZFMethodTypeVirtual, _, zfidentity, objectHashImpl
)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectCompare
        , ZFMP_IN(ZFObject *, anotherObj)
        )
ZFMETHOD_USER_REGISTER_DETAIL_1({
    return invokerObject->_ZFP_ZFObject_objectCompareImpl(anotherObj);
}, ZFObject, protected, ZFMethodTypeVirtual, _, ZFCompareResult, objectCompareImpl
, ZFMP_IN(ZFObject *, anotherObj)
)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectCompareValue
        , ZFMP_IN(ZFObject *, anotherObj)
        )
ZFMETHOD_USER_REGISTER_DETAIL_1({
    return invokerObject->_ZFP_ZFObject_objectCompareValueImpl(anotherObj);
}, ZFObject, protected, ZFMethodTypeVirtual, _, ZFCompareResult, objectCompareValueImpl
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
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ZFObject, zfbool, invokeT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_OUT(zfstring *, errorHint)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        // ZFMETHOD_MAX_PARAM
        // , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        // , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
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
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, on
        , ZFMP_IN(const zfstring &, eventName)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, on
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, onInit
        , ZFMP_IN(const ZFListener &, impl)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, onDealloc
        , ZFMP_IN(const ZFListener &, impl)
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
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerNotifyReversely
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerNotifyReverselyWithSender
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

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfany, ZFObjectCast
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const zfany &, obj)
        )
ZFMETHOD_FUNC_USER_REGISTER_2({
        if(cls == zfnull || obj == zfnull || !obj->classData()->classIsTypeOf(cls)) {
            return zfnull;
        }
        else {
            return obj;
        }
    }, ZFObject *, zfcast
    , ZFMP_IN(const ZFClass *, cls)
    , ZFMP_IN(ZFObject *, obj)
    )

ZF_NAMESPACE_GLOBAL_END
#endif

