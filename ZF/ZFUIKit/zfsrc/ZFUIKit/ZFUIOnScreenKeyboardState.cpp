#include "ZFUIOnScreenKeyboardState.h"
#include "protocol/ZFProtocolZFUIOnScreenKeyboardState.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardState)

ZFEVENT_REGISTER(ZFUIOnScreenKeyboardState, KeyboardStateOnUpdate)

ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, zfanyT<ZFUIOnScreenKeyboardState>, instanceForSysWindow
        , ZFMP_IN_OPT(ZFUISysWindow *, sysWindow, zfnull)
        ) {
    if(sysWindow == zfnull) {
        sysWindow = ZFUISysWindow::mainWindow();
    }
    ZFUIOnScreenKeyboardState *ret = sysWindow->objectTag("_ZFP_ZFUIOnScreenKeyboardState");
    if(ret == zfnull) {
        zfobj<ZFUIOnScreenKeyboardState> tmp;
        sysWindow->objectTag("_ZFP_ZFUIOnScreenKeyboardState", tmp);
        tmp->_ZFP_ZFUIOnScreenKeyboardState_keyboardOwnerSysWindow = sysWindow;
        ret = tmp;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, zfanyT<ZFUIOnScreenKeyboardState>, instanceForView
        , ZFMP_IN_OPT(ZFUIView *, view, zfnull)
        ) {
    return ZFUIOnScreenKeyboardState::instanceForSysWindow(ZFUIWindow::sysWindowForView(view));
}

ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, zfanyT<ZFUISysWindow>, keyboardOwnerSysWindow) {
    return _ZFP_ZFUIOnScreenKeyboardState_keyboardOwnerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, zfbool, keyboardShowing) {
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing;
}
ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, const ZFUIRect &, keyboardFrame) {
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
}
ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, const ZFUIRect &, keyboardFramePrev) {
    return this->_ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev;
}

ZFMETHOD_DEFINE_1(ZFUIOnScreenKeyboardState, void, keyboardFixClientFrameT
        , ZFMP_OUT(ZFUIRect &, clientFrame)
        ) {
    zffloat scale = this->keyboardOwnerSysWindow()->rootView()->UIScaleFixed();
    ZFPROTOCOL_ACCESS(ZFUIOnScreenKeyboardState)->keyboardFixClientFrame(
        this,
        ZFUIRectApplyScale(this->keyboardFrame(), scale),
        clientFrame);
    ZFUIRectApplyScaleReverselyT(clientFrame, clientFrame, scale);
}
ZFMETHOD_DEFINE_0(ZFUIOnScreenKeyboardState, ZFUIRect, keyboardFixClientFrame) {
    ZFUIRect ret = ZFUIRectZero();
    this->keyboardFixClientFrameT(ret);
    return ret;
}

void ZFUIOnScreenKeyboardState::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsFromPointerT(ret, this->keyboardOwnerSysWindow());
    if(this->keyboardShowing()) {
        ret += " keyboardFrame: ";
        ZFUIRectToStringT(ret, this->keyboardFrame());
    }
    else {
        ret += " keyboardNotShow";
    }
}

ZF_NAMESPACE_GLOBAL_END

