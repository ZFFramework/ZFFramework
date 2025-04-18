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

    ZFArgs zfargs;
    zfargs.paramInit();
    for(int i = 0; i < paramCount; ++i) {
        if(!ZFImpl_ZFLua_toGeneric(zfargs.paramAt(i), L, luaParamOffset + i, errorHint)) {
            return zffalse;
        }
    }
    zfargs.ignoreErrorEvent(errorHint != zfnull);
    ZFDI_alloc(zfargs, cls, zftrue);
    if(zfargs.success()) {
        ret = zfargs.result();
        return zftrue;
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
        return zffalse;
    }
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

    zfstring clsName;
    if(!ZFImpl_ZFLua_toStringT(clsName, L, 1) || zfstringIsEmpty(clsName)) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] unable to access class name");
    }
    const ZFClass *cls = ZFClass::classForName(clsName, zfnull);
    if(cls == zfnull) {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] unable to find class: %s", clsName);
    }

    ZFArgs zfargs;
    zfargs.paramInit();
    for(int i = 0; i < paramCount; ++i) {
        if(!ZFImpl_ZFLua_toGeneric(zfargs.paramAt(i), L, luaParamOffset + i)) {
            return ZFImpl_ZFLua_luaError(L,
                "[zfAlloc] invalid param: %s", ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i));
        }
    }

    zfargs
        .ignoreErrorEvent(zftrue)
        ;
    ZFDI_alloc(zfargs, cls, zftrue);
    if(zfargs.success()) {
        ZFImpl_ZFLua_luaPush(L, zfargs.result());
        return 1;
    }

    zfstring errorHint;
    if(zfargs.errorHint()) {
        errorHint = zfargs.errorHint();
    }
    else {
        errorHint += "failed to alloc: ";
        errorHint += cls->classNameFull();
        errorHint += "(";
        ZFDI_paramInfoT(errorHint, zfargs);
        errorHint += ")";
    }
    return ZFImpl_ZFLua_luaError(L, "%s", errorHint);
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfAlloc, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfAlloc", _ZFP_ZFImpl_ZFLua_zfAlloc);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

