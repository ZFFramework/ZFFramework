#include "ZFCallback.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(void) {
    ZFCoreCriticalMessageTrim("try execute a null callback");
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL(ZFLIB_ZFCore, CallbackClearPrepare)
ZFEVENT_GLOBAL_REGISTER(CallbackClearPrepare)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlhashmap<zfstring, zfauto> _ZFP_ZFCallbackTagMap;

zfclassNotPOD _ZFP_ZFCallbackPrivateExt {
public:
    zfstring callbackId;
    _ZFP_ZFCallbackTagMap callbackTagMap;

    zfstring serializableCustomType;
    ZFSerializableData callbackSerializeData;
    ZFPathInfo pathInfo;

public:
    _ZFP_ZFCallbackPrivateExt(void)
    : callbackId()
    , callbackTagMap()
    , serializableCustomType()
    , callbackSerializeData()
    , pathInfo()
    {
    }
};

zfclassNotPOD _ZFP_ZFCallbackPrivate {
public:
    zfuint refCount;

    struct D {
        ZFCallbackType callbackType;
        union {
            // for ZFCallbackTypeMethod
            const ZFMethod *callbackMethod;

            // for ZFCallbackTypeMemberMethod
            zfclassPOD {
            public:
                ZFObject *callbackOwnerObject; // assign only
                zfuint callbackOwnerObjectRetainFlag;
                const ZFMethod *callbackMethod;
            } memberType;

            // for ZFCallbackTypeRawFunction
            ZFFuncAddrType callbackRawFunction;

            // for ZFCallbackTypeLambda
            zfclassPOD {
            public:
                void *callbackLambdaImpl;
                _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy;
                ZFFuncAddrType callbackLambdaInvoker;
            } lambdaType;
        } d;
        _ZFP_ZFCallbackPrivateExt *ext;
    };
    D d;

public:
    void callbackClear(void) {
        if(d.ext) {
            _ZFP_ZFCallbackTagMap::iterator it = d.ext->callbackTagMap.find("_ZFP_ZFCallbackClearPrepare");
            if(it != d.ext->callbackTagMap.end()) {
                zfautoT<v_ZFObserver> holder = it->second;

                zfobj<v_ZFCallback> sender;
                sender->zfv._ZFP_ZFCallback_d(this);
                ++(this->refCount);

                holder->zfv.observerNotifyWithSender(sender, ZFGlobalEvent::E_CallbackClearPrepare());
                d.ext->callbackTagMap.erase(it);
            }
        }

        D dTmp;
        zfmemcpy(&dTmp, &d, sizeof(d));
        d.callbackType = ZFCallbackTypeDummy;
        d.ext = zfnull;

        if(dTmp.ext) {
            zfpoolDelete(dTmp.ext);
        }
        switch(dTmp.callbackType) {
            case ZFCallbackTypeMemberMethod:
                if(dTmp.d.memberType.callbackOwnerObjectRetainFlag != 0) {
                    zfRelease(dTmp.d.memberType.callbackOwnerObject);
                }
                break;
            case ZFCallbackTypeLambda:
                if(dTmp.d.lambdaType.callbackLambdaImplDestroy) {
                    dTmp.d.lambdaType.callbackLambdaImplDestroy(dTmp.d.lambdaType.callbackLambdaImpl);
                }
                break;
            default:
                break;
        }
    }
    void callbackClearPrepareAdd(
            ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            ) {
        if(d.ext == zfnull) {
            d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
        }
        _ZFP_ZFCallbackTagMap::iterator it = d.ext->callbackTagMap.find("_ZFP_ZFCallbackClearPrepare");
        if(it != d.ext->callbackTagMap.end()) {
            v_ZFObserver *holder = it->second;
            holder->zfv.observerAdd(ZFGlobalEvent::E_CallbackClearPrepare(), observer, observerLevel);
        }
        else {
            zfobj<v_ZFObserver> holder;
            d.ext->callbackTagMap["_ZFP_ZFCallbackClearPrepare"] = holder;
            holder->zfv.observerAdd(ZFGlobalEvent::E_CallbackClearPrepare(), observer, observerLevel);
        }
    }
    void callbackClearPrepareRemove(ZF_IN const ZFListener &observer) {
        if(d.ext) {
            _ZFP_ZFCallbackTagMap::iterator it = d.ext->callbackTagMap.find("_ZFP_ZFCallbackClearPrepare");
            if(it != d.ext->callbackTagMap.end()) {
                v_ZFObserver *holder = it->second;
                holder->zfv.observerRemove(ZFGlobalEvent::E_CallbackClearPrepare(), observer);
                if(!holder->zfv.observerHasAdd()) {
                    d.ext->callbackTagMap.erase(it);
                }
            }
        }
    }

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , d()
    {
    }
};

