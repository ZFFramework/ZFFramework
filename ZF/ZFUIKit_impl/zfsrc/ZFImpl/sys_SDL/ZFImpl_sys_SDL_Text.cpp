#include "ZFImpl_sys_SDL_Text.h"
#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFImpl_sys_SDL_textMeasure(
        ZF_OUT int &w
        , ZF_OUT int &h
        , ZF_IN const zfstring &text
        , ZF_IN int widthHint
        , ZF_IN zfbool singleLine
        , ZF_IN TTF_Font *sdlFont
        ) {
    if(sdlFont == zfnull) {
        return zffalse;
    }
    if(text.isEmpty()) {
        w = 0;
        h = 0;
        return zftrue;
    }

    // result to extra size, to ensure compact text views won't be overlapped
    const int extraSize = 1;

    int lineSkip = TTF_FontLineSkip(sdlFont);

    zfindex lineEndPos = zfstringFind(text, '\n');
    if(singleLine) {
        if(TTF_SizeUTF8(sdlFont, lineEndPos != zfindexMax() ? zfstring(text, lineEndPos) : text, &w, &h) != 0) {
            return zffalse;
        }
        h = zfmMax(h, lineSkip);
        w += extraSize;
        return zftrue;
    }

    zfstring tmp;
    zfindex textLen = text.length();
    zfindex iText = 0;
    w = 0;
    h = 0;
    int line = 0;
    while(iText < textLen) {
        ++line;
        int extent, count;
        if(TTF_MeasureUTF8(
                    sdlFont
                    , lineEndPos == zfindexMax() ? text + iText : tmp.assign(text + iText, lineEndPos + 1).cString()
                    , widthHint
                    , &extent
                    , &count
                    ) != 0) {
            return zffalse;
        }
        w = zfmMax(w, extent);
        iText += count;
        lineEndPos = zfstringFind(text + iText, '\n');
    }
    h = zfmMax(h, lineSkip) + lineSkip * (line - 1);
    w += extraSize;
    h += extraSize;
    return zftrue;
}

// ============================================================
static void _ZFP_ZFImpl_sys_SDL_textAlignApply(
        ZF_OUT SDL_Rect &srcRect
        , ZF_OUT SDL_Rect &targetRectFixed
        , ZF_IN const SDL_Rect &targetRect
        , ZF_IN const ZFUIAlignFlags &textAlign
        , ZF_IN int w
        , ZF_IN int h
        ) {
    if(ZFBitTest(textAlign, v_ZFUIAlign::e_Left)) {
        srcRect.x = 0;
        targetRectFixed.x = targetRect.x;
        srcRect.w = targetRectFixed.w = (w <= targetRect.w ? w : targetRect.w);
    }
    else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Right)) {
        if(w <= targetRect.w) {
            srcRect.x = 0;
            srcRect.w = w;
            targetRectFixed.x = targetRect.x + targetRect.w - w;
            targetRectFixed.w = w;
        }
        else {
            srcRect.x = w - targetRect.w;
            srcRect.w = targetRect.w;
            targetRectFixed.x = targetRect.x;
            targetRectFixed.w = targetRect.w;
        }
    }
    else {
        if(w <= targetRect.w) {
            srcRect.x = 0;
            srcRect.w = w;
            targetRectFixed.x = targetRect.x + (targetRect.w - w) / 2;
            targetRectFixed.w = w;
        }
        else {
            srcRect.x = (w - targetRect.w) / 2;
            srcRect.w = targetRect.w;
            targetRectFixed.x = targetRect.x;
            targetRectFixed.w = targetRect.w;
        }
    }

    if(ZFBitTest(textAlign, v_ZFUIAlign::e_Top)) {
        srcRect.y = 0;
        targetRectFixed.y = targetRect.y;
        srcRect.h = targetRectFixed.h = (h <= targetRect.h ? h : targetRect.h);
    }
    else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Bottom)) {
        if(h <= targetRect.h) {
            srcRect.y = 0;
            srcRect.h = h;
            targetRectFixed.y = targetRect.y + targetRect.h - h;
            targetRectFixed.h = h;
        }
        else {
            srcRect.y = h - targetRect.h;
            srcRect.h = targetRect.h;
            targetRectFixed.y = targetRect.y;
            targetRectFixed.h = targetRect.h;
        }
    }
    else {
        if(h <= targetRect.h) {
            srcRect.y = 0;
            srcRect.h = h;
            targetRectFixed.y = targetRect.y + (targetRect.h - h) / 2;
            targetRectFixed.h = h;
        }
        else {
            srcRect.y = (h - targetRect.h) / 2;
            srcRect.h = targetRect.h;
            targetRectFixed.y = targetRect.y;
            targetRectFixed.h = targetRect.h;
        }
    }
}

