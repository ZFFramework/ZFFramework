#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewCapture.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl_sys_SDL_Image.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewCaptureImpl_sys_SDL, ZFUIViewCapture, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "SDL_image")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual zfbool viewCapture(
            ZF_IN ZFUIView *view
            , ZF_IN_OUT ZFUIImage *image
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)view->nativeView();
        SDL_FRect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = nativeView->rect.w;
        rect.h = nativeView->rect.h;
        SDL_Surface *nativeImage = SDL_CreateSurface(rect.w, rect.h, ZFImpl_sys_SDL_PixelFormatPreferred());
        SDL_SetSurfaceBlendMode(nativeImage, SDL_BLENDMODE_BLEND);
        SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(nativeImage);
        ZFImpl_sys_SDL_RendererNotifyCreate(renderer);
        zffloat alpha = 1;
        for(ZFUIView *p = view->parent(); p != zfnull; p = p->parent()) {
            alpha *= p->alpha();
        }
        nativeView->render(renderer, rect, rect, alpha);
        image->nativeImage(ZFImpl_sys_SDL_Image::implCreate(nativeImage), zffalse);
        ZFImpl_sys_SDL_RendererNotifyDestroy(renderer);
        SDL_DestroyRenderer(renderer);
        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewCaptureImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

