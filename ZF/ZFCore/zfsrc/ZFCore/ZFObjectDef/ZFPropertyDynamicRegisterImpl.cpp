#include "ZFPropertyDynamicRegister.h"
#include "ZFPropertyDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic) {
}
zfstlhashmap<const ZFProperty *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFPropertyDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyDynamicRegisterAutoRemove) {
    zfstlhashmap<const ZFProperty *, zfbool> t;
    t.swap(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m);
    for(zfstlhashmap<const ZFProperty *, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
        _ZFP_ZFPropertyUnregister(it->first);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFPropertyDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_ZFPropertyLifeCycleIsReadonly(ZF_IN ZFPropertyLifeCycle lifeCycle) {
    switch(lifeCycle) {
        case ZFPropertyLifeCycleOnInit:
        case ZFPropertyLifeCycleOnUpdate:
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
        case ZFPropertyLifeCycleOnUpdate:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnUpdate;
        case ZFPropertyLifeCycleOnAttach:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnAttach;
        case ZFPropertyLifeCycleOnDetach:
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDetach;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
    }
}

zfclass _ZFP_I_PropDynRetainHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRetainHolder, ZFObject)
public:
    zfauto zfv;
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
            obj->observerAdd(ZFObject::E_ObjectBeforeDealloc(), this->_objOnDeallocListener);
        }
        this->_objAttached[obj] = zftrue;
    }
    void objectDetach(ZF_IN ZFObject *obj) {
        zfstlhashmap<ZFObject *, zfbool>::iterator it = this->_objAttached.find(obj);
        ZFCoreAssert(it != this->_objAttached.end());
        this->_objAttached.erase(it);
        this->objectDetachAction(obj);
    }
    void objectDetachAll(void) {
        zfstlhashmap<ZFObject *, zfbool> t;
        t.swap(this->_objAttached);
        for(zfstlhashmap<ZFObject *, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
            this->objectDetachAction(it->first);
        }
    }
    void objectDetachAction(ZF_IN ZFObject *obj) {
        obj->observerRemove(ZFObject::E_ObjectBeforeDealloc(), this->_objOnDeallocListener);
        zfauto propertyValue = obj->objectTagRemoveAndGet(this->tagKey_propertyValue);
        if(propertyValue) {
            this->propLifeCycle(ZFPropertyLifeCycleOnDetach, obj, this->propertySaved, propertyValue, propertyValue);
        }
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
        if(property->isRetainProperty()) {
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
        ZFCoreAssert(propertyValueOldHolder != zfnull);

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
                if(propertyOwnerObject->classData()->classIsTypeOf(p.ownerClass)) {
                    p.propertyLifeCycleWrapper(propertyOwnerObject, property, propertyValue, propertyValueOld, p.propertyLifeCycleUserData);
                }
            }
        }
        else {
            for(zfindex i = 0, iEnd = d.count(); i < iEnd; ++i) {
                const _ZFP_PropLifeCycleData &p = d[i];
                if(propertyOwnerObject->classData()->classIsTypeOf(p.ownerClass)) {
                    p.propertyLifeCycleWrapper(propertyOwnerObject, property, propertyValue, propertyValueOld, p.propertyLifeCycleUserData);
                }
            }
        }
    }
private:
    zfstlhashmap<ZFObject *, zfbool> _objAttached;
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
    zfstlhashmap<void *, zfbool> m;
};

