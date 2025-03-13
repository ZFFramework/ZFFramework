/**
 * @file ZFImpl_ZFLua.h
 * @brief global header for lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_h_
#define _ZFI_ZFImpl_ZFLua_h_

#include "ZFLua.h"
#include "../ZFImpl_ZFLua_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief create new lua state
 *
 * how the impl works:
 * -  all attach steps can be registered by #ZFImpl_ZFLua_implSetupCallback_DEFINE,
 *   when #ZFImpl_ZFLua_luaStateAttach,
 *   all necessary register steps would be performed
 * -  in ZFLua, all types are wrapped by #zfauto
 *   (wrapped directly for ZFObject type,
 *   wrapped by #ZFTypeIdWrapper for non-ZFObject type)
 * -  class and topmost namespace are wrapped by lua raw string value as a global variable,
 *   and should be registered by #ZFImpl_ZFLua_ImplSetupHelper::addGlobalScope\n
 *   these things are equal:
 *   -  `MyClass.myFunc(param);`
 *   -  `zfl_call(zfnull, "MyClass.myFunc", param);`
 *   -  `local cls = MyClass; cls.myFunc(param);`
 * -  member functions are dispatched as `obj:myFunc(param)`,
 *   which equals to `obj.myFunc(obj, param)`
 * -  #zfauto type must be registered by #ZFImpl_ZFLua_implSetupMetatable,
 *   and all methods would be dispatched internally,
 *   you should not modify #zfauto's "__index" by other lua bind tools
 */
extern ZFLIB_ZFLua_impl void *ZFImpl_ZFLua_luaStateOpen(void);
/**
 * @brief see #ZFImpl_ZFLua_luaStateOpen
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L);

/**
 * @brief see #ZFImpl_ZFLua_luaStateOpen
 *
 * must not be attached more than one time
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L);
/**
 * @brief see #ZFImpl_ZFLua_luaStateAttach
 *
 * note, this method only detach L from ZFFramework,
 * all methods registered to L can not be undo
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L);

/**
 * @brief called when class data changed, for each lua state of each thread
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_classDataUpdate(
        ZF_IN lua_State *L
        , ZF_IN const ZFClassDataUpdateData &data
        );

// ============================================================
/** @brief see #ZFImpl_ZFLua_luaStateOpen */
zfclassLikePOD ZFLIB_ZFLua_impl ZFImpl_ZFLua_ImplSetupHelper {
public:
    /** @cond ZFPrivateDoc */
    explicit ZFImpl_ZFLua_ImplSetupHelper(ZF_IN lua_State *L);
    ~ZFImpl_ZFLua_ImplSetupHelper(void);
    /** @endcond */
public:
    /** @brief see #ZFImpl_ZFLua_luaStateOpen */
    void addGlobalScope(ZF_IN const zfstring &scope);
    /** @brief see #ZFImpl_ZFLua_luaStateOpen */
    void addGenericScope(ZF_IN const zfstring &genericScope);
    /** @brief see #ZFImpl_ZFLua_luaStateOpen */
    void addCustomCode(ZF_IN const zfstring &code);
private:
    /** @cond ZFPrivateDoc */
    ZFImpl_ZFLua_ImplSetupHelper(ZF_IN ZFImpl_ZFLua_ImplSetupHelper const &ref);
    ZFImpl_ZFLua_ImplSetupHelper &operator = (ZF_IN ZFImpl_ZFLua_ImplSetupHelper const &ref);
    void _commit(void);
    /** @endcond */
private:
    lua_State *_L;
    zfstring _code;
    void *_m;
};

/**
 * @brief setup metatable for zfauto in lua env
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_implSetupMetatable(
        ZF_IN_OUT lua_State *L
        , ZF_IN_OPT int metatableIndex = -1
        );

// ============================================================
typedef void (*_ZFP_ZFImpl_ZFLua_ImplSetupAttach)(
        ZF_IN_OUT lua_State *L
        , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper
        );
typedef void (*_ZFP_ZFImpl_ZFLua_ImplSetupDetach)(ZF_IN_OUT lua_State *L);
typedef void (*_ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate)(
        ZF_IN_OUT lua_State *L
        , ZF_IN const ZFClassDataUpdateData &data
        , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper
        );
extern ZFLIB_ZFLua_impl void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(
        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupAttach setupAttachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupDetach setupDetachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate setupClassDataUpdate
        );
extern ZFLIB_ZFLua_impl void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(
        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupAttach setupAttachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupDetach setupDetachCallback
        , ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupClassDataUpdate setupClassDataUpdate
        );

/**
 * @brief for impl to add attach and detach actions
 *
 * usage:
 * @code
 *   ZFImpl_ZFLua_implSetupCallback_DEFINE(YourSetupSig, {
 *           // your own attach action, callback proto type:
 *           //   void implSetupAttach(
 *           //           ZF_IN_OUT lua_State *L
 *           //           , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper
 *           //           );
 *       }, {
 *           // your own detach action, callback proto type:
 *           //   void implSetupDetach(ZF_IN_OUT lua_State *L);
 *       }, {
 *           // your own class data change action, callback proto type:
 *           //   void implSetupClassDataUpdate(
 *           //           ZF_IN_OUT lua_State *L
 *           //           , ZF_IN const ZFClassDataUpdateData &data
 *           //           , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper
 *           //           );
 *       })
 * @endcode
 */
