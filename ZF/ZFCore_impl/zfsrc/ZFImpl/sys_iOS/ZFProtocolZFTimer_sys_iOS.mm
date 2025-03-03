#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFTimerImpl_sys_iOS_TimerOwner : NSObject
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
@property (nonatomic, assign) ZFTimer *ownerZFTimer;
@property (nonatomic, strong) id _selfHolder;

- (void)startTimer:(NSNumber *)timerImplId;
- (void)stopTimer;

// private
@property (nonatomic, strong) NSTimer *_timer;
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer;
@end
@implementation _ZFP_ZFTimerImpl_sys_iOS_TimerOwner
- (id)zfnativeImpl {
    return self._timer;
}

- (void)startTimer:(NSNumber *)timerImplId {
    [self stopTimer];

    self._selfHolder = self;
    self._timer = [NSTimer timerWithTimeInterval:((zffloat)self.ownerZFTimer->interval() / 1000) target:self selector:@selector(_timerOwnerOnTimerEvent:) userInfo:timerImplId repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:self._timer forMode:NSRunLoopCommonModes];
    self._selfHolder = nil;
}
- (void)stopTimer {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    if(self._timer != nil) {
        [self._timer invalidate];
        self._timer = nil;
    }
}
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer {
    NSNumber *timerImplId = (NSNumber *)[timer userInfo];
    self.impl->notifyTimerActivate(self.ownerZFTimer, (zft_zfidentity)[timerImplId longValue]);
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_iOS, ZFTimer, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:NSObject:NSTimer")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *timerOwner = [_ZFP_ZFTimerImpl_sys_iOS_TimerOwner new];
        timerOwner.impl = this;
        timerOwner.ownerZFTimer = timer;
        return (__bridge_retained void *)timerOwner;
    }
    virtual void nativeTimerDestroy(
            ZF_IN ZFTimer *timer
            , ZF_IN void *nativeTimer
            ) {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *tmp = (__bridge_transfer _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *)nativeTimer;
        tmp = nil;
    }
    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *timerOwner = (__bridge _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *)timer->nativeTimer();
        [timerOwner startTimer:[NSNumber numberWithLong:(long)timerImplId]];
    }
    virtual void stop(ZF_IN ZFTimer *timer) {
        [(__bridge _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *)timer->nativeTimer() stopTimer];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

