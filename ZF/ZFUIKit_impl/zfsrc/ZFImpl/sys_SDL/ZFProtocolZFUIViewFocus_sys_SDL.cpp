#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_SDL, ZFUIViewFocus, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusable(ZF_IN ZFUIView *view,
                               ZF_IN zfbool viewFocusable)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(!viewFocusable && nativeView->sysWindow != zfnull && nativeView->sysWindow->viewFocused == nativeView)
        {
            nativeView->sysWindow->viewFocusChange(zfnull);
        }
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        return nativeView->sysWindow != zfnull && nativeView->sysWindow->viewFocused == nativeView;
    }
    virtual ZFUIView *viewFocusFind(ZF_IN ZFUIView *view)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeView->sysWindow != zfnull && nativeView->sysWindow->viewFocused != zfnull)
        {
            ZFImpl_sys_SDL_View *viewFocused = nativeView->sysWindow->viewFocused;
            while(nativeView != zfnull && nativeView != viewFocused)
            {
                nativeView = nativeView->parent;
            }
            if(nativeView != zfnull)
            {
                return viewFocused->ownerZFUIView;
            }
        }
        return zfnull;
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeView->sysWindow != zfnull)
        {
            nativeView->sysWindow->viewFocusChange(nativeView);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_SDL)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

