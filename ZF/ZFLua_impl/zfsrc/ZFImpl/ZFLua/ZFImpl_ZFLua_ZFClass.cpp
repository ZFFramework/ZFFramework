#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFClass, ZFM_EXPAND({
        ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
        if(!allClass.isEmpty())
        {
            ZFCoreArrayPOD<const zfchar *> classNameList;
            classNameList.capacity(allClass.count());
            for(zfindex i = 0; i < allClass.count(); ++i)
            {
                const ZFClass *cls = allClass[i];
                if(!cls->classIsInternalPrivate() && cls->classNamespace() == zfnull)
                {
                    classNameList.add(cls->className());
                }
            }

            ZFImpl_ZFLua_implSetupScope(
                ZFCoreArrayPODCreate(lua_State *, L),
                classNameList);
        }
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedClass != zfnull && data.changedClass != zfnull
            && !data.changedClass->classIsInternalPrivate()
            && data.changedClass->classNamespace() == zfnull)
        {
            ZFCoreArrayPOD<lua_State *> stateList;
            stateList.add(L);
            if(data.changeType == ZFClassDataChangeTypeAttach)
            {
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayPODCreate(const zfchar *, data.changedClass->className()));
            }
            else if(data.changeType == ZFClassDataChangeTypeClassAliasAttach)
            {
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayPODCreate(const zfchar *, data.name));
            }
        }
    }))

ZF_NAMESPACE_GLOBAL_END

