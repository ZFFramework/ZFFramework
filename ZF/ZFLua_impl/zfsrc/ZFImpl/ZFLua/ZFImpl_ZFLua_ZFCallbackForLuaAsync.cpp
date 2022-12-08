#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef enum {
    _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_ZFObject,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_localFunc,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_bool,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_int,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_number,
    _ZFP_I_ZFCallbackForLuaAsyncValueType_string,
} _ZFP_I_ZFCallbackForLuaAsyncValueType;

// ============================================================
zfclass _ZFP_I_ZFCallbackForLuaAsyncValueHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFCallbackForLuaAsyncValueHolder, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })

public:
    _ZFP_I_ZFCallbackForLuaAsyncValueType valueType;
    union {
        ZFObject *objectValue;
        zfchar *localFuncName;
        int boolValue;
        lua_Integer intValue;
        lua_Number numberValue;
        zfchar *stringValue;
    } v;
    int upvalueIndex;

private:
    void _cleanup(void)
    {
        switch(this->valueType)
        {
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid:
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_ZFObject:
                zfRetainChange(v.objectValue, zfnull);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_localFunc:
                zffree(v.localFuncName);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_bool:
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_int:
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_number:
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_string:
                if(v.stringValue != zfnull)
                {
                    zffree(v.stringValue);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        this->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid;
    }
protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret)
    {
        switch(this->valueType)
        {
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid:
                ret += "(Invalid)";
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_ZFObject:
                ret += "(ZFObject)";
                ZFObjectInfoT(ret, v.objectValue);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_localFunc:
                ret += "(localFunc)";
                ret += v.localFuncName;
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_bool:
                ret += "(bool)";
                zfboolToString(ret, (zfbool)v.boolValue);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_int:
                ret += "(int)";
                zfintToString(ret, (zfint)v.intValue);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_number:
                ret += "(number)";
                zflongdoubleToString(ret, (zflongdouble)v.numberValue);
                break;
            case _ZFP_I_ZFCallbackForLuaAsyncValueType_string:
                ret += "(string)";
                ret += v.stringValue;
                break;
            default:
                ret += "(Unknown)";
                zfintToString(ret, (zfint)this->valueType);
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFCallbackForLuaAsyncValueHolder)

// ============================================================
zfclass _ZFP_I_ZFCallbackForLuaAsyncCallback : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFCallbackForLuaAsyncCallback, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->pathInfo.pathType.removeAll();
        cache->pathInfo.pathData.removeAll();
        cache->func.bufferFree();
        cache->upvalues.removeAll();
    })

public:
    ZFPathInfo pathInfo;
    ZFBuffer func;
    zfbool funcReaderDone;
    ZFCoreArray<zfautoObject> upvalues; // _ZFP_I_ZFCallbackForLuaAsyncValueHolder

public:
    zfstring logTag(void)
    {
        if(this->pathInfo.pathType.isEmpty() && this->pathInfo.pathData.isEmpty())
        {
            return "[ZFCallbackForLuaAsync]";
        }
        else
        {
            return zfstringWithFormat("[ZFCallbackForLuaAsync:%s]", ZFPathInfoToString(this->pathInfo).cString());
        }
    }

public:
    ZFMETHOD_DECLARE_1(void, callback,
                       ZFMP_IN(const ZFArgs &, zfargs))
};
ZFOBJECT_REGISTER(_ZFP_I_ZFCallbackForLuaAsyncCallback)

