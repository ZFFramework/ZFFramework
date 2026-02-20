#include "ZFBezier.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFBezier &ZFBezier::controlPoint(
        ZF_IN zffloat p0x
        , ZF_IN zffloat p0y
        , ZF_IN zffloat p1x
        , ZF_IN zffloat p1y
        ) {
    this->p0x = p0x;
    this->p0y = p0y;
    this->p1x = p1x;
    this->p1y = p1y;

    cx = 3.0f * p0x;
    bx = 3.0f * (p1x - p0x) - cx;
    ax = 1.0f - cx -bx;

    cy = 3.0f * p0y;
    by = 3.0f * (p1y - p0y) - cy;
    ay = 1.0f - cy - by;
    return *this;
}

zffloat ZFBezier::y_by_t(ZF_IN zffloat t) const {
    return ((ay * t + by) * t + cy) * t;
}

zffloat ZFBezier::y_by_x(ZF_IN zffloat x) const {
    return this->y_by_t(this->t_by_x(x));
}

zffloat ZFBezier::x_by_t(ZF_IN zffloat t) const {
    return ((ax * t + bx) * t + cx) * t;
}

zffloat ZFBezier::x_by_y(ZF_IN zffloat y) const {
    return this->x_by_t(this->t_by_y(y));
}

zffloat ZFBezier::t_by_x(ZF_IN zffloat x) const {
    zffloat t0;
    zffloat t1;
    zffloat t2 = x;
    zffloat x2;
    zffloat d2;

    for(zfindex i = 0; i < 8; ++i) {
        x2 = this->x_by_t(t2) - x;
        if(x2 == 0) {
            return t2;
        }
        d2 = ((3.0f * ax * t2 + 2.0f * bx) * t2 + cx);
        if(d2 == 0) {
            break;
        }
        t2 = t2 - x2 / d2;
    }

    t0 = 0.0f;
    t1 = 1.0f;
    t2 = x;

    if(t2 < t0) {
        return t0;
    }
    if(t2 > t1) {
        return t1;
    }

    while(t0 < t1) {
        x2 = this->x_by_t(t2);
        if(x2 == x) {
            return t2;
        }
        if(x > x2) {
            t0 = t2;
        }
        else {
            t1 = t2;
        }
        t2 = (t1 - t0) * 0.5f + t0;
    }

    return t2;
}

zffloat ZFBezier::t_by_y(ZF_IN zffloat y) const {
    zffloat t0;
    zffloat t1;
    zffloat t2 = y;
    zffloat y2;
    zffloat d2;

    for(zfindex i = 0; i < 8; ++i) {
        y2 = this->y_by_t(t2) - y;
        if(y2 == 0) {
            return t2;
        }
        d2 = ((3.0f * ay * t2 + 2.0f * by) * t2 + cy);
        if(d2 == 0) {
            break;
        }
        t2 = t2 - y2 / d2;
    }

    t0 = 0.0f;
    t1 = 1.0f;
    t2 = y;

    if(t2 < t0) {
        return t0;
    }
    if(t2 > t1) {
        return t1;
    }

    while(t0 < t1) {
        y2 = this->y_by_t(t2);
        if(y2 == y) {
            return t2;
        }
        if(y > y2) {
            t0 = t2;
        }
        else {
            t1 = t2;
        }
        t2 = (t1 - t0) * 0.5f + t0;
    }

    return t2;
}

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierZero, ZFBezier(0, 0, 1, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierLinear, ZFBezier(0, 0, 1, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierEaseIn, ZFBezier(0.8f, 0, 1, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierEaseOut, ZFBezier(0, 0, 0.2f, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierEaseInOut, ZFBezier(0.8f, 0, 0.2f, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierBackIn, ZFBezier(0.2f, -0.5f, 1, 1))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierBackOut, ZFBezier(0, 0, 0.8f, 1.5f))
ZFEXPORT_VAR_READONLY_DEFINE(ZFBezier, ZFBezierBackInOut, ZFBezier(0.2f, -0.5f, 0.8f, 1.5f))

// ============================================================
ZFTYPEID_DEFINE(ZFBezier, ZFBezier, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFBezier(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        zffloat p0x = 0;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFBezier_p0x, zffloat, p0x, {
                    return zffalse;
                });

        zffloat p0y = 0;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFBezier_p0y, zffloat, p0y, {
                    return zffalse;
                });

        zffloat p1x = 1;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFBezier_p1x, zffloat, p1x, {
                    return zffalse;
                });

        zffloat p1y = 1;
        ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFBezier_p1y, zffloat, p1y, {
                    return zffalse;
                });

        v.controlPoint(p0x, p0y, p1x, p1y);

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFBezier());

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFBezier_p0x, zffloat, v.p0x, 0, {
                    return zffalse;
                });
        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFBezier_p0y, zffloat, v.p0y, 0, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFBezier_p1x, zffloat, v.p1x, 1, {
                    return zffalse;
                });
        ZFSerializableUtilSerializeAttrToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFBezier_p1y, zffloat, v.p1y, 1, {
                    return zffalse;
                });

        return zftrue;
    }, {
        ZFCoreArray<zffloat> tmp;
        if(!ZFCoreDataPairSplitFloat(tmp, 4, src, srcLen)) {
            return zffalse;
        }
        v.controlPoint(tmp[0], tmp[1], tmp[2], tmp[3]);
        return zftrue;
    }, {
        s += "(";
        zfsFromFloatT(s, v.p0x);
        s += ", ";
        zfsFromFloatT(s, v.p0y);
        s += ", ";
        zfsFromFloatT(s, v.p1x);
        s += ", ";
        zfsFromFloatT(s, v.p1y);
        s += ")";
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFBezier, void, controlPoint
        , ZFMP_IN(zffloat, p0x)
        , ZFMP_IN(zffloat, p0y)
        , ZFMP_IN(zffloat, p1x)
        , ZFMP_IN(zffloat, p1y)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, y_by_t
        , ZFMP_IN(zffloat, t)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, y_by_x
        , ZFMP_IN(zffloat, x)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, x_by_t
        , ZFMP_IN(zffloat, t)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, x_by_y
        , ZFMP_IN(zffloat, y)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, t_by_x
        , ZFMP_IN(zffloat, x)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBezier, zffloat, t_by_y
        , ZFMP_IN(zffloat, y)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFBezier, zffloat, p0x)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFBezier, zffloat, p0y)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFBezier, zffloat, p1x)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFBezier, zffloat, p1y)

ZF_NAMESPACE_GLOBAL_END

