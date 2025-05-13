#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// stack before:
//     [func, zfargs]
// stack after: (no lua return value)
//     []
// stack after: (has lua return value)
//     [ret]
static zfbool _ZFP_ZFCallbackForLua_invoke(
        ZF_IN lua_State *L
        , ZF_IN const ZFArgs &zfargs
        , ZF_IN const ZFPathInfo &ownerPathInfo
        , ZF_IN_OUT zfstring &errorHint
        ) {
    int stackCountPrev = lua_gettop(L) - 2;
    int error = lua_pcall(L, 1, LUA_MULTRET, 0);
    if(error != 0) {
        ZFImpl_ZFLua_execute_errorHandle(L
                , error
                , &errorHint
                , ownerPathInfo ? ZFPathInfoToString(ownerPathInfo).cString() : zfnull
                );
        return zffalse;
    }
    int stackCount = lua_gettop(L);
    if(stackCount > stackCountPrev) {
        if(stackCount == stackCountPrev + 1) {
            zfauto result;
            ZFImpl_ZFLua_toGeneric(result, L, -1);
            lua_pop(L, 1);

            if(result) {
                if(zfargs.ownerMethod()) {
                    if(zfargs.ownerMethod()->returnTypeId() == ZFTypeId_void()) {
                        result = zfnull;
                    }
                    else {
                        zfauto resultTmp;
                        zfstring errorHintTmp;
                        if(!ZFDI_implicitConvertT(resultTmp, zfargs.ownerMethod()->returnTypeId(), result, &errorHintTmp)) {
                            zfstringAppend(errorHint, "[%s] invalid return value, desired type: %s, got value: %s, reason: %s, for method: %s"
                                    , ownerPathInfo ? ZFPathInfoToString(ownerPathInfo) : zftext("ZFCallbackForLua")
                                    , zfargs.ownerMethod()->returnTypeId()
                                    , result
                                    , errorHintTmp
                                    , zfargs.ownerMethod()
                                    );
                            return zffalse;
                        }
                        result = resultTmp;
                    }
                }
                zfargs.result(result);
            }
        }
        else {
            lua_pop(L, stackCount - stackCountPrev);
            zfstringAppend(errorHint, "[%s] only 0 or 1 return value is allowed for ZFCallbackForLua, got: %s%s"
                    , ownerPathInfo ? ZFPathInfoToString(ownerPathInfo) : zftext("ZFCallbackForLua")
                    , stackCount - stackCountPrev
                    , zfargs.ownerMethod() ? zfstr(", for method: %s", zfargs.ownerMethod()) : zfstring()
                    );
            return zffalse;
        }
    }
    return zftrue;
}

