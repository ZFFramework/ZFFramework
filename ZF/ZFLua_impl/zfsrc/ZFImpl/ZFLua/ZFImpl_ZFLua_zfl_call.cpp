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
static int _ZFP_ZFImpl_ZFLua_zfl_call_impl(ZF_IN lua_State *L,
                                           ZF_IN ZFObject *obj,
                                           ZF_IN const zfchar *name,
                                           ZF_IN int paramCount,
                                           ZF_IN int luaParamOffset)
{
    zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
        ZFMethodGenericInvokerDefaultParamHolder(),
    };

    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return ZFImpl_ZFLua_luaError(L,
                "[zfl_call] failed to get param%d, got %s, while executing: %s",
                i,
                ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue).cString(),
                name);
        }
    }

    zfautoObject ret;
    if(ZFLogLevelIsActive(ZFLogLevel::e_Debug))
    {
        zfstring errorHint;
        if(ZFDI_invoke(ret, &errorHint, obj, name, (zfindex)paramCount, paramList))
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] %s",
            errorHint.cString());
    }
    else
    {
        if(ZFDI_invoke(ret, zfnull, obj, name, (zfindex)paramCount, paramList))
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
        zfstring errorHint = "[zfl_call] failed to invoke: ";
        errorHint += name;
        errorHint += "(";
        for(zfindex i = 0; i < paramCount; ++i)
        {
            if(i != 0)
            {
                errorHint += ", ";
            }
            ZFObjectInfoT(errorHint, paramList[i]);
        }
        errorHint += "), obj: ";
        ZFObjectInfoT(errorHint, obj);
        return ZFImpl_ZFLua_luaError(L, "%s", errorHint.cString());
    }
}

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/"NS.methodName"/"NS.ClassName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 3;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %zi param",
            (zfindex)count);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] failed to access caller object, expect zfautoObject, got %s, while executing: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
    }
    const zfchar *name = zfnull;
    if(!ZFImpl_ZFLua_toString(name, L, 2) || zfsIsEmpty(name))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_call] unable to access method name, got: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        obj,
        name,
        paramCount,
        luaParamOffset);
}

ZF_NAMESPACE_GLOBAL_END

