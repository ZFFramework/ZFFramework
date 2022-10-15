/**
 * @file ZFTimeLineCurve_UI.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_UI_h_
#define _ZFI_ZFTimeLineCurve_UI_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveEaseIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveEaseIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveEaseOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveEaseOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveEaseInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveEaseInOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveBounceIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveBounceIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveBounceOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveBounceOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZFLIB_ZFUIKit ZFTimeLineCurveBounceInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFCurveBounceInOut().y_by_x(time);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_UI_h_

