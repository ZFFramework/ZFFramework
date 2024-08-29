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

protected:
    /** @brief called to load image */
    virtual void imageOnLoad(ZF_IN const ZFInput &src);
    /** @brief when #imageUrl or #imageSrc set, #ZFUIImageView::image would be ignored from serialize */
    ZFMETHOD_DECLARE_PROTECTED_1(ZFSerializablePropertyType, serializableOnCheckPropertyType
            , ZFMP_IN(const ZFProperty *, property)
            )
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageViewExt_h_

