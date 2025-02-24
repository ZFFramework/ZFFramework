#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_iOS

#import <objc/runtime.h>

@interface _ZFP_ZFUIViewImpl_sys_iOS_View : UIView
@property (nonatomic, assign) ZFUIView *_ZFP_ownerZFUIView;
@property (nonatomic, strong) UIView *_ZFP_nativeImplView;
@property (nonatomic, strong) NSMutableArray *_ZFP_mouseRecords; // UITouch
@property (nonatomic, assign) BOOL _ZFP_uiEnable;
@property (nonatomic, assign) BOOL _ZFP_ZFUIViewFocus_focusable;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_View
// ============================================================
// init and dealloc
- (id)init {
    self = [super init];

    self._ZFP_mouseRecords = [NSMutableArray array];

    self.autoresizesSubviews = NO;
    self.multipleTouchEnabled = YES;
    self.exclusiveTouch = YES;
    self.clipsToBounds = YES;

    // status init
    self._ZFP_uiEnable = YES;

    self._ZFP_ZFUIViewFocus_focusable = NO;

    return self;
}
- (void)dealloc {
    ZFCoreAssert(self._ZFP_nativeImplView == nil);
}

// ============================================================
// ui and tree enable
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *tmp = [super hitTest:point withEvent:event];
    if(!self._ZFP_uiEnable && tmp == self) {
        return nil;
    }
    else {
        return tmp;
    }
}

// ============================================================
// frame and layout
- (CGSize)sizeThatFits:(CGSize)size {
    return self.frame.size;
}
- (void)layoutSubviews {
    if(self._ZFP_ownerZFUIView != zfnull
        && self._ZFP_ownerZFUIView->layoutRequested()
        && (self._ZFP_ownerZFUIView->parent() == zfnull || !self._ZFP_ownerZFUIView->parent()->layoutRequested())
    ) {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(self._ZFP_ownerZFUIView, ZFImpl_sys_iOS_ZFUIRectFromCGRect(self.frame));
    }
}

// ============================================================
// touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesBegan:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull) {
        return;
    }

    for(UITouch *touch in touches) {
        [self._ZFP_mouseRecords addObject:touch];

        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = (zft_zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_Down;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_Left;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesMoved:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull) {
        return;
    }

    for(UITouch *touch in touches) {
        if([self._ZFP_mouseRecords indexOfObject:touch] == NSNotFound) {
            [self._ZFP_mouseRecords addObject:touch];
        }
    }

    for(UITouch *touch in self._ZFP_mouseRecords) {
        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = (zft_zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_Move;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_Left;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesEnded:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull) {
        return;
    }

    for(UITouch *touch in touches) {
        [self._ZFP_mouseRecords removeObject:touch];

        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = (zft_zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_Up;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_Left;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesCancelled:touches withEvent:event];

    if(self._ZFP_ownerZFUIView == zfnull) {
        return;
    }

    for(UITouch *touch in touches) {
        [self._ZFP_mouseRecords removeObject:touch];

        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = (zft_zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_Cancel;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIPointFromCGPoint([touch locationInView:self]);
        ev->mouseButton = ZFUIMouseButton::e_Left;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}

// ============================================================
// ZFUIViewFocus
- (BOOL)canBecomeFirstResponder {
    if(self._ZFP_nativeImplView != nil) {
        return [self._ZFP_nativeImplView canBecomeFirstResponder];
    }
    else {
        return self._ZFP_ZFUIViewFocus_focusable;
    }
}
- (BOOL)becomeFirstResponder {
    if(self._ZFP_nativeImplView != nil) {
        return [self._ZFP_nativeImplView becomeFirstResponder];
    }
    else {
        return [super becomeFirstResponder];
    }
}
- (BOOL)resignFirstResponder {
    if(self._ZFP_nativeImplView != nil) {
        return [self._ZFP_nativeImplView resignFirstResponder];
    }
    else {
        return [super resignFirstResponder];
    }
}
@end

static void _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusUpdate(ZF_IN UIView *nativeImplView) {
    _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = nil;
    if([nativeImplView isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]]) {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView;
    }
    else if([nativeImplView.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]]) {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview;
    }
    else if([nativeImplView.superview.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]]) {
        nativeView = (_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview.superview;
    }

    if(nativeView._ZFP_ownerZFUIView != zfnull) {
        ZFProtocolZFUIViewFocus_notifyViewFocusUpdate(nativeView._ZFP_ownerZFUIView);
    }
}

@implementation UIResponder (_ZFP_ZFUIViewImpl_sys_iOS_MethodSwizzling)
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder {
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder];
    if(!old && self.isFirstResponder && [self isKindOfClass:[UIView class]]) {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusUpdate((UIView *)self);
    }
    return ret;
}
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder {
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder];
    if(old && !self.isFirstResponder && [self isKindOfClass:[UIView class]]) {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusUpdate((UIView *)self);
    }
    return ret;
}
@end

