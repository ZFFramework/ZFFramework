#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindow.h"
#include "ZFImpl/sys_iOS/ZFMainEntry_sys_iOS.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow_Dummy : UIViewController
@property (nonatomic, weak) UIViewController *_ZFP_owner;
@end
@implementation _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow_Dummy
- (BOOL)shouldAutorotate {
    return [self._ZFP_owner shouldAutorotate];
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return [self._ZFP_owner supportedInterfaceOrientations];
}
@end

@interface _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow : UIViewController
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIRootWindow) *impl;
@property (nonatomic, assign) ZFUIRootWindow *ownerZFUIRootWindow;
@property (nonatomic, assign) ZFUIOrientation windowOrientation;
@property (nonatomic, assign) ZFUIOrientationFlags windowOrientationFlags;
@property (nonatomic, assign) zfint _ZFP_windowRotateOverrideFlag;
- (void)_ZFP_updateLayout;
@end
@implementation _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow
- (void)setWindowOrientationFlags:(ZFUIOrientationFlags)windowOrientationFlags {
    if(self->_windowOrientationFlags == windowOrientationFlags) {
        return;
    }
    self->_windowOrientationFlags = windowOrientationFlags;

    [UIViewController attemptRotationToDeviceOrientation];
#if 1 // force to refresh orientation state
    if(self.ownerZFUIRootWindow != zfnull && self.ownerZFUIRootWindow->nativeWindowIsResumed()) {
        self._ZFP_windowRotateOverrideFlag += 1;
        _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow_Dummy *dummy = [_ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow_Dummy new];
        UIViewController *t = self;
        while(t.presentedViewController != nil) {
            t = t.presentedViewController;
        }
        dummy._ZFP_owner = t;
        [t presentViewController:dummy animated:NO completion:nil];
        __block _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *weakSelf = self;
        [t dismissViewControllerAnimated:NO completion:^{
            weakSelf._ZFP_windowRotateOverrideFlag -= 1;
            if(weakSelf.ownerZFUIRootWindow != zfnull) {
                if(!weakSelf.ownerZFUIRootWindow->nativeWindowIsResumed()) {
                    weakSelf.impl->notifyOnResume(weakSelf.ownerZFUIRootWindow);
                }
            }
        }];
    }
#endif
}
// ============================================================
- (void)_ZFP_updateLayout {
    if(self.ownerZFUIRootWindow != zfnull) {
        ZFUIMargin windowMargin = ZFUIMarginZero();

        UIView *tmpView = [UIApplication sharedApplication].delegate.window.rootViewController.view;
        if([tmpView respondsToSelector:@selector(safeAreaInsets)]) {
            NSMethodSignature *methodSig = [tmpView methodSignatureForSelector:@selector(safeAreaInsets)];
            NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
            invocation.target = tmpView;
            invocation.selector = @selector(safeAreaInsets);
            [invocation invoke];
            UIEdgeInsets t = UIEdgeInsetsZero;
            [invocation getReturnValue:&t];

            windowMargin.left = t.left;
            windowMargin.top = t.top;
            windowMargin.right = t.right;
            windowMargin.bottom = t.bottom;
        }
        else {
            windowMargin.top = [UIApplication sharedApplication].delegate.window.windowScene.statusBarManager.statusBarFrame.size.height;
        }

        UIView *nativeRootView = (__bridge UIView *)self.ownerZFUIRootWindow->rootView()->nativeView();
        nativeRootView.frame = ZFImpl_sys_iOS_ZFUIRectToCGRect(self.impl->notifyMeasureWindow(
            self.ownerZFUIRootWindow,
            ZFImpl_sys_iOS_ZFUIRectFromCGRect(self.view.bounds),
            windowMargin));
    }
}
// ============================================================
// init and dealloc
- (id)init {
    self = [super init];
    if(self) {
        self.windowOrientation = v_ZFUIOrientation::e_Top;
        self->_windowOrientationFlags = v_ZFUIOrientation::e_Top;
    }
    return self;
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    UIInterfaceOrientation toInterfaceOrientation = [UIApplication sharedApplication].delegate.window.windowScene.interfaceOrientation;
    switch(toInterfaceOrientation) {
        case UIInterfaceOrientationPortrait:
            self.windowOrientation = v_ZFUIOrientation::e_Top;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            self.windowOrientation = v_ZFUIOrientation::e_Bottom;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            self.windowOrientation = v_ZFUIOrientation::e_Left;
            break;
        case UIInterfaceOrientationLandscapeRight:
            self.windowOrientation = v_ZFUIOrientation::e_Right;
            break;
        default:
            self.windowOrientation = v_ZFUIOrientation::e_Top;
            break;
    }
    if(self.ownerZFUIRootWindow != zfnull) {
        self.impl->notifyOnRotate(self.ownerZFUIRootWindow);
    }
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    if(self.presentingViewController != nil) {
        return [self.presentingViewController supportedInterfaceOrientations];
    }

    UIInterfaceOrientationMask ret = 0;
    if(ZFBitTest(self.windowOrientationFlags, v_ZFUIOrientation::e_Top)) {
        ret |= UIInterfaceOrientationMaskPortrait;
    }
    if(ZFBitTest(self.windowOrientationFlags, v_ZFUIOrientation::e_Left)) {
        ret |= UIInterfaceOrientationMaskLandscapeLeft;
    }
    if(ZFBitTest(self.windowOrientationFlags, v_ZFUIOrientation::e_Right)) {
        ret |= UIInterfaceOrientationMaskLandscapeRight;
    }
    if(ZFBitTest(self.windowOrientationFlags, v_ZFUIOrientation::e_Bottom)) {
        ret |= UIInterfaceOrientationMaskPortraitUpsideDown;
    }
    if(ret == 0) {
        ret = UIInterfaceOrientationMaskPortrait;
    }
    return ret;
}
- (BOOL)shouldAutorotate {
    if(self.presentingViewController != nil) {
        return [self.presentingViewController shouldAutorotate];
    }

    return YES;
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    [self _ZFP_updateLayout];
}
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    if(self.ownerZFUIRootWindow != zfnull && self._ZFP_windowRotateOverrideFlag == 0) {
        if(!self.ownerZFUIRootWindow->nativeWindowIsResumed()) {
            self.impl->notifyOnResume(self.ownerZFUIRootWindow);
        }
    }
}
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    if(self.ownerZFUIRootWindow != zfnull && self._ZFP_windowRotateOverrideFlag == 0) {
        if(self.ownerZFUIRootWindow->nativeWindowIsResumed()) {
            self.impl->notifyOnPause(self.ownerZFUIRootWindow);
        }
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIRootWindowImpl_sys_iOS, ZFUIRootWindow, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIViewController")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUIRootWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfRetain(ZFUIRootWindow::ClassData()->newInstance().to<ZFUIRootWindow *>());
            _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *nativeWindow = [_ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow new];
            nativeWindow.ownerZFUIRootWindow = this->_mainWindow;

            // delay to create, because:
            // * allow app to change orientation before actually show,
            //   prevent strange rotate animation during launch
            // * during app launch, chaning window orientation would cause
            //   "Unbalanced calls to begin/end appearance transitions"
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                this->notifyOnCreate(this->_mainWindow, (__bridge_retained void *)nativeWindow);

                ZFImpl_sys_iOS_rootWindow().rootViewController = nativeWindow;
                [ZFImpl_sys_iOS_rootWindow() makeKeyAndVisible];
            });
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge_transfer _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *)this->_mainWindow->nativeWindow();
            nativeWindow.ownerZFUIRootWindow = zfnull;
            if(ZFImpl_sys_iOS_rootWindow().rootViewController == nativeWindow) {
                ZFImpl_sys_iOS_rootWindow().rootViewController = nil;
            }

            if(this->_mainWindow->nativeWindowIsResumed()) {
                this->notifyOnPause(this->_mainWindow);
            }
            zfscopeRelease(this->_mainWindow);
            this->notifyOnDestroy(this->_mainWindow);
            this->_mainWindow = zfnull;
            nativeWindow = nil;
        }
    }
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *)rootWindow->nativeWindow();

        UIView *nativeRootView = (__bridge UIView *)rootWindow->rootView()->nativeView();
        [nativeWindow.view addSubview:nativeRootView];
        [nativeWindow _ZFP_updateLayout];
        nativeParentView = (__bridge void *)nativeWindow.view;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        UIView *nativeRootView = (__bridge UIView *)rootWindow->rootView()->nativeView();
        [nativeRootView removeFromSuperview];
    }

    virtual zfauto modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
        if(owner->nativeWindow() == zfnull) {
            return zfnull;
        }
        zfauto modalWindow = ZFUIRootWindow::ClassData()->newInstance();
        _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *nativeModalWindow = [_ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow new];
        nativeModalWindow.ownerZFUIRootWindow = modalWindow;

        this->notifyOnCreate(modalWindow, (__bridge_retained void *)nativeModalWindow);
        [(__bridge UIViewController *)owner->nativeWindow() presentViewController:nativeModalWindow animated:YES completion:nil];

        return modalWindow;
    }
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) {
        if(toHide->nativeWindow() == zfnull) {
            return;
        }
        [(__bridge UIViewController *)owner->nativeWindow() dismissViewControllerAnimated:YES completion:nil];
        this->notifyOnDestroy(toHide);
    }

    virtual void layoutParamOnInit(ZF_IN ZFUIRootWindow *rootWindow) {
    }
    virtual void layoutParamOnUpdate(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        [(__bridge _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *)rootWindow->nativeWindow() _ZFP_updateLayout];
    }

    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) {
        return ((__bridge _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *)rootWindow->nativeWindow()).windowOrientation;
    }
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        ((__bridge _ZFP_ZFUIRootWindowImpl_sys_iOS_NativeWindow *)rootWindow->nativeWindow()).windowOrientationFlags = flags;
    }

private:
    ZFUIRootWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIRootWindowImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

