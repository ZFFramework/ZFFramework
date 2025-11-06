#ifndef _ZFI_ZFImpl_sys_SDL_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_SDL_ZFUIKit_impl_h_

#include "../ZFImpl_ZFUIKit_impl.h"

#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_ZF_impl.h"

#include "ZFImpl_sys_SDL_RootWindow.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFUIKit_impl void ZFImpl_sys_SDL_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN ZFImpl_sys_SDL_View *view
        );
inline zfstring ZFImpl_sys_SDL_viewTreePrint(ZF_IN ZFImpl_sys_SDL_View *view) {
    zfstring ret;
    ZFImpl_sys_SDL_viewTreePrintT(ret, view);
    return ret;
}

// convert utility
inline void ZFImpl_sys_SDL_ZFUIPointToSDL_PointT(
        ZF_OUT SDL_Point &ret
        , ZF_IN const ZFUIPoint &point
        ) {
    ret.x = (int)point.x;
    ret.y = (int)point.y;
}
inline SDL_Point ZFImpl_sys_SDL_ZFUIPointToSDL_Point(ZF_IN const ZFUIPoint &point) {
    SDL_Point ret;
    ZFImpl_sys_SDL_ZFUIPointToSDL_PointT(ret, point);
    return ret;
}
inline void ZFImpl_sys_SDL_ZFUIPointFromSDL_PointT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN const SDL_Point &sdlPoint
        ) {
    ret.x = (zffloat)sdlPoint.x;
    ret.y = (zffloat)sdlPoint.y;
}
inline ZFUIPoint ZFImpl_sys_SDL_ZFUIPointFromSDL_Point(ZF_IN const SDL_Point &sdlPoint) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_SDL_ZFUIPointFromSDL_PointT(ret, sdlPoint);
    return ret;
}

inline void ZFImpl_sys_SDL_ZFUIPointToSDL_FPointT(
        ZF_OUT SDL_FPoint &ret
        , ZF_IN const ZFUIPoint &point
        ) {
    ret.x = (float)point.x;
    ret.y = (float)point.y;
}
inline SDL_FPoint ZFImpl_sys_SDL_ZFUIPointToSDL_FPoint(ZF_IN const ZFUIPoint &point) {
    SDL_FPoint ret;
    ZFImpl_sys_SDL_ZFUIPointToSDL_FPointT(ret, point);
    return ret;
}
inline void ZFImpl_sys_SDL_ZFUIPointFromSDL_FPointT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN const SDL_FPoint &sdlPoint
        ) {
    ret.x = (zffloat)sdlPoint.x;
    ret.y = (zffloat)sdlPoint.y;
}
inline ZFUIPoint ZFImpl_sys_SDL_ZFUIPointFromSDL_FPoint(ZF_IN const SDL_FPoint &sdlPoint) {
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_SDL_ZFUIPointFromSDL_FPointT(ret, sdlPoint);
    return ret;
}

inline void ZFImpl_sys_SDL_ZFUIRectToSDL_RectT(
        ZF_OUT SDL_Rect &ret
        , ZF_IN const ZFUIRect &rect
        ) {
    ret.x = (int)rect.x;
    ret.y = (int)rect.y;
    ret.w = (int)rect.width;
    ret.h = (int)rect.height;
}
inline SDL_Rect ZFImpl_sys_SDL_ZFUIRectToSDL_Rect(ZF_IN const ZFUIRect &rect) {
    SDL_Rect ret;
    ZFImpl_sys_SDL_ZFUIRectToSDL_RectT(ret, rect);
    return ret;
}
inline void ZFImpl_sys_SDL_ZFUIRectFromSDL_RectT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const SDL_Rect &sdlRect
        ) {
    ret.x = (zffloat)sdlRect.x;
    ret.y = (zffloat)sdlRect.y;
    ret.width = (zffloat)sdlRect.w;
    ret.height = (zffloat)sdlRect.h;
}
inline ZFUIRect ZFImpl_sys_SDL_ZFUIRectFromSDL_Rect(ZF_IN const SDL_Rect &sdlRect) {
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_SDL_ZFUIRectFromSDL_RectT(ret, sdlRect);
    return ret;
}

inline void ZFImpl_sys_SDL_ZFUIRectToSDL_FRectT(
        ZF_OUT SDL_FRect &ret
        , ZF_IN const ZFUIRect &rect
        ) {
    ret.x = (float)rect.x;
    ret.y = (float)rect.y;
    ret.w = (float)rect.width;
    ret.h = (float)rect.height;
}
inline SDL_FRect ZFImpl_sys_SDL_ZFUIRectToSDL_FRect(ZF_IN const ZFUIRect &rect) {
    SDL_FRect ret;
    ZFImpl_sys_SDL_ZFUIRectToSDL_FRectT(ret, rect);
    return ret;
}
inline void ZFImpl_sys_SDL_ZFUIRectFromSDL_FRectT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN const SDL_FRect &sdlRect
        ) {
    ret.x = (zffloat)sdlRect.x;
    ret.y = (zffloat)sdlRect.y;
    ret.width = (zffloat)sdlRect.w;
    ret.height = (zffloat)sdlRect.h;
}
inline ZFUIRect ZFImpl_sys_SDL_ZFUIRectFromSDL_FRect(ZF_IN const SDL_FRect &sdlRect) {
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_SDL_ZFUIRectFromSDL_FRectT(ret, sdlRect);
    return ret;
}

inline void ZFImpl_sys_SDL_ZFUIColorToSDL_ColorT(
        ZF_IN SDL_Color &ret
        , ZF_IN const ZFUIColor &color
        ) {
    ret.r = (Uint8)(ZFUIColorGetR(color) * 0xFF);
    ret.g = (Uint8)(ZFUIColorGetG(color) * 0xFF);
    ret.b = (Uint8)(ZFUIColorGetB(color) * 0xFF);
    ret.a = (Uint8)(ZFUIColorGetA(color) * 0xFF);
}
inline SDL_Color ZFImpl_sys_SDL_ZFUIColorToSDL_Color(ZF_IN const ZFUIColor &color) {
    SDL_Color ret;
    ZFImpl_sys_SDL_ZFUIColorToSDL_ColorT(ret, color);
    return ret;
}
inline void ZFImpl_sys_SDL_ZFUIColorFromSDL_ColorT(
        ZF_OUT ZFUIColor &ret
        , ZF_IN const SDL_Color sdlColor
        ) {
    ret = ZFUIColorCreate(
            (zffloat)sdlColor.r / 0xFF,
            (zffloat)sdlColor.g / 0xFF,
            (zffloat)sdlColor.b / 0xFF,
            (zffloat)sdlColor.a / 0xFF
        );
}
inline ZFUIColor ZFImpl_sys_SDL_ZFUIColorFromSDL_Color(ZF_IN const SDL_Color sdlColor) {
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_SDL_ZFUIColorFromSDL_ColorT(ret, sdlColor);
    return ret;
}

extern ZFLIB_ZF_impl zfbool ZFImpl_sys_SDL_SurfaceToOutput(
        ZF_IN const ZFOutput &callback
        , ZF_IN SDL_Surface *sdlSurface
        );
extern ZFLIB_ZF_impl zfbool ZFImpl_sys_SDL_TextureToOutput(
        ZF_IN const ZFOutput &callback
        , ZF_IN SDL_Texture *sdlTexture
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_ZFUIKit_impl_h_

