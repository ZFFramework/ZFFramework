#include "ZFPropertyDynamicRegister.h"
#include "ZFPropertyDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic) {
}
zfstlmap<const ZFProperty *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFPropertyDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyDynamicRegisterAutoRemove) {
    zfstlmap<const ZFProperty *, zfbool> t;
    t.swap(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m);
    for(zfstlmap<const ZFProperty *, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
        _ZFP_ZFPropertyUnregister(it->first);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFPropertyDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_ZFPropertyLifeCycleIsReadonly(ZF_IN ZFPropertyLifeCycle lifeCycle) {
    switch(lifeCycle) {
        case ZFPropertyLifeCycleOnInit:
        case ZFPropertyLifeCycleOnVerify:
            return zffalse;
        default:
            return zftrue;
    }
}
static ZFCoreArray<_ZFP_PropLifeCycleData> &_ZFP_ZFPropertyLifeCycleDataRef(
        ZF_IN ZFPropertyLifeCycle lifeCycle
        , ZF_IN const ZFProperty *property
        ) {
    switch(lifeCycle) {
        case ZFPropertyLifeCycleOnInit:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
        case ZFPropertyLifeCycleOnVerify:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnVerify;
        case ZFPropertyLifeCycleOnAttach:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnAttach;
        case ZFPropertyLifeCycleOnDetach:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDetach;
        default:
            zfCoreCriticalShouldNotGoHere();
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
    }
}

zfclass _ZFP_I_PropDynRetainHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRetainHolder, ZFObject)
public:
    zfauto zfv;

public:
    zfoverride
    ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj) {
        return ZFObjectCompare(this->zfv, anotherObj);
    }
};
zfclass _ZFP_I_PropDynRegData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRegData, ZFObject)

public:
    const ZFProperty *propertySaved;
    const ZFTypeInfo *typeInfo; // null for retain property
    ZFPropertyCallbackDynamicRegisterInitValueGetter initValueCallback;
    /*
     * for assign property, store ZFTypeIdWrapper, ensured not null if accessed
     * for retain property, store _ZFP_I_PropDynRetainHolder, ensured not null if accessed
     */
    zfstring tagKey_propertyValue;
