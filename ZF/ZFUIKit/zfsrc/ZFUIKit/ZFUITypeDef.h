/**
 * @file ZFUITypeDef.h
 * @brief necessary header file for all ZFUIKit member
 */

#ifndef _ZFI_ZFUITypeDef_h_
#define _ZFI_ZFUITypeDef_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// basic size
/**
 * @brief return a scaled int
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyScaleT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(zffloat const &, v)
        , ZFMP_IN(zffloat, scale)
        ) {
    ret = v * scale;
}
/**
 * @brief return a scaled int
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, zffloat, ZFUISizeApplyScale
        , ZFMP_IN(zffloat const &, v)
        , ZFMP_IN(zffloat, scale)
        ) {
    zffloat ret = 0;
    ZFUISizeApplyScaleT(ret, v, scale);
    return ret;
}
/**
 * @brief return a scaled int reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyScaleReverselyT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(zffloat const &, v)
        , ZFMP_IN(zffloat, scale)
        ) {
    ret = v / scale;
}
/**
 * @brief return a scaled int reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, zffloat, ZFUISizeApplyScaleReversely
        , ZFMP_IN(zffloat const &, v)
        , ZFMP_IN(zffloat, scale)
        ) {
    zffloat ret = 0;
    ZFUISizeApplyScaleReverselyT(ret, v, scale);
    return ret;
}

// ============================================================
// ZFUIPoint
/**
 * @brief 2D point
 */
zfclassPOD ZFLIB_ZFUIKit ZFUIPoint {
public:
    zffloat x;   /**< @brief x */
    zffloat y;   /**< @brief y */
};
ZFCORE_POD_DECLARE(ZFUIPoint)
ZFCORE_POD_COMPARER_DECLARE(ZFUIPoint)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIPoint
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPoint)
ZFTYPEID_REG(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPoint)
ZFOUTPUT_TYPE(ZFUIPoint, {ZFUIPointToStringT(s, v);})

/**
 * @brief (0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPointZero)

/**
 * @brief make a ZFUIPoint
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPointCreate
        , ZFMP_IN(zffloat const &, x)
        , ZFMP_IN(zffloat const &, y)
        ) {
    ZFUIPoint ret = {x, y};
    return ret;
}

/**
 * @brief return a scaled point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIPointApplyScaleT
        , ZFMP_OUT(ZFUIPoint &, ret)
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleT(ret.x, point.x, scale);
    ZFUISizeApplyScaleT(ret.y, point.y, scale);
}
/**
 * @brief return a scaled point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPointApplyScale
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIPoint ret = {ZFUISizeApplyScale(point.x, scale), ZFUISizeApplyScale(point.y, scale)};
    return ret;
}
/**
 * @brief return a scaled point reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIPointApplyScaleReverselyT
        , ZFMP_OUT(ZFUIPoint &, ret)
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleReverselyT(ret.x, point.x, scale);
    ZFUISizeApplyScaleReverselyT(ret.y, point.y, scale);
}
/**
 * @brief return a scaled point reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIPointApplyScaleReversely
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIPoint ret = {ZFUISizeApplyScaleReversely(point.x, scale), ZFUISizeApplyScaleReversely(point.y, scale)};
    return ret;
}

// ============================================================
// ZFUIMargin
/**
 * @brief margin
 */
zfclassPOD ZFLIB_ZFUIKit ZFUIMargin {
public:
    zffloat left;    /**< @brief left margin */
    zffloat top;     /**< @brief top margin */
    zffloat right;   /**< @brief right margin */
    zffloat bottom;  /**< @brief bottom margin */
};
ZFCORE_POD_DECLARE(ZFUIMargin)
ZFCORE_POD_COMPARER_DECLARE(ZFUIMargin)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIMargin
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMargin)
ZFTYPEID_REG(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMargin)
ZFOUTPUT_TYPE(ZFUIMargin, {ZFUIMarginToStringT(s, v);})

