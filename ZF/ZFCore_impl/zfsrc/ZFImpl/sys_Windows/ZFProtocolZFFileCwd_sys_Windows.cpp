#include "ZFImpl_sys_Windows_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileCwd.h"
#include "ZFCore/zfstringW.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileCwdImpl_sys_Windows, ZFFileCwd, ZFProtocolLevel::e_SystemLow)
public:
    virtual zfstring pathForCwd(void) {
        zfstring _pathForCwd;
        wchar_t *buf = _wgetcwd(NULL, 0);
        if(buf) {
            zfstringToUTF8(_pathForCwd, buf, ZFStringEncoding::e_UTF16);
            free(buf);
            return _pathForCwd;
        }
        else {
            return zfnull;
        }
    }
    virtual zfbool pathForCwd(ZF_IN const zfstring &pathForCwd) {
        return (_wchdir(zfstringToUTF16(pathForCwd, ZFStringEncoding::e_UTF8).cString()) == 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileCwdImpl_sys_Windows)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Windows

