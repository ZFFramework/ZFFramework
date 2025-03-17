#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfl_trace(ZF_IN lua_State *L) {
    zfobj<v_zfstring> ret;
    ZFImpl_ZFLua_stacktraceT(L, ret->zfv);
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_trace, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_trace", _ZFP_ZFImpl_ZFLua_zfl_trace);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