/**
 * @brief (0, 0, 0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginZero)

/**
 * @brief make a margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginCreate
        , ZFMP_IN(zffloat const &, left)
        , ZFMP_IN(zffloat const &, top)
        , ZFMP_IN(zffloat const &, right)
        , ZFMP_IN(zffloat const &, bottom)
        ) {
    ZFUIMargin ret = {left, top, right, bottom};
    return ret;
}
/**
 * @brief make a margin with all sides set to margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginCreate
        , ZFMP_IN(zffloat const &, margin)
        ) {
    ZFUIMargin ret = {margin, margin, margin, margin};
    return ret;
}
/**
 * @brief make a margin with left/right to x and top/bottom to y
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginCreate
        , ZFMP_IN(zffloat const &, x)
        , ZFMP_IN(zffloat const &, y)
        ) {
    ZFUIMargin ret = {x, y, x, y};
    return ret;
}

/**
 * @brief return a scaled margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIMarginApplyScaleT
        , ZFMP_OUT(ZFUIMargin &, ret)
        , ZFMP_IN(const ZFUIMargin &, margin)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleT(ret.left, margin.left, scale);
    ZFUISizeApplyScaleT(ret.top, margin.top, scale);
    ZFUISizeApplyScaleT(ret.right, margin.right, scale);
    ZFUISizeApplyScaleT(ret.bottom, margin.bottom, scale);
}
/**
 * @brief return a scaled margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginApplyScale
        , ZFMP_IN(const ZFUIMargin &, margin)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIMargin ret = {
        ZFUISizeApplyScale(margin.left, scale),
        ZFUISizeApplyScale(margin.top, scale),
        ZFUISizeApplyScale(margin.right, scale),
        ZFUISizeApplyScale(margin.bottom, scale)
    };
    return ret;
}
/**
 * @brief return a scaled margin reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIMarginApplyScaleReverselyT
        , ZFMP_OUT(ZFUIMargin &, ret)
        , ZFMP_IN(const ZFUIMargin &, margin)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleReverselyT(ret.left, margin.left, scale);
    ZFUISizeApplyScaleReverselyT(ret.top, margin.top, scale);
    ZFUISizeApplyScaleReverselyT(ret.right, margin.right, scale);
    ZFUISizeApplyScaleReverselyT(ret.bottom, margin.bottom, scale);
}
/**
 * @brief return a scaled margin reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginApplyScaleReversely
        , ZFMP_IN(const ZFUIMargin &, margin)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIMargin ret = {
        ZFUISizeApplyScaleReversely(margin.left, scale),
        ZFUISizeApplyScaleReversely(margin.top, scale),
        ZFUISizeApplyScaleReversely(margin.right, scale),
        ZFUISizeApplyScaleReversely(margin.bottom, scale)
    };
    return ret;
}
/**
 * @brief left + right margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIMarginGetWidth
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    return margin.left + margin.right;
}
/**
 * @brief top + bottom margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIMarginGetHeight
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    return margin.top + margin.bottom;
}
/**
 * @brief v0 + v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIMarginInc
        , ZFMP_IN_OUT(ZFUIMargin &, ret)
        , ZFMP_IN(const ZFUIMargin &, v0)
        , ZFMP_IN(const ZFUIMargin &, v1)
        ) {
    ret.left = v0.left + v1.left;
    ret.top = v0.top + v1.top;
    ret.right = v0.right + v1.right;
    ret.bottom = v0.bottom + v1.bottom;
}
/**
 * @brief v0 + v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginInc
        , ZFMP_IN(const ZFUIMargin &, v0)
        , ZFMP_IN(const ZFUIMargin &, v1)
        ) {
    ZFUIMargin ret = {
        v0.left + v1.left,
        v0.top + v1.top,
        v0.right + v1.right,
        v0.bottom + v1.bottom
    };
    return ret;
}
/**
 * @brief v0 + v1
 */
inline ZFUIMargin operator + (
        ZF_IN const ZFUIMargin &v0
        , ZF_IN const ZFUIMargin &v1
        ) {
    ZFUIMargin ret = {
        v0.left + v1.left,
        v0.top + v1.top,
        v0.right + v1.right,
        v0.bottom + v1.bottom
    };
    return ret;
}
/**
 * @brief v0 += v1
 */
inline ZFUIMargin &operator += (
        ZF_IN_OUT ZFUIMargin &v0
        , ZF_IN const ZFUIMargin &v1
        ) {
    v0.left += v1.left;
    v0.top += v1.top;
    v0.right += v1.right;
    v0.bottom += v1.bottom;
    return v0;
}
/**
 * @brief v0 - v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIMarginDec
        , ZFMP_IN_OUT(ZFUIMargin &, ret)
        , ZFMP_IN(const ZFUIMargin &, v0)
        , ZFMP_IN(const ZFUIMargin &, v1)
        ) {
    ret.left = v0.left - v1.left;
    ret.top = v0.top - v1.top;
    ret.right = v0.right - v1.right;
    ret.bottom = v0.bottom - v1.bottom;
}
/**
 * @brief v0 - v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIMargin, ZFUIMarginDec
        , ZFMP_IN(const ZFUIMargin &, v0)
        , ZFMP_IN(const ZFUIMargin &, v1)
        ) {
    ZFUIMargin ret = {
        v0.left - v1.left,
        v0.top - v1.top,
        v0.right - v1.right,
        v0.bottom - v1.bottom
    };
    return ret;
}
/**
 * @brief v0 - v1
 */
inline ZFUIMargin operator - (
        ZF_IN const ZFUIMargin &v0
        , ZF_IN const ZFUIMargin &v1
        ) {
    ZFUIMargin ret = {
        v0.left - v1.left,
        v0.top - v1.top,
        v0.right - v1.right,
        v0.bottom - v1.bottom
    };
    return ret;
}
/**
 * @brief v0 -= v1
 */
inline ZFUIMargin &operator -= (
        ZF_IN_OUT ZFUIMargin &v0
        , ZF_IN const ZFUIMargin &v1
        ) {
    v0.left -= v1.left;
    v0.top -= v1.top;
    v0.right -= v1.right;
    v0.bottom -= v1.bottom;
    return v0;
}

// ============================================================
// ZFUISize
/**
 * @brief 2D size
 */
