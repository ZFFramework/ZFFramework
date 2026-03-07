#include "ZFProperty.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

// #define _ZFP_ZFProperty_DEBUG 1

#if _ZFP_ZFProperty_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFProperty_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFProp] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFProperty_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFProperty::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += '(';
    if(zfslen(this->propertyTypeId()) > 0) {
        ret += this->propertyTypeId();
    }
    else {
        ret += this->propertyTypeName();
    }
    ret += ')';
    ret += this->ownerClass()->classNameFull();
    ret += "::";
    ret += this->propertyName();
}

zfbool ZFProperty::propertySerializable(void) const {
    if(zfstringIsEqual(this->propertyTypeId(), ZFTypeId_none())) {
        return zffalse;
    }
    const ZFTypeInfo *t = ZFTypeInfoForName(this->propertyTypeId());
    if(t == zfnull || !t->typeIdSerializable()) {
        return zffalse;
    }
    return zftrue;
}

ZFProperty::ZFProperty(void)
: callbackIsValueAccessed(zfnull)
, callbackIsInitValue(zfnull)
, callbackValueReset(zfnull)
, callbackUserRegisterInitValueSetup(zfnull)
, callbackDynamicRegisterInitValueGetter(zfnull)
, _refCount(1)
, _propertyId(zfnull)
, _dynamicRegisterUserData(zfnull)
, _dynamicRegisterUserDataWrapper(zfnull)
, _ownerClass(zfnull)
, _propertyName(zfnull)
, _propertyTypeName(zfnull)
, _propertyTypeId(zfnull)
, _setterMethod(zfnull)
, _getterMethod(zfnull)
, _propertyClassOfRetainProperty(zfnull)
, _callbackEnsureInit(zfnull)
, _callbackDealloc(zfnull)
, _ZFP_ZFPropertyLifeCycle_OnInit()
, _ZFP_ZFPropertyLifeCycle_OnUpdate()
, _ZFP_ZFPropertyLifeCycle_OnAttach()
, _ZFP_ZFPropertyLifeCycle_OnDetach()
{
}
ZFProperty::~ZFProperty(void) {
}
void ZFProperty::_ZFP_ZFPropertyInit(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN const zfstring &name
        , ZF_IN const zfstring &typeName
        , ZF_IN const zfstring &typeIdName
        , ZF_IN const ZFMethod *setterMethod
        , ZF_IN const ZFMethod *getterMethod
        , ZF_IN const ZFClass *propertyClassOfRetainProperty
        ) {
    if(getterMethod->isPrivate() || zfstringBeginWith(name, "_ZFP_")) {
        ZFBitSet(_stateFlags, _stateFlags_isInternal);
        ZFBitSet(_stateFlags, _stateFlags_isInternalPrivate);
    }
    else if(!getterMethod->isPublic() || name[0] == '_') {
        ZFBitSet(_stateFlags, _stateFlags_isInternal);
    }
    if(isUserRegister) {
        ZFBitSet(_stateFlags, _stateFlags_isUserRegister);
    }
    if(isDynamicRegister) {
        ZFBitSet(_stateFlags, _stateFlags_isDynamicRegister);
    }
    _dynamicRegisterUserData = zfobjRetain(dynamicRegisterUserData);
    _ownerClass = ownerClass;
    _propertyName = name;
    _propertyTypeName = typeName;
    _propertyTypeId = typeIdName;
    _setterMethod = setterMethod;
    _getterMethod = getterMethod;
    setterMethod->_ZFP_ZFMethod_ownerProperty(this);
    getterMethod->_ZFP_ZFMethod_ownerProperty(this);
    _propertyClassOfRetainProperty = propertyClassOfRetainProperty;
}

void ZFProperty::_ZFP_ZFProperty_dynamicRegisterUserDataWrapper(ZF_IN ZFObject *v) const {
    zfobjRetainChange(_ZFP_ZFProperty_removeConst()->_dynamicRegisterUserDataWrapper, v);
}

// ============================================================
typedef zfstlhashmap<zfstring, ZFProperty *> _ZFP_ZFPropertyMapType;
static _ZFP_ZFPropertyMapType &_ZFP_ZFPropertyMap(void) {
    static _ZFP_ZFPropertyMapType m;
    return m;
}

// ============================================================
void ZFPropertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFPropertyMapType &m = _ZFP_ZFPropertyMap();
    for(_ZFP_ZFPropertyMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ZFProperty *v = it->second;
        ret.add(v);
    }
}

