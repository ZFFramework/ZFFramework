#include "ZFUIMouseEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIMouseAction)
ZFENUM_DEFINE(ZFUIMouseButton)

ZFOBJECT_REGISTER(ZFUIMouseEvent)

void ZFUIMouseEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    ret += ZFUIMouseAction::EnumNameForValue(this->mouseAction);
    ret += " ";
    ZFUIPointToStringT(ret, this->mousePoint);

    if(this->mouseButton != ZFUIMouseButton::e_Left) {
        ret += " ";
        ret += ZFUIMouseButton::EnumNameForValue(this->mouseButton);
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
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseActionEnum, mouseAction)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIPoint, mousePoint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIMouseEvent, ZFUIMouseButtonEnum, mouseButton)

ZF_NAMESPACE_GLOBAL_END

