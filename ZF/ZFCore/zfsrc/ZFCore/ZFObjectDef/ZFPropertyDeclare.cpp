#include "ZFPropertyDeclare.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFProperty *ZFPropertyForName(
        ZF_IN const zfstring &classNameOrFullName
        , ZF_IN const zfstring &propertyName
        ) {
    const ZFClass *cls = ZFClass::classForName(classNameOrFullName);
    if(cls != zfnull) {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}

// ============================================================
// parent or interface before child
static ZFCoreArray<_ZFP_PropLifeCycleData> &_ZFP_ZFPropertyLifeCycleDataRef(
        ZF_IN const zfchar *lifeCycleName
        , ZF_IN const ZFProperty *property
        ) {
    if(zffalse) {
    }
    else if(zfstringIsEqual(lifeCycleName, "OnInit")) {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
    }
    else if(zfstringIsEqual(lifeCycleName, "OnUpdate")) {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnUpdate;
    }
    else if(zfstringIsEqual(lifeCycleName, "OnAttach")) {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnAttach;
    }
    else if(zfstringIsEqual(lifeCycleName, "OnDetach")) {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDetach;
    }

    ZFCoreCriticalShouldNotGoHere();
    return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
}
void _ZFP_ZFPropertyLifeCycleRegister(
        ZF_IN const zfchar *lifeCycleName
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper
        ) {
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    zfindex index = d.count();
    for(zfindex i = 0; i < d.count(); ++i) {
        if(d[i].ownerClass == ownerClass) {
            d[i].propertyLifeCycleWrapper = propertyLifeCycleWrapper;
            return;
        }
        else if(d[i].ownerClass->classIsTypeOf(ownerClass)) {
            index = i;
            break;
        }
        else if(ownerClass->classIsTypeOf(d[i].ownerClass)) {
            index = i + 1;
        }
    }
    _ZFP_PropLifeCycleData data;
    data.ownerClass = ownerClass;
    data.propertyLifeCycleWrapper = propertyLifeCycleWrapper;
    d.add(data, index);
}
void _ZFP_ZFPropertyLifeCycleUnregister(
        ZF_IN const zfchar *lifeCycleName
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClass
        ) {
    ZFCoreArray<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    for(zfindex i = 0; i < d.count(); ++i) {
        if(d[i].ownerClass == ownerClass) {
            d.remove(i);
            break;
        }
    }
}

static void _ZFP_ZFPropertyLifeCycleCallAction(
        ZF_IN ZFCoreArray<_ZFP_PropLifeCycleData> const &d
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN void *propertyValue
        , ZF_IN const void *propertyValueOld
        , ZF_IN_OPT zfbool subclassFirst = zffalse
        ) {
    if(subclassFirst) {
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

static void _ZFP_ZFPropertyWeakAttach(
        ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *valueNew
        );
static void _ZFP_ZFPropertyWeakDetach(
        ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *valueOld
        );

void _ZFP_ZFPropertyLifeCycleCall_init_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN ZFObject *value
        , ZF_IN zfbool notifyOwnerAttach
        , ZF_IN void (*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN ZFObject *value
            )
        , ZF_IN void *rawValueStoreToken
        ) {
    zfauto valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        property,
        &valueHolder,
        &valueHolder);
    rawValueStoreCallback(rawValueStoreToken, valueHolder);

    if(notifyOwnerAttach) {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            property,
            &valueHolder,
            &valueHolder);

        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_init_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN void *value
        , ZF_IN zfbool notifyOwnerAttach
        , ZF_IN ZFObject *weakProp
        ) {
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        property,
        value,
        value);

    if(notifyOwnerAttach) {
        if(weakProp) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakProp);}
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            property,
            value,
            value);

        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN ZFObject *value
        , ZF_IN zfbool notifyOwnerDetach
        ) {
    zfauto valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        property,
        &valueHolder,
        &valueHolder,
        zftrue);
    if(notifyOwnerDetach) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property);}
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN void *value
        , ZF_IN zfbool notifyOwnerDetach
        , ZF_IN ZFObject *weakProp
        ) {
    if(weakProp) {_ZFP_ZFPropertyWeakDetach(propertyOwnerObject, property, weakProp);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        property,
        value,
        value,
        zftrue);
    if(notifyOwnerDetach) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property);}
}
void _ZFP_ZFPropertyLifeCycleCall_setter_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN ZFObject *propertyValueOld
        , ZF_IN ZFObject *propertyValueNew
        , ZF_IN void (*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN ZFObject *value)
        , ZF_IN void *rawValueStoreToken
        ) {
    zfauto valueOld = propertyValueOld;
    zfauto valueNew = propertyValueNew;
    if(accessed) {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            property,
            &valueOld,
            &valueOld,
            zftrue);
    }

    rawValueStoreCallback(rawValueStoreToken, valueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        property,
        &valueNew,
        &valueOld);
    rawValueStoreCallback(rawValueStoreToken, valueNew);

    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        property,
        &valueNew,
        &valueOld);
    if(!accessed) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);}

    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN void *propertyValueOld
        , ZF_IN const void *propertyValueNew
        , ZF_IN void *(*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN const void *value)
        , ZF_IN void *rawValueStoreToken
        , ZF_IN ZFObject *weakPropOld
        , ZF_IN ZFObject *weakPropNew
        ) {
    if(accessed) {
        if(weakPropOld) {_ZFP_ZFPropertyWeakDetach(propertyOwnerObject, property, weakPropOld);}
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            property,
            propertyValueOld,
            propertyValueOld,
            zftrue);
    }
    void *valueRef = rawValueStoreCallback(rawValueStoreToken, propertyValueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        property,
        valueRef,
        propertyValueOld);

    if(weakPropNew) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakPropNew);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        property,
        valueRef,
        propertyValueOld);
    if(!accessed) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);}
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}

// ============================================================
static void _ZFP_ZFPropertyWeakAttach(
        ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *valueNew
        ) {
    ZFLISTENER_2(propertyOnDealloc
            , ZFObject *, propertyOwnerObject
            , const ZFProperty *, property
            ) {
        property->callbackValueReset(property, propertyOwnerObject);
    } ZFLISTENER_END()
    zfobj<v_ZFListener> holder(propertyOnDealloc);
    propertyOwnerObject->objectTag(zfstr("_ZFP_PropWeak_%s", property->propertyName()), holder);

    valueNew->observerAdd(
        ZFObject::E_ObjectBeforeDealloc(),
        propertyOnDealloc);
}
static void _ZFP_ZFPropertyWeakDetach(
        ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *valueOld
        ) {
    v_ZFListener *holder = propertyOwnerObject->objectTag(zfstr("_ZFP_PropWeak_%s", property->propertyName()));
    valueOld->observerRemove(
        ZFObject::E_ObjectBeforeDealloc(),
        holder->zfv);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyForName
        , ZFMP_IN(const zfstring &, classNameOrFullName)
        , ZFMP_IN(const zfstring &, propertyName)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

