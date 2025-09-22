#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
        ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper
        , ZF_IN const ZFMethod *method
        ) {
    printf("zfzfzf reg: %s\n", method->objectInfo().cString());
    if(method->methodNamespace()) {
        helper.addGenericScope(method->methodNamespace());
    }
    else {
        helper.addCustomCode(zfstr(
                    "function %s(...) return zfl_call(zfnull, '%s', ...) end\n"
                    , method->methodName()
                    , method->methodName()
                    ));
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFMethod, ZFM_EXPAND({
        ZFCoreArray<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
        for(zfindex i = 0; i < allMethod.count(); ++i) {
            const ZFMethod *method = allMethod[i];
            if(method->isFuncType()
                    && !method->isInternalPrivate()
                    ) {
                _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(helper, method);
            }
        }
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedMethod != zfnull
                && data.changeType == ZFClassDataUpdateTypeAttach
                && data.changedMethod->isFuncType()
                && !data.changedMethod->isInternalPrivate()
                ) {
            _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(helper, data.changedMethod);
        }
    }))

ZF_NAMESPACE_GLOBAL_END

