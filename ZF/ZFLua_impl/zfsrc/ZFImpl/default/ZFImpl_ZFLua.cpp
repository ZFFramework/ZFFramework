#include "ZFImpl_ZFLua.h"

#include "ZFImpl_ZFLua_ZFCallbackForLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

// #define _ZFP_ZFImpl_ZFLua_DEBUG 1

#if _ZFP_ZFImpl_ZFLua_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFImpl_ZFLua_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFLuaImpl] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFImpl_ZFLua_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

void *ZFImpl_ZFLua_luaStateOpen(void) {
    return ZFImpl_ZFLua_luaOpen();
}
void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaClose(L);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_luaStateGlobalHolder, ZFLevelZFFrameworkStatic) {
}
ZFCoreArray<_ZFP_ZFImpl_ZFLua_ImplSetupAttach> setupAttach;
ZFCoreArray<_ZFP_ZFImpl_ZFLua_ImplSetupDetach> setupDetach;
ZFCoreArray<_ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate> setupClassDataUpdate;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_luaStateGlobalHolder)

// ============================================================
typedef zfstlmap<zfstring, zfbool> _ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType;
ZFImpl_ZFLua_ImplSetupHelper::ZFImpl_ZFLua_ImplSetupHelper(ZF_IN lua_State *L)
    : _L(L)
    , _code()
    , _m(zfnew(_ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType))
{
    _code.capacity(1000);
}
ZFImpl_ZFLua_ImplSetupHelper::~ZFImpl_ZFLua_ImplSetupHelper(void) {
    _ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType &m = *(_ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType *)_m;
    for(_ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType::iterator it = m.begin(); it != m.end(); ++it) {
        const zfstring &scope = it->first;
        zfstringAppend(_code,
                "if %s == nil then "
                "%s = {ZFNS='%s'};"
                "debug.setmetatable(%s, _ZFP_zfl_metatable);"
                "end\n"
                , scope
                , scope
                , scope
                , scope
                );
        if(_code.length() >= 10000) {
            _commit();
        }
    }
    if(_code) {
        _commit();
    }
    zfdelete(&m);
}
void ZFImpl_ZFLua_ImplSetupHelper::addGlobalScope(ZF_IN const zfstring &scope) {
    if(scope) {
        (*(_ZFP_ZFImpl_ZFLua_ImplSetupHelper_MapType *)_m)[scope] = zftrue;
    }
}
void ZFImpl_ZFLua_ImplSetupHelper::addGenericScope(ZF_IN const zfstring &genericScope) {
    zfindex dotPos = zfstringFind(genericScope, '.');
    this->addGlobalScope(dotPos != zfindexMax()
            ? zfstring(genericScope, dotPos)
            : genericScope
            );
}
void ZFImpl_ZFLua_ImplSetupHelper::addCustomCode(ZF_IN const zfstring &code) {
    _code += code;
    if(_code.length() >= 10000) {
        _commit();
    }
}
void ZFImpl_ZFLua_ImplSetupHelper::_commit(void) {
    ZFImpl_ZFLua_execute(_L, _code, _code.length());
    _code.zfunsafe_buffer()[0] = '\0';
    _code.zfunsafe_length(0);
}

