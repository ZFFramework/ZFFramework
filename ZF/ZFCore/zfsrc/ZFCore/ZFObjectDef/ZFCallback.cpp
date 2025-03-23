#include "ZFCallback.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(void) {
    ZFCoreCriticalMessageTrim("try execute a null callback");
}

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlmap<zfstring, zfauto> _ZFP_ZFCallbackTagMap;

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

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , callbackType(ZFCallbackTypeDummy)
    , d()
    , ext(zfnull)
    {
    }
    ~_ZFP_ZFCallbackPrivate(void) {
        if(callbackType == ZFCallbackTypeMemberMethod && d.memberType.callbackOwnerObjectRetainFlag != 0) {
            zfRelease(d.memberType.callbackOwnerObject);
        }
        if(ext) {
            zfpoolDelete(ext);
        }
        if(callbackType == ZFCallbackTypeLambda && d.lambdaType.callbackLambdaImplDestroy) {
            d.lambdaType.callbackLambdaImplDestroy(d.lambdaType.callbackLambdaImpl);
        }
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
        --dTmp->refCount;
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
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
    callback.d->callbackType = ZFCallbackTypeMethod;
    callback.d->d.callbackMethod = callbackMethod;
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
    callback.d->callbackType = ZFCallbackTypeMemberMethod;
    callback.d->d.memberType.callbackOwnerObject = callbackOwnerObject;
    callback.d->d.memberType.callbackMethod = callbackMethod;
    callback.d->d.memberType.callbackOwnerObjectRetainFlag = 0;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateRawFunction(ZF_IN ZFFuncAddrType callbackRawFunction) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    ZFCoreAssertWithMessageTrim(callbackRawFunction, "[ZFCallback] invalid function address");
    callback.d->callbackType = ZFCallbackTypeRawFunction;
    callback.d->d.callbackRawFunction = callbackRawFunction;
    return callback;
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreateLambda(
        ZF_IN void *callbackLambdaImpl
        , ZF_IN _ZFP_ZFCallbackLambdaDeleteCallback callbackLambdaImplDestroy
        , ZF_IN ZFFuncAddrType callbackLambdaInvoker
        ) {
    ZFCallback callback;
    callback.d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    callback.d->callbackType = ZFCallbackTypeLambda;
    callback.d->d.lambdaType.callbackLambdaImpl = callbackLambdaImpl;
    callback.d->d.lambdaType.callbackLambdaImplDestroy = callbackLambdaImplDestroy;
    callback.d->d.lambdaType.callbackLambdaInvoker = callbackLambdaInvoker;
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
                && (d == zfnull || d->callbackType == ZFCallbackTypeDummy)
                && (ref.d == zfnull || ref.d->callbackType == ZFCallbackTypeDummy)
                )
                ) {
        return ZFCompareEqual;
    }
    else if(d && ref.d && d->callbackType == ref.d->callbackType) {
        _ZFP_ZFCallbackPrivate &d0 = *d;
        _ZFP_ZFCallbackPrivate &d1 = *(ref.d);
        switch(d0.callbackType) {
            case ZFCallbackTypeMethod:
                return zftrue
                    && d0.d.callbackMethod == d1.d.callbackMethod
                    ? ZFCompareEqual : ZFCompareUncomparable;
            case ZFCallbackTypeMemberMethod:
                return zftrue
                    && d0.d.memberType.callbackOwnerObject == d1.d.memberType.callbackOwnerObject
                    && d0.d.memberType.callbackMethod == d1.d.memberType.callbackMethod
                    ? ZFCompareEqual : ZFCompareUncomparable;
            case ZFCallbackTypeRawFunction:
                return zftrue
                    && d0.d.callbackRawFunction == d1.d.callbackRawFunction
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
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}

void ZFCallback::callbackId(ZF_IN const zfstring &callbackId) {
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    }
    if(d->ext == zfnull) {
        d->ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }
    d->ext->callbackId = callbackId;
}
const zfstring &ZFCallback::callbackId(void) const {
    return (d && d->ext ? d->ext->callbackId : zfstring::Empty());
}

