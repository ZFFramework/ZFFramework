#include "ZFImpl_sys_SDL_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFImpl/sys_SDL/ZFMainEntry_sys_SDL.h"

#include "ZFCore/ZFSTLWrapper/zfstllist.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

// for sdl, we just extend the main thread executor for the default thread impl

static zfstllist<ZFListener> _ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue;

static void _ZFP_ZFImpl_sys_SDL_MainThreadRunNext(void);
static void *_ZFP_ZFImpl_sys_SDL_MainThreadExecute(ZF_IN const ZFListener &runnable) {
    ZFCoreMutexLock();
    _ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.push_back(runnable);
    zfstllist<ZFListener>::iterator it = _ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.end();
    --it;
    ZFCoreMutexUnlock();
    _ZFP_ZFImpl_sys_SDL_MainThreadRunNext();
    return new zfstllist<ZFListener>::iterator(it);
}
static void _ZFP_ZFImpl_sys_SDL_MainThreadCleanup(ZF_IN void *nativeToken) {
    ZFCoreMutexLocker();
    zfstllist<ZFListener>::iterator *it = (zfstllist<ZFListener>::iterator *)nativeToken;
    _ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.erase(*it);
    delete it;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_SDL_MainThreadImpl, ZFLevelZFFrameworkStatic) {
    ZFMainThreadTaskImplSet(_ZFP_ZFImpl_sys_SDL_MainThreadExecute, _ZFP_ZFImpl_sys_SDL_MainThreadCleanup);
}
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_SDL_MainThreadImpl)

// ============================================================
ZFIMPL_SYS_SDL_USER_EVENT_HANDLER(MainThreadTask, ZFLevelZFFrameworkNormal) {
    ZFCoreMutexLock();
    if(_ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.empty()) {
        ZFCoreMutexUnlock();
        return zftrue;
    }
    ZFListener runnable = *(_ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.begin());
    ZFCoreMutexUnlock();
    runnable.execute();
    if(!_ZFP_ZFImpl_sys_SDL_MainThreadTaskQueue.empty()) {
        _ZFP_ZFImpl_sys_SDL_MainThreadRunNext();
    }
    return zftrue;
}
static void _ZFP_ZFImpl_sys_SDL_MainThreadRunNext(void) {
    ZFIMPL_SYS_SDL_USER_EVENT_POST(MainThreadTask, zfnull, zfnull);
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

