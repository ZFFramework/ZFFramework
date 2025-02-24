#include "ZFMethodDynamicRegister.h"
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
static void _ZFP_I_ZFMethodDynamicRegisterGI(ZF_IN_OUT const ZFArgs &zfargs) {
    if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
        return;
    }
    const ZFMethod *ownerMethod = zfargs.ownerMethod();
    for(zfindex i = 0; i < ownerMethod->paramCount(); ++i) {
        if(zfargs.paramAt(i) == ZFMP_DEF()) {
            zfargs.param(i, ownerMethod->paramDefaultValueAt(i));
        }
    }
    ZFListener methodImpl = zfcast(v_ZFListener *, ownerMethod->dynamicRegisterUserData())->zfv;
    methodImpl.execute(zfargs);
}

// ============================================================
const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFMethodDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(param.ownerClass() != zfnull && param.methodNamespace() != zfnull) {
        zfstringAppend(errorHint,
            "ownerClass(%s) and methodNamespace(%s) can not both set",
            param.ownerClass(),
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
    if(methodImplValid && param.dynamicRegisterUserData() != zfnull) {
        zfstringAppend(errorHint,
            "when methodImpl specified, dynamicRegisterUserData must not set");
        return zfnull;
    }
    ZFMethodType methodType = param.methodType();
    ZFMethodAccessType methodAccessType = param.methodAccessType();
    if(param.ownerClass() == zfnull) {
        methodType = ZFMethodTypeStatic;
        methodAccessType = ZFMethodAccessTypePublic;
    }
    if(param.methodName() == zfnull) {
        zfstringAppend(errorHint, "methodName not set");
        return zfnull;
    }
    if(param.returnTypeId() == zfnull) {
        zfstringAppend(errorHint, "returnTypeId not set");
        return zfnull;
    }
    for(zfindex i = 0; i < param.paramCount(); ++i) {
        if(param.paramDefaultValueCallbackAt(i) != zfnull) {
            for( ; i < param.paramCount(); ++i) {
                if(param.paramDefaultValueCallbackAt(i) == zfnull) {
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
        if(param.ownerClass() != zfnull) {
            existMethod = param.ownerClass()->methodForNameIgnoreParent(
                    param.methodName()
                    , param.paramTypeIdAt(0)
                    , param.paramTypeIdAt(1)
                    , param.paramTypeIdAt(2)
                    , param.paramTypeIdAt(3)
                    , param.paramTypeIdAt(4)
                    , param.paramTypeIdAt(5)
                    , param.paramTypeIdAt(6)
                    , param.paramTypeIdAt(7)
                );
        }
        else {
            existMethod = ZFMethodFuncForName(
                    param.methodNamespace()
                    , param.methodName()
                    , param.paramTypeIdAt(0)
                    , param.paramTypeIdAt(1)
                    , param.paramTypeIdAt(2)
                    , param.paramTypeIdAt(3)
                    , param.paramTypeIdAt(4)
                    , param.paramTypeIdAt(5)
                    , param.paramTypeIdAt(6)
                    , param.paramTypeIdAt(7)
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
    for(zfindex i = 0; i < param.paramCount(); ++i) {
        mp.add(
                param.paramTypeIdAt(i)
                , param.paramNameAt(i)
                , param.paramDefaultValueCallbackAt(i)
                );
    }
    ZFMethod *method = _ZFP_ZFMethodRegister(zffalse
            , zftrue
            , methodImplValid
                ? zfobj<v_ZFListener>(param.methodImpl()).toObject()
                : param.dynamicRegisterUserData().toObject()
            , zfnull
            , methodImplValid ? _ZFP_I_ZFMethodDynamicRegisterGI : param.methodGenericInvoker()
            , methodType
            , param.ownerClass()
            , methodAccessType
            , param.methodNamespace()
            , param.methodName()
            , param.returnTypeId()
            , mp
        );
    _ZFP_ZFMethodDynRegData()[method] = zftrue;
    _ZFP_ZFNamespaceRegister(zfnull, ZFNamespaceSkipGlobal(method->methodNamespace()));
    return method;
}

const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFClass *ownerClass
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType /* = ZFMethodTypeVirtual */
        , ZF_IN_OPT ZFMethodAccessType methodAccessType /* = ZFMethodAccessTypePublic */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFMethodDynamicRegisterParam p;
    p.returnTypeId(returnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    p.methodType(methodType);
    p.methodAccessType(methodAccessType);
    for(zfindex i = 0; i < methodParam.paramCount(); ++i) {
        p.methodParam(
            methodParam.paramTypeIdAt(i),
            methodParam.paramNameAt(i),
            methodParam.paramDefaultValueCallbackAt(i));
    }
    p.ownerClass(ownerClass);
    return ZFMethodDynamicRegister(p, errorHint);
}
const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType /* = ZFMethodTypeVirtual */
        , ZF_IN_OPT ZFMethodAccessType methodAccessType /* = ZFMethodAccessTypePublic */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFMethodDynamicRegisterParam p;
    p.returnTypeId(returnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    p.methodType(methodType);
    p.methodAccessType(methodAccessType);
    for(zfindex i = 0; i < methodParam.paramCount(); ++i) {
        p.methodParam(
            methodParam.paramTypeIdAt(i),
            methodParam.paramNameAt(i),
            methodParam.paramDefaultValueCallbackAt(i));
    }
    const ZFClass *cls = ZFClass::classForName(methodNamespace);
    if(cls != zfnull) {
        p.ownerClass(cls);
    }
    else {
        p.methodNamespace(methodNamespace);
    }
    return ZFMethodDynamicRegister(p, errorHint);
}

void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method) {
    if(method != zfnull) {
        ZFCoreAssert(method->isDynamicRegister());
        ZFCoreMutexLocker();
        _ZFP_ZFNamespaceUnregister(ZFNamespaceSkipGlobal(method->methodNamespace()));
        _ZFP_ZFMethodDynRegData().erase(method);
        _ZFP_ZFMethodUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

