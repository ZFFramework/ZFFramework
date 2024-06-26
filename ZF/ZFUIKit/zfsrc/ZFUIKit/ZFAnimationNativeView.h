/**
 * @file ZFAnimationNativeView.h
 * @brief native animation
 */

#ifndef _ZFI_ZFAnimationNativeView_h_
#define _ZFI_ZFAnimationNativeView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief native animation's curve
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFAnimationNativeViewCurve)
    ZFENUM_VALUE(Linear) /**< @brief linear */
    ZFENUM_VALUE(EaseIn) /**< @brief slow to fast */
    ZFENUM_VALUE(EaseOut) /**< @brief fast to slow */
    ZFENUM_VALUE(EaseInOut) /**< @brief slow to fast to slow */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Linear)
    ZFENUM_VALUE_REGISTER(EaseIn)
    ZFENUM_VALUE_REGISTER(EaseOut)
    ZFENUM_VALUE_REGISTER(EaseInOut)
ZFENUM_END(ZFLIB_ZFUIKit, ZFAnimationNativeViewCurve)

// ============================================================
zfclassFwd _ZFP_ZFAnimationNativeViewPrivate;
/**
 * @brief animation using native implementation
 *
 * native animation usually has better performance,
 * but may work incorrectly with local transform
 * (such as #ZFUIView::viewRotateZ)
 */
zfclass ZFLIB_ZFUIKit ZFAnimationNativeView : zfextend ZFAnimation {
    ZFOBJECT_DECLARE(ZFAnimationNativeView, ZFAnimation)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    // ============================================================
public:
    /**
     * @brief native animation
     */
    ZFMETHOD_DECLARE_0(void *, nativeAnimation)

    // ============================================================
    // property
public:
    /**
     * @brief whether automatically disable #ZFAnimation::aniTarget while animating, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, aniTargetAutoDisable, zftrue)

    /**
     * @brief animation's curve, #ZFAnimationNativeViewCurve::e_Linear by default
     */
    ZFPROPERTY_ASSIGN(ZFAnimationNativeViewCurveEnum, aniCurve, ZFAnimationNativeViewCurve::e_Linear)

    /**
     * @brief alpha from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniAlphaFrom, 1.0f)
    /**
     * @brief alpha to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniAlphaTo, 1.0f)

    /**
     * @brief scale x from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleXFrom, 1.0f)
    /**
     * @brief scale x to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleXTo, 1.0f)
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleYFrom, 1.0f)
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleYTo, 1.0f)
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleZFrom, 1.0f)
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniScaleZTo, 1.0f)

    /**
     * @brief translate x from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateXFrom, 0)
    /**
     * @brief translate x to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateXTo, 0)
    /**
     * @brief translate y from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateYFrom, 0)
    /**
     * @brief translate y to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateYTo, 0)
    /**
     * @brief translate y from, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateZFrom, 0)
    /**
     * @brief translate y to, 0 by default
     *
     * this value is relative to target view's size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslateZTo, 0)

    /**
     * @brief translate x from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelXFrom, 0)
    /**
     * @brief translate x to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelXTo, 0)
    /**
     * @brief translate y from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelYFrom, 0)
    /**
     * @brief translate y to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelYTo, 0)
    /**
     * @brief translate y from, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelZFrom, 0)
    /**
     * @brief translate y to, 0 by default
     *
     * this value is absolute pixel size
     */
    ZFPROPERTY_ASSIGN(zffloat, aniTranslatePixelZTo, 0)

    /**
     * @brief rotate x from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateXFrom, 0)
    /**
     * @brief rotate x to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateXTo, 0)
    /**
     * @brief rotate y from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateYFrom, 0)
    /**
     * @brief rotate y to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateYTo, 0)
    /**
     * @brief rotate z from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateZFrom, 0)
    /**
     * @brief rotate z to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, aniRotateZTo, 0)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniOnStart(void);
    zfoverride
    virtual void aniOnStop(ZF_IN ZFResultTypeEnum resultType);

protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);
public:
    zffinal void _ZFP_ZFAnimationNativeView_notifyStop(void) {
        this->aniImplNotifyStop();
    }

private:
    _ZFP_ZFAnimationNativeViewPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationNativeView_h_

