/**
 * @file ZFUIImageIO.h
 * @brief common image load logic
 */

#ifndef _ZFI_ZFUIImageIO_h_
#define _ZFI_ZFUIImageIO_h_

#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// raw image io
/**
 * @brief load image from base64 data
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageFromBase64
        , ZFMP_IN(const ZFInput &, inputCallback)
        )
/**
 * @brief save image to base64 data
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIImageToBase64
        , ZFMP_OUT(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFUIImage *, image)
        )
/**
 * @brief load image from file
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageFromInput
        , ZFMP_IN(const ZFInput &, inputCallback)
        )
/** @brief see #ZFUIImageFromInput */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIImageFromInputT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, inputCallback)
        )
/**
 * @brief save image to file
 *
 * @note save #ZFUIImage::nativeImage to file,
 *   the #ZFUIImage::imageStateImpl is not processed,
 *   you may use #ZFObjectToData to serialize the image object
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfbool, ZFUIImageToOutput
        , ZFMP_OUT(const ZFOutput &, outputCallback)
        , ZFMP_IN(ZFUIImage *, image)
        )

/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFUIImage imageType="ZFUIImageFromInput">
 *       <ZFCallback category="imageData" ... /> // callback serialization, see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *   </ZFUIImage>
 * @endcode
 */
#define ZFUIImageSerializeType_ZFUIImageFromInput "ZFUIImageFromInput"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageFromInput_imageData "imageData"

// ============================================================
// ZFUIImageScale
/**
 * @brief scale and return the newly created scaled image
 *
 * if the source image have nine patch,
 * scale would use the nine patch setting\n
 * return null if source image or newSize invalid\n
 * this method would create a new image even if size not changed
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageScale
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUISize &, newSize)
        )

// ============================================================
// ZFUIImageInFrame
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFUIImage
 *       imageType="ZFUIImageInFrame"
 *       refFrame="ZFUIRect"
 *   >
 *       <ZFUIImage category="ref" ... />
 *   </ZFUIImage>
 * @endcode
 */
#define ZFUIImageSerializeType_ZFUIImageInFrame "ZFUIImageInFrame"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageInFrame_ref "ref"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageInFrame_refFrame "refFrame"

/**
 * @brief clip an exist image and sharing low level data if possible
 * @note this method has no cache logic
 * @note see #ZFUIImage::imageScale for more info
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageInFrame
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUIRect &, frame)
        )
/** @brief see #ZFUIImageInFrame */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, zfbool, ZFUIImageInFrameT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN(const ZFUIRect &, frame)
        )

// ============================================================
// ZFUIImageFromNativeImage
/**
 * @brief create image from native image
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageFromNativeImage
        , ZFMP_IN(void *, nativeImage)
        , ZFMP_IN_OPT(zfbool, retainNativeImage, zftrue)
        )

// ============================================================
// color
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFUIImage
 *       imageType="ZFUIImageFromColor"
 *       color="ZFUIColor" // optional, transparent by default
 *       size="ZFUISize" // optional, (1, 1) by default
 *   >
 *   </ZFUIImage>
 * @endcode
 */
#define ZFUIImageSerializeType_ZFUIImageFromColor "ZFUIImageFromColor"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageFromColor_color "color"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageFromColor_size "size"

/**
 * @brief load image from color
 *
 * invalid size is automatically convert to 1
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, ZFUIImageFromColor
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero())
        )
/** @brief see #ZFUIImageFromColor */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, zfbool, ZFUIImageFromColorT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageIO_h_

