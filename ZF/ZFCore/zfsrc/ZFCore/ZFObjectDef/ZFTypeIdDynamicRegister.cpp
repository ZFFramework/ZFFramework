#include "ZFTypeIdDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic) {
}
zfstlmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *> > m;
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTypeIdDynamicRegisterAutoRemove) {
    zfstlmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *> > &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder)->m;
    if(!m.empty()) {
        zfstlmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *> > t;
        t.swap(m);
        for(zfstlmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *> >::iterator it = t.begin(); it != t.end(); ++it) {
            _ZFP_ZFTypeInfoUnregister(it->first);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_ZFTypeIdGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 0
                )) {
        return zffalse;
    }
    ret = zfobj<v_zfstring>(invokerMethod->methodName() + zfslen("ZFTypeId_"));
    return zftrue;
}
zfbool ZFTypeIdDynamicRegister(
        ZF_IN const zfchar *typeIdName
        , ZF_IN const ZFCorePointerForObject<ZFTypeInfo *> &typeIdData
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(zfstringIsEmpty(typeIdName)) {
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
    if(!ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodGenericInvoker(_ZFP_ZFTypeIdGI)
                .methodReturnTypeId(ZFTypeId_zfstring())
                .methodName(zfstr("ZFTypeId_%s", typeIdName))
                , errorHint)
                ) {
        return zffalse;
    }
    d->m[typeIdName] = typeIdData;
    _ZFP_ZFTypeInfoRegister(typeIdName, typeIdData);
    return zftrue;
}
void ZFTypeIdDynamicUnregister(ZF_IN const zfchar *typeIdName) {
    if(!zfstringIsEmpty(typeIdName)) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamicRegisterDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder);
        zfstlmap<zfstring, ZFCorePointerForObject<ZFTypeInfo *> >::iterator it = d->m.find(typeIdName);
        if(it != d->m.end()) {
            ZFMethodDynamicUnregister(ZFMethodFuncForName(ZF_NAMESPACE_GLOBAL_NAME,
                zfstr("ZFTypeId_%s", typeIdName)));
            _ZFP_ZFTypeInfoUnregister(typeIdName);
            d->m.erase(it);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

