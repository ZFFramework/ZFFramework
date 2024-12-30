#include "ZFUIMouseEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIMouseAction)
ZFENUM_DEFINE(ZFUIMouseButton)

ZFOBJECT_REGISTER(ZFUIMouseEvent)

void ZFUIMouseEvent::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    ret += "Mouse ";
    ret += v_ZFUIMouseAction::EnumNameForValue(this->mouseAction);
    ret += " ";
    ZFUIPointToStringT(ret, this->mousePoint);

    if(this->mouseButton != ZFUIMouseButton::e_Left) {
        ret += " ";
        ret += v_ZFUIMouseButton::EnumNameForValue(this->mouseButton);
    }

    if(this->eventResolved()) {
        ret += " (resolved)";
    }
}

void ZFUIMouseEvent::eventOnApplyScale(ZF_IN zffloat scale) {
    zfsuper::eventOnApplyScale(scale);
    ZFUIPointApplyScaleT(this->mousePoint, this->mousePoint, scale);
}
void ZFUIMouseEvent::eventOnApplyScaleReversely(ZF_IN zffloat scale) {
    zfsuper::eventOnApplyScale(scale);
    ZFUIPointApplyScaleReverselyT(this->mousePoint, this->mousePoint, scale);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, zfidentity, mouseId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseAction, mouseAction)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIPoint, mousePoint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseButton, mouseButton)

ZF_NAMESPACE_GLOBAL_END