zfclassPOD ZFLIB_ZFUIKit ZFUISize {
public:
    zffloat width;   /**< @brief width */
    zffloat height;  /**< @brief height */
};
ZFCORE_POD_DECLARE(ZFUISize)
ZFCORE_POD_COMPARER_DECLARE(ZFUISize)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISize
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUISize, ZFUISize)
ZFTYPEID_REG(ZFLIB_ZFUIKit, ZFUISize, ZFUISize)
ZFOUTPUT_TYPE(ZFUISize, {ZFUISizeToStringT(s, v);})

/**
 * @brief (0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeZero)
/**
 * @brief (-1, -1)
 *
 * since zero size is valid on some cases,
 * we supply -1 as special size to indicate an invalid size
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeInvalid)

/**
 * @brief make a ZFUISize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeCreate
        , ZFMP_IN(zffloat const &, width)
        , ZFMP_IN(zffloat const &, height)
        ) {
    ZFUISize ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeCreate
        , ZFMP_IN(zffloat const &, v)
        ) {
    ZFUISize ret = {v, v};
    return ret;
}

/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, void, ZFUISizeApplyRangeT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, orgSize)
        , ZFMP_IN(const ZFUISize &, minSize)
        , ZFMP_IN(const ZFUISize &, maxSize)
        ) {
    ret.width = zfmApplyRange(orgSize.width, minSize.width < 0 ? (zffloat)0 : minSize.width, maxSize.width < 0 ? orgSize.width : maxSize.width);
    ret.height = zfmApplyRange(orgSize.height, minSize.height < 0 ? (zffloat)0 : minSize.height, maxSize.height < 0 ? orgSize.height : maxSize.height);
}
/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyRange
        , ZFMP_IN(const ZFUISize &, orgSize)
        , ZFMP_IN(const ZFUISize &, minSize)
        , ZFMP_IN(const ZFUISize &, maxSize)
        ) {
    ZFUISize ret;
    ZFUISizeApplyRangeT(ret, orgSize, minSize, maxSize);
    return ret;
}
/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, void, ZFUISizeApplyRangeT
        , ZFMP_OUT(zffloat &, ret)
        , ZFMP_IN(const zffloat &, orgSize)
        , ZFMP_IN(const zffloat &, minSize)
        , ZFMP_IN(const zffloat &, maxSize)
        ) {
    ret = zfmApplyRange(orgSize, minSize < 0 ? (zffloat)0 : minSize, maxSize < 0 ? orgSize : maxSize);
}
/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, zffloat, ZFUISizeApplyRange
        , ZFMP_IN(const zffloat &, orgSize)
        , ZFMP_IN(const zffloat &, minSize)
        , ZFMP_IN(const zffloat &, maxSize)
        ) {
    zffloat ret = orgSize;
    ZFUISizeApplyRangeT(ret, orgSize, minSize, maxSize);
    return ret;
}
/**
 * @brief return a scaled size
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyScaleT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleT(ret.width, size.width, scale);
    ZFUISizeApplyScaleT(ret.height, size.height, scale);
}
/**
 * @brief return a scaled size
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyScale
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISize ret = {
        ZFUISizeApplyScale(size.width, scale),
        ZFUISizeApplyScale(size.height, scale)
    };
    return ret;
}
/**
 * @brief return a scaled size reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyScaleReverselyT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleReverselyT(ret.width, size.width, scale);
    ZFUISizeApplyScaleReverselyT(ret.height, size.height, scale);
}
/**
 * @brief return a scaled size reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyScaleReversely
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISize ret = {
        ZFUISizeApplyScaleReversely(size.width, scale),
        ZFUISizeApplyScaleReversely(size.height, scale)
    };
    return ret;
}

/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyAspectRatioT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUISize &, refSize)
        )
/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyAspectRatio
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUISize &, refSize)
        ) {
    ZFUISize ret = ZFUISizeZero();
    ZFUISizeApplyAspectRatioT(ret, size, refSize);
    return ret;
}

/**
 * @brief return a size excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyMarginT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ret.width = size.width - margin.left - margin.right;
    ret.height = size.height - margin.top - margin.bottom;
}
/**
 * @brief return a size excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyMargin
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUISize ret = {
        size.width - margin.left - margin.right,
        size.height - margin.top - margin.bottom
    };
    return ret;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUISizeApplyMarginReverselyT
        , ZFMP_OUT(ZFUISize &, ret)
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ret.width = size.width + margin.left + margin.right;
    ret.height = size.height + margin.top + margin.bottom;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUISize, ZFUISizeApplyMarginReversely
        , ZFMP_IN(const ZFUISize &, size)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUISize ret = {
        size.width + margin.left + margin.right,
        size.height + margin.top + margin.bottom
    };
    return ret;
}

// ============================================================
// ZFUIRect
/**
 * @brief 2D rectangle
 */
zfclassPOD ZFLIB_ZFUIKit ZFUIRect {
public:
    zffloat x; /**< @brief x */
    zffloat y; /**< @brief y */
    zffloat width; /**< @brief width */
    zffloat height; /**< @brief height */
};
ZFCORE_POD_DECLARE(ZFUIRect)
ZFCORE_POD_COMPARER_DECLARE(ZFUIRect)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIRect
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRect)
ZFTYPEID_REG(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRect)
ZFOUTPUT_TYPE(ZFUIRect, {ZFUIRectToStringT(s, v);})

/**
 * @brief (0, 0, 0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectZero)

/**
 * @brief make a ZFUIRect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectCreate
        , ZFMP_IN(zffloat const &, x)
        , ZFMP_IN(zffloat const &, y)
        , ZFMP_IN(zffloat const &, w)
        , ZFMP_IN(zffloat const &, h)
        ) {
    ZFUIRect ret = {x, y, w, h};
    return ret;
}
/**
 * @brief make a ZFUIRect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectCreate
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(const ZFUISize &, size)
        ) {
    ZFUIRect ret = {point.x, point.y, size.width, size.height};
    return ret;
}

/**
 * @brief is rect empty
 *
 * rect is empty if width or height <= 0
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsEmpty
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return (rect.width <= 0 || rect.height <= 0);
}
/**
 * @brief union two rect, return a rect that contains both rect1 and rect2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectUnion
        , ZFMP_IN(const ZFUIRect &, rect1)
        , ZFMP_IN(const ZFUIRect &, rect2)
        ) {
    ZFUIRect ret = {
        (rect1.x < rect2.x) ? rect1.x : rect2.x,
        (rect1.y < rect2.y) ? rect1.y : rect2.y,
        (rect1.x + rect1.width > rect2.x + rect2.width)
            ? (rect1.x + rect1.width)
            : (rect2.x + rect2.width),
        (rect1.y + rect1.height > rect2.y + rect2.height)
            ? (rect1.y + rect1.height)
            : (rect2.y + rect2.height)
    };
    if(ZFUIRectIsEmpty(ret)) {
        return ZFUIRectZero();
    }
    else {
        return ret;
    }
}
/**
 * @brief intersect two rect, return #ZFUIRectZero if not intersected
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectIntersection
        , ZFMP_IN(const ZFUIRect &, rect1)
        , ZFMP_IN(const ZFUIRect &, rect2)
        ) {
    ZFUIRect ret = {
        (rect1.x > rect2.x) ? rect1.x : rect2.x,
        (rect1.y > rect2.y) ? rect1.y : rect2.y,
        (rect1.x + rect1.width < rect2.x + rect2.width)
            ? (rect1.x + rect1.width)
            : (rect2.x + rect2.width),
        (rect1.y + rect1.height < rect2.y + rect2.height)
            ? (rect1.y + rect1.height)
            : (rect2.y + rect2.height)
    };
    if(ZFUIRectIsEmpty(ret)) {
        return ZFUIRectZero();
    }
    else {
        return ret;
    }
}
/**
 * @brief is two rect intersected,
 *   two tangented rect is not intersected
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsIntersectToRect
        , ZFMP_IN(const ZFUIRect &, rect1)
        , ZFMP_IN(const ZFUIRect &, rect2)
        ) {
    return !ZFUIRectIsEmpty(ZFUIRectIntersection(rect1, rect2));
}
/**
 * @brief is rect contains point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsContainPoint
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIPoint &, point)
        ) {
    return (point.x >= rect.x
        && point.x <= rect.x + rect.width
        && point.y >= rect.y
        && point.y <= rect.y + rect.height);
}
/**
 * @brief see #ZFUIRectIsContainPoint
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsContainPoint
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIPoint &, point)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    return (point.x >= rect.x + margin.left
        && point.x <= rect.x + rect.width - margin.right
        && point.y >= rect.y + margin.top
        && point.y <= rect.y + rect.height - margin.bottom);
}
/**
 * @brief return true if rect2 is completely contained by rect1,
 *   two equal rect is regarded as contain each other
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsContainRect
        , ZFMP_IN(const ZFUIRect &, rect1)
        , ZFMP_IN(const ZFUIRect &, rect2)
        ) {
    return (rect2.x >= rect1.x
        && rect2.x + rect2.width <= rect1.x + rect1.width
        && rect2.y >= rect1.y
        && rect2.y + rect2.height <= rect1.y + rect1.height);
}
/**
 * @brief see #ZFUIRectIsContainRect
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, zfbool, ZFUIRectIsContainRect
        , ZFMP_IN(const ZFUIRect &, rect1)
        , ZFMP_IN(const ZFUIRect &, rect2)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    return (rect2.x >= rect1.x + margin.left
        && rect2.x + rect2.width <= rect1.x + rect1.width - margin.right
        && rect2.y >= rect1.y + margin.top
        && rect2.y + rect2.height <= rect1.y + rect1.height - margin.bottom);
}
/**
 * @brief return a scaled rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIRectApplyScaleT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleT(ret.x, rect.x, scale);
    ZFUISizeApplyScaleT(ret.y, rect.y, scale);
    ZFUISizeApplyScaleT(ret.width, rect.width, scale);
    ZFUISizeApplyScaleT(ret.height, rect.height, scale);
}
/**
 * @brief return a scaled rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectApplyScale
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIRect ret = {
        ZFUISizeApplyScale(rect.x, scale),
        ZFUISizeApplyScale(rect.y, scale),
        ZFUISizeApplyScale(rect.width, scale),
        ZFUISizeApplyScale(rect.height, scale)
    };
    return ret;
}
/**
 * @brief return a scaled rect reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIRectApplyScaleReverselyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUISizeApplyScaleReverselyT(ret.x, rect.x, scale);
    ZFUISizeApplyScaleReverselyT(ret.y, rect.y, scale);
    ZFUISizeApplyScaleReverselyT(ret.width, rect.width, scale);
    ZFUISizeApplyScaleReverselyT(ret.height, rect.height, scale);
}
/**
 * @brief return a scaled rect reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectApplyScaleReversely
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(zffloat, scale)
        ) {
    ZFUIRect ret = {
        ZFUISizeApplyScaleReversely(rect.x, scale),
        ZFUISizeApplyScaleReversely(rect.y, scale),
        ZFUISizeApplyScaleReversely(rect.width, scale),
        ZFUISizeApplyScaleReversely(rect.height, scale)
    };
    return ret;
}
/**
 * @brief get bounds for rect, i.e. (0, 0, width, height)
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectGetBounds
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    ZFUIRect ret = {0, 0, rect.width, rect.height};
    return ret;
}
/**
 * @brief get size of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUISize, ZFUIRectGetSize
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return (ZFUISize const &)(rect.width);
}
/**
 * @brief get center point of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIPoint, ZFUIRectGetCenter
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    ZFUIPoint ret = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    return ret;
}
/**
 * @brief get center point of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetCenterX
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return rect.x + rect.width / 2;
}
/**
 * @brief get center point of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetCenterY
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return rect.y + rect.height / 2;
}
/**
 * @brief get left edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetLeft
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return rect.x;
}
/**
 * @brief get top edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetTop
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return rect.y;
}
/**
 * @brief get right edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetRight
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return (rect.x + rect.width);
}
/**
 * @brief get bottom edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIRectGetBottom
        , ZFMP_IN(const ZFUIRect &, rect)
        ) {
    return (rect.y + rect.height);
}

/**
 * @brief return a rect excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIRectApplyMarginT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ret.x = rect.x + margin.left;
    ret.y = rect.y + margin.top;
    ret.width = rect.width - margin.left - margin.right;
    ret.height = rect.height - margin.top - margin.bottom;
}
/**
 * @brief return a rect excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectApplyMargin
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUIRect ret = {
        rect.x + margin.left,
        rect.y + margin.top,
        rect.width - margin.left - margin.right,
        rect.height - margin.top - margin.bottom
    };
    return ret;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, void, ZFUIRectApplyMarginReverselyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ret.x = rect.x - margin.left;
    ret.y = rect.y - margin.top;
    ret.width = rect.width + margin.left + margin.right;
    ret.height = rect.height + margin.top + margin.bottom;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIRect, ZFUIRectApplyMarginReversely
        , ZFMP_IN(const ZFUIRect &, rect)
        , ZFMP_IN(const ZFUIMargin &, margin)
        ) {
    ZFUIRect ret = {
        rect.x - margin.left,
        rect.y - margin.top,
        rect.width + margin.left + margin.right,
        rect.height + margin.top + margin.bottom
    };
    return ret;
}

/**
 * @brief util method for impl to safe convert float rect to int values
 */
template<typename T_Size>
void ZFUIRectRound(
        ZF_OUT T_Size &x
        , ZF_OUT T_Size &y
        , ZF_OUT T_Size &w
        , ZF_OUT T_Size &h
        , ZF_IN const ZFUIRect &rect
        ) {
    x = (T_Size)zfmRound(rect.x);
    y = (T_Size)zfmRound(rect.y);
    w = (T_Size)zfmRound(rect.x + rect.width) - x;
    h = (T_Size)zfmRound(rect.y + rect.height) - y;
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundX(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.x);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundY(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.y);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundWidth(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.x + rect.width) - (zfint)zfmRound(rect.x);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundHeight(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.y + rect.height) - (zfint)zfmRound(rect.y);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundLeft(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.x);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundTop(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.y);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundRight(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.x + rect.width);
}
/** @brief see #ZFUIRectRound */
inline zfint ZFUIRectRoundBottom(ZF_IN const ZFUIRect &rect) {
    return (zfint)zfmRound(rect.y + rect.height);
}

// ============================================================
// ZFUIAlignFlags
/**
 * @brief align types for UI in ZFFramework
 * @see ZFUIAlignFlags
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIAlign)
    ZFENUM_VALUE_WITH_INIT(Center, 0) /**< @brief ensured 0 */
    ZFENUM_VALUE_WITH_INIT(Left, 1 << 0)
    ZFENUM_VALUE_WITH_INIT(Top, 1 << 1)
    ZFENUM_VALUE_WITH_INIT(Right, 1 << 2)
    ZFENUM_VALUE_WITH_INIT(Bottom, 1 << 3)
    ZFENUM_VALUE_WITH_INIT(LeftEdge, 1 << 4)
    ZFENUM_VALUE_WITH_INIT(TopEdge, 1 << 5)
    ZFENUM_VALUE_WITH_INIT(RightEdge, 1 << 6)
    ZFENUM_VALUE_WITH_INIT(BottomEdge, 1 << 7)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
    ZFENUM_VALUE_REGISTER(LeftEdge)
    ZFENUM_VALUE_REGISTER(TopEdge)
    ZFENUM_VALUE_REGISTER(RightEdge)
    ZFENUM_VALUE_REGISTER(BottomEdge)
