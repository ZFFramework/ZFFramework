#include "ZFObjectDeclare.h"
#include "ZFObjectImpl.h"
#include "ZFMethodDynamicRegisterExtra.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFMethod *ZFObjectOnInitDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFLISTENER_1(methodImplWrapper
            , ZFListener, methodImpl
            ) {
        // call `this->objectOnInit();`
        ZFMethodInvokeData *ivk = zfargs.param0();
        ivk->invokerObject->_ZFP_ZFObject_objectOnInit();
        methodImpl.execute(zfargs);
    } ZFLISTENER_END()
    return ZFMethodDynamicRegister(
            cls
            , ZFTypeId_void()
            , "objectOnInit"
            , methodParam
            , methodImplWrapper
            , errorHint
            );
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(const ZFMethod *, ZFObjectOnInitDynamicRegister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

