#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_SDL, ZFUIViewFocus, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void focusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focusable
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(!focusable && nativeView->rootWindow != zfnull && nativeView->rootWindow->focused == nativeView) {
            nativeView->rootWindow->focus(zfnull);
        }
    }
    virtual zfbool focused(ZF_IN ZFUIView *view) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        return nativeView->rootWindow != zfnull && nativeView->rootWindow->focused == nativeView;
    }
    virtual ZFUIView *focusFind(ZF_IN ZFUIView *view) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeView->rootWindow != zfnull && nativeView->rootWindow->focused != zfnull) {
            ZFImpl_sys_SDL_View *focused = nativeView->rootWindow->focused;
            while(nativeView != zfnull && nativeView != focused) {
                nativeView = nativeView->parent;
            }
            if(nativeView != zfnull) {
                return focused->ownerZFUIView;
            }
        }
        return zfnull;
    }
    virtual void focusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focus
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        if(nativeView->rootWindow != zfnull) {
            nativeView->rootWindow->focus(nativeView);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

