#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_ZFLog_DEBUG_ENABLE 1

static int _ZFP_ZFImpl_ZFLua_ZFLog(ZF_IN lua_State *L) {
    ZFOutput o = ZFLogTrim();

    if(ZFLogHeaderDefault_logTime()) {
        o << ZFLogCurTimeString() << " ";
    }

    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s)) {
        o << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_ZFLogTrim(ZF_IN lua_State *L) {
    ZFOutput o = ZFLogTrim();

    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s)) {
        o << s;
    }
    return 0;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFLog, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFLog", _ZFP_ZFImpl_ZFLua_ZFLog);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFLogTrim", _ZFP_ZFImpl_ZFLua_ZFLogTrim);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

#if _ZFP_ZFImpl_ZFLua_ZFLog_DEBUG_ENABLE
    ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLog,
            "function (f, ...)"
            "  if ZFLogHeaderDefault_logCaller() then"
            "    local n = zfstring()"
            "    ZFPathInfoToFileName(n, zfl_l or ZFPathInfo('file','unknown'))"
            "    return _G['ZFLog']('[' .. n .. ' (' .. debug.getinfo(2).currentline .. ')] ' .. (f or ''), ...);"
            "  else"
            "    return _G['ZFLog'](f or '', ...);"
            "  end;"
            "end"
        )
#else
    ZFImpl_ZFLua_implPathInfo_DEFINE(ZFLog,
            "function (f, ...)"
            "  if ZFLogHeaderDefault_logCaller() then"
            "    local n = zfstring()"
            "    ZFPathInfoToFileName(n, zfl_l or ZFPathInfo('file','unknown'))"
            "    return _G['ZFLog']('[' .. n .. '] ' .. (f or ''), ...);"
            "  else"
            "    return _G['ZFLog'](f or '', ...);"
            "  end;"
            "end"
        )
#endif

ZF_NAMESPACE_GLOBAL_END