zfidentity ZFCallback::callbackHash(void) const {
    return zfidentityCalcPointer(d);
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
    if(d->ext == zfnull) {
        d->ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }

    _ZFP_ZFCallbackTagMap &m = d->ext->callbackTagMap;
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
    if(d && d->ext && key) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->ext->callbackTagMap;
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
    if(d && d->ext) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->ext->callbackTagMap;
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it) {
            allKey.add(it->first);
            allValue.add(it->second);
        }
    }
}
zfauto ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfstring &key) {
    if(d && d->ext && key) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap &m = d->ext->callbackTagMap;
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
    if(d && d->ext && !d->ext->callbackTagMap.empty()) {
        ZFCoreMutexLocker();
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->ext->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const {
    return (d ? d->callbackType : ZFCallbackTypeDummy);
}

zfany ZFCallback::callbackOwnerObject(void) const {
    return (d && d->callbackType == ZFCallbackTypeMemberMethod ? d->d.memberType.callbackOwnerObject : zfnull);
}

const ZFMethod *ZFCallback::callbackMethod(void) const {
    switch(d ? d->callbackType : ZFCallbackTypeDummy) {
        case ZFCallbackTypeMethod:
            return d->d.callbackMethod;
        case ZFCallbackTypeMemberMethod:
            return d->d.memberType.callbackMethod;
        default:
            return zfnull;
    }
}

ZFFuncAddrType ZFCallback::callbackRawFunction(void) const {
    return (d && d->callbackType == ZFCallbackTypeRawFunction ? d->d.callbackRawFunction : zfnull);
}

void *ZFCallback::callbackLambdaImpl(void) const {
    return d && d->callbackType == ZFCallbackTypeLambda ? d->d.lambdaType.callbackLambdaImpl : zfnull;
}
ZFFuncAddrType ZFCallback::callbackLambdaInvoker(void) const {
    return d && d->callbackType == ZFCallbackTypeLambda ? d->d.lambdaType.callbackLambdaInvoker : zfnull;
}
void ZFCallback::_ZFP_ZFCallback_callbackLambdaInvoker(ZF_IN ZFFuncAddrType v) const {
    d->d.lambdaType.callbackLambdaInvoker = v;
}

void ZFCallback::callbackOwnerObjectRetain(void) const {
    ZFCoreMutexLocker();
    if(d && d->callbackType == ZFCallbackTypeMemberMethod && d->d.memberType.callbackOwnerObject) {
        ++(d->d.memberType.callbackOwnerObjectRetainFlag);
        if(d->d.memberType.callbackOwnerObjectRetainFlag == 1) {
            zfunsafe_zfRetain(d->d.memberType.callbackOwnerObject);
        }
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const {
    ZFCoreMutexLocker();
    if(d && d->callbackType == ZFCallbackTypeMemberMethod && d->d.memberType.callbackOwnerObjectRetainFlag > 0) {
        --(d->d.memberType.callbackOwnerObjectRetainFlag);
        if(d->d.memberType.callbackOwnerObjectRetainFlag == 0) {
            zfRelease(d->d.memberType.callbackOwnerObject);
        }
    }
}

void ZFCallback::callbackSerializeType(ZF_IN const zfstring &customType) {
    ZFCoreMutexLocker();
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCallbackPrivate);
    }
    if(d->ext == zfnull) {
        d->ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
    }
    d->ext->serializableCustomType = customType;
}
const zfstring &ZFCallback::callbackSerializeType(void) const {
    return d && d->ext ? d->ext->serializableCustomType : zfstring::Empty();
}
void ZFCallback::callbackSerializeData(ZF_IN const ZFSerializableData &customData) {
    if(customData == zfnull) {
        if(d && d->ext) {
            d->ext->callbackSerializeData = zfnull;
        }
    }
    else {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFCallbackPrivate);
        }
        if(d->ext == zfnull) {
            d->ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
        }
        d->ext->callbackSerializeData = customData;
    }
}
ZFSerializableData ZFCallback::callbackSerializeData(void) const {
    return (d && d->ext ? d->ext->callbackSerializeData : ZFSerializableData());
}

ZFPathInfo ZFCallback::pathInfo(void) const {
    return (d && d->ext ? d->ext->pathInfo : ZFPathInfo());
}
void ZFCallback::pathInfo(ZF_IN const ZFPathInfo &pathInfo) {
    ZFCoreMutexLocker();
    if(pathInfo) {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFCallbackPrivate);
        }
        if(d->ext == zfnull) {
            d->ext = zfpoolNew(_ZFP_ZFCallbackPrivateExt);
        }
        d->ext->pathInfo = pathInfo;
    }
    else {
        if(d && d->ext) {
            d->ext->pathInfo = zfnull;
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
    zfbool, execute)
ZFMETHOD_USER_REGISTER_1({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(zfargs);
    }, v_ZFCallback,
    zfbool, execute
    , ZFMP_IN(const ZFArgs &, zfargs)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackClear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackId
        , ZFMP_IN(const zfstring &, callbackId)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfstring, callbackId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfidentity, callbackHash)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, tag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfany, callbackTag
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagGetAllKeyValue
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, allKey)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, allValue)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackTagRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfauto, callbackTagRemoveAndGet
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFCallbackType, callbackType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfany, callbackOwnerObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFMethod *, callbackMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFFuncAddrType, callbackRawFunction)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRetain)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRelease)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackInfoCopy
        , ZFMP_IN(const ZFCallback &, src)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeType
        , ZFMP_IN(const zfstring &, customType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfstring, callbackSerializeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeData
        , ZFMP_IN(const ZFSerializableData &, customData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFSerializableData, callbackSerializeData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeDisable
        , ZFMP_IN(zfbool, disable)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackSerializeDisable)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFPathInfo, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, pathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
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