// ============================================================
static void _ZFP_PropDynReg_setterGI(ZF_IN_OUT const ZFArgs &zfargs) {
    if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
        return;
    }
    ZFObject *ownerObject = zfargs.sender();
    const ZFProperty *property = zfargs.ownerMethod()->ownerProperty();
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
    zfauto valueOld = ownerObject->objectTag(d->tagKey_propertyValue);
    zfbool firstTime = (valueOld == zfnull);

    zfauto value;
    if(firstTime) {
        zfstring errorHint;
        if(!d->initValue(value, property, !zfargs.ignoreError() ? &errorHint : zfnull)) {
            zfargs.success(zffalse);
            if(!zfargs.ignoreError()) {
                zfargs.errorHint(errorHint);
            }
            return;
        }
        if(!d->initValue(valueOld, property, !zfargs.ignoreError() ? &errorHint : zfnull)) {
            zfargs.success(zffalse);
            if(!zfargs.ignoreError()) {
                zfargs.errorHint(errorHint);
            }
            return;
        }
        d->objectAttach(ownerObject);
        ownerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnInit, ownerObject, property, value, value);
        value = zfnull;
    }

    zfauto valueNew = zfargs.param0();
    do {
        if(property->isRetainProperty()) {
            if(valueNew != zfnull && !valueNew->classData()->classIsTypeOf(property->propertyClassOfRetainProperty())) {
                break;
            }
            zfobj<_ZFP_I_PropDynRetainHolder> holder;
            holder->zfv = valueNew;
            value = holder;
        }
        else {
            const ZFClass *propClass = d->typeInfo->typeIdClass();
            if(propClass) {
                if(!propClass->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
                    zfobj<_ZFP_I_PropDynRetainHolder> holder;
                    holder->zfv = valueNew;
                    value = holder;
                }
                else {
                    if(!valueNew) {
                        value = propClass->newInstance();
                    }
                    else {
                        ZFTypeIdWrapper *wrapper = zfcast(ZFTypeIdWrapper *, valueNew);
                        if(wrapper == zfnull
                                || !zfstringIsEqual(wrapper->zfvTypeId(), property->propertyTypeId())
                                ) {
                            break;
                        }
                        value = valueNew;
                    }
                }
            }
        }
    } while(zffalse);
    if(value == zfnull) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("invalid value: (%s)%s, desired: %s"
                        , valueNew != zfnull ? valueNew->classData()->className().cString() : ZFTOKEN_zfnull
                        , valueNew
                        , property->isRetainProperty()
                        ? property->propertyClassOfRetainProperty()->classNameFull()
                        : property->propertyTypeId()
                        ));
        }
        return;
    }

    if(firstTime) {
        ownerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnUpdate, ownerObject, property, value, valueOld);
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, ownerObject, property, value, valueOld);
        ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
    else {
        d->propLifeCycle(ZFPropertyLifeCycleOnDetach, ownerObject, property, valueOld, valueOld);
        ownerObject->objectTag(d->tagKey_propertyValue, value);
        d->propLifeCycle(ZFPropertyLifeCycleOnUpdate, ownerObject, property, value, valueOld);
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, ownerObject, property, value, valueOld);

        _ZFP_I_PropDynRetainHolder *holder = valueOld;
        if(holder != zfnull) {
            ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &(holder->zfv));
        }
        else {
            ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property,
                    zfcast(ZFTypeIdWrapper *, valueOld)->wrappedValue()
                );
        }
    }
}
static void _ZFP_PropDynReg_getterGI(ZF_IN_OUT const ZFArgs &zfargs) {
    if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
        return;
    }
    ZFObject *ownerObject = zfargs.sender();
    // ZFTAG_LIMITATION: property's getter would return value reference,
    //   we would store the value to owner object's tag,
    //   see #ZFMethodDynamicRegister for the reason for this behavior
    const ZFProperty *property = zfargs.ownerMethod()->ownerProperty();
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
    ZFObject *wrapper = ownerObject->objectTag(d->tagKey_propertyValue);
    zfbool firstTime = (wrapper == zfnull);
    if(firstTime) {
        zfauto initValue;
        zfstring errorHint;
        if(!d->initValue(initValue, property, !zfargs.ignoreError() ? &errorHint : zfnull)) {
            zfargs.success(zffalse);
            if(!zfargs.ignoreError()) {
                zfargs.errorHint(errorHint);
            }
            return;
        }
        wrapper = initValue;
        ownerObject->objectTag(d->tagKey_propertyValue, initValue);
        d->objectAttach(ownerObject);
        d->propLifeCycle(ZFPropertyLifeCycleOnInit, ownerObject, property, wrapper, wrapper);
    }
    _ZFP_I_PropDynRetainHolder *wrapperTmp = zfcast(_ZFP_I_PropDynRetainHolder *, wrapper);
    if(wrapperTmp != zfnull) {
        zfargs.result(wrapperTmp->zfv);
    }
    else {
        zfargs.result(wrapper);
    }
    if(firstTime) {
        d->propLifeCycle(ZFPropertyLifeCycleOnAttach, ownerObject, property, wrapper, wrapper);
        ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}