public:
    void objectAttach(ZF_IN ZFObject *obj) {
        if(this->_objAttached.find(obj) == this->_objAttached.end()) {
            obj->observerAdd(ZFObject::EventObjectBeforeDealloc(), this->_objOnDeallocListener);
        }
        this->_objAttached[obj] = zftrue;
    }
    void objectDetach(ZF_IN ZFObject *obj) {
        zfstlmap<ZFObject *, zfbool>::iterator it = this->_objAttached.find(obj);
        zfCoreAssert(it != this->_objAttached.end());
        this->_objAttached.erase(it);
        this->objectDetachAction(obj);
    }
    void objectDetachAll(void) {
        zfstlmap<ZFObject *, zfbool> t;
        t.swap(this->_objAttached);
        for(zfstlmap<ZFObject *, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
            this->objectDetachAction(it->first);
        }
    }
    void objectDetachAction(ZF_IN ZFObject *obj) {
        obj->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->_objOnDeallocListener);
        zfauto propertyValue = obj->objectTagRemoveAndGet(this->tagKey_propertyValue);
        this->propLifeCycle(ZFPropertyLifeCycleOnDetach, obj, this->propertySaved, propertyValue, propertyValue);
    }

    /*
     * if success, ret ensured:
     * * store ZFTypeIdWrapper for assign property
     * * store _ZFP_I_PropDynRetainHolder for retain property
     */
    zfbool initValue(
            ZF_OUT zfauto &ret
            , ZF_IN const ZFProperty *property
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        if(property->propertyIsRetainProperty()) {
            zfobj<_ZFP_I_PropDynRetainHolder> holder;
            if(this->initValueCallback != zfnull) {
                holder->zfv = this->initValueCallback(property);
            }
            if(holder->zfv != zfnull && !holder->zfv->classData()->classIsTypeOf(property->propertyClassOfRetainProperty())) {
                zfstringAppend(errorHint,
                    "invalid init value %s, desired: %s",
                    holder->zfv,
                    property->propertyClassOfRetainProperty()->classNameFull());
                return zffalse;
            }
            ret = holder;
        }
        else {
            if(this->initValueCallback != zfnull) {
                ret = this->initValueCallback(property);
            }
            if(ret == zfnull) {
                const ZFClass *propClass = this->typeInfo->typeIdClass();
                if(propClass != zfnull) {
                    if(propClass->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
                        ret = propClass->newInstance();
                    }
                    else {
                        zfobj<_ZFP_I_PropDynRetainHolder> holder;
                        ret = holder;
                    }
                    return zftrue;
                }
            }
            ZFTypeIdWrapper *wrapper = ret;
            if(wrapper == zfnull) {
                zfstringAppend(errorHint,
                    "invalid init value %s, desired: %s",
                    ret,
                    property->propertyTypeId());
                return zffalse;
            }
        }
        return zftrue;
    }
    void propLifeCycle(
            ZF_IN ZFPropertyLifeCycle lifeCycle
            , ZF_IN ZFObject *propertyOwnerObject
            , ZF_IN const ZFProperty *property
            , ZF_IN_OUT ZFObject *propertyValueHolder
            , ZF_IN ZFObject *propertyValueOldHolder
            ) {
        zfCoreAssert(propertyValueOldHolder != zfnull);

        zfbool isReadonly = _ZFP_ZFPropertyLifeCycleIsReadonly(lifeCycle);
        zfauto valueHolder;
        void *propertyValue = zfnull;
        const void *propertyValueOld = zfnull;
        _ZFP_I_PropDynRetainHolder *holder = zfcast(_ZFP_I_PropDynRetainHolder *, propertyValueHolder);
        _ZFP_I_PropDynRetainHolder *holderOld = zfcast(_ZFP_I_PropDynRetainHolder *, propertyValueOldHolder);
        if(holder != zfnull) {
            if(isReadonly) {
                valueHolder = holder->zfv;
                propertyValue = &valueHolder;
            }
            else {
                propertyValue = &(holder->zfv);
            }
            propertyValueOld = &(holderOld->zfv);
        }
        else {
            if(isReadonly) {
                valueHolder = this->typeInfo->typeIdClass()->newInstance();
                valueHolder.to<ZFTypeIdWrapper *>()->wrappedValue(propertyValueHolder->to<ZFTypeIdWrapper *>()->wrappedValue());
                propertyValue = valueHolder.to<ZFTypeIdWrapper *>()->wrappedValue();
            }
            else {
                propertyValue = propertyValueHolder->to<ZFTypeIdWrapper *>()->wrappedValue();
            }
            propertyValueOld = propertyValueOldHolder->to<ZFTypeIdWrapper *>()->wrappedValue();
        }

        ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycle, property);
        if(lifeCycle == ZFPropertyLifeCycleOnDetach) {
            for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
                const _ZFP_PropLifeCycleData &p = d[i];
                if(propertyOwnerObject->classData()->classIsTypeOf(p.propertyOwnerClass)) {
                    p.propertyLifeCycleWrapper(propertyOwnerObject, property, propertyValue, propertyValueOld, p.propertyLifeCycleUserData);
                }
            }
        }
        else {
            for(zfindex i = 0, iEnd = d.count(); i < iEnd; ++i) {
                const _ZFP_PropLifeCycleData &p = d[i];
                if(propertyOwnerObject->classData()->classIsTypeOf(p.propertyOwnerClass)) {
                    p.propertyLifeCycleWrapper(propertyOwnerObject, property, propertyValue, propertyValueOld, p.propertyLifeCycleUserData);
                }
            }
        }
    }
private:
    zfstlmap<ZFObject *, zfbool> _objAttached;
    ZFListener _objOnDeallocListener;
    ZFMETHOD_INLINE_1(void, _objOnDealloc
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        this->objectDetach(zfargs.sender());
    }
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_objOnDeallocListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, _objOnDealloc));
    }
};
zfclass _ZFP_I_PropDynRegValueStore : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRegValueStore, ZFObject)

public:
    zfstlmap<void *, zfauto> m;
};

// ============================================================
static zfbool _ZFP_PropDynReg_setterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    const ZFProperty *property = invokerMethod->methodOwnerProperty();
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 1
                , d->typeInfo->typeIdClass()
                )) {
        return zffalse;
    }
    zfauto valueOld = invokerObject->objectTag(d->tagKey_propertyValue);
    zfbool firstTime = (valueOld == zfnull);

    zfauto value;
    if(firstTime) {
        if(!d->initValue(value, property, errorHint)) {
            return zffalse;
        }
        if(!d->initValue(valueOld, property, errorHint)) {
            return zffalse;
        }
        d->objectAttach(invokerObject);
        invokerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnInit, invokerObject, property, value, value);
        value = zfnull;
    }

    zfauto &valueNew = paramList[0];
    do {
        if(property->propertyIsRetainProperty()) {
            if(valueNew != zfnull && !valueNew->classData()->classIsTypeOf(property->propertyClassOfRetainProperty())) {
                break;
            }
            zfobj<_ZFP_I_PropDynRetainHolder> holder;
            holder->zfv = valueNew;
            value = holder;
        }
        else {
            const ZFClass *propClass = d->typeInfo->typeIdClass();
            if(propClass != zfnull && !propClass->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
                zfobj<_ZFP_I_PropDynRetainHolder> holder;
                holder->zfv = valueNew;
                value = holder;
            }
            else {
                ZFTypeIdWrapper *wrapper = zfcast(ZFTypeIdWrapper *, valueNew);
                if(wrapper == zfnull
                        || !zfstringIsEqual(wrapper->wrappedValueTypeId(), property->propertyTypeId())
                        ) {
                    break;
                }
                value = valueNew;
            }
        }
    } while(zffalse);
    if(value == zfnull) {
        zfstringAppend(errorHint, "invalid value: (%s)%s, desired: %s",
            valueNew != zfnull ? valueNew->classData()->className() : ZFTOKEN_zfnull,
            valueNew,
            property->propertyIsRetainProperty()
                ? property->propertyClassOfRetainProperty()->classNameFull()
                : property->propertyTypeId());
        return zffalse;
    }

    if(firstTime) {
        invokerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnVerify, invokerObject, property, value, valueOld);
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, invokerObject, property, value, valueOld);
        invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
    else {
        d->propLifeCycle(ZFPropertyLifeCycleOnDetach, invokerObject, property, valueOld, valueOld);
        invokerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnVerify, invokerObject, property, value, valueOld);
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, invokerObject, property, value, valueOld);

        _ZFP_I_PropDynRetainHolder *holder = valueOld;
        if(holder != zfnull) {
            invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &(holder->zfv));
        }
        else {
            invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property,
                    zfcast(ZFTypeIdWrapper *, valueOld)->wrappedValue()
                );
        }
    }
    return zftrue;
}
static zfbool _ZFP_PropDynReg_getterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 0
                )) {
        return zffalse;
    }
    // ZFTAG_LIMITATION: property's getter would return value reference,
    //   we would store the value to owner object's tag,
    //   see #ZFMethodDynamicRegister for the reason for this behavior
    const ZFProperty *property = invokerMethod->methodOwnerProperty();
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    ZFObject *wrapper = invokerObject->objectTag(d->tagKey_propertyValue);
    zfbool firstTime = (wrapper == zfnull);
    if(firstTime) {
        zfauto initValue;
        if(!d->initValue(initValue, property, errorHint)) {
            return zffalse;
        }
        wrapper = initValue;
        invokerObject->objectTag(d->tagKey_propertyValue, initValue);
        d->objectAttach(invokerObject);
        d->propLifeCycle(ZFPropertyLifeCycleOnInit, invokerObject, property, wrapper, wrapper);
    }
    _ZFP_I_PropDynRetainHolder *wrapperTmp = zfcast(_ZFP_I_PropDynRetainHolder *, wrapper);
    if(wrapperTmp != zfnull) {
        ret = wrapperTmp->zfv;
    }
    else {
        ret = wrapper;
    }
    if(firstTime) {
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, invokerObject, property, wrapper, wrapper);
        invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
    return zftrue;
}

