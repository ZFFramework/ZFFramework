#include "ZFTypeIdDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlhashmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *>, zfstring_zfstlHash, zfstring_zfstlEqual> _ZFP_ZFTypeIdDynamicMapType;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic) {
}
_ZFP_ZFTypeIdDynamicMapType m;
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTypeIdDynamicRegisterAutoRemove) {
    _ZFP_ZFTypeIdDynamicMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder)->m;
    if(!m.empty()) {
        _ZFP_ZFTypeIdDynamicMapType t;
        t.swap(m);
        for(_ZFP_ZFTypeIdDynamicMapType::iterator it = t.begin(); it != t.end(); ++it) {
            _ZFP_ZFTypeInfoUnregister(it->first);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterAutoRemove)

// ============================================================
static void _ZFP_ZFTypeIdGI(ZF_IN_OUT const ZFArgs &zfargs) {
    if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
        return;
    }
    zfargs.result(zfobj<v_zfstring>(zfargs.ownerMethod()->methodName() + zfslen("ZFTypeId_")));
}
zfbool ZFTypeIdDynamicRegister(
        ZF_IN const zfstring &typeIdName
        , ZF_IN const ZFCorePointerForObject<ZFTypeInfo *> &typeIdData
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!typeIdName) {
        zfstringAppend(errorHint, "empty typeIdName");
        return zffalse;
    }
    if(typeIdData == zfnull) {
        zfstringAppend(errorHint, "null typeIdData");
        return zffalse;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamicRegisterDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder);
    if(d->m.find(typeIdName) != d->m.end()) {
        zfstringAppend(errorHint, "type id %s already registered", typeIdName);
        return zffalse;
    }

    zfstring methodName;
    zfstring methodNamespace;
    const ZFClass *ownerClass = zfnull;
    zfindex dotPos = zfstringFindReversely(typeIdName, '.');
    if(dotPos != zfindexMax()) {
        methodName.assign(typeIdName + dotPos + 1);
        zfstring tmp(typeIdName, dotPos);
        ownerClass = ZFClass::classForName(tmp);
        if(ownerClass == zfnull) {
            methodNamespace = tmp;
        }
    }
    else {
        methodName = typeIdName;
    }
    if(!ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodNamespace(methodNamespace)
                .ownerClass(ownerClass)
                .methodType(ZFMethodTypeStatic)
                .methodGenericInvoker(_ZFP_ZFTypeIdGI)
                .returnTypeId(ZFTypeId_zfstring())
                .methodName(zfstr("ZFTypeId_%s", methodName))
                , errorHint)
                ) {
        return zffalse;
    }

    d->m[typeIdName] = typeIdData;
    _ZFP_ZFTypeInfoRegister(typeIdName, typeIdData);
    return zftrue;
}
void ZFTypeIdDynamicUnregister(ZF_IN const zfstring &typeIdName) {
    if(typeIdName) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamicRegisterDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder);
        _ZFP_ZFTypeIdDynamicMapType::iterator it = d->m.find(typeIdName);
        if(it != d->m.end()) {
            zfindex dotPos = zfstringFindReversely(typeIdName, '.');
            if(dotPos == zfindexMax()) {
                ZFMethodDynamicUnregister(ZFMethodFuncForName(
                            zfnull
                            , zfstr("ZFTypeId_%s", typeIdName)
                            ));
            }
            else {
                ZFMethodDynamicUnregister(ZFMethodFuncForName(
                            zfstring(typeIdName, dotPos)
                            , zfstr("ZFTypeId_%s", zfstring(typeIdName + dotPos + 1))
                            ));
            }

            _ZFP_ZFTypeInfoUnregister(typeIdName);
            d->m.erase(it);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

