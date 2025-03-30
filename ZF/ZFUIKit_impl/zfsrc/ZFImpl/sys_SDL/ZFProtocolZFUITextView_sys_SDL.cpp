#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_Font.h"
#include "ZFImpl/sys_SDL/ZFImpl_sys_SDL_Text.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_SDL, ZFUITextView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

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
        ZFImpl_sys_SDL_fontAccess(sdlFont, textView->textAppearance(), textSize);
        if(ZFImpl_sys_SDL_textMeasure(w, h, textView->text(), sizeHint.width, textView->singleLine(), sdlFont)) {
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

        ZFImpl_sys_SDL_fontAccess(sdlFont, owner->textAppearance(), owner->textSizeCurrent());
        if(sdlFont == zfnull) {
            return zffalse;
        }

        ZFUIRect targetRectTmp = ZFUIRectApplyScale(owner->nativeImplViewFrame(), owner->UIScaleFixed());
        targetRectTmp.x += parentRect.x + childRect.x;
        targetRectTmp.y += parentRect.y + childRect.y;
        SDL_Rect targetRect = ZFImpl_sys_SDL_ZFUIRectToSDL_Rect(targetRectTmp);

        zfobj<ZFUITextConfig> textConfig;
        textConfig->textColor(owner->textColor());
        ZFImpl_sys_SDL_textRender(
                renderer
                , targetRect
                , owner->text()
                , sdlFont
                , owner->textAlign()
                , ZFImpl_sys_SDL_ZFUIColorToSDL_Color(owner->textColor())
                , owner->singleLine()
                , owner->textTruncateMode()
                , treeAlpha);
        return zffalse;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

