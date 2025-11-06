#ifndef _ZFI_ZFImpl_sys_SDL_RootWindow_h_
#define _ZFI_ZFImpl_sys_SDL_RootWindow_h_

#include "ZFImpl_sys_SDL_View.h"
#include "ZFUIKit/ZFUIRootWindow.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief mouse state for impl
 */
zfclassNotPOD ZFImpl_sys_SDL_MouseState {
public:
    /** @brief current mouse id */
    zfidentity mouseId;
    /** @brief prev mouse down view */
    ZFImpl_sys_SDL_View *viewDown;
    /** @brief whether viewDown grabbed the mouse event */
    zfbool mouseGrab;
    /** @brief prev mouse hover view */
    ZFImpl_sys_SDL_View *viewHover;

public:
    /** @cond ZFPrivateDoc */
    ZFImpl_sys_SDL_MouseState(void)
    : mouseId(zfidentityInvalid())
    , viewDown(zfnull)
    , mouseGrab(zffalse)
    , viewHover(zfnull)
    {
    }
    /** @endcond */
};

zfclassFwd _ZFP_ZFImpl_sys_SDL_RootWindowPrivate;
/**
 * @brief native view impl for SDL
 */
zffinal zfclassNotPOD ZFLIB_ZFUIKit_impl ZFImpl_sys_SDL_RootWindow {
public:
    /** @brief access all window instance */
    static ZFCoreArray<ZFImpl_sys_SDL_RootWindow *> &allWindow(void);

private:
    _ZFP_ZFImpl_sys_SDL_RootWindowPrivate *d;
public:
    /** @brief the owner sys window */
    ZFUIRootWindow *ownerZFUIRootWindow;
    /** @brief whether builtin window created by ZFFramework */
    zfbool builtinWindow;
    /** @brief sdl window */
    SDL_Window *sdlWindow;
    /** @brief sdl renderer */
    SDL_Renderer *sdlRenderer;
    /** @brief root view */
    ZFImpl_sys_SDL_View *rootView;

    /** @brief focused view */
    ZFImpl_sys_SDL_View *focused;

    /** @brief mouse id generator */
    ZFIdentityGenerator mouseIdGen;

public:
    /**
     * @brief attach and start render window and all of its content children view
     */
    void renderStart(void);
    /** @brief see #renderStart */
    void renderStop(void);

    /** @brief schedule a render task */
    void renderRequest(void);

    /** @brief schedule a layout task to notify layout the ZFUIRootWindow::rootView */
    void layoutRequest(void);

public:
    /**
     * @brief try to change focused view, or change to nearest focusable parent
     *
     * null to clear current focused view,
     * do nothing if view and all of its parents are not focusable
     */
    void focus(ZF_IN ZFImpl_sys_SDL_View *view);

    /**
     * @brief access mouse state
     *
     * which: SDL_MouseButtonEvent.which or SDL_MouseMotionEvent.which
     * button: SDL_MouseButtonEvent.button or -1 for SDL_MouseMotionEvent
     */
    ZFImpl_sys_SDL_MouseState &mouseState(
            ZF_IN Uint32 which
            , ZF_IN Uint8 button
            );

public:
    /** @cond ZFPrivateDoc */
    ZFImpl_sys_SDL_RootWindow(void);
    ~ZFImpl_sys_SDL_RootWindow(void);
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_RootWindow_h_