// ============================================================
static int _ZFP_ZFCallbackForLuaAsync_funcWriter(lua_State *L, const void *p, size_t sz, void *ud)
{
    _ZFP_I_ZFCallbackForLuaAsyncCallback *callbackOwner = (_ZFP_I_ZFCallbackForLuaAsyncCallback *)ud;
    callbackOwner->func.bufferAppend(p, (zfindex)sz);
    return 0;
}
static const char *_ZFP_ZFCallbackForLuaAsync_funcReader(lua_State *L, void *data, size_t *size)
{
    _ZFP_I_ZFCallbackForLuaAsyncCallback *callbackOwner = (_ZFP_I_ZFCallbackForLuaAsyncCallback *)data;
    if(callbackOwner->funcReaderDone)
    {
        return NULL;
    }
    callbackOwner->funcReaderDone = zftrue;
    *size = (size_t)callbackOwner->func.textLength();
    return callbackOwner->func.text();
}

// convert upvalue from top stack, pop one value
static zfbool _ZFP_ZFCallbackForLuaAsync_fromUpvalue(ZF_OUT zfautoObject &ret, ZF_IN lua_State *L, ZF_IN const char *name, ZF_IN int upvalueIndex)
{
    zfblockedAlloc(_ZFP_I_ZFCallbackForLuaAsyncValueHolder, holder);
    ret = holder;
    holder->upvalueIndex = upvalueIndex;
    if(lua_isboolean(L, -1))
    {
        holder->v.boolValue = lua_toboolean(L, -1);
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_bool;
    }
    else if(lua_isinteger(L, -1))
    {
        holder->v.intValue = lua_tointeger(L, -1);
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_int;
    }
    else if(lua_isnumber(L, -1))
    {
        holder->v.numberValue = lua_tonumber(L, -1);
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_number;
    }
    else if(lua_isstring(L, -1))
    {
        holder->v.stringValue = zfsCopy(lua_tostring(L, -1));
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_string;
    }
    else if(lua_isuserdata(L, -1))
    {
        zfautoObject obj;
        if(!ZFImpl_ZFLua_toObject(obj, L, -1))
        {
            return zffalse;
        }
        holder->v.objectValue = zfRetain(obj);
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_ZFObject;
    }
    else if(lua_isfunction(L, -1))
    {
        if(!ZFImpl_ZFLua_implPathInfoExist(name))
        {
            return zffalse;
        }
        holder->v.localFuncName = zfsCopy(name);
        holder->valueType = _ZFP_I_ZFCallbackForLuaAsyncValueType_localFunc;
    }
    else
    {
        return zffalse;
    }
    return zftrue;
}
// convert to upvalue and push to stack top
static zfbool _ZFP_ZFCallbackForLuaAsync_toUpvalue(ZF_IN _ZFP_I_ZFCallbackForLuaAsyncValueHolder *holder, ZF_IN lua_State *L, ZF_IN int luaFuncIndex,
                                                   ZF_IN const ZFCoreArrayPOD<const zfchar *> &luaLocalFuncNameList, ZF_IN int luaLocalFuncIndex)
{
    if(holder == zfnull)
    {
        return zffalse;
    }
    switch(holder->valueType)
    {
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_Invalid:
            return zffalse;
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_ZFObject:
            ZFImpl_ZFLua_luaPush(L, holder->v.objectValue);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_localFunc:
        {
            const zfchar *name = holder->v.localFuncName;
            int offset = -1;
            for(zfindex i = 0; i < luaLocalFuncNameList.count(); ++i)
            {
                if(zfscmpTheSame(name, luaLocalFuncNameList[i]))
                {
                    offset = (int)i;
                    break;
                }
            }
            if(offset == zfindexMax())
            {
                return zffalse;
            }
            lua_pushvalue(L, luaLocalFuncIndex + offset);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        }
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_bool:
            lua_pushboolean(L, holder->v.boolValue);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_int:
            lua_pushinteger(L, holder->v.intValue);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_number:
            lua_pushnumber(L, holder->v.numberValue);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        case _ZFP_I_ZFCallbackForLuaAsyncValueType_string:
            lua_pushstring(L, holder->v.stringValue);
            lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, holder->upvalueIndex);
            return zftrue;
        default:
            return zffalse;
    }
}

