/**
 * @file ZFUIImageViewExt.h
 * @brief util extension for #ZFUIImageView, see #ZFCLASS_EXTEND
 */

#ifndef _ZFI_ZFUIImageViewExt_h_
#define _ZFI_ZFUIImageViewExt_h_

#include "ZFUIExtDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util extension for #ZFUIImageView, see #ZFCLASS_EXTEND
 */
zfclass ZFLIB_ZFUIExt ZFUIImageViewExt : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIImageViewExt, ZFObject)

public:
    /**
     * @brief the image to show while #imageUrl is loading
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, imageLoading)
    /**
     * @brief the image to show while #imageUrl load failed
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, imageLoadFail)

    /**
     * @brief async load image from url, by #ZFUIImageLoad
     */
    ZFPROPERTY_ASSIGN(zfstring, imageUrl)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, imageUrl)

    /**
     * @brief async load image from input, by #ZFUIImageLoad
     */
    ZFPROPERTY_ASSIGN(ZFInput, imageSrc)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFInput, imageSrc)

public:
    /**
     * @brief animated image from url, see #ZFUIImageAniLoad
     */
    ZFPROPERTY_ASSIGN(zfstring, imageAniUrl)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, imageAniUrl)

    /**
     * @brief animated image from input, see #ZFUIImageAniLoad
     */
    ZFPROPERTY_ASSIGN(ZFInput, imageAniSrc)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFInput, imageAniSrc)

    /**
     * @brief animated image from url, see #ZFUIImageAni
     */
    ZFMETHOD_DECLARE_4(void, imageAniUrl
            , ZFMP_IN(const zfchar *, url)
            , ZFMP_IN(const ZFUISize &, frameSize)
            , ZFMP_IN(zfindex, frameCount)
            , ZFMP_IN_OPT(zftimet, frameDuration, 0)
            )
    /**
     * @brief animated image from input, see #ZFUIImageAni
     */
    ZFMETHOD_DECLARE_4(void, imageAniSrc
            , ZFMP_IN(const ZFInput &, src)
            , ZFMP_IN(const ZFUISize &, frameSize)
            , ZFMP_IN(zfindex, frameCount)
            , ZFMP_IN_OPT(zftimet, frameDuration, 0)
            )

protected:
    /** @brief when #imageUrl or #imageSrc set, #ZFUIImageView::image would be ignored from serialize */
    ZFMETHOD_DECLARE_PROTECTED_1(ZFSerializablePropertyType, serializableOnCheckPropertyType
            , ZFMP_IN(const ZFProperty *, property)
            )
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageViewExt_h_