#define ZFImpl_ZFLua_implSetupCallback_DEFINE(SetupSig, setupAttachAction, setupDetachAction, setupClassDataUpdate) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implSetupAction_##SetupSig, ZFLevelZFFrameworkLow) { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(zfself::implSetupAttach, zfself::implSetupDetach, zfself::implSetupClassDataUpdate); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implSetupAction_##SetupSig) { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(zfself::implSetupAttach, zfself::implSetupDetach, zfself::implSetupClassDataUpdate); \
    } \
    public: \
        static void implSetupAttach( \
                ZF_IN_OUT lua_State *L \
                , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper \
                ) { \
            setupAttachAction \
        } \
        static void implSetupDetach(ZF_IN_OUT lua_State *L) { \
            setupDetachAction \
        } \
        static void implSetupClassDataUpdate( \
                ZF_IN_OUT lua_State *L \
                , ZF_IN const ZFClassDataUpdateData &data \
                , ZF_IN_OUT ZFImpl_ZFLua_ImplSetupHelper &helper \
                ) { \
            setupClassDataUpdate \
        } \
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implSetupAction_##SetupSig)

// ============================================================
/**
 * @brief run lua code, L must be first initialized by #ZFImpl_ZFLua_luaStateAttach
 *
 * NOTE: the lua code can return any number of values (any type that #ZFImpl_ZFLua_toGeneric supports),
 * when more than one value returned,
 * the luaResult would holds a #v_ZFCoreArray
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_execute(
        ZF_IN lua_State *L
        , ZF_IN const zfchar *buf
        , ZF_IN_OPT zfindex bufLen = zfindexMax()
        , ZF_OUT_OPT zfauto *luaResult = zfnull
        , ZF_IN_OPT const ZFCoreArray<zfauto> *luaParams = zfnull
        , ZF_OUT_OPT zfstring *errHint = zfnull
        , ZF_IN_OPT const zfchar *chunkInfo = zfnull
        , ZF_IN_OPT const zfchar *srcInfo = zfnull
        );
/**
 * @brief error handler for #ZFImpl_ZFLua_execute, lua_pcall, etc
 */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_execute_errorHandle(
        ZF_IN lua_State *L
        , ZF_IN int error
        , ZF_OUT_OPT zfstring *errHint = zfnull
        , ZF_IN_OPT const zfchar *chunkInfo = zfnull
        , ZF_IN_OPT zfindex errorLine = zfindexMax()
        );

// ============================================================
// utils
/** @brief see #ZFImpl_ZFLua_luaObjectInfo */
extern ZFLIB_ZFLua_impl void ZFImpl_ZFLua_luaObjectInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool printLuaType = zffalse
        );
/**
 * @brief get raw lua object info
 */
inline zfstring ZFImpl_ZFLua_luaObjectInfo(
        ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool printLuaType = zffalse
        ) {
    zfstring ret;
    ZFImpl_ZFLua_luaObjectInfoT(ret, L, luaStackOffset, printLuaType);
    return ret;
}

/**
 * @brief get params from lua
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toObject(
        ZF_OUT zfauto &param
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        );

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  zfauto
 * -  any types that can be converted by #ZFImpl_ZFLua_toCallback
 * -  any type that can be converted to string by #ZFImpl_ZFLua_toString,
 *   result would be stored as #ZFDI_WrapperBase,
 *   and would be converted to #ZFTypeIdWrapper during function call
 * -  #ZFImpl_ZFLuaValue
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toGeneric(
        ZF_OUT zfauto &param
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  #v_ZFCallback
 * -  lua function, converted by ZFCallbackForLua (available in lua code only)
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toCallback(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua string
 * -  #v_zfstring
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT zfstring &s
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );
/** @brief see #ZFImpl_ZFLua_toString */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT zfstring &s
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );

/** @brief see #ZFImpl_ZFLua_toString */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT const zfchar *&s
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );
/** @brief see #ZFImpl_ZFLua_toString */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toString(
        ZF_IN_OUT const zfchar *&s
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toNumberT(
        ZF_OUT zfauto &ret
        , ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );
/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua number
 * -  all types that #ZFDI_toNumberT support
 *
 * return proper #v_zfdouble if success, or empty if fail
 */
inline zfauto ZFImpl_ZFLua_toNumber(
        ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        ) {
    zfauto ret;
    ZFImpl_ZFLua_toNumberT(ret, L, luaStackOffset, allowEmpty, holderCls);
    return ret;
}

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toNumberT(
        ZF_OUT zfauto &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        , ZF_OUT_OPT const ZFClass **holderCls = zfnull
        );

/**
 * @brief convert native type to lua type
 *
 * support all types that can be converted by:
 * -  #ZFImpl_ZFLua_toString
 * -  #ZFImpl_ZFLua_toNumber
 * -  #ZFImpl_ZFLuaValue
 *
 * if success, push result value to top of lua's stack and return true,
 * otherwise, push nothing and return false\n
 * if allowEmpty, push 0 number if obj is null
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_toLuaValue(
        ZF_IN lua_State *L
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool allowEmpty = zffalse
        );

/**
 * @brief append with va_arg, see #ZFLuaExecute
 */
extern ZFLIB_ZFLua_impl zfbool ZFImpl_ZFLua_zfstringAppend(
        ZF_IN lua_State *L
        , ZF_IN_OUT zfstring &s
        , ZF_IN_OPT int luaParamOffset = 1
        );

/**
 * @brief return info of current stack
 */
extern ZFLIB_ZFLua_impl zfstring ZFImpl_ZFLua_luaStackInfo(
        ZF_IN lua_State *L
        , ZF_IN_OPT int luaStackOffset = 1
        );

// ============================================================
/**
 * @brief raw lua value created by #ZFImpl_ZFLua_toGeneric,
 *   and can be converted by #ZFImpl_ZFLua_toLuaValue
 */
zfclass ZFLIB_ZFLua_impl ZFImpl_ZFLuaValue : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFImpl_ZFLuaValue, ZFObject)

public:
    lua_State *L; /**< @brief the lua state */
    int luaValue; /**< @brief the lua value ref */

public:
    zfoverride
    virtual zfbool objectIsInternal(void) {return zftrue;}
    zfoverride
    virtual zfbool objectIsInternalPrivate(void) {return zftrue;}
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->L = zfnull;
        this->luaValue = -1;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(L != zfnull) {
            luaL_unref(L, LUA_REGISTRYINDEX, luaValue);
            L = zfnull;
        }
        luaValue = -1;
        zfsuper::objectOnDeallocPrepare();
    }
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        zfsuper::objectInfoImplAppend(ret);
        if(L != zfnull && luaValue != -1) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, luaValue);
            ret += " luaValue=";
            ZFImpl_ZFLua_luaObjectInfoT(ret, L, -1, zftrue);
            lua_pop(L, 1);
        }
    }
};

