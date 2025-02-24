/**
 * @file ZFCurve.h
 * @brief time based event driver
 */

#ifndef _ZFI_ZFCurve_h_
#define _ZFI_ZFCurve_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief time based event driver
 *
 * you may supply a subclass to achive custom curve,
 * but must make sure it is serializable and copyable
 */
zfabstract ZFLIB_ZFCore ZFCurve : zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable {
    ZFOBJECT_DECLARE_ABSTRACT(ZFCurve, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable)

public:
    /**
     * @brief calculate progress at time
     *
     * time ensured increasing from 0.0f to 1.0f (with some proper increasing step value),
     * while result have no limits\n
     * e.g. a linear curve would have same result as the time,
     * while a bounce curve may exceeds range [0, 1] as result
     */
    ZFMETHOD_DECLARE_1(zffloat, progressUpdate
            , ZFMP_IN(zffloat, time)
            )

protected:
    /**
     * @brief called by #progressUpdate to update progress
     */
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) zfpurevirtual;
};

// ============================================================
/**
 * @brief linear curve, see #ZFCurve
 */
zfclass ZFLIB_ZFCore ZFCurveLinear : zfextend ZFCurve {
    ZFOBJECT_DECLARE(ZFCurveLinear, ZFCurve)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time) {
        return time;
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj) {return ZFCompareEqual;}
        zfself *another = zfcast(zfself *, anotherObj);
        if(another == zfnull) {return ZFCompareUncomparable;}
        return ZFCompareEqual;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCurve_h_

