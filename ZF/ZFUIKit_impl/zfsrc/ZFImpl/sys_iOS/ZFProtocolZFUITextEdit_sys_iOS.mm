#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit : UITextField<UITextFieldDelegate>
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUITextEdit) *impl;
@property (nonatomic, assign) ZFUITextEdit *ownerZFUITextEdit;
@property (nonatomic, assign) zfstring lastText;
@property (nonatomic, assign) zfbool textOverrideFlag;

@property (nonatomic, strong) NSString *fontName;
@property (nonatomic, assign) zffloat textSize;

- (void)_textFieldTextOnUpdate:(UITextField *)textField;
@end

@implementation _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit

// ============================================================
// init and dealloc
- (id)init {
    self = [super init];

    self.autoresizesSubviews = NO;
    self.backgroundColor = [UIColor clearColor];

    self.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;

    self.delegate = self;
    [self addTarget:self action:@selector(_textFieldTextOnUpdate:) forControlEvents:UIControlEventEditingChanged];

    return self;
}
- (void)dealloc {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    [self removeTarget:self action:@selector(_textFieldTextOnUpdate:) forControlEvents:UIControlEventEditingChanged];
    self.fontName = nil;
}

- (void)setTextSize:(zffloat)newTextSize ZFImpl_sys_iOS_overrideProperty {
    if(self->_textSize != newTextSize) {
        self->_textSize = newTextSize;
        UIFont *newFont = [UIFont fontWithName:self.fontName size:newTextSize];
        self.font = newFont;
    }
}

- (void)setSelectedTextRange:(UITextRange *)selectedTextRange ZFImpl_sys_iOS_overrideProperty {
    [super setSelectedTextRange:selectedTextRange];
    [self _ZFP_selectedRangeNotifyUpdate];
}
- (void)_ZFP_selectedRangeNotifyUpdate {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeOnUpdate(self.ownerZFUITextEdit);
}

- (void)textFieldDidBeginEditing:(UITextField *)textField {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(self.ownerZFUITextEdit);
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeOnUpdate(self.ownerZFUITextEdit);
}
- (void)textFieldDidEndEditing:(UITextField *)textField {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(self.ownerZFUITextEdit);
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(self.ownerZFUITextEdit);
    return YES;
}

