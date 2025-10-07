#define SDL_MAIN_HANDLED

#include "ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ZFFrameworkInit();

    ZFCoreArray<zfstring> params;
    ZFCoreArgSplit(params, lpCmdLine);
    zfint ret = ZFMainExecute(params);

    ZFFrameworkCleanup();
    return (int)ret;
}
#else // #if ZF_ENV_sys_Windows
int main(int argc, char **argv) {
    ZFFrameworkInit();

    ZFCoreArray<zfstring> params;
    for(int i = 1; i < argc; ++i) {
        params.add(argv[i]);
    }
    zfint ret = ZFMainExecute(params);

    ZFFrameworkCleanup();
    return (int)ret;
}
#endif // #if ZF_ENV_sys_Windows