static zfbool _ZFP_PropDynReg_callbackIsValueAccessed(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        ) {
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    return (ownerObj->objectTag(d->tagKey_propertyValue) != zfnull);
}
static zfbool _ZFP_PropDynReg_callbackIsInitValue(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        , ZF_OUT_OPT zfauto *outInitValue /* = zfnull */
        ) {
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    zfbool ret = zffalse;
    ZFObject *tag = ownerObj->objectTag(d->tagKey_propertyValue);
    if(tag == zfnull) {
        ret = zftrue;
        if(outInitValue != zfnull) {
            zfauto initValue;
            if(d->initValue(initValue, property)) {
                _ZFP_I_PropDynRetainHolder *wrapperTmp = initValue;
                if(wrapperTmp != zfnull) {
                    *outInitValue = wrapperTmp->zfv;
                }
                else {
                    *outInitValue = initValue;
                }
            }
        }
    }
    else {
        zfauto initValue;
        if(d->initValue(initValue, property)) {
            _ZFP_I_PropDynRetainHolder *wrapperTmp = initValue;
            if(wrapperTmp != zfnull) {
                if(outInitValue != zfnull) {
                    *outInitValue = wrapperTmp->zfv;
                }
            }
            else {
                if(outInitValue != zfnull) {
                    *outInitValue = initValue;
                }
            }
            ret = (tag->objectCompare(initValue) == ZFCompareTheSame);
        }
    }
    return ret;
}
static void _ZFP_PropDynReg_callbackValueReset(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        ) {
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    ownerObj->objectTagRemove(d->tagKey_propertyValue);
}

