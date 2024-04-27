#include "ZFImpl_ZFLua_zfAlloc.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFImpl_ZFLua_zfAlloc(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN const ZFClass *cls
        , ZF_IN int paramCount
        , ZF_IN int luaParamOffset
        , ZF_IN_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(paramCount == 0) {
        ret = cls->newInstance();
        return zftrue;
    }

    zfauto paramList[ZFMETHOD_MAX_PARAM] = {
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
    };
    for(int i = 0; i < paramCount; ++i) {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i, errorHint)) {
            return zffalse;
        }
    }
    return ZFDI_alloc(ret, errorHint, cls, (zfindex)paramCount, paramList, zftrue);
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfAlloc(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    static const int luaParamOffset = 2;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] takes at least one param");
    }
    int paramCount = (count - (luaParamOffset - 1));

    const zfchar *clsName = zfnull;
    if(!ZFImpl_ZFLua_toString(clsName, L, 1) || zfstringIsEmpty(clsName)) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] unable to access class name");
    }
    const ZFClass *cls = ZFDI_classForName(clsName, zfnull);
    if(cls == zfnull) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] unable to find class: %s", clsName);
    }

    zfauto paramList[ZFMETHOD_MAX_PARAM] = {
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
    };
    for(int i = 0; i < paramCount; ++i) {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i)) {
            return ZFImpl_ZFLua_luaError(L,
                "[zfAlloc] invalid param: %s", ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i));
        }
    }

    zfauto ret;
    ZFDI_alloc(ret, zfnull, cls, (zfindex)paramCount, paramList, zftrue);
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfAlloc, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfAlloc", _ZFP_ZFImpl_ZFLua_zfAlloc);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

