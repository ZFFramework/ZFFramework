/**
 * @file ZFUIDraw.h
 * @brief global style for ui elements
 */

#ifndef _ZFI_ZFUIDraw_h_
#define _ZFI_ZFUIDraw_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
#include "ZFUIText.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDrawableView
/**
 * @brief abstract view that can draw user content
 */
zfclass ZFLIB_ZFUIKit ZFUIDrawableView : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIDrawableView, ZFUIView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to draw the contents,
     * see #ZFUIDrawableView
     */
    ZFEVENT(OnDraw)

public:
    /**
     * @brief mark the view needs to redraw, see #ZFUIDrawableView
     */
    ZFMETHOD_DECLARE_0(void, drawRequest)

    /** @brief true if during #onDraw */
    ZFMETHOD_DECLARE_0(zfbool const &, drawing)

protected:
    /** @brief see #E_OnDraw */
    virtual inline void onDraw(void) {
        this->observerNotify(zfself::E_OnDraw());
    }
public:
    zffinal inline void _ZFP_ZFUIDrawableView_onDraw(void) {
        this->_drawing = zftrue;
        this->onDraw();
        this->_drawing = zffalse;
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
private:
    zfbool _drawing;
};

ZF_NAMESPACE_BEGIN_REGISTER(ZFUIDraw, ZF_NAMESPACE_GLOBAL)

// ============================================================
// context
/**
 * @brief begin a draw context
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void *, beginForView
        , ZFMP_IN(ZFUIDrawableView *, view)
        )
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void, endForView
        , ZFMP_IN(void *, context)
        )

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, void *, beginForImage
        , ZFMP_IN(const ZFUISize &, imageSize)
        )
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUIKit, zfautoT<ZFUIImage>, endForImage
        , ZFMP_IN(void *, context)
        )

// ============================================================
// draw api
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFUIKit, zfbool, drawClear
        , ZFMP_IN(void *, context)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        )

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUIKit, zfbool, drawColor
        , ZFMP_IN(void *, context)
        , ZFMP_IN(const ZFUIColor &, color)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        )

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIKit, zfbool, drawImage
        , ZFMP_IN(void *, context)
        , ZFMP_IN(ZFUIImage *, image)
        , ZFMP_IN_OPT(const ZFUIRect &, imageFrame, ZFUIRectZero())
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        )

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFUIKit, zfbool, drawText
        , ZFMP_IN(void *, context)
        , ZFMP_IN(const zfstring &, text)
        , ZFMP_IN_OPT(ZFUITextConfig *, config, zfnull)
        , ZFMP_IN_OPT(const ZFUIRect &, targetFrame, ZFUIRectZero())
        )

ZF_NAMESPACE_END(ZFUIDraw)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDraw_h_

