#include "ZFCallback.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(void) {
    zfCoreCriticalMessageTrim("try execute a null callback");
}

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlmap<zfstring, zfauto> _ZFP_ZFCallbackTagMap;
zfclassNotPOD _ZFP_ZFCallbackPrivate {
public:
    zfuint refCount;
    zfchar *callbackId;
    _ZFP_ZFCallbackTagMap callbackTagMap;

    ZFCallbackType callbackType;
    ZFObject *callbackOwnerObject; // assign only
    zfuint callbackOwnerObjectRetainFlag;
    const ZFMethod *callbackMethod;
    ZFFuncAddrType callbackRawFunction;
    void *callbackLambdaImpl;
    _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy;
    ZFFuncAddrType callbackLambdaInvoker;

    zfchar *serializableCustomType;
    ZFSerializableData *serializableCustomData;
    ZFPathInfo *pathInfo;

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , callbackId(zfnull)
    , callbackTagMap()
    , callbackType(ZFCallbackTypeDummy)
    , callbackOwnerObject(zfnull)
    , callbackOwnerObjectRetainFlag(0)
    , callbackMethod(zfnull)
    , callbackRawFunction(zfnull)
    , callbackLambdaImpl(zfnull)
    , callbackLambdaImplDestroy(zfnull)
    , callbackLambdaInvoker(zfnull)
    , serializableCustomType(zfnull)
    , serializableCustomData(zfnull)
    , pathInfo(zfnull)
    {
    }
    ~_ZFP_ZFCallbackPrivate(void) {
        zffree(this->callbackId);
        if(this->callbackOwnerObjectRetainFlag != 0) {
            zfRelease(this->callbackOwnerObject);
        }
        zffree(this->serializableCustomType);
        zfdelete(this->serializableCustomData);
        zfdelete(this->pathInfo);
        if(this->callbackLambdaImplDestroy) {
            this->callbackLambdaImplDestroy(this->callbackLambdaImpl);
        }
    }
};

// ============================================================
// global
static void _ZFP_ZFCallbackPrivateDataChange(_ZFP_ZFCallbackPrivate *&oldData, _ZFP_ZFCallbackPrivate *newData) {
    zfCoreMutexLock();
    _ZFP_ZFCallbackPrivate *dTmp = oldData;
    oldData = newData;
    if(newData != zfnull) {
        ++newData->refCount;
    }
    if(dTmp != zfnull) {
        --dTmp->refCount;
        if(dTmp->refCount == 0) {
            zfdelete(dTmp);
        }
    }
    zfCoreMutexUnlock();
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
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    zfCoreAssertWithMessageTrim(callbackMethod != zfnull, "[ZFCallback] method is null");
    zfCoreAssertWithMessageTrim(callbackMethod->methodType() == ZFMethodTypeStatic,
        "[ZFCallback] method \"%s\" is not class static member type",
        callbackMethod);
    callback.d->callbackType = ZFCallbackTypeMethod;
    callback.d->callbackMethod = callbackMethod;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateMemberMethod(
        ZF_IN ZFObject *callbackOwnerObject
        , ZF_IN const ZFMethod *callbackMethod
        ) {
    ZFCallback callback;
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    zfCoreAssertWithMessageTrim(callbackOwnerObject != zfnull && callbackMethod != zfnull,
        "[ZFCallback] invalid callback, ownerObj: %s, method: %s",
        callbackOwnerObject,
        callbackMethod);
    zfCoreAssertWithMessageTrim(callbackMethod->methodType() != ZFMethodTypeStatic,
        "[ZFCallback] method \"%s\" is not class member type",
        callbackMethod);
    zfCoreAssertWithMessageTrim(callbackOwnerObject->classData()->classIsTypeOf(callbackMethod->methodOwnerClass()),
        "[ZFCallback] object %s has no such method \"%s\"",
        callbackOwnerObject->objectInfoOfInstance(),
        callbackMethod);
    callback.d->callbackType = ZFCallbackTypeMemberMethod;
    callback.d->callbackOwnerObject = callbackOwnerObject;
    callback.d->callbackMethod = callbackMethod;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateRawFunction(ZF_IN ZFFuncAddrType callbackRawFunction) {
    ZFCallback callback;
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    zfCoreAssertWithMessageTrim(callbackRawFunction != zfnull, "[ZFCallback] invalid function address");
    callback.d->callbackType = ZFCallbackTypeRawFunction;
    callback.d->callbackRawFunction = callbackRawFunction;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateLambda(
        ZF_IN void *callbackLambdaImpl
        , ZF_IN _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy
        , ZF_IN ZFFuncAddrType callbackLambdaInvoker
        ) {
    ZFCallback callback;
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    callback.d->callbackType = ZFCallbackTypeLambda;
    callback.d->callbackLambdaImpl = callbackLambdaImpl;
    callback.d->callbackLambdaImplDestroy = callbackLambdaImplDestroy;
    callback.d->callbackLambdaInvoker = callbackLambdaInvoker;
    return callback;
}

zfindex ZFCallback::objectRetainCount(void) const {
    return (d ? d->refCount : 1);
}

void ZFCallback::objectInfoT(ZF_IN_OUT zfstring &ret) const {
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
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

ZFCompareResult ZFCallback::objectCompare(ZF_IN const ZFCallback &ref) const {
    return ((d == ref.d || (
            this->callbackType() == ref.callbackType()
            && this->callbackOwnerObject() == ref.callbackOwnerObject()
            && this->callbackMethod() == ref.callbackMethod()
            && this->callbackRawFunction() == ref.callbackRawFunction()
            ))
        ? ZFCompareEqual : ZFCompareUncomparable);
}

void ZFCallback::callbackClear(void) {
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}

void ZFCallback::callbackId(ZF_IN const zfchar *callbackId) {
    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(callbackId != zfnull && *callbackId == '\0') {
        callbackId = zfnull;
    }
    zfsChange(d->callbackId, callbackId);
}
const zfchar *ZFCallback::callbackId(void) const {
    return (d ? d->callbackId : zfnull);
}

void ZFCallback::callbackTag(
        ZF_IN const zfchar *key
        , ZF_IN ZFObject *tag
        ) {
    if(key == zfnull) {
        return;
    }
    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }

    _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
    _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag != zfnull) {
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
zfany ZFCallback::callbackTag(ZF_IN const zfchar *key) const {
    if(d != zfnull && key != zfnull) {
        zfCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
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
    if(d != zfnull) {
        zfCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it) {
            allKey.add(it->first);
            allValue.add(it->second);
        }
    }
}
zfauto ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfchar *key) {
    if(d != zfnull && key != zfnull) {
        zfCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
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
    if(d != zfnull && !d->callbackTagMap.empty()) {
        zfCoreMutexLocker();
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const {
    return (d ? d->callbackType : ZFCallbackTypeDummy);
}

zfany ZFCallback::callbackOwnerObject(void) const {
    return (d ? d->callbackOwnerObject : zfnull);
}

const ZFMethod *ZFCallback::callbackMethod(void) const {
    return (d ? d->callbackMethod : zfnull);
}

ZFFuncAddrType ZFCallback::callbackRawFunction(void) const {
    return (d ? d->callbackRawFunction : zfnull);
}

void *ZFCallback::callbackLambdaImpl(void) const {
    return d->callbackLambdaImpl;
}
ZFFuncAddrType ZFCallback::callbackLambdaInvoker(void) const {
    return d->callbackLambdaInvoker;
}
void ZFCallback::_ZFP_ZFCallback_callbackLambdaInvoker(ZF_IN ZFFuncAddrType v) const {
    d->callbackLambdaInvoker = v;
}

void ZFCallback::callbackOwnerObjectRetain(void) const {
    if(d != zfnull && d->callbackOwnerObject != zfnull && !d->callbackOwnerObjectRetainFlag) {
        zfRetain(d->callbackOwnerObject);
        ++(d->callbackOwnerObjectRetainFlag);
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const {
    if(d != zfnull && d->callbackOwnerObjectRetainFlag > 0) {
        --(d->callbackOwnerObjectRetainFlag);
        if(d->callbackOwnerObjectRetainFlag == 0) {
            zfRetainChange(d->callbackOwnerObject, zfnull);
        }
    }
}

void ZFCallback::callbackSerializeCustomType(ZF_IN const zfchar *customType) {
    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    zfsChange(d->serializableCustomType, customType);
}
const zfchar *ZFCallback::callbackSerializeCustomType(void) const {
    return (d ? d->serializableCustomType : zfnull);
}
void ZFCallback::callbackSerializeCustomData(ZF_IN const ZFSerializableData *customData) {
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(customData == zfnull) {
        if(d->serializableCustomData != zfnull) {
            zfdelete(d->serializableCustomData);
            d->serializableCustomData = zfnull;
        }
    }
    else {
        if(d->serializableCustomData != zfnull) {
            *(d->serializableCustomData) = *customData;
        }
        else {
            d->serializableCustomData = zfnew(ZFSerializableData, *customData);
        }
    }
}
const ZFSerializableData *ZFCallback::callbackSerializeCustomData(void) const {
    return (d ? d->serializableCustomData : zfnull);
}

const ZFPathInfo *ZFCallback::pathInfo(void) const {
    return (d ? d->pathInfo : zfnull);
}
void ZFCallback::pathInfo(ZF_IN const ZFPathInfo *pathInfo) {
    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(pathInfo != zfnull && !pathInfo->isEmpty()) {
        if(d->pathInfo == zfnull) {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else {
        if(d->pathInfo != zfnull) {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFCallback::pathInfo(
        ZF_IN const zfchar *pathType
        , ZF_IN const zfchar *pathData
        ) {
    zfCoreMutexLocker();
    if(d == zfnull) {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(!zfstringIsEmpty(pathType) || !zfstringIsEmpty(pathData)) {
        if(d->pathInfo == zfnull) {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else {
        if(d->pathInfo != zfnull) {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_0({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute();
    }, v_ZFCallback,
    void, execute)
ZFMETHOD_USER_REGISTER_1({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(zfargs);
    }, v_ZFCallback,
    void, execute
    , ZFMP_IN(const ZFArgs &, zfargs)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackClear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackId
        , ZFMP_IN(const zfchar *, callbackId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTag
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfany, callbackTag
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackTagRemove
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfauto, callbackTagRemoveAndGet
        , ZFMP_IN(const zfchar *, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFCallbackType, callbackType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfany, callbackOwnerObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFMethod *, callbackMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFFuncAddrType, callbackRawFunction)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRetain)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRelease)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackInfoCopy
        , ZFMP_IN(const ZFCallback &, src)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomType
        , ZFMP_IN(const zfchar *, customType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackSerializeCustomType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomData
        , ZFMP_IN(const ZFSerializableData &, customData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFSerializableData *, callbackSerializeCustomData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomDisable
        , ZFMP_IN(zfbool, disable)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackSerializeCustomDisabled)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, pathInfo
        , ZFMP_IN(const ZFPathInfo *, pathInfo)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, pathInfo
        , ZFMP_IN(const zfchar *, pathType)
        , ZFMP_IN(const zfchar *, pathData)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForMethod
        , ZFMP_IN(const ZFMethod *, zfmethod)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCallback, ZFCallbackForMemberMethod
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN(const ZFMethod *, zfmethod)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForFunc
        , ZFMP_IN(ZFFuncAddrType, callbackRawFunction)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

