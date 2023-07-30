#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE 1

static int _ZFP_ZFImpl_ZFLua_zfLog(ZF_IN lua_State *L) {
    ZFOutput o = zfLogTrim();
    o << zfLogCurTimeString();

    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s)) {
        o << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfLogTrim(ZF_IN lua_State *L) {
    ZFOutput o = zfLogTrim();

    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s)) {
        o << s;
    }
    return 0;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfLog, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLog", _ZFP_ZFImpl_ZFLua_zfLog);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogTrim", _ZFP_ZFImpl_ZFLua_zfLogTrim);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

#if _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l or 'unknown') .. ' (' .. debug.getinfo(2).currentline .. ')] ' .. (f or ''), ...);"
            "end"
        )
#else
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l or 'unknown') .. '] ' .. (f or ''), ...);"
            "end"
        )
#endif

ZF_NAMESPACE_GLOBAL_END

