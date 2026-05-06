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
            ZF_IN_OUT void *rawValueStoreToken
            , ZF_IN ZFObject *value
            )
        , ZF_IN void *rawValueStoreToken
        ) {
    zfauto valueHolder = value;
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnInit, propertyOwnerObject, &valueHolder, &valueHolder);
    rawValueStoreCallback(rawValueStoreToken, valueHolder);

    if(notifyOwnerAttach) {
        property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnAttach, propertyOwnerObject, &valueHolder, &valueHolder);
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
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnInit, propertyOwnerObject, value, value);
    if(notifyOwnerAttach) {
        if(weakProp) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakProp);}
        property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnAttach, propertyOwnerObject, value, value);
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
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnDetach, propertyOwnerObject, &valueHolder, &valueHolder);
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
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnDetach, propertyOwnerObject, value, value);
    if(notifyOwnerDetach) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property);}
}
void _ZFP_ZFPropertyLifeCycleCall_setter_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN ZFObject *propertyValueOld
        , ZF_IN ZFObject *propertyValueNew
        , ZF_IN void (*rawValueStoreCallback)(
            ZF_IN_OUT void *rawValueStoreToken
            , ZF_IN ZFObject *value)
        , ZF_IN void *rawValueStoreToken
        ) {
    zfauto valueOld = propertyValueOld;
    zfauto valueNew = propertyValueNew;
    if(accessed) {
        property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnDetach, propertyOwnerObject, &valueOld, &valueOld);
    }

    rawValueStoreCallback(rawValueStoreToken, valueNew);
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnUpdate, propertyOwnerObject, &valueNew, &valueOld);
    rawValueStoreCallback(rawValueStoreToken, valueNew);

    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnAttach, propertyOwnerObject, &valueNew, &valueOld);
    if(!accessed) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);}

    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &valueOld);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN void *propertyValueOld
        , ZF_IN const void *propertyValueNew
        , ZF_IN void *(*rawValueStoreCallback)(
            ZF_IN_OUT void *rawValueStoreToken
            , ZF_IN const void *value)
        , ZF_IN void *rawValueStoreToken
        , ZF_IN ZFObject *weakPropOld
        , ZF_IN ZFObject *weakPropNew
        ) {
    if(accessed) {
        if(weakPropOld) {_ZFP_ZFPropertyWeakDetach(propertyOwnerObject, property, weakPropOld);}
        property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnDetach, propertyOwnerObject, propertyValueOld, propertyValueOld);
    }
    void *valueRef = rawValueStoreCallback(rawValueStoreToken, propertyValueNew);
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnUpdate, propertyOwnerObject, valueRef, propertyValueOld);

    if(weakPropNew) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakPropNew);}
    property->_ZFP_ZFPropertyLifeCycleInvoke(ZFPropertyLifeCycleOnAttach, propertyOwnerObject, valueRef, propertyValueOld);
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

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyForName
        , ZFMP_IN(const zfstring &, classNameOrFullName)
        , ZFMP_IN(const zfstring &, propertyName)
        )

ZF_NAMESPACE_GLOBAL_END

