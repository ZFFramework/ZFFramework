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

    zfstringAppend(s
            , "<%s %08X> (%s, %s, %s, %s)"
            , ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"%@", [view class]])
            , (zfuint)[view hash]
            , (zfint)zfmRound(view.frame.origin.x)
            , (zfint)zfmRound(view.frame.origin.y)
            , (zfint)zfmRound(view.frame.size.width)
            , (zfint)zfmRound(view.frame.size.height)
            );

#if 1 // scroll content
    if([view isKindOfClass:[UIScrollView class]]) {
        UIScrollView *scrollView = (UIScrollView *)view;
        zfstringAppend(s
                , " scroll:(%s, %s, %s, %s)"
                , (zfint)zfmRound(scrollView.contentOffset.x)
                , (zfint)zfmRound(scrollView.contentOffset.y)
                , (zfint)zfmRound(scrollView.contentSize.width)
                , (zfint)zfmRound(scrollView.contentSize.height)
                );
    }
#endif // scroll content

#if 1 // bg
    if(view.backgroundColor != nil) {
        zfstringAppend(s
                , " bg:%s"
                , ZFImpl_sys_iOS_ZFUIColorFromUIColor(view.backgroundColor)
                );
    }
#endif // bg

#if 1 // hidden
    if(view.hidden) {
        s += " hidden";
    }
#endif // hidden

#if 1 // UIDisabled
    if(!view.userInteractionEnabled) {
        s += " UIDisabled";
    }
#endif // UIDisabled

#if 1 // text
    if([view respondsToSelector:@selector(text)]) {
        id value = [view performSelector:@selector(text)];
        if(value != nil) {
            zfstringAppend(s
                    , " text:\"%s\""
                    , ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"%@", value])
                    );
        }
    }
#endif // text

#if 1 // title
    if([view respondsToSelector:@selector(currentTitle)]) {
        id value = [view performSelector:@selector(currentTitle)];
        if(value != nil) {
            zfstringAppend(s
                    , " currentTitle:\"%s\""
                    , ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"%@", value])
                    );
        }
    }
    else if([view respondsToSelector:@selector(titleLabel)]) {
        id value = ((UILabel *)[view performSelector:@selector(titleLabel)]).text;
        if(value != nil) {
            zfstringAppend(s
                    , " titleLabel:\"%s\""
                    , ZFImpl_sys_iOS_zfstringFromNSString([NSString stringWithFormat:@"%@", value])
                    );
        }
    }
#endif // title

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
    ret += "======================= iOS view tree ======================\n";
    if(view != nil) {
        @autoreleasepool {
            _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(ret, view, 0, 0);
        }
    }
    ret += "----------------------- iOS view tree ----------------------\n";
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUIRootWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_iOS_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUIRootWindow *rootWindow = zfargs.sender();
            zfstring s;
            ZFImpl_sys_iOS_viewTreePrintT(s, (__bridge UIView *)rootWindow->rootView()->nativeView());
            ZFLogTrim() << s;
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUIRootWindow::E_WindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_iOS_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUIRootWindow::E_WindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_iOS_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_iOS

