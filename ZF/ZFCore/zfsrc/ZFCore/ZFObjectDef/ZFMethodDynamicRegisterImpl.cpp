#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"
#include "ZFMethodFuncDeclare.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlhashmap<const ZFMethod *, zfbool, zfpointer_zfstlHash<const ZFMethod *>, zfpointer_zfstlEqual<const ZFMethod *> > _ZFP_ZFMethodDynRegMapType;

static _ZFP_ZFMethodDynRegMapType &_ZFP_ZFMethodDynRegData(void) {
    static _ZFP_ZFMethodDynRegMapType d;
    return d;
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodDynamicRegisterAutoRemove) {
    _ZFP_ZFMethodDynRegMapType t;
    t.swap(_ZFP_ZFMethodDynRegData());
    for(_ZFP_ZFMethodDynRegMapType::iterator it = t.begin(); it != t.end(); ++it) {
        _ZFP_ZFMethodUnregister(it->first);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_I_ZFMethodDynamicRegisterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheckWithMethod(errorHint, paramCount, paramList, invokerMethod)) {
        return zffalse;
    }
    zfobj<ZFMethodInvokeData> d;
    d->invokerMethod = invokerMethod;
    d->invokerObject = invokerObject;
    d->param0.zfunsafe_assign(paramList[0]);
    d->param1.zfunsafe_assign(paramList[1]);
    d->param2.zfunsafe_assign(paramList[2]);
    d->param3.zfunsafe_assign(paramList[3]);
    d->param4.zfunsafe_assign(paramList[4]);
    d->param5.zfunsafe_assign(paramList[5]);
    d->param6.zfunsafe_assign(paramList[6]);
    d->param7.zfunsafe_assign(paramList[7]);

    ZFListener methodImpl = zfcast(v_ZFListener *, invokerMethod->methodDynamicRegisterUserData())->zfv;
    methodImpl.execute(ZFArgs()
            .sender(invokerObject)
            .param0(d)
        );
    ret = d->ret;
    if(errorHint != zfnull) {
        *errorHint += d->errorHint;
    }
    if(d->invokeSuccess) {
        paramList[0].zfunsafe_assign(d->param0);
        paramList[1].zfunsafe_assign(d->param1);
        paramList[2].zfunsafe_assign(d->param2);
        paramList[3].zfunsafe_assign(d->param3);
        paramList[4].zfunsafe_assign(d->param4);
        paramList[5].zfunsafe_assign(d->param5);
        paramList[6].zfunsafe_assign(d->param6);
        paramList[7].zfunsafe_assign(d->param7);
    }
    return d->invokeSuccess;
}

