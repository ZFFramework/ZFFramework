#include "ZFImpl_default_ZF_impl.h"
#include "ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFImpl/ZFImpl_env.h"

#if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS && !ZF_ENV_sys_Qt
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
#endif // #if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS && !ZF_ENV_sys_Qt

