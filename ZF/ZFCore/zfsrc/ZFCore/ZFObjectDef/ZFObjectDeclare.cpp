#include "ZFObjectDeclare.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFMethod *ZFObjectOnInitDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(methodParam.paramCount() <= 0) {
        zfstringAppend(errorHint, "dynamic objectOnInit must take at least 1 param");
        return zfnull;
    }

    ZFLISTENER_1(methodImplWrapper
            , ZFListener, methodImpl
            ) {
        // call `this->objectOnInit();`
        zfargs.sender()->_ZFP_ZFObject_objectOnInit();
        methodImpl.execute(zfargs);
    } ZFLISTENER_END()
    return ZFMethodDynamicRegister(
            cls
            , ZFTypeId_void()
            , "objectOnInit"
            , methodParam
            , methodImplWrapper
            , ZFMethodTypeVirtual
            , ZFMethodAccessTypeProtected
            , errorHint
            );
}

static void _ZFP_ZFObjectOnInitGI(ZF_IN_OUT const ZFArgs &zfargs) {
    ZFValueHolder *valueHolder = zfargs.ownerMethod()->dynamicRegisterUserData()->objectTag("_ZFP_ZFObjectOnInitGI");
    ZFCoreAssert(valueHolder != zfnull);
    zfargs.sender()->_ZFP_ZFObject_objectOnInit();
    ZFMethodGenericInvoker methodGI = *valueHolder->holdedDataPointer<ZFMethodGenericInvoker *>();
    methodGI(zfargs);
}
const ZFMethod *ZFObjectOnInitDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFMethodDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.paramCount() <= 0) {
        zfstringAppend(errorHint, "dynamic objectOnInit must take at least 1 param");
        return zfnull;
    }

    ZFMethodDynamicRegisterParam paramTmp = param;
    paramTmp.ownerClass(cls);
    paramTmp.methodName("objectOnInit");
    paramTmp.returnTypeId(ZFTypeId_void());
    paramTmp.methodType(ZFMethodTypeVirtual);
    paramTmp.methodAccessType(ZFMethodAccessTypeProtected);

    if(param.methodImpl()) {
        ZFListener methodImpl = param.methodImpl();
        ZFLISTENER_1(methodImplWrapper
                , ZFListener, methodImpl
                ) {
            // call `this->objectOnInit();`
            zfargs.sender()->_ZFP_ZFObject_objectOnInit();
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

    ZFMethodGenericInvoker methodGISaved = paramTmp.methodGenericInvoker();
    paramTmp.methodGenericInvoker(_ZFP_ZFObjectOnInitGI);

    zfauto userData = param.dynamicRegisterUserData();
    if(userData == zfnull) {
        userData = zfobj<ZFObject>();
        paramTmp.dynamicRegisterUserData(userData);
    }
    ZFMethodGenericInvoker *methodGI = (ZFMethodGenericInvoker *)zfmalloc(sizeof(ZFMethodGenericInvoker));
    *methodGI = methodGISaved;
    userData->objectTag("_ZFP_ZFObjectOnInitGI", zfobj<ZFValueHolder>(
                methodGI
                , ZFValueHolderTypePOD()
                ));

    return ZFMethodDynamicRegister(paramTmp, errorHint);
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

