#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFLIB_ZFUIKit _ZFP_ZFUISysWindowEmbedNativeViewImpl : zfextends ZFObject, zfimplements ZFUISysWindowEmbedImpl
{
    ZFOBJECT_DECLARE(_ZFP_ZFUISysWindowEmbedNativeViewImpl, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUISysWindowEmbedImpl)

public:
    void *nativeParent;

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewAdd(
            sysWindow, this->nativeParent, sysWindow->rootView()->nativeView());
        nativeParentView = this->nativeParent;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindowEmbedNativeView)->nativeViewRemove(
            sysWindow, this->nativeParent, sysWindow->rootView()->nativeView());
    }

    virtual zfautoObjectT<ZFUISysWindow *> modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        return ZFUISysWindow::nativeWindowEmbedNativeView(this->nativeParent);
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        sysWindowToFinish->nativeWindowEmbedImplDestroy();
    }
};

// ============================================================
ZFMETHOD_DEFINE_1(ZFUISysWindow, zfautoObjectT<ZFUISysWindow *>, nativeWindowEmbedNativeView,
                  ZFMP_IN(void *, nativeParent))
{
    zfblockedAlloc(_ZFP_ZFUISysWindowEmbedNativeViewImpl, embedImpl);
    embedImpl->nativeParent = nativeParent;
    zfautoObjectT<ZFUISysWindow *> ret = ZFUISysWindow::nativeWindowEmbed(embedImpl);
    ZFUISysWindow *window = ret;
    embedImpl->notifyOnCreate(window, zfnull);
    embedImpl->notifyOnResume(window);
    return ret;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUISysWindowEmbedNativeViewAutoRemove, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUISysWindowEmbedNativeViewAutoRemove)
{
    while(!m.empty())
    {
        zfstlmap<zfstring, zfbool> tmp;
        tmp.swap(m);
        for(zfstlmap<zfstring, zfbool>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            const ZFMethod *method = ZFMethodFuncForName(zfnull, it->first);
            if(method != zfnull && method->methodIsUserRegister())
            {
                ZFMethodFuncUserUnregister(method);
            }
        }
    }
}
zfstlmap<zfstring, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowEmbedNativeViewAutoRemove)
ZFMETHOD_DEFINE_2(ZFUISysWindow, zfautoObjectT<ZFUISysWindow *>, nativeWindowEmbedNativeView,
                  ZFMP_IN(void *, nativeParent),
                  ZFMP_IN(const zfchar *, sysWindowName))
{
    if(!zfsIsEmpty(sysWindowName) && ZFMethodFuncForName(zfnull, sysWindowName) != zfnull)
    {
        zfCoreLog("window \"%s\" already registered", sysWindowName);
        return zfnull;
    }

    zfautoObjectT<ZFUISysWindow *> sysWindow = ZFUISysWindow::nativeWindowEmbedNativeView(nativeParent);
    if(sysWindow == zfnull || zfsIsEmpty(sysWindowName))
    {
        return sysWindow;
    }

    ZFMethodFuncUserRegister_0(method, {
            return invokerMethod->methodUserRegisterUserData()->to<ZFUISysWindow *>();
        }, "", ZFUISysWindow *, sysWindowName);
    method->methodUserRegisterUserData(sysWindow);

    // auto clean
    ZFLISTENER_1(sysWindowOnDealloc
            , zfstring, sysWindowName
            ) {
        ZFUISysWindow::nativeWindowEmbedNativeViewCleanup(sysWindowName);
    } ZFLISTENER_END(sysWindowOnDealloc)
    sysWindow->observerAdd(ZFObject::EventObjectBeforeDealloc(), sysWindowOnDealloc);

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUISysWindowEmbedNativeViewAutoRemove)->m[sysWindowName] = zftrue;
    return sysWindow;
}
ZFMETHOD_DEFINE_1(ZFUISysWindow, void, nativeWindowEmbedNativeViewCleanup,
                  ZFMP_IN(const zfchar *, sysWindowName))
{
    const ZFMethod *method = ZFMethodFuncForName(zfnull, sysWindowName);
    if(method != zfnull && method->methodIsUserRegister())
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUISysWindowEmbedNativeViewAutoRemove)->m.erase(sysWindowName);
        ZFMethodFuncUserUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

