#include "ZFUIKeyEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKeyEvent)

void ZFUIKeyEvent::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    ret += "Key ";
    ret += v_ZFUIKeyAction::EnumNameForValue(this->keyAction);
    ret += " ";
    ret += v_ZFUIKeyCode::EnumNameForValue(this->keyCode);

    if(this->eventResolved()) {
        ret += " (resolved)";
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, zfidentity, keyId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, ZFUIKeyAction, keyAction)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, ZFUIKeyCode, keyCode)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, zfflags, keyCodeRaw)

ZF_NAMESPACE_GLOBAL_END