- (void)_textFieldTextOnUpdate:(UITextField *)textField {
    if(self.textOverrideFlag) {
        return;
    }

    zfstring text = textField.text.UTF8String;
    if(textField.markedTextRange != nil) {
        NSInteger start = [textField offsetFromPosition:textField.beginningOfDocument toPosition:textField.markedTextRange.start];
        NSInteger count = [textField offsetFromPosition:textField.markedTextRange.start toPosition:textField.markedTextRange.end];
        NSMutableString *s = [NSMutableString stringWithString:textField.text];
        [s replaceCharactersInRange:NSMakeRange(start, count) withString:@""];
        text = s.UTF8String;
    }

    if(self.lastText.compare(text) == 0) {
        return;
    }

    if(!ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldUpdate(self.ownerZFUITextEdit, text)) {
        self.textOverrideFlag = zftrue;
        textField.text = [NSString stringWithUTF8String:text.cString()];
        self.textOverrideFlag = zffalse;
        return;
    }
    self.lastText = text;
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextUpdate(self.ownerZFUITextEdit, self.lastText);
    [self _ZFP_selectedRangeNotifyUpdate];
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_iOS, ZFUITextEdit, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UITextField")
public:
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = [_ZFP_ZFUITextEditImpl_sys_iOS_TextEdit new];
        nativeImplView.ownerZFUITextEdit = textEdit;
        return (__bridge_retained void *)nativeImplView;
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *tmp = (__bridge_transfer _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        tmp = nil;
    }

// ============================================================
// properties
public:
    virtual void editEnable(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editEnable
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.enabled = editEnable;
    }
    virtual void textEditSecure(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editSecured
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.secureTextEntry = editSecured;
    }
    virtual void keyboardType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardType keyboardType
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(keyboardType) {
            case v_ZFUITextEditKeyboardType::e_Normal:
                nativeImplView.keyboardType = UIKeyboardTypeDefault;
                break;
            case v_ZFUITextEditKeyboardType::e_CharBased:
                nativeImplView.keyboardType = UIKeyboardTypeASCIICapable;
                break;
            case v_ZFUITextEditKeyboardType::e_PhonePad:
                nativeImplView.keyboardType = UIKeyboardTypePhonePad;
                break;
            case v_ZFUITextEditKeyboardType::e_NumberPad:
                nativeImplView.keyboardType = UIKeyboardTypeNumberPad;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }
    virtual void keyboardReturnType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardReturnType keyboardReturnType
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(keyboardReturnType) {
            case v_ZFUITextEditKeyboardReturnType::e_Normal:
                nativeImplView.returnKeyType = UIReturnKeyDefault;
                break;
            case v_ZFUITextEditKeyboardReturnType::e_Next:
                nativeImplView.returnKeyType = UIReturnKeyNext;
                break;
            case v_ZFUITextEditKeyboardReturnType::e_Search:
                nativeImplView.returnKeyType = UIReturnKeySearch;
                break;
            case v_ZFUITextEditKeyboardReturnType::e_Done:
                nativeImplView.returnKeyType = UIReturnKeyDone;
                break;
            case v_ZFUITextEditKeyboardReturnType::e_Go:
                nativeImplView.returnKeyType = UIReturnKeyGo;
                break;
            case v_ZFUITextEditKeyboardReturnType::e_Send:
                nativeImplView.returnKeyType = UIReturnKeySend;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }

    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT ZFIndexRange &selectedRange
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextRange *rangeImpl = nativeImplView.selectedTextRange;
        if(rangeImpl == nil) {
            selectedRange = ZFIndexRangeZero();
            return;
        }
        NSInteger start = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.start];
        NSInteger end = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.end];
        selectedRange.start = start;
        selectedRange.count = end - start;
    }
    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFIndexRange &selectedRange
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextPosition *start = [nativeImplView positionFromPosition:nativeImplView.beginningOfDocument offset:selectedRange.start];
        UITextPosition *end = [nativeImplView positionFromPosition:start offset:selectedRange.count];
        [nativeImplView setSelectedTextRange:[nativeImplView textRangeFromPosition:start toPosition:end]];
    }

public:
    virtual void text(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &text
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.text = ZFImpl_sys_iOS_zfstringToNSString(text);
    }
    virtual void textAppearance(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
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
            case v_ZFUITextAppearance::e_BoldItalic:
                nativeImplView.fontName = @"Helvetica-BoldOblique";
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
    }
    virtual void textAlign(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
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
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIColor const &textColor
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textColor = ZFImpl_sys_iOS_ZFUIColorToUIColor(textColor);
    }
    virtual void textSize(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textSize = textSize;
    }

// ============================================================
// layout
public:
    virtual ZFUISize measureNativeTextEdit(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        zffloat textSizeSaved = nativeImplView.textSize;
        nativeImplView.textSize = textSize;
        CGSize sizeTmp = ZFImpl_sys_iOS_ZFUISizeToCGSize(sizeHint);
        if(sizeTmp.width <= 0) {
            sizeTmp.width = 30000;
        }
        if(sizeTmp.height <= 0) {
            sizeTmp.height = 30000;
        }
        sizeTmp = [nativeImplView sizeThatFits:sizeTmp];
        nativeImplView.textSize = textSizeSaved;
        return ZFImpl_sys_iOS_ZFUISizeFromCGSize(sizeTmp);
    }

public:
    virtual void editBegin(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView becomeFirstResponder];
    }
    virtual void editEnd(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView resignFirstResponder];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

