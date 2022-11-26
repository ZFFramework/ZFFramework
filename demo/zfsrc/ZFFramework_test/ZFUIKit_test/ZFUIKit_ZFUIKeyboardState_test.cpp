#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZFUIKeyboardState_test)
{
    this->keyPressedOnChangeListener = ZFCallbackForFunc(zfself::keyPressedOnChange);
    ZFGlobalObserver().observerAdd(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardState_test)
{
    ZFGlobalObserver().observerRemove(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
private:
    ZFListener keyPressedOnChangeListener;
    static void keyPressedOnChange(ZF_IN const ZFArgs &zfargs)
    {
        zfLogTrimT() << "[ZFUIKeyboardState]" << zfargs.sender();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

