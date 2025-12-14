#include "ZFObserverGroup.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObserverGroupPrivate {
public:
    zfuint refCount;
    zfweak owner;
    zfweak target;
    ZFObserver *targetObserver; // auto retain within life cycle

public:
    void removeAll(void) {
        this->owner = zfnull;
        this->target = zfnull;
        if(this->targetObserver != zfnull) {
            zfpoolDelete(this->targetObserver);
            this->targetObserver = zfnull;
        }
    }

public:
    _ZFP_ZFObserverGroupPrivate(void)
    : refCount(1)
    , owner(zfnull)
    , target(zfnull)
    , targetObserver(zfnull)
    {
    }
    ~_ZFP_ZFObserverGroupPrivate(void) {
        this->removeAll();
    }
};

// ============================================================
zfclassNotPOD _ZFP_ZFObserverGroupTaskData {
public:
    ZFObject *owner;
    ZFObject *target;
    ZFObserver realTarget;
    ZFListener realObserver;
    zfidentity eventId;
    ZFListener observer;

    _ZFP_ZFObserverGroupTaskData *ownerMapPrev;
    _ZFP_ZFObserverGroupTaskData *ownerMapNext;
    _ZFP_ZFObserverGroupTaskData *targetMapPrev;
    _ZFP_ZFObserverGroupTaskData *targetMapNext;

public:
    _ZFP_ZFObserverGroupTaskData(
            ZF_IN ZFObject *owner
            , ZF_IN ZFObject *target
            , ZF_IN ZFObserver realTarget
            , ZF_IN ZFListener realObserver
            , ZF_IN zfidentity eventId
            , ZF_IN ZFListener observer
            )
    : owner(owner)
    , target(target)
    , realTarget(realTarget)
    , realObserver(realObserver)
    , eventId(eventId)
    , observer(observer)
    , ownerMapPrev(zfnull)
    , ownerMapNext(zfnull)
    , targetMapPrev(zfnull)
    , targetMapNext(zfnull)
    {
    }
    ~_ZFP_ZFObserverGroupTaskData(void) {
        this->realTarget.observerRemove(this->eventId, this->realObserver);
    }
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObserverGroupDataHolder, ZFLevelZFFrameworkEssential) {
    zfself *dataHolder = this;

    ZFLISTENER_1(ownerOnDealloc
            , zfself *, dataHolder
            ) {
        dataHolder->detachByOwner(zfargs.sender());
    } ZFLISTENER_END()
    this->ownerOnDeallocListener = ownerOnDealloc;

    ZFLISTENER_1(targetOnDealloc
            , zfself *, dataHolder
            ) {
        dataHolder->detachByTarget(zfargs.sender());
    } ZFLISTENER_END()
    this->targetOnDeallocListener = targetOnDealloc;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObserverGroupDataHolder) {
    while(!this->ownerMap.empty()) {
        this->detachByOwner(this->ownerMap.begin()->first);
    }
}
private:
    typedef zfstlhashmap<ZFObject *, _ZFP_ZFObserverGroupTaskData *> _TaskMap;
    _TaskMap ownerMap; // <owner, taskDataList>
    _TaskMap targetMap; // <target, taskDataList>

private:
    ZFListener ownerOnDeallocListener;
    ZFListener targetOnDeallocListener;

