#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFObserver types
zfclassLikePOD _ZFP_ZFObserverData {
public:
    zfidentity eventId;
    ZFListener observer;
    ZFLevel observerLevel;
    zfbool once;
    _ZFP_ZFObserverData *pPrev;
    _ZFP_ZFObserverData *pNext;

public:
    explicit _ZFP_ZFObserverData(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN ZFLevel observerLevel
            , ZF_IN zfbool once
            )
    : eventId(eventId)
    , observer(observer)
    , observerLevel(observerLevel)
    , once(once)
    , pPrev(zfnull)
    , pNext(zfnull)
    {
    }
private:
    _ZFP_ZFObserverData(ZF_IN const _ZFP_ZFObserverData &ref);
    _ZFP_ZFObserverData &operator = (ZF_IN const _ZFP_ZFObserverData &ref);
};

zfclassPOD _ZFP_ZFObserverAttachState {
public:
    zfuint *flag;
    zft_zfuint flagBit;
};
typedef zfstlhashmap<zfidentity, zfstldeque<_ZFP_ZFObserverAttachState> > _ZFP_ZFObserverAttachStateMapType;

zfclassNotPOD _ZFP_ZFObserverPrivate {
public:
    zfuint refCount;
    ZFObject *observerOwner;
    zfstlhashmap<zfidentity, _ZFP_ZFObserverData *> observerMap; // <eventId, pList>
    _ZFP_ZFObserverAttachStateMapType attachMap;
public:
    _ZFP_ZFObserverPrivate(void)
    : refCount(1)
    , observerOwner(zfnull)
    , observerMap()
    , attachMap()
    {
    }

public:
    void attachMapAttach(ZF_IN zfidentity eventId) {
        _ZFP_ZFObserverAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end()) {
            for(zfstlsize i = 0; i < it->second.size(); ++i) {
                _ZFP_ZFObserverAttachState &state = it->second[i];
                ZFBitSet(*state.flag, state.flagBit);
            }
        }
    }
    void attachMapDetach(ZF_IN zfidentity eventId) {
        _ZFP_ZFObserverAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end()) {
            for(zfstlsize i = 0; i < it->second.size(); ++i) {
                _ZFP_ZFObserverAttachState &state = it->second[i];
                ZFBitUnset(*state.flag, state.flagBit);
            }
        }
    }

public:
    void observerAdd(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN ZFLevel observerLevel
            , ZF_IN zfbool once
            ) {
        ZFCoreMutexLocker();

        if(eventId == zfidentityInvalid()
                || !observer
                ) {
            return;
        }
        if(this->observerOwner && this->observerOwner->objectDeallocRunning()) {
            ZFCoreCriticalMessageTrim("[ZFObject] you must not add observer while object is deallocating, class: %s, event: %s",
                this->observerOwner->classData()->classNameFull(),
                ZFEventNameForId(eventId));
            return;
        }

        this->attachMapAttach(eventId);

        _ZFP_ZFObserverData *t = zfpoolNew(_ZFP_ZFObserverData
                , eventId
                , observer
                , observerLevel
                , once
            );

        _ZFP_ZFObserverData *&head = this->observerMap[eventId];
        if(head == zfnull || head->observerLevel > observerLevel) {
            t->pNext = head;
            if(head != zfnull) {
                head->pPrev = t;
            }
            head = t;
            if(this->observerOwner != zfnull) {
                this->observerOwner->observerOnAdd(eventId);
            }
        }
        else {
            _ZFP_ZFObserverData *p = head;
            while(p->pNext != zfnull && p->pNext->observerLevel <= observerLevel) {
                p = p->pNext;
            }
            if(p->pNext != zfnull) {
                p->pNext->pPrev = t;
            }
            t->pNext = p->pNext;
            t->pPrev = p;
            p->pNext = t;
        }
    }
    void observerNotifyPrepare(
            ZF_IN_OUT zfstldeque<_ZFP_ZFObserverData *> &toNotify
            , ZF_IN_OUT _ZFP_ZFObserverData *&toDelete
            , ZF_IN zfidentity eventId
            , ZF_IN ZFObject *observerOwner
            ) {
        zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end()) {
            _ZFP_ZFObserverData *p = it->second;
            do {
                toNotify.push_back(p);
                if(p->once) {
                    _ZFP_ZFObserverData *pNext = p->pNext;
                    this->observerDetach(it, p);

                    if(toDelete == zfnull) {
                        toDelete = p;
                        toDelete->pPrev = toDelete->pNext = zfnull;
                    }
                    else {
                        p->pPrev = zfnull;
                        p->pNext = toDelete;
                        toDelete->pPrev = p;
                        toDelete = p;
                    }
                    p = pNext;
                }
                else {
                    p = p->pNext;
                }
            } while(p != zfnull);

            if(it->second == zfnull) {
                this->observerMap.erase(it);
                this->attachMapDetach(eventId);
                if(observerOwner) {
                    observerOwner->observerOnRemove(eventId);
                }
            }
        }
    }
    void observerDetach(
            ZF_IN_OUT zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator &it
            , ZF_IN_OUT _ZFP_ZFObserverData *p
            ) {
        if(it->second == p) {
            it->second = p->pNext;
        }

        if(p->pPrev != zfnull) {
            p->pPrev->pNext = p->pNext;
        }
        if(p->pNext != zfnull) {
            p->pNext->pPrev = p->pPrev;
        }
    }
};

