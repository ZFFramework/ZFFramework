#include "ZFPropertyUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFPropertyMethodCleanup_UserReg(ZF_IN const ZFMethod *method) {
    ZFMethodUserUnregister(method);
}

zfauto _ZFP_I_PropURDIVH::create(
        ZF_IN void *v
        , ZF_IN DeleteCallback deleteCallback
        , ZF_IN_OPT ZFObject *retainValue /* = zfnull */
        ) {
    zfobj<_ZFP_I_PropURDIVH> holder;
    holder->v = v;
    holder->deleteCallback = deleteCallback;
    holder->retainValueHolder = retainValue;
    return holder;
}

void ZFPropertyUserUnregister(ZF_IN const ZFProperty *zfproperty) {
    if(zfproperty == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    ZFCoreAssertWithMessageTrim(zfproperty->isUserRegister(),
            "[ZFPropertyUserUnregister] property %s is not user registered",
            zfproperty
        );

    _ZFP_ZFPropertyUnregister(zfproperty);
}

void ZFPropertyUserRegisterNotifyUpdate(
        ZF_IN ZFObject *ownerObject
        , ZF_IN const ZFProperty *property
        , ZF_IN void *propertyValueOld
        ) {
    ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}

ZF_NAMESPACE_GLOBAL_END

