/**
 * @file ZFCurve.h
 * @brief curve
 */

#ifndef _ZFI_ZFCurve_h_
#define _ZFI_ZFCurve_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief curve
 */
zffinal zfclassLikePOD ZFLIB_ZFAlgorithm ZFCurve {
public:
    /** @brief construct a linear curve */
    ZFCurve(void) {
        this->controlPoint(0, 0, 1, 1);
    }
    /** @brief construct from another curve */
    ZFCurve(ZF_IN const ZFCurve &ref)
    : p0x(ref.p0x)
    , p0y(ref.p0y)
    , p1x(ref.p1x)
    , p1y(ref.p1y)
    , ax(ref.ax)
    , bx(ref.bx)
    , cx(ref.cx)
    , ay(ref.ay)
    , by(ref.by)
    , cy(ref.cy)
    {
    }
    /** @brief construct with control point */
    ZFCurve(
            ZF_IN zffloat p0x
            , ZF_IN zffloat p0y
            , ZF_IN zffloat p1x
            , ZF_IN zffloat p1y
            ) {
        this->controlPoint(p0x, p0y, p1x, p1y);
    }
    /** @cond ZFPrivateDoc */
    ZFCurve &operator = (ZF_IN const ZFCurve &ref) {
        this->p0x = ref.p0x;
        this->p0y = ref.p0y;
        this->p1x = ref.p1x;
        this->p1y = ref.p1y;
        this->ax = ref.ax;
        this->bx = ref.bx;
        this->cx = ref.cx;
        this->ay = ref.ay;
        this->by = ref.by;
        this->cy = ref.cy;
        return *this;
    }
    zfbool operator == (ZF_IN const ZFCurve &ref) const {
        return (this->p0x == ref.p0x
            && this->p0y == ref.p0y
            && this->p1x == ref.p1x
            && this->p1y == ref.p1y
            );
    }
    inline zfbool operator != (ZF_IN const ZFCurve &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief change control point */
    ZFCurve &controlPoint(
            ZF_IN zffloat p0x
            , ZF_IN zffloat p0y
            , ZF_IN zffloat p1x
            , ZF_IN zffloat p1y
            );

public:
    /**
     * @brief solve y with given t
     *
     * t must in range [0, 1]
     */
    zffloat y_by_t(ZF_IN zffloat t) const;

    /**
     * @brief solve y with given x
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat y_by_x(ZF_IN zffloat x) const;

    /**
     * @brief solve x with given t
     *
     * t must in range [0, 1]
     */
    zffloat x_by_t(ZF_IN zffloat t) const;

    /**
     * @brief solve x with given y
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat x_by_y(ZF_IN zffloat y) const;

    /**
     * @brief solve t with given x
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat t_by_x(ZF_IN zffloat x) const;

    /**
     * @brief solve t with given y
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat t_by_y(ZF_IN zffloat y) const;

public:
    zffloat p0x; /**< @brief control point */
    zffloat p0y; /**< @brief control point */
    zffloat p1x; /**< @brief control point */
    zffloat p1y; /**< @brief control point */
private:
    zffloat ax;
    zffloat bx;
    zffloat cx;

    zffloat ay;
    zffloat by;
    zffloat cy;
};

// ============================================================
/**
 * @brief linear curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveZero)
/**
 * @brief linear curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveLinear)
/**
 * @brief ease in curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveEaseIn)
/**
 * @brief ease out curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveEaseOut)
/**
 * @brief ease in out curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveEaseInOut)
/**
 * @brief bounce in curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveBounceIn)
/**
 * @brief bounce out curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveBounceOut)
/**
 * @brief bounce in out curve
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurveBounceInOut)

// ============================================================
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCurve
 *       p0x="0" // optional, 0 by default
 *       p0y="0" // optional, 0 by default
 *       p1x="1" // optional, 1 by default
 *       p1y="1" // optional, 1 by default
 *   />
 * @endcode
 *
 * string data:
 * @code
 *   (p0x, p0y, p1x, p1y)
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFCurve, ZFCurve)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCurve_p0x "p0x"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCurve_p0y "p0y"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCurve_p1x "p1x"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCurve_p1y "p1y"

// ============================================================
ZFOUTPUT_TYPE(ZFCurve, {output << ZFCurveToString(v);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCurve_h_

