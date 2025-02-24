#include "ZFMethodFuncUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFMethodFuncUserUnregister(ZF_IN const ZFMethod *method) {
    if(method == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    ZFCoreAssertWithMessageTrim(method->isUserRegister(),
            "[ZFMethodFuncUserUnregister] method %s is not user registered",
            method
        );
    ZFCoreAssertWithMessageTrim(method->isFuncType(),
            "[ZFMethodFuncUserUnregister] method %s is not function type",
            method
        );

    _ZFP_ZFMethodUnregister(method);
}

ZF_NAMESPACE_GLOBAL_END

