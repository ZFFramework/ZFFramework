#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFObserverHolder types
zfclassLikePOD _ZFP_ZFObserverData
{
public:
    zfidentity taskId;
    zfidentity eventId;
    ZFListener observer;
    ZFObject *owner;
    ZFLevel observerLevel;
    zfbool autoRemoveAfterActivate;
    _ZFP_ZFObserverData *pPrev;
    _ZFP_ZFObserverData *pNext;

public:
    explicit _ZFP_ZFObserverData(ZF_IN zfidentity taskId,
                                 ZF_IN zfidentity eventId,
                                 ZF_IN const ZFListener &observer,
                                 ZF_IN ZFObject *owner,
                                 ZF_IN ZFLevel observerLevel,
                                 ZF_IN zfbool autoRemoveAfterActivate)
    : taskId(taskId)
    , eventId(eventId)
    , observer(observer)
    , owner(owner)
    , observerLevel(observerLevel)
    , autoRemoveAfterActivate(autoRemoveAfterActivate)
    , pPrev(zfnull)
    , pNext(zfnull)
    {
    }
private:
    _ZFP_ZFObserverData(ZF_IN const _ZFP_ZFObserverData &ref);
    _ZFP_ZFObserverData &operator = (ZF_IN const _ZFP_ZFObserverData &ref);
};

zfclassPOD _ZFP_ZFObserverHolderAttachState
{
public:
    zfuint *flag;
    zft_zfuint flagBit;
};
typedef zfstlmap<zfidentity, zfstldeque<_ZFP_ZFObserverHolderAttachState> > _ZFP_ZFObserverHolderAttachStateMapType;

zfclassNotPOD _ZFP_ZFObserverHolderPrivate
{
public:
    zfuint refCount;
    ZFObject *observerOwner;
    zfstlmap<zfidentity, _ZFP_ZFObserverData *> observerMap; // <eventId, pList>
    zfstlmap<zfidentity, _ZFP_ZFObserverData *> observerTaskIdMap; // <taskId, p>
    zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> > observerOwnerMap; // <owner, <p, eventId> >
    ZFIdentityGenerator taskIdGenerator;
    _ZFP_ZFObserverHolderAttachStateMapType attachMap;
public:
    _ZFP_ZFObserverHolderPrivate(void)
    : refCount(1)
    , observerOwner(zfnull)
    , observerMap()
    , observerTaskIdMap()
    , observerOwnerMap()
    , taskIdGenerator()
    , attachMap()
    {
    }

public:
    void attachMapAttach(ZF_IN zfidentity eventId)
    {
        _ZFP_ZFObserverHolderAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObserverHolderAttachState &state = it->second[i];
                ZFBitSet(*state.flag, state.flagBit);
            }
        }
    }
    void attachMapDetach(ZF_IN zfidentity eventId)
    {
        _ZFP_ZFObserverHolderAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObserverHolderAttachState &state = it->second[i];
                ZFBitUnset(*state.flag, state.flagBit);
            }
        }
    }
    void observerNotifyPrepare(ZF_IN_OUT zfstldeque<_ZFP_ZFObserverData *> &toNotify,
                               ZF_IN_OUT _ZFP_ZFObserverData *&toDelete,
                               ZF_IN zfidentity eventId,
                               ZF_IN ZFObject *observerOwner)
    {
        zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            _ZFP_ZFObserverData *p = it->second;
            do
            {
                toNotify.push_back(p);
                if(p->autoRemoveAfterActivate)
                {
                    this->observerDetach(it, p);

                    if(toDelete == zfnull)
                    {
                        toDelete = p;
                        toDelete->pPrev = toDelete->pNext = zfnull;
                    }
                    else
                    {
                        p->pPrev = zfnull;
                        p->pNext = toDelete;
                        toDelete->pPrev = p;
                        toDelete = p;
                    }
                }
                p = p->pNext;
            } while(p != zfnull);

            if(it->second == zfnull)
            {
                this->observerMap.erase(it);
                this->attachMapDetach(eventId);
                if(observerOwner)
                {
                    observerOwner->observerOnRemove(eventId);
                }
            }
        }
    }
    void observerDetach(ZF_IN_OUT _ZFP_ZFObserverData *p)
    {
        this->taskIdGenerator.idRelease(p->taskId);
        this->observerTaskIdMap.erase(p->taskId);
        if(p->owner != zfnull)
        {
            zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> >::iterator itTmp = this->observerOwnerMap.find(p->owner);
            itTmp->second.erase(p);
            if(itTmp->second.empty())
            {
                this->observerOwnerMap.erase(itTmp);
            }
        }
    }
    void observerDetach(ZF_IN_OUT zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator &it,
                        ZF_IN_OUT _ZFP_ZFObserverData *p)
    {
        this->observerDetach(p);

        if(it->second == p)
        {
            it->second = p->pNext;
        }

        if(p->pPrev != zfnull)
        {
            p->pPrev->pNext = p->pNext;
        }
        if(p->pNext != zfnull)
        {
            p->pNext->pPrev = p->pPrev;
        }
    }
};