// ============================================================
zfbool ZFImpl_ZFLua_ZFCallbackForLuaAsync(ZF_OUT zfautoObject &ret,
                                          ZF_IN lua_State *L,
                                          ZF_IN int luaStackOffset,
                                          ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(!lua_isfunction(L, luaStackOffset))
    {
        zfstringAppend(errorHint,
            "[ZFCallbackForLuaAsync] invalid param: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString());
        return zffalse;
    }

    zfCoreMutexLocker();
    zfblockedAlloc(_ZFP_I_ZFCallbackForLuaAsyncCallback, callbackOwner);

    // store path info
    {
        lua_Debug ar;
        for(int iStack = 1; iStack <= 2; ++iStack)
        {
            int success = lua_getstack(L, iStack, &ar);
            if(!success)
            {
                zfstringAppend(errorHint,
                    "[ZFCallbackForLuaAsync] unable to obtain stack %d",
                    (zfint)iStack);
                return zffalse;
            }
            for(int iLocal = 1; ; ++iLocal)
            {
                const char *varName = lua_getlocal(L, &ar, iLocal);
                if(varName == NULL)
                {
                    break;
                }
                if(strcmp(varName, "ZFLuaPathInfo") != 0)
                {
                    lua_pop(L, 1);
                    continue;;
                }
                int error = lua_pcall(L, 0, 1, 0);
                if(error != 0)
                {
                    lua_pop(L, 1);
                    zfstringAppend(errorHint,
                        "[ZFCallbackForLuaAsync] unable to obtain path info, invoke fail");
                    return zffalse;
                }
                zfautoObject pathInfoHolder;
                if(!ZFImpl_ZFLua_toObject(pathInfoHolder, L, -1))
                {
                    lua_pop(L, 1);
                    zfstringAppend(errorHint,
                        "[ZFCallbackForLuaAsync] unable to obtain path info");
                    return zffalse;
                }
                v_ZFPathInfo *pathInfo = pathInfoHolder;
                if(pathInfo == zfnull)
                {
                    lua_pop(L, 1);
                    zfstringAppend(errorHint,
                        "[ZFCallbackForLuaAsync] unable to obtain path info, got: %s",
                        ZFObjectInfo(pathInfoHolder).cString());
                    return zffalse;
                }
                callbackOwner->pathInfo = pathInfo->zfv;
                lua_pop(L, 1);
            }
        }
    }

    // dump the function
    lua_pushvalue(L, luaStackOffset);
    int dumpError = lua_dump(L, _ZFP_ZFCallbackForLuaAsync_funcWriter, callbackOwner, 0);
    if(dumpError != 0)
    {
        lua_pop(L, 1);
        zfstringAppend(errorHint,
            "%s unable to dump function: %s",
            callbackOwner->logTag().cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString());
        return zffalse;
    }

    // save upvalue
    for(int upvalueIndex = 1; ; ++upvalueIndex)
    {
        const char *name = lua_getupvalue(L, luaStackOffset, upvalueIndex);
        if(name == NULL)
        {
            break;
        }
        else if(strcmp(name, "_ENV") == 0)
        {
            lua_pop(L, 1);
            continue;
        }
        zfautoObject upvalue;
        if(!_ZFP_ZFCallbackForLuaAsync_fromUpvalue(upvalue, L, name, upvalueIndex))
        {
            zfstringAppend(errorHint,
                "%s unable to store upvalue at index %d: %s",
                callbackOwner->logTag().cString(),
                (zfint)upvalueIndex,
                ZFImpl_ZFLua_luaObjectInfo(L, -1, zftrue).cString());
            lua_pop(L, 1);
            return zffalse;
        }
        lua_pop(L, 1);
        callbackOwner->upvalues.add(upvalue);
    }

    // result
    zfblockedAlloc(v_ZFCallback, callback);
    callback->zfv = ZFCallbackForMemberMethod(callbackOwner, ZFMethodAccess(_ZFP_I_ZFCallbackForLuaAsyncCallback, callback));
    callback->zfv.callbackOwnerObjectRetain();
    ret = callback;
    return zftrue;
}

ZFMETHOD_DEFINE_1(_ZFP_I_ZFCallbackForLuaAsyncCallback, void, callback,
                  ZFMP_IN(const ZFArgs &, zfargs))
{
    lua_State *L = (lua_State *)ZFLuaState();
    _ZFP_I_ZFCallbackForLuaAsyncCallback *callbackOwner = this;
    zfstring logTag = callbackOwner->logTag();
    {
        zfCoreMutexLocker();

        // restore function, stack: [func]
        callbackOwner->funcReaderDone = zffalse;
        int loadError = lua_load(L, _ZFP_ZFCallbackForLuaAsync_funcReader, callbackOwner, logTag.cString(), "b");
        if(loadError != LUA_OK)
        {
            zfCoreCriticalMessageTrim(
                "%s unable to load function",
                logTag.cString());
            return;
        }
        int luaFuncIndex = lua_gettop(L);

        // set function args, stack: [func, zfargs]
        zfblockedAlloc(v_ZFArgs, zfargsHolder);
        zfargsHolder->zfv = zfargs;
        ZFImpl_ZFLua_luaPush(L, zfargsHolder);

        // restore local path info, stack: [func, zfargs, localFunc0, localFunc1, ...]
        const ZFCoreArrayPOD<const zfchar *> &luaLocalFuncNameList = ZFImpl_ZFLua_implPathInfoList();
        int luaLocalFuncIndex = lua_gettop(L) + 1;
        if(!callbackOwner->pathInfo.pathType.isEmpty() && !luaLocalFuncNameList.isEmpty())
        {
            zfstring code;
            ZFImpl_ZFLua_implPathInfoSetup(L, code, &(callbackOwner->pathInfo), zffalse);
            int error = luaL_loadbuffer(L, code.cString(), code.length(), zfnull);
            ZFImpl_ZFLua_execute_errorHandle(L, error, zfnull, logTag.cString());
            error = lua_pcall(L, 0, (int)luaLocalFuncNameList.count(), 0);
            ZFImpl_ZFLua_execute_errorHandle(L, error, zfnull, logTag.cString());
        }

        // restore upvalue
        for(zfindex i = 0; i < callbackOwner->upvalues.count(); ++i)
        {
            _ZFP_I_ZFCallbackForLuaAsyncValueHolder *holder = callbackOwner->upvalues[i];
            if(!_ZFP_ZFCallbackForLuaAsync_toUpvalue(holder, L, luaFuncIndex, luaLocalFuncNameList, luaLocalFuncIndex))
            {
                zfCoreCriticalMessageTrim("%s unable to restore upvalue: %s",
                    logTag.cString(),
                    holder->objectInfo().cString());
                return;
            }
        }

        // cleanup local path info
        lua_pop(L, (int)luaLocalFuncNameList.count());
    }

    // finally call, stack: [func, zfargs]
    int error = lua_pcall(L, 1, 0, 0);
    ZFImpl_ZFLua_execute_errorHandle(L, error, zfnull, logTag.cString());
}

// ============================================================
static int _ZFP_ZFCallbackForLuaAsync(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[ZFCallbackForLuaAsync] expect one param, got %zi param",
            (zfindex)count);
    }
    if(!lua_isfunction(L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[ZFCallbackForLuaAsync] takes function(zfargs) as param, got: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
    }

    zfstring errorHint;
    zfautoObject ret;
    if(!ZFImpl_ZFLua_ZFCallbackForLuaAsync(ret, L, 1, &errorHint))
    {
        return ZFImpl_ZFLua_luaError(L, "%s", errorHint.cString());
    }
    else
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCallbackForLuaAsync, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCallbackForLuaAsync", _ZFP_ZFCallbackForLuaAsync);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

