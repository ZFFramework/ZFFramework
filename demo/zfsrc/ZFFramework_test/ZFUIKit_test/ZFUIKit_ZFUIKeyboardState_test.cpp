#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZFUIKeyboardState_test) {
    this->keyPressedOnUpdateListener = ZFCallbackForFunc(zfself::keyPressedOnUpdate);
    ZFGlobalObserver().observerAdd(ZFUIKeyboardState::E_KeyPressedOnUpdate(), this->keyPressedOnUpdateListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardState_test) {
    ZFGlobalObserver().observerRemove(ZFUIKeyboardState::E_KeyPressedOnUpdate(), this->keyPressedOnUpdateListener);
}
private:
    ZFListener keyPressedOnUpdateListener;
    static void keyPressedOnUpdate(ZF_IN const ZFArgs &zfargs) {
        ZFLogTrim() << "[ZFUIKeyboardState] " << zfargs.sender();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