// ============================================================
void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L) {
    ZFCoreAssert(L != zfnull);
    ZFCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    ZFImpl_ZFLua_luaClassRegister<zfauto>(L, "zfauto");
    { // metatable for zfauto
        luaL_getmetatable(L, "zfauto");
        ZFImpl_ZFLua_implSetupMetatable(L, -1);
        lua_pop(L, 1);
    }

    // zfnull
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_zfnullHolder {
    public:
        static zfauto get_zfnull(void) {
            return zfnull;
        }
    };
    ZFImpl_ZFLua_luaFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zfnull", _ZFP_ZFImpl_ZFLua_zfnullHolder::get_zfnull);
    ZFImpl_ZFLua_execute(L,
            "zfnull = _ZFP_ZFImpl_ZFLua_zfnull()\n"
        );

    // zftrue, zffalse
    ZFImpl_ZFLua_execute(L,
            "zftrue = true\n"
            "zffalse = false\n"
        );

    // zfl_L
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_LHolder {
    public:
        static int f(ZF_IN lua_State *L) {
            zfobj<v_zfptr> ret;
            ret->zfv = (void *)L;
            zfauto tmp = ret;
            ZFImpl_ZFLua_luaPush(L, tmp);
            return 1;
        }
    };
    ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_L", _ZFP_ZFImpl_ZFLua_LHolder::f);

    // zfl_call metatable
    ZFImpl_ZFLua_execute(L,
            "_ZFP_zfl_index = function(tbl, k)"
            "    local t = {ZFNS=tbl.ZFNS .. '.' .. k};"
            "    local d = {};"
            "    d.__index = _ZFP_zfl_index;"
            "    d.__call = _ZFP_zfl_call;"
            "    debug.setmetatable(t, d);"
            "    return t;"
            "end;"
            "_ZFP_zfl_call = function(tbl, ...);"
            "    return zfl_call(zfnull, tbl.ZFNS, ...);"
            "end;"
            "_ZFP_zfl_metatable = {__index = _ZFP_zfl_index, __call = _ZFP_zfl_call};"
        , zfindexMax(), zfnull, zfnull, zfnull, zfnull, "");

    // global NS
    ZFImpl_ZFLua_ImplSetupHelper helper(L);
    helper.addGlobalScope(ZF_NAMESPACE_GLOBAL_NAME);
    helper.addGlobalScope(ZF_NAMESPACE_GLOBAL_ABBR_NAME);

    // each impl setup callback
    for(zfindex i = 0; i < d->setupAttach.count(); ++i) {
        d->setupAttach[i](L, helper);
    }

    if(ZFResIsExist("ZFLua_impl/debugger.lua")) {
        ZFImpl_ZFLua_execute(L,
                "zfl_dbg = zfl_value(zfimport('res:ZFLua_impl/debugger.lua'))"
                );
    }
}
void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L) {
    ZFCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    // each impl setup callback
    for(zfindex i = 0; i < d->setupDetach.count(); ++i) {
        d->setupDetach[i](L);
    }
}

void ZFImpl_ZFLua_classDataUpdate(
        ZF_IN lua_State *L
        , ZF_IN const ZFClassDataUpdateData &data
        ) {
    _ZFP_ZFImpl_ZFLua_invokeTimeLogger("cls change %s", data.objectInfo().cString());
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    ZFImpl_ZFLua_ImplSetupHelper helper(L);
    for(zfindex i = 0; i < d->setupClassDataUpdate.count(); ++i) {
        d->setupClassDataUpdate[i](L, data, helper);
    }
}

// ============================================================
void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(
        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupAttach setupAttachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupDetach setupDetachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate setupClassDataUpdate
        ) {
    ZFCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.add(setupAttachCallback);
    d->setupDetach.add(setupDetachCallback);
    d->setupClassDataUpdate.add(setupClassDataUpdate);
}
void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(
        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupAttach setupAttachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupDetach setupDetachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate setupClassDataUpdate
        ) {
    ZFCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.removeElement(setupAttachCallback);
    d->setupDetach.removeElement(setupDetachCallback);
    d->setupClassDataUpdate.removeElement(setupClassDataUpdate);
}

