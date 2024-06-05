/**
 * @file ZFCurve_common.h
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
 * @brief ease out time line, see #ZFCurve
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
 * @brief ease in and out time line, see #ZFCurve
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
 * @brief bounce in time line, see #ZFCurve
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
 * @brief bounce out time line, see #ZFCurve
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
 * @brief bounce in and out time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceInOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief loop time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoop : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoop, ZFCurve)

    /** @brief the curve to loop */
    ZFPROPERTY_RETAIN(zfanyT<ZFCurve>, curve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        time = (time < 0.5f ? 2 * time : 2 * (1 - time));
        if(this->curve()) {
            return this->curve()->progressUpdate(time);
        }
        else {
            return time;
        }
    }
};
/**
 * @brief loop with linear time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopLinear : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopLinear, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return (time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with ease in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseIn().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with ease out time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseOut().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with ease in and out time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseInOut().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with bounce in time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceIn().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with bounce out time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceOut().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};
/**
 * @brief loop with bounce in and out time line, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceInOut().y_by_x(time < 0.5f ? 2 * time : 2 * (1 - time));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCurve_common_h_

