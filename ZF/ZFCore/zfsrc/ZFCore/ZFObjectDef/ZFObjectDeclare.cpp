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
    ZFMethodGenericInvoker methodGI = valueHolder->valueT<ZFMethodGenericInvoker>();
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
    if(param.methodImpl() == zfnull && param.methodGenericInvoker() == zfnull) {
        zfstringAppend(errorHint, "dynamic objectOnInit with no impl");
        return zfnull;
    }

    ZFMethodDynamicRegisterParam paramTmp = param;
    paramTmp.ownerClass(cls);
    paramTmp.methodName("objectOnInit");
    paramTmp.returnTypeId(ZFTypeId_void());
    paramTmp.methodType(ZFMethodTypeVirtual);
    paramTmp.methodAccessType(ZFMethodAccessTypeProtected);

    if(param.methodGenericInvoker() == zfnull) {
        paramTmp.methodImpl(param.methodImpl());
        return ZFMethodDynamicRegister(paramTmp, errorHint);
    }

    ZFMethodGenericInvoker methodGISaved = paramTmp.methodGenericInvoker();
    paramTmp.methodGenericInvoker(_ZFP_ZFObjectOnInitGI);

    zfauto userData = param.dynamicRegisterUserData();
    if(userData == zfnull) {
        userData = zfobj<ZFObject>();
        paramTmp.dynamicRegisterUserData(userData);
    }
    userData->objectTag("_ZFP_ZFObjectOnInitGI", zfobj<ZFValueHolder>(
                (void *)methodGISaved
                , ZFValueHolderTypePointerRef()
                ));

    return ZFMethodDynamicRegister(paramTmp, errorHint);
}

// ============================================================
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