static SDL_Surface *_ZFP_ZFImpl_sys_SDL_textRender(
        ZF_IN SDL_Renderer *renderer
        , ZF_IN const SDL_Rect &targetRect
        , ZF_IN const zfstring &text
        , ZF_IN TTF_Font *sdlFont
        , ZF_IN SDL_Color textColor
        , ZF_IN zfbool singleLine
        , ZF_IN ZFUITextTruncateMode textTruncateMode
        ) {
    if(textTruncateMode == v_ZFUITextTruncateMode::e_Disable) {
        if(singleLine) {
            return TTF_RenderUTF8_Blended(sdlFont, text, textColor);
        }
        else {
            return TTF_RenderUTF8_Blended_Wrapped(sdlFont, text, textColor, targetRect.w);
        }
    }

    int w, h;
    if(singleLine) {
        if(TTF_SizeUTF8(sdlFont, text, &w, &h) != 0) {
            return zfnull;
        }
        if(w <= targetRect.w) {
            return TTF_RenderUTF8_Blended(sdlFont, text, textColor);
        }
    }
    else {
        if(!ZFImpl_sys_SDL_textMeasure(w, h, text, targetRect.w, singleLine, sdlFont)) {
            return zfnull;
        }
        if(w <= targetRect.w && h <= targetRect.h) {
            return TTF_RenderUTF8_Blended_Wrapped(sdlFont, text, textColor, (Uint32)targetRect.w);
        }
    }

    zfindex p;
    switch(textTruncateMode) {
        case v_ZFUITextTruncateMode::e_Head:
            p = 0;
            break;
        case v_ZFUITextTruncateMode::e_Tail:
            p = text.length() - 1;
            break;
        case v_ZFUITextTruncateMode::e_Middle:
        default:
            p = text.length() / 2;
            break;
    }
    zfindex stripMin = 1;
    zfindex stripMax = zfmMax(p, text.length() - p);

    zfstring textNew;
    do {
        zfindex strip = (stripMin + stripMax) / 2;
        textNew.removeAll();
        if(strip < p) {
            textNew.append(text, p - strip);
        }
        textNew.append("..");
        if(p + strip < text.length()) {
            textNew.append(text + p + strip, text.length() - p - strip);
        }
        if(singleLine) {
            if(TTF_SizeUTF8(sdlFont, textNew, &w, &h) != 0) {
                return zfnull;
            }
            if(w <= targetRect.w) {
                if(stripMin == stripMax || strip == stripMin + 1) {
                    return TTF_RenderUTF8_Blended(sdlFont, textNew, textColor);
                }
                stripMax = strip;
            }
            else {
                stripMin = (stripMin == strip ? strip + 1 : strip);
            }
        }
        else {
            if(!ZFImpl_sys_SDL_textMeasure(w, h, textNew, targetRect.w, singleLine, sdlFont)) {
                return zfnull;
            }
            if(w <= targetRect.w && h <= targetRect.h) {
                if(stripMin == stripMax || strip == stripMin + 1) {
                    return TTF_RenderUTF8_Blended_Wrapped(sdlFont, textNew, textColor, (Uint32)targetRect.w);
                }
                stripMax = strip;
            }
            else {
                stripMin = (stripMin == strip ? strip + 1 : strip);
            }
        }

        if(strip >= p && p + strip >= text.length()) {
            return zfnull;
        }
    } while(zftrue);
}

zfbool ZFImpl_sys_SDL_textRender(
        ZF_IN SDL_Renderer *renderer
        , ZF_IN const SDL_Rect &targetRect
        , ZF_IN const zfstring &text
        , ZF_IN TTF_Font *sdlFont
        , ZF_IN const ZFUIAlignFlags &textAlign
        , ZF_IN SDL_Color textColor
        , ZF_IN zfbool singleLine
        , ZF_IN ZFUITextTruncateMode textTruncateMode
        , ZF_IN zffloat alpha
        ) {
    if(text.isEmpty()) {
        return zftrue;
    }

    if(sdlFont == zfnull) {
        return zffalse;
    }

    int sdlFontAlign = TTF_WRAPPED_ALIGN_LEFT;
    if(ZFBitTest(textAlign, v_ZFUIAlign::e_Left)) {
        sdlFontAlign = TTF_WRAPPED_ALIGN_LEFT;
    }
    else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Right)) {
        sdlFontAlign = TTF_WRAPPED_ALIGN_RIGHT;
    }
    else {
        sdlFontAlign = TTF_WRAPPED_ALIGN_CENTER;
    }
    TTF_SetFontWrappedAlign(sdlFont, sdlFontAlign);

    SDL_Surface *sdlSurface = zfnull;
    if(singleLine) {
        zfindex lineEndPos = zfstringFind(text, '\n');
        sdlSurface = _ZFP_ZFImpl_sys_SDL_textRender(
                renderer
                , targetRect
                , lineEndPos != zfindexMax() ? zfstring(text, lineEndPos) : text
                , sdlFont
                , textColor
                , singleLine
                , textTruncateMode
                );
    }
    else {
        sdlSurface = _ZFP_ZFImpl_sys_SDL_textRender(
                renderer
                , targetRect
                , text
                , sdlFont
                , textColor
                , singleLine
                , textTruncateMode
                );
    }
    if(sdlSurface == zfnull) {
        return zffalse;
    }
    ZFImpl_sys_SDL_zfblockedDestroySurface(sdlSurface);
    SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, sdlSurface);
    SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
    ZFImpl_sys_SDL_zfblockedDestroyTexture(sdlTexture);

    SDL_Rect srcRect;
    SDL_Rect targetRectFixed;
    _ZFP_ZFImpl_sys_SDL_textAlignApply(srcRect, targetRectFixed, targetRect, textAlign, sdlSurface->w, sdlSurface->h);
    if(alpha != 1) {
        SDL_SetTextureAlphaMod(sdlTexture, (Uint8)(alpha * 255));
    }
    SDL_RenderCopy(renderer, sdlTexture, &srcRect, &targetRectFixed);
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

