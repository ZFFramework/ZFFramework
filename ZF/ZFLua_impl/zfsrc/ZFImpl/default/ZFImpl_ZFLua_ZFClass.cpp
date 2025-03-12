#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFClass, ZFM_EXPAND({
        ZFCoreArray<const ZFClass *> allClass = ZFClassGetAll();
        for(zfindex i = 0; i < allClass.count(); ++i) {
            const ZFClass *cls = allClass[i];
            if(!cls->classIsInternalPrivate()) {
                helper.addGenericScope(cls->classNameFull());
                for(zfindex i = 0; i < cls->classAliasTo().count(); ++i) {
                    helper.addGenericScope(cls->classAliasTo()[i]);
                }
            }
        }
    }), ZFM_EXPAND({
    }), ZFM_EXPAND({
        if(data.changedClass != zfnull
                && !data.changedClass->classIsInternalPrivate()
                ) {
            if(data.changeType == ZFClassDataUpdateTypeAttach) {
                helper.addGenericScope(data.changedClass->classNameFull());
            }
            else if(data.changeType == ZFClassDataUpdateTypeClassAliasAttach) {
                helper.addGenericScope(data.name);
            }
        }
    }))

ZF_NAMESPACE_GLOBAL_END

