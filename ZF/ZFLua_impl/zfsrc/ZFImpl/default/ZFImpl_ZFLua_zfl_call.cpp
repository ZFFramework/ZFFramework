#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/"NS.methodName"/"NS.ClassName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L);

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_call, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_call", _ZFP_ZFImpl_ZFLua_zfl_call);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/"NS.methodName"/"NS.ClassName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    static const int luaParamOffset = 3;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %s param",
            (zfindex)count);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfauto obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1)) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] failed to access caller object, expect zfauto, got %s, while executing: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue),
            ZFImpl_ZFLua_luaObjectInfo(L, 2));
    }
    zfstring name;
    if(!ZFImpl_ZFLua_toStringT(name, L, 2) || zfstringIsEmpty(name)) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] unable to access method name, got: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 2));
    }

    ZFArgs zfargs;
    zfargs.paramInit();
    if(ZFLogLevelIsActive(v_ZFLogLevel::e_Debug)) {
        zfstring errorHint;
        for(int i = 0; i < paramCount; ++i) {
            errorHint.removeAll();
            if(!ZFImpl_ZFLua_toGeneric(zfargs.paramAt(i), L, luaParamOffset + i, &errorHint)) {
                return ZFImpl_ZFLua_luaError(L,
                    "[zfl_call] failed to get param%s, got %s, error: %s, while executing: %s",
                    i,
                    ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue),
                    errorHint,
                    name);
            }
        }
    }
    else {
        for(int i = 0; i < paramCount; ++i) {
            if(!ZFImpl_ZFLua_toGeneric(zfargs.paramAt(i), L, luaParamOffset + i)) {
                return ZFImpl_ZFLua_luaError(L,
                    "[zfl_call] failed to get param%s, got %s, while executing: %s",
                    i,
                    ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue),
                    name);
            }
        }
    }

    zfargs
        .sender(obj)
        .ignoreErrorEvent(zftrue)
        ;
    ZFDI_invoke(zfargs, name, zftrue);
    if(zfargs.success()) {
        if(zfargs.result()) {
            ZFImpl_ZFLua_luaPush(L, zfargs.result());
        }
        else {
            lua_pushnil(L);
        }
        return 1;
    }

    zfstring errorHint;
    if(zfargs.errorHint()) {
        errorHint = zfargs.errorHint();
    }
    else {
        errorHint += "failed to invoke: ";
        errorHint += name;
        errorHint += "(";
        ZFDI_paramInfoT(errorHint, zfargs);
        errorHint += "), obj: ";
        ZFObjectInfoT(errorHint, obj);
    }
    return ZFImpl_ZFLua_luaError(L, "%s", errorHint);
}

ZF_NAMESPACE_GLOBAL_END

