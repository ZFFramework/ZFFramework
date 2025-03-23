/**
 * @file ZFProtocolZFUIDraw.h
 * @brief protocol for #ZFUIDraw
 */

#ifndef _ZFI_ZFProtocolZFUIDraw_h_
#define _ZFI_ZFProtocolZFUIDraw_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIDraw.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief token for impl */
zfclassNotPOD ZFLIB_ZFUIKit ZFUIDrawToken {
public:
    /** @brief type */
    typedef enum {
        TypeView, /**< @brief target is ZFUIDrawableView */
        TypeImage, /**< @brief target is ZFUIImage */
    } Type;
    Type type; /**< @brief type */
    zfauto target; /**< @brief target */
    void *impl; /**< @brief impl */

public:
    /** @brief main constructor */
    explicit ZFUIDrawToken(
            ZF_IN ZFUIDrawToken::Type type
            , ZF_IN ZFObject *target
            )
    : type(type)
    , target(target)
    , impl(zfnull)
    {
    }
};

// ============================================================
/**
 * @brief protocol for #ZFUIDrawableView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIDrawForView)
public:
    /**
     * @brief create native drawable view
     */
    virtual void *nativeDrawableViewCreate(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) zfpurevirtual;
    /**
     * @brief destroy native text view
     */
    virtual void nativeDrawableViewDestroy(
            ZF_IN ZFUIDrawableView *drawableView
            , ZF_IN void *nativeDrawableView
            ) zfpurevirtual;

    /** @brief see #ZFUIDrawableView::drawRequest */
    virtual void drawRequest(ZF_IN ZFUIDrawableView *drawableView) zfpurevirtual;

public:
    /** @brief see #ZFUIDraw::beginForView */
    virtual zfbool beginForView(ZF_IN_OUT ZFUIDrawToken &token) zfpurevirtual;
    /** @brief see #ZFUIDraw::beginForView */
    virtual void endForView(ZF_IN_OUT ZFUIDrawToken &token) zfpurevirtual;

public:
    /**
     * @brief implementations must notify when need to draw the view
     */
    zffinal void notifyOnDraw(ZF_IN ZFUIDrawableView *drawableView) {
        drawableView->_ZFP_ZFUIDrawableView_onDraw();
    }
ZFPROTOCOL_INTERFACE_END(ZFUIDrawForView)

// ============================================================
/**
 * @brief protocol for #ZFUIDraw
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIDrawForImage)
public:
    /** @brief see #ZFUIDraw::beginForImage */
    virtual zfbool beginForImage(
            ZF_IN_OUT ZFUIDrawToken &token
            , ZF_IN const ZFUISize &imageSizePixel
            ) zfpurevirtual;
    /** @brief see #ZFUIDraw::beginForImage */
    virtual void *endForImage(ZF_IN_OUT ZFUIDrawToken &token) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIDrawForImage)

// ============================================================
/**
 * @brief protocol for #ZFUIDraw
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFUIKit, ZFUIDraw)
public:
    /** @brief see #ZFUIDraw::beginForView */
    virtual zfbool drawClear(
            ZF_IN ZFUIDrawToken &token
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        return zffalse;
    }
    /** @brief see #ZFUIDraw::beginForView */
    virtual zfbool drawColor(
            ZF_IN ZFUIDrawToken &token
            , ZF_IN const ZFUIColor &color
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        return zffalse;
    }
    /** @brief see #ZFUIDraw::beginForView */
    virtual zfbool drawImage(
            ZF_IN ZFUIDrawToken &token
            , ZF_IN ZFUIImage *image
            , ZF_IN const ZFUIRect &imageFramePixel
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        return zffalse;
    }
    /** @brief see #ZFUIDraw::beginForView */
    virtual zfbool drawText(
            ZF_IN ZFUIDrawToken &token
            , ZF_IN const zfstring &text
            , ZF_IN ZFUITextConfig *config
            , ZF_IN const ZFUIRect &targetFramePixel
            ) {
        return zffalse;
    }
ZFPROTOCOL_INTERFACE_END(ZFUIDraw)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIDraw_h_

