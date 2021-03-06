#include "ZFImpl_default_ZF_impl.h"
#include "ZFCore.h"

#if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS && !ZF_ENV_sys_Qt

#if !ZF_ENV_sys_WindowsCE
int main(int argc, char **argv)
#else // #if ZF_ENV_sys_WindowsCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#endif // #if ZF_ENV_sys_WindowsCE #else
{

#if !ZF_ENV_sys_WindowsCE
    return ZFMainCommonEntry(argc, argv);
#else
    ZFFrameworkInit();
    ZFCoreArray<zfstring> params;
    zfCoreArgSplit(params, zfstringToUTF8(lpCmdLine, ZFStringEncoding::e_UTF16).cString());
    zfint result = ZFMainExecute(params);
    ZFFrameworkCleanup();
    return result;
#endif // #if ZF_ENV_sys_WindowsCE #else
}

#endif // #if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS

