#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileCwd.h"

#if ZF_ENV_sys_iOS
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileCwdImpl_sys_iOS, ZFFileCwd, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:SandboxPath")
public:
    virtual zfstring pathForCwd(void) {
        zfstring _pathForCwd;
        ZFImpl_sys_iOS_zfstringFromNSString(_pathForCwd, [NSFileManager defaultManager].currentDirectoryPath);
        return _pathForCwd;
    }
    virtual zfbool pathForCwd(ZF_IN const zfstring &pathForCwd) {
        return ([[NSFileManager defaultManager] changeCurrentDirectoryPath:ZFImpl_sys_iOS_zfstringToNSString(pathForCwd)] == YES);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileCwdImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

