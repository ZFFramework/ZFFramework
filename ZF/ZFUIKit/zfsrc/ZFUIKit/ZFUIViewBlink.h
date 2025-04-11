/**
 * @file ZFUIViewBlink.h
 * @brief blink view so that it's easy to be noticed
 */

#ifndef _ZFI_ZFUIViewBlink_h_
#define _ZFI_ZFUIViewBlink_h_

#include "ZFUIView.h"
#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief default mask image for #ZFUIViewBlink, an alpha white image by default
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIViewBlinkImageDefault)

/**
 * @brief param for #ZFUIViewBlink
 */
zfclassLikePOD ZFLIB_ZFUIKit ZFUIViewBlinkParam {
    ZFCORE_PARAM_DECLARE_SELF(ZFUIViewBlinkParam)

public:
    /** @brief image to blink, null to use default */
    ZFCORE_PARAM(zfautoT<ZFUIImage>, blinkImage)

    /** @brief duration to blink, 0 to use #ZFAniDuration, 0 by default */
    ZFCORE_PARAM(zftimet, blinkDuration)
    /** @brief blink count, 1 by default */
    ZFCORE_PARAM(zfindex, blinkCount, 1)

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIViewBlinkParam &ref) const {
        return (zftrue
                && this->blinkImage() == ref.blinkImage()
                && this->blinkDuration() == ref.blinkDuration()
                && this->blinkCount() == ref.blinkCount()
            );
    }
    zfbool operator != (ZF_IN const ZFUIViewBlinkParam &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFUIKit, ZFUIViewBlinkParam, ZFUIViewBlinkParam)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFUIKit, ZFUIViewBlinkParam, ZFUIViewBlinkParam)
ZFOUTPUT_TYPE(ZFUIViewBlinkParam, {v.objectInfoT(s);})
/**
 * @brief blink view so that it's easy to be noticed
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, void, ZFUIViewBlink
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFUIViewBlinkParam &, blinkParam, ZFUIViewBlinkParam())
        )
/**
 * @brief blink view so that it's easy to be noticed
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_ZFUIKit, void, ZFUIViewBlink
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN(ZFUIImage *, blinkImage)
        , ZFMP_IN_OPT(zftimet, blinkDuration, 0)
        , ZFMP_IN_OPT(zfindex, blinkCount, 1)
        ) {
    ZFUIViewBlink(view, ZFUIViewBlinkParam()
            .blinkImage(blinkImage)
            .blinkDuration(blinkDuration)
            .blinkCount(blinkCount)
        );
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view start to blink
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkOn)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was blinked\n
 * notified when a view stop blink
 */
ZFEVENT_GLOBAL(ZFLIB_ZFUIKit, ViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewBlink_h_

