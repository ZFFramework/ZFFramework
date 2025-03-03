#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_font.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_SDL, ZFUITextView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        TTF_Init();
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        TTF_Quit();
        zfsuper::protocolOnDealloc();
    }

public:
    static void sdlMeasureCallback(
            ZF_OUT ZFUISize &ret
            , ZF_IN ZFImpl_sys_SDL_View *owner
            , ZF_IN const ZFUISize &sizeHint
            ) {
        ZFUITextView *textView = zfcast(ZFUITextView *, owner->ownerZFUIView);
        if(textView != zfnull) {
            ret = ZFPROTOCOL_ACCESS(ZFUITextView)->measureNativeTextView(textView, sizeHint, textView->textSize() * textView->UIScaleFixed());
        }
    }

public:
    virtual void *nativeTextViewCreate(
            ZF_IN ZFUITextView *textView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderImpls.add(zfself::renderCallback);
        nativeView->sdlMeasureCallback = zfself::sdlMeasureCallback;
        ++nativeView->renderCacheRequired;

        // no actual impl view, use renderImpls
        nativeImplViewRequireVirtualIndex = zffalse;
        return zfnull;
    }
    virtual void nativeTextViewDestroy(
            ZF_IN ZFUITextView *textView
            , ZF_IN void *nativeTextView
            ) {
        // nothing to do
    }

