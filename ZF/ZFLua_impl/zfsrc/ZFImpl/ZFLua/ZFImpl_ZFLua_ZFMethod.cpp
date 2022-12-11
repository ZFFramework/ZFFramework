#include "ZFImpl_ZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(ZF_IN const ZFCoreArrayPOD<lua_State *> &luaStateList,
                                                         ZF_IN const ZFCoreArrayPOD<const ZFMethod *> &methodList)
{
    zfstring code;
    if(methodList.count() >= 100)
    {
        code.capacity(10000);
    }
    else if(methodList.count() >= 10)
    {
        code.capacity(1000);
    }
    for(zfindex i = 0; i < methodList.count(); ++i)
    {
        const ZFMethod *method = methodList[i];
        if(!method->methodIsFunctionType()
            || method->methodNamespace() != zfnull
            || method->methodIsInternalPrivate()
        ) {
            continue;
        }
        zfstringAppend(code,
                "function %s(...) return zfl_call(zfnull, '%s', ...) end\n"
            , method->methodName(), method->methodName());
        if(i != 0 && (i % 100) == 0)
        {
            for(zfindex iL = 0; iL < luaStateList.count(); ++iL)
            {
                ZFImpl_ZFLua_execute(luaStateList[iL], code);
            }
            code.removeAll();
        }
    }
    if(!code.isEmpty())
    {
        for(zfindex iL = 0; iL < luaStateList.count(); ++iL)
        {
            ZFImpl_ZFLua_execute(luaStateList[iL], code);
        }
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFMethod, ZFM_EXPAND({
        ZFCoreArrayPOD<lua_State *> luaStateList;
        luaStateList.add(L);
        ZFCoreArrayPOD<const zfchar *> scopeNameList;

        ZFCoreArrayPOD<const ZFMethod *> allMethod = ZFMethodFuncGetAll();
        zfstlmap<zfstringRO, zfbool> methodNamespaceList;
        if(!allMethod.isEmpty())
        {
            _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
                luaStateList,
                allMethod);
            for(zfindex i = 0; i < allMethod.count(); ++i)
            {
                const ZFMethod *method = allMethod[i];
                if(method->methodNamespace() != zfnull)
                {
                    zfindex dotPos = zfstringFind(method->methodNamespace(), zfindexMax(), ZFNamespaceSeparator());
                    if(dotPos == zfindexMax())
                    {
                        methodNamespaceList[method->methodNamespace()] = zftrue;
                    }
                    else
                    {
                        methodNamespaceList[zfstring(method->methodNamespace(), dotPos).cString()] = zftrue;
                    }
                }
            }

            if(!methodNamespaceList.empty())
            {
                scopeNameList.capacity(scopeNameList.capacity() + (zfindex)methodNamespaceList.size());
                zfindex i = 0;
                for(zfstlmap<zfstringRO, zfbool>::iterator it = methodNamespaceList.begin(); it != methodNamespaceList.end(); ++it)
                {
                    scopeNameList.add(it->first);
                    ++i;
                }
            }
        }

        ZFImpl_ZFLua_implSetupScope(luaStateList, scopeNameList);
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedMethod != zfnull && data.changeType == ZFClassDataChangeTypeAttach)
        {
            if(data.changedMethod->methodIsFunctionType())
            {
                ZFCoreArrayPOD<lua_State *> stateList;
                stateList.add(L);
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayPODCreate(const zfchar *, data.changedMethod->methodNamespace()));

                _ZFP_ZFImpl_ZFLua_ZFMethod_setupGlobalMethod(
                    stateList,
                    ZFCoreArrayPODCreate(const ZFMethod *, data.changedMethod));
            }
        }
    }))

ZF_NAMESPACE_GLOBAL_END