// ============================================================
static void _ZFP_ZFPropertyInstanceSig(
        ZF_OUT zfstring &ret
        , ZF_IN const ZFSigName &classNameFull
        , ZF_IN const ZFSigName &propertyName
        ) {
    if(classNameFull) {
        zfindexToStringT(ret, classNameFull.sigId());
    }
    ret += ':';
    if(propertyName) {
        zfindexToStringT(ret, propertyName.sigId());
    }
}
static ZFProperty *_ZFP_ZFPropertyInstanceFind(ZF_IN const zfstring &propertyId) {
    ZFCoreMutexLocker();
    _ZFP_ZFPropertyMapType &m = _ZFP_ZFPropertyMap();
    _ZFP_ZFPropertyMapType::iterator it = m.find(propertyId);
    if(it != m.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}

ZFProperty *ZFProperty::_ZFP_ZFPropertyRegister(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN const zfstring &name
        , ZF_IN const zfstring &typeName
        , ZF_IN const zfstring &typeIdName
        , ZF_IN const ZFMethod *setterMethod
        , ZF_IN const ZFMethod *getterMethod
        , ZF_IN const ZFClass *propertyClassOfRetainProperty
        , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
        , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
        , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
        , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
        , ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter
        , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
        , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
        ) {
    _ZFP_ZFProperty_invokeTimeLogger("reg: %s::%s", ownerClass->className().cString(), name.cString());
    ZFCoreMutexLocker();
    ZFProperty *propertyInfo = zfnull;

    ZFCoreAssert(ownerClass != zfnull);
    ZFCoreAssert(name != zfnull && *name != '\0');
    ZFCoreAssert(typeName != zfnull && *typeName != '\0');
    ZFCoreAssert(typeIdName != zfnull && *typeIdName != '\0');
    ZFCoreAssert(setterMethod != zfnull);
    ZFCoreAssert(getterMethod != zfnull);
    ZFCoreAssert(callbackIsValueAccessed != zfnull);
    ZFCoreAssert(callbackIsInitValue != zfnull);
    ZFCoreAssert(callbackValueReset != zfnull);

    zfstring propertyId;
    ZFSigName nameHolder = name;
    _ZFP_ZFPropertyInstanceSig(propertyId, ownerClass->classNameFull(), nameHolder);

    if(isUserRegister) {
        propertyInfo = _ZFP_ZFPropertyInstanceFind(propertyId);
        ZFCoreAssertWithMessageTrim(propertyInfo == zfnull,
            "[ZFPropertyUserRegister] registering a property that already registered, class: %s, propertyName: %s",
            ownerClass->classNameFull(),
            name);
    }
    else if(isDynamicRegister) {
        propertyInfo = _ZFP_ZFPropertyInstanceFind(propertyId);
        ZFCoreAssertWithMessageTrim(propertyInfo == zfnull,
            "[ZFPropertyDynamicRegister] registering a property that already registered, class: %s, propertyName: %s",
            ownerClass->classNameFull(),
            name);
    }
    {
        ZFCoreMutexLocker();
        _ZFP_ZFPropertyMapType &m = _ZFP_ZFPropertyMap();
        _ZFP_ZFPropertyMapType::iterator it = m.find(propertyId);
        if(it != m.end()) {
            propertyInfo = it->second;
            ++(propertyInfo->_refCount);
        }
        else {
            propertyInfo = zfnew(ZFProperty);
            propertyInfo->_propertyId = propertyId;
            m[propertyId] = propertyInfo;
        }
    }

    if(propertyInfo->_propertyName == zfnull) {
        propertyInfo->_ZFP_ZFPropertyInit(
                isUserRegister
                , isDynamicRegister
                , dynamicRegisterUserData
                , ownerClass
                , name
                , typeName
                , typeIdName
                , setterMethod
                , getterMethod
                , propertyClassOfRetainProperty
                );
        propertyInfo->callbackIsValueAccessed = callbackIsValueAccessed;
        propertyInfo->callbackIsInitValue = callbackIsInitValue;
        propertyInfo->callbackValueReset = callbackValueReset;
        propertyInfo->callbackUserRegisterInitValueSetup = callbackUserRegisterInitValueSetup;
        propertyInfo->callbackDynamicRegisterInitValueGetter = callbackDynamicRegisterInitValueGetter;
        propertyInfo->_callbackEnsureInit = callbackEnsureInit;
        propertyInfo->_callbackDealloc = callbackDealloc;

        ownerClass->_ZFP_ZFClass_propertyRegister(propertyInfo);
        _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeAttach, zfnull, propertyInfo, zfnull);
    }

    return propertyInfo;
}
void ZFProperty::_ZFP_ZFPropertyUnregister(ZF_IN const ZFProperty *propertyInfo) {
    _ZFP_ZFProperty_invokeTimeLogger("unreg: %s::%s", propertyInfo->ownerClass()->className().cString(), propertyInfo->propertyName().cString());
    ZFCoreMutexLocker();
    _ZFP_ZFPropertyMapType &m = _ZFP_ZFPropertyMap();
    _ZFP_ZFPropertyMapType::iterator it = m.find(propertyInfo->_propertyId);
    if(it == m.end()) {
        return;
    }

    ZFProperty *v = it->second;
    --(v->_refCount);
    if(v->_refCount != 0) {
        return;
    }
    m.erase(it);

    if(v->setterMethod()->aliasFrom() == zfnull) {
        if(v->setterMethod()->isUserRegister()) {
            ZFMethodUserUnregister(v->setterMethod());
        }
        else if(v->setterMethod()->isDynamicRegister()) {
            ZFMethodDynamicUnregister(v->setterMethod());
        }
    }

    if(v->getterMethod()->aliasFrom() == zfnull) {
        if(v->getterMethod()->isUserRegister()) {
            ZFMethodUserUnregister(v->getterMethod());
        }
        else if(v->getterMethod()->isDynamicRegister()) {
            ZFMethodDynamicUnregister(v->getterMethod());
        }
    }

    v->ownerClass()->_ZFP_ZFClass_propertyUnregister(v);
    zfobjRetainChange(v->_dynamicRegisterUserData, zfnull);
    zfobjRetainChange(v->_dynamicRegisterUserDataWrapper, zfnull);

    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeDetach, zfnull, v, zfnull);

    zfdelete(v);
}

ZF_NAMESPACE_GLOBAL_END

