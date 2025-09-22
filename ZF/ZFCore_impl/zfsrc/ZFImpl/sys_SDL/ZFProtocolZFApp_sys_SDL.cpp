#include "ZFImpl_sys_SDL_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"

#if ZF_ENV_sys_SDL
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_sys_SDL, ZFApp, v_ZFProtocolLevel::e_SystemHigh)
public:
    zfoverride
    virtual void appExit(ZF_IN zfint appExitCode) {
        SDL_Event e;
        SDL_zero(e);
        e.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&e);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL

