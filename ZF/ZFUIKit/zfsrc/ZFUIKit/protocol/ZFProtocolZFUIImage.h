/**
 * @file ZFProtocolZFUIImage.h
 * @brief protocol for ZFUIImage
 */

#ifndef _ZFI_ZFProtocolZFUIImage_h_
#define _ZFI_ZFProtocolZFUIImage_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIImage
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIImage)
public:
    /**
     * @brief load image from input as binary data,
     *   created image would be released by #nativeImageRelease
     */
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) zfpurevirtual;
    /**
     * @brief save image to output as binary data
     */
    virtual zfbool nativeImageToOutput(
            ZF_IN void *nativeImage
            , ZF_OUT const ZFOutput &outputCallback
            ) zfpurevirtual;

    /**
     * @brief copy the nativeImage and return the copied one,
     *   copied image would be released by #nativeImageRelease
     */
    virtual void *nativeImageCopy(ZF_IN void *nativeImage) zfpurevirtual;

    /**
     * @brief retain the image,
     *   you may return a different one
     */
    virtual void *nativeImageRetain(ZF_IN void *nativeImage) zfpurevirtual;
    /**
     * @brief release the image
     *
     * you should implement retain count logic internally
     */
    virtual void nativeImageRelease(ZF_IN void *nativeImage) zfpurevirtual;

    /**
     * @brief get size of the image (in pixel)
     */
    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIImage)

/**
 * @brief see #ZFUIImageImplNinePatchCalc
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIImageImplNinePatchPos)
    ZFENUM_VALUE(TopLeft)
    ZFENUM_VALUE(TopCenter)
    ZFENUM_VALUE(TopRight)
    ZFENUM_VALUE(CenterLeft)
    ZFENUM_VALUE(CenterCenter)
    ZFENUM_VALUE(CenterRight)
    ZFENUM_VALUE(BottomLeft)
    ZFENUM_VALUE(BottomCenter)
    ZFENUM_VALUE(BottomRight)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(TopLeft)
    ZFENUM_VALUE_REGISTER(TopCenter)
    ZFENUM_VALUE_REGISTER(TopRight)
    ZFENUM_VALUE_REGISTER(CenterLeft)
    ZFENUM_VALUE_REGISTER(CenterCenter)
    ZFENUM_VALUE_REGISTER(CenterRight)
    ZFENUM_VALUE_REGISTER(BottomLeft)
    ZFENUM_VALUE_REGISTER(BottomCenter)
    ZFENUM_VALUE_REGISTER(BottomRight)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIImageImplNinePatchPos)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUIImageImplNinePatchPos)

/**
 * @brief see #ZFUIImageImplNinePatchCalc
 */
zfclassPOD ZFLIB_ZFUIKit ZFUIImageImplNinePatchDrawData {
public:
    ZFUIImageImplNinePatchPos position; /**< @brief position of the nine patch piece */
    ZFUIRect src; /**< @brief src rect to draw bitmap from, ensured valid */
    ZFUIRect dst; /**< @brief dst rect to draw bitmapt to, ensured valid */
};
ZFCORE_POD_DECLARE(ZFUIImageImplNinePatchDrawData)
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFUIKit, ZFUIImageImplNinePatchDrawData)

/**
 * @brief util method for implementation to calculate nine patch areas
 *
 * return draw areas containing src and dst rect info,
 * only valid rects would be stored in result,
 * outBuf should be at least (9 * sizeof(ZFUIImageImplNinePatchDrawData)),
 * typically you may use "ZFUIImageImplNinePatchDrawData buf[9];"
 * to supply the buf
 * @note all size use raw pixel as size unit
 */
extern ZFLIB_ZFUIKit zfindex ZFUIImageImplNinePatchCalc(
        ZF_OUT ZFUIImageImplNinePatchDrawData *outBuf
        , ZF_IN const ZFUISize &srcSize
        , ZF_IN const ZFUIMargin &srcNinePatch
        , ZF_IN const ZFUISize &dstSize
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIImage_h_