// ============================================================
static const zfchar *_ZFP_ZFImpl_ZFLua_sourceInfo(
        ZF_IN_OUT zfstring &sourceHolder
        , ZF_IN const zfchar *buf
        , ZF_IN_OPT zfindex bufLen
        , ZF_IN_OPT const zfchar *chunkInfo
        , ZF_IN_OPT const zfchar *srcInfo
        ) {
    if(srcInfo) {
        return srcInfo;
    }
    else if(ZFLogLevelIsActive(v_ZFLogLevel::e_Debug) && chunkInfo) {
        // append special pathInfo, for stack trace when critical error in cpp
        sourceHolder += "{{<<";
        sourceHolder += chunkInfo;
        sourceHolder += ">>}}";
        sourceHolder.append(buf, bufLen);
        return sourceHolder;
    }
    else if(ZFLogLevelIsActive(v_ZFLogLevel::e_Error)) {
        if(bufLen == zfindexMax() || buf[bufLen] == '\0') {
            return buf;
        }
        else {
            sourceHolder.append(buf, bufLen);
            return sourceHolder;
        }
    }
    else {
        return zfnull;
    }
}
zfbool ZFImpl_ZFLua_execute(
        ZF_IN lua_State *L
        , ZF_IN const zfchar *buf
        , ZF_IN_OPT zfindex bufLen /* = zfindexMax() */
        , ZF_OUT_OPT zfauto *luaResult /* = zfnull */
        , ZF_IN_OPT const ZFCoreArray<zfauto> *luaParams /* = zfnull */
        , ZF_OUT_OPT zfstring *errHint /* = zfnull */
        , ZF_IN_OPT const zfchar *chunkInfo /* = zfnull */
        , ZF_IN_OPT const zfchar *srcInfo /* = zfnull */
        ) {
    _ZFP_ZFImpl_ZFLua_invokeTimeLogger("execute %d: %s"
            , (int)(bufLen == zfindexMax() ? zfslen(buf) : bufLen)
            , zfstringReplace(
                zfstring(buf, zfmMin<zfindex>(bufLen == zfindexMax() ? zfslen(buf) : bufLen, 128))
                , "\n", "\\n"
                ).cString()
            );
    ZFImpl_ZFLua_DEBUG_luaStackChecker(ck, L, 0);

    int luaStackNum = lua_gettop(L);
    zfstring sourceHolder;
    int error = luaL_loadbuffer(L
            , buf, (bufLen == zfindexMax()) ? zfslen(buf) : bufLen
            , _ZFP_ZFImpl_ZFLua_sourceInfo(sourceHolder, buf, bufLen, chunkInfo, srcInfo)
            );
    if(error == 0) {
        if(luaParams != zfnull && !luaParams->isEmpty()) {
            for(zfindex i = 0; i < luaParams->count(); ++i) {
                ZFImpl_ZFLua_luaPush(L, luaParams->get(i));
            }
        }
        error = lua_pcall(
            L,
            (luaParams != zfnull ? (int)luaParams->count() : 0),
            LUA_MULTRET,
            0);
    }
    if(error != 0) {
        ZFImpl_ZFLua_execute_errorHandle(L, error, errHint, chunkInfo);
        return zffalse;
    }

    int luaResultNum = lua_gettop(L) - luaStackNum;
    if(luaResult != zfnull) {
        if(luaResultNum == 0) {
            *luaResult = zfnull;
        }
        else if(luaResultNum == 1) {
            ZFImpl_ZFLua_toGeneric(*luaResult, L, -1);
        }
        else {
            ZFCoreArray<zfauto> ret;
            *luaResult = zfobj<v_ZFCoreArray>(ret);
            ret.capacity((zfindex)luaResultNum);
            for(int i = 0; i < luaResultNum; ++i) {
                zfauto tmp;
                ZFImpl_ZFLua_toGeneric(tmp, L, -1 - (luaResultNum - 1 - i));
                ret.add(tmp);
            }
        }
    }
    if(luaResultNum > 0) {
        lua_pop(L, luaResultNum);
    }

    return zftrue;
}
void ZFImpl_ZFLua_execute_errorHandle(
        ZF_IN lua_State *L
        , ZF_IN int error
        , ZF_OUT_OPT zfstring *errHint /* = zfnull */
        , ZF_IN_OPT const zfchar *chunkInfo /* = zfnull */
        , ZF_IN_OPT zfindex errorLine /* = zfindexMax() */
        ) {
    if(error == 0) {
        return;
    }
    if(errHint == zfnull) {
        lua_pop(L, 1);
        return;
    }

    const char *nativeError = lua_tostring(L, -1);

    // detect error line
    // case 1: (lua syntax error, etc, usually the `123` is correct)
    //     [string "?"]:123: xxx
    //     ^^^^^^^^^^^^^^^^^^
    //     lua error header
    //
    // case 2: (by luaL_error, the `123` may be incorrect)
    //     [string "?"]:123: <<{{456}}>>xxx
    //     ^^^^^^^^^^^^^^^^^^
    //     lua error header
    //                       ^^^^^^^
    //                       special header passed from ZFImpl_ZFLua_luaError
    {
        zfindex tokenL = zfindexMax();
        zfindex tokenR = zfindexMax();
        zfindex offset = 0;
        const zfchar *KEY_tokenL = "<<{{";
        const zfchar *KEY_tokenR = "}}>>";
        tokenL = zfstringFind((const zfchar *)nativeError, KEY_tokenL);
        if(tokenL != zfindexMax()) {
            tokenL += zfslen(KEY_tokenL);
            tokenR = zfstringFind((const zfchar *)nativeError + tokenL, KEY_tokenR);
            if(tokenR != zfindexMax()) {
                tokenR += tokenL;
                offset = tokenR + zfslen(KEY_tokenR);
            }
        }
        else {
            KEY_tokenL = "?\"]:";
            KEY_tokenR = ": ";
            tokenL = zfstringFind((const zfchar *)nativeError, KEY_tokenL);
            if(tokenL != zfindexMax()) {
                tokenL += zfslen(KEY_tokenL);
                tokenR = zfstringFind((const zfchar *)nativeError + tokenL, KEY_tokenR);
                if(tokenR != zfindexMax()) {
                    tokenR += tokenL;
                    offset = tokenR + zfslen(KEY_tokenR);
                }
            }
        }
        if(tokenL != zfindexMax() && tokenR != zfindexMax()) {
            if(errorLine == zfindexMax()) {
                zfindexFromStringT(errorLine, (const zfchar *)nativeError + tokenL, tokenR - tokenL);
            }
            nativeError += offset;
        }
    }

    if(!zfstringIsEmpty(chunkInfo)) {
        *errHint += "[";
        *errHint += chunkInfo;
        if(errorLine != zfindexMax()) {
            *errHint += " (";
            zfindexToStringT(*errHint, errorLine);
            *errHint += ")";
        }
        *errHint += "] ";
    }
    *errHint += nativeError;
    lua_pop(L, 1);
}

