#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFArgsPrivate {
public:
    zfuint refCount;
    zfbool success;
    zfbool ignoreError;
    zfbool ignoreErrorEvent;
    zfbool eventFiltered;
    zfidentity eventId;
    zfstring errorHint;
    const ZFMethod *ownerMethod;
    const ZFProperty *ownerProperty;
    zfany sender;
    zfauto result;
    zfauto param[ZFMETHOD_MAX_PARAM];
public:
    _ZFP_ZFArgsPrivate(void)
    : refCount(1)
    , success(zftrue)
    , ignoreError(zffalse)
    , ignoreErrorEvent(zffalse)
    , eventFiltered(zffalse)
    , eventId(zfidentityInvalid())
    , errorHint()
    , ownerMethod(zfnull)
    , ownerProperty(zfnull)
    , sender()
    , result()
    , param()
    {
    }
};

// ============================================================
zfany const &ZFArgs::sender(void) const {
    return d->sender;
}
ZFArgs &ZFArgs::sender(ZF_IN zfany const &v) {
    d->sender = v;
    return *this;
}

zfauto &ZFArgs::result(void) const {
    return d->result;
}
ZFArgs const &ZFArgs::result(ZF_IN zfauto const &v) const {
    d->result = v;
    return *this;
}
ZFArgs &ZFArgs::result(ZF_IN zfauto const &v) {
    d->result = v;
    return *this;
}

zfauto &ZFArgs::param0(void) const {
    return d->param[0];
}
ZFArgs const &ZFArgs::param0(ZF_IN zfauto const &v) const {
    d->param[0] = v;
    return *this;
}
ZFArgs &ZFArgs::param0(ZF_IN zfauto const &v) {
    d->param[0] = v;
    return *this;
}
zfauto &ZFArgs::param1(void) const {
    return d->param[1];
}
ZFArgs const &ZFArgs::param1(ZF_IN zfauto const &v) const {
    d->param[1] = v;
    return *this;
}
ZFArgs &ZFArgs::param1(ZF_IN zfauto const &v) {
    d->param[1] = v;
    return *this;
}
zfauto &ZFArgs::param2(void) const {
    return d->param[2];
}
ZFArgs const &ZFArgs::param2(ZF_IN zfauto const &v) const {
    d->param[2] = v;
    return *this;
}
ZFArgs &ZFArgs::param2(ZF_IN zfauto const &v) {
    d->param[2] = v;
    return *this;
}
zfauto &ZFArgs::param3(void) const {
    return d->param[3];
}
ZFArgs const &ZFArgs::param3(ZF_IN zfauto const &v) const {
    d->param[3] = v;
    return *this;
}
ZFArgs &ZFArgs::param3(ZF_IN zfauto const &v) {
    d->param[3] = v;
    return *this;
}
zfauto &ZFArgs::param4(void) const {
    return d->param[4];
}
ZFArgs const &ZFArgs::param4(ZF_IN zfauto const &v) const {
    d->param[4] = v;
    return *this;
}
ZFArgs &ZFArgs::param4(ZF_IN zfauto const &v) {
    d->param[4] = v;
    return *this;
}
zfauto &ZFArgs::param5(void) const {
    return d->param[5];
}
ZFArgs const &ZFArgs::param5(ZF_IN zfauto const &v) const {
    d->param[5] = v;
    return *this;
}
ZFArgs &ZFArgs::param5(ZF_IN zfauto const &v) {
    d->param[5] = v;
    return *this;
}
zfauto &ZFArgs::param6(void) const {
    return d->param[6];
}
ZFArgs const &ZFArgs::param6(ZF_IN zfauto const &v) const {
    d->param[6] = v;
    return *this;
}
ZFArgs &ZFArgs::param6(ZF_IN zfauto const &v) {
    d->param[6] = v;
    return *this;
}
zfauto &ZFArgs::param7(void) const {
    return d->param[7];
}
ZFArgs const &ZFArgs::param7(ZF_IN zfauto const &v) const {
    d->param[7] = v;
    return *this;
}
ZFArgs &ZFArgs::param7(ZF_IN zfauto const &v) {
    d->param[7] = v;
    return *this;
}

// ============================================================
zfbool ZFArgs::success(void) const {
    return d->success;
}
ZFArgs const &ZFArgs::success(ZF_IN zfbool v) const {
    d->success = v;
    return *this;
}
ZFArgs &ZFArgs::success(ZF_IN zfbool v) {
    d->success = v;
    return *this;
}

zfstring const &ZFArgs::errorHint(void) const {
    return d->errorHint;
}
ZFArgs const &ZFArgs::errorHint(ZF_IN zfstring const &v) const {
    d->errorHint = v;
    return *this;
}
ZFArgs &ZFArgs::errorHint(ZF_IN zfstring const &v) {
    d->errorHint = v;
    return *this;
}

zfbool ZFArgs::ignoreError(void) const {
    return d->ignoreError;
}
ZFArgs &ZFArgs::ignoreError(ZF_IN zfbool v) {
    d->ignoreError = v;
    return *this;
}