// ============================================================
// global
static void _ZFP_ZFCallbackPrivateDataChange(_ZFP_ZFCallbackPrivate *&oldData, _ZFP_ZFCallbackPrivate *newData) {
    ZFCoreMutexLock();
    _ZFP_ZFCallbackPrivate *dTmp = oldData;
    oldData = newData;
    if(newData) {
        ++newData->refCount;
    }
    if(dTmp) {
        if(dTmp->refCount == 1) {
            dTmp->callbackClear();
            --(dTmp->refCount);
            if(dTmp->refCount == 0) {
                zfpoolDelete(dTmp);
            }
        }
        else {
            --(dTmp->refCount);
        }
    }
    ZFCoreMutexUnlock();
}

// ============================================================
// ZFCallback
ZFCallback::ZFCallback(void)
: d(zfnull)
{
}
ZFCallback::ZFCallback(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFCallback::ZFCallback(ZF_IN const ZFCallback &ref)
: d(zfnull)
{
    this->operator = (ref);
}
ZFCallback &ZFCallback::operator = (ZF_IN const ZFCallback &ref) {
    _ZFP_ZFCallbackPrivateDataChange(d, ref.d);
    return *this;
}
ZFCallback &ZFCallback::operator = (ZF_IN const zfnullT &dummy) {
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
    return *this;
}
ZFCallback::~ZFCallback(void) {
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateMethod(ZF_IN const ZFMethod *callbackMethod) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    ZFCoreAssertWithMessageTrim(callbackMethod, "[ZFCallback] method is null");
    ZFCoreAssertWithMessageTrim(callbackMethod->methodType() == ZFMethodTypeStatic,
        "[ZFCallback] method \"%s\" is not class static member type",
        callbackMethod);
    callback.d->d.callbackType = ZFCallbackTypeMethod;
    callback.d->d.d.callbackMethod = callbackMethod;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateMemberMethod(
        ZF_IN ZFObject *callbackOwnerObject
        , ZF_IN const ZFMethod *callbackMethod
        ) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    ZFCoreAssertWithMessageTrim(callbackOwnerObject && callbackMethod,
        "[ZFCallback] invalid callback, ownerObj: %s, method: %s",
        callbackOwnerObject,
        callbackMethod);
    ZFCoreAssertWithMessageTrim(callbackMethod->methodType() != ZFMethodTypeStatic,
        "[ZFCallback] method \"%s\" is not class member type",
        callbackMethod);
    ZFCoreAssertWithMessageTrim(callbackOwnerObject->classData()->classIsTypeOf(callbackMethod->ownerClass()),
        "[ZFCallback] object %s has no such method \"%s\"",
        callbackOwnerObject->objectInfoOfInstance(),
        callbackMethod);
    callback.d->d.callbackType = ZFCallbackTypeMemberMethod;
    callback.d->d.d.memberType.callbackOwnerObject = callbackOwnerObject;
    callback.d->d.d.memberType.callbackMethod = callbackMethod;
    callback.d->d.d.memberType.callbackOwnerObjectRetainFlag = 0;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateRawFunction(ZF_IN ZFFuncAddrType callbackRawFunction) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    ZFCoreAssertWithMessageTrim(callbackRawFunction, "[ZFCallback] invalid function address");
    callback.d->d.callbackType = ZFCallbackTypeRawFunction;
    callback.d->d.d.callbackRawFunction = callbackRawFunction;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateLambda(
        ZF_IN void *callbackLambdaImpl
        , ZF_IN _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy
        , ZF_IN ZFFuncAddrType callbackLambdaInvoker
        ) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    callback.d->d.callbackType = ZFCallbackTypeLambda;
    callback.d->d.d.lambdaType.callbackLambdaImpl = callbackLambdaImpl;
    callback.d->d.d.lambdaType.callbackLambdaImplDestroy = callbackLambdaImplDestroy;
    callback.d->d.d.lambdaType.callbackLambdaInvoker = callbackLambdaInvoker;
    return callback;
}

zfindex ZFCallback::objectRetainCount(void) const {
    return (d ? d->refCount : 1);
}

void ZFCallback::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    if(this->callbackId()) {
        ret += this->callbackId();
    }
    else {
        switch(this->callbackType()) {
            case ZFCallbackTypeDummy:
                ret += "ZFCallbackNull";
                break;
            case ZFCallbackTypeMethod:
            case ZFCallbackTypeMemberMethod:
                this->callbackMethod()->objectInfoT(ret);
                break;
            case ZFCallbackTypeRawFunction:
                ret += ZFTOKEN_ZFObjectInfoLeft;
                ret += "ZFCallback func: ";
                zfsFromPointerT(ret, reinterpret_cast<const void *>(this->callbackRawFunction()));
                ret += ZFTOKEN_ZFObjectInfoRight;
                break;
            case ZFCallbackTypeLambda:
                ret += ZFTOKEN_ZFObjectInfoLeft;
                ret += "ZFCallback lambda: ";
                zfsFromPointerT(ret, reinterpret_cast<const void *>(this->callbackLambdaInvoker()));
                ret += ZFTOKEN_ZFObjectInfoRight;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
    }
}

ZFCompareResult ZFCallback::objectCompare(ZF_IN const ZFCallback &ref) const {
    if(d == ref.d
            || (zftrue
                && (d == zfnull || d->d.callbackType == ZFCallbackTypeDummy)
                && (ref.d == zfnull || ref.d->d.callbackType == ZFCallbackTypeDummy)
                )
                ) {
        return ZFCompareEqual;
    }
    else if(d && ref.d && d->d.callbackType == ref.d->d.callbackType) {
        _ZFP_ZFCallbackPrivate &d0 = *d;
        _ZFP_ZFCallbackPrivate &d1 = *(ref.d);
        switch(d0.d.callbackType) {
            case ZFCallbackTypeMethod:
                return zftrue
                    && d0.d.d.callbackMethod == d1.d.d.callbackMethod
                    ? ZFCompareEqual : ZFCompareUncomparable;
            case ZFCallbackTypeMemberMethod:
                return zftrue
                    && d0.d.d.memberType.callbackOwnerObject == d1.d.d.memberType.callbackOwnerObject
                    && d0.d.d.memberType.callbackMethod == d1.d.d.memberType.callbackMethod
                    ? ZFCompareEqual : ZFCompareUncomparable;
            case ZFCallbackTypeRawFunction:
                return zftrue
                    && d0.d.d.callbackRawFunction == d1.d.d.callbackRawFunction
                    ? ZFCompareEqual : ZFCompareUncomparable;
            case ZFCallbackTypeLambda:
            case ZFCallbackTypeDummy:
            default:
                return ZFCompareUncomparable;
        }
    }
    else {
        return ZFCompareUncomparable;
    }
}

void ZFCallback::callbackClear(void) {
    ZFCoreMutexLocker();
    if(d->refCount == 1) {
        _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
    }
    else {
        _ZFP_ZFCallbackPrivate *dTmp = d;
        d = zfnull;
        --(dTmp->refCount);
        dTmp->callbackClear();
    }
}
void ZFCallback::callbackClearPrepareAdd(
        ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        ) const {
    ZFCoreMutexLocker();
    if(d) {
        d->callbackClearPrepareAdd(observer, observerLevel);
    }
}
void ZFCallback::callbackClearPrepareRemove(ZF_IN const ZFListener &observer) const {
    ZFCoreMutexLocker();
    if(d) {
        d->callbackClearPrepareRemove(observer);
    }
}

void ZFCallback::callbackId(ZF_IN const zfstring &callbackId) {
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    }
    if(d->d.ext == zfnull) {
        d->d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }
    d->d.ext->callbackId = callbackId;
}
const zfstring &ZFCallback::callbackId(void) const {
    return (d && d->d.ext ? d->d.ext->callbackId : zfstring::Empty());
}