// ============================================================
zfclassLikePOD _ZFP_ZFCallbackForLua_SyncMode {
protected:
    typedef _ZFP_ZFCallbackForLua_SyncMode zfself;

public:
    ZFPathInfo ownerPathInfo;
    zfweakT<ZFThread> ownerThread;
    lua_State *ownerL;
    int luaFunc;

public:
    _ZFP_ZFCallbackForLua_SyncMode(void)
    : ownerThread()
    , ownerL(zfnull)
    , luaFunc(0)
    {
    }
    ~_ZFP_ZFCallbackForLua_SyncMode(void) {
        this->removeAll();
    }

public:
    void removeAll(void) {
        this->ownerPathInfo.removeAll();
        if(ownerL != zfnull) {
            luaL_unref(ownerL, LUA_REGISTRYINDEX, luaFunc);
            ownerL = zfnull;
        }
    }

public:
    zfstring logTag(void) {
        if(this->ownerPathInfo.isEmpty()) {
            return "ZFCallbackForLua";
        }
        else {
            return ZFPathInfoToString(this->ownerPathInfo);
        }
    }

    void luaStateOnDetach(ZF_IN lua_State *L) {
        if(L == this->ownerL) {
            this->removeAll();
        }
    }
    zfbool setup(
            ZF_IN lua_State *L
            , ZF_IN int luaStackOffset
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_IN const ZFPathInfo &ownerPathInfo
            ) {
        this->ownerPathInfo = ownerPathInfo;
        this->ownerL = L;
        lua_pushvalue(this->ownerL, luaStackOffset);
        this->luaFunc = luaL_ref(this->ownerL, LUA_REGISTRYINDEX);
        if(ZFLogLevelIsActive(v_ZFLogLevel::e_Debug) && ZFThread::implAvailable()) {
            ZFCoreMutexLocker();
            this->ownerThread = ZFThread::currentThread();
        }
        return zftrue;
    }
    void invoke(
            ZF_IN lua_State *L
            , ZF_IN const ZFArgs &zfargs
            ) {
        ZFImpl_ZFLua_luaErrorPrepare(L);
        if(this->ownerL == zfnull) {
            return;
        }

        if(ZFLogLevelIsActive(v_ZFLogLevel::e_Debug) && ZFThread::implAvailable()) {
            ZFCoreMutexLocker();
            if(this->ownerThread) {
                ZFThread *curThread = ZFThread::currentThread();
                if(curThread != zfnull && curThread != this->ownerThread) {
                    ZFCoreCriticalMessageTrim(
                        "[%s] can not execute in different thread"
                        , this->logTag()
                        );
                    return;
                }
            }
        }

        lua_rawgeti(this->ownerL, LUA_REGISTRYINDEX, luaFunc);
        if(!lua_isfunction(this->ownerL, -1)) {
            ZFCoreCriticalMessageTrim(
                "[%s] invalid function: %s"
                , ZFImpl_ZFLua_luaObjectInfo(this->ownerL, -1, zftrue)
                , this->logTag()
                );
            return;
        }

        zfobj<v_ZFArgs> zfargsHolder;
        zfargsHolder->zfv = zfargs;
        ZFImpl_ZFLua_luaPush(this->ownerL, zfargsHolder);

        zfstring errorHint;
        if(!_ZFP_ZFCallbackForLua_invoke(this->ownerL, zfargs, this->ownerPathInfo, errorHint)) {
            ZFLuaErrorOccurredTrim("%s", errorHint);
            return;
        }
    }
};