// ============================================================
static zfbool _ZFP_ZFPropertyDynamicRegisterCustomImplCheck(
        ZF_IN const ZFPropertyDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
static void _ZFP_ZFPropertyMethodCleanup_DynamicReg(ZF_IN const ZFMethod *method) {
    ZFMethodDynamicUnregister(method);
}
const ZFProperty *ZFPropertyDynamicRegister(
        ZF_IN const ZFPropertyDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.propertyOwnerClass() == zfnull) {
        zfstringAppend(errorHint, "propertyOwnerClass not set");
        return zfnull;
    }
    if(param.propertyTypeId() == zfnull) {
        zfstringAppend(errorHint, "propertyTypeId not set");
        return zfnull;
    }
    if(param.propertyName() == zfnull) {
        zfstringAppend(errorHint, "propertyName not set");
        return zfnull;
    }
    if(param.propertyClassOfRetainProperty() == zfnull) {
        if(ZFClass::classForName(param.propertyTypeId()) != zfnull) {
            zfstringAppend(errorHint,
                "propertyTypeId %s is ZFObject type but propertyClassOfRetainProperty not set, weak property not supported",
                param.propertyTypeId());
            return zfnull;
        }
    }
    else {
        if(!zfstringIsEqual(param.propertyTypeId(), param.propertyClassOfRetainProperty()->classNameFull())) {
            zfstringAppend(errorHint,
                "propertyTypeId must be same as propertyClassOfRetainProperty for retain property");
            return zfnull;
        }
    }

    const ZFTypeInfo *d = zfnull;
    if(param.propertyClassOfRetainProperty() == zfnull) {
        d = ZFTypeInfoForName(param.propertyTypeId());
        if(d == zfnull) {
            zfstringAppend(errorHint,
                "propertyTypeId %s not registered",
                param.propertyTypeId());
            return zfnull;
        }
    }

    const ZFProperty *existProperty = param.propertyOwnerClass()->propertyForName(param.propertyName());
    if(existProperty != zfnull) {
        zfstringAppend(errorHint,
            "property with same name already exists: %s",
            existProperty);
        return zfnull;
    }

    zfobj<_ZFP_I_PropDynRegData> userDataWrapper;
    userDataWrapper->propertySaved = zfnull;
    userDataWrapper->typeInfo = d;
    userDataWrapper->initValueCallback = param.propertyInitValueCallback();
    zfstringAppend(userDataWrapper->tagKey_propertyValue, "PropDyn_%s", param.propertyName());
    const ZFProperty *property = zfnull;

    if(param.propertyCustomImplSetterMethod() != zfnull
            || param.propertyCustomImplGetterMethod() != zfnull
            ) {
        if(!_ZFP_ZFPropertyDynamicRegisterCustomImplCheck(param, errorHint)) {
            return zfnull;
        }
        property = _ZFP_ZFPropertyRegister(zffalse
            , zftrue
            , param.propertyDynamicRegisterUserData()
            , param.propertyOwnerClass()
            , param.propertyName()
            , param.propertyTypeName()
            , param.propertyTypeId()
            , param.propertyCustomImplSetterMethod()
            , param.propertyCustomImplGetterMethod()
            , zfnull
            , zfnull
            , param.propertyClassOfRetainProperty()
            , param.propertyCustomImplCallbackIsValueAccessed()
            , param.propertyCustomImplCallbackIsInitValue()
            , param.propertyCustomImplCallbackValueReset()
            , zfnull
            , param.propertyInitValueCallback()
            , zfnull
            , zfnull
            );
    }
    else {
        zfstring errorHintTmp;
        const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodDynamicRegisterUserData(userDataWrapper)
                .methodOwnerClass(param.propertyOwnerClass())
                .methodGenericInvoker(_ZFP_PropDynReg_setterGI)
                .methodType(ZFMethodTypeVirtual)
                .methodPrivilegeType(param.propertySetterType())
                .methodName(param.propertyName())
                .methodReturnTypeId(ZFTypeId_void())
                .methodReturnTypeName("void")
                .methodParamAdd(
                    param.propertyTypeId(),
                    zfstr("%s const &", param.propertyTypeName()))
            , &errorHintTmp);
        if(setterMethod == zfnull) {
            zfstringAppend(errorHint,
                "failed to register setter method, reason: %s",
                errorHintTmp);
            return zfnull;
        }
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodDynamicRegisterUserData(userDataWrapper)
                .methodOwnerClass(param.propertyOwnerClass())
                .methodGenericInvoker(_ZFP_PropDynReg_getterGI)
                .methodType(ZFMethodTypeVirtual)
                .methodPrivilegeType(param.propertyGetterType())
                .methodName(param.propertyName())
                .methodReturnTypeId(param.propertyTypeId())
                .methodReturnTypeName(zfstr("%s const &", param.propertyTypeName()))
            , &errorHintTmp);
        if(getterMethod == zfnull) {
            zfstringAppend(errorHint,
                "failed to register getter method, reason: %s",
                errorHintTmp);
            ZFMethodDynamicUnregister(setterMethod);
            return zfnull;
        }

        property = _ZFP_ZFPropertyRegister(zffalse
            , zftrue
            , param.propertyDynamicRegisterUserData()
            , param.propertyOwnerClass()
            , param.propertyName()
            , param.propertyTypeName()
            , param.propertyTypeId()
            , setterMethod
            , getterMethod
            , _ZFP_ZFPropertyMethodCleanup_DynamicReg
            , _ZFP_ZFPropertyMethodCleanup_DynamicReg
            , param.propertyClassOfRetainProperty()
            , _ZFP_PropDynReg_callbackIsValueAccessed
            , _ZFP_PropDynReg_callbackIsInitValue
            , _ZFP_PropDynReg_callbackValueReset
            , zfnull
            , param.propertyInitValueCallback()
            , zfnull
            , zfnull
            );
    }

    userDataWrapper->propertySaved = property;
    property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper = zfRetain(userDataWrapper);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m[property] = zftrue;
    return property;
}
void ZFPropertyDynamicUnregister(ZF_IN const ZFProperty *property) {
    if(property != zfnull) {
        zfCoreAssert(property->propertyIsDynamicRegister());
        zfCoreMutexLocker();
        zfblockedRelease(property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m.erase(property);
        _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
        d->objectDetachAll();
        _ZFP_ZFPropertyUnregister(property);
    }
}

static zfbool _ZFP_ZFPropertyDynamicRegisterCustomImplCheck(
        ZF_IN const ZFPropertyDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.propertyCustomImplSetterMethod() == zfnull) {
        zfstringAppend(errorHint, "propertyCustomImplSetterMethod not set");
        return zffalse;
    }
    if(param.propertyCustomImplGetterMethod() == zfnull) {
        zfstringAppend(errorHint, "propertyCustomImplGetterMethod not set");
        return zffalse;
    }

    if(zfsncmp(param.propertyCustomImplSetterMethod()->methodName(), param.propertyName(), zfslen(param.propertyName())) != 0) {
        zfstringAppend(errorHint,
            "setter method name \"%s\" does not match property name \"%s\"",
            param.propertyCustomImplSetterMethod()->methodName(),
            param.propertyName());
        return zffalse;
    }
    if(param.propertyCustomImplSetterMethod()->methodParamCount() != 1
            || !zfstringIsEqual(param.propertyCustomImplSetterMethod()->methodReturnTypeId(), ZFTypeId_void())
            || !zfstringIsEqual(param.propertyCustomImplSetterMethod()->methodParamTypeIdAt(0), param.propertyTypeId())
            ) {
        zfstringAppend(errorHint,
            "setter method signature mismatch: %s, desired: void setter(%s const &)",
            param.propertyCustomImplSetterMethod(),
            param.propertyTypeId());
        return zffalse;
    }

    if(!zfstringIsEqual(param.propertyCustomImplGetterMethod()->methodName(), param.propertyName())) {
        zfstringAppend(errorHint,
            "getter method name \"%s\" does not match property name \"%s\"",
            param.propertyCustomImplGetterMethod()->methodName(),
            param.propertyName());
        return zffalse;
    }
    if(param.propertyCustomImplGetterMethod()->methodParamCount() != 0
            || !zfstringIsEqual(param.propertyCustomImplGetterMethod()->methodReturnTypeId(), param.propertyTypeId())
            ) {
        zfstringAppend(errorHint,
            "getter method signature mismatch: %s, desired: %s const &getter(void)",
            param.propertyCustomImplGetterMethod(),
            param.propertyTypeId());
        return zffalse;
    }

    if(param.propertyCustomImplCallbackIsValueAccessed() == zfnull) {
        zfstringAppend(errorHint, "propertyCustomImplCallbackIsValueAccessed not set");
        return zffalse;
    }

    if(param.propertyCustomImplCallbackIsInitValue() == zfnull) {
        zfstringAppend(errorHint, "propertyCustomImplCallbackIsInitValue not set");
        return zffalse;
    }

    if(param.propertyCustomImplCallbackValueReset() == zfnull) {
        zfstringAppend(errorHint, "propertyCustomImplCallbackValueReset not set");
        return zffalse;
    }

    return zftrue;
}

