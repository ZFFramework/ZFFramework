
// added by ZFFramework
// system() not available since iOS11
#include "ZFImpl/sys_iOS/ZFImpl_sys_iOS_ZF_impl.h"
#if ZF_ENV_sys_iOS
#define system(...) (-1)
#endif

