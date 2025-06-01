/**
 * @file ZFUIImageAni.h
 * @brief animated image load util
 */

#ifndef _ZFI_ZFUIImageAni_h_
#define _ZFI_ZFUIImageAni_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// images
/**
 * @brief animated image util
 *
 * how it works:
 * -# return a dummy holder image
 * -# update each image frame by #ZFUIImage::imageStateImplNotifyUpdate
 *
 * @note the src image would be retained by the returned image object
 * @note see also #ZFUIImage::imageScale for frame size
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )

// ============================================================
// split
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        )
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        )

/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        )
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(const ZFUISize &, frameSize)
        , ZFMP_IN(zfindex, frameCount)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        )

// ============================================================
// frames
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(ZFUIImage *, ref)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )

/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAni
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )
/** @brief see #ZFUIImageAni */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniT
        , ZFMP_IN_OUT(ZFUIImage *, ret)
        , ZFMP_IN(const ZFInput &, refSrc)
        , ZFMP_IN(ZFArray *, frameRects)
        , ZFMP_IN_OPT(zftimet, duration, 0)
        , ZFMP_IN_OPT(ZFArray *, frameDurations, zfnull)
        )

// ============================================================
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFUIImage imageType="ZFUIImageAni">
 *
 *       // type 1 : each image frame
 *       <node category="images" duration="123" >
 *           <ZFUIImage duration="123" />
 *           ...
 *       </node>
 *
 *       // type 2 : split large ref image by same config for each frame
 *       <ZFUIImage category="ref" ... /> // large ref image
 *       <ZFCallback category="refSrc" ... /> // large ref image
 *       <node category="split" // how the large ref image split to frames
 *           size="(w,h)"
 *           count="123"
 *           duration="123"
 *           />
 *
 *       // type 3 : split large ref image by different config
 *       <ZFUIImage category="ref" ... /> // large ref image
 *       <ZFCallback category="refSrc" ... /> // large ref image
 *       <node category="frames" duration="123" >
 *           <node rect="(l,t,w,h)" duration="123" />
 *           ...
 *       </node>
 *   </ZFUIImage>
 * @endcode
 */
#define ZFUIImageSerializeType_ZFUIImageAni "ZFUIImageAni"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_images "images"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_ref "ref"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_refSrc "refSrc"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_split "split"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_frames "frames"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_size "size"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_count "count"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_duration "duration"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageAni_rect "rect"

// ============================================================
/**
 * @brief util function to load animated image with special naming logic
 *
 * the src input must support #ZFCallback::callbackId or #ZFCallback::pathInfo with this naming rule:
 * -  `xxx.wxh-N.xxx`, for example: `path/test.40x30-10.png`
 * -  `xxx.wxh-N-DURATION.xxx`, for example: `path/test.40x30-10-33.png`
 *
 * then the result animated image would have:
 * -  `40x30` as each frame size
 * -  `10` as frame count
 * -  `33` as each frame duration
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIWidget, zfautoT<ZFUIImage>, ZFUIImageAniLoad
        , ZFMP_IN(const ZFInput &, input)
        )

/** @brief try parse rules declared in #ZFUIImageAniLoad */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniLoadCheck
        , ZFMP_OUT(ZFUISize &, frameSize)
        , ZFMP_OUT(zfindex &, frameCount)
        , ZFMP_OUT(zftimet &, frameDuration)
        , ZFMP_IN(const zfstring &, fileName)
        )

/**
 * @brief util function to save animated image with special logic
 *
 * save to a single large image with special naming rule, see #ZFUIImageAniLoad\n
 * \n
 * the dst path info must support these to achieve the special naming rule:
 * -  #ZFPathInfoToFileName
 * -  #ZFPathInfoToParent
 * -  #ZFPathInfoToChild
 *
 * a special file name would be appended to dst,
 * for example, if dst is `path/test.png`,
 * then the final dst to output may be `path/test.40x30-10.png`
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIWidget, zfbool, ZFUIImageAniSave
        , ZFMP_IN(const ZFPathInfo &, dst)
        , ZFMP_IN(ZFArray *, images)
        , ZFMP_IN_OPT(zftimet, frameDuration, 0)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageAni_h_

