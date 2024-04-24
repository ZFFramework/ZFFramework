#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static int _ZFP_ZFImpl_ZFLua_zfstr(ZF_IN lua_State *L) {
    zfobj<v_zfstring> s;
    ZFImpl_ZFLua_zfstringAppend(L, s->zfv);
    ZFImpl_ZFLua_luaPush(L, s);
    return 1;
}

static int _ZFP_ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L) {
    zfauto obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1)) {
        ZFImpl_ZFLua_luaPush(L, obj);
        return 1;
    }
    v_zfstring *s = obj;
    if(s == zfnull) {
        ZFImpl_ZFLua_luaPush(L, zfobj<v_zfstring>());
        return 1;
    }

    ZFImpl_ZFLua_zfstringAppend(L, s->zfv);
    ZFImpl_ZFLua_luaPush(L, s);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfstr, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfstr", _ZFP_ZFImpl_ZFLua_zfstr);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfstringAppend", _ZFP_ZFImpl_ZFLua_zfstringAppend);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

