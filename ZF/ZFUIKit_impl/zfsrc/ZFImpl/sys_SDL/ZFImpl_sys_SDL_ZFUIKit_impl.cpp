#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"

#if ZF_ENV_sys_SDL

#include "SDL_image.h"

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
    return 0 == IMG_SavePNG_RW(sdlSurface, ZFImpl_sys_SDL_ZFOutputToSDL_RWops(callback), 1);
}
zfbool ZFImpl_sys_SDL_TextureToOutput(
        ZF_IN const ZFOutput &callback
        , ZF_IN SDL_Texture *sdlTexture
        ) {
    int width, height;
    if(SDL_QueryTexture(sdlTexture, zfnull, zfnull, &width, &height) != 0) {
        return zffalse;
    }
    SDL_Surface *sdlSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    ZFImpl_sys_SDL_zfblockedDestroySurface(sdlSurface);
    SDL_SetSurfaceBlendMode(sdlSurface, SDL_BLENDMODE_BLEND);

    SDL_Renderer *sdlRenderer = ZFImpl_sys_SDL_mainRenderer();
    ZFImpl_sys_SDL_zfblockedRenderTarget(success, sdlRenderer, sdlTexture);
    return success
        && SDL_RenderReadPixels(sdlRenderer, zfnull, sdlSurface->format->format, sdlSurface->pixels, sdlSurface->pitch) == 0
        && IMG_SavePNG_RW(sdlSurface, ZFImpl_sys_SDL_ZFOutputToSDL_RWops(callback), 1) == 0
        ;
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