// ============================================================
zfclass _ZFP_I_PropDynRegLifeCycleData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRegLifeCycleData, ZFObject)
public:
    ZFPropertyLifeCycle lifeCycle;
    ZFListener callback;
};

static void _ZFP_ZFPropertyDynamicRegisterLifeCycleWrapper(
        ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN void *propertyValue
        , ZF_IN const void *propertyValueOld
        , ZF_IN ZFObject *propertyLifeCycleUserData
        ) {
    _ZFP_I_PropDynRegLifeCycleData *implUserData = zfcast(_ZFP_I_PropDynRegLifeCycleData *, propertyLifeCycleUserData);
    if(implUserData == zfnull || !implUserData->callback) {
        return;
    }

    zfobj<ZFPropertyInvokeData> invokeData;
    invokeData->invokerObject = propertyOwnerObject;
    invokeData->invokerProperty = property;

    if(property->propertyIsRetainProperty()) {
        invokeData->propertyValue = ((zfauto *)propertyValue)->toObject();
        invokeData->propertyValueOld = *(zfauto *)propertyValueOld;

        implUserData->callback.execute(ZFArgs()
                .sender(propertyOwnerObject)
                .param0(invokeData)
            );
        if(!_ZFP_ZFPropertyLifeCycleIsReadonly(implUserData->lifeCycle)) {
            *(zfauto *)propertyValue = invokeData->propertyValue;
        }
    }
    else {
        const ZFTypeInfo *typeInfo = ZFTypeInfoForName(property->propertyTypeId());
        if(typeInfo == zfnull
            || typeInfo->typeIdClass() == zfnull
            || !typeInfo->typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())
        ) {
            return;
        }
        invokeData->propertyValue = typeInfo->typeIdClass()->newInstance();
        invokeData->propertyValueOld = typeInfo->typeIdClass()->newInstance();
        ZFTypeIdWrapper *propertyValueHolder = invokeData->propertyValue;
        ZFTypeIdWrapper *propertyValueOldHolder = invokeData->propertyValueOld;
        if(propertyValueHolder == zfnull || propertyValueOldHolder == zfnull) {
            return;
        }
        propertyValueHolder->wrappedValue(propertyValue);
        propertyValueOldHolder->wrappedValue(propertyValueOld);

        implUserData->callback.execute(ZFArgs()
                .sender(propertyOwnerObject)
                .param0(invokeData)
            );
        if(!_ZFP_ZFPropertyLifeCycleIsReadonly(implUserData->lifeCycle)) {
            propertyValueHolder->wrappedValueCopy(propertyValue);
        }
    }
}

