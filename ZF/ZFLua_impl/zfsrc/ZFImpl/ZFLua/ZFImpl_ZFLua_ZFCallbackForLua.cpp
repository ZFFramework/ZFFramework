#include "ZFImpl_ZFLua.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder;
static void _ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback(ZF_IN const ZFArgs &zfargs);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_ZFCallbackAutoClean, ZFLevelZFFrameworkNormal)
{
    this->luaStateOnDetachListener = ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback);
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::EventLuaStateOnDetach(), this->luaStateOnDetachListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_ZFCallbackAutoClean)
{
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::EventLuaStateOnDetach(), this->luaStateOnDetachListener);
}
ZFListener luaStateOnDetachListener;
ZFCoreArrayPOD<_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *> attachList;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_ZFCallbackAutoClean)

zfclass _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->luaFuncInfo.removeAll();
        cache->ownerThread = zfnull;
    })

public:
    lua_State *L;
    int luaFunc;
    zfstring luaFuncInfo;
    zfautoObjectT<ZFObjectHolder *> ownerThread;

    ZFMETHOD_INLINE_1(void, callback,
                      ZFMP_IN(const ZFArgs &, zfargs))
    {
        if(L == zfnull)
        {
            return;
        }

        if(ZFLogLevelIsActive(ZFLogLevel::e_Debug) && ZFPROTOCOL_IS_AVAILABLE(ZFThread) && this->ownerThread != zfnull)
        {
            zfCoreMutexLocker();
            if(this->ownerThread != zfnull && this->ownerThread->objectHolded() != zfnull)
            {
                ZFThread *curThread = ZFThread::currentThread();
                if(curThread != zfnull && curThread != this->ownerThread->objectHolded())
                {
                    zfCoreCriticalMessageTrim(
                        "[ZFCallbackForLua] can not execute in different thread, use ZFCallbackForLuaAsync instead, func: %s",
                        this->luaFuncInfo.cString());
                    return;
                }
            }
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, luaFunc);
        if(!lua_isfunction(L, -1))
        {
            zfCoreCriticalMessageTrim(
                "[ZFCallbackForLua] invalid function: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, -1, zftrue).cString());
            return;
        }

        zfblockedAlloc(v_ZFArgs, zfargsHolder);
        zfargsHolder->zfv = zfargs;
        ZFImpl_ZFLua_luaPush(L, zfargsHolder);

        int error = lua_pcall(L, 1, 0, 0);
        ZFImpl_ZFLua_execute_errorHandle(L, error, zfnull, this->luaFuncInfo);
    }
protected:
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList.add(this);
        this->L = zfnull;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        if(L != zfnull)
        {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList.removeElement(this);
            luaL_unref(L, LUA_REGISTRYINDEX, luaFunc);
            L = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }
};
ZFOBJECT_REGISTER(ZFImpl_ZFLuaValue)

static void _ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback(ZF_IN const ZFArgs &zfargs)
{
    lua_State *L = (lua_State *)zfargs.param0()->to<v_ZFPtr *>()->zfv;
    ZFCoreArrayPOD<_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *> &attachList = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList;
    for(zfindex i = attachList.count() - 1; i != zfindexMax(); --i)
    {
        _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *p = attachList[i];
        if(p->L == L)
        {
            attachList.remove(i);
            luaL_unref(p->L, LUA_REGISTRYINDEX, p->luaFunc);
            p->L = zfnull;
        }
    }
}

// ============================================================
zfbool ZFImpl_ZFLua_ZFCallbackForLua(ZF_OUT zfautoObject &ret,
                                     ZF_IN lua_State *L,
                                     ZF_IN int luaStackOffset,
                                     ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(ZFImpl_ZFLua_toObject(ret, L, luaStackOffset))
    {
        v_ZFCallback *callbackTmp = ret;
        if(callbackTmp != zfnull)
        {
            return zftrue;
        }
        else
        {
            zfstringAppend(errorHint,
                "[ZFCallbackForLua] invalid param: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString());
            return zffalse;
        }
    }

    if(lua_isfunction(L, luaStackOffset))
    {
        zfblockedAlloc(v_ZFCallback, callback);
        zfblockedAlloc(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, holder);
        holder->L = L;
        lua_pushvalue(L, luaStackOffset);
        holder->luaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
        if(ZFLogLevelIsActive(ZFLogLevel::e_Debug) && ZFPROTOCOL_IS_AVAILABLE(ZFThread))
        {
            zfCoreMutexLocker();
            ZFThread *curThread = ZFThread::currentThread();
            if(curThread != zfnull)
            {
                holder->ownerThread = curThread->objectHolder();
            }
        }
        callback->zfv = ZFCallbackForMemberMethod(holder, ZFMethodAccess(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, callback));
        callback->zfv.callbackOwnerObjectRetain();
        ret = callback;

        if(ZFLogLevelIsActive(ZFLogLevel::e_Debug))
        {
            const zfchar *buf =
                    "local arg={...}\n"
                    "local info=debug.getinfo(arg[1])\n"
                    "local i = 1\n"
                    "while true do\n"
                    "    local n,v = debug.getlocal(4, i)\n"
                    "    if not n then break end\n"
                    "    if n == 'ZFLuaPathInfo' then\n"
                    "        return v() .. ':' .. info['linedefined']\n"
                    "    end\n"
                    "    i = i + 1\n"
                    "end\n"
                    "return info['source'] .. ':' .. info['linedefined']\n"
                ;
            int error = luaL_loadbuffer(L, buf, zfslen(buf), "[ZFLuaDebug]");
            if(error == 0)
            {
                lua_pushvalue(L, luaStackOffset);
                error = lua_pcall(L, 1, 1, 0);
                if(error == 0)
                {
                    ZFImpl_ZFLua_toString(holder->luaFuncInfo, L, -1, zftrue);
                    lua_pop(L, 1);
                }
            }
            if(error != 0)
            {
                const char *nativeError = lua_tostring(L, -1);
                zfCoreCriticalMessageTrim("[ZFLuaDebug] %s", nativeError);
                lua_pop(L, 1);
            }
        }
        return zftrue;
    }
    else
    {
        zfstringAppend(errorHint,
            "[ZFCallbackForLua] invalid param: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString());
        return zffalse;
    }
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_ZFCallbackForLua(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[ZFCallbackForLua] expect one param, got %zi",
            (zfindex)count);
    }
    if(!lua_isfunction(L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[ZFCallbackForLua] takes function(zfargs) as param, got: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
    }

    zfstring errorHint;
    zfautoObject ret;
    if(!ZFImpl_ZFLua_ZFCallbackForLua(ret, L, 1, &errorHint))
    {
        return ZFImpl_ZFLua_luaError(L, "%s", errorHint.cString());
    }
    else
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCallbackForLua, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCallbackForLua", _ZFP_ZFImpl_ZFLua_ZFCallbackForLua);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