// ============================================================
zfclassLikePOD _ZFP_ZFCallbackForLua_AsyncMode {
protected:
    typedef _ZFP_ZFCallbackForLua_AsyncMode zfself;

public:
    typedef enum {
        ValueType_Invalid,
        ValueType_ENV,
        ValueType_ZFObject,
        ValueType_localFunc,
        ValueType_bool,
        ValueType_int,
        ValueType_number,
        ValueType_string,
    } ValueType;

public:
    zfclassPOD ValueHolder {
    public:
        ValueType valueType;
        union {
            ZFObject *objectValue;
            zfchar *localFuncName;
            int boolValue;
            lua_Integer intValue;
            lua_Number numberValue;
            zfchar *stringValue;
        } v;
        int upvalueIndex;
    };

public:
    static void valueHolderCleanup(ZF_IN const ValueHolder &v) {
        switch(v.valueType) {
            case ValueType_Invalid:
                break;
            case ValueType_ENV:
                break;
            case ValueType_ZFObject:
                zfRelease(v.v.objectValue);
                break;
            case ValueType_localFunc:
                zffree(v.v.localFuncName);
                break;
            case ValueType_bool:
                break;
            case ValueType_int:
                break;
            case ValueType_number:
                break;
            case ValueType_string:
                zffree(v.v.stringValue);
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
    }
    static zfstring valueHolderInfo(ZF_IN const ValueHolder &v) {
        zfstring ret;
        switch(v.valueType) {
            case ValueType_Invalid:
                ret += "(Invalid)";
                break;
            case ValueType_ENV:
                ret += "(ENV)";
                break;
            case ValueType_ZFObject:
                ret += "(ZFObject)";
                ZFObjectInfoT(ret, v.v.objectValue);
                break;
            case ValueType_localFunc:
                ret += "(localFunc)";
                ret += v.v.localFuncName;
                break;
            case ValueType_bool:
                ret += "(bool)";
                zfboolToStringT(ret, (zfbool)v.v.boolValue);
                break;
            case ValueType_int:
                ret += "(int)";
                zfintToStringT(ret, (zfint)v.v.intValue);
                break;
            case ValueType_number:
                ret += "(number)";
                zfdoubleToStringT(ret, (zfdouble)v.v.numberValue);
                break;
            case ValueType_string:
                ret += "(string)";
                ret += v.v.stringValue;
                break;
            default:
                ret += "(Unknown)";
                zfintToStringT(ret, (zfint)v.valueType);
                break;
        }
        return ret;
    }

public:
    ZFPathInfo ownerPathInfo;
    zfstring func;
    zfbool funcReaderDone;
    ZFCoreArray<ValueHolder> upvalues;

public:
    ~_ZFP_ZFCallbackForLua_AsyncMode(void) {
        removeAll();
    }

public:
    void removeAll(void) {
        this->ownerPathInfo.removeAll();
        this->func.removeAll();
        if(!this->upvalues.isEmpty()) {
            ZFCoreArray<ValueHolder> tmp = this->upvalues;
            this->upvalues = ZFCoreArray<ValueHolder>();
            for(zfindex i = 0; i < tmp.count(); ++i) {
                valueHolderCleanup(tmp[i]);
            }
        }
    }

private:
    static int _funcWriter(lua_State *L, const void *p, size_t sz, void *ud) {
        zfself *owner = (zfself *)ud;
        owner->func.append(p, (zfindex)sz);
        return 0;
    }
    static const char *_funcReader(lua_State *L, void *data, size_t *size) {
        zfself *owner = (zfself *)data;
        if(owner->funcReaderDone) {
            return NULL;
        }
        owner->funcReaderDone = zftrue;
        *size = (size_t)owner->func.length();
        return owner->func.cString();
    }

private:
    // convert upvalue from top stack, pop one value
    static zfbool _fromUpvalue(
            ZF_OUT ValueHolder &ret
            , ZF_IN lua_State *L
            , ZF_IN const char *name
            , ZF_IN int upvalueIndex
            ) {
        ret.upvalueIndex = upvalueIndex;
        if(lua_isboolean(L, -1)) {
            ret.v.boolValue = lua_toboolean(L, -1);
            ret.valueType = ValueType_bool;
        }
        else if(lua_isinteger(L, -1)) {
            ret.v.intValue = lua_tointeger(L, -1);
            ret.valueType = ValueType_int;
        }
        else if(lua_isnumber(L, -1)) {
            ret.v.numberValue = lua_tonumber(L, -1);
            ret.valueType = ValueType_number;
        }
        else if(lua_isstring(L, -1)) {
            ret.v.stringValue = zfsCopy(lua_tostring(L, -1));
            ret.valueType = ValueType_string;
        }
        else if(lua_isuserdata(L, -1)) {
            zfauto obj;
            if(!ZFImpl_ZFLua_toObject(obj, L, -1)) {
                return zffalse;
            }
            ret.v.objectValue = zfRetain(obj);
            ret.valueType = ValueType_ZFObject;
        }
        else if(lua_isfunction(L, -1)) {
            if(!ZFImpl_ZFLua_implPathInfoExist(name)) {
                return zffalse;
            }
            ret.v.localFuncName = zfsCopy(name);
            ret.valueType = ValueType_localFunc;
        }
        else if(strcmp(name, "_ENV") == 0) {
            ret.valueType = ValueType_ENV;
        }
        else {
            return zffalse;
        }
        return zftrue;
    }
    // convert to upvalue and push to stack top
    static zfbool _ZFP_ZFCallbackForLua_toUpvalue(
            ZF_IN const ValueHolder &v
            , ZF_IN lua_State *L
            , ZF_IN int luaFuncIndex
            , ZF_IN const ZFCoreArray<zfstring> &luaLocalFuncNameList
            , ZF_IN int luaLocalFuncIndex
            ) {
        switch(v.valueType) {
            case ValueType_Invalid:
                return zffalse;
            case ValueType_ENV:
                lua_getglobal(L, "_G");
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            case ValueType_ZFObject:
                ZFImpl_ZFLua_luaPush(L, v.v.objectValue);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            case ValueType_localFunc: {
                const zfchar *name = v.v.localFuncName;
                int offset = -1;
                for(zfindex i = 0; i < luaLocalFuncNameList.count(); ++i) {
                    if(zfstringIsEqual(name, luaLocalFuncNameList[i])) {
                        offset = (int)i;
                        break;
                    }
                }
                if(offset == zfindexMax()) {
                    return zffalse;
                }
                lua_pushvalue(L, luaLocalFuncIndex + offset);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            }
            case ValueType_bool:
                lua_pushboolean(L, v.v.boolValue);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            case ValueType_int:
                lua_pushinteger(L, v.v.intValue);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            case ValueType_number:
                lua_pushnumber(L, v.v.numberValue);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            case ValueType_string:
                lua_pushstring(L, v.v.stringValue);
                lua_setupvalue(L, luaFuncIndex < 0 ? luaFuncIndex - 1 : luaFuncIndex, v.upvalueIndex);
                return zftrue;
            default:
                return zffalse;
        }
    }

public:
    zfstring logTag(void) {
        if(this->ownerPathInfo.isEmpty()) {
            return "ZFCallbackForLua";
        }
        else {
            return ZFPathInfoToString(this->ownerPathInfo);
        }
    }

    void luaStateOnDetach(ZF_IN lua_State *L) {
        // nothing to do
    }

    zfbool setup(
            ZF_IN lua_State *L
            , ZF_IN int luaStackOffset
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_IN const ZFPathInfo &ownerPathInfo
            ) {
        this->ownerPathInfo = ownerPathInfo;

        // dump the function
        lua_pushvalue(L, luaStackOffset);
        int dumpError = lua_dump(L, _funcWriter, this, ZFLogLevelIsActive(v_ZFLogLevel::e_Debug) ? 0 : 1);
        lua_pop(L, 1);
        if(dumpError != 0) {
            zfstringAppend(errorHint
                    , "[%s] unable to dump function: %s"
                    , this->logTag()
                    , ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue)
                    );
            return zffalse;
        }

        // save upvalue
        for(int upvalueIndex = 1; ; ++upvalueIndex) {
            const char *name = lua_getupvalue(L, luaStackOffset, upvalueIndex);
            if(name == NULL) {
                break;
            }
            ValueHolder v;
            if(!_fromUpvalue(v, L, name, upvalueIndex)) {
                zfstringAppend(errorHint
                        , "[%s] unable to store upvalue \"%s\" at index %s: %s"
                        , this->logTag()
                        , name
                        , (zfint)upvalueIndex
                        , ZFImpl_ZFLua_luaObjectInfo(L, -1, zftrue)
                        );
                lua_pop(L, 1);
                return zffalse;
            }
            lua_pop(L, 1);
            this->upvalues.add(v);
        }

        return zftrue;
    }

    void invoke(
            ZF_IN lua_State *L
            , ZF_IN const ZFArgs &zfargs
            ) {
        ZFImpl_ZFLua_luaErrorPrepare(L);

        { // stack from empty to [func, zfargs]
            ZFCoreMutexLocker();
            ZFImpl_ZFLua_DEBUG_luaStackChecker(ck, L, 2);

            // restore function, stack: [func]
            this->funcReaderDone = zffalse;
            int loadError = lua_load(L, _funcReader, this, this->logTag().cString(), "b");
            if(loadError != LUA_OK) {
                ZFCoreCriticalMessageTrim(
                        "[%s] unable to load function"
                        , this->logTag()
                        );
                return;
            }
            int luaFuncIndex = lua_gettop(L);

            // set function args, stack: [func, zfargs]
            zfobj<v_ZFArgs> zfargsHolder;
            zfargsHolder->zfv = zfargs;
            ZFImpl_ZFLua_luaPush(L, zfargsHolder);

            // restore local path info, stack: [func, zfargs, localFunc0, localFunc1, ...]
            const ZFCoreArray<zfstring> &luaLocalFuncNameList = ZFImpl_ZFLua_implPathInfoList();
            int luaLocalFuncIndex = lua_gettop(L) + 1;
            {
                zfstring code;
                ZFImpl_ZFLua_implPathInfoSetup(L, code, this->ownerPathInfo, zffalse);
                int error = luaL_loadbuffer(L, code.cString(), code.length(), code.cString());
                ZFCoreAssert(error == 0);
                error = lua_pcall(L, 0, (int)luaLocalFuncNameList.count(), 0);
                ZFCoreAssert(error == 0);
            }

            // restore upvalue
            for(zfindex i = 0; i < this->upvalues.count(); ++i) {
                const ValueHolder &v = this->upvalues[i];
                if(!_ZFP_ZFCallbackForLua_toUpvalue(v, L, luaFuncIndex, luaLocalFuncNameList, luaLocalFuncIndex)) {
                    ZFCoreCriticalMessageTrim(
                            "[%s] unable to restore upvalue: %s"
                            , this->logTag()
                            , valueHolderInfo(v)
                            );
                    return;
                }
            }

            // cleanup local path info, stack: [func, zfargs]
            lua_pop(L, (int)luaLocalFuncNameList.count());
        }

        // finally call, stack: [func, zfargs]
        zfstring errorHint;
        if(!_ZFP_ZFCallbackForLua_invoke(L, zfargs, this->ownerPathInfo, errorHint)) {
            ZFLuaErrorOccurredTrim("%s", errorHint);
            return;
        }
    }
};

