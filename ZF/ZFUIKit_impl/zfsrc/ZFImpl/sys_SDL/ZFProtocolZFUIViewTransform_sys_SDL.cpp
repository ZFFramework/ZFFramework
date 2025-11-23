#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_SDL, ZFUIViewTransform, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual ZFUITransformFlags transformAvailable(void) {
        return v_ZFUITransform::e_Transform2D;
    }

    virtual void viewTransform(ZF_IN ZFUIView *view) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        nativeView->viewTransformPrepare();
        nativeView->viewTransform->translateX = view->translateX() * view->UIScaleFixed();
        nativeView->viewTransform->translateY = view->translateY() * view->UIScaleFixed();
        nativeView->viewTransform->scaleX = view->scaleX();
        nativeView->viewTransform->scaleY = view->scaleY();
        nativeView->viewTransform->rotateZ = view->rotateZ();
        nativeView->renderRequest();
    }
    virtual void viewTransformReset(ZF_IN ZFUIView *view) {
        // nothing to do
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

