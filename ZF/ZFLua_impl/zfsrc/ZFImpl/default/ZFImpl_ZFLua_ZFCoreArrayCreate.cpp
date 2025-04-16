#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static int _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    ZFCoreArray<zfauto> retArr;
    int count = (int)lua_gettop(L);

    for(int i = 0; i < count; ++i) {
        zfauto p;
        if(ZFImpl_ZFLua_toObject(p, L, i + 1)
                || ZFImpl_ZFLua_toCallback(p, L, i + 1)
                ) {
            retArr.add(p);
            continue;
        }

        zfobj<v_zfstring> pTmp;
        if(ZFImpl_ZFLua_toString(pTmp->zfv, L, i + 1, zftrue)) {
            retArr.add(pTmp);
            continue;
        }

        return ZFImpl_ZFLua_luaError(L,
            "[ZFCoreArrayCreate] unknown param type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, i + 1, zftrue));
    }

    ZFImpl_ZFLua_luaPush(L, zfobj<v_ZFCoreArray>(retArr));
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCoreArrayCreate, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCoreArrayCreate", _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

