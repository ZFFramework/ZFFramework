#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAniForNative.h"

#if ZF_ENV_sys_iOS

#import <QuartzCore/QuartzCore.h>

@class _ZFP_ZFAniForNativeImpl_sys_iOS_AniDelegate;

@interface _ZFP_ZFAniForNativeImpl_sys_iOS_Ani : CAAnimationGroup
@property (nonatomic, assign) ZFAniForNative *ownerAni;
@property (nonatomic, assign) zffloat nativeAniScale;
- (void)nativeAniStart;
- (void)nativeAniStop;

@property (nonatomic, strong) _ZFP_ZFAniForNativeImpl_sys_iOS_AniDelegate *_nativeAniDelegate;
@property (nonatomic, strong) NSString *_nativeAniKey;
- (void)_nativeAniOnStart:(zfidentity)aniId;
- (void)_nativeAniOnStop:(zfidentity)aniId finished:(BOOL)finished;
@end

@interface _ZFP_ZFAniForNativeImpl_sys_iOS_AniDelegate : NSObject<CAAnimationDelegate>
@property (nonatomic, weak) _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *owner;
@property (nonatomic, assign) zfidentity aniId;
@end
@implementation _ZFP_ZFAniForNativeImpl_sys_iOS_AniDelegate
- (void)animationDidStart:(CAAnimation *)anim {
    [self.owner _nativeAniOnStart:self.aniId];
}
- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag {
    [self.owner _nativeAniOnStop:self.aniId finished:flag];
}
@end

@implementation _ZFP_ZFAniForNativeImpl_sys_iOS_Ani
- (instancetype)init {
    self = [super init];
    if(self) {
        self.nativeAniScale = 1;

        self._nativeAniKey = [NSString stringWithFormat:@"_ZFP_%08X", (zfuint)[self hash]];

        self.removedOnCompletion = NO;
        self.fillMode = kCAFillModeForwards;
    }
    return self;
}
- (void)dealloc {
    self._nativeAniDelegate = nil;
    self._nativeAniKey = nil;
}
- (void)nativeAniStart {
    UIView *target = (__bridge UIView *)zfcast(ZFUIView *, self.ownerAni->target())->nativeView();
    CAMediaTimingFunction *nativeCurve = nil;
    switch(self.ownerAni->curve()) {
        case ZFAniForNativeCurve::e_Linear:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
            break;
        case ZFAniForNativeCurve::e_EaseIn:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn];
            break;
        case ZFAniForNativeCurve::e_EaseOut:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
            break;
        case ZFAniForNativeCurve::e_EaseInOut:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
            break;
        default:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
            break;
    }
    NSTimeInterval nativeDuration = (NSTimeInterval)self.ownerAni->durationFixed() / 1000;
    NSMutableArray *nativeAnimations = [NSMutableArray array];

    {
        CATransform3D transformFrom = CATransform3DIdentity;
        CATransform3D transformTo = CATransform3DIdentity;

        transformFrom = CATransform3DTranslate(
            transformFrom,
            self.ownerAni->translateXFrom() * self.nativeAniScale,
            self.ownerAni->translateYFrom() * self.nativeAniScale,
            self.ownerAni->translateZFrom() * self.nativeAniScale);
        transformTo = CATransform3DTranslate(
            transformTo,
            self.ownerAni->translateXTo() * self.nativeAniScale,
            self.ownerAni->translateYTo() * self.nativeAniScale,
            self.ownerAni->translateZTo() * self.nativeAniScale);

        if(self.ownerAni->rotateXFrom() != 0) {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->rotateXFrom() * M_PI / 180, 1, 0, 0);
        }
        if(self.ownerAni->rotateYFrom() != 0) {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->rotateYFrom() * M_PI / 180, 0, 1, 0);
        }
        if(self.ownerAni->rotateZFrom() != 0) {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->rotateZFrom() * M_PI / 180, 0, 0, 1);
        }
        if(self.ownerAni->rotateXTo() != 0) {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->rotateXTo() * M_PI / 180, 1, 0, 0);
        }
        if(self.ownerAni->rotateYTo() != 0) {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->rotateYTo() * M_PI / 180, 0, 1, 0);
        }
        if(self.ownerAni->rotateZTo() != 0) {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->rotateZTo() * M_PI / 180, 0, 0, 1);
        }

        if(self.ownerAni->scaleXFrom() != 1 || self.ownerAni->scaleYFrom() != 1) {
            transformFrom = CATransform3DScale(transformFrom, self.ownerAni->scaleXFrom(), self.ownerAni->scaleYFrom(), 1);
        }
        if(self.ownerAni->scaleXTo() != 1 || self.ownerAni->scaleYTo() != 1) {
            transformTo = CATransform3DScale(transformTo, self.ownerAni->scaleXTo(), self.ownerAni->scaleYTo(), 1);
        }

        CABasicAnimation *nativeTransformAni = [CABasicAnimation animationWithKeyPath:@"transform"];
        [nativeAnimations addObject:nativeTransformAni];
        nativeTransformAni.fromValue = [NSValue valueWithCATransform3D:transformFrom];
        nativeTransformAni.toValue = [NSValue valueWithCATransform3D:transformTo];
        nativeTransformAni.additive = YES;
    }

    if(self.ownerAni->alphaFrom() != 1 || self.ownerAni->alphaTo() != 1) {
        CABasicAnimation *nativeAlphaAni = [CABasicAnimation animationWithKeyPath:@"opacity"];
        [nativeAnimations addObject:nativeAlphaAni];
        nativeAlphaAni.fromValue = [NSNumber numberWithFloat:self.ownerAni->alphaFrom()];
        nativeAlphaAni.toValue = [NSNumber numberWithFloat:self.ownerAni->alphaTo()];
    }

    for(NSUInteger i = 0; i < [nativeAnimations count]; ++i) {
        CABasicAnimation *ani = (CABasicAnimation *)[nativeAnimations objectAtIndex:i];
        ani.duration = nativeDuration;
        ani.timingFunction = nativeCurve;
    }
    self.animations = nativeAnimations;
    self.duration = nativeDuration;
    self.timingFunction = nativeCurve;

    self._nativeAniDelegate = [_ZFP_ZFAniForNativeImpl_sys_iOS_AniDelegate new];
    self._nativeAniDelegate.owner = self;
    self._nativeAniDelegate.aniId = self.ownerAni->aniId();
    self.delegate = self._nativeAniDelegate;
    [target.layer addAnimation:self forKey:self._nativeAniKey];
}
- (void)nativeAniStop {
    UIView *target = (__bridge UIView *)zfcast(ZFUIView *, self.ownerAni->target())->nativeView();
    [target.layer removeAnimationForKey:self._nativeAniKey];
    self._nativeAniDelegate.owner = nil;
    self._nativeAniDelegate = nil;
    self.delegate = nil;
}