static zfbool _ZFP_PropDynReg_callbackIsValueAccessed(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        ) {
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
    return (ownerObj->objectTag(d->tagKey_propertyValue) != zfnull);
}
static zfbool _ZFP_PropDynReg_callbackIsInitValue(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        , ZF_OUT_OPT zfauto *outInitValue /* = zfnull */
        ) {
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
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
            _ZFP_I_PropDynRetainHolder *wrapperTmpCur = zfcast(_ZFP_I_PropDynRetainHolder *, tag);
            _ZFP_I_PropDynRetainHolder *wrapperTmp = initValue;
            if(wrapperTmp != zfnull) {
                if(outInitValue != zfnull) {
                    *outInitValue = wrapperTmp->zfv;
                }
                ret = (ZFObjectCompare(wrapperTmpCur->zfv, wrapperTmp->zfv) == ZFCompareEqual);
            }
            else {
                if(outInitValue != zfnull) {
                    *outInitValue = initValue;
                }
                ret = (tag->objectCompare(initValue) == ZFCompareEqual);
            }
        }
    }
    return ret;
}
static void _ZFP_PropDynReg_callbackValueReset(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &ownerObj
        ) {
    if(_ZFP_PropDynReg_callbackIsValueAccessed(property, ownerObj)) {
        ownerObj->_ZFP_ZFObject_objectPropertyValueOnReset(property);
    }
    _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
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
    if(param.ownerClass() == zfnull) {
        zfstringAppend(errorHint, "ownerClass not set");
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
    const ZFTypeInfo *d = zfnull;
    if(param.propertyClassOfRetainProperty() == zfnull) {
        d = ZFTypeInfoForName(param.propertyTypeId());
        if(d == zfnull) {
            const ZFClass *clsTmp = ZFClass::classForName(param.propertyTypeId());
            if(clsTmp != zfnull) {
                zfstringAppend(errorHint,
                    "propertyTypeId %s is ZFObject type but propertyClassOfRetainProperty not set, weak property not supported",
                    param.propertyTypeId());
            }
            else {
                zfstringAppend(errorHint,
                    "propertyTypeId %s not registered",
                    param.propertyTypeId());
            }
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

    if(!param.zfunsafe_disableChecker()) {
        const ZFProperty *existProperty = param.ownerClass()->propertyForName(param.propertyName());
        if(existProperty != zfnull) {
            zfstringAppend(errorHint,
                "property with same name already exists: %s",
                existProperty);
            return zfnull;
        }
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
            , param.dynamicRegisterUserData()
            , param.ownerClass()
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
                .dynamicRegisterUserData(userDataWrapper)
                .ownerClass(param.ownerClass())
                .methodGenericInvoker(_ZFP_PropDynReg_setterGI)
                .methodType(ZFMethodTypeVirtual)
                .methodAccessType(param.propertySetterType())
                .methodName(param.propertyName())
                .returnTypeId(ZFTypeId_void())
                .methodParam(param.propertyTypeId(), "v")
            , &errorHintTmp);
        if(setterMethod == zfnull) {
            zfstringAppend(errorHint,
                "failed to register setter method, reason: %s",
                errorHintTmp);
            return zfnull;
        }
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .dynamicRegisterUserData(userDataWrapper)
                .ownerClass(param.ownerClass())
                .methodGenericInvoker(_ZFP_PropDynReg_getterGI)
                .methodType(ZFMethodTypeVirtual)
                .methodAccessType(param.propertyGetterType())
                .methodName(param.propertyName())
                .returnTypeId(param.propertyTypeId())
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
            , param.dynamicRegisterUserData()
            , param.ownerClass()
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
    property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper = zfRetain(userDataWrapper);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m[property] = zftrue;
    return property;
}
void ZFPropertyDynamicUnregister(ZF_IN const ZFProperty *property) {
    if(property != zfnull) {
        ZFCoreAssert(property->isDynamicRegister());
        ZFCoreMutexLocker();
        zfblockedRelease(property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterDataHolder)->m.erase(property);
        _ZFP_I_PropDynRegData *d = zfcast(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_dynamicRegisterUserDataWrapper);
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

    if(!zfstringBeginWith(param.propertyCustomImplSetterMethod()->methodName(), param.propertyName())) {
        zfstringAppend(errorHint,
            "setter method name \"%s\" does not match property name \"%s\"",
            param.propertyCustomImplSetterMethod()->methodName(),
            param.propertyName());
        return zffalse;
    }
    if(param.propertyCustomImplSetterMethod()->paramCount() != 1
            || !zfstringIsEqual(param.propertyCustomImplSetterMethod()->returnTypeId(), ZFTypeId_void())
            || !zfstringIsEqual(param.propertyCustomImplSetterMethod()->paramTypeIdAt(0), param.propertyTypeId())
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
    if(param.propertyCustomImplGetterMethod()->paramCount() != 0
            || !zfstringIsEqual(param.propertyCustomImplGetterMethod()->returnTypeId(), param.propertyTypeId())
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

    ZFArgs zfargs;
    zfargs.sender(propertyOwnerObject);
    zfargs.ownerProperty(property);

    if(property->isRetainProperty()) {
        zfargs.propValue(*(zfauto *)propertyValue);
        zfargs.propValueOld(*(zfauto *)propertyValueOld);

        implUserData->callback.execute(zfargs);
        if(!_ZFP_ZFPropertyLifeCycleIsReadonly(implUserData->lifeCycle)) {
            *(zfauto *)propertyValue = zfargs.propValue();
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
        zfargs.propValue(typeInfo->typeIdClass()->newInstance());
        zfargs.propValueOld(typeInfo->typeIdClass()->newInstance());
        ZFTypeIdWrapper *propertyValueHolder = zfargs.propValue();
        ZFTypeIdWrapper *propertyValueOldHolder = zfargs.propValueOld();
        if(propertyValueHolder == zfnull || propertyValueOldHolder == zfnull) {
            return;
        }
        propertyValueHolder->wrappedValue(propertyValue);
        propertyValueOldHolder->wrappedValue(propertyValueOld);

        implUserData->callback.execute(zfargs);
        if(!_ZFP_ZFPropertyLifeCycleIsReadonly(implUserData->lifeCycle)) {
            // get again, since impl may modify propValue
            propertyValueHolder = zfargs.propValue();
            if(propertyValueHolder != zfnull) {
                propertyValueHolder->wrappedValueCopy(propertyValue);
            }
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
        ownerClassOrNull = property->ownerClass();
    }
    else {
        if(!ownerClassOrNull->classIsTypeOf(property->ownerClass())) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "ownerClassOrNull must be type of %s, got: %s",
                    property->ownerClass()->className(),
                    ownerClassOrNull->className());
            }
            return zffalse;
        }
    }
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycle, property);
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i) {
        if(d[i].ownerClass == ownerClassOrNull) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "property already has a custom life cycle handler: %s",
                    property);
            }
            return zffalse;
        }
    }

    _ZFP_PropLifeCycleData data;
    data.ownerClass = ownerClassOrNull;
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
        ownerClassOrNull = property->ownerClass();
    }
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycle, property);
    for(zfindex i = 0; i < d.count(); ++i) {
        _ZFP_I_PropDynRegLifeCycleData *implUserData = d[i].propertyLifeCycleUserData;
        if(d[i].ownerClass == ownerClassOrNull && implUserData != zfnull) {
            d.remove(i);
            return zftrue;
        }
    }
    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

