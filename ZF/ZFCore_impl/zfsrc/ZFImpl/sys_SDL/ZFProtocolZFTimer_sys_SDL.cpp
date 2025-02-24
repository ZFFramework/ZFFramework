#include "ZFImpl_sys_SDL_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_SDL

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFTimerImpl_sys_SDL_TimerData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFTimerImpl_sys_SDL_TimerData, ZFObject)
public:
    zfautoT<ZFTimer> timer;
    zfidentity timerImplId;
    SDL_TimerID nativeTimerId;
    ZFListener timerMainThreadListener;
    zfautoT<ZFSemaphore> timerMainThreadSema;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_SDL, ZFTimer, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("SDL:Timer")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer) {
        return zfAlloc(_ZFP_I_ZFTimerImpl_sys_SDL_TimerData);
    }
    virtual void nativeTimerDestroy(
            ZF_IN ZFTimer *timer
            , ZF_IN void *nativeTimer
            ) {
        zfRelease((_ZFP_I_ZFTimerImpl_sys_SDL_TimerData *)nativeTimer);
    }
    virtual void start(
            ZF_IN ZFTimer *timer
            , ZF_IN zfidentity timerImplId
            ) {
        _ZFP_I_ZFTimerImpl_sys_SDL_TimerData *nativeTimer = (_ZFP_I_ZFTimerImpl_sys_SDL_TimerData *)timer->nativeTimer();
        nativeTimer->timer = timer;
        nativeTimer->timerImplId = timerImplId;
        nativeTimer->nativeTimerId = SDL_AddTimer((Uint32)timer->interval(), _ZFP_timerCallback, nativeTimer);
    }
    virtual void stop(ZF_IN ZFTimer *timer) {
        _ZFP_I_ZFTimerImpl_sys_SDL_TimerData *nativeTimer = (_ZFP_I_ZFTimerImpl_sys_SDL_TimerData *)timer->nativeTimer();
        SDL_RemoveTimer(nativeTimer->nativeTimerId);
    }
private:
    static Uint32 _ZFP_timerCallback(Uint32 interval, void *param) {
        _ZFP_I_ZFTimerImpl_sys_SDL_TimerData *nativeTimer = (_ZFP_I_ZFTimerImpl_sys_SDL_TimerData *)param;
        zfblockedRelease(zfRetain(nativeTimer));
        if(nativeTimer->timer->eventOnMainThread()) {
            if(!nativeTimer->timerMainThreadListener) {
                ZFLISTENER_1(mainThreadCallback
                        , zfautoT<_ZFP_I_ZFTimerImpl_sys_SDL_TimerData>, nativeTimer
                        ) {
                    ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerActivate(nativeTimer->timer, nativeTimer->timerImplId);
                    nativeTimer->timerMainThreadSema->lockAndBroadcast();
                } ZFLISTENER_END()
                nativeTimer->timerMainThreadListener = mainThreadCallback;
            }
            if(!nativeTimer->timerMainThreadSema) {
                nativeTimer->timerMainThreadSema = zfobj<ZFSemaphore>();
            }
            zfpost(nativeTimer->timerMainThreadListener);
            nativeTimer->timerMainThreadSema->lockAndWait();
        }
        else {
            ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerActivate(nativeTimer->timer, nativeTimer->timerImplId);
        }
        return interval;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL

