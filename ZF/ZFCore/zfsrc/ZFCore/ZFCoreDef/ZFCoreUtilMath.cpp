#include "ZFCoreUtilMath.h"
#include "ZFCoreStaticInitializer.h"
#include <ctime>
#include <cstdlib>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_REGISTER_INIT(ZFCoreUtilMathDataHolder) {
    srand((zfuint)time(zfnull));
    rand();
}
ZF_STATIC_REGISTER_END(ZFCoreUtilMathDataHolder)

zffloat zfmRandFloat(void) {
    return ((zffloat)rand()) / ((zffloat)RAND_MAX);
}

zfint zfmRandMax(void) {
    return (zfint)RAND_MAX;
}
zfint zfmRand(void) {
    return (zfint)rand();
}

ZF_NAMESPACE_GLOBAL_END