zfbool ZFImpl_ZFLua_stacktraceT(
        ZF_IN lua_State *L
        , ZF_IN_OUT zfstring &ret
        ) {
    lua_Debug ar;
    ret += "====================== lua stack trace =====================";
    for(int i = 1; ; ++i) {
        if(lua_getstack(L, i, &ar) == 0) {
            if(i == 1) {
                return zffalse;
            }
            else {
                ret += "\n";
                ret += "---------------------- lua stack trace ---------------------";
                return zftrue;
            }
        }
        lua_getinfo(L, "nSl", &ar);
        if(ar.source != NULL) {
            if(ar.source[0] != '\0' && ar.currentline > 0) {
                const zfchar *source = ar.source;

                // parse special chunkInfo, passed from _ZFP_ZFImpl_ZFLua_sourceInfo
                zfstring chunkInfo;
                const zfchar *KEY_tokenL = "{{<<";
                const zfchar *KEY_tokenR = ">>}}";
                if(zfsncmp(source, KEY_tokenL, zfslen(KEY_tokenL)) == 0) {
                    zfindex p = zfstringFind(source, KEY_tokenR);
                    if(p != zfindexMax()) {
                        chunkInfo.assign(source + zfslen(KEY_tokenL), p - zfslen(KEY_tokenL));
                        source += p + zfslen(KEY_tokenR);
                    }
                }

                ZFCoreArray<ZFIndexRange> pos;
                zfstringSplitIndexT(pos, source, "\n", zftrue);
                if(ar.currentline <= pos.count()) {
                    ret += "\n| ";
                    zfstringAppend(ret, "[%s (%s)]  ", chunkInfo ? chunkInfo.cString() : "?", ar.currentline);

                    ZFIndexRange p = pos[ar.currentline - 1];
                    while(p.count != 0 && (source[p.start] == ' ' || source[p.start] == '\t')) {++p.start; --p.count;}
                    ret.append(source + p.start, p.count);
                }
            }
        }
        else {
            zfstringAppend(ret, "\n|   %s : %d", ar.short_src, ar.currentline);
        }
    }
}

