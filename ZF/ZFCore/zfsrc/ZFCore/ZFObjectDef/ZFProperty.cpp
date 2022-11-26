#include "ZFProperty.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFProperty::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    if(zfslen(this->propertyTypeId()) > 0)
    {
        ret += this->propertyTypeId();
    }
    else
    {
        ret += this->propertyTypeName();
    }
    ret += ')';
    ret += this->propertyOwnerClass()->classNameFull();
    ret += "::";
    ret += this->propertyName();
}

zfbool ZFProperty::propertySerializable(void) const
{
    if(zfscmpTheSame(this->propertyTypeId(), ZFTypeId_none()))
    {
        return zffalse;
    }
    const ZFTypeInfo *t = ZFTypeInfoForName(this->propertyTypeId());
    if(t == zfnull || !t->typeIdSerializable())
    {
        return zffalse;
    }
    return zftrue;
}

ZFProperty::ZFProperty(void)
: callbackIsValueAccessed(zfnull)
, callbackIsInitValue(zfnull)
, callbackValueReset(zfnull)
, callbackUserRegisterInitValueSetup(zfnull)
, _ZFP_ZFPropertyNeedInit(zftrue)
, _ZFP_ZFProperty_refCount(1)
, _ZFP_ZFProperty_propertyInternalId()
, _ZFP_ZFProperty_propertyIsUserRegister(zffalse)
, _ZFP_ZFProperty_propertyIsDynamicRegister(zffalse)
, _ZFP_ZFProperty_propertyDynamicRegisterUserData(zfnull)
, _ZFP_ZFProperty_propertyOwnerClass(zfnull)
, _ZFP_ZFProperty_name()
, _ZFP_ZFProperty_typeName()
, _ZFP_ZFProperty_typeId()
, _ZFP_ZFProperty_setterMethod(zfnull)
, _ZFP_ZFProperty_getterMethod(zfnull)
, _ZFP_ZFProperty_setterMethodCleanup(zfnull)
, _ZFP_ZFProperty_getterMethodCleanup(zfnull)
, _ZFP_ZFProperty_propertyClassOfRetainProperty(zfnull)
, _ZFP_ZFProperty_callbackEnsureInit(zfnull)
, _ZFP_ZFProperty_callbackDealloc(zfnull)
, _ZFP_ZFPropertyLifeCycle_OnInit()
, _ZFP_ZFPropertyLifeCycle_OnVerify()
, _ZFP_ZFPropertyLifeCycle_OnAttach()
, _ZFP_ZFPropertyLifeCycle_OnDetach()
{
}
ZFProperty::~ZFProperty(void)
{
}
void ZFProperty::_ZFP_ZFPropertyInit(ZF_IN zfbool propertyIsUserRegister,
                                     ZF_IN zfbool propertyIsDynamicRegister,
                                     ZF_IN ZFObject *propertyDynamicRegisterUserData,
                                     ZF_IN const ZFClass *propertyOwnerClass,
                                     ZF_IN const zfchar *name,
                                     ZF_IN const zfchar *typeName,
                                     ZF_IN const zfchar *typeIdName,
                                     ZF_IN const ZFMethod *setterMethod,
                                     ZF_IN const ZFMethod *getterMethod,
                                     ZF_IN _ZFP_ZFPropertyMethodCleanup setterMethodCleanup,
                                     ZF_IN _ZFP_ZFPropertyMethodCleanup getterMethodCleanup,
                                     ZF_IN const ZFClass *propertyClassOfRetainProperty)
{
    this->_ZFP_ZFProperty_propertyIsUserRegister = propertyIsUserRegister;
    this->_ZFP_ZFProperty_propertyIsDynamicRegister = propertyIsDynamicRegister;
    this->_ZFP_ZFProperty_propertyDynamicRegisterUserData = zfRetain(propertyDynamicRegisterUserData);
    this->_ZFP_ZFProperty_propertyOwnerClass = propertyOwnerClass;
    this->_ZFP_ZFProperty_name = name;
    this->_ZFP_ZFProperty_typeName = typeName;
    if(propertyClassOfRetainProperty == zfnull && ZFClass::classForName(typeIdName) != zfnull)
    { // assign property with ZFObject type, is not serializable
        this->_ZFP_ZFProperty_typeId = ZFTypeId_none();
    }
    else
    {
        this->_ZFP_ZFProperty_typeId = typeIdName;
    }
    this->_ZFP_ZFProperty_setterMethod = setterMethod;
    this->_ZFP_ZFProperty_getterMethod = getterMethod;
    this->_ZFP_ZFProperty_setterMethodCleanup = setterMethodCleanup;
    this->_ZFP_ZFProperty_getterMethodCleanup = getterMethodCleanup;
    setterMethod->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodOwnerProperty = this;
    getterMethod->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodOwnerProperty = this;
    this->_ZFP_ZFProperty_propertyClassOfRetainProperty = propertyClassOfRetainProperty;
}

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFPropertyDataHolder)
{
}
ZFCoreMap propertyMap; // ZFProperty *
ZF_STATIC_INITIALIZER_END(ZFPropertyDataHolder)
#define _ZFP_ZFPropertyMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFPropertyDataHolder)->propertyMap)

