#include "ZFImpl_ZFLua_zfl_dbg.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_DEFINE(zfuint, ZFLuaDebugPort, 32199)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaDebugImpl, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaDebugImpl) {
    if(impl) {
        impl->invoke("stop");
    }
}
zfauto impl; // ZFHttpServer
zfautoT<ZFSemaphore> sema;
zfstring readBuf;
zfstring writeBuf;
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

    if(ZFLuaDebugPort() != 0
            && ZFProtocolIsAvailable("ZFHttpServer")
            ) {
        do {
            ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaDebugImpl) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaDebugImpl);
            zfauto &impl = d->impl;
            zfautoT<ZFSemaphore> &sema = d->sema;
            if(!impl) {
                impl = ZFClass::classForName("ZFHttpServer")->newInstance();
                if(!impl) {
                    break;
                }

                sema = zfobj<ZFSemaphore>();
                impl->invoke("port", zfobj<v_zfuint>(ZFLuaDebugPort()));
                ZFLISTENER_1(onRecv
                        , ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaDebugImpl) *, d
                        ) {
                    zfauto task = zfargs.param0();
                    d->readBuf = task->invoke("recvBody")->to<v_zfstring *>()->zfv;
                    d->sema->lockAndBroadcast();

                    ZFThread::sleep(100); // wait for debugger's output (may or may not have further output)
                    ZFCoreMutexLock();
                    zfstring writeBuf = d->writeBuf;
                    d->writeBuf = zfnull;
                    ZFCoreMutexUnlock();

                    task->invoke("respBody"
                            , zfobj<v_zfstring>(writeBuf)
                            , zfobj<v_zfstring>("application/json")
                            );
                } ZFLISTENER_END()
                impl->invoke("onRequest", zfobj<v_ZFCallback>(onRecv));
                impl->invoke("start");
            }

            d->readBuf = "c";
            sema->lockAndWait();
            ZFLogTrim("%s%s", prompt, d->readBuf);
            lua_pushstring(L, d->readBuf.cString());
            return 1;
        } while(zffalse);
    }

    char buf[1024];
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    lua_pushstring(L, buf);
    ZFLogTrim("%s%s", prompt, buf);
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_zfl_dbg_write(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);
    int count = (int)lua_gettop(L);
    if(count != 1) {
        return ZFImpl_ZFLua_luaError(L
                , "zfl_dbg.read(prompt) takes only one param, got %s"
                , (zfindex)count
                );
    }

    ZF_GLOBAL_INITIALIZER_CLASS(ZFLuaDebugImpl) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaDebugImpl);
    zfstring prompt = ZFImpl_ZFLua_toString(L, 1);
    d->writeBuf += prompt;
    if(prompt && prompt[prompt.length() - 1] == '\n') {
        prompt.remove(prompt.length() - 1);
    }
    ZFLogTrim("%s", prompt);
    return 0;
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_dbg, ZFM_EXPAND({
        ZFInput srcInput = ZFInputForResRaw("ZFLua_impl/debugger.lua");
        if(srcInput) {
            ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zfl_dbg_read", _ZFP_ZFImpl_ZFLua_zfl_dbg_read);
            ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zfl_dbg_write", _ZFP_ZFImpl_ZFLua_zfl_dbg_write);

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
                    "zfl_dbg.write = _ZFP_ZFImpl_ZFLua_zfl_dbg_write\n"
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

