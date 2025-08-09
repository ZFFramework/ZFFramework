#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"

#if ZF_ENV_sys_SDL

#include "SDL3_image/SDL_image.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFImpl_sys_SDL_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, ZFImpl_sys_SDL_View *view, zfindex depth, zfindex siblingIndex) {
    zfstringAppend(s, "|%2s ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i) {
        s += "| ";
    }

    {
        if(view->ownerZFUIView != zfnull) {
            s += view->ownerZFUIView->classData()->className();
        }
        else {
            zfstringAppend(s, "%s", view->implType());
        }

        zfstringAppend(s, " (%s, %s, %s, %s)"
                , (zfint)view->rect.x
                , (zfint)view->rect.y
                , (zfint)view->rect.w
                , (zfint)view->rect.h
            );
    }

    s += "\n";

    for(zfindex i = 0; i < view->children.count(); ++i) {
        _ZFP_ZFImpl_sys_SDL_viewTreePrint_recursive(s, view->children[i], depth + 1, i);
    }
}
void ZFImpl_sys_SDL_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN ZFImpl_sys_SDL_View *view
        ) {
    ret += "======================= SDL view tree ======================\n";
    if(view != zfnull) {
        _ZFP_ZFImpl_sys_SDL_viewTreePrint_recursive(ret, view, 0, 0);
    }
    ret += "----------------------- SDL view tree ----------------------\n";
}

zfbool ZFImpl_sys_SDL_SurfaceToOutput(
        ZF_IN const ZFOutput &callback
        , ZF_IN SDL_Surface *sdlSurface
        ) {
    return IMG_SavePNG_IO(sdlSurface, ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(callback), true);
}
zfbool ZFImpl_sys_SDL_TextureToOutput(
        ZF_IN const ZFOutput &callback
        , ZF_IN SDL_Texture *sdlTexture
        ) {
    float w, h;
    SDL_TextureAccess access = (SDL_TextureAccess)SDL_GetNumberProperty(SDL_GetTextureProperties(sdlTexture), SDL_PROP_TEXTURE_ACCESS_NUMBER, -1);
    if(!SDL_GetTextureSize(sdlTexture, &w, &h)) {
        return zffalse;
    }
    SDL_Renderer *sdlRenderer = ZFImpl_sys_SDL_mainRenderer();

    if(ZFBitTest(access, SDL_TEXTUREACCESS_TARGET)) {
        ZFImpl_sys_SDL_zfscopeRenderTarget(success, sdlRenderer, sdlTexture);
        SDL_Surface *sdlSurface = SDL_RenderReadPixels(sdlRenderer, zfnull);
        ZFImpl_sys_SDL_zfscopeDestroySurface(sdlSurface);
        return success
            && IMG_SavePNG_IO(sdlSurface, ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(callback), true)
            ;
    }
    else {
        SDL_Texture *tmp = SDL_CreateTexture(sdlRenderer, ZFImpl_sys_SDL_PixelFormatPreferred(), SDL_TEXTUREACCESS_TARGET, w, h);
        ZFImpl_sys_SDL_zfscopeDestroyTexture(tmp);
        {
            SDL_BlendMode blendMode;
            SDL_GetTextureBlendMode(sdlTexture, &blendMode);
            ZFImpl_sys_SDL_zfscopeRenderTarget(success, sdlRenderer, tmp);
            SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_NONE);
            SDL_RenderTexture(sdlRenderer, sdlTexture, zfnull, zfnull);
            SDL_SetTextureBlendMode(sdlTexture, blendMode);
        }
        sdlTexture = tmp;

        ZFImpl_sys_SDL_zfscopeRenderTarget(success, sdlRenderer, sdlTexture);
        SDL_Surface *sdlSurface = SDL_RenderReadPixels(sdlRenderer, zfnull);
        ZFImpl_sys_SDL_zfscopeDestroySurface(sdlSurface);
        return success
            && IMG_SavePNG_IO(sdlSurface, ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(callback), true)
            ;
    }
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUISysWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_SDL_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUISysWindow *sysWindow = zfargs.sender();
            zfstring s;
            ZFImpl_sys_SDL_viewTreePrintT(s, (ZFImpl_sys_SDL_View *)sysWindow->rootView()->nativeView());
            ZFLogTrim() << s;
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_SDL

