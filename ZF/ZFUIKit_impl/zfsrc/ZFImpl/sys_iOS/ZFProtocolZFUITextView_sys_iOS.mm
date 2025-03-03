#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUITextViewImpl_sys_iOS_TextView : UILabel
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl;
@property (nonatomic, assign) ZFUITextView *ownerZFUITextView;
@property (nonatomic, strong) NSString *fontName;
@property (nonatomic, assign) zffloat textSize;
@end
@implementation _ZFP_ZFUITextViewImpl_sys_iOS_TextView
// ============================================================
// init and dealloc
- (id)init {
    self = [super init];

    self.autoresizesSubviews = NO;
    self.backgroundColor = [UIColor clearColor];

    // property init
    self.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
    self.textColor = [UIColor blackColor];
    self.shadowColor = nil;
    self.numberOfLines = 1;
    self.lineBreakMode = NSLineBreakByClipping;

    return self;
}
- (void)dealloc {
    self.fontName = nil;
}
- (void)setTextSize:(zffloat)newTextSize ZFImpl_sys_iOS_overrideProperty {
    if(self->_textSize != newTextSize) {
        self->_textSize = newTextSize;
        self.font = [UIFont fontWithName:self.fontName size:newTextSize];
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_iOS, ZFUITextView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UILabel")

public:
    virtual void *nativeTextViewCreate(
            ZF_IN ZFUITextView *textView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        return (__bridge_retained void *)[_ZFP_ZFUITextViewImpl_sys_iOS_TextView new];
    }
    virtual void nativeTextViewDestroy(
            ZF_IN ZFUITextView *textView
            , ZF_IN void *nativeTextView
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *tmp = (__bridge_transfer _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)nativeTextView;
        tmp = nil;
    }

// ============================================================
// properties
public:
    virtual void text(
            ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.text = ZFImpl_sys_iOS_zfstringToNSString(text);
    }
    virtual void textAppearance(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        switch(textAppearance) {
            case v_ZFUITextAppearance::e_Normal:
                nativeImplView.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case v_ZFUITextAppearance::e_Bold:
                nativeImplView.fontName = [UIFont boldSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case v_ZFUITextAppearance::e_Italic:
                nativeImplView.fontName = [UIFont italicSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case v_ZFUITextAppearance::e_BoldItalic: {
                UIFontDescriptor *fontD = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontDescriptor;
                fontD = [fontD fontDescriptorWithSymbolicTraits:(UIFontDescriptorTraitBold | UIFontDescriptorTraitItalic)];
                nativeImplView.fontName = [UIFont fontWithDescriptor:fontD size:[UIFont systemFontSize]].fontName;
            }
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        nativeImplView.font = [UIFont fontWithName:nativeImplView.fontName size:nativeImplView.textSize];
    }
    virtual void textAlign(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        if(ZFBitTest(textAlign, v_ZFUIAlign::e_Left)) {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
        else if(ZFBitTest(textAlign, v_ZFUIAlign::e_Right)) {
            nativeImplView.textAlignment = NSTextAlignmentRight;
        }
        else if(textAlign == v_ZFUIAlign::e_Center) {
            nativeImplView.textAlignment = NSTextAlignmentCenter;
        }
        else {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
    }
    virtual void textColor(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIColor const &textColor
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.textColor = ZFImpl_sys_iOS_ZFUIColorToUIColor(textColor);
    }
    virtual void textSize(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSize
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMin(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMin
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMax(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMax
            ) {
        // changed during layoutNativeTextView
    }
    virtual void singleLine(
            ZF_IN ZFUITextView *textView
            , ZF_IN zfbool singleLine
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        nativeImplView.numberOfLines = (singleLine ? 1 : 0);
    }
    virtual void textTruncateMode(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextTruncateMode const &textTruncateMode
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        switch(textTruncateMode) {
            case v_ZFUITextTruncateMode::e_Disable:
                nativeImplView.lineBreakMode = NSLineBreakByClipping;
                break;
            case v_ZFUITextTruncateMode::e_Head:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingHead;
                break;
            case v_ZFUITextTruncateMode::e_Middle:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingMiddle;
                break;
            case v_ZFUITextTruncateMode::e_Tail:
                nativeImplView.lineBreakMode = NSLineBreakByTruncatingTail;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        CGSize sizeHintTmp = ZFImpl_sys_iOS_ZFUISizeToCGSize(sizeHint);
        if(sizeHintTmp.width <= 0) {
            sizeHintTmp.width = 30000;
        }
        if(sizeHintTmp.height <= 0) {
            sizeHintTmp.height = 30000;
        }
        if(textSize == nativeImplView.textSize) {
            sizeHintTmp = [nativeImplView sizeThatFits:sizeHintTmp];
        }
        else {
            zffloat textSizeSaved = nativeImplView.textSize;
            nativeImplView.textSize = textSize;
            sizeHintTmp = [nativeImplView sizeThatFits:sizeHintTmp];
            nativeImplView.textSize = textSizeSaved;
        }
        return ZFImpl_sys_iOS_ZFUISizeFromCGSize(sizeHintTmp);
    }

    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) {
        return ((__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView()).textSize;
    }

    virtual void layoutNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &viewSize
            ) {
        _ZFP_ZFUITextViewImpl_sys_iOS_TextView *nativeImplView = (__bridge _ZFP_ZFUITextViewImpl_sys_iOS_TextView *)textView->nativeImplView();
        zffloat fixedTextSize = this->calcTextSizeAuto(textView, viewSize);
        nativeImplView.textSize = fixedTextSize;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

