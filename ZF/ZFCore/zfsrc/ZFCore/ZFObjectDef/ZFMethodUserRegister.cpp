#include "ZFMethodUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFMethodUserUnregister(ZF_IN const ZFMethod *method) {
    if(method == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    ZFCoreAssertWithMessageTrim(method->methodIsUserRegister(),
            "[ZFMethodUserUnregister] method %s is not user registered",
            method
        );
    ZFCoreAssertWithMessageTrim(!method->methodIsFunctionType(),
            "[ZFMethodUserUnregister] method %s is not class member type",
            method
        );

    _ZFP_ZFMethodUnregister(method);
}

ZF_NAMESPACE_GLOBAL_END

