#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(param, luaStackOffset) \
    zfauto _##param; \
    const ZFClass *param##Cls = zfnull; \
    if(!ZFImpl_ZFLua_toNumberT(_##param, L, luaStackOffset, zftrue, &param##Cls)) { \
        return ZFImpl_ZFLua_luaError(L, \
            "[LuaMetatable] unknown param type: %s", \
            ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue)); \
    } \
    v_zfdouble *param = _##param.to<v_zfdouble *>()

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatableStoreResult(
        ZF_IN lua_State *L
        , ZF_IN lua_Number const &n
        , ZF_IN v_zfdouble *param0
        , ZF_IN const ZFClass *paramClass0
        , ZF_IN v_zfdouble *param1
        , ZF_IN const ZFClass *paramClass1
        );

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatable_add(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv + v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_sub(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv - v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_mul(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv * v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_div(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv / v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_mod(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv % (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_unm(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(-(v1->zfv)),
        v1, v1Cls, zfnull, zfnull);
}
static int _ZFP_ZFImpl_ZFLua_metatable_band(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfuint)v1->zfv & (zfuint)v2->zfv),
        v1, v_zfflags::ClassData(), v2, v_zfflags::ClassData());
}
static int _ZFP_ZFImpl_ZFLua_metatable_bor(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    const ZFClass *cls = v_zfflags::ClassData();
    if(v1Cls != zfnull && v1Cls->classIsTypeOf(ZFEnum::ClassData())) {
        cls = v1Cls;
    }
    else if(v2Cls != zfnull && v2Cls->classIsTypeOf(ZFEnum::ClassData())) {
        cls = v2Cls;
    }
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfflags)v1->zfv | (zfflags)v2->zfv),
        v1, cls, v2, cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_bxor(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfflags)v1->zfv ^ (zfflags)v2->zfv),
        v1, v_zfflags::ClassData(), v2, v_zfflags::ClassData());
}
static int _ZFP_ZFImpl_ZFLua_metatable_bnot(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(zfflags)(~((zfflags)v1->zfv)),
        v1, v_zfflags::ClassData(), zfnull, zfnull);
}
static int _ZFP_ZFImpl_ZFLua_metatable_shl(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv << (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_shr(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv >> (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static zfbool _ZFP_ZFImpl_ZFLua_metatable_concat_action(
        ZF_IN_OUT zfstring &v
        , ZF_OUT zfstring &errorHint
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        ) {
    if(ZFImpl_ZFLua_toString(v, L, luaStackOffset, zftrue)) {
        return zftrue;
    }
    if(lua_isuserdata(L, luaStackOffset)) {
        zfauto const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        ZFTypeIdWrapper *wrapper = param;
        if(wrapper != zfnull) {
            if(wrapper->zfvToString(v)) {
                return zftrue;
            }
        }
        ZFObjectInfoT(v, param);
        return zftrue;
    }
    zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
        ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue));
    return zffalse;
}
static int _ZFP_ZFImpl_ZFLua_metatable_concat(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    zfstring v;
    zfstring errorHint;
    if(_ZFP_ZFImpl_ZFLua_metatable_concat_action(v, errorHint, L, 1)
            && _ZFP_ZFImpl_ZFLua_metatable_concat_action(v, errorHint, L, 2)
            ) {
        lua_pushstring(L, v.cString());
        return 1;
    }
    else {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
}
static int _ZFP_ZFImpl_ZFLua_metatable_len(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    const zfchar *v = zfnull;
    if(!ZFImpl_ZFLua_toString(v, L, 1, zftrue)) {
        return ZFImpl_ZFLua_luaError(L,
            "[LuaMetatable] unknown param type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue));
    }
    lua_pushinteger(L, (lua_Integer)zfslen(v));
    return 1;
}
static zfbool _ZFP_ZFImpl_ZFLua_metatable_cmp(
        ZF_OUT ZFCompareResult &ret
        , ZF_OUT zfstring &errorHint
        , ZF_IN lua_State *L
        ) {
    if(zftrue
            && (lua_isuserdata(L, 1) || lua_isnil(L, 1))
            && (lua_isuserdata(L, 2) || lua_isnil(L, 2))
            ) {
        zfauto v1;
        if(!ZFImpl_ZFLua_toObject(v1, L, 1)) {
            zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue));
            return zffalse;
        }
        zfauto v2;
        if(!ZFImpl_ZFLua_toObject(v2, L, 2)) {
            zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue));
            return zffalse;
        }

        if(v1 == zfnull) {
            if(v2 == zfnull) {
                ret = ZFCompareEqual;
            }
            else {
                ZFTypeIdWrapper *t = v2;
                if(t != zfnull && t->zfvIsInit()) {
                    ret = ZFCompareEqual;
                }
                else {
                    ret = ZFCompareUncomparable;
                }
            }
        }
        else {
            if(v2 == zfnull) {
                ZFTypeIdWrapper *t = v1;
                if(t != zfnull && t->zfvIsInit()) {
                    ret = ZFCompareEqual;
                }
                else {
                    ret = ZFCompareUncomparable;
                }
            }
            else {
                if(zffalse
                        || v1->classData()->classIsTypeOf(v2->classData())
                        || v2->classData()->classIsTypeOf(v1->classData())
                        ) {
                    ret = v1->objectCompare(v2);
                }
                else {
                    zfauto t1;
                    zfauto t2;
                    if(ZFImpl_ZFLua_toNumberT(t1, L, 1)
                            && ZFImpl_ZFLua_toNumberT(t2, L, 2)
                            ) {
                        ret = ZFComparerDefault(t1, t2);
                    }
                    else {
                        ret = v1->objectCompare(v2);
                    }
                }
            }
        }
        return zftrue;
    }
    else {
        if(lua_isnumber(L, 1) || lua_isnumber(L, 2)) {
            zfauto v1;
            zfauto v2;
            if(ZFImpl_ZFLua_toNumberT(v1, L, 1)
                    && ZFImpl_ZFLua_toNumberT(v2, L, 2)
                    ) {
                ret = ZFComparerDefault(v1, v2);
                return zftrue;
            }
        }
        if(lua_isstring(L, 1) || lua_isstring(L, 2)) {
            const zfchar *v1 = zfnull;
            const zfchar *v2 = zfnull;
            if(ZFImpl_ZFLua_toString(v1, L, 1, zftrue)
                    && ZFImpl_ZFLua_toString(v2, L, 2, zftrue)
                    ) {
                ret = ZFComparerDefault(v1, v2);
                return zftrue;
            }
        }
    }

    zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s and %s",
        ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue),
        ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue));
    return zffalse;
}
static int _ZFP_ZFImpl_ZFLua_metatable_eq(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L)) {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
    lua_pushboolean(L, (result == ZFCompareEqual));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_lt(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L)) {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
    lua_pushboolean(L, (result == ZFCompareSmaller));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_le(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L)) {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
    lua_pushboolean(L, (result == ZFCompareSmaller || result == ZFCompareEqual));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_tostring(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    zfauto obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1)) {
        return ZFImpl_ZFLua_luaError(L,
            "[__tostring] unknown object type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue));
    }

    zfstring ret;
    ZFObjectInfoT(ret, obj);
    lua_pushstring(L, ret.cString());
    return 1;
}