// ============================================================
void ZFPropertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret,
                       ZF_IN_OPT const ZFFilterForZFProperty *propertyFilter /* = zfnull */)
{
    zfCoreMutexLocker();
    const ZFCoreMap &m = _ZFP_ZFPropertyMap;
    if(propertyFilter != zfnull)
    {
        for(zfiterator it = m.iter(); m.iterValid(it); m.iterNext(it))
        {
            ZFProperty *v = m.iterValue<ZFProperty *>(it);
            if(propertyFilter->filterCheckActive(v))
            {
                ret.add(v);
            }
        }
    }
    else
    {
        for(zfiterator it = m.iter(); m.iterValid(it); m.iterNext(it))
        {
            ZFProperty *v = m.iterValue<ZFProperty *>(it);
            ret.add(v);
        }
    }
}

// ============================================================
static void _ZFP_ZFPropertyInstanceSig(ZF_OUT zfstring &ret,
                                       ZF_IN const zfchar *classNameFull,
                                       ZF_IN const zfchar *propertyName)
{
    if(classNameFull)
    {
        zfindexToString(ret, _ZFP_ZFSigForName(classNameFull));
    }
    ret += ':';
    if(propertyName)
    {
        zfindexToString(ret, _ZFP_ZFSigForName(propertyName));
    }
}
static ZFProperty *_ZFP_ZFPropertyInstanceFind(ZF_IN const zfchar *propertyInternalId)
{
    zfCoreMutexLocker();
    ZFProperty *v = _ZFP_ZFPropertyMap.get<ZFProperty *>(propertyInternalId);
    if(v == zfnull)
    {
        return zfnull;
    }
    else
    {
        return v;
    }
}
static ZFProperty *_ZFP_ZFPropertyInstanceAccess(ZF_IN const zfchar *propertyInternalId)
{
    zfCoreMutexLocker();
    ZFProperty *v = _ZFP_ZFPropertyMap.get<ZFProperty *>(propertyInternalId);
    if(v == zfnull)
    {
        v = zfnew(ZFProperty);
        _ZFP_ZFPropertyMap.set(propertyInternalId, ZFCorePointerForObject<ZFProperty *>(v));
        v->_ZFP_ZFProperty_propertyInternalId = propertyInternalId;
    }
    else
    {
        ++(v->_ZFP_ZFProperty_refCount);
    }
    return v;
}
static zfbool _ZFP_ZFPropertyInstanceCleanup(ZF_IN const ZFProperty *property)
{
    zfCoreMutexLocker();
    ZFProperty *v = _ZFP_ZFPropertyMap.get<ZFProperty *>(property->propertyInternalId());
    if(v == zfnull)
    {
        return zffalse;
    }
    --(v->_ZFP_ZFProperty_refCount);
    if(v->_ZFP_ZFProperty_refCount == 0)
    {
        _ZFP_ZFPropertyMap.remove(property->propertyInternalId());
    }
    return zftrue;
}

