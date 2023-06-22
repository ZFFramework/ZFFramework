#ifndef _ZFI_ZFImpl_sys_SDL_font_h_
#define _ZFI_ZFImpl_sys_SDL_font_h_

#include "ZFImpl_sys_SDL_ZF_impl.h"

#include "ZFCore.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/_repo/SDL/SDL_ttf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief sdl font type */
typedef enum {
    ZFImpl_sys_SDL_FontType_normal, /**< @brief normal */
    ZFImpl_sys_SDL_FontType_bold, /**< @brief bold */
    ZFImpl_sys_SDL_FontType_italic, /**< @brief italic */
    ZFImpl_sys_SDL_FontType_bold_italic, /**< @brief bold italic */
} ZFImpl_sys_SDL_FontType;

/** @brief sdl font cache for #ZFImpl_sys_SDL_fontAlloc */
zfclassNotPOD ZFLIB_ZF_impl ZFImpl_sys_SDL_FontData
{
public:
    ZFImpl_sys_SDL_FontType fontType; /**< @brief the font type, may differ from desired */
    zfuint ptsize; /**< @brief desired ptsize */
    TTF_Font *font; /**< @brief the font */
public:
    /** @cond ZFPrivateDoc */
    ~ZFImpl_sys_SDL_FontData(void)
    {
        TTF_CloseFont(this->font);
    }
    /** @endcond */
};

/**
 * @brief util to access font
 *
 * return null if unable to load specified font type,
 * otherwise, you must call #ZFImpl_sys_SDL_fontRelease to release the font when done,
 * or use #ZFImpl_sys_SDL_fontAccess for short
 */
extern ZFLIB_ZF_impl ZFImpl_sys_SDL_FontData *ZFImpl_sys_SDL_fontAlloc(ZF_IN ZFImpl_sys_SDL_FontType fontType, ZF_IN zfuint ptsize);
/** @brief see #ZFImpl_sys_SDL_fontAlloc */
extern ZFLIB_ZF_impl void ZFImpl_sys_SDL_fontRelease(ZF_IN ZFImpl_sys_SDL_FontData *fontData);

#define ZFImpl_sys_SDL_fontAccess(varName, fontType, ptsize) \
    _ZFP_ZFImpl_sys_SDL_fontAccess varName(fontType, ptsize)
zfclassNotPOD ZFLIB_ZF_impl _ZFP_ZFImpl_sys_SDL_fontAccess
{
public:
    _ZFP_ZFImpl_sys_SDL_fontAccess(ZF_IN ZFImpl_sys_SDL_FontType fontType, ZF_IN zfuint ptsize)
    : fontData(ZFImpl_sys_SDL_fontAlloc(fontType, ptsize))
    {
    }
    ~_ZFP_ZFImpl_sys_SDL_fontAccess(void)
    {
        ZFImpl_sys_SDL_fontRelease(this->fontData);
    }
public:
    ZFImpl_sys_SDL_FontData *fontData;
public:
    operator bool (void) const
    {
        return this->fontData != zfnull;
    }
    operator TTF_Font * (void) const
    {
        return this->fontData != zfnull ? this->fontData->font : zfnull;
    }
    TTF_Font *operator -> (void) const
    {
        return this->fontData->font;
    }
    _ZFP_ZFImpl_sys_SDL_fontAccess &operator = (ZF_IN zfnullT dummy)
    {
        ZFImpl_sys_SDL_fontRelease(this->fontData);
        this->fontData = zfnull;
        return *this;
    }
    zfbool operator == (ZF_IN zfnullT dummy) const
    {
        return this->fontData == zfnull;
    }
    zfbool operator != (ZF_IN zfnullT dummy) const
    {
        return this->fontData != zfnull;
    }
};

// ============================================================
/** @brief max cache size of #ZFImpl_sys_SDL_fontAlloc, default 10 */
extern ZFLIB_ZF_impl zfindex ZFImpl_sys_SDL_fontCacheSize;

/** @brief custom font load for #ZFImpl_sys_SDL_fontAlloc */
typedef ZFInput (*ZFImpl_sys_SDL_FontLoader)(ZF_IN ZFImpl_sys_SDL_FontType fontType);

/**
 * @brief change the font source of #ZFImpl_sys_SDL_fontAlloc
 *
 * by default, we would access font by:
 * -  normal: zfres/ZF_impl/sys_SDL/font/IBMPlexMono-Regular.ttf
 * -  bold: zfres/ZF_impl/sys_SDL/font/IBMPlexMono-Bold.ttf
 * -  italic: zfres/ZF_impl/sys_SDL/font/IBMPlexMono-Italic.ttf
 * -  bold italic: zfres/ZF_impl/sys_SDL/font/IBMPlexMono-BoldItalic.ttf
 *
 * you may supply your own font by supply font loader callback,
 * which would return proper input callback points to a ttf font file\n
 * call this method again with null params would reset to default font\n
 * call this method would reset all cache of #ZFImpl_sys_SDL_fontAlloc
 */
extern ZFLIB_ZF_impl ZFImpl_sys_SDL_FontLoader ZFImpl_sys_SDL_fontLoader;

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL
#endif // #ifndef _ZFI_ZFImpl_sys_SDL_font_h_