// ============================================================
// wrapper for impl
/** @brief util for impl */
inline lua_State *ZFImpl_ZFLua_luaOpen(void) {
    return ELuna::openLua();
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaClose(ZF_IN lua_State *L) {
    ELuna::closeLua(L);
}

/** @brief util for impl */
template<typename T>
inline void ZFImpl_ZFLua_luaClassRegister(
        ZF_IN lua_State *L
        , ZF_IN const zfchar *name
        ) {
    ELuna::registerClass<T>(L, name, ELuna::constructor<zfauto>);
}
/** @brief util for impl */
template<typename F>
inline void ZFImpl_ZFLua_luaFunctionRegister(
        ZF_IN lua_State *L
        , ZF_IN const zfchar *name
        , ZF_IN F f
        ) {
    ELuna::registerFunction(L, name, f);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaCFunctionRegister(
        ZF_IN lua_State *L
        , ZF_IN const zfchar *name
        , ZF_IN int (*f)(lua_State *)
        ) {
    lua_register(L, name, f);
}

/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(
        ZF_IN lua_State *L
        , ZF_IN zfauto &v
        ) {
    v_zfbool *t = v;
    if(t != zfnull) {
        lua_pushboolean(L, t->zfv);
        return;
    }
    ELuna::convert2LuaType<zfauto>::convertType(L, v);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(
        ZF_IN lua_State *L
        , ZF_IN const zfauto &v
        ) {
    zfauto t = v;
    ZFImpl_ZFLua_luaPush(L, t);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(
        ZF_IN lua_State *L
        , ZF_IN ZFObject *v
        ) {
    zfauto t = v;
    ZFImpl_ZFLua_luaPush(L, t);
}
/** @brief util for impl */
inline zfauto &ZFImpl_ZFLua_luaGet(
        ZF_IN lua_State *L
        , ZF_IN int luaStackOffset
        ) {
    return ELuna::convert2CppType<zfauto &>::convertType(L, luaStackOffset);
}
/**
 * @brief util for impl
 *
 * note: when calling this method without ZF_ENV_ZFLUA_USE_EXCEPTION defined,
 * you must ensure any cpp object has been properly cleaned up
 * (from nearest lua c function registered by #ZFImpl_ZFLua_luaCFunctionRegister, to this method),
 * otherwise all of the cpp object's destructor would be skipped due to
 * usage of longjmp instead of exception handler\n
 * \n
 * a typical recommended use case:
 * @code
 *   static int myLuaCallback(ZF_IN lua_State *L) {
 *       // must placed at top of lua c function registered by ZFImpl_ZFLua_luaCFunctionRegister
 *       ZFImpl_ZFLua_luaErrorPrepare(L);
 *
 *       ... // your code
 *
 *       if(error) {
 *           return ZFImpl_ZFLua_luaError(L, xxx);
 *       }
 *
 *       ...
 *   }
 *   ZFImpl_ZFLua_luaCFunctionRegister(L, "myLuaCallback", myLuaCallback);
 * @endcode
 */
#define ZFImpl_ZFLua_luaErrorPrepare(L) \
    _ZFP_ZFImpl_ZFLua_luaErrorPrepare _ZFP_ZFImpl_ZFLua_luaErrorPrepareNotCalled(L)
zfclassNotPOD ZFLIB_ZFLua_impl _ZFP_ZFImpl_ZFLua_luaErrorPrepare {
public:
    _ZFP_ZFImpl_ZFLua_luaErrorPrepare(ZF_IN lua_State *L)
    : L(L)
    , errorHint()
    {
    }
    ~_ZFP_ZFImpl_ZFLua_luaErrorPrepare(void) {
        if(this->errorHint) {
            lua_Debug ar;
            for(int iStack = 1; ; ++iStack) {
                int success = lua_getstack(L, iStack, &ar);
                if(!success) {
                    break;
                }
                success = lua_getinfo(L, "nSltu", &ar);
                if(!success) {
                    break;
                }
                if((iStack == 2 || ar.name == NULL) && ar.currentline > 0) {
                    // pass special header to ZFImpl_ZFLua_execute_errorHandle
                    this->errorHint.insert(0, zfstr("<{%s}>", (zfindex)ar.currentline));
                    break;
                }
            }

            luaL_error(L, "%s", this->errorHint.cString());
        }
    }
public:
    int luaError(
            ZF_IN lua_State *L
            , ZF_IN const zfchar *text
            ) {
        ZFCoreAssert(this->errorHint == zfnull);
        this->errorHint += text;
        return 0;
    }
private:
    lua_State *L;
    zfstring errorHint;
};

/** @brief see #ZFImpl_ZFLua_luaErrorPrepare */
#define ZFImpl_ZFLua_luaError(L, fmt, ...) \
    _ZFP_ZFImpl_ZFLua_luaErrorPrepareNotCalled.luaError(L, zfstr(fmt, ##__VA_ARGS__))

zfclassLikePOD ZFLIB_ZFLua_impl _ZFP_ZFLuaStackChecker {
public:
    lua_State *L;
    int stackChange;
    int stackSaved;
public:
    _ZFP_ZFLuaStackChecker(
            ZF_IN lua_State *L
            , ZF_IN_OPT int stackChange = 0
            )
    : L(L), stackChange(stackChange), stackSaved(lua_gettop(L))
    {
    }
    ~_ZFP_ZFLuaStackChecker(void) {
        int cur = lua_gettop(L);
        ZFCoreAssertWithMessageTrim(
            cur == stackSaved + stackChange,
            "stack messed up: %s(%s) => %s",
            stackSaved, stackChange, cur);
    }
};
/** @brief util to debug lua stack, assert fail if stack messed up */
#if ZF_ENV_DEBUG
    #define ZFImpl_ZFLua_DEBUG_luaStackChecker(name, L, stackChange) \
        _ZFP_ZFLuaStackChecker _DEBUG_luaStackChecker_##name(L, stackChange)
#else
    #define ZFImpl_ZFLua_DEBUG_luaStackChecker(name, L, stackChange)
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_h_

