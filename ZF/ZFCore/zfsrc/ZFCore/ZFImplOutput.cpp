#include "ZFImplOutput.h"
#include "protocol/ZFProtocolZFImplOutput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// core log
ZFCoreArray<ZFImplOutputCoreLogCallback> &_ZFP_ZFImplOutputCoreLogCallbackList(void) {
    static ZFCoreArray<ZFImplOutputCoreLogCallback> d;
    return d;
}
void ZFImplOutputCoreLog(ZF_IN const zfchar *src) {
    static ZFCoreArray<ZFImplOutputCoreLogCallback> &d = ZFImplOutputCoreLogCallbackList;
    for(zfindex i = 0; i < d.count(); ++i) {
        d[i](src);
    }

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
ZFCoreArray<ZFImplOutputCallback> &_ZFP_ZFImplOutputCallbackList(void) {
    static ZFCoreArray<ZFImplOutputCallback> d;
    return d;
}
void ZFImplOutput(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    static ZFCoreArray<ZFImplOutputCallback> &d = ZFImplOutputCallbackList;
    for(zfindex i = 0; i < d.count(); ++i) {
        d[i](src, srcLen);
    }

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

