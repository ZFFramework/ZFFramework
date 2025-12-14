#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFObserver types
zfclassLikePOD _ZFP_ZFObserverData {
public:
    zfuint refCount;
    zfbool once;
    zfidentity eventId;
    ZFListener observer;
    ZFLevel observerLevel;
public:
    explicit _ZFP_ZFObserverData(
            ZF_IN zfidentity eventId
            , ZF_IN const ZFListener &observer
            , ZF_IN ZFLevel observerLevel
            , ZF_IN zfbool once
            )
    : refCount(1)
    , once(once)
    , eventId(eventId)
    , observer(observer)
    , observerLevel(observerLevel)
    {
    }
private:
    _ZFP_ZFObserverData(void);
    _ZFP_ZFObserverData(ZF_IN const _ZFP_ZFObserverData &ref);
    _ZFP_ZFObserverData &operator = (ZF_IN const _ZFP_ZFObserverData &ref);
};
typedef zfstlhashmap<zfidentity, zfstlvector<_ZFP_ZFObserverData *> > _ZFP_ZFObserverMapType;

zfclassPOD _ZFP_ZFObserverAttachState {
public:
    zfuint *flag;
    zft_zfuint flagBit;
};
typedef zfstlhashmap<zfidentity, zfstlvector<_ZFP_ZFObserverAttachState> > _ZFP_ZFObserverAttachStateMapType;

