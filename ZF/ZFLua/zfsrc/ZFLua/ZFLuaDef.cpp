#include "ZFLuaDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFOutput> &, ZFLuaErrorCallbacks) {
    static ZFCoreArray<ZFOutput> d;
    return d;
}
void _ZFP_ZFLuaErrorOccurred(
        ZF_IN const ZFCallerInfo &callerInfo
        , ZF_IN const zfchar *text
        ) {
    if(!ZFLuaErrorCallbacks().isEmpty()) {
        zfstring s;
        if(callerInfo.callerInfoT(s)) {
            s += " ";
        }
        s += text;
        s += "\n";
        const ZFCoreArray<ZFOutput> &t = ZFLuaErrorCallbacks();
        for(zfindex i = 0; i < t.count(); ++i) {
            t[i].execute(s.cString(), s.length());
        }
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_debug_autoOutputError, ZFLevelZFFrameworkNormal) {
    this->debugCallback = ZFOutputDefault();
    ZFLuaErrorCallbacks().add(this->debugCallback);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_debug_autoOutputError) {
    ZFLuaErrorCallbacks().removeElement(this->debugCallback);
}
ZFOutput debugCallback;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_debug_autoOutputError)

ZF_NAMESPACE_GLOBAL_END

