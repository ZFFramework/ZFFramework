#include "ZFImpl_ZFLua_zfl_dbg.h"
#include "ZFNet.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zfuint, ZFLuaDebugPort, 30000)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaDebugImpl, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaDebugImpl) {
    if(impl) {
        impl->stop();
    }
}
zfautoT<ZFHttpServer> impl;
zfautoT<ZFSemaphore> sema;
zfstring buf;
ZF_GLOBAL_INITIALIZER_END(ZFLuaDebugImpl)

static int _ZFP_ZFImpl_ZFLua_zfl_dbg_read(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);
    int count = (int)lua_gettop(L);
    if(count != 1) {
        return ZFImpl_ZFLua_luaError(L
                , "zfl_dbg.read(prompt) takes only one param, got %s"
                , (zfindex)count
                );
    }

    zfstring prompt = ZFImpl_ZFLua_toString(L, 1);
    ZFLogTrim("%s", prompt);

    if(ZFLuaDebugPort() == 0) {
        char buf[256];
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        lua_pushstring(L, buf);
        return 1;
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaDebugImpl) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaDebugImpl);
    zfautoT<ZFHttpServer> &impl = d->impl;
    zfautoT<ZFSemaphore> &sema = d->sema;
    d->buf = "q";
    if(!impl) {
        sema = zfobj<ZFSemaphore>();
        impl = zfobj<ZFHttpServer>();
        impl->port(ZFLuaDebugPort());
        ZFLISTENER_1(onRecv
                , zfautoT<ZFSemaphore>, sema
                ) {
            ZFHttpServerTask *task = zfargs.param0();
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaDebugImpl)->buf = task->recvBody();
            task->respBody("{}", "application/json");
            sema->lockAndBroadcast();
        } ZFLISTENER_END()
        impl->onRequest(onRecv);
        impl->start();
    }

    sema->lockAndWait();
    lua_pushstring(L, d->buf.cString());
    return 1;
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_value, ZFM_EXPAND({
        ZFInput srcInput = ZFInputForResRaw("ZFLua_impl/debugger.lua");
        if(srcInput) {
            ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zfl_dbg_read", _ZFP_ZFImpl_ZFLua_zfl_dbg_read);

            zfstring src;
            ZFInputRead(src, srcInput);
            int error = 0;
            int luaStackNum = lua_gettop(L);

            // note, at this time, most of ZFFramework's method may not been initialized
            // use raw loading

            // load debugger.lua
            // stack: [] => [dbg]
            error = luaL_loadbuffer(L, src.cString(), src.length(), src.cString());
            ZFCoreAssert(error == 0);
            error = lua_pcall(L, 0, 1, 0);
            ZFCoreAssert(error == 0);

            const zfchar *code =
                    "local args = {...}\n"
                    "zfl_dbg = args[1]\n"
                    "\n"
                    "local findLast = function(s, pattern)\n"
                    "    local i = 0\n"
                    "    while true do\n"
                    "        local j = string.find(s, pattern, i + 1)\n"
                    "        if not j then\n"
                    "            break\n"
                    "        end\n"
                    "        i = j\n"
                    "    end\n"
                    "    return i\n"
                    "end\n"
                    "local format_stack_frame_info_orig = zfl_dbg.format_stack_frame_info\n"
                    "zfl_dbg.format_stack_frame_info = function(info)\n"
                    "    local pathInfo = info.source:match('{{<<(.-)>>}}')\n"
                    "    if pathInfo == nil then\n"
                    "        return '\\t\\t\\t\\t' .. format_stack_frame_info_orig(info)\n"
                    "    end\n"
                    "    local lineInfo = ''\n"
                    "    local lines = {}\n"
                    "    for line in info.source:gmatch('([^\\n]*)\\n?') do table.insert(lines, line) end\n"
                    "    if lines[info.currentline] ~= nil then\n"
                    "        lineInfo = lines[info.currentline]:gsub('^%s*(.-)%s*$', '%1') .. '\\t\\t\\t'\n"
                    "    end\n"
                    "    local p0 = pathInfo:find(':')\n"
                    "    local p1 = findLast(pathInfo, '/')\n"
                    "    if p1 <= 0 or p0 <= 0 then\n"
                    "        source = '[' .. pathInfo .. ']'\n"
                    "    else\n"
                    "        source = '[' .. string.sub(pathInfo, 1, p0) .. string.sub(pathInfo, p1 + 1) .. ']'\n"
                    "    end\n"
                    "    return lineInfo..zfl_dbg.COLOR_BLUE..source..zfl_dbg.COLOR_RESET..':'..zfl_dbg.COLOR_YELLOW..info.currentline..zfl_dbg.COLOR_RESET\n"
                    "end\n"
                    "zfl_dbg.read = _ZFP_ZFImpl_ZFLua_zfl_dbg_read\n"
                    ;
            // load code
            // stack: [dbg] => [dbg, func]
            error = luaL_loadbuffer(L, code, zfslen(code), code);
            ZFCoreAssert(error == 0);
            // push param
            // stack: [dbg, func] => [dbg, func, dbg]
            lua_pushvalue(L, -2);
            // run code
            // stack: [dbg, func, dbg] => [dbg]
            error = lua_pcall(L, 1, 0, 0);
            ZFCoreAssert(error == 0);
            // stack: [dbg] => []
            lua_pop(L, 1);
            ZFCoreAssert(lua_gettop(L) == luaStackNum);
        }
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