// ============================================================
// utils
void ZFImpl_ZFLua_luaObjectInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool printLuaType /* = zffalse */
        ) {
    if(printLuaType) {
        ret += "(";
        ret += luaL_typename(L, luaStackOffset);
        ret += ")";
    }

    switch(lua_type(L, luaStackOffset)) {
        case LUA_TNIL:
            ret += "<nil>";
            break;
        case LUA_TBOOLEAN:
            zfboolToStringT(ret, (lua_toboolean(L, luaStackOffset) != 0));
            break;
        case LUA_TLIGHTUSERDATA:
        case LUA_TUSERDATA: {
                zfauto obj;
                if(ZFImpl_ZFLua_toObject(obj, L, luaStackOffset)) {
                    ZFObjectInfoT(ret, obj);
                }
                else {
                    zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
                }
            }
            break;
        case LUA_TNUMBER:
            zfdoubleToStringT(ret, (zfdouble)lua_tonumber(L, luaStackOffset));
            break;
        case LUA_TSTRING:
            ret += lua_tostring(L, luaStackOffset);
            break;
        case LUA_TTABLE:
            lua_getglobal(L, "zfl_tableInfo");
            if(lua_isnil(L, -1)) {
                lua_pop(L, 1);
                zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            }
            else {
                if(luaStackOffset < 0) {
                    lua_pushvalue(L, luaStackOffset - 1);
                }
                else {
                    lua_pushvalue(L, luaStackOffset);
                }
                lua_call(L, 1, 1);
                ret += lua_tostring(L, -1);
                lua_pop(L, 1);
            }
            break;
        case LUA_TFUNCTION:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        case LUA_TTHREAD:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        default:
            ret += "<Unknown>";
            break;
    }
}
zfbool ZFImpl_ZFLua_toObject(
        ZF_OUT zfauto &param
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        ) {
    if(lua_isuserdata(L, luaStackOffset)) {
        param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        return zftrue;
    }
    else {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toGeneric(
        ZF_OUT zfauto &param
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(ZFImpl_ZFLua_toObject(param, L, luaStackOffset)) {
        return zftrue;
    }
    else if(lua_isfunction(L, luaStackOffset)) {
        return ZFImpl_ZFLua_toCallback(param, L, luaStackOffset, errorHint);
    }
    else if(lua_isboolean(L, luaStackOffset)) {
        zfobj<v_zfbool> wrapper;
        wrapper->zfv = (zfbool)lua_toboolean(L, luaStackOffset);
        param = wrapper;
        return zftrue;
    }

    zfobj<ZFDI_Wrapper> wrapper;
    if(lua_isnumber(L, luaStackOffset)) {
        lua_Number n = lua_tonumber(L, luaStackOffset);
        if(zfmAbs(n - (long)n) < zffloatEpsilon) {
            wrapper->zfv(zfsFromInt((long)n));
        }
        else {
            wrapper->zfv(lua_tostring(L, luaStackOffset));
        }
        param = wrapper;
        return zftrue;
    }
    if(lua_isstring(L, luaStackOffset)) {
        wrapper->zfv(lua_tostring(L, luaStackOffset));
        param = wrapper;
        return zftrue;
    }
    if(lua_isnil(L, luaStackOffset)) {
        param = wrapper;
        return zftrue;
    }

    zfobj<ZFImpl_ZFLuaValue> holder;
    holder->L = L;
    lua_pushvalue(L, luaStackOffset);
    holder->luaValue = luaL_ref(L, LUA_REGISTRYINDEX);
    param = holder;
    return zftrue;
}

zfbool ZFImpl_ZFLua_toCallback(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(ZFImpl_ZFLua_toObject(ret, L, luaStackOffset)) {
        v_ZFCallback *callbackTmp = ret;
        if(callbackTmp != zfnull) {
            return zftrue;
        }
        else {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint,
                    "[ZFCallbackForLua] invalid param: %s",
                    ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue));
            }
            return zffalse;
        }
    }
    else {
        return ZFImpl_ZFLua_ZFCallbackForLua(ret, L, luaStackOffset, errorHint);
    }
}

zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT zfstring &s
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    const zfchar *t = zfnull;
    if(ZFImpl_ZFLua_toString(t, L, luaStackOffset, allowEmpty,  holderCls)) {
        s += t;
        return zftrue;
    }
    else {
        return zffalse;
    }
}
zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT zfstring &s
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    const zfchar *t = zfnull;
    if(ZFImpl_ZFLua_toString(t, obj, allowEmpty,  holderCls)) {
        s += t;
        return zftrue;
    }
    else {
        return zffalse;
    }
}
zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT const zfchar *&s
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(lua_isstring(L, luaStackOffset)) {
        s = lua_tostring(L, luaStackOffset);
        return zftrue;
    }
    if(lua_isboolean(L, luaStackOffset)) {
        if((zfbool)lua_toboolean(L, luaStackOffset)) {
            s = ZFTOKEN_zfbool_zftrue;
        }
        else {
            s = ZFTOKEN_zfbool_zffalse;
        }
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset)) {
        return zffalse;
    }

    zfauto const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    return ZFImpl_ZFLua_toString(s, param, allowEmpty, holderCls);
}
zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT const zfchar *&s
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull) {
        s = "";
        return allowEmpty;
    }

    const ZFClass *cls = obj->classData();
    if(cls->classIsTypeOf(v_zfstring::ClassData())) {
        if(holderCls != zfnull) {*holderCls = v_zfstring::ClassData();}
        s = obj->to<v_zfstring *>()->zfv;
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFDI_WrapperBase::ClassData())) {
        if(holderCls != zfnull) {*holderCls = ZFDI_WrapperBase::ClassData();}
        s = obj->to<ZFDI_WrapperBase *>()->zfv();
        return zftrue;
    }
    else {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toNumberT(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(lua_isuserdata(L, luaStackOffset)) {
        zfauto const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        return ZFImpl_ZFLua_toNumberT(ret, param, allowEmpty, holderCls);
    }

    int success = 0;
    lua_Number num = lua_tonumberx(L, luaStackOffset, &success);
    if(success) {
        ret = zfobj<v_zfdouble>((zfdouble)num);
        return zftrue;
    }
    return zffalse;
}

zfbool ZFImpl_ZFLua_toNumberT(
        ZF_OUT zfauto &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        , ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */
        ) {
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull) {
        if(allowEmpty) {
            ret = zfobj<v_zfdouble>(0);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }

    const ZFClass *cls = obj->classData();
    if(holderCls != zfnull) {*holderCls = cls;}
    zfobj<v_zfdouble> tmp;
    if(!ZFDI_toNumberT(tmp->zfv, obj)) {
        return zffalse;
    }
    ret = tmp;
    return zftrue;
}

zfbool ZFImpl_ZFLua_toLuaValue(
        ZF_IN lua_State *L
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty /* = zffalse */
        ) {
    if(obj == zfnull) {
        if(allowEmpty) {
            lua_pushnumber(L, 0);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }

    if(obj->classData()->classIsTypeOf(ZFImpl_ZFLuaValue::ClassData())) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, obj->to<ZFImpl_ZFLuaValue *>()->luaValue);
        return zftrue;
    }
    if(obj->classData()->classIsTypeOf(v_zfbool::ClassData())) {
        lua_pushboolean(L, zfcast(v_zfbool *, obj)->zfv);
        return zftrue;
    }
    zfauto t;
    if(ZFImpl_ZFLua_toNumberT(t, obj, allowEmpty)) {
        lua_pushnumber(L, (lua_Number)t.to<v_zfdouble *>()->zfv);
        return zftrue;
    }

    const zfchar *s = zfnull;
    if(ZFImpl_ZFLua_toString(s, obj, allowEmpty)) {
        lua_pushstring(L, s);
        return zftrue;
    }

    return zffalse;
}