zfidentity ZFCallback::callbackHash(void) const {
    return zfidentityCalc(d);
}

void ZFCallback::callbackTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) {
    if(key == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    }
    if(d->d.ext == zfnull) {
        d->d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }

    _ZFP_ZFCallbackTagMap &m = d->d.ext->callbackTagMap;
    _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag) {
            m[key] = tag;
        }
    }
    else {
        if(tag == zfnull) {
            zfauto holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else {
            zfauto holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
zfany ZFCallback::callbackTag(ZF_IN const zfstring &key) const {
    if(d && d->d.ext && key) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->d.ext->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end()) {
            return it->second;
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagGetAllKeyValue(
        ZF_IN_OUT ZFCoreArray<zfstring> &allKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
        ) const {
    if(d && d->d.ext) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->d.ext->callbackTagMap;
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it) {
            allKey.add(it->first);
            allValue.add(it->second);
        }
    }
}
zfauto ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfstring &key) {
    if(d && d->d.ext && key) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->d.ext->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end()) {
            zfauto ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagRemoveAll(void) {
    if(d && d->d.ext && !d->d.ext->callbackTagMap.empty()) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->d.ext->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const {
    return (d ? d->d.callbackType : ZFCallbackTypeDummy);
}

zfany ZFCallback::callbackOwnerObject(void) const {
    return (d && d->d.callbackType == ZFCallbackTypeMemberMethod ? d->d.d.memberType.callbackOwnerObject : zfnull);
}

const ZFMethod *ZFCallback::callbackMethod(void) const {
    switch(d ? d->d.callbackType : ZFCallbackTypeDummy) {
        case ZFCallbackTypeMethod:
            return d->d.d.callbackMethod;
        case ZFCallbackTypeMemberMethod:
            return d->d.d.memberType.callbackMethod;
        default:
            return zfnull;
    }
}

ZFFuncAddrType ZFCallback::callbackRawFunction(void) const {
    return (d && d->d.callbackType == ZFCallbackTypeRawFunction ? d->d.d.callbackRawFunction : zfnull);
}

void *ZFCallback::callbackLambdaImpl(void) const {
    return d && d->d.callbackType == ZFCallbackTypeLambda ? d->d.d.lambdaType.callbackLambdaImpl : zfnull;
}
ZFFuncAddrType ZFCallback::callbackLambdaInvoker(void) const {
    return d && d->d.callbackType == ZFCallbackTypeLambda ? d->d.d.lambdaType.callbackLambdaInvoker : zfnull;
}
void ZFCallback::_ZFP_ZFCallback_callbackLambdaInvoker(ZF_IN ZFFuncAddrType v) const {
    d->d.d.lambdaType.callbackLambdaInvoker = v;
}

void ZFCallback::callbackOwnerObjectRetain(void) const {
    ZFCoreMutexLocker();
    if(d && d->d.callbackType == ZFCallbackTypeMemberMethod && d->d.d.memberType.callbackOwnerObject) {
        ++(d->d.d.memberType.callbackOwnerObjectRetainFlag);
        if(d->d.d.memberType.callbackOwnerObjectRetainFlag == 1) {
            zfunsafe_zfRetain(d->d.d.memberType.callbackOwnerObject);
        }
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const {
    ZFCoreMutexLocker();
    if(d && d->d.callbackType == ZFCallbackTypeMemberMethod && d->d.d.memberType.callbackOwnerObjectRetainFlag > 0) {
        --(d->d.d.memberType.callbackOwnerObjectRetainFlag);
        if(d->d.d.memberType.callbackOwnerObjectRetainFlag == 0) {
            zfRelease(d->d.d.memberType.callbackOwnerObject);
        }
    }
}

void ZFCallback::callbackSerializeType(ZF_IN const zfstring &customType) {
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    }
    if(d->d.ext == zfnull) {
        d->d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }
    d->d.ext->serializableCustomType = customType;
}
const zfstring &ZFCallback::callbackSerializeType(void) const {
    return d && d->d.ext ? d->d.ext->serializableCustomType : zfstring::Empty();
}
void ZFCallback::callbackSerializeData(ZF_IN const ZFSerializableData &customData) {
    if(customData == zfnull) {
        if(d && d->d.ext) {
            d->d.ext->callbackSerializeData = zfnull;
        }
    }
    else {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFCallbackPrivate);
        }
        if(d->d.ext == zfnull) {
            d->d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
        }
        d->d.ext->callbackSerializeData = customData;
    }
}
ZFSerializableData ZFCallback::callbackSerializeData(void) const {
    return (d && d->d.ext ? d->d.ext->callbackSerializeData : ZFSerializableData());
}

ZFPathInfo ZFCallback::pathInfo(void) const {
    return (d && d->d.ext ? d->d.ext->pathInfo : ZFPathInfo());
}
void ZFCallback::pathInfo(ZF_IN const ZFPathInfo &pathInfo) {
    ZFCoreMutexLocker();
    if(pathInfo) {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFCallbackPrivate);
        }
        if(d->d.ext == zfnull) {
            d->d.ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
        }
        d->d.ext->pathInfo = pathInfo;
    }
    else {
        if(d && d->d.ext) {
            d->d.ext->pathInfo = zfnull;
        }
    }
}