// ============================================================
void ZFImpl_ZFLua_implSetupMetatable(
        ZF_IN_OUT lua_State *L
        , ZF_IN_OPT int metatableIndex /* = -1 */
        ) {
    ZFImpl_ZFLua_execute(L,
            "function _ZFP_ZFImpl_ZFLua_metatable_index(obj, k)\n"
            "    return function(obj, ...)\n"
            "        return zfl_call(obj, k, ...)\n"
            "    end\n"
            "end\n"
        , zfindexMax(), zfnull, zfnull, zfnull, zfnull, "");

    if(metatableIndex >= 0) {
        metatableIndex += 2;
    }
    else {
        metatableIndex -= 2;
    }

    lua_pushstring(L, "__index");
    lua_getglobal(L, "_ZFP_ZFImpl_ZFLua_metatable_index");
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__add");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_add);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__sub");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_sub);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__mul");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_mul);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__div");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_div);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__mod");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_mod);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__unm");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_unm);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__band");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_band);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__bor");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_bor);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__bxor");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_bxor);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__bnot");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_bnot);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__shl");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_shl);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__shr");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_shr);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__concat");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_concat);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__len");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_len);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__eq");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_eq);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__lt");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_lt);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__le");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_le);
    lua_rawset(L, metatableIndex);

    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, _ZFP_ZFImpl_ZFLua_metatable_tostring);
    lua_rawset(L, metatableIndex);
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatableStoreResult(
        ZF_IN lua_State *L
        , ZF_IN lua_Number const &n
        , ZF_IN v_zfdouble *param0
        , ZF_IN const ZFClass *paramClass0
        , ZF_IN v_zfdouble *param1
        , ZF_IN const ZFClass *paramClass1
        ) {
    if(paramClass0 == zfnull) {
        paramClass0 = paramClass1;
    }
    if(paramClass0 == zfnull) {
        lua_pushnumber(L, n);
        return 1;
    }
    if(paramClass1 == zfnull) {
        paramClass1 = paramClass0;
    }

    if(paramClass0->classIsTypeOf(ZFEnum::ClassData())) {
        zfauto ret = paramClass0->newInstance();
        ret->classData()->propertySetterForName("enumValue")->methodInvoke(ret, zfobj<v_zfuint>((zfuint)n));
        ZFImpl_ZFLua_luaPush(L, ret);
        return zftrue;
    }
    if(paramClass1->classIsTypeOf(ZFEnum::ClassData())) {
        zfauto ret = paramClass1->newInstance();
        ret->classData()->propertySetterForName("enumValue")->methodInvoke(ret, zfobj<v_zfuint>((zfuint)n));
        ZFImpl_ZFLua_luaPush(L, ret);
        return zftrue;
    }

    if(zffalse) {
    }
    else if(paramClass0->classIsTypeOf(v_zfbool::ClassData()) || paramClass1->classIsTypeOf(v_zfbool::ClassData())) {
        zfobj<v_zfbool> ret;
        ret->zfv = (zfmAbs(n) <= zffloatEpsilon);
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfflags::ClassData()) || paramClass1->classIsTypeOf(v_zfflags::ClassData())) {
        zfobj<v_zfflags> ret;
        ret->zfv = (zfflags)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfidentity::ClassData()) || paramClass1->classIsTypeOf(v_zfidentity::ClassData())) {
        zfobj<v_zfidentity> ret;
        ret->zfv = (zfidentity)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfindex::ClassData()) || paramClass1->classIsTypeOf(v_zfindex::ClassData())) {
        zfobj<v_zfindex> ret;
        ret->zfv = (zfindex)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zftimet::ClassData()) || paramClass1->classIsTypeOf(v_zftimet::ClassData())) {
        zfobj<v_zftimet> ret;
        ret->zfv = (zftimet)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfbyte::ClassData()) || paramClass1->classIsTypeOf(v_zfbyte::ClassData())) {
        zfobj<v_zfbyte> ret;
        ret->zfv = (zfbyte)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }

    lua_pushnumber(L, n);
    return 1;
}

// ============================================================
// zfl_cmp
static int _ZFP_ZFImpl_ZFLua_zfl_cmp(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);
    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L)) {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
    switch(result) {
        case ZFCompareSmaller:
            lua_pushnumber(L, -1);
            break;
        case ZFCompareEqual:
            lua_pushnumber(L, 0);
            break;
        case ZFCompareGreater:
            lua_pushnumber(L, 1);
            break;
        case ZFCompareUncomparable:
        default:
            lua_pushnumber(L, -2);
            break;
    }
    return 1;
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_cmp, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_cmp", _ZFP_ZFImpl_ZFLua_zfl_cmp);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

// ============================================================
// zfl_eq
static int _ZFP_ZFImpl_ZFLua_zfl_eq(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);
    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L)) {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint);
    }
    lua_pushboolean(L, result == ZFCompareEqual);
    return 1;
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_eq, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_eq", _ZFP_ZFImpl_ZFLua_zfl_eq);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