zfbool ZFArgs::ignoreErrorEvent(void) const {
    return d->ignoreErrorEvent;
}
ZFArgs &ZFArgs::ignoreErrorEvent(ZF_IN zfbool v) {
    d->ignoreErrorEvent = v;
    return *this;
}

// ============================================================
const ZFMethod *ZFArgs::ownerMethod(void) const {
    return d->ownerMethod;
}
ZFArgs &ZFArgs::ownerMethod(ZF_IN const ZFMethod *v) {
    d->ownerMethod = v;
    return *this;
}

const ZFProperty *ZFArgs::ownerProperty(void) const {
    return d->ownerProperty;
}
ZFArgs &ZFArgs::ownerProperty(ZF_IN const ZFProperty *v) {
    d->ownerProperty = v;
    return *this;
}

// ============================================================
zfidentity const &ZFArgs::eventId(void) const {
    return d->eventId;
}
ZFArgs &ZFArgs::eventId(ZF_IN zfidentity const &v) {
    d->eventId = v;
    return *this;
}

zfbool ZFArgs::eventFiltered(void) const {
    return d->eventFiltered;
}
ZFArgs const &ZFArgs::eventFiltered(ZF_IN zfbool eventFiltered) const {
    d->eventFiltered = eventFiltered;
    return *this;
}
ZFArgs &ZFArgs::eventFiltered(ZF_IN zfbool eventFiltered) {
    d->eventFiltered = eventFiltered;
    return *this;
}

// ============================================================
ZFArgs &ZFArgs::paramInit(void) {
    ZFCoreMutexLock();
    _ZFP_ZFArgsPrivate &dTmp = *d;
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
        dTmp.param[i].zfunsafe_assign(ZFMP_DEF());
    }
    ZFCoreMutexUnlock();
    return *this;
}
ZFArgs &ZFArgs::paramInit(
        ZF_IN ZFObject *param0
        , ZF_IN ZFObject *param1
        , ZF_IN ZFObject *param2
        , ZF_IN ZFObject *param3
        , ZF_IN ZFObject *param4
        , ZF_IN ZFObject *param5
        , ZF_IN ZFObject *param6
        , ZF_IN ZFObject *param7
        ) {
    ZFCoreMutexLock();
    _ZFP_ZFArgsPrivate &dTmp = *d;
    dTmp.param[0].zfunsafe_assign(param0);
    dTmp.param[1].zfunsafe_assign(param1);
    dTmp.param[2].zfunsafe_assign(param2);
    dTmp.param[3].zfunsafe_assign(param3);
    dTmp.param[4].zfunsafe_assign(param4);
    dTmp.param[5].zfunsafe_assign(param5);
    dTmp.param[6].zfunsafe_assign(param6);
    dTmp.param[7].zfunsafe_assign(param7);
    ZFCoreMutexUnlock();
    return *this;
}
ZFArgs &ZFArgs::paramInit(ZF_IN const ZFCoreArray<zfauto> &params) {
    ZFCoreAssert(params.count() <= ZFMETHOD_MAX_PARAM);
    ZFCoreMutexLock();
    _ZFP_ZFArgsPrivate &dTmp = *d;
    for(zfindex i = 0; i < params.count(); ++i) {
        dTmp.param[i].zfunsafe_assign(params[i]);
    }
    for(zfindex i = params.count(); i < ZFMETHOD_MAX_PARAM; ++i) {
        dTmp.param[i].zfunsafe_assign(ZFMP_DEF());
    }
    ZFCoreMutexUnlock();
    return *this;
}

zfauto &ZFArgs::paramAt(ZF_IN zfindex index) const {
    return d->param[index];
}
ZFArgs const &ZFArgs::param(ZF_IN zfindex index, ZF_IN zfauto const &v) const {
    d->param[index] = v;
    return *this;
}
ZFArgs &ZFArgs::param(ZF_IN zfindex index, ZF_IN zfauto const &v) {
    d->param[index] = v;
    return *this;
}

// ============================================================
ZFArgs::ZFArgs(void) {
    d = zfpoolNew(_ZFP_ZFArgsPrivate);
}
ZFArgs::ZFArgs(ZF_IN const ZFArgs &ref) {
    d = ref.d;
    ++(d->refCount);
}

