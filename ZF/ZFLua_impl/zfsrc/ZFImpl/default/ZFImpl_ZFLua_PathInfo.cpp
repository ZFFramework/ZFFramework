#include "ZFImpl_ZFLua_PathInfo.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef zfstlmap<lua_State *, zfbool> _ZFP_ZFImpl_ZFLua_PathInfoStateMapType;
typedef zfstlmap<zfstring, zfstring> _ZFP_ZFImpl_ZFLua_PathInfoMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implPathInfoData, ZFLevelZFFrameworkEssential) {
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnAttach(), ZFCallbackForFunc(zfself::luaStateOnAttach));
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(zfself::luaStateOnDetach));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implPathInfoData) {
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnAttach(), ZFCallbackForFunc(zfself::luaStateOnAttach));
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_LuaStateOnDetach(), ZFCallbackForFunc(zfself::luaStateOnDetach));
}
public:
    _ZFP_ZFImpl_ZFLua_PathInfoStateMapType stateMap; // <lua_State *, needUpdate>
    _ZFP_ZFImpl_ZFLua_PathInfoMapType pathInfoMap; // <luaFuncName, luaFuncBody>
    ZFCoreArray<zfstring> luaFuncNameList;
private:
    static void luaStateOnAttach(ZF_IN const ZFArgs &zfargs) {
        ZFCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
        lua_State *L = (lua_State *)const_cast<void *>(zfargs.param0()->to<v_zfptr *>()->zfv);
        d->stateMap[L] = zftrue;
    }
    static void luaStateOnDetach(ZF_IN const ZFArgs &zfargs) {
        ZFCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
        lua_State *L = (lua_State *)const_cast<void *>(zfargs.param0()->to<v_zfptr *>()->zfv);
        d->stateMap.erase(L);
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implPathInfoData)

// ============================================================
static void _ZFP_ZFImpl_ZFLua_implPathInfoSetup_escape(
        ZF_OUT zfstring &ret
        , ZF_IN const zfchar *p
        );
void ZFImpl_ZFLua_implPathInfoSetup(
        ZF_IN lua_State *L
        , ZF_OUT zfstring &ret
        , ZF_IN const ZFPathInfo &pathInfo
        , ZF_IN_OPT zfbool localMode /* = zftrue */
        ) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
    if(d->pathInfoMap.empty()) {
        return;
    }
    _ZFP_ZFImpl_ZFLua_PathInfoStateMapType::iterator itState = d->stateMap.find(L);
    if(itState == d->stateMap.end()) {
        return;
    }

    if(itState->second) {
        itState->second = zffalse;
        zfstring cmd;
        if(d->pathInfoMap.empty()) {
            cmd += "_ZFP_l=nil;";
        }
        else {
            cmd += "function _ZFP_l(zfl_l)";
            cmd += "    return ";
            zfbool first = zftrue;
            for(_ZFP_ZFImpl_ZFLua_PathInfoMapType::iterator it = d->pathInfoMap.begin(); it != d->pathInfoMap.end(); ++it) {
                if(first) {
                    first = zffalse;
                }
                else {
                    cmd += ',';
                }
                cmd += it->second;
            }
            cmd += "    ;";
            cmd += "end;";
        }
        ZFImpl_ZFLua_execute(L, cmd);
    }

    // no endl, to prevent native lua error from having wrong line number
    if(localMode) {
        ret += "local ";
        zfbool first = zftrue;
        for(_ZFP_ZFImpl_ZFLua_PathInfoMapType::iterator it = d->pathInfoMap.begin(); it != d->pathInfoMap.end(); ++it) {
            if(first) {
                first = zffalse;
            }
            else {
                ret += ',';
            }
            ret += it->first;
        }
        ret += '=';
    }
    else {
        ret += "return ";
    }

    if(pathInfo == zfnull) {
        ret += "_ZFP_l(nil);";
    }
    else {
        ret += "_ZFP_l(ZFPathInfo('";
        _ZFP_ZFImpl_ZFLua_implPathInfoSetup_escape(ret, pathInfo.pathType());
        ret += ZFSerializableKeyword_ZFPathInfo_separator;
        _ZFP_ZFImpl_ZFLua_implPathInfoSetup_escape(ret, pathInfo.pathData());
        ret += "'));";
    }
}
void _ZFP_ZFImpl_ZFLua_implPathInfoRegister(
        ZF_IN const zfstring &luaFuncName
        , ZF_IN const zfchar *luaFuncBody
        ) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
    for(_ZFP_ZFImpl_ZFLua_PathInfoStateMapType::iterator itState = d->stateMap.begin(); itState != d->stateMap.end(); ++itState) {
        itState->second = zftrue;
    }
    d->pathInfoMap[luaFuncName] = luaFuncBody;

    d->luaFuncNameList.removeAll();
    for(_ZFP_ZFImpl_ZFLua_PathInfoMapType::iterator it = d->pathInfoMap.begin(); it != d->pathInfoMap.end(); ++it) {
        d->luaFuncNameList.add(it->first);
    }
}
void _ZFP_ZFImpl_ZFLua_implPathInfoUnregister(ZF_IN const zfstring &luaFuncName) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
    d->pathInfoMap.erase(luaFuncName);
    for(_ZFP_ZFImpl_ZFLua_PathInfoStateMapType::iterator itState = d->stateMap.begin(); itState != d->stateMap.end(); ++itState) {
        itState->second = zftrue;
    }

    d->luaFuncNameList.removeAll();
    for(_ZFP_ZFImpl_ZFLua_PathInfoMapType::iterator it = d->pathInfoMap.begin(); it != d->pathInfoMap.end(); ++it) {
        d->luaFuncNameList.add(it->first);
    }
}

zfbool ZFImpl_ZFLua_implPathInfoExist(ZF_IN const zfchar *luaFuncName) {
    ZFCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_implPathInfoData) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData);
    return d->pathInfoMap.find(luaFuncName) != d->pathInfoMap.end();
}

const ZFCoreArray<zfstring> &ZFImpl_ZFLua_implPathInfoList(void) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_implPathInfoData)->luaFuncNameList;
}

// ============================================================
static void _ZFP_ZFImpl_ZFLua_implPathInfoSetup_escape(
        ZF_OUT zfstring &ret
        , ZF_IN const zfchar *p
        ) {
    const zfchar *pL = p;
    while(*p) {
        if(*p == '\'') {
            ret.append(pL, p - pL);
            pL = p + 1;
            ret += "\\'";
        }
        ++p;
    }
    if(p != pL) {
        ret.append(pL, p - pL);
    }
}

ZF_NAMESPACE_GLOBAL_END