public:
    void attach(
            ZF_IN ZFObject *owner
            , ZF_IN ZFObject *target
            , ZF_IN const ZFObserver &realTarget
            , ZF_IN const ZFListener &realObserver
            , ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            ) {
        _ZFP_ZFObserverGroupTaskData *task = zfpoolNew(_ZFP_ZFObserverGroupTaskData
                , owner
                , target
                , realTarget
                , realObserver
                , eventId
                , observer
            );

        // if(owner != zfnull)
        {
            _ZFP_ZFObserverGroupTaskData **head = zfnull;
            {
                _TaskMap::iterator it = this->ownerMap.find(owner);
                if(it == this->ownerMap.end()) {
                    head = &(this->ownerMap[owner]);
                    *head = zfnull;
                    owner->observerAdd(ZFObject::E_ObjectBeforeDealloc(), this->ownerOnDeallocListener);
                }
                else {
                    head = &(it->second);
                }
            }
            task->ownerMapNext = *head;
            if(*head != zfnull) {
                (*head)->ownerMapPrev = task;
            }
            *head = task;
        }

        if(target != zfnull) {
            _ZFP_ZFObserverGroupTaskData **head = zfnull;
            {
                _TaskMap::iterator it = this->targetMap.find(target);
                if(it == this->targetMap.end()) {
                    head = &(this->targetMap[target]);
                    *head = zfnull;
                    target->observerAdd(ZFObject::E_ObjectBeforeDealloc(), this->targetOnDeallocListener);
                }
                else {
                    head = &(it->second);
                }
            }
            task->targetMapNext = *head;
            if(*head != zfnull) {
                (*head)->targetMapPrev = task;
            }
            *head = task;
        }
    }
    void detachByOwner(ZF_IN ZFObject *owner) {
        _TaskMap::iterator it = this->ownerMap.find(owner);
        if(it == this->ownerMap.end()) {
            return;
        }
        _ZFP_ZFObserverGroupTaskData *head = it->second;
        this->ownerMap.erase(it);
        owner->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->ownerOnDeallocListener);

        _ZFP_ZFObserverGroupTaskData *p = head;
        while(p != zfnull) {
            if(p->target != zfnull && p->targetMapPrev == zfnull) {
                if(p->targetMapNext != zfnull) {
                    this->targetMap.find(p->target)->second = p->targetMapNext;
                }
                else {
                    this->targetMap.erase(p->target);
                    p->target->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->targetOnDeallocListener);
                }
            }

            if(p->targetMapPrev != zfnull) {
                p->targetMapPrev->targetMapNext = p->targetMapNext;
            }
            if(p->targetMapNext != zfnull) {
                p->targetMapNext->targetMapPrev = p->targetMapPrev;
            }

            p = p->ownerMapNext;
        }
        while(head != zfnull) {
            p = head;
            head = head->ownerMapNext;
            zfpoolDelete(p);
        }
    }
    void detachByTarget(ZF_IN ZFObject *target) {
        _TaskMap::iterator it = this->targetMap.find(target);
        if(it == this->targetMap.end()) {
            return;
        }
        _ZFP_ZFObserverGroupTaskData *head = it->second;
        this->targetMap.erase(it);
        target->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->targetOnDeallocListener);

        _ZFP_ZFObserverGroupTaskData *p = head;
        while(p != zfnull) {
            if(p->owner != zfnull && p->ownerMapPrev == zfnull) {
                if(p->ownerMapNext != zfnull) {
                    this->ownerMap.find(p->owner)->second = p->ownerMapNext;
                }
                else {
                    this->ownerMap.erase(p->owner);
                    p->owner->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->ownerOnDeallocListener);
                }
            }

            if(p->ownerMapPrev != zfnull) {
                p->ownerMapPrev->ownerMapNext = p->ownerMapNext;
            }
            if(p->ownerMapNext != zfnull) {
                p->ownerMapNext->ownerMapPrev = p->ownerMapPrev;
            }

            p = p->targetMapNext;
        }
        while(head != zfnull) {
            p = head;
            head = head->targetMapNext;
            zfpoolDelete(p);
        }
    }
    void detachExact(
            ZF_IN ZFObject *owner
            , ZF_IN ZFObject *target
            , ZF_IN const ZFObserver &realTarget
            , ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            ) {
        if(owner != zfnull) {
            _TaskMap::iterator it = this->ownerMap.find(owner);
            if(it != this->ownerMap.end()) {
                _ZFP_ZFObserverGroupTaskData *head = it->second;
                while(head != zfnull) {
                    if(head->realTarget == realTarget
                        && head->eventId == eventId
                        && head->observer == observer
                    ) {
                        this->detach(head);
                        return;
                    }
                    head = head->ownerMapNext;
                }
            }
        }
        else if(target != zfnull) {
            _TaskMap::iterator it = this->targetMap.find(target);
            if(it != this->targetMap.end()) {
                _ZFP_ZFObserverGroupTaskData *head = it->second;
                while(head != zfnull) {
                    if(head->realTarget == realTarget
                        && head->eventId == eventId
                        && head->observer == observer
                    ) {
                        this->detach(head);
                        return;
                    }
                    head = head->targetMapNext;
                }
            }
        }
        else {
            for(_TaskMap::iterator it = this->ownerMap.begin(); it != this->ownerMap.end(); ++it) {
                _ZFP_ZFObserverGroupTaskData *head = it->second;
                while(head != zfnull) {
                    if(head->realTarget == realTarget
                        && head->eventId == eventId
                        && head->observer == observer
                    ) {
                        this->detach(head);
                        return;
                    }
                    head = head->ownerMapNext;
                }
            }
        }
    }