// ============================================================
// ZFImpl_ZFLua_zfstringAppend
static zfbool _ZFP_ZFImpl_ZFLua_zfstringAppend_bool(
        ZF_IN lua_State *L
        , ZF_IN int i
        ) {
    zfbool ret = zftrue;
    zfauto t;
    if(ZFImpl_ZFLua_toObject(t, L, i)) {
        if(t == zfnull) {
            ret = zffalse;
        }
        else if(t->classData()->classIsTypeOf(v_zfbool::ClassData())) {
            ret = t->to<v_zfbool *>()->zfv;
        }
        else if(ZFImpl_ZFLua_toNumberT(t, L, i)) {
            ret = t->to<v_zfdouble *>()->zfv;
        }
    }
    else if(lua_isboolean(L, i)) {
        ret = (zfbool)lua_toboolean(L, i);
    }
    else if(lua_isnoneornil(L, i)) {
        ret = zffalse;
    }
    else if(lua_isnumber(L, i)) {
        ret = (lua_tonumber(L, i) != 0);
    }
    return ret;
}
static zfdouble _ZFP_ZFImpl_ZFLua_zfstringAppend_number(
        ZF_IN lua_State *L
        , ZF_IN int i
        ) {
    zfauto t;
    if(ZFImpl_ZFLua_toNumberT(t, L, i, zftrue)) {
        return t->to<v_zfdouble *>()->zfv;
    }
    else {
        return 0;
    }
}
static zfstring _ZFP_ZFImpl_ZFLua_zfstringAppend_pointer(
        ZF_IN lua_State *L
        , ZF_IN int i
        ) {
    zfstring ret;
    zfauto t;
    if(ZFImpl_ZFLua_toObject(t, L, i)) {
        zfstringAppend(ret, "%s", (const void *)t.toObject());
    }
    else {
        ZFImpl_ZFLua_luaObjectInfoT(ret, L, i);
    }
    return ret;
}
static zfstring _ZFP_ZFImpl_ZFLua_zfstringAppend_string(
        ZF_IN lua_State *L
        , ZF_IN int i
        , ZF_IN zfbool toString
        ) {
    zfstring ret;
    zfauto t;
    if(ZFImpl_ZFLua_toObject(t, L, i)) {
        ZFObjectInfoT(ret, t);
    }
    else if(ZFImpl_ZFLua_toString(ret, L, i)) {
        // nothing to do
    }
    else if(toString) {
        ZFImpl_ZFLua_luaObjectInfoT(ret, L, i);
    }
    return ret;
}
zfbool ZFImpl_ZFLua_zfstringAppend(
        ZF_IN lua_State *L
        , ZF_IN_OUT zfstring &s
        , ZF_IN_OPT int luaParamOffset /* = 1 */
        ) {
    int count = (int)lua_gettop(L);
    if(count <= luaParamOffset - 1) {
        return zftrue;
    }

    const zfchar *fmt = zfnull;
    if(!ZFImpl_ZFLua_toString(fmt, L, luaParamOffset)) {
        return zffalse;
    }

    zfstring fmtTmp;
    const zfchar *pFmt = fmt;
    const zfchar *pFmtL = fmt;
    const zfchar *pToken = zfnull;
    for(int i = luaParamOffset + 1; /* i <= count */; ++i) {
        do {
            while(*pFmt != '\0' && *pFmt != '%') {++pFmt;}
            if(*pFmt == '%' && *(pFmt + 1) == '%') {
                pFmt += 2;
                continue;
            }
        } while(zffalse);
        if(*pFmt == '\0' || *(pFmt + 1) == '\0') {
            break;
        }
        s.append(pFmtL, pFmt - pFmtL);

        pToken = pFmt + 1;
        while(zffalse
            || *pToken == '-'
            || *pToken == '+'
            || *pToken == ' '
            || *pToken == '.'
            || (*pToken >= '0' && *pToken <= '9')
        ) {
            ++pToken;
        }

        switch(*pToken) {
            case 'b':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_bool(L, i));
                }
                break;
            case 'z':
                if(*(pToken + 1) != 'i') {
                    return zffalse;
                }
                pFmtL = pToken + 2;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, (zfindex)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                }
                break;
            case 'd':
            case 'i':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, (zfint)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                }
                break;
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, (zfuint)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                }
                break;
            case 'f':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, (zffloat)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                }
                break;
            case 'p':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    fmtTmp.set(fmtTmp.length() - 1, 's');
                    zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_pointer(L, i));
                }
                break;
            case 'c':
            case 'C':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstring tmp = _ZFP_ZFImpl_ZFLua_zfstringAppend_string(L, i, zffalse);
                    zfstringAppend(s, fmtTmp, (zfchar)(tmp.isEmpty() ? '?' : tmp[0]));
                }
                break;
            case 's':
            case 'S':
                pFmtL = pToken + 1;
                if(i <= count) {
                    fmtTmp.assign(pFmt, pFmtL - pFmt);
                    zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_string(L, i, zftrue));
                }
                break;
            default:
                return zffalse;
        }
        pFmt = pFmtL;
    }
    s.append(pFmtL, (fmt + zfslen(fmt)) - pFmtL);

    return zftrue;
}

// ============================================================
zfstring ZFImpl_ZFLua_luaStackInfo(
        ZF_IN lua_State *L
        , ZF_IN_OPT int luaStackOffset /* = 1 */
        ) {
    zfstring ret;
    int count = lua_gettop(L);
    ret += "========================= lua stack ========================\n";
    for(int i = count; i >= luaStackOffset; --i) {
        zfstringAppend(ret, "\t%s: ", i);
        ZFImpl_ZFLua_luaObjectInfoT(ret, L, i, zftrue);
        ret += '\n';
    }
    ret += "------------------------- lua stack ------------------------\n";
    return ret;
}

ZF_STATIC_INITIALIZER_INIT(ZFImpl_ZFLua_criticalErrorHandler) {
    ZFCoreCriticalErrorCallbackAdd(zfself::impl);
}
static void impl(const ZFCallerInfo &callerInfo) {
    lua_State *L = (lua_State *)ZFLuaStateCheck();
    if(L != NULL) {
        zfstring info;
        if(ZFImpl_ZFLua_stacktraceT(L, info)) {
            ZFCoreLogTrim("%s", info);
        }
    }
}
ZF_STATIC_INITIALIZER_END(ZFImpl_ZFLua_criticalErrorHandler)

ZF_NAMESPACE_GLOBAL_END

