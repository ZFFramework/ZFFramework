#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfl_value(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    int count = (int)lua_gettop(L);
    if(count != 1) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_value] takes only one param, got %s",
            (zfindex)count);
    }

    if(!lua_isuserdata(L, 1)) {
        lua_pushvalue(L, 1);
        return 1;
    }

    return ZFImpl_ZFLua_toLuaValue(L, ZFImpl_ZFLua_luaGet(L, 1));
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_value, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_value", _ZFP_ZFImpl_ZFLua_zfl_value);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