ZFENUM_END_FLAGS(ZFLIB_ZFUIKit, ZFUIAlign, ZFUIAlignFlags)
ZFENUM_REG_FLAGS(ZFLIB_ZFUIKit, ZFUIAlign, ZFUIAlignFlags)

/**
 * @brief true if align is valid
 *
 * a align is valid only if:
 * -  only bits in #ZFUIAlign are set
 * -  x or y align type must not conflict, i.e.
 *   "Left, LeftEdge, Right, RightEdge" could only exist at most one,
 *   "Top, TopEdge, Bottom, BottomEdge" could only exist at most one
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, zfbool, ZFUIAlignValid
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        )
/**
 * @brief apply align and calculate result, or #ZFUIRectZero if error
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIKit, void, ZFUIAlignApplyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        , ZFMP_IN(const ZFUIRect &, refRect)
        , ZFMP_IN(const ZFUISize &, itemSize)
        )
/**
 * @brief apply align and calculate result, or #ZFUIRectZero if error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_ZFUIKit, ZFUIRect, ZFUIAlignApply
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        , ZFMP_IN(const ZFUIRect &, refRect)
        , ZFMP_IN(const ZFUISize &, itemSize)
        ) {
    ZFUIRect ret = ZFUIRectZero();
    ZFUIAlignApplyT(ret, align, refRect, itemSize);
    return ret;
}
/**
 * @brief get horizontal align or #v_ZFUIAlign::e_Center if invalid
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, ZFUIAlign, ZFUIAlignGetX
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        )
/**
 * @brief get vertical align or #v_ZFUIAlign::e_Center if invalid
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, ZFUIAlign, ZFUIAlignGetY
        , ZFMP_IN(const ZFUIAlignFlags &, align)
        )

// ============================================================
// ZFUIOrientation
/**
 * @brief orientation types for UI in ZFFramework
 *
 * for example, for a mobile phone,
 * Top means user hold the phone normally,
 * Left means user rotate the phone 90 degrees counter clockwise,
 * Right means user rotate the phone 90 degrees clockwise,
 * and Bottom means user rotate the phone 180 degrees
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIOrientation)
    ZFENUM_VALUE_WITH_INIT(Left, 1 << 0)
    ZFENUM_VALUE_WITH_INIT(Top, 1 << 1)
    ZFENUM_VALUE_WITH_INIT(Right, 1 << 2)
    ZFENUM_VALUE_WITH_INIT(Bottom, 1 << 3)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
ZFENUM_END_FLAGS(ZFLIB_ZFUIKit, ZFUIOrientation, ZFUIOrientationFlags)
ZFENUM_REG_FLAGS(ZFLIB_ZFUIKit, ZFUIOrientation, ZFUIOrientationFlags)

/** @brief (v_ZFUIOrientation::e_Left | v_ZFUIOrientation::e_Top | v_ZFUIOrientation::e_Right | v_ZFUIOrientation::e_Bottom) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIOrientationFlags, ZFUIOrientationFlagsAll)
/** @brief (v_ZFUIOrientation::e_Left | v_ZFUIOrientation::e_Right) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIOrientationFlags, ZFUIOrientationFlagsHorizontal)
/** @brief (v_ZFUIOrientation::e_Top | v_ZFUIOrientation::e_Bottom) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIOrientationFlags, ZFUIOrientationFlagsVertical)

/**
 * @brief true if vertical orientation
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zfbool, ZFUIOrientationIsVertical
        , ZFMP_IN(ZFUIOrientation, orientation)
        ) {
    return (orientation == v_ZFUIOrientation::e_Top || orientation == v_ZFUIOrientation::e_Bottom);
}
/**
 * @brief true if vertical orientation
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zfbool, ZFUIOrientationIsHorizontal
        , ZFMP_IN(ZFUIOrientation, orientation)
        ) {
    return (orientation == v_ZFUIOrientation::e_Left || orientation == v_ZFUIOrientation::e_Right);
}

// ============================================================
// ZFUIColor
/**
 * @brief color with `AARRGGBB` format
 *
 * note, color is ensured as POD type,
 * but the actual color type is not ensured,
 * use float to make and store a/r/g/b element,
 * and use string converter (#ZFUIColorToString) if necessary\n
 * for now, the color is 32bit integer with `AARRGGBB` format,
 * but may extend to 64bit for future update
 */
ZFT_INT_STRONG(zft_zfuint32, ZFUIColor)
ZFCORE_POD_DECLARE(ZFUIColor)

/** @brief zero color */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorZero)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIColor
 *       value="value" // optional, 0 by default
 *   />
 * @endcode
 *
 * valid value format:
 * -  "AARRGGBB"
 * -  "RRGGBB"
 * -  "ARGB"
 * -  "RGB"
 *
 * valid prefix:
 * -  empty ("AARRGGBB")
 * -  "#" ("#AARRGGBB")
 * -  "0x" ("0xAARRGGBB")
 *
 * when serialize back to string,
 * it's ensured to use "#" as prefix,
 * and will try to use the shortest form if available
 * (e.g. "RGB" instead of "AARRGGBB")
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColor)
ZFTYPEID_REG(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColor)
ZFOUTPUT_TYPE(ZFUIColor, {ZFUIColorToStringT(s, v);})

