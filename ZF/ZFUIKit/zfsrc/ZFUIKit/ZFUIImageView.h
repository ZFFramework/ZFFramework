/**
 * @file ZFUIImageView.h
 * @brief image view
 */

#ifndef _ZFI_ZFUIImageView_h_
#define _ZFI_ZFUIImageView_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageView
/**
 * @brief native image view
 */
zfclass ZFLIB_ZFUIKit ZFUIImageView : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIImageView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIImageView)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, viewUIEnable)
    ZFPROPERTY_ON_INIT_DECLARE(zfbool, viewUIEnableTree)

    /**
     * @brief image for this view
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, image)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, image)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImage>, image)

    /**
     * @brief util method to access #ZFUIImage::imageState
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUIImage>, imageState)

    /**
     * @brief image scale type, fill by default
     */
    ZFPROPERTY_ASSIGN(ZFUIScaleType, imageScaleType, v_ZFUIScaleType::EnumDefault())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIScaleType, imageScaleType)

    /**
     * @brief margin of the content image
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageMargin)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIMargin, imageMargin)

public:
    /**
     * @brief measure image view according to current image
     */
    ZFMETHOD_DECLARE_1(void, measureImageView
            , ZFMP_OUT(ZFUISize &, ret)
            )

protected:
    /**
     * @brief init with image
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFUIImage *, image))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

protected:
    zfoverride
    virtual void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    zfoverride
    virtual void nativeImplViewOnLayout(
            ZF_OUT ZFUIRect &ret
            , ZF_IN const ZFUIRect &bounds
            , ZF_IN const ZFUIMargin &nativeImplViewMargin
            );
    /**
     * @brief image view would measure according image size
     */
    zfoverride
    virtual void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            );

    zfoverride
    virtual void viewTreeVisibleOnUpdate(void);

private:
    ZFListener _ZFP_imageStateOnUpdateListener;
    void _ZFP_imageStateAttach(void);
    void _ZFP_imageStateDetach(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageView_h_