// ============================================================
// properties
public:
    virtual void text(
            ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textAppearance(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textAlign(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textColor(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIColor const &textColor
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textSize(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSize
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textSizeAutoMin(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMin
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textSizeAutoMax(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMax
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void singleLine(
            ZF_IN ZFUITextView *textView
            , ZF_IN zfbool singleLine
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }
    virtual void textTruncateMode(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextTruncateMode const &textTruncateMode
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        nativeView->renderRequest();
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        int w, h;
        if(zfself::measureNativeTextView(w, h, textView, textView->text(), textView->text().length(), (int)sizeHint.width, textSize)) {
            return ZFUISizeCreate((zffloat)w, (zffloat)h);
        }
        else {
            return ZFUISizeZero();
        }
    }

    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) {
        v_zffloat *textSizeCurrent = textView->objectTag("_ZFP_ZFImpl_sys_SDL_textSize");
        return textSizeCurrent != zfnull ? textSizeCurrent->zfv : textView->textSize();
    }

    virtual void layoutNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &viewSize
            ) {
        ZFImpl_sys_SDL_View *nativeView = (ZFImpl_sys_SDL_View *)textView->nativeView();
        zffloat textSizeCurrent = this->calcTextSizeAuto(textView, viewSize);
        textView->objectTag("_ZFP_ZFImpl_sys_SDL_textSize", zfobj<v_zffloat>(textSizeCurrent));
    }

private:
    static zfbool measureNativeTextView(
            ZF_OUT int &w
            , ZF_OUT int &h
            , ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            , ZF_IN zfindex textLen
            , ZF_IN int widthHint
            , ZF_IN zffloat textSize
            ) {
        ZFImpl_sys_SDL_FontType sdlFontType = ZFImpl_sys_SDL_FontType_normal;
        switch(textView->textAppearance()) {
            case v_ZFUITextAppearance::e_Bold:
                sdlFontType = ZFImpl_sys_SDL_FontType_bold;
                break;
            case v_ZFUITextAppearance::e_Italic:
                sdlFontType = ZFImpl_sys_SDL_FontType_italic;
                break;
            case v_ZFUITextAppearance::e_BoldItalic:
                sdlFontType = ZFImpl_sys_SDL_FontType_bold_italic;
                break;
            case v_ZFUITextAppearance::e_Normal:
            default:
                sdlFontType = ZFImpl_sys_SDL_FontType_normal;
                break;
        }
        ZFImpl_sys_SDL_fontAccess(sdlFont, sdlFontType, textSize);
        if(sdlFont == zfnull) {
            return zffalse;
        }

        int lineSkip = TTF_FontLineSkip(sdlFont);
        if(TTF_SizeUTF8(sdlFont, text, &w, &h) != 0) {
            return zffalse;
        }
        if(widthHint <= 0 || w < widthHint || textView->singleLine()) {
            h = zfmMax(h, lineSkip);
            ++w;
            return zftrue;
        }

        zfstring textTmp = text;
        for(zfchar *p = textTmp.zfunsafe_buffer(); *p; ++p) {
            if(*p == '\n') {
                *p = '\0';
            }
        }
        zfindex iText = 0;
        w = 0;
        int line = 0;
        while(iText < textLen) {
            ++line;
            int extent, count;
            if(TTF_MeasureUTF8(sdlFont, textTmp + iText, widthHint, &extent, &count) != 0) {
                return zffalse;
            }
            w = zfmMax(w, extent);
            iText += count;
            if(textTmp[iText] == '\0') {
                ++iText;
            }
        }
        h = zfmMax(h, lineSkip) + lineSkip * (line - 1);
        ++w;
        ++h;
        return zftrue;
    }

private:
    static zfbool renderCallback(
            ZF_IN SDL_Renderer *renderer
            , ZF_IN ZFImpl_sys_SDL_View *nativeView
            , ZF_IN const SDL_Rect &childRect
            , ZF_IN const SDL_Rect &parentRect
            , ZF_IN zffloat treeAlpha
            ) {
        ZFUITextView *owner = zfcast(ZFUITextView *, nativeView->ownerZFUIView);
        if(owner == zfnull || owner->text().isEmpty()) {
            return zffalse;
        }

        ZFImpl_sys_SDL_FontType sdlFontType = ZFImpl_sys_SDL_FontType_normal;
        switch(owner->textAppearance()) {
            case v_ZFUITextAppearance::e_Bold:
                sdlFontType = ZFImpl_sys_SDL_FontType_bold;
                break;
            case v_ZFUITextAppearance::e_Italic:
                sdlFontType = ZFImpl_sys_SDL_FontType_italic;
                break;
            case v_ZFUITextAppearance::e_BoldItalic:
                sdlFontType = ZFImpl_sys_SDL_FontType_bold_italic;
                break;
            case v_ZFUITextAppearance::e_Normal:
            default:
                sdlFontType = ZFImpl_sys_SDL_FontType_normal;
                break;
        }
        ZFImpl_sys_SDL_fontAccess(sdlFont, sdlFontType, owner->textSizeCurrent());
        if(sdlFont == zfnull) {
            return zffalse;
        }

        int sdlFontAlign = TTF_WRAPPED_ALIGN_LEFT;
        if(ZFBitTest(owner->textAlign(), v_ZFUIAlign::e_Left)) {
            sdlFontAlign = TTF_WRAPPED_ALIGN_LEFT;
        }
        else if(ZFBitTest(owner->textAlign(), v_ZFUIAlign::e_Right)) {
            sdlFontAlign = TTF_WRAPPED_ALIGN_RIGHT;
        }
        else {
            sdlFontAlign = TTF_WRAPPED_ALIGN_CENTER;
        }
        TTF_SetFontWrappedAlign(sdlFont, sdlFontAlign);

        ZFUIRect targetRectTmp = ZFUIRectApplyScale(owner->nativeImplViewFrame(), owner->UIScaleFixed());
        targetRectTmp.x += parentRect.x + childRect.x;
        targetRectTmp.y += parentRect.y + childRect.y;
        SDL_Rect targetRect = ZFImpl_sys_SDL_ZFUIRectToSDL_Rect(targetRectTmp);

        zffloat textSizeCurrent;
        {
            v_zffloat *tmp = owner->objectTag("_ZFP_ZFImpl_sys_SDL_textSize");
            if(tmp == zfnull) {
                textSizeCurrent = ZFPROTOCOL_ACCESS(ZFUITextView)->calcTextSizeAuto(owner, ZFUIRectGetSize(targetRectTmp));
                owner->objectTag("_ZFP_ZFImpl_sys_SDL_textSize", zfobj<v_zffloat>(textSizeCurrent));
            }
            else {
                textSizeCurrent = tmp->zfv;
            }
        }

        SDL_Surface *sdlSurface = zfnull;
        if(owner->singleLine() || owner->textTruncateMode() != v_ZFUITextTruncateMode::e_Disable) {
            sdlSurface = renderWrapped(owner, targetRect, sdlFont, textSizeCurrent);
        }
        else {
            sdlSurface = TTF_RenderUTF8_Blended_Wrapped(
                sdlFont,
                owner->text(),
                ZFImpl_sys_SDL_ZFUIColorToSDL_Color(owner->textColor()),
                (Uint32)targetRect.w);
        }
        if(sdlSurface == zfnull) {
            return zffalse;
        }
        ZFImpl_sys_SDL_zfblockedDestroySurface(sdlSurface);

        SDL_Rect srcRect;
        SDL_Rect targetRectFixed;
        ZFUIAlignFlags textAlign = owner->textAlign();

        if(ZFBitTest(textAlign, v_ZFUIAlign::e_Left)) {
            srcRect.x = 0;
            targetRectFixed.x = targetRect.x;
            srcRect.w = targetRectFixed.w = (sdlSurface->w <= targetRect.w ? sdlSurface->w : targetRect.w);
        }
        else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Right)) {
            if(sdlSurface->w <= targetRect.w) {
                srcRect.x = 0;
                srcRect.w = sdlSurface->w;
                targetRectFixed.x = targetRect.x + targetRect.w - sdlSurface->w;
                targetRectFixed.w = sdlSurface->w;
            }
            else {
                srcRect.x = sdlSurface->w - targetRect.w;
                srcRect.w = targetRect.w;
                targetRectFixed.x = targetRect.x;
                targetRectFixed.w = targetRect.w;
            }
        }
        else {
            if(sdlSurface->w <= targetRect.w) {
                srcRect.x = 0;
                srcRect.w = sdlSurface->w;
                targetRectFixed.x = targetRect.x + (targetRect.w - sdlSurface->w) / 2;
                targetRectFixed.w = sdlSurface->w;
            }
            else {
                srcRect.x = (sdlSurface->w - targetRect.w) / 2;
                srcRect.w = targetRect.w;
                targetRectFixed.x = targetRect.x;
                targetRectFixed.w = targetRect.w;
            }
        }

        if(ZFBitTest(textAlign, v_ZFUIAlign::e_Top)) {
            srcRect.y = 0;
            targetRectFixed.y = targetRect.y;
            srcRect.h = targetRectFixed.h = (sdlSurface->h <= targetRect.h ? sdlSurface->h : targetRect.h);
        }
        else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Bottom)) {
            if(sdlSurface->h <= targetRect.h) {
                srcRect.y = 0;
                srcRect.h = sdlSurface->h;
                targetRectFixed.y = targetRect.y + targetRect.h - sdlSurface->h;
                targetRectFixed.h = sdlSurface->h;
            }
            else {
                srcRect.y = sdlSurface->h - targetRect.h;
                srcRect.h = targetRect.h;
                targetRectFixed.y = targetRect.y;
                targetRectFixed.h = targetRect.h;
            }
        }
        else {
            if(sdlSurface->h <= targetRect.h) {
                srcRect.y = 0;
                srcRect.h = sdlSurface->h;
                targetRectFixed.y = targetRect.y + (targetRect.h - sdlSurface->h) / 2;
                targetRectFixed.h = sdlSurface->h;
            }
            else {
                srcRect.y = (sdlSurface->h - targetRect.h) / 2;
                srcRect.h = targetRect.h;
                targetRectFixed.y = targetRect.y;
                targetRectFixed.h = targetRect.h;
            }
        }

        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, sdlSurface);
        ZFImpl_sys_SDL_zfblockedDestroyTexture(sdlTexture);
        if(treeAlpha != 1) {
            SDL_SetTextureAlphaMod(sdlTexture, (Uint8)(treeAlpha * 255));
        }
        SDL_RenderCopy(renderer, sdlTexture, &srcRect, &targetRectFixed);
        return zffalse;
    }
    static SDL_Surface *renderWrapped(
            ZF_IN ZFUITextView *owner
            , ZF_IN const SDL_Rect &targetRect
            , ZF_IN TTF_Font *sdlFont
            , ZF_IN zffloat textSizeCurrent
            ) {
        zfbool singleLine = owner->singleLine();
        ZFUITextTruncateMode textTruncateMode = owner->textTruncateMode();
        SDL_Color textColor = ZFImpl_sys_SDL_ZFUIColorToSDL_Color(owner->textColor());
        if(textTruncateMode == v_ZFUITextTruncateMode::e_Disable) {
            if(singleLine) {
                return TTF_RenderUTF8_Blended(sdlFont, owner->text(), textColor);
            }
            else {
                return TTF_RenderUTF8_Blended_Wrapped(sdlFont, owner->text(), textColor, targetRect.w);
            }
        }

        int w, h;
        if(singleLine) {
            if(TTF_SizeUTF8(sdlFont, owner->text(), &w, &h) != 0) {
                return zfnull;
            }
            if(w <= targetRect.w) {
                return TTF_RenderUTF8_Blended(sdlFont, owner->text(), textColor);
            }
        }
        else {
            if(!zfself::measureNativeTextView(w, h, owner, owner->text(), owner->text().length(), targetRect.w, textSizeCurrent)) {
                return zfnull;
            }
            if(w <= targetRect.w && h <= targetRect.h) {
                return TTF_RenderUTF8_Blended_Wrapped(sdlFont, owner->text(), textColor, (Uint32)targetRect.w);
            }
        }

        zfstring text = owner->text();
        zfindex p;
        switch(owner->textTruncateMode()) {
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
                if(!zfself::measureNativeTextView(w, h, owner, textNew, textNew.length(), targetRect.w, textSizeCurrent)) {
                    return zfnull;
                }
                if(w <= targetRect.w && h <= targetRect.h) {
                    if(stripMin == stripMax || strip == stripMin + 1) {
                        return TTF_RenderUTF8_Blended_Wrapped(sdlFont, textNew, textColor, targetRect.w);
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