- (void)_nativeAniOnStart:(zfidentity)aniId {
}
- (void)_nativeAniOnStop:(zfidentity)aniId finished:(BOOL)finished {
    if(aniId == self.ownerAni->aniId()) {
        self.delegate = nil;
        UIView *target = (__bridge UIView *)zfcast(ZFUIView *, self.ownerAni->target())->nativeView();
        [target.layer removeAnimationForKey:self._nativeAniKey];
        ZFPROTOCOL_ACCESS(ZFAniForNative)->notifyAniStop(self.ownerAni);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAniForNativeImpl_sys_iOS, ZFAniForNative, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:CAAnimation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAniForNative *ani) {
        _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *nativeAni = [_ZFP_ZFAniForNativeImpl_sys_iOS_Ani new];
        nativeAni.ownerAni = ani;
        return (__bridge_retained void *)nativeAni;
    }
    virtual void nativeAniDestroy(
            ZF_IN ZFAniForNative *ani
            , ZF_IN void *nativeAni
            ) {
        NSObject *tmp = (__bridge_transfer NSObject *)nativeAni;
        tmp = nil;
    }

    virtual void nativeAniStart(
            ZF_IN ZFAniForNative *ani
            , ZF_IN zffloat nativeAniScale
            ) {
        _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *nativeAni = (__bridge _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *)ani->nativeAnimation();
        nativeAni.nativeAniScale = nativeAniScale;
        [nativeAni nativeAniStart];
    }
    virtual void nativeAniStop(ZF_IN ZFAniForNative *ani) {
        _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *nativeAni = (__bridge _ZFP_ZFAniForNativeImpl_sys_iOS_Ani *)ani->nativeAnimation();
        [nativeAni nativeAniStop];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAniForNativeImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

