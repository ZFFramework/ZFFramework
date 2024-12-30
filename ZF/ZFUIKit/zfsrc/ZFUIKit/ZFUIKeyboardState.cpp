#include "ZFUIKeyboardState.h"
#include "protocol/ZFProtocolZFUIKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKeyboardState)

ZFEVENT_REGISTER(ZFUIKeyboardState, KeyPressedOnUpdate)

ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFUIKeyboardState, instance, ZFLevelZFFrameworkHigh)

ZFMETHOD_DEFINE_1(ZFUIKeyboardState, zfbool, keyPressed
        , ZFMP_IN(ZFUIKeyCode, keyCode)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull) {
        return impl->keyPressed(keyCode);
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFUIKeyboardState, zfbool, keyPressedRaw
        , ZFMP_IN(zfflags, keyCodeRaw)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIKeyboardState);
    if(impl != zfnull) {
        return impl->keyPressedRaw(keyCodeRaw);
    }
    return zffalse;
}

void ZFUIKeyboardState::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    zfstring tmp;
    for(zfindex i = 0; i < v_ZFUIKeyCode::EnumCount(); ++i) {
        if(this->keyPressed((ZFUIKeyCode)v_ZFUIKeyCode::EnumValueAt(i))) {
            if(!tmp.isEmpty()) {
                tmp += ", ";
            }
            tmp += v_ZFUIKeyCode::EnumNameAt(i);
        }
    }
    if(!tmp.isEmpty()) {
        ret += ", pressed: ";
        ret += tmp;
    }
}

ZF_NAMESPACE_GLOBAL_END

