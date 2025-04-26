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

// ============================================================
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

// ============================================================
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
 * @brief bounce in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceIn().y_by_x(time);
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
 * @brief bounce out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceOut().y_by_x(time);
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
 * @brief bounce in and out curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveBounceInOut : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveBounceInOut, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceInOut().y_by_x(time);
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
/**
 * @brief loop with bounce in curve, see #ZFCurve
 */
zfclass ZFLIB_ZFAlgorithm ZFCurveLoopBounceIn : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLoopBounceIn, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return ZFBezierBounceIn().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
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
        return ZFBezierBounceOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
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
        return ZFBezierBounceInOut().y_by_x(time < (zffloat)0.5f ? 2 * time : 2 * (1 - time));
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

