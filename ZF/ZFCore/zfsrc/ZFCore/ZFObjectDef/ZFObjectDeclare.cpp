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
            , ZFMethodTypeVirtual
            , ZFMethodPrivilegeTypeProtected
            , errorHint
            );
}

zfclass _ZFP_I_ZFObjectOnInitGIData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFObjectOnInitGIData, ZFObject)
public:
    ZFMethodGenericInvoker methodGI;
    zfauto userData;
};
static zfbool _ZFP_ZFObjectOnInitGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    _ZFP_I_ZFObjectOnInitGIData *userDataWrapper = invokerMethod->methodDynamicRegisterUserData()->toAny();
    zfauto userData = userDataWrapper->userData;
    invokerMethod->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodDynamicRegisterUserData = userData;
    zfbool success = userDataWrapper->methodGI(
            invokerMethod
            , invokerObject
            , errorHint
            , ret
            , paramCount
            , paramList
            );
    invokerMethod->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodDynamicRegisterUserData = userDataWrapper;
    return success;
}
const ZFMethod *ZFObjectOnInitDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFMethodDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.methodImpl().callbackValid()) {
        ZFMethodDynamicRegisterParam paramTmp = param;
        ZFListener methodImpl = param.methodImpl();
        ZFLISTENER_1(methodImplWrapper
                , ZFListener, methodImpl
                ) {
            // call `this->objectOnInit();`
            ZFMethodInvokeData *ivk = zfargs.param0();
            ivk->invokerObject->_ZFP_ZFObject_objectOnInit();
            methodImpl.execute(zfargs);
        } ZFLISTENER_END()
        paramTmp.methodImpl(methodImplWrapper);
        const ZFMethod *method = ZFMethodDynamicRegister(paramTmp, errorHint);
        paramTmp.methodImpl(methodImpl);
        return method;
    }
    else if(param.methodGenericInvoker() == zfnull) {
        return ZFMethodDynamicRegister(param, errorHint);
    }

    ZFMethodDynamicRegisterParam paramTmp = param;
    zfblockedAlloc(_ZFP_I_ZFObjectOnInitGIData, userDataWrapper);
    userDataWrapper->methodGI = param.methodGenericInvoker();
    userDataWrapper->userData = param.methodDynamicRegisterUserData();
    paramTmp.methodGenericInvoker(_ZFP_ZFObjectOnInitGI);
    paramTmp.methodDynamicRegisterUserData(userDataWrapper);
    const ZFMethod *method = ZFMethodDynamicRegister(paramTmp, errorHint);
    paramTmp.methodGenericInvoker(userDataWrapper->methodGI);
    paramTmp.methodDynamicRegisterUserData(userDataWrapper->userData);
    return method;
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
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(const ZFMethod *, ZFObjectOnInitDynamicRegister
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const ZFMethodDynamicRegisterParam &, param)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

