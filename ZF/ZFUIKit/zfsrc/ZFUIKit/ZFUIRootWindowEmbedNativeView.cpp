#include "ZFUIRootWindow.h"
#include "protocol/ZFProtocolZFUIRootWindowEmbedNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFLIB_ZFUIKit _ZFP_ZFUIRootWindowEmbedNativeViewImpl : zfextend ZFObject, zfimplement ZFUIRootWindowEmbedImpl {
    ZFOBJECT_DECLARE(_ZFP_ZFUIRootWindowEmbedNativeViewImpl, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFUIRootWindowEmbedImpl)

public:
    void *nativeParent;

    // ============================================================
public:
    zfoverride
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    zfoverride
    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        ZFPROTOCOL_ACCESS(ZFUIRootWindowEmbedNativeView)->nativeViewAdd(
            rootWindow, this->nativeParent, rootWindow->rootView()->nativeView());
        nativeParentView = this->nativeParent;
    }
    zfoverride
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
        ZFPROTOCOL_ACCESS(ZFUIRootWindowEmbedNativeView)->nativeViewRemove(
            rootWindow, this->nativeParent, rootWindow->rootView()->nativeView());
    }

    zfoverride
    virtual zfautoT<ZFUIRootWindow> modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
        return ZFUIRootWindow::nativeWindowEmbedNativeView(this->nativeParent);
    }
    zfoverride
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) {
        toHide->nativeWindowEmbedImplDestroy();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFUIRootWindowEmbedNativeViewImpl)

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIRootWindow, zfautoT<ZFUIRootWindow>, nativeWindowEmbedNativeView
        , ZFMP_IN(void *, nativeParent)
        ) {
    zfobj<_ZFP_ZFUIRootWindowEmbedNativeViewImpl> embedImpl;
    embedImpl->nativeParent = nativeParent;
    zfautoT<ZFUIRootWindow> ret = ZFUIRootWindow::nativeWindowEmbed(embedImpl);
    ZFUIRootWindow *window = ret;
    embedImpl->notifyOnCreate(window, zfnull);
    embedImpl->notifyOnResume(window);
    return ret;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIRootWindowEmbedNativeViewAutoRemove, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIRootWindowEmbedNativeViewAutoRemove) {
    while(!m.isEmpty()) {
        ZFCoreSet<zfstring> tmp;
        tmp.swap(m);
        for(zfiter it = tmp.iter(); it; ++it) {
            const ZFMethod *method = ZFMethodFuncForName(zfnull, tmp.iterValue(it));
            if(method != zfnull && method->isUserRegister()) {
                ZFMethodFuncUserUnregister(method);
            }
        }
    }
}
ZFCoreSet<zfstring> m;
ZF_GLOBAL_INITIALIZER_END(ZFUIRootWindowEmbedNativeViewAutoRemove)
ZFMETHOD_DEFINE_2(ZFUIRootWindow, zfautoT<ZFUIRootWindow>, nativeWindowEmbedNativeView
        , ZFMP_IN(void *, nativeParent)
        , ZFMP_IN(const zfstring &, rootWindowName)
        ) {
    if(rootWindowName && ZFMethodFuncForName(zfnull, rootWindowName) != zfnull) {
        ZFCoreLog("window \"%s\" already registered", rootWindowName);
        return zfnull;
    }

    zfautoT<ZFUIRootWindow> rootWindow = ZFUIRootWindow::nativeWindowEmbedNativeView(nativeParent);
    if(rootWindow == zfnull || !rootWindowName) {
        return rootWindow;
    }

    ZFMethodFuncUserRegister_0(method, {
            return invokerMethod->userRegisterUserData();
        }, "", ZFUIRootWindow *, rootWindowName);
    method->userRegisterUserData(rootWindow);

    // auto clean
    ZFLISTENER_1(rootWindowOnDealloc
            , zfstring, rootWindowName
            ) {
        ZFUIRootWindow::nativeWindowEmbedNativeViewCleanup(rootWindowName);
    } ZFLISTENER_END()
    rootWindow->observerAdd(ZFObject::E_ObjectBeforeDealloc(), rootWindowOnDealloc);

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIRootWindowEmbedNativeViewAutoRemove)->m.add(rootWindowName);
    return rootWindow;
}
ZFMETHOD_DEFINE_1(ZFUIRootWindow, void, nativeWindowEmbedNativeViewCleanup
        , ZFMP_IN(const zfstring &, rootWindowName)
        ) {
    const ZFMethod *method = ZFMethodFuncForName(zfnull, rootWindowName);
    if(method != zfnull && method->isUserRegister()) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIRootWindowEmbedNativeViewAutoRemove)->m.remove(rootWindowName);
        ZFMethodFuncUserUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

