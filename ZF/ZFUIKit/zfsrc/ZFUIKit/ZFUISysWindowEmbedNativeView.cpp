#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFLIB_ZFUIKit _ZFP_ZFUISysWindowEmbedNativeViewImpl : zfextend ZFObject, zfimplement ZFUISysWindowEmbedImpl {
    ZFOBJECT_DECLARE(_ZFP_ZFUISysWindowEmbedNativeViewImpl, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFUISysWindowEmbedImpl)

public:
    void *nativeParent;

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewAdd(
            sysWindow, this->nativeParent, sysWindow->rootView()->nativeView());
        nativeParentView = this->nativeParent;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) {
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewRemove(
            sysWindow, this->nativeParent, sysWindow->rootView()->nativeView());
    }

    virtual zfautoT<ZFUISysWindow> modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) {
        return ZFUISysWindow::nativeWindowEmbedNativeView(this->nativeParent);
    }
    virtual void modalWindowFinish(
            ZF_IN ZFUISysWindow *sysWindowOwner
            , ZF_IN ZFUISysWindow *sysWindowToFinish
            ) {
        sysWindowToFinish->nativeWindowEmbedImplDestroy();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFUISysWindowEmbedNativeViewImpl)

// ============================================================
ZFMETHOD_DEFINE_1(ZFUISysWindow, zfautoT<ZFUISysWindow>, nativeWindowEmbedNativeView
        , ZFMP_IN(void *, nativeParent)
        ) {
    zfobj<_ZFP_ZFUISysWindowEmbedNativeViewImpl> embedImpl;
    embedImpl->nativeParent = nativeParent;
    zfautoT<ZFUISysWindow> ret = ZFUISysWindow::nativeWindowEmbed(embedImpl);
    ZFUISysWindow *window = ret;
    embedImpl->notifyOnCreate(window, zfnull);
    embedImpl->notifyOnResume(window);
    return ret;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUISysWindowEmbedNativeViewAutoRemove, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUISysWindowEmbedNativeViewAutoRemove) {
    while(!m.empty()) {
        zfstlhashmap<zfstring, zfbool> tmp;
        tmp.swap(m);
        for(zfstlhashmap<zfstring, zfbool>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            const ZFMethod *method = ZFMethodFuncForName(zfnull, it->first);
            if(method != zfnull && method->isUserRegister()) {
                ZFMethodFuncUserUnregister(method);
            }
        }
    }
}
zfstlhashmap<zfstring, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowEmbedNativeViewAutoRemove)
ZFMETHOD_DEFINE_2(ZFUISysWindow, zfautoT<ZFUISysWindow>, nativeWindowEmbedNativeView
        , ZFMP_IN(void *, nativeParent)
        , ZFMP_IN(const zfstring &, sysWindowName)
        ) {
    if(sysWindowName && ZFMethodFuncForName(zfnull, sysWindowName) != zfnull) {
        ZFCoreLog("window \"%s\" already registered", sysWindowName);
        return zfnull;
    }

    zfautoT<ZFUISysWindow> sysWindow = ZFUISysWindow::nativeWindowEmbedNativeView(nativeParent);
    if(sysWindow == zfnull || !sysWindowName) {
        return sysWindow;
    }

    ZFMethodFuncUserRegister_0(method, {
            return invokerMethod->userRegisterUserData();
        }, "", ZFUISysWindow *, sysWindowName);
    method->userRegisterUserData(sysWindow);

    // auto clean
    ZFLISTENER_1(sysWindowOnDealloc
            , zfstring, sysWindowName
            ) {
        ZFUISysWindow::nativeWindowEmbedNativeViewCleanup(sysWindowName);
    } ZFLISTENER_END()
    sysWindow->observerAdd(ZFObject::E_ObjectBeforeDealloc(), sysWindowOnDealloc);

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUISysWindowEmbedNativeViewAutoRemove)->m[sysWindowName] = zftrue;
    return sysWindow;
}
ZFMETHOD_DEFINE_1(ZFUISysWindow, void, nativeWindowEmbedNativeViewCleanup
        , ZFMP_IN(const zfstring &, sysWindowName)
        ) {
    const ZFMethod *method = ZFMethodFuncForName(zfnull, sysWindowName);
    if(method != zfnull && method->isUserRegister()) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUISysWindowEmbedNativeViewAutoRemove)->m.erase(sysWindowName);
        ZFMethodFuncUserUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

