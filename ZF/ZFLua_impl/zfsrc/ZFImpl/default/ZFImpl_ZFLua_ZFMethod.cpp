#include "ZFImpl_ZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
        ZF_IN const ZFCoreArray<lua_State *> &luaStateList
        , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
        ) {
    zfstring code;
    if(methodList.count() >= 100) {
        code.capacity(10000);
    }
    else if(methodList.count() >= 10) {
        code.capacity(1000);
    }
    for(zfindex i = 0; i < methodList.count(); ++i) {
        const ZFMethod *method = methodList[i];
        if(!method->isFuncType()
            || method->methodNamespace()
            || method->isInternalPrivate()
        ) {
            continue;
        }
        zfstringAppend(code,
                "function %s(...) return zfl_call(zfnull, '%s', ...) end\n"
            , method->methodName(), method->methodName());
        if(i != 0 && (i % 100) == 0) {
            for(zfindex iL = 0; iL < luaStateList.count(); ++iL) {
                ZFImpl_ZFLua_execute(luaStateList[iL], code);
            }
            code.removeAll();
        }
    }
    if(!code.isEmpty()) {
        for(zfindex iL = 0; iL < luaStateList.count(); ++iL) {
            ZFImpl_ZFLua_execute(luaStateList[iL], code);
        }
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFMethod, ZFM_EXPAND({
        ZFCoreArray<lua_State *> luaStateList;
        luaStateList.add(L);
        ZFCoreArray<zfstring> scopeNameList;

        ZFCoreArray<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
        zfstlmap<zfstring, zfbool> methodNamespaceList;
        if(!allMethod.isEmpty()) {
            _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
                luaStateList,
                allMethod);
            for(zfindex i = 0; i < allMethod.count(); ++i) {
                const ZFMethod *method = allMethod[i];
                if(method->methodNamespace()) {
                    zfindex dotPos = zfstringFind(method->methodNamespace(), zfindexMax(), ".");
                    if(dotPos == zfindexMax()) {
                        methodNamespaceList[method->methodNamespace()] = zftrue;
                    }
                    else {
                        methodNamespaceList[zfstring(method->methodNamespace(), dotPos)] = zftrue;
                    }
                }
            }

            if(!methodNamespaceList.empty()) {
                scopeNameList.capacity(scopeNameList.capacity() + (zfindex)methodNamespaceList.size());
                zfindex i = 0;
                for(zfstlmap<zfstring, zfbool>::iterator it = methodNamespaceList.begin(); it != methodNamespaceList.end(); ++it) {
                    scopeNameList.add(it->first);
                    ++i;
                }
            }
        }

        ZFImpl_ZFLua_implSetupScope(luaStateList, scopeNameList);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedMethod != zfnull && data.changeType == ZFClassDataUpdateTypeAttach) {
            if(data.changedMethod->isFuncType()) {
                ZFCoreArray<lua_State *> stateList;
                stateList.add(L);
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayCreate(zfstring, data.changedMethod->methodNamespace()));

                _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
                    stateList,
                    ZFCoreArrayCreate(const ZFMethod *, data.changedMethod));
            }
        }
    }))

ZF_NAMESPACE_GLOBAL_END

