#include "ZFCurve.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFCurve)

ZFMETHOD_DEFINE_1(ZFCurve, zffloat, curveUpdate
        , ZFMP_IN(zffloat, time)
        ) {
    return this->curveOnUpdate(time);
}

// ============================================================
ZFOBJECT_REGISTER(ZFCurveLinear)

ZF_NAMESPACE_GLOBAL_END

