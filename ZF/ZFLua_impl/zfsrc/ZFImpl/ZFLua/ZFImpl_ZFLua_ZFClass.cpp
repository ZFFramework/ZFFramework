#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_ZFClass_classOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    const ZFClassDataChangeData &data = listenerData.param0<v_ZFClassDataChangeData *>()->zfv;
    if(data.changedClass != zfnull && data.changeType == ZFClassDataChangeTypeAttach
        && !data.changedClass->classIsPrivate()
        && data.changedClass->classNamespace() == zfnull)
    {
        ZFImpl_ZFLua_implSetupScope(
            ZFImpl_ZFLua_luaStateList(),
            ZFCoreArrayPODCreate(const zfchar *, data.changedClass->className()));
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFClass, {
        ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
        if(!allClass.isEmpty())
        {
            ZFCoreArrayPOD<const zfchar *> classNameList;
            classNameList.capacity(allClass.count());
            for(zfindex i = 0; i < allClass.count(); ++i)
            {
                const ZFClass *cls = allClass[i];
                if(!cls->classIsPrivate() && cls->classNamespace() == zfnull)
                {
                    classNameList.add(cls->className());
                }
            }

            ZFImpl_ZFLua_implSetupScope(
                ZFCoreArrayPODCreate(lua_State *, L),
                classNameList);
        }

        ZFClassDataChangeObserver.observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFClass_classOnChange));
    }, {
        ZFClassDataChangeObserver.observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFClass_classOnChange));
    })

ZF_NAMESPACE_GLOBAL_END

