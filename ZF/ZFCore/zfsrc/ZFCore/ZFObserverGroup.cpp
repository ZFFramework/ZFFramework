#include "ZFObserverGroup.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObserverGroupHolderPrivate {
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
            zfdelete(this->targetObserver);
            this->targetObserver = zfnull;
        }
    }

public:
    _ZFP_ZFObserverGroupHolderPrivate(void)
    : refCount(1)
    , owner(zfnull)
    , target(zfnull)
    , targetObserver(zfnull)
    {
    }
    ~_ZFP_ZFObserverGroupHolderPrivate(void) {
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
    typedef zfstlmap<ZFObject *, _ZFP_ZFObserverGroupTaskData *> _TaskMap;
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
        _ZFP_ZFObserverGroupTaskData *task = zfnew(_ZFP_ZFObserverGroupTaskData
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
            this->cleanup(p);
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
            this->cleanup(p);
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

        this->cleanup(task);
    }
    void cleanup(ZF_IN _ZFP_ZFObserverGroupTaskData *task) {
        task->realTarget.observerRemove(task->eventId, task->realObserver);
        zfdelete(task);
    }
ZF_GLOBAL_INITIALIZER_END(ZFObserverGroupDataHolder)

// ============================================================
const ZFObserverGroupHolder &ZFObserverGroupHolder::observerAdd(
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
const ZFObserverGroupHolder &ZFObserverGroupHolder::observerAddForOnce(
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
    } ZFLISTENER_END()
    realTarget.observerAddForOnce(eventId, realObserver, observerLevel);

    if(owner != zfnull) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->attach(owner, target, realTarget, realObserver, eventId, observer);
    }
    return *this;
}

ZFObserverGroupHolder::ZFObserverGroupHolder(void)
: d(zfnew(_ZFP_ZFObserverGroupHolderPrivate))
{
}
ZFObserverGroupHolder::ZFObserverGroupHolder(ZF_IN const ZFObserverGroupHolder &ref) {
    d = ref.d;
    ++(d->refCount);
}
ZFObserverGroupHolder::~ZFObserverGroupHolder(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}

ZFObserverGroupHolder &ZFObserverGroupHolder::operator = (ZF_IN const ZFObserverGroupHolder &ref) {
    _ZFP_ZFObserverGroupHolderPrivate *dTmp = d;
    ++(ref.d->refCount);
    d = ref.d;
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfdelete(dTmp);
    }
    return *this;
}

void ZFObserverGroupHolder::_ZFP_update(
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
        d->targetObserver = zfnew(ZFObserver, target);
    }
}
void ZFObserverGroupHolder::_ZFP_update(
        ZF_IN ZFObject *owner
        , ZF_IN ZFObject *target
        ) {
    ZFCoreAssert(owner != zfnull);

    if(target != zfnull) {
        d->owner = owner;
        d->target = target;
        if(d->targetObserver != zfnull) {
            zfdelete(d->targetObserver);
            d->targetObserver = zfnull;
        }
    }
    else {
        d->removeAll();
    }
}

void ZFObserverGroupHolder::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFObserverGroupHolder:";
    if(d->targetObserver != zfnull) {
        zfstringAppend(ret, "<ZFObserver(%s)>", (const void *)d->targetObserver);
    }
    else if(d->target != zfnull) {
        d->target->objectInfoOfInstanceT(ret);
    }
    ret += ">";
}

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFObserverGroupHolder, ZFObserverGroupHolder)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroupHolder, const ZFObserverGroupHolder &, observerAdd
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroupHolder, const ZFObserverGroupHolder &, observerAddForOnce
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverGroupHolder, const ZFObserverGroupHolder &, on
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, observer)
        , ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFObserverGroupHolder, ZFObserverGroup
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(const ZFObserver &, target)
        ) {
    ZFObserverGroupHolder ret;
    ret._ZFP_update(owner, target);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFObserverGroupHolder, ZFObserverGroup
        , ZFMP_IN(ZFObject *, owner)
        , ZFMP_IN(ZFObject *, target)
        ) {
    ZFObserverGroupHolder ret;
    ret._ZFP_update(owner, target);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFObserverGroupRemove
        , ZFMP_IN(ZFObject *, owner)
        ) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverGroupDataHolder)->detachByOwner(owner);
}

ZF_NAMESPACE_GLOBAL_END
