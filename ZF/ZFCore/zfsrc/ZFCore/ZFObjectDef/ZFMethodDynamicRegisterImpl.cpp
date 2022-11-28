#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"
#include "ZFMethodFuncDeclare.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic)
{
}
zfstlmap<const ZFMethod *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodDynamicRegisterAutoRemove)
{
    zfstlmap<const ZFMethod *, zfbool> t;
    t.swap(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m);
    for(zfstlmap<const ZFMethod *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
    {
        _ZFP_ZFMethodUnregister(it->first);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_I_ZFMethodDynamicRegisterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    zfblockedAlloc(ZFMethodInvokeData, d);
    d->invokerMethod = invokerMethod;
    d->invokerObject = invokerObject;
    d->param0 = paramList[0];
    d->param1 = paramList[1];
    d->param2 = paramList[2];
    d->param3 = paramList[3];
    d->param4 = paramList[4];
    d->param5 = paramList[5];
    d->param6 = paramList[6];
    d->param7 = paramList[7];

    ZFListener methodImpl = ZFCastZFObjectUnchecked(v_ZFListener *, invokerMethod->methodDynamicRegisterUserData())->zfv;
    methodImpl.execute(ZFArgs()
            .sender(invokerObject)
            .param0(d)
        );
    ret = d->ret;
    if(errorHint != zfnull)
    {
        *errorHint += d->errorHint;
    }
    if(d->invokeSuccess)
    {
        paramList[0] = d->param0;
        paramList[1] = d->param1;
        paramList[2] = d->param2;
        paramList[3] = d->param3;
        paramList[4] = d->param4;
        paramList[5] = d->param5;
        paramList[6] = d->param6;
        paramList[7] = d->param7;
    }
    return d->invokeSuccess;
}

// ============================================================
const ZFMethod *ZFMethodDynamicRegister(ZF_IN const ZFMethodDynamicRegisterParam &param,
                                        ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(param.methodOwnerClass() != zfnull && param.methodNamespace() != zfnull)
    {
        zfstringAppend(errorHint,
            "methodOwnerClass(%s) and methodNamespace(%s) can not both set",
            param.methodOwnerClass()->objectInfo().cString(),
            param.methodNamespace());
        return zfnull;
    }
    zfbool methodImplValid = param.methodImpl();
    if(param.methodGenericInvoker() == zfnull && !methodImplValid)
    {
        zfstringAppend(errorHint, "methodGenericInvoker / methodImpl not set");
        return zfnull;
    }
    if(param.methodGenericInvoker() != zfnull && methodImplValid)
    {
        zfstringAppend(errorHint,
            "methodGenericInvoker(%p) and methodImpl(%s) can not both set",
            param.methodGenericInvoker(),
            param.methodImpl().objectInfo().cString());
        return zfnull;
    }
    if(methodImplValid && param.methodDynamicRegisterUserData() != zfnull)
    {
        zfstringAppend(errorHint,
            "when methodImpl specified, methodDynamicRegisterUserData must not set");
        return zfnull;
    }
    ZFMethodType methodType = param.methodType();
    ZFMethodPrivilegeType methodPrivilegeType = param.methodPrivilegeType();
    if(param.methodOwnerClass() == zfnull)
    {
        methodType = ZFMethodTypeStatic;
        methodPrivilegeType = ZFMethodPrivilegeTypePublic;
    }
    if(param.methodName() == zfnull)
    {
        zfstringAppend(errorHint, "methodName not set");
        return zfnull;
    }
    if(param.methodReturnTypeId() == zfnull)
    {
        zfstringAppend(errorHint, "methodReturnTypeId not set");
        return zfnull;
    }
    for(zfindex i = 0; i < param.methodParamCount(); ++i)
    {
        if(param.methodParamDefaultValueCallbackAt(i) != zfnull)
        {
            for( ; i < param.methodParamCount(); ++i)
            {
                if(param.methodParamDefaultValueCallbackAt(i) == zfnull)
                {
                    zfstringAppend(errorHint,
                        "param %zi has no default value but previous param has",
                        i);
                }
            }
            break;
        }
    }
    const ZFMethod *existMethod = zfnull;
    if(param.methodOwnerClass() != zfnull)
    {
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
    else
    {
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
    if(existMethod != zfnull)
    {
        zfstringAppend(errorHint,
            "method with same sig already exists: %s",
            existMethod->objectInfo().cString());
        return zfnull;
    }

    zfCoreMutexLocker();
    ZFMethod *method = _ZFP_ZFMethodRegister(zffalse
            , zftrue
            , methodImplValid
                ? zflineAlloc(v_ZFListener, param.methodImpl())
                : param.methodDynamicRegisterUserData()
            , zfnull
            , methodImplValid ? _ZFP_I_ZFMethodDynamicRegisterGI : param.methodGenericInvoker()
            , methodType
            , param.methodOwnerClass()
            , methodPrivilegeType
            , param.methodNamespace()
            , param.methodName()
            , param.methodReturnTypeId()
            , param.methodReturnTypeName()
            , param.methodParamTypeIdAt(0), param.methodParamTypeNameAt(0), param.methodParamNameAt(0), param.methodParamDefaultValueCallbackAt(0)
            , param.methodParamTypeIdAt(1), param.methodParamTypeNameAt(1), param.methodParamNameAt(1), param.methodParamDefaultValueCallbackAt(1)
            , param.methodParamTypeIdAt(2), param.methodParamTypeNameAt(2), param.methodParamNameAt(2), param.methodParamDefaultValueCallbackAt(2)
            , param.methodParamTypeIdAt(3), param.methodParamTypeNameAt(3), param.methodParamNameAt(3), param.methodParamDefaultValueCallbackAt(3)
            , param.methodParamTypeIdAt(4), param.methodParamTypeNameAt(4), param.methodParamNameAt(4), param.methodParamDefaultValueCallbackAt(4)
            , param.methodParamTypeIdAt(5), param.methodParamTypeNameAt(5), param.methodParamNameAt(5), param.methodParamDefaultValueCallbackAt(5)
            , param.methodParamTypeIdAt(6), param.methodParamTypeNameAt(6), param.methodParamNameAt(6), param.methodParamDefaultValueCallbackAt(6)
            , param.methodParamTypeIdAt(7), param.methodParamTypeNameAt(7), param.methodParamNameAt(7), param.methodParamDefaultValueCallbackAt(7)
            , (const zfchar *)zfnull
        );
    method->_ZFP_ZFMethod_paramDefaultValueList.capacity(param.methodParamCount());
    for(zfindex i = 0; i < param.methodParamCount(); ++i)
    {
        method->_ZFP_ZFMethod_paramDefaultValueList.add(param.methodParamDefaultValueAt(i));
    }
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m[method] = zftrue;
    return method;
}
void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method)
{
    if(method != zfnull)
    {
        zfCoreAssert(method->methodIsDynamicRegister());
        zfCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m.erase(method);
        _ZFP_ZFMethodUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

