#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"
#include <cstdio>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_default, ZFImplOutput, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("C:printf")
public:
    virtual void outputCoreLog(ZF_IN const zfchar *s) {
        printf("%s", s);
        #if ZF_ENV_DEBUG
            fflush(stdout);
        #endif
    }
    virtual void outputLog(
            ZF_IN const zfchar *s
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        if(count == zfindexMax()) {
            printf("%s", s);
        }
        else {
            printf("%s", zfstring(s, count).cString());
        }
        #if ZF_ENV_DEBUG
            fflush(stdout);
        #endif
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_default)

ZF_NAMESPACE_GLOBAL_END

