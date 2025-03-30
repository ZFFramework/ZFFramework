#ifndef _ZFI_ZFImpl_sys_SDL_Text_h_
#define _ZFI_ZFImpl_sys_SDL_Text_h_

#include "ZFImpl_sys_SDL_Font.h"
#include "ZFUIKit/ZFUIText.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFUIKit_impl zfbool ZFImpl_sys_SDL_textMeasure(
        ZF_OUT int &w
        , ZF_OUT int &h
        , ZF_IN const zfstring &text
        , ZF_IN int widthHint
        , ZF_IN zfbool singleLine
        , ZF_IN TTF_Font *sdlFont
        );

extern ZFLIB_ZFUIKit_impl zfbool ZFImpl_sys_SDL_textRender(
        ZF_IN SDL_Renderer *renderer
        , ZF_IN const SDL_Rect &targetRect
        , ZF_IN const zfstring &text
        , ZF_IN TTF_Font *sdlFont
        , ZF_IN const ZFUIAlignFlags &textAlign
        , ZF_IN SDL_Color textColor
        , ZF_IN zfbool singleLine
        , ZF_IN ZFUITextTruncateMode textTruncateMode
        , ZF_IN zffloat alpha
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_Text_h_