zfbool ZFPropertyDynamicRegisterLifeCycle(
        ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClassOrNull
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        , ZF_IN const ZFListener &callback
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(property == zfnull) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "null property");
        }
        return zffalse;
    }
    if(property == zfnull || !callback) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint, "invalid callback");
        }
        return zffalse;
    }
    if(ownerClassOrNull == zfnull) {
        ownerClassOrNull = property->propertyOwnerClass();
    }
    else {
        if(!ownerClassOrNull->classIsTypeOf(property->propertyOwnerClass())) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "ownerClassOrNull must be type of %s, got: %s",
                    property->propertyOwnerClass()->className(),
                    ownerClassOrNull->className());
            }
            return zffalse;
        }
    }
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycle, property);
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
        if(d[i].propertyOwnerClass == ownerClassOrNull) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "property already has a custom life cycle handler: %s",
                    property);
            }
            return zffalse;
        }
    }

    _ZFP_PropLifeCycleData data;
    data.propertyOwnerClass = property->propertyOwnerClass();
    data.propertyLifeCycleWrapper = _ZFP_ZFPropertyDynamicRegisterLifeCycleWrapper;
    zfobj<_ZFP_I_PropDynRegLifeCycleData> implUserData;
    implUserData->lifeCycle = lifeCycle;
    implUserData->callback = callback;
    data.propertyLifeCycleUserData = implUserData;
    d.add(data);
    return zftrue;
}
zfbool ZFPropertyDynamicUnregisterLifeCycle(
        ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClassOrNull
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        ) {
    if(property == zfnull) {
        return zffalse;
    }
    if(ownerClassOrNull == zfnull) {
        ownerClassOrNull = property->propertyOwnerClass();
    }
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycle, property);
    for(zfindex i = 0; i < d.count(); ++i) {
        _ZFP_I_PropDynRegLifeCycleData *implUserData = d[i].propertyLifeCycleUserData;
        if(d[i].propertyOwnerClass == ownerClassOrNull && implUserData != zfnull) {
            d.remove(i);
            return zftrue;
        }
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