ZFProperty *_ZFP_ZFPropertyRegister(ZF_IN zfbool propertyIsUserRegister
                                    , ZF_IN zfbool propertyIsDynamicRegister
                                    , ZF_IN ZFObject *propertyDynamicRegisterUserData
                                    , ZF_IN const ZFClass *propertyOwnerClass
                                    , ZF_IN const zfchar *name
                                    , ZF_IN const zfchar *typeName
                                    , ZF_IN const zfchar *typeIdName
                                    , ZF_IN const ZFMethod *setterMethod
                                    , ZF_IN const ZFMethod *getterMethod
                                    , ZF_IN _ZFP_ZFPropertyMethodCleanup setterMethodCleanup
                                    , ZF_IN _ZFP_ZFPropertyMethodCleanup getterMethodCleanup
                                    , ZF_IN const ZFClass *propertyClassOfRetainProperty
                                    , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
                                    , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
                                    , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
                                    , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
                                    , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
                                    , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
                                    )
{
    zfCoreMutexLocker();
    ZFProperty *propertyInfo = zfnull;

    zfCoreAssert(propertyOwnerClass != zfnull);
    zfCoreAssert(name != zfnull && *name != '\0');
    zfCoreAssert(typeName != zfnull && *typeName != '\0');
    zfCoreAssert(typeIdName != zfnull && *typeIdName != '\0');
    zfCoreAssert(setterMethod != zfnull);
    zfCoreAssert(getterMethod != zfnull);
    zfCoreAssert(callbackIsValueAccessed != zfnull);
    zfCoreAssert(callbackIsInitValue != zfnull);
    zfCoreAssert(callbackValueReset != zfnull);

    zfstring propertyInternalId;
    _ZFP_ZFPropertyInstanceSig(propertyInternalId, propertyOwnerClass->classNameFull(), name);

    if(propertyIsUserRegister)
    {
        propertyInfo = _ZFP_ZFPropertyInstanceFind(propertyInternalId);
        zfCoreAssertWithMessageTrim(propertyInfo == zfnull,
            "[ZFPropertyUserRegister] registering a property that already registered, class: %s, propertyName: %s",
            propertyOwnerClass->classNameFull(),
            name);
    }
    else if(propertyIsDynamicRegister)
    {
        propertyInfo = _ZFP_ZFPropertyInstanceFind(propertyInternalId);
        zfCoreAssertWithMessageTrim(propertyInfo == zfnull,
            "[ZFPropertyDynamicRegister] registering a property that already registered, class: %s, propertyName: %s",
            propertyOwnerClass->classNameFull(),
            name);
    }
    propertyInfo = _ZFP_ZFPropertyInstanceAccess(propertyInternalId);

    if(propertyInfo->_ZFP_ZFPropertyNeedInit)
    {
        propertyInfo->_ZFP_ZFPropertyNeedInit = zffalse;
        propertyInfo->_ZFP_ZFPropertyInit(
            propertyIsUserRegister,
            propertyIsDynamicRegister,
            propertyDynamicRegisterUserData,
            propertyOwnerClass,
            name,
            typeName,
            typeIdName,
            setterMethod,
            getterMethod,
            setterMethodCleanup,
            getterMethodCleanup,
            propertyClassOfRetainProperty);
        propertyInfo->callbackIsValueAccessed = callbackIsValueAccessed;
        propertyInfo->callbackIsInitValue = callbackIsInitValue;
        propertyInfo->callbackValueReset = callbackValueReset;
        propertyInfo->callbackUserRegisterInitValueSetup = callbackUserRegisterInitValueSetup;
        propertyInfo->_ZFP_ZFProperty_callbackEnsureInit = callbackEnsureInit;
        propertyInfo->_ZFP_ZFProperty_callbackDealloc = callbackDealloc;

        propertyOwnerClass->_ZFP_ZFClass_propertyRegister(propertyInfo);
        _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, zfnull, propertyInfo, zfnull);
    }

    return propertyInfo;
}
void _ZFP_ZFPropertyUnregister(ZF_IN const ZFProperty *propertyInfo)
{
    zfCoreMutexLocker();
    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeDetach, zfnull, propertyInfo, zfnull);

    if(propertyInfo->_ZFP_ZFProperty_setterMethodCleanup != zfnull)
    {
        propertyInfo->_ZFP_ZFProperty_setterMethodCleanup(propertyInfo->setterMethod());
    }
    if(propertyInfo->_ZFP_ZFProperty_getterMethodCleanup != zfnull)
    {
        propertyInfo->_ZFP_ZFProperty_getterMethodCleanup(propertyInfo->getterMethod());
    }
    propertyInfo->propertyOwnerClass()->_ZFP_ZFClass_propertyUnregister(propertyInfo);
    zfRetainChange(propertyInfo->_ZFP_ZFProperty_removeConst()->_ZFP_ZFProperty_propertyDynamicRegisterUserData, zfnull);
    _ZFP_ZFPropertyInstanceCleanup(propertyInfo);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFPropertyGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret), ZFMP_IN_OPT(const ZFFilterForZFProperty *, propertyFilter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFProperty *>, ZFPropertyGetAll, ZFMP_IN_OPT(const ZFFilterForZFProperty *, propertyFilter, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