// ============================================================
// ZFObserverHolder
ZFObserverHolder::ZFObserverHolder(void)
: d(zfpoolNew(_ZFP_ZFObserverHolderPrivate))
{
}
ZFObserverHolder::ZFObserverHolder(ZF_IN ZFObserverHolder const &ref)
{
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
}
ZFObserverHolder::~ZFObserverHolder(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfpoolDelete(d);
    }
}
ZFObserverHolder &ZFObserverHolder::operator = (ZF_IN ZFObserverHolder const &ref)
{
    _ZFP_ZFObserverHolderPrivate *dTmp = d;
    if(ref.d)
    {
        ++(ref.d->refCount);
    }
    d = ref.d;
    if(dTmp)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFObserverHolder::operator == (ZF_IN ZFObserverHolder const &ref) const
{
    return (d == ref.d);
}

zfidentity ZFObserverHolder::observerAdd(ZF_IN zfidentity eventId,
                                         ZF_IN const ZFListener &observer,
                                         ZF_IN_OPT ZFObject *owner /* = zfnull */,
                                         ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                                         ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */)
{
    zfCoreMutexLocker();

    if(eventId == zfidentityInvalid()
        || !observer)
    {
        return zfidentityInvalid();
    }
    if(d->observerOwner && ZFBitTest(d->observerOwner->objectInstanceState(), ZFObjectInstanceStateOnDealloc))
    {
        zfCoreCriticalMessageTrim("[ZFObject] you must not add observer while object is deallocating, class: %s, event: %s",
            d->observerOwner->classData()->classNameFull(),
            ZFIdMapNameForId(eventId));
        return zfidentityInvalid();
    }

    d->attachMapAttach(eventId);

    zfidentity taskId = d->taskIdGenerator.idAcquire();
    _ZFP_ZFObserverData *t = zfpoolNew(_ZFP_ZFObserverData
            , taskId
            , eventId
            , observer
            , owner
            , observerLevel
            , autoRemoveAfterActivate
        );
    d->observerTaskIdMap[taskId] = t;
    if(owner != zfnull)
    {
        d->observerOwnerMap[owner][t] = eventId;
    }

    _ZFP_ZFObserverData *&head = d->observerMap[eventId];
    if(head == zfnull || head->observerLevel > observerLevel)
    {
        t->pNext = head;
        if(head != zfnull)
        {
            head->pPrev = t;
        }
        head = t;
        if(d->observerOwner != zfnull)
        {
            d->observerOwner->observerOnAdd(eventId);
        }
    }
    else
    {
        _ZFP_ZFObserverData *p = head;
        while(p->pNext != zfnull && p->pNext->observerLevel <= observerLevel)
        {
            p = p->pNext;
        }
        if(p->pNext != zfnull)
        {
            p->pNext->pPrev = t;
        }
        t->pNext = p->pNext;
        t->pPrev = p;
        p->pNext = t;
    }

    return taskId;
}
zfidentity ZFObserverHolder::observerAdd(ZF_IN const ZFObserverAddParam &param)
{
    return this->observerAdd(
        param.eventId(),
        param.observer(),
        param.owner(),
        param.autoRemoveAfterActivate(),
        param.observerLevel());
}
void ZFObserverHolder::observerMoveToFirst(ZF_IN zfidentity taskId)
{
    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator itTaskId = d->observerTaskIdMap.find(taskId);
    if(itTaskId == d->observerTaskIdMap.end())
    {
        return ;
    }

    _ZFP_ZFObserverData *p = itTaskId->second;
    _ZFP_ZFObserverData *pos = p;
    while(pos->pPrev != zfnull)
    {
        if(pos->pPrev->observerLevel == p->observerLevel)
        {
            pos = pos->pPrev;
        }
        else
        {
            break;
        }
    }
    if(pos == p)
    {
        return ;
    }
    p->pPrev->pNext = p->pNext;
    if(p->pNext != zfnull)
    {
        p->pNext->pPrev = p->pPrev;
    }
    if(pos->pPrev != zfnull)
    {
        pos->pPrev->pNext = p;
    }
    p->pPrev = pos->pPrev;
    pos->pPrev = p;
    p->pNext = pos;
}
void ZFObserverHolder::observerRemove(ZF_IN zfidentity eventId,
                                      ZF_IN const ZFListener &callback)
{
    zfCoreMutexLocker();

    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end())
    {
        _ZFP_ZFObserverData *p = it->second;
        do
        {
            if(p->observer.objectCompareByInstance(callback) == ZFCompareTheSame)
            {
                d->observerDetach(it, p);

                if(it->second == zfnull)
                {
                    d->observerMap.erase(it);
                    d->attachMapDetach(eventId);
                    if(d->observerOwner)
                    {
                        d->observerOwner->observerOnRemove(eventId);
                    }
                }
                zfpoolDelete(p);
                break;
            }
            p = p->pNext;
        } while(p != zfnull);
    }
}
void ZFObserverHolder::observerRemoveByTaskId(ZF_IN zfidentity taskId)
{
    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator itTaskId = d->observerTaskIdMap.find(taskId);
    if(itTaskId == d->observerTaskIdMap.end())
    {
        return ;
    }

    _ZFP_ZFObserverData *p = itTaskId->second;
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(p->eventId);
    d->observerDetach(it, p);
    if(it->second == zfnull)
    {
        d->observerMap.erase(it);
        d->attachMapDetach(p->eventId);
        if(d->observerOwner)
        {
            d->observerOwner->observerOnRemove(p->taskId);
        }
    }
    zfpoolDelete(p);
}
void ZFObserverHolder::observerRemoveByOwner(ZF_IN ZFObject *owner)
{
    zfCoreMutexLocker();
    zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> >::iterator itOwner = d->observerOwnerMap.find(owner);
    if(itOwner == d->observerOwnerMap.end())
    {
        return ;
    }

    zfstlmap<_ZFP_ZFObserverData *, zfidentity> toRemove = itOwner->second;
    for(zfstlmap<_ZFP_ZFObserverData *, zfidentity>::iterator itToRemove = toRemove.begin(); itToRemove != toRemove.end(); ++itToRemove)
    {
        zfidentity eventId = itToRemove->second;
        _ZFP_ZFObserverData *p = itToRemove->first;
        zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
        if(it != d->observerMap.end())
        {
            d->observerDetach(it, p);
            if(it->second == zfnull)
            {
                d->observerMap.erase(it);
                if(d->observerOwner)
                {
                    d->attachMapDetach(eventId);
                    d->observerOwner->observerOnRemove(eventId);
                }
            }
        }
        zfpoolDelete(p);
    }
}
void ZFObserverHolder::observerRemoveAll(ZF_IN zfidentity eventId)
{
    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it == d->observerMap.end())
    {
        return ;
    }
    zfstldeque<_ZFP_ZFObserverData *> toDelete;
    while(it->second != zfnull)
    {
        toDelete.push_back(it->second);
        d->observerDetach(it, it->second);
    }
    d->observerMap.erase(it);

    d->attachMapDetach(eventId);
    if(d->observerOwner)
    {
        d->observerOwner->observerOnRemove(eventId);
    }

    for(zfstlsize i = toDelete.size() - 1; i != (zfstlsize)-1; --i)
    {
        zfpoolDelete(toDelete[i]);
    }
}
void ZFObserverHolder::observerRemoveAll(void)
{
    if(d->observerMap.empty())
    {
        return ;
    }

    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *> tmp;
    tmp.swap(d->observerMap);
    d->observerMap.clear();
    d->observerTaskIdMap.clear();
    d->observerOwnerMap.clear();

    if(d->observerOwner)
    {
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
            d->observerOwner->observerOnRemove(it->first);
        }
    }
    else
    {
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
        }
    }

    for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
        it != tmp.end();
        ++it)
    {
        _ZFP_ZFObserverData *p = it->second;
        do
        {
            _ZFP_ZFObserverData *t = p;
            p = p->pNext;
            zfpoolDelete(t);
        } while(p != zfnull);
    }
}
zfbool ZFObserverHolder::observerHasAdd(void) const
{
    if(d->observerOwner != zfnull)
    {
        return !d->observerMap.empty()
            || ZFGlobalObserver().observerHasAdd();
    }
    else
    {
        return !d->observerMap.empty();
    }
}
zfbool ZFObserverHolder::observerHasAdd(ZF_IN zfidentity eventId) const
{
    if(d->observerOwner != zfnull)
    {
        return (d->observerMap.find(eventId) != d->observerMap.end())
            || ZFGlobalObserver().observerHasAdd(eventId);
    }
    else
    {
        return (d->observerMap.find(eventId) != d->observerMap.end());
    }
}