private:
    void detach(ZF_IN _ZFP_ZFObserverGroupTaskData *task) {
        if(task->owner != zfnull) {
            _TaskMap::iterator it = this->ownerMap.find(task->owner);
            ZFCoreAssert(it != this->ownerMap.end());
            if(it->second == task) {
                if(task->ownerMapNext == zfnull) {
                    this->ownerMap.erase(it);
                    task->owner->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->ownerOnDeallocListener);
                }
                else {
                    it->second = task->ownerMapNext;
                }
            }

            if(task->ownerMapPrev != zfnull) {
                task->ownerMapPrev->ownerMapNext = task->ownerMapNext;
            }
            if(task->ownerMapNext != zfnull) {
                task->ownerMapNext->ownerMapPrev = task->ownerMapPrev;
            }
        }

        if(task->target != zfnull) {
            _TaskMap::iterator it = this->targetMap.find(task->target);
            ZFCoreAssert(it != this->targetMap.end());
            if(it->second == task) {
                if(task->targetMapNext == zfnull) {
                    this->targetMap.erase(it);
                    task->target->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->targetOnDeallocListener);
                }
                else {
                    it->second = task->targetMapNext;
                }
            }

            if(task->targetMapPrev != zfnull) {
                task->targetMapPrev->targetMapNext = task->targetMapNext;
            }
            if(task->targetMapNext != zfnull) {
                task->targetMapNext->targetMapPrev = task->targetMapPrev;
            }
        }

        zfpoolDelete(task);
    }
ZF_GLOBAL_INITIALIZER_END(ZFObserverGroupDataHolder)

// ============================================================
const ZFObserverGroup &ZFObserverGroup::observerAdd(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) const {
    ZFCoreMutexLocker();
    ZFObject *owner = d->owner;
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return *this;
    }

    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    realTarget.observerAdd(eventId, observer, observerLevel);

    if(owner != zfnull) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->attach(owner, target, realTarget, observer, eventId, observer);
    }
    return *this;
}
const ZFObserverGroup &ZFObserverGroup::observerAddForOnce(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) const {
    ZFCoreMutexLocker();
    ZFObject *owner = d->owner;
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return *this;
    }

    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    ZFLISTENER_5(realObserver
            , ZFObject *, owner
            , ZFObject *, target
            , ZFObserver, realTarget
            , zfidentity, eventId
            , ZFListener, observer
            ) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->detachExact(owner, target, realTarget, eventId, observer);
        observer.execute(zfargs);
    } ZFLISTENER_END()
    realTarget.observerAddForOnce(eventId, realObserver, observerLevel);

    if(owner != zfnull) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->attach(owner, target, realTarget, realObserver, eventId, observer);
    }
    return *this;
}

void ZFObserverGroup::observerRemove(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &callback
        ) const {
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return;
    }
    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->detachExact(d->owner, target, realTarget, eventId, callback);
}
void ZFObserverGroup::observerRemoveAll(ZF_IN zfidentity eventId) const {
    ZFCoreCriticalNotSupported();
}
void ZFObserverGroup::observerRemoveAll(void) const {
    ZFCoreCriticalNotSupported();
}
zfbool ZFObserverGroup::observerHasAdd(void) const {
    ZFCoreCriticalNotSupported();
    return zffalse;
}
zfbool ZFObserverGroup::observerHasAdd(ZF_IN zfidentity eventId) const {
    ZFCoreCriticalNotSupported();
    return zffalse;
}
void ZFObserverGroup::observerNotify(
        ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return;
    }
    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    realTarget.observerNotify(eventId, param0, param1);
}
void ZFObserverGroup::observerNotifyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return;
    }
    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    realTarget.observerNotifyWithSender(customSender, eventId, param0, param1);
}
void ZFObserverGroup::observerNotifyReversely(
        ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return;
    }
    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    realTarget.observerNotifyReversely(eventId, param0, param1);
}
void ZFObserverGroup::observerNotifyReverselyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    ZFObject *target = d->target;
    if(target == zfnull && d->targetObserver == zfnull) {
        return;
    }
    ZFObserver &realTarget = (target != zfnull ? target->observerHolder() : *(d->targetObserver));
    realTarget.observerNotifyReverselyWithSender(customSender, eventId, param0, param1);
}

const ZFObserverGroup &ZFObserverGroup::on(
        ZF_IN const zfstring &eventName
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) const {
    if(d->target) {
        d->target->on(eventName, observer, observerLevel);
    }
    else if(d->targetObserver) {
        d->targetObserver->on(eventName, observer, observerLevel);
    }
    return *this;
}

ZFObserverGroup::ZFObserverGroup(
        ZF_IN ZFObject *owner
        , ZF_IN const ZFObserver &target
        )
: d(zfpoolNew(_ZFP_ZFObserverGroupPrivate))
{
    this->_ZFP_update(owner, target);
}
ZFObserverGroup::ZFObserverGroup(
        ZF_IN ZFObject *owner
        , ZF_IN ZFObject *target
        )
