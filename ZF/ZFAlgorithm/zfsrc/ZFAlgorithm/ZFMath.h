/**
 * @file ZFMath.h
 * @brief math function wrapper
 */

#ifndef _ZFI_ZFMath_h_
#define _ZFI_ZFMath_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief math function wrapper */
inline zffloat zfm_PI(void) {
    return 3.14159265359f;
}
/** @brief math function wrapper */
inline zffloat zfm_E(void) {
    return 2.71828182846f;
}

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zffloat, zfm_fmod
        , ZFMP_IN(zffloat, v)
        , ZFMP_IN(zffloat, mod)
        )

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zffloat, zfm_pow
        , ZFMP_IN(zffloat, x)
        , ZFMP_IN(zffloat, y)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_sqrt
        , ZFMP_IN(zffloat, v)
        )

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_sin
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_cos
        , ZFMP_IN(zffloat, v)
        )

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_asin
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_acos
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_atan
        , ZFMP_IN(zffloat, v)
        )

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zffloat, zfm_atan2
        , ZFMP_IN(zffloat, y)
        , ZFMP_IN(zffloat, x)
        )

/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_exp
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_log
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_log2
        , ZFMP_IN(zffloat, v)
        )
/** @brief math function wrapper */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, zffloat, zfm_log10
        , ZFMP_IN(zffloat, v)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMath_h_

