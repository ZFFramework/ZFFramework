/**
 * @file ZFAniForNative.h
 * @brief native animation
 */

#ifndef _ZFI_ZFAniForNative_h_
#define _ZFI_ZFAniForNative_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief native animation's curve
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFAniForNativeCurve)
    ZFENUM_VALUE(Linear) /**< @brief linear */
    ZFENUM_VALUE(EaseIn) /**< @brief slow to fast */
    ZFENUM_VALUE(EaseOut) /**< @brief fast to slow */
    ZFENUM_VALUE(EaseInOut) /**< @brief slow to fast to slow */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Linear)
    ZFENUM_VALUE_REGISTER(EaseIn)
    ZFENUM_VALUE_REGISTER(EaseOut)
    ZFENUM_VALUE_REGISTER(EaseInOut)
ZFENUM_END(ZFLIB_ZFUIKit, ZFAniForNativeCurve)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFAniForNativeCurve)

// ============================================================
zfclassFwd _ZFP_ZFAniForNativePrivate;
/**
 * @brief animation using native implementation
 *
 * native animation usually has better performance,
 * but may work incorrectly with local transform
 * (such as #ZFUIView::rotateZ)
 */
zfclass ZFLIB_ZFUIKit ZFAniForNative : zfextend ZFAnimation {
    ZFOBJECT_DECLARE(ZFAniForNative, ZFAnimation)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);

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
     * @brief whether automatically disable #ZFAnimation::target while animating, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, targetAutoDisable, zftrue)

    /**
     * @brief animation's curve, #v_ZFAniForNativeCurve::e_Linear by default
     */
    ZFPROPERTY_ASSIGN(ZFAniForNativeCurve, curve, ZFAniForNativeCurve::e_Linear)

    /**
     * @brief alpha from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, alphaFrom, 1.0f)
    /**
     * @brief alpha to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, alphaTo, 1.0f)

    /**
     * @brief scale x from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleXFrom, 1.0f)
    /**
     * @brief scale x to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleXTo, 1.0f)
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleYFrom, 1.0f)
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleYTo, 1.0f)
    /**
     * @brief scale y from, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleZFrom, 1.0f)
    /**
     * @brief scale y to, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, scaleZTo, 1.0f)

    /**
     * @brief translate x from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateXFrom, 0)
    /**
     * @brief translate x to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateXTo, 0)
    /**
     * @brief translate y from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateYFrom, 0)
    /**
     * @brief translate y to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateYTo, 0)
    /**
     * @brief translate y from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateZFrom, 0)
    /**
     * @brief translate y to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, translateZTo, 0)

    /**
     * @brief rotate x from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateXFrom, 0)
    /**
     * @brief rotate x to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateXTo, 0)
    /**
     * @brief rotate y from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateYFrom, 0)
    /**
     * @brief rotate y to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateYTo, 0)
    /**
     * @brief rotate z from, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateZFrom, 0)
    /**
     * @brief rotate z to, 0 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, rotateZTo, 0)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual zfbool aniImplCheckValid(void);
    zfoverride
    virtual void aniOnStart(void);
    zfoverride
    virtual void aniOnStop(ZF_IN ZFResultType resultType);

protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);
public:
    zffinal void _ZFP_ZFAniForNative_notifyStop(void) {
        this->aniImplNotifyStop();
    }

private:
    _ZFP_ZFAniForNativePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniForNative_h_