void ZFObserverHolder::observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                                      ZF_IN zfidentity eventId,
                                                      ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                                      ZF_IN_OPT ZFObject *param1 /* = zfnull */) const
{
    if(eventId == zfidentityInvalid())
    {
        return ;
    }

    zfCoreMutexLock();
    zfstldeque<_ZFP_ZFObserverData *> toNotify;
    _ZFP_ZFObserverData *toDelete = zfnull;

    d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    ZFArgs zfargs;
    zfargs
        .eventId(eventId)
        .sender(customSender)
        .param0(param0)
        .param1(param1)
        .eventFilterEnable(zftrue)
        ;
    if(d->observerOwner != zfnull)
    {
        d->observerOwner->observerOnEvent(zfargs);
        ZFGlobalObserver().d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    }
    zfCoreMutexUnlock();

    if(!toNotify.empty())
    {
        for(zfstlsize i = 0; i < toNotify.size() && !zfargs.eventFiltered(); ++i)
        {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(zfargs
                    .userData(observerData.observer.userData())
                );
        }
    }

    if(toDelete != zfnull)
    {
        zfCoreMutexLock();
        do
        {
            _ZFP_ZFObserverData *t = toDelete;
            toDelete = toDelete->pNext;
            zfpoolDelete(t);
        } while(toDelete != zfnull);
        zfCoreMutexUnlock();
    }
}

void ZFObserverHolder::observerHasAddStateAttach(ZF_IN zfidentity eventId,
                                                 ZF_IN_OUT zfuint *flag,
                                                 ZF_IN_OUT zfuint flagBit)
{
    _ZFP_ZFObserverHolderAttachState state;
    state.flag = flag;
    state.flagBit = flagBit;
    d->attachMap[eventId].push_back(state);
}
void ZFObserverHolder::observerHasAddStateDetach(ZF_IN zfidentity eventId,
                                                 ZF_IN_OUT zfuint *flag,
                                                 ZF_IN_OUT zfuint flagBit)
{
    _ZFP_ZFObserverHolderAttachStateMapType::iterator it = d->attachMap.find(eventId);
    if(it != d->attachMap.end())
    {
        for(zfstlsize i = 0; i < it->second.size(); ++i)
        {
            _ZFP_ZFObserverHolderAttachState &state = it->second[i];
            if(state.flag == flag && state.flagBit == flagBit)
            {
                it->second.erase(it->second.begin() + i);
                break;
            }
        }
    }
}

void ZFObserverHolder::objectInfoT(ZF_OUT zfstring &ret) const
{
    zfCoreMutexLocker();
    ret += "<ZFObserverHolder";

    if(d->observerOwner != zfnull)
    {
        ret += " ";
        d->observerOwner->objectInfoT(ret);
    }

    if(d->observerMap.empty())
    {
        ret += ">";
    }
    else
    {
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.begin();
            it != d->observerMap.end();
            ++it)
        {
            ret += "\n  ";
            ret += ZFIdMapNameForId(it->first);
            ret += ":";

            _ZFP_ZFObserverData *p = it->second;
            do
            {
                ret += "\n    ";
                ret += p->observer.objectInfo();
                p = p->pNext;
            } while(p != zfnull);
        }

        ret += "\n  >";
    }
}

ZFObject *ZFObserverHolder::observerOwner(void) const
{
    return d->observerOwner;
}
void ZFObserverHolder::_ZFP_ZFObserverHolder_observerOwner(ZF_IN ZFObject *obj)
{
    /*
     * if observerOwner is not null,
     * means it belongs to a ZFObject, which would:
     * * also send all events to ZFGlobalObserver
     * * notify events to owner ZFObject, such as observerOnEvent
     */
    d->observerOwner = obj;
}

// ============================================================
ZFObserverHolder &ZFGlobalObserver(void)
{
    static ZFObserverHolder d;
    return d;
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZF_NAMESPACE_END_WITH_REGISTER(ZFGlobalEvent, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFObserverHolder &, ZFGlobalObserver)

ZF_NAMESPACE_GLOBAL_END
#endif

