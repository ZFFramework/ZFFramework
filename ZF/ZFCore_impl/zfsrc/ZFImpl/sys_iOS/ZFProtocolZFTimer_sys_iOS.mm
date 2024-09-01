#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFTimerImpl_sys_iOS_TimerOwner : NSObject
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
@property (nonatomic, assign) ZFTimer *ownerZFTimer;
@property (nonatomic, strong) id _selfHolder;

- (void)startTimer;
- (void)stopTimer;

// private
@property (nonatomic, strong) NSTimer *_timer;
@property (nonatomic, assign) zfint _timerTaskId;
- (zfbool)_timerOwnerIsTaskIdValid:(NSNumber *)savedTimerTaskId;
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer;
@end
@implementation _ZFP_ZFTimerImpl_sys_iOS_TimerOwner
- (id)zfnativeImpl {
    return self._timer;
}

- (void)startTimer {
    [self stopTimer];

    self._selfHolder = self;
    NSNumber *taskId = [NSNumber numberWithInt:self._timerTaskId];
    self._timer = [NSTimer timerWithTimeInterval:((zffloat)self.ownerZFTimer->timerInterval() / 1000) target:self selector:@selector(_timerOwnerOnTimerEvent:) userInfo:taskId repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:self._timer forMode:NSRunLoopCommonModes];
    self._selfHolder = nil;
}
- (void)stopTimer {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    self._timerTaskId = self._timerTaskId + 1;
    if(self._timer != nil) {
        [self._timer invalidate];
        self._timer = nil;
    }
}
- (zfbool)_timerOwnerIsTaskIdValid:(NSNumber *)savedTimerTaskId {
    return (savedTimerTaskId != nil && self._timerTaskId == [savedTimerTaskId intValue]);
}
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer {
    NSNumber *taskId = (NSNumber *)[timer userInfo];
    if([self _timerOwnerIsTaskIdValid:taskId]) {
        self.impl->notifyTimerActivate(self.ownerZFTimer);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_iOS, ZFTimer, ZFProtocolLevel::e_SystemNormal)
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
    virtual void timerStart(ZF_IN ZFTimer *timer) {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *timerOwner = (__bridge _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *)timer->nativeTimer();
        [timerOwner startTimer];
    }
    virtual void timerStop(ZF_IN ZFTimer *timer) {
        [(__bridge _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *)timer->nativeTimer() stopTimer];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