/**
 * @brief make a color
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorCreate
        , ZFMP_IN(zffloat, r)
        , ZFMP_IN(zffloat, g)
        , ZFMP_IN(zffloat, b)
        , ZFMP_IN_OPT(zffloat, a, 1.0f)
        ) {
    return ZFUIColor(
          (((zfuint)((a) * 0xFF) & 0xFF) << 24)
        | (((zfuint)((r) * 0xFF) & 0xFF) << 16)
        | (((zfuint)((g) * 0xFF) & 0xFF) << 8)
        | ((zfuint)((b) * 0xFF) & 0xFF));
}
/**
 * @brief make a color for hex int
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorMakeARGB
        , ZFMP_IN(zfuint, argb)
        ) {
    return ZFUIColor((zft_ZFUIColor)(argb));
}
/**
 * @brief make a color for hex int
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorMakeRGB
        , ZFMP_IN(zfuint, rgb)
        ) {
    return ZFUIColor((zft_ZFUIColor)(0xFF000000 | (rgb)));
}

/**
 * @brief return a copy of color with changed alpha
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorWithA
        , ZFMP_IN(ZFUIColor const &, c)
        , ZFMP_IN(zffloat, a)
        ) {
    return ((zft_ZFUIColor)(((c) & 0x00FFFFFF) | (((zfuint)((a) * 0xFF) & 0xFF) << 24)));
}
/**
 * @brief return a copy of color with changed red
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorWithR
        , ZFMP_IN(ZFUIColor const &, c)
        , ZFMP_IN(zffloat, r)
        ) {
    return ((zft_ZFUIColor)(((c) & 0xFF00FFFF) | (((zfuint)((r) * 0xFF) & 0xFF) << 16)));
}
/**
 * @brief return a copy of color with changed green
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorWithG
        , ZFMP_IN(ZFUIColor const &, c)
        , ZFMP_IN(zffloat, g)
        ) {
    return ((zft_ZFUIColor)(((c) & 0xFFFF00FF) | (((zfuint)((g) * 0xFF) & 0xFF) <<  8)));
}
/**
 * @brief return a copy of color with changed blue
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorWithB
        , ZFMP_IN(ZFUIColor const &, c)
        , ZFMP_IN(zffloat, b)
        ) {
    return ((zft_ZFUIColor)(((c) & 0xFFFFFF00) | (((zfuint)((b) * 0xFF) & 0xFF) <<  0)));
}

/**
 * @brief get alpha from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIColorGetA
        , ZFMP_IN(ZFUIColor const &, c)
        ) {
    return ((zffloat)((((c) >> 24) & 0xFF) / (zft_zffloat)0xFF));
}
/**
 * @brief get red from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIColorGetR
        , ZFMP_IN(ZFUIColor const &, c)
        ) {
    return ((zffloat)((((c) >> 16) & 0xFF) / (zft_zffloat)0xFF));
}
/**
 * @brief get green from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIColorGetG
        , ZFMP_IN(ZFUIColor const &, c)
        ) {
    return ((zffloat)((((c) >>  8) & 0xFF) / (zft_zffloat)0xFF));
}
/**
 * @brief get blue from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, zffloat, ZFUIColorGetB
        , ZFMP_IN(ZFUIColor const &, c)
        ) {
    return ((zffloat)((((c) >>  0) & 0xFF) / (zft_zffloat)0xFF));
}

/**
 * @brief set alpha to a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor &, ZFUIColorSetA
        , ZFMP_IN_OUT(ZFUIColor &, c)
        , ZFMP_IN(zffloat, a)
        ) {
    return ((c) = ZFUIColorWithA(c, a));
}
/**
 * @brief set red from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor &, ZFUIColorSetR
        , ZFMP_IN_OUT(ZFUIColor &, c)
        , ZFMP_IN(zffloat, r)
        ) {
    return ((c) = ZFUIColorWithR(c, r));
}
/**
 * @brief set green from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor &, ZFUIColorSetG
        , ZFMP_IN_OUT(ZFUIColor &, c)
        , ZFMP_IN(zffloat, g)
        ) {
    return ((c) = ZFUIColorWithG(c, g));
}
/**
 * @brief set blue from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_ZFUIKit, ZFUIColor &, ZFUIColorSetB
        , ZFMP_IN_OUT(ZFUIColor &, c)
        , ZFMP_IN(zffloat, b)
        ) {
    return ((c) = ZFUIColorWithB(c, b));
}

/**
 * @brief create a random color with specified alpha
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_ZFUIKit, ZFUIColor, ZFUIColorRandom
        , ZFMP_IN_OPT(zffloat, alpha, 1.0f)
        ) {
    return ZFUIColorCreate(zfmRand(0xFF) / (zft_zffloat)0xFF, zfmRand(0xFF) / (zft_zffloat)0xFF, zfmRand(0xFF) / (zft_zffloat)0xFF, alpha);
}

// ============================================================
// common colors
/** @brief 0x000000 */
#define ZFUIColorBlack() ZFUIColorMakeRGB(0x000000)
/** @brief 0x555555 */
#define ZFUIColorGrayDark() ZFUIColorMakeRGB(0x555555)
/** @brief 0x808080 */
#define ZFUIColorGray() ZFUIColorMakeRGB(0x808080)
/** @brief 0xAAAAAA */
#define ZFUIColorGrayLight() ZFUIColorMakeRGB(0xAAAAAA)
/** @brief 0xFFFFFF */
#define ZFUIColorWhite() ZFUIColorMakeRGB(0xFFFFFF)