static void _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare(void) {
    {
        Method methodOrig =  class_getInstanceMethod([UIResponder class], @selector(becomeFirstResponder));
        Method methodNew = class_getInstanceMethod([UIResponder class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder));
        method_exchangeImplementations(methodOrig, methodNew);
    }
    {
        Method methodOrig =  class_getInstanceMethod([UIResponder class], @selector(resignFirstResponder));
        Method methodNew = class_getInstanceMethod([UIResponder class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder));
        method_exchangeImplementations(methodOrig, methodNew);
    }
}

static __weak id _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = nil;
@interface UIResponder(_ZFP_ZFUIViewImpl_sys_iOS_FirstResponder)
+ (id)_ZFP_ZFUIViewImpl_sys_iOS_firstResponder;
@end
@implementation UIResponder(_ZFP_ZFUIViewImpl_sys_iOS_FirstResponder)
+ (id)_ZFP_ZFUIViewImpl_sys_iOS_firstResponder {
     _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = nil;
     [[UIApplication sharedApplication] sendAction:@selector(_ZFP_ZFUIViewImpl_sys_iOS_firstResponderFind:) to:nil from:nil forEvent:nil];
     return _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef;
}
-(void)_ZFP_ZFUIViewImpl_sys_iOS_firstResponderFind:(id)sender {
    _ZFP_ZFUIViewImpl_sys_iOS_firstResponderRef = self;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_iOS, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        static zfbool _needMethodSwizzling = zftrue;
        if(_needMethodSwizzling) {
            _needMethodSwizzling = zffalse;
            _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare();
        }
    }
    zfoverride
    virtual void protocolOnInitFinish(void) {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;

        nativeViewTmp.hidden = NO;
        nativeViewTmp.alpha = 1;
        nativeViewTmp._ZFP_uiEnable = YES;
        nativeViewTmp.userInteractionEnabled = YES;
        nativeViewTmp.backgroundColor = nil;
        nativeViewTmp._ZFP_ZFUIViewFocus_focusable = NO;
        nativeViewTmp.layer.transform = CATransform3DIdentity;

        [nativeViewTmp._ZFP_mouseRecords removeAllObjects];
        nativeViewTmp._ZFP_ownerZFUIView = zfnull;
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeView
            ) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;
        nativeViewTmp._ZFP_ownerZFUIView = view;
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = [_ZFP_ZFUIViewImpl_sys_iOS_View new];
        nativeView._ZFP_ownerZFUIView = view;
        return (__bridge_retained void *)nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeViewTmp = (__bridge_transfer _ZFP_ZFUIViewImpl_sys_iOS_View *)nativeView;
        nativeViewTmp._ZFP_ownerZFUIView = zfnull;
        nativeViewTmp = nil;
    }

    virtual void nativeImplView(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeImplViewOld
            , ZF_IN void *nativeImplView
            , ZF_IN zfindex virtualIndex
            , ZF_IN zfbool nativeImplViewRequireVirtualIndex
            ) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        if(nativeView._ZFP_nativeImplView != nil) {
            [nativeView._ZFP_nativeImplView removeFromSuperview];
        }
        nativeView._ZFP_nativeImplView = (__bridge UIView *)nativeImplView;

        if(nativeImplViewRequireVirtualIndex && nativeView._ZFP_nativeImplView != nil) {
            [nativeView insertSubview:nativeView._ZFP_nativeImplView atIndex:virtualIndex];
        }
    }
    virtual void nativeImplViewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        nativeView._ZFP_nativeImplView.frame = ZFImpl_sys_iOS_ZFUIRectToCGRect(rect);
    }
    virtual zffloat UIScaleForImpl(ZF_IN void *nativeView) {
        return 1;
    }
    virtual zffloat UIScaleForPixel(ZF_IN void *nativeView) {
        UIScreen *ownerScreen = ((__bridge UIView *)nativeView).window.screen;
        return (zffloat)((ownerScreen == nil) ? [UIScreen mainScreen].scale : ownerScreen.scale);
    }

    // ============================================================
    // properties
