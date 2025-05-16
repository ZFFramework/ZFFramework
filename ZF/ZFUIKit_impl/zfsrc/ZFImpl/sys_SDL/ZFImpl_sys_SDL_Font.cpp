#include "ZFImpl_sys_SDL_Font.h"

#if ZF_ENV_sys_SDL

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_Font_init, ZFLevelZFFrameworkEssential) {
    TTF_Init();
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_Font_init) {
    TTF_Quit();
}
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_Font_init)

// ============================================================
ZFImpl_sys_SDL_FontLoader ZFImpl_sys_SDL_fontLoader = zfnull;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_fontDataHolder, ZFLevelZFFrameworkNormal) {
    ZFImpl_sys_SDL_fontLoader = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_SDL_fontDataHolder) {
    this->fontCacheRemoveAll();
}
public:
    zfstlhashmap<ZFImpl_sys_SDL_FontType, ZFImpl_sys_SDL_FontData *> fontCache;
public:
    void fontCacheRemoveAll(void) {
        for(zfstlhashmap<ZFImpl_sys_SDL_FontType, ZFImpl_sys_SDL_FontData *>::iterator it = this->fontCache.begin(); it != this->fontCache.end(); ++it) {
            ZFImpl_sys_SDL_fontRelease(it->second);
        }
        this->fontCache.clear();
    }
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_fontDataHolder)

static ZFImpl_sys_SDL_FontData *ZFImpl_sys_SDL_fontLoad(
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

    TTF_SetFontHinting(fontData->font, TTF_HINTING_NORMAL);
    TTF_SetFontKerning(fontData->font, 1);
    TTF_SetFontOutline(fontData->font, 0);
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

    return fontData;
}
ZFImpl_sys_SDL_FontData *ZFImpl_sys_SDL_fontAlloc(
        ZF_IN ZFImpl_sys_SDL_FontType fontType
        , ZF_IN zffloat ptsize
        ) {
    zfuint ptsizeFixed = (zfuint)zfmRound(ptsize);

    if(ZFImpl_sys_SDL_fontLoader != zfnull) {
        for(zfint fontTypeTmp = fontType; fontTypeTmp >= ZFImpl_sys_SDL_FontType_normal && fontTypeTmp <= ZFImpl_sys_SDL_FontType_bold_italic; --fontTypeTmp) {
            ZFInput input = ZFImpl_sys_SDL_fontLoader((ZFImpl_sys_SDL_FontType)fontTypeTmp);
            if(input) {
                ZFImpl_sys_SDL_FontData *fontData = ZFImpl_sys_SDL_fontLoad((ZFImpl_sys_SDL_FontType)fontTypeTmp, ptsizeFixed, input);
                if(fontData != zfnull) {
                    return fontData;
                }
            }
        }
    }

    for(zfint fontTypeTmp = fontType; fontTypeTmp >= ZFImpl_sys_SDL_FontType_normal && fontTypeTmp <= ZFImpl_sys_SDL_FontType_bold_italic; --fontTypeTmp) {
        ZFInput input;
        {
            ZFLISTENER_1(impl
                    , ZFInput &, input
                    ) {
                const ZFFileFindData &fd = zfargs.param1().to<v_ZFFileFindData *>()->zfv;
                if(fd.name().length() > 4 && zfstringIsEqual(fd.name() + (fd.name().length() - 4), ".ttf")) {
                    zfargs.eventFiltered(zftrue);
                    input = ZFInputForPathInfo(zfargs.param0().to<v_ZFPathInfo *>()->zfv);
                }
            } ZFLISTENER_END()
            ZFPathInfoForEachFile(ZFPathInfo(ZFPathType_res(), "ZFUIKit_impl/sys_SDL/font"), impl);
        }
        if(input) {
            ZFImpl_sys_SDL_FontData *fontData = ZFImpl_sys_SDL_fontLoad((ZFImpl_sys_SDL_FontType)fontTypeTmp, ptsizeFixed, input);
            if(fontData != zfnull) {
                return fontData;
            }
        }
    }

    return zfnull;
}

void ZFImpl_sys_SDL_fontRelease(ZF_IN ZFImpl_sys_SDL_FontData *fontData) {
    if(fontData != zfnull) {
        zfdelete(fontData);
    }
}

// ============================================================
void _ZFP_ZFImpl_sys_SDL_fontAccess::_load(
        ZF_IN ZFImpl_sys_SDL_FontType fontType
        , ZF_IN zffloat ptsize
        ) {
    this->fontData = zfnull;
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_sys_SDL_fontDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_SDL_fontDataHolder);
    zfstlhashmap<ZFImpl_sys_SDL_FontType, ZFImpl_sys_SDL_FontData *>::iterator it = d->fontCache.find(fontType);
    if(it != d->fontCache.end()) {
        this->fontData = it->second;
        this->fontData->ptsize = ptsize;
        TTF_SetFontSize(this->fontData->font, ptsize);
        return;
    }
    this->fontData = ZFImpl_sys_SDL_fontAlloc(fontType, ptsize);
    if(this->fontData != zfnull) {
        d->fontCache[fontType] = this->fontData;
    }
}
_ZFP_ZFImpl_sys_SDL_fontAccess::_ZFP_ZFImpl_sys_SDL_fontAccess(
        ZF_IN ZFImpl_sys_SDL_FontType fontType
        , ZF_IN zffloat ptsize
        ) {
    this->_load(fontType, ptsize);
}
_ZFP_ZFImpl_sys_SDL_fontAccess::_ZFP_ZFImpl_sys_SDL_fontAccess(
        ZF_IN ZFUITextAppearance fontType
        , ZF_IN zffloat ptsize
        ) {
    switch(fontType) {
        case v_ZFUITextAppearance::e_Bold:
            this->_load(ZFImpl_sys_SDL_FontType_bold, ptsize);
            break;
        case v_ZFUITextAppearance::e_Italic:
            this->_load(ZFImpl_sys_SDL_FontType_italic, ptsize);
            break;
        case v_ZFUITextAppearance::e_BoldItalic:
            this->_load(ZFImpl_sys_SDL_FontType_bold_italic, ptsize);
            break;
        case v_ZFUITextAppearance::e_Normal:
        default:
            this->_load(ZFImpl_sys_SDL_FontType_normal, ptsize);
            break;
    }
}
_ZFP_ZFImpl_sys_SDL_fontAccess::~_ZFP_ZFImpl_sys_SDL_fontAccess(void) {
    // nothing to do
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