// ============================================================
// ZFCallbackWeakRef
ZFOBJECT_REGISTER(ZFCallbackWeakRef)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCallbackWeakRef, ZFCallback, get)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCallbackWeakRef, void, set
        , ZFMP_IN(const ZFCallback &, v)
        )
ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject.to<ZFCallbackWeakRef *>()->set(v);
    }, ZFCallbackWeakRef
    , ZFMP_IN(const ZFCallback &, v)
    )

ZFCallback ZFCallbackWeakRef::get(void) {
    ZFCoreMutexLocker();
    if(d) {
        ZFCallback ret;
        ret._ZFP_ZFCallback_d(d);
        ++(d->refCount);
        return ret;
    }
    else {
        return zfnull;
    }
}
void ZFCallbackWeakRef::set(ZF_IN const ZFCallback &v) {
    ZFCoreMutexLocker();
    if(v) {
        if(d != v._ZFP_ZFCallback_d()) {
            if(d) {
                d->callbackClearPrepareRemove(this->c);
            }

            if(!this->c) {
                ZFLISTENER_1(onDealloc
                        , _ZFP_ZFCallbackPrivate *&, d
                        ) {
                    d = zfnull;
                } ZFLISTENER_END()
                this->c = onDealloc;
            }
            d = v._ZFP_ZFCallback_d();
            d->callbackClearPrepareAdd(this->c);
        }
    }
    else {
        if(d) {
            d->callbackClearPrepareRemove(this->c);
            d = zfnull;
        }
    }
}
void ZFCallbackWeakRef::set(ZF_IN const zfnullT &v) {
    if(d) {
        ZFCoreMutexLocker();
        d->callbackClearPrepareRemove(this->c);
        d = zfnull;
    }
}

void ZFCallbackWeakRef::objectOnDeallocPrepare(void) {
    this->set(zfnull);
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

