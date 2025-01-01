#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"

#if ZF_ENV_sys_iOS
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// util method
zfbool ZFImpl_sys_iOS_UIColorToARGB(
        ZF_IN_OUT zffloat *pARGB
        , ZF_IN UIColor *color
        ) {
    for(zfindex i = 0; i < 4; ++i) {
        pARGB[i] = 0;
    }
    if(color == nil) {
        return zftrue;
    }
    zfindex numComponents = CGColorGetNumberOfComponents(color.CGColor);
    if(numComponents == 4) {
        const CGFloat *buf = CGColorGetComponents([color CGColor]);
        pARGB[0] = (zffloat)buf[3];
        for(zfindex i = 1; i < 4; ++i) {
            pARGB[i] = (zffloat)buf[i - 1];
        }
    }
    else {
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char buf[5] = {0};
        CGContextRef context = CGBitmapContextCreate(
            &buf,
            1,
            1,
            8,
            4,
            rgbColorSpace,
            kCGImageAlphaNoneSkipLast);
        CGContextSetFillColorWithColor(context, [color CGColor]);
        CGContextFillRect(context, CGRectMake(0, 0, 1, 1));
        CGContextRelease(context);
        CGColorSpaceRelease(rgbColorSpace);
        for (zfindex i = 0; i < 4; ++i) {
            pARGB[i] = (zfuint)buf[i] / 255.0f;
        }
    }

    return zftrue;
}

static void _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, UIView *view, zfindex depth, zfindex siblingIndex) {
    zfstringAppend(s, "|%2d ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i) {
        s += "| ";
    }

    NSString *viewInfo = [NSString stringWithFormat:@"<%@ %08X>", [view class], (zfuint)[view hash]];
    viewInfo = [viewInfo stringByAppendingFormat:@" (%d, %d, %d, %d)"
        , (int)zfmRound(view.frame.origin.x)
        , (int)zfmRound(view.frame.origin.y)
        , (int)zfmRound(view.frame.size.width)
        , (int)zfmRound(view.frame.size.height)
    ];

#if 0 // test
#define _TEST_PROPERTY
    if([view respondsToSelector:@selector(_TEST_PROPERTY)]) {
        id value = [view performSelector:@selector(_TEST_PROPERTY)];
        if(value != nil) {
            viewInfo = [viewInfo stringByAppendingFormat:@" %s:%@", ZFM_TOSTRING(_TEST_PROPERTY), value];
        }
    }
#endif // test

#if 1 // scroll content
    if([view isKindOfClass:[UIScrollView class]]) {
        UIScrollView *scrollView = (UIScrollView *)view;
        zfstring info = zfstr("(%d, %d, %d, %d)"
            , (zfint)zfmRound(scrollView.contentOffset.x)
            , (zfint)zfmRound(scrollView.contentOffset.y)
            , (zfint)zfmRound(scrollView.contentSize.width)
            , (zfint)zfmRound(scrollView.contentSize.height)
            );
        viewInfo = [viewInfo stringByAppendingFormat:@" scroll:%@", [NSString stringWithUTF8String:info]];
    }
#endif // scroll content

#if 1 // bg
    if(view.backgroundColor != nil) {
        zfstring colorInfo = ZFUIColorToString(ZFImpl_sys_iOS_ZFUIColorFromUIColor(view.backgroundColor));
        viewInfo = [viewInfo stringByAppendingFormat:@" bg:%@", [NSString stringWithUTF8String:colorInfo]];
    }
#endif // bg

#if 1 // hidden
    if(view.hidden) {
        viewInfo = [viewInfo stringByAppendingString:@" hidden"];
    }
#endif // hidden

#if 1 // UIDisabled
    if(!view.userInteractionEnabled) {
        viewInfo = [viewInfo stringByAppendingString:@" UIDisabled"];
    }
#endif // UIDisabled

#if 1 // text
    if([view respondsToSelector:@selector(text)]) {
        id value = [view performSelector:@selector(text)];
        if(value != nil) {
            viewInfo = [viewInfo stringByAppendingFormat:@" text:\"%@\"", value];
        }
    }
#endif // text

#if 1 // title
    if([view respondsToSelector:@selector(currentTitle)]) {
        id value = [view performSelector:@selector(currentTitle)];
        if(value != nil) {
            viewInfo = [viewInfo stringByAppendingFormat:@" currentTitle:%@", value];
        }
    }
    else if([view respondsToSelector:@selector(titleLabel)]) {
        id value = ((UILabel *)[view performSelector:@selector(titleLabel)]).text;
        if(value != nil) {
            viewInfo = [viewInfo stringByAppendingFormat:@" titleLabel:%@", value];
        }
    }
#endif // title

    s += [viewInfo UTF8String];
    s += "\n";

    NSArray *subviews = view.subviews;
    for(NSUInteger i = 0; i < [subviews count]; ++i) {
        _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(s, [subviews objectAtIndex:i], depth + 1, i);
    }
}
void ZFImpl_sys_iOS_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN UIView *view
        ) {
    ret += "==================== UIView tree begin ====================\n";
    if(view != nil) {
        @autoreleasepool {
            _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(ret, view, 0, 0);
        }
    }
    ret += "==================== UIView tree  end  ====================\n";
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUISysWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_iOS_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUISysWindow *sysWindow = zfargs.sender();
            zfstring s;
            ZFImpl_sys_iOS_viewTreePrintT(s, (__bridge UIView *)sysWindow->rootView()->nativeView());
            ZFLogTrim() << s;
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_iOS_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_iOS_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_iOS

