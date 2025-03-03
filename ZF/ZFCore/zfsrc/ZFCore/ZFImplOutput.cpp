#include "ZFImplOutput.h"
#include "protocol/ZFProtocolZFImplOutput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// core log
void ZFImplOutputCoreLog(ZF_IN const zfchar *src) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFImplOutput) *impl = ZFPROTOCOL_TRY_ACCESS(ZFImplOutput);
    if(impl == zfnull) {
        // try to print to std output
        fprintf(stderr, "%s", src);
    }
    else {
        impl->outputCoreLog(src);
    }
}

// ============================================================
// normal log
void ZFImplOutput(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFImplOutput) *impl = ZFPROTOCOL_TRY_ACCESS(ZFImplOutput);
    if(impl == zfnull) {
        if(srcLen == zfindexMax()) {
            printf("%s", src);
        }
        else {
            printf("%s", zfstring(src, srcLen).cString());
        }
    }
    else {
        impl->outputLog(src, srcLen);
    }
}

ZF_NAMESPACE_GLOBAL_END

