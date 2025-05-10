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
 * @brief ease in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseIn().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief ease out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseOut().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief ease in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveEaseInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveEaseInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseInOut().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
/**
 * @brief back in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBackIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBackIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackIn().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief back out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBackOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBackOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackOut().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief back in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBackInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBackInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackInOut().y_by_x(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
static zffloat _ZFP_ZFCurveBounceOut(ZF_IN zffloat time) {
    if(time < 1 / 2.75) {
        return (zffloat)(7.5625 * time * time);
    }
    else if(time < 2 / 2.75) {
        time -= 1.5 / 2.75;
        return (zffloat)(7.5625 * time * time + 0.75);
    }
    else if(time < 2.5 / 2.75) {
        time -= 2.25 / 2.75;
        return (zffloat)(7.5625 * time * time + 0.9375);
    }
    else {
        time -= 2.625 / 2.75;
        return (zffloat)(7.5625 * time * time + 0.984375);
    }
}
static zffloat _ZFP_ZFCurveBounceIn(ZF_IN zffloat time) {
    return 1 - _ZFP_ZFCurveBounceOut(1 - time);
}
/**
 * @brief bounce in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return _ZFP_ZFCurveBounceIn(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief bounce out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return _ZFP_ZFCurveBounceOut(time);
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief bounce in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        if(time < 0.5f) {
            return (zffloat)(_ZFP_ZFCurveBounceIn(time * 2) / 2);
        }
        else {
            return (zffloat)(_ZFP_ZFCurveBounceOut(time * 2 - 1) / 2 + 0.5f);
        }
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
/**
 * @brief loop curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoop : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoop, ZFCurve)

    /** @brief the curve to loop */
    ZFPROPERTY_RETAIN(zfanyT<ZFCurve>, curve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        time = (time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
        if(this->curve()) {
            return this->curve()->progressUpdate(time);
        }
        else {
            return time;
        }
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        if(ZFObjectCompareValue(this->curve(), another->curve()) != ZFCompareEqual) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with linear curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopLinear : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopLinear, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return (time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
/**
 * @brief loop with ease in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseIn().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with ease out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with ease in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopEaseInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopEaseInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierEaseInOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
/**
 * @brief loop with back in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBackIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBackIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackIn().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with back out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBackOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBackOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with back in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBackInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBackInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBackInOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

// ============================================================
/**
 * @brief loop with bounce in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return _ZFP_ZFCurveBounceIn(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with bounce out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return _ZFP_ZFCurveBounceOut(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};
/**
 * @brief loop with bounce in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        if(time < 0.5f) {
            time = 2 * time;
        }
        else {
            time = 2 * (1 - time);
        }
        if(time < 0.5f) {
            return (zffloat)(_ZFP_ZFCurveBounceIn(time * 2) / 2);
        }
        else {
            return (zffloat)(_ZFP_ZFCurveBounceOut(time * 2 - 1) / 2 + 0.5f);
        }
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret = zfself::ClassData()->className();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCurve_common_h_

