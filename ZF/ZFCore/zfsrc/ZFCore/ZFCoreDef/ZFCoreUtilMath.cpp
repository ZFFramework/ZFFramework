#include "ZFCoreUtilMath.h"
#include "ZFCoreStaticInitializer.h"
#include <ctime>
#include <cstdlib>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_REGISTER_INIT(ZFCoreUtilMathDataHolder) {
    srand((zfuint)time(zfnull));
}
ZF_STATIC_REGISTER_END(ZFCoreUtilMathDataHolder)

zfuint zfmRand(void) {
    return (zfuint)rand();
}

ZF_NAMESPACE_GLOBAL_END