zfclassNotPOD _ZFP_ZFObserverPrivate {
public:
    zfuint refCount;
    ZFObject *observerOwner;
    _ZFP_ZFObserverMapType observerMap; // <eventId, dataList>
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

        zfstlvector<_ZFP_ZFObserverData *> &dataList = this->observerMap[eventId];
        if(dataList.empty()) {
            dataList.push_back(t);
            if(this->observerOwner != zfnull) {
                this->observerOwner->observerOnAdd(eventId);
            }
        }
        else {
            zfstlsize pos = 0;
            while(pos < dataList.size() && dataList[pos]->observerLevel <= observerLevel) {
                ++pos;
            }
            dataList.insert(dataList.begin() + pos, t);
        }
    }
    void observerNotifyPrepare(
            ZF_IN_OUT zfstlvector<_ZFP_ZFObserverData *> &toNotify
            , ZF_IN zfidentity eventId
            , ZF_IN ZFObject *observerOwner
            ) {
        _ZFP_ZFObserverMapType::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end()) {
            zfstlvector<_ZFP_ZFObserverData *> &dataList = it->second;
            toNotify.reserve(toNotify.size() + dataList.size());
            for(zfstlsize i = 0; i < dataList.size(); ) {
                _ZFP_ZFObserverData *t = dataList[i];
                toNotify.push_back(t);
                if(t->once) {
                    dataList.erase(dataList.begin() + i);
                }
                else {
                    ++(t->refCount);
                    ++i;
                }
            }
            if(it->second.empty()) {
                this->observerMap.erase(it);
                this->attachMapDetach(eventId);
                if(observerOwner) {
                    observerOwner->observerOnRemove(eventId);
                }
            }
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

    _ZFP_ZFObserverMapType::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end()) {
        zfstlvector<_ZFP_ZFObserverData *> &dataList = it->second;
        for(zfstlsize i = 0; i < dataList.size(); ++i) {
            _ZFP_ZFObserverData *t = dataList[i];
            if(t->observer.objectCompareByInstance(callback) == ZFCompareEqual) {
                dataList.erase(dataList.begin() + i);
                if(it->second.empty()) {
                    d->observerMap.erase(it);
                    d->attachMapDetach(eventId);
                    if(d->observerOwner) {
                        d->observerOwner->observerOnRemove(eventId);
                    }
                }
                if((--(t->refCount)) == 0) {
                    zfpoolDelete(t);
                }
                break;
            }
        }
    }
}
void ZFObserver::observerRemoveAll(ZF_IN zfidentity eventId) {
    ZFCoreMutexLocker();
    _ZFP_ZFObserverMapType::iterator it = d->observerMap.find(eventId);
    if(it == d->observerMap.end()) {
        return;
    }
    zfstlvector<_ZFP_ZFObserverData *> toDelete;
    toDelete.swap(it->second);
    d->observerMap.erase(it);

    d->attachMapDetach(eventId);
    if(d->observerOwner) {
        d->observerOwner->observerOnRemove(eventId);
    }

    for(zfstlsize i = 0; i < toDelete.size(); ++i) {
        _ZFP_ZFObserverData *t = toDelete[i];
        if((--(t->refCount)) == 0) {
            zfpoolDelete(t);
        }
    }
}
void ZFObserver::observerRemoveAll(void) {
    if(d->observerMap.empty()) {
        return;
    }

    ZFCoreMutexLocker();
    _ZFP_ZFObserverMapType tmp;
    tmp.swap(d->observerMap);

    if(d->observerOwner) {
        for(_ZFP_ZFObserverMapType::iterator it = tmp.begin();
                it != tmp.end();
                ++it
                ) {
            d->attachMapDetach(it->first);
            d->observerOwner->observerOnRemove(it->first);
        }
    }
    else {
        for(_ZFP_ZFObserverMapType::iterator it = tmp.begin();
                it != tmp.end();
                ++it
                ) {
            d->attachMapDetach(it->first);
        }
    }

    for(_ZFP_ZFObserverMapType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
        zfstlvector<_ZFP_ZFObserverData *> &dataList = it->second;
        for(zfstlsize i = 0; i < dataList.size(); ++i) {
            _ZFP_ZFObserverData *t = dataList[i];
            if((--(t->refCount)) == 0) {
                zfpoolDelete(t);
            }
        }
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
    zfstlvector<_ZFP_ZFObserverData *> toNotify;

    d->observerNotifyPrepare(toNotify, eventId, d->observerOwner);
    ZFArgs zfargs;
    zfargs
        .eventId(eventId)
        .sender(customSender)
        .param0(param0)
        .param1(param1)
        ;
    if(d->observerOwner != zfnull) {
        d->observerOwner->observerOnEvent(zfargs);
        ZFGlobalObserver().d->observerNotifyPrepare(toNotify, eventId, d->observerOwner);
    }
    ZFCoreMutexUnlock();

    if(!toNotify.empty()) {
        for(zfstlsize i = 0; i < toNotify.size() && !zfargs.eventFiltered(); ++i) {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(zfargs);
        }

        ZFCoreMutexLocker();
        for(zfstlsize i = 0; i < toNotify.size(); ++i) {
            _ZFP_ZFObserverData *t = toNotify[i];
            if((--(t->refCount)) == 0) {
                zfpoolDelete(t);
            }
        }
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
    zfstlvector<_ZFP_ZFObserverData *> toNotify;

    d->observerNotifyPrepare(toNotify, eventId, d->observerOwner);
    ZFArgs zfargs;
    zfargs
        .eventId(eventId)
        .sender(customSender)
        .param0(param0)
        .param1(param1)
        ;
    if(d->observerOwner != zfnull) {
        d->observerOwner->observerOnEvent(zfargs);
        ZFGlobalObserver().d->observerNotifyPrepare(toNotify, eventId, d->observerOwner);
    }
    ZFCoreMutexUnlock();

    if(!toNotify.empty()) {
        for(zfstlsize i = toNotify.size() - 1; i != (zfstlsize)-1 && !zfargs.eventFiltered(); --i) {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(zfargs);
        }

        ZFCoreMutexLocker();
        for(zfstlsize i = 0; i < toNotify.size(); ++i) {
            _ZFP_ZFObserverData *t = toNotify[i];
            if((--(t->refCount)) == 0) {
                zfpoolDelete(t);
            }
        }
    }
}

void ZFObserver::on(
        ZF_IN const zfstring &eventName
        , ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) {
    const ZFClass *cls = this->observerOwner() ? this->observerOwner()->classData() : zfnull;
    zfidentity eventId = ZFEventIdForEventName(eventName, cls);
    ZFCoreAssertWithMessage(eventId != zfidentityInvalid()
            , "no such event \"%s\" for class: %s"
            , eventName
            , cls
            );
    this->observerAdd(eventId, observer, observerLevel);
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
        ZFCoreMutexLocker();
        for(_ZFP_ZFObserverMapType::iterator it = d->observerMap.begin(); it != d->observerMap.end(); ++it) {
            ret += "\n  ";
            ret += ZFEventNameForId(it->first);
            ret += ":";

            const zfstlvector<_ZFP_ZFObserverData *> &dataList = it->second;
            for(zfstlsize i = 0; i < dataList.size(); ++i) {
                ret += "\n    ";
                ret += dataList[i]->observer.objectInfo();
            }
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
zfidentity ZFEventIdForEventName(
        ZF_IN const zfstring &eventName
        , ZF_IN const ZFClass *cls
        ) {
    zfidentity eventId = ZFIdMapIdForName(eventName);
    if(eventId != zfidentityInvalid()) {
        return eventId;
    }
    if(cls != zfnull) {
        eventId = ZFIdMapIdForName(zfstr("%s.E_%s", cls->classNameFull(), eventName));
        if(eventId != zfidentityInvalid()) {
            return eventId;
        }
        ZFCoreArray<const ZFClass *> allParent = cls->parentGetAll();
        for(zfindex i = 0; i < allParent.count(); ++i) {
            eventId = ZFIdMapIdForName(zfstr("%s.E_%s", allParent[i]->classNameFull(), eventName));
            if(eventId != zfidentityInvalid()) {
                return eventId;
            }
        }
    }
    return ZFIdMapIdForName(zfstr("%s.E_%s", ZF_NAMESPACE_NAME(ZFGlobalEvent), eventName));
}

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

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, ZFEventIdForEventName
        , ZFMP_IN(const zfstring &, eventName)
        , ZFMP_IN(const ZFClass *, cls)
        )

ZF_NAMESPACE_GLOBAL_END

