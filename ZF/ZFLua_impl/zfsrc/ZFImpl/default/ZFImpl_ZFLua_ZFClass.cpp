#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFClass, ZFM_EXPAND({
        ZFCoreArray<const ZFClass *> allClass = ZFClassGetAll();
        if(!allClass.isEmpty()) {
            ZFCoreArray<zfstring> classNameList;
            classNameList.capacity(allClass.count());
            for(zfindex i = 0; i < allClass.count(); ++i) {
                const ZFClass *cls = allClass[i];
                if(!cls->classIsInternalPrivate() && !cls->classNamespace()) {
                    classNameList.add(cls->className());
                }
            }

            ZFImpl_ZFLua_implSetupScope(
                ZFCoreArrayCreate(lua_State *, L),
                classNameList);
        }
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedClass != zfnull && data.changedClass != zfnull
                && !data.changedClass->classIsInternalPrivate()
                && !data.changedClass->classNamespace()
                ) {
            ZFCoreArray<lua_State *> stateList;
            stateList.add(L);
            if(data.changeType == ZFClassDataUpdateTypeAttach) {
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayCreate(zfstring, data.changedClass->className()));
            }
            else if(data.changeType == ZFClassDataUpdateTypeClassAliasAttach) {
                ZFImpl_ZFLua_implSetupScope(
                    stateList,
                    ZFCoreArrayCreate(zfstring, data.name));
            }
        }
    }))

ZF_NAMESPACE_GLOBAL_END

