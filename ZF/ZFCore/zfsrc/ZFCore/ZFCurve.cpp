#include "ZFCurve.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFCurve)

ZFMETHOD_DEFINE_1(ZFCurve, zffloat, progressUpdate
        , ZFMP_IN(zffloat, time)
        ) {
    return this->progressOnUpdate(time);
}

// ============================================================
ZFOBJECT_REGISTER(ZFCurveLinear)

ZF_NAMESPACE_GLOBAL_END