ZFArgs::~ZFArgs(void) {
    if(d->refCount == 1) {
        zfpoolDelete(d);
    }
    else {
        --(d->refCount);
    }
}
ZFArgs &ZFArgs::operator = (ZF_IN const ZFArgs &ref) {
    if(d != ref.d) {
        _ZFP_ZFArgsPrivate *dTmp = d;
        d = ref.d;
        ++(d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFArgs::operator == (ZF_IN const ZFArgs &ref) const {
    return d == ref.d;
}

void ZFArgs::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += "ZFArgs";
    zfstring eventName = ZFEventNameForId(this->eventId());
    if(eventName != zfnull) {
        ret += " event:";
        ret += eventName;
    }
    if(this->eventFiltered()) {
        ret += " filtered:true";
    }
    if(this->sender() != zfnull) {
        ret += " sender:";
        ZFObjectInfoOfInstanceT(ret, this->sender());
    }
    if(this->ownerMethod()) {
        ret += " method:";
        ret += this->ownerMethod()->methodName();
    }
    if(this->ownerProperty()) {
        ret += " property:";
        ret += this->ownerProperty()->propertyName();
    }
    if(this->result() != zfnull) {
        ret += " result:";
        ZFObjectInfoT(ret, this->result());
    }
    if(!this->success()) {
        ret += " success:false";
    }
    if(this->ignoreError()) {
        ret += " ignoreError:true";
    }
    else if(this->ignoreErrorEvent()) {
        ret += " ignoreErrorEvent:true";
    }
    if(this->errorHint()) {
        ret += " errorHint:";
        ret += this->errorHint();
    }
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
        zfany p = this->paramAt(i);
        if(p && p != ZFMP_DEF()) {
            ret += " param";
            zfindexToStringT(ret, i);
            ret += ":";
            this->paramAt(i)->objectInfoT(ret);
        }
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
zfclass _ZFP_I_ZFArgsCallSuperCache : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFArgsCallSuperCache, ZFObject)
public:
    const ZFMethod *ownerMethod;
    const ZFMethod *superMethod;
    ZFListener classDataUpdateListener;
public:
    static zfself *attach(ZF_IN const ZFArgs &zfargs) {
        zfstring cacheKey = zfstr("%s:%s", zfself::ClassData()->classNameFull(), zfargs.ownerMethod()->methodId());
        zfself *cache = zfargs.ownerMethod()->ownerClass()->classTag(cacheKey);
        if(cache == zfnull) {
            zfobj<zfself> holder;
            cache = holder;
            cache->ownerMethod = zfargs.ownerMethod();
            cache->_update(zfargs);
            ZFLISTENER_1(classDataOnUpdate
                    , zfweakT<zfself>, cache
                    ) {
                cache->detach();
            } ZFLISTENER_END()
            cache->classDataUpdateListener = classDataOnUpdate;
            ZFClassDataUpdateObserver().observerAdd(ZFGlobalEvent::E_ClassDataUpdate(), cache->classDataUpdateListener);
            zfargs.ownerMethod()->ownerClass()->classTag(cacheKey, cache);
        }
        return cache;
    }
    void detach(void) {
        if(this->ownerMethod == zfnull) {
            return;
        }
        const ZFClass *cls = this->ownerMethod->ownerClass();
        zfstring cacheKey = zfstr("%s:%s", zfself::ClassData()->classNameFull(), this->ownerMethod->methodId());
        this->ownerMethod = zfnull;
        this->superMethod = zfnull;
        ZFClassDataUpdateObserver().observerRemove(ZFGlobalEvent::E_ClassDataUpdate(), this->classDataUpdateListener);
        cls->classTagRemove(cacheKey);
    }
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->ownerMethod = zfnull;
        this->superMethod = zfnull;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->detach();
        zfsuper::objectOnDeallocPrepare();
    }
private:
    void _update(ZF_IN const ZFArgs &zfargs) {
        ZFCoreQueuePOD<const ZFClass *> toCheck;
        const ZFClass *cls = zfargs.ownerMethod()->ownerClass();
        ZFCoreArray<const ZFMethod *> buf;
        do {
            if(cls->classParent() != zfnull) {
                toCheck.add(cls->classParent());
            }
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
                toCheck.add(cls->implementedInterfaceAt(i));
            }
            for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
                toCheck.add(cls->dynamicInterfaceAt(i));
            }
            if(toCheck.isEmpty()) {
                break;
            }

            cls = toCheck.take();
            const ZFMethod *chain = zfargs.ownerMethod();
            do {
                cls->methodForNameGetAllIgnoreParentT(buf, chain->methodName());
                for(zfindex i = 0; i < buf.count(); ++i) {
                    const ZFMethod *m = buf[i];
                    if(m->paramTypeIdIsMatch(zfargs.ownerMethod())) {
                        this->superMethod = m;
                        return;
                    }
                }
                buf.removeAll();
                chain = chain->aliasFrom();
            } while(chain != zfnull);
        } while(zftrue);
    }
};
zfauto ZFArgs::callSuper(void) const {
    ZFCoreAssertWithMessage(this->ownerMethod() && this->ownerMethod()->isDynamicRegister(),
        "callSuper() only works for dynamic registered method");
    ZFCoreAssertWithMessage(this->ownerMethod()->ownerClass() != zfnull && this->ownerMethod()->methodType() == ZFMethodTypeVirtual,
        "callSuper() only works for class virtual member method");

    _ZFP_I_ZFArgsCallSuperCache *cache = _ZFP_I_ZFArgsCallSuperCache::attach(*this);
    if(cache->superMethod) {
        return cache->superMethod->methodInvoke(this->sender()
                , this->param0()
                , this->param1()
                , this->param2()
                , this->param3()
                , this->param4()
                , this->param5()
                , this->param6()
                , this->param7()
                );
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

