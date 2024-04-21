#include "ZFImpl_sys_SDL_font.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

zfindex ZFImpl_sys_SDL_fontCacheSize = 10;
ZFImpl_sys_SDL_FontLoader ZFImpl_sys_SDL_fontLoader = zfnull;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_fontDataHolder, ZFLevelZFFrameworkNormal) {
    ZFImpl_sys_SDL_fontCacheSize = 10;
    ZFImpl_sys_SDL_fontLoader = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_fontDataHolder) {
    this->fontCacheRemoveAll();
}
public:
    ZFCoreArray<ZFImpl_sys_SDL_FontData *> fontCache; // latest cache at tail
public:
    void fontCacheRemoveAll(void) {
        while(!fontCache.isEmpty()) {
            zfdelete(fontCache.removeLastAndGet());
        }
    }
    ZFImpl_sys_SDL_FontData *fontLoad(
            ZF_IN ZFImpl_sys_SDL_FontType fontType
            , ZF_IN zfuint ptsize
            , ZF_IN const ZFInput &input
            ) {
        TTF_Font *font = TTF_OpenFontRW(ZFImpl_sys_SDL_ZFInputToSDL_RWops(input), 1, ptsize);
        if(font == zfnull) {
            return zfnull;
        }
        ZFImpl_sys_SDL_FontData *fontData = zfnew(ZFImpl_sys_SDL_FontData);
        fontData->fontType = fontType;
        fontData->ptsize = ptsize;
        fontData->font = font;
        return fontData;
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_fontDataHolder)

ZFImpl_sys_SDL_FontData *ZFImpl_sys_SDL_fontAlloc(
        ZF_IN ZFImpl_sys_SDL_FontType fontType
        , ZF_IN zfuint ptsize
        ) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_sys_SDL_fontDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_SDL_fontDataHolder);
    for(zfindex i = d->fontCache.count() - 1; i != zfindexMax(); --i) {
        ZFImpl_sys_SDL_FontData *fontData = d->fontCache[i];
        if(fontData->fontType == fontType && fontData->ptsize == ptsize) {
            d->fontCache.remove(i);
            return fontData;
        }
    }

    if(ZFImpl_sys_SDL_fontLoader != zfnull) {
        for(zfint fontTypeTmp = fontType; fontTypeTmp >= ZFImpl_sys_SDL_FontType_normal && fontTypeTmp <= ZFImpl_sys_SDL_FontType_bold_italic; --fontTypeTmp) {
            ZFInput input = ZFImpl_sys_SDL_fontLoader((ZFImpl_sys_SDL_FontType)fontTypeTmp);
            if(input) {
                ZFImpl_sys_SDL_FontData *fontData = d->fontLoad((ZFImpl_sys_SDL_FontType)fontTypeTmp, ptsize, input);
                if(fontData != zfnull) {
                    return fontData;
                }
            }
        }
    }

    for(zfint fontTypeTmp = fontType; fontTypeTmp >= ZFImpl_sys_SDL_FontType_normal && fontTypeTmp <= ZFImpl_sys_SDL_FontType_bold_italic; --fontTypeTmp) {
        ZFInput input;
        switch(fontTypeTmp) {
            case ZFImpl_sys_SDL_FontType_bold:
                input = ZFInputForRes("ZF_impl/sys_SDL/font/IBMPlexMono-Bold.ttf");
                break;
            case ZFImpl_sys_SDL_FontType_italic:
                input = ZFInputForRes("ZF_impl/sys_SDL/font/IBMPlexMono-Italic.ttf");
                break;
            case ZFImpl_sys_SDL_FontType_bold_italic:
                input = ZFInputForRes("ZF_impl/sys_SDL/font/IBMPlexMono-BoldItalic.ttf");
                break;
            case ZFImpl_sys_SDL_FontType_normal:
            default:
                input = ZFInputForRes("ZF_impl/sys_SDL/font/IBMPlexMono-Regular.ttf");
                break;
        }
        if(input) {
            ZFImpl_sys_SDL_FontData *fontData = d->fontLoad((ZFImpl_sys_SDL_FontType)fontTypeTmp, ptsize, input);
            if(fontData != zfnull) {
                return fontData;
            }
        }
    }

    return zfnull;
}

void ZFImpl_sys_SDL_fontRelease(ZF_IN ZFImpl_sys_SDL_FontData *fontData) {
    if(fontData == zfnull || fontData->font == zfnull) {
        return;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_sys_SDL_fontDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_SDL_fontDataHolder);
    d->fontCache.add(fontData);

    // reset font property
    // TTF_SetFontDirection(fontData->font, TTF_DIRECTION_LTR);
    TTF_SetFontHinting(fontData->font, TTF_HINTING_NORMAL);
    // TTF_SetFontKerning(fontData->font, 1);
    // TTF_SetFontOutline(fontData->font, 0);
    TTF_SetFontSize(fontData->font, fontData->ptsize);
    switch(fontData->fontType) {
        case ZFImpl_sys_SDL_FontType_bold:
            TTF_SetFontStyle(fontData->font, TTF_STYLE_BOLD);
            break;
        case ZFImpl_sys_SDL_FontType_italic:
            TTF_SetFontStyle(fontData->font, TTF_STYLE_ITALIC);
            break;
        case ZFImpl_sys_SDL_FontType_bold_italic:
            TTF_SetFontStyle(fontData->font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
            break;
        case ZFImpl_sys_SDL_FontType_normal:
        default:
            TTF_SetFontStyle(fontData->font, TTF_STYLE_NORMAL);
            break;
    }
    TTF_SetFontWrappedAlign(fontData->font, TTF_WRAPPED_ALIGN_LEFT);

    while(d->fontCache.count() > ZFImpl_sys_SDL_fontCacheSize) {
        zfdelete(d->fontCache.removeFirstAndGet());
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

