#include "ZFMath.h"
#include <cmath>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zffloat, zfm_PI)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zffloat, zfm_E)

ZFMETHOD_FUNC_DEFINE_2(zffloat, zfm_fmod
        , ZFMP_IN(zffloat, v)
        , ZFMP_IN(zffloat, mod)
        ) {
    return (zffloat)fmodf(v, mod);
}

ZFMETHOD_FUNC_DEFINE_2(zffloat, zfm_pow
        , ZFMP_IN(zffloat, x)
        , ZFMP_IN(zffloat, y)
        ) {
    return (zffloat)pow(x, y);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_sqrt
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)sqrt(v);
}

ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_sin
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)sin(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_cos
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)cos(v);
}

ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_asin
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)asin(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_acos
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)acos(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_atan
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)atan(v);
}

ZFMETHOD_FUNC_DEFINE_2(zffloat, zfm_atan2
        , ZFMP_IN(zffloat, y)
        , ZFMP_IN(zffloat, x)
        ) {
    return (zffloat)atan2(y, x);
}

ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_exp
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)exp(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_log
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)log(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_log2
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)log2(v);
}
ZFMETHOD_FUNC_DEFINE_1(zffloat, zfm_log10
        , ZFMP_IN(zffloat, v)
        ) {
    return (zffloat)log10(v);
}

ZF_NAMESPACE_GLOBAL_END