: d(zfpoolNew(_ZFP_ZFObserverGroupPrivate))
{
    this->_ZFP_update(owner, target);
}

ZFObserverGroup::ZFObserverGroup(void)
: d(zfpoolNew(_ZFP_ZFObserverGroupPrivate))
{
}
ZFObserverGroup::ZFObserverGroup(ZF_IN const ZFObserverGroup &ref) {
    d = ref.d;
    ++(d->refCount);
}
ZFObserverGroup::~ZFObserverGroup(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
}

ZFObserverGroup &ZFObserverGroup::operator = (ZF_IN const ZFObserverGroup &ref) {
    _ZFP_ZFObserverGroupPrivate *dTmp = d;
    ++(ref.d->refCount);
    d = ref.d;
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfpoolDelete(dTmp);
    }
    return *this;
}

void ZFObserverGroup::_ZFP_update(
        ZF_IN ZFObject *owner
        , ZF_IN const ZFObserver &target
        ) {
    ZFCoreAssert(owner != zfnull);

    d->owner = owner;
    d->target = zfnull;
    if(d->targetObserver != zfnull) {
        *(d->targetObserver) = target;
    }
    else {
        d->targetObserver = zfpoolNew(ZFObserver, target);
    }
}
void ZFObserverGroup::_ZFP_update(
        ZF_IN ZFObject *owner
        , ZF_IN ZFObject *target
        ) {
    ZFCoreAssert(owner != zfnull);

    if(target != zfnull) {
        d->owner = owner;
        d->target = target;
        if(d->targetObserver != zfnull) {
            zfpoolDelete(d->targetObserver);
            d->targetObserver = zfnull;
        }
    }
    else {
        d->removeAll();
    }
}

void ZFObserverGroup::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFObserverGroup:";
    if(d->targetObserver != zfnull) {
        zfstringAppend(ret, "<ZFObserver(%s)>", (const void *)d->targetObserver);
    }
    else if(d->target != zfnull) {
        d->target->objectInfoOfInstanceT(ret);
    }
    ret += ">";
}

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFObserverGroup, ZFObserverGroup)

ZFOBJECT_ON_INIT_USER_REGISTER_2({
        invokerObject.to<v_ZFObserverGroup *>()->zfv._ZFP_update(owner, target);
    }, v_ZFObserverGroup
    , ZFMP_IN(ZFObject *, owner)
    , ZFMP_IN(const ZFObserver &, target)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        invokerObject.to<v_ZFObserverGroup *>()->zfv._ZFP_update(owner, target);
    }, v_ZFObserverGroup
    , ZFMP_IN(ZFObject *, owner)
    , ZFMP_IN(ZFObject *, target)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, const ZFObserverGroup &, observerAdd
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, const ZFObserverGroup &, observerAddForOnce
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFObserverGroup, void, observerRemove
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverGroup, void, observerRemoveAll
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverGroup, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverGroup, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverGroup, zfbool, observerHasAdd
        , ZFMP_IN(zfidentity, eventId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, void, observerNotify
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverGroup, void, observerNotifyWithSender
        , ZFMP_IN(ZFObject *, customSender)
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, void, observerNotifyReversely
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverGroup, void, observerNotifyReverselyWithSender
        , ZFMP_IN(ZFObject *, customSender)
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
        , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, const ZFObserverGroup &, on
        , ZFMP_IN(const zfstring &, eventName)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroup, const ZFObserverGroup &, on
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFObserverGroupRemove
        , ZFMP_IN(ZFObject *, owner)
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->detachByOwner(owner);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalObserverGroup, ZFLevelZFFrameworkEssential) {
}
zfobj<ZFObject> owner;
ZF_GLOBAL_INITIALIZER_END(ZFGlobalObserverGroup)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalObserverGroupAutoClean, ZFLevelAppNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFGlobalObserverGroupAutoClean) {
    ZFObserverGroupRemove(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalObserverGroup)->owner);
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalObserverGroupAutoClean)

ZFMETHOD_FUNC_DEFINE_1(ZFObserverGroup, ZFGlobalObserverGroup
        , ZFMP_IN(const ZFObserver &, target)
        ) {
    return ZFObserverGroup(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalObserverGroup)->owner, target);
}
ZFMETHOD_FUNC_DEFINE_1(ZFObserverGroup, ZFGlobalObserverGroup
        , ZFMP_IN(ZFObject *, target)
        ) {
    return ZFObserverGroup(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalObserverGroup)->owner, target);
}

ZF_NAMESPACE_GLOBAL_END
