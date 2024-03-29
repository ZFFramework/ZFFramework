#include "ZFMethodFuncUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFMethodFuncUserUnregister(ZF_IN const ZFMethod *method) {
    if(method == zfnull) {
        return;
    }
    zfCoreMutexLocker();
    zfCoreAssertWithMessageTrim(method->methodIsUserRegister(),
            "[ZFMethodFuncUserUnregister] method %s is not user registered",
            method
        );
    zfCoreAssertWithMessageTrim(method->methodIsFunctionType(),
            "[ZFMethodFuncUserUnregister] method %s is not function type",
            method
        );

    _ZFP_ZFMethodUnregister(method);
}

ZF_NAMESPACE_GLOBAL_END