/** @brief 0xFF0000 */
#define ZFUIColorRed() ZFUIColorMakeRGB(0xFF0000)
/** @brief 0x00FF00 */
#define ZFUIColorGreen() ZFUIColorMakeRGB(0x00FF00)
/** @brief 0x0000FF */
#define ZFUIColorBlue() ZFUIColorMakeRGB(0x0000FF)
/** @brief 0xFFFF00 */
#define ZFUIColorYellow() ZFUIColorMakeRGB(0xFFFF00)
/** @brief 0xFF00FF */
#define ZFUIColorMagenta() ZFUIColorMakeRGB(0xFF00FF)
/** @brief 0x00FFFF */
#define ZFUIColorCyan() ZFUIColorMakeRGB(0x00FFFF)

// ============================================================
// ZFUITextAppearance
/**
 * @brief text appearance type, normal, bold or italic
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITextAppearance)
    ZFENUM_VALUE(Normal) /**< @brief normal */
    ZFENUM_VALUE(Bold) /**< @brief bold */
    ZFENUM_VALUE(Italic) /**< @brief italic */
    ZFENUM_VALUE(BoldItalic) /**< @brief bold and italic */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Bold)
    ZFENUM_VALUE_REGISTER(Italic)
    ZFENUM_VALUE_REGISTER(BoldItalic)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUITextAppearance)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUITextAppearance)

// ============================================================
// ZFUITextTruncateMode
/**
 * @brief text truncate mode
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUITextTruncateMode)
    ZFENUM_VALUE(Disable) /**< @brief never truncate */
    ZFENUM_VALUE(Head) /**< @brief truncate at head */
    ZFENUM_VALUE(Middle) /**< @brief truncate at middle */
    ZFENUM_VALUE(Tail) /**< @brief truncate at end */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Disable)
    ZFENUM_VALUE_REGISTER(Head)
    ZFENUM_VALUE_REGISTER(Middle)
    ZFENUM_VALUE_REGISTER(Tail)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUITextTruncateMode)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUITextTruncateMode)

// ============================================================
/**
 * @brief scale type when stretch items
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIContentScaleType)
    /**
     * @brief simply fill to parent
     */
    ZFENUM_VALUE(Fill)
    /**
     * @brief simply center the content without scale,
     *   may clip if content too large
     */
    ZFENUM_VALUE(Center)
    /**
     * @brief always keep aspect ratio and fill to min bounds,
     *   may leave blank area if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenter)
    /**
     * @brief always keep aspect ratio and fill to max bounds,
     *   may clip if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenterClipped)
    /**
     * @brief same as #e_Center if content not exceeds parent,
     *   same as #e_FillCenter if content exceeds parent
     */
    ZFENUM_VALUE(FitCenter)
    /**
     * @brief always fill x, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillX)
    /**
     * @brief same as #e_Center if x not exceeds parent,
     *   same as #e_FillX if x exceeds parent
     */
    ZFENUM_VALUE(FitX)
    /**
     * @brief always fill y, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillY)
    /**
     * @brief same as #e_Center if y not exceeds parent,
     *   same as #e_FillY if y exceeds parent
     */
    ZFENUM_VALUE(FitY)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Fill)
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(FillCenter)
    ZFENUM_VALUE_REGISTER(FillCenterClipped)
    ZFENUM_VALUE_REGISTER(FitCenter)
    ZFENUM_VALUE_REGISTER(FillX)
    ZFENUM_VALUE_REGISTER(FitX)
    ZFENUM_VALUE_REGISTER(FillY)
    ZFENUM_VALUE_REGISTER(FitY)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIContentScaleType)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUIContentScaleType)

/**
 * @brief apply scale for #ZFUIContentScaleType
 */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFUIKit, void, ZFUIContentScaleTypeApplyT
        , ZFMP_OUT(ZFUIRect &, ret)
        , ZFMP_IN(ZFUIContentScaleType, scaleType)
        , ZFMP_IN(const ZFUIRect &, bounds)
        , ZFMP_IN(const ZFUISize &, contentSize)
        , ZFMP_IN_OPT(const ZFUIAlignFlags &, alignFlags, v_ZFUIAlign::e_Center)
        )
/**
 * @brief apply scale for #ZFUIContentScaleType
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, ZFUIRect, ZFUIContentScaleTypeApply
        , ZFMP_IN(ZFUIContentScaleType, scaleType)
        , ZFMP_IN(const ZFUIRect &, bounds)
        , ZFMP_IN(const ZFUISize &, contentSize)
        , ZFMP_IN_OPT(const ZFUIAlignFlags &, alignFlags, v_ZFUIAlign::e_Center)
        ) {
    ZFUIRect ret = ZFUIRectZero();
    ZFUIContentScaleTypeApplyT(ret, scaleType, bounds, contentSize, alignFlags);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITypeDef_h_

