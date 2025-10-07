#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"
#include <cstdio>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_default, ZFImplOutput, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("C:printf")
public:
    virtual void outputCoreLog(ZF_IN const zfchar *s) {
        printf("%s", s);
        fflush(stdout);
    }
    virtual void outputLog(
            ZF_IN const zfchar *s
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        if(count == zfindexMax() || s[count] == '\0') {
            printf("%s", s);
            if(zfstringFind(s, count == zfindexMax() ? zfslen(s) : count, "\n") >= 0) {
                fflush(stdout);
            }
        }
        else {
            printf("%s", zfstring(s, count).cString());
            if(zfstringFind(s, count, "\n") >= 0) {
                fflush(stdout);
            }
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_default)

ZF_NAMESPACE_GLOBAL_END