// ============================================================
// ZFObserver
ZFObserver::ZFObserver(void)
: d(zfpoolNew(_ZFP_ZFObserverPrivate))
{
}
ZFObserver::ZFObserver(ZF_IN ZFObserver const &ref) {
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
}
ZFObserver::~ZFObserver(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
}
ZFObserver &ZFObserver::operator = (ZF_IN ZFObserver const &ref) {
    _ZFP_ZFObserverPrivate *dTmp = d;
    if(ref.d) {
        ++(ref.d->refCount);
    }
    d = ref.d;
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFObserver::operator == (ZF_IN ZFObserver const &ref) const {
    return (d == ref.d);
}

void ZFObserver::observerAdd(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    d->observerAdd(eventId, observer, observerLevel, zffalse);
}
void ZFObserver::observerAddForOnce(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    d->observerAdd(eventId, observer, observerLevel, zftrue);
}
void ZFObserver::observerRemove(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &callback
        ) {
    ZFCoreMutexLocker();

    zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end()) {
        _ZFP_ZFObserverData *p = it->second;
        do {
            if(p->observer.objectCompareByInstance(callback) == ZFCompareEqual) {
                d->observerDetach(it, p);

                if(it->second == zfnull) {
                    d->observerMap.erase(it);
                    d->attachMapDetach(eventId);
                    if(d->observerOwner) {
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
void ZFObserver::observerRemoveAll(ZF_IN zfidentity eventId) {
    ZFCoreMutexLocker();
    zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it == d->observerMap.end()) {
        return;
    }
    zfstldeque<_ZFP_ZFObserverData *> toDelete;
    while(it->second != zfnull) {
        toDelete.push_back(it->second);
        d->observerDetach(it, it->second);
    }
    d->observerMap.erase(it);

    d->attachMapDetach(eventId);
    if(d->observerOwner) {
        d->observerOwner->observerOnRemove(eventId);
    }

    for(zfstlsize i = toDelete.size() - 1; i != (zfstlsize)-1; --i) {
        zfpoolDelete(toDelete[i]);
    }
}
void ZFObserver::observerRemoveAll(void) {
    if(d->observerMap.empty()) {
        return;
    }

    ZFCoreMutexLocker();
    zfstlhashmap<zfidentity, _ZFP_ZFObserverData *> tmp;
    tmp.swap(d->observerMap);
    d->observerMap.clear();

    if(d->observerOwner) {
        for(zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
                it != tmp.end();
                ++it
                ) {
            d->attachMapDetach(it->first);
            d->observerOwner->observerOnRemove(it->first);
        }
    }
    else {
        for(zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
                it != tmp.end();
                ++it
                ) {
            d->attachMapDetach(it->first);
        }
    }

    for(zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
            it != tmp.end();
            ++it
            ) {
        _ZFP_ZFObserverData *p = it->second;
        do {
            _ZFP_ZFObserverData *t = p;
            p = p->pNext;
            zfpoolDelete(t);
        } while(p != zfnull);
    }
}
zfbool ZFObserver::observerHasAdd(void) const {
    return !d->observerMap.empty();
}
zfbool ZFObserver::observerHasAdd(ZF_IN zfidentity eventId) const {
    if(d->observerOwner != zfnull) {
        return (d->observerMap.find(eventId) != d->observerMap.end())
            || ZFGlobalObserver().observerHasAdd(eventId);
    }
    else {
        return (d->observerMap.find(eventId) != d->observerMap.end());
    }
}

void ZFObserver::observerNotifyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    if(eventId == zfidentityInvalid()) {
        return;
    }

    ZFCoreMutexLock();
    zfstldeque<_ZFP_ZFObserverData *> toNotify;
    _ZFP_ZFObserverData *toDelete = zfnull;

    d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    ZFArgs zfargs;
    zfargs
        .eventId(eventId)
        .sender(customSender)
        .param0(param0)
        .param1(param1)
        ;
    if(d->observerOwner != zfnull) {
        d->observerOwner->observerOnEvent(zfargs);
        ZFGlobalObserver().d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    }
    ZFCoreMutexUnlock();

    if(!toNotify.empty()) {
        for(zfstlsize i = 0; i < toNotify.size() && !zfargs.eventFiltered(); ++i) {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(zfargs);
        }
    }

    if(toDelete != zfnull) {
        ZFCoreMutexLock();
        do {
            _ZFP_ZFObserverData *t = toDelete;
            toDelete = toDelete->pNext;
            zfpoolDelete(t);
        } while(toDelete != zfnull);
        ZFCoreMutexUnlock();
    }
}
void ZFObserver::observerNotifyReverselyWithSender(
        ZF_IN ZFObject *customSender
        , ZF_IN zfidentity eventId
        , ZF_IN_OPT ZFObject *param0 /* = zfnull */
        , ZF_IN_OPT ZFObject *param1 /* = zfnull */
        ) const {
    if(eventId == zfidentityInvalid()) {
        return;
    }

    ZFCoreMutexLock();
    zfstldeque<_ZFP_ZFObserverData *> toNotify;
    _ZFP_ZFObserverData *toDelete = zfnull;

    d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    ZFArgs zfargs;
    zfargs
        .eventId(eventId)
        .sender(customSender)
        .param0(param0)
        .param1(param1)
        ;
    if(d->observerOwner != zfnull) {
        d->observerOwner->observerOnEvent(zfargs);
        ZFGlobalObserver().d->observerNotifyPrepare(toNotify, toDelete, eventId, d->observerOwner);
    }
    ZFCoreMutexUnlock();

    if(!toNotify.empty()) {
        for(zfstlsize i = toNotify.size() - 1; i != (zfstlsize)-1 && !zfargs.eventFiltered(); --i) {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(zfargs);
        }
    }

    if(toDelete != zfnull) {
        ZFCoreMutexLock();
        do {
            _ZFP_ZFObserverData *t = toDelete;
            toDelete = toDelete->pNext;
            zfpoolDelete(t);
        } while(toDelete != zfnull);
        ZFCoreMutexUnlock();
    }
}

void ZFObserver::observerHasAddStateAttach(
        ZF_IN zfidentity eventId
        , ZF_IN_OUT zfuint *flag
        , ZF_IN_OUT zfuint flagBit
        ) {
    _ZFP_ZFObserverAttachState state;
    state.flag = flag;
    state.flagBit = flagBit;
    d->attachMap[eventId].push_back(state);
}
void ZFObserver::observerHasAddStateDetach(
        ZF_IN zfidentity eventId
        , ZF_IN_OUT zfuint *flag
        , ZF_IN_OUT zfuint flagBit
        ) {
    _ZFP_ZFObserverAttachStateMapType::iterator it = d->attachMap.find(eventId);
    if(it != d->attachMap.end()) {
        for(zfstlsize i = 0; i < it->second.size(); ++i) {
            _ZFP_ZFObserverAttachState &state = it->second[i];
            if(state.flag == flag && state.flagBit == flagBit) {
                it->second.erase(it->second.begin() + i);
                break;
            }
        }
    }
}

void ZFObserver::objectInfoT(ZF_OUT zfstring &ret) const {
    ZFCoreMutexLocker();
    ret += "<ZFObserver";

    if(d->observerOwner != zfnull) {
        ret += " ";
        d->observerOwner->objectInfoT(ret);
    }

    if(d->observerMap.empty()) {
        ret += ">";
    }
    else {
        for(zfstlhashmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.begin();
                it != d->observerMap.end();
                ++it
                ) {
            ret += "\n  ";
            ret += ZFEventNameForId(it->first);
            ret += ":";

            _ZFP_ZFObserverData *p = it->second;
            do {
                ret += "\n    ";
                ret += p->observer.objectInfo();
                p = p->pNext;
            } while(p != zfnull);
        }

        ret += "\n  >";
    }
}

zfany ZFObserver::observerOwner(void) const {
    return d->observerOwner;
}
void ZFObserver::_ZFP_ZFObserver_observerOwner(ZF_IN ZFObject *obj) {
    /*
     * if observerOwner is not null,
     * means it belongs to a ZFObject, which would:
     * * also send all events to ZFGlobalObserver
     * * notify events to owner ZFObject, such as observerOnEvent
     */
    d->observerOwner = obj;
}

// ============================================================
ZFObserver &ZFGlobalObserver(void) {
    static ZFObserver d;
    return d;
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFObserver &, ZFGlobalObserver)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFEventNameForId
        , ZFMP_IN(zfidentity, idValue)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFEventIdForName
        , ZFMP_IN(const zfstring &, idName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFEventDynamicRegister
        , ZFMP_IN(const zfstring &, idName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFEventDynamicUnregister
        , ZFMP_IN(zfidentity, idValue)
        )

ZF_NAMESPACE_GLOBAL_END