// ============================================================
const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFMethodDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.methodOwnerClass() != zfnull && param.methodNamespace() != zfnull) {
        zfstringAppend(errorHint,
            "methodOwnerClass(%s) and methodNamespace(%s) can not both set",
            param.methodOwnerClass(),
            param.methodNamespace());
        return zfnull;
    }
    zfbool methodImplValid = param.methodImpl();
    if(param.methodGenericInvoker() == zfnull && !methodImplValid) {
        zfstringAppend(errorHint, "methodGenericInvoker / methodImpl not set");
        return zfnull;
    }
    if(param.methodGenericInvoker() != zfnull && methodImplValid) {
        zfstringAppend(errorHint,
            "methodGenericInvoker(%s) and methodImpl(%s) can not both set",
            param.methodGenericInvoker(),
            param.methodImpl());
        return zfnull;
    }
    if(methodImplValid && param.methodDynamicRegisterUserData() != zfnull) {
        zfstringAppend(errorHint,
            "when methodImpl specified, methodDynamicRegisterUserData must not set");
        return zfnull;
    }
    ZFMethodType methodType = param.methodType();
    ZFMethodPrivilegeType methodPrivilegeType = param.methodPrivilegeType();
    if(param.methodOwnerClass() == zfnull) {
        methodType = ZFMethodTypeStatic;
        methodPrivilegeType = ZFMethodPrivilegeTypePublic;
    }
    if(param.methodName() == zfnull) {
        zfstringAppend(errorHint, "methodName not set");
        return zfnull;
    }
    if(param.methodReturnTypeId() == zfnull) {
        zfstringAppend(errorHint, "methodReturnTypeId not set");
        return zfnull;
    }
    for(zfindex i = 0; i < param.methodParamCount(); ++i) {
        if(param.methodParamDefaultValueCallbackAt(i) != zfnull) {
            for( ; i < param.methodParamCount(); ++i) {
                if(param.methodParamDefaultValueCallbackAt(i) == zfnull) {
                    zfstringAppend(errorHint,
                        "param %s has no default value but previous param has",
                        i);
                }
            }
            break;
        }
    }

    if(!param.zfunsafe_disableChecker()) {
        const ZFMethod *existMethod = zfnull;
        if(param.methodOwnerClass() != zfnull) {
            existMethod = param.methodOwnerClass()->methodForNameIgnoreParent(
                    param.methodName()
                    , param.methodParamTypeIdAt(0)
                    , param.methodParamTypeIdAt(1)
                    , param.methodParamTypeIdAt(2)
                    , param.methodParamTypeIdAt(3)
                    , param.methodParamTypeIdAt(4)
                    , param.methodParamTypeIdAt(5)
                    , param.methodParamTypeIdAt(6)
                    , param.methodParamTypeIdAt(7)
                );
        }
        else {
            existMethod = ZFMethodFuncForName(
                    param.methodNamespace()
                    , param.methodName()
                    , param.methodParamTypeIdAt(0)
                    , param.methodParamTypeIdAt(1)
                    , param.methodParamTypeIdAt(2)
                    , param.methodParamTypeIdAt(3)
                    , param.methodParamTypeIdAt(4)
                    , param.methodParamTypeIdAt(5)
                    , param.methodParamTypeIdAt(6)
                    , param.methodParamTypeIdAt(7)
                );
        }
        if(existMethod != zfnull) {
            zfstringAppend(errorHint,
                "method with same sig already exists: %s",
                existMethod);
            return zfnull;
        }
    }

    ZFCoreMutexLocker();
    _ZFP_ZFMethodMP mp;
    for(zfindex i = 0; i < param.methodParamCount(); ++i) {
        mp.add(
                param.methodParamTypeIdAt(i)
                , param.methodParamNameAt(i)
                , param.methodParamDefaultValueCallbackAt(i)
                );
    }
    ZFMethod *method = _ZFP_ZFMethodRegister(zffalse
            , zftrue
            , methodImplValid
                ? zfobj<v_ZFListener>(param.methodImpl()).toObject()
                : param.methodDynamicRegisterUserData().toObject()
            , zfnull
            , methodImplValid ? _ZFP_I_ZFMethodDynamicRegisterGI : param.methodGenericInvoker()
            , methodType
            , param.methodOwnerClass()
            , methodPrivilegeType
            , param.methodNamespace()
            , param.methodName()
            , param.methodReturnTypeId()
            , mp
        );
    _ZFP_ZFMethodDynRegData()[method] = zftrue;
    _ZFP_ZFNamespaceRegister(zfnull, ZFNamespaceSkipGlobal(method->methodNamespace()));
    return method;
}

const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN const zfstring &methodReturnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType /* = ZFMethodTypeVirtual */
        , ZF_IN_OPT ZFMethodPrivilegeType methodPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFMethodDynamicRegisterParam p;
    p.methodReturnTypeId(methodReturnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    p.methodType(methodType);
    p.methodPrivilegeType(methodPrivilegeType);
    for(zfindex i = 0; i < methodParam.methodParamCount(); ++i) {
        p.methodParamAdd(
            methodParam.methodParamTypeIdAt(i),
            methodParam.methodParamNameAt(i),
            methodParam.methodParamDefaultValueCallbackAt(i));
    }
    p.methodOwnerClass(methodOwnerClass);
    return ZFMethodDynamicRegister(p, errorHint);
}
const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodReturnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType /* = ZFMethodTypeVirtual */
        , ZF_IN_OPT ZFMethodPrivilegeType methodPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFMethodDynamicRegisterParam p;
    p.methodReturnTypeId(methodReturnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    p.methodType(methodType);
    p.methodPrivilegeType(methodPrivilegeType);
    for(zfindex i = 0; i < methodParam.methodParamCount(); ++i) {
        p.methodParamAdd(
            methodParam.methodParamTypeIdAt(i),
            methodParam.methodParamNameAt(i),
            methodParam.methodParamDefaultValueCallbackAt(i));
    }
    const ZFClass *cls = ZFClass::classForName(methodNamespace);
    if(cls != zfnull) {
        p.methodOwnerClass(cls);
    }
    else {
        p.methodNamespace(methodNamespace);
    }
    return ZFMethodDynamicRegister(p, errorHint);
}

void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method) {
    if(method != zfnull) {
        ZFCoreAssert(method->methodIsDynamicRegister());
        ZFCoreMutexLocker();
        _ZFP_ZFNamespaceUnregister(ZFNamespaceSkipGlobal(method->methodNamespace()));
        _ZFP_ZFMethodDynRegData().erase(method);
        _ZFP_ZFMethodUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

