/**
 * @file ZFUIImageAsync.h
 * @brief async image load util
 */

#ifndef _ZFI_ZFUIImageAsync_h_
#define _ZFI_ZFUIImageAsync_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief async image load util
 *
 * how it works:
 * -# return a dummy holder image
 * -# loading image in background, by #ZFUIImageLoad
 * -# notify impl to redraw, by #ZFUIImage::imageStateImplNotifyUpdate
 *
 * @note the imageLoadFail and imageLoading would be retained by the returned image object
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAsync
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoadFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
        )
/** @brief see #ZFUIImageAsync */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAsyncT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, src)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoadFail, zfnull)
        , ZFMP_IN_OPT(ZFUIImage *, imageLoading, zfnull)
        )

/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFUIImage imageType="ZFUIImageAsync">
 *       <ZFCallback category="imageSrc" ... /> // callback serialization, see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *       <ZFUIImage category="imageLoadFail" ... /> // load fail image
 *       <ZFUIImage category="imageLoading" ... /> // loading image
 *   </ZFUIImage>
 * @endcode
 */
#define ZFUIImageSerializeType_ZFUIImageAsync "ZFUIImageAsync"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAsync_imageSrc "imageSrc"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAsync_imageLoadFail "imageLoadFail"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAsync_imageLoading "imageLoading"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageAsync_h_

