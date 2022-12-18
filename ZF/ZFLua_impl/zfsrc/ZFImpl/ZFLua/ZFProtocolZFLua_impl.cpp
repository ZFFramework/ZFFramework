#include "ZFImpl.h"
#include "ZFLua/protocol/ZFProtocolZFLua.h"
#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFLua_impl, ZFLua, ZFProtocolLevel::e_Default)
public:
    virtual void *luaStateOpen(void)
    {
        return ZFImpl_ZFLua_luaStateOpen();
    }
    virtual void luaStateClose(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateClose((lua_State *)L);
    }

    virtual void luaStateAttach(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateAttach((lua_State *)L);
    }
    virtual void luaStateDetach(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateDetach((lua_State *)L);
    }

    virtual void classDataChange(ZF_IN void *L,
                                 ZF_IN const ZFClassDataChangeData &data)
    {
        ZFImpl_ZFLua_classDataChange((lua_State *)L, data);
    }

    virtual zfbool luaExecute(ZF_IN void *L,
                              ZF_IN const ZFPathInfo *pathInfoOrNull,
                              ZF_IN const ZFInput &input,
                              ZF_OUT_OPT zfautoObject *luaResult = zfnull,
                              ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams = zfnull,
                              ZF_OUT_OPT zfstring *errorHint = zfnull)
    {
        if(ZFFrameworkStateCheck(ZFLevelZFFrameworkLow) != ZFFrameworkStateAvailable)
        {
            ZFLuaErrorOccurredTrim(
                "[ZFLua] lua module only available after ZFFrameworkStateCheck(ZFLevelZFFrameworkLow)"
                );
            return zffalse;
        }

        zfstring buf;
        ZFImpl_ZFLua_implPathInfoSetup((lua_State *)L, buf, pathInfoOrNull);
        ZFInputReadAll(buf, input);
        return ZFImpl_ZFLua_execute((lua_State *)L, buf.cString(), buf.length(), luaResult, luaParams, errorHint, input.callbackId());
    }

    virtual void luaGC(ZF_IN void *L)
    {
        lua_gc((lua_State *)L, LUA_GCCOLLECT, 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFLua_impl)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFLua_impl)

ZF_NAMESPACE_GLOBAL_END

