/**
 * @file ZFCurve.h
 * @brief time based event driver
 */

#ifndef _ZFI_ZFCurve_common_h_
#define _ZFI_ZFCurve_common_h_

#include "ZFBezier.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseInOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceInOut().y_by_x(time);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCurve_common_h_