// ============================================================
zfclass _ZFP_I_ZFCallbackForLuaCallback : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFCallbackForLuaCallback, ZFObject)
    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })

private:
    ZFListener luaStateOnDetachListener;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->luaStateOnDetachListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, luaStateOnDetach));
        ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnDetach(), this->luaStateOnDetachListener);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnDetach(), this->luaStateOnDetachListener);
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    ZFPathInfo ownerPathInfo;
    _ZFP_ZFCallbackForLua_SyncMode syncMode;
    _ZFP_ZFCallbackForLua_AsyncMode asyncMode;

public:
    zfbool ownerPathInfoSetup(
            ZF_IN lua_State *L
            , ZF_OUT_OPT zfstring *errorHint
            ) {
        this->ownerPathInfo.removeAll();
        lua_Debug ar;
        for(int iStack = 1; !this->ownerPathInfo; ++iStack) {
            int success = lua_getstack(L, iStack, &ar);
            if(!success) {
                break;
            }
            for(int iLocal = 1; ; ++iLocal) {
                const char *varName = lua_getlocal(L, &ar, iLocal); // [var]
                if(varName == NULL) {
                    break;
                }
                if(strcmp(varName, "_ENV") == 0) {
                    lua_pushstring(L, "ZFLocalPathInfo"); // [var, 'ZFLocalPathInfo']
                    lua_rawget(L, -2); // [var, (function)ZFLocalPathInfo]
                    lua_pcall(L, 0, 1, 0); // [var, pathInfoObj]
                    zfauto pathInfoHolder;
                    ZFImpl_ZFLua_toObject(pathInfoHolder, L, -1);
                    lua_pop(L, 2); // []

                    v_ZFPathInfo *pathInfo = pathInfoHolder;
                    if(pathInfo != zfnull) {
                        this->ownerPathInfo = pathInfo->zfv;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                else if(strcmp(varName, "ZFLocalPathInfo") == 0) {
                    int error = lua_pcall(L, 0, 1, 0); // [pathInfoObj]
                    if(error != 0) {
                        lua_pop(L, 1);
                        zfstringAppend(errorHint,
                                "[%s] unable to obtain path info, invoke fail"
                                , this->logTag()
                                );
                        return zffalse;
                    }
                    zfauto pathInfoHolder;
                    if(!ZFImpl_ZFLua_toObject(pathInfoHolder, L, -1)) {
                        lua_pop(L, 1);
                        zfstringAppend(errorHint,
                                "[%s] unable to obtain path info"
                                , this->logTag()
                                );
                        return zffalse;
                    }
                    v_ZFPathInfo *pathInfo = pathInfoHolder;
                    if(pathInfo != zfnull) {
                        this->ownerPathInfo = pathInfo->zfv;
                    }
                    lua_pop(L, 1);
                    break;
                }
                else {
                    lua_pop(L, 1);
                }
            }
        }
        return zftrue;
    }

    zfstring logTag(void) {
        if(this->ownerPathInfo.isEmpty()) {
            return "ZFCallbackForLua";
        }
        else {
            return ZFPathInfoToString(this->ownerPathInfo);
        }
    }

private:
    void _cleanup(void) {
        ZFCoreMutexLocker();
        this->ownerPathInfo.removeAll();
        this->syncMode.removeAll();
        this->asyncMode.removeAll();
    }

private:
    ZFMETHOD_DECLARE_1(void, luaStateOnDetach
            , ZFMP_IN(const ZFArgs &, zfargs)
            )
    ZFMETHOD_DECLARE_1(void, callback
            , ZFMP_IN(const ZFArgs &, zfargs)
            )
};
ZFOBJECT_REGISTER(_ZFP_I_ZFCallbackForLuaCallback)

ZFMETHOD_DEFINE_1(_ZFP_I_ZFCallbackForLuaCallback, void, luaStateOnDetach
        , ZFMP_IN(const ZFArgs &, zfargs)
        ) {
    ZFCoreMutexLocker();
    v_zfptr *L = zfargs.param0();
    this->syncMode.luaStateOnDetach((lua_State *)const_cast<void *>(L->zfv));
    this->asyncMode.luaStateOnDetach((lua_State *)const_cast<void *>(L->zfv));
}

ZFMETHOD_DEFINE_1(_ZFP_I_ZFCallbackForLuaCallback, void, callback
        , ZFMP_IN(const ZFArgs &, zfargs)
        ) {
    lua_State *L = (lua_State *)ZFLuaState();
    if(L) {
        if(this->syncMode.ownerL == L) {
            this->syncMode.invoke(L, zfargs);
        }
        else {
            this->asyncMode.invoke(L, zfargs);
        }
    }
}

// ============================================================
zfbool ZFImpl_ZFLua_ZFCallbackForLua(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!lua_isfunction(L, luaStackOffset)) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint
                    , "[ZFCallbackForLua] invalid param: %s"
                    , ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue)
                    );
        }
        return zffalse;
    }

    ZFCoreMutexLocker();
    ZFImpl_ZFLua_DEBUG_luaStackChecker(ck, L, 0);

    zfobj<_ZFP_I_ZFCallbackForLuaCallback> callbackOwner;
    if(!callbackOwner->ownerPathInfoSetup(L, errorHint)
            || !callbackOwner->syncMode.setup(L, luaStackOffset, errorHint, callbackOwner->ownerPathInfo)
            || !callbackOwner->asyncMode.setup(L, luaStackOffset, errorHint, callbackOwner->ownerPathInfo)
            ) {
        return zffalse;
    }

    // result
    zfobj<v_ZFCallback> callback;
    callback->zfv = ZFCallbackForMemberMethod(callbackOwner, ZFMethodAccess(_ZFP_I_ZFCallbackForLuaCallback, callback));
    callback->zfv.callbackOwnerObjectRetain();
    ret = callback;
    return zftrue;
}

// ============================================================
static int _ZFP_ZFCallbackForLua(ZF_IN lua_State *L) {
    ZFImpl_ZFLua_luaErrorPrepare(L);

    int count = (int)lua_gettop(L);
    if(count != 1) {
        return ZFImpl_ZFLua_luaError(L
                , "[ZFCallbackForLua] expect one param, got %s param"
                , (zfindex)count
                );
    }
    if(!lua_isfunction(L, 1)) {
        return ZFImpl_ZFLua_luaError(L
                , "[ZFCallbackForLua] takes function(zfargs) as param, got: %s"
                , ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue)
                );
    }

    zfstring errorHint;
    zfauto ret;
    if(!ZFImpl_ZFLua_ZFCallbackForLua(ret, L, 1, &errorHint)) {
        return ZFImpl_ZFLua_luaError(L, "%s", errorHint);
    }
    else {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCallbackForLua, ZFM_EXPAND({
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCallbackForLua", _ZFP_ZFCallbackForLua);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
    }))

ZF_NAMESPACE_GLOBAL_END

