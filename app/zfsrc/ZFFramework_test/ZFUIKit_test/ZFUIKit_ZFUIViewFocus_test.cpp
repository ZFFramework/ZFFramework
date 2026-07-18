#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIViewFocusState_test) {
    this->listener = ZFCallbackForFunc(zfself::focusOnUpdate);
    ZFGlobalObserver().observerAdd(ZFUIView::E_ViewFocusOnUpdate(), this->listener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusState_test) {
    ZFGlobalObserver().observerRemove(ZFUIView::E_ViewFocusOnUpdate(), this->listener);
}
private:
    ZFListener listener;
    static void focusOnUpdate(ZF_IN const ZFArgs &zfargs) {
        ZFLogTrim() << "[ZFUIViewFocus]"
            << zfargs.sender()
            << " changed to " << zfargs.sender()->to<ZFUIView *>()->focused();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusState_test)
#endif

ZF_NAMESPACE_GLOBAL_END