public:
    virtual void visible(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool visible
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).hidden = !visible;
    }
    virtual void alpha(
            ZF_IN ZFUIView *view
            , ZF_IN zffloat alpha
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).alpha = alpha;
    }
    virtual void viewUIEnable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnable
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView())._ZFP_uiEnable = viewUIEnable;
    }
    virtual void viewUIEnableTree(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnableTree
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).userInteractionEnabled = viewUIEnableTree;
    }
    virtual void bgColor(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIColor &bgColor
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).backgroundColor = ZFImpl_sys_iOS_ZFUIColorToUIColor(bgColor);
    }

public:
    virtual void child(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        [(__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)parent->nativeView()
            insertSubview:(__bridge UIView *)child->nativeView()
            atIndex:virtualIndex];
    }
    virtual void childRemove(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        [(__bridge UIView *)child->nativeView() removeFromSuperview];
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)parent->nativeView();
        NSArray *children = nativeView.subviews;
        if(!(
                    children.count == 0
                    || (nativeView._ZFP_nativeImplView != nil && children.count == 1)
                    )) {
            for(NSUInteger i = children.count - 1; i != (NSUInteger)-1; --i) {
                UIView *child = [children objectAtIndex:i];
                if(child != nativeView._ZFP_nativeImplView) {
                    [child removeFromSuperview];
                }
            }
        }
    }

public:
    virtual void viewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        ((__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView()).frame =
            ZFImpl_sys_iOS_ZFUIRectToCGRect(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        [nativeView setNeedsLayout];
    }

    virtual void measureNativeView(
            ZF_OUT ZFUISize &ret
            , ZF_IN void *nativeView
            , ZF_IN const ZFUISize &sizeHint
            ) {
        ret = ZFUISizeCreate(zfmMax(sizeHint.width, (zffloat)0), zfmMax(sizeHint.height, (zffloat)0));
        ZFImpl_sys_iOS_ZFUISizeFromCGSizeT(ret,
            [(__bridge UIView *)nativeView sizeThatFits:ZFImpl_sys_iOS_ZFUISizeToCGSize(ret)]);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_iOS)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_iOS, ZFUIViewFocus, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void focusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focusable
            ) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        nativeView._ZFP_ZFUIViewFocus_focusable = focusable;
        if(!focusable) {
            [nativeView resignFirstResponder];
        }
    }
    virtual zfbool focused(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        return (nativeView.isFirstResponder
            || (nativeView._ZFP_nativeImplView != nil && nativeView._ZFP_nativeImplView.isFirstResponder));
    }
    virtual ZFUIView *focusFind(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = (__bridge _ZFP_ZFUIViewImpl_sys_iOS_View *)view->nativeView();
        id focused = [UIResponder _ZFP_ZFUIViewImpl_sys_iOS_firstResponder];
        if(focused == nil || ![focused isKindOfClass:[UIView class]]) {
            return zfnull;
        }
        ZFUIView *ret = zfnull;
        UIView *t = (UIView *)focused;
        while(t != nil) {
            if(ret == zfnull && [t isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]]) {
                ret = ((_ZFP_ZFUIViewImpl_sys_iOS_View *)t)._ZFP_ownerZFUIView;
            }
            if(t == nativeView) {
                return ret;
            }
            t = t.superview;
        }
        return zfnull;
    }
    virtual void focusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focus
            ) {
        UIView *nativeView = (__bridge UIView *)view->nativeView();
        if(focus) {
            [nativeView becomeFirstResponder];
        }
        else {
            [nativeView resignFirstResponder];
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

