/**
 * @file ZFUIScrollThumbDefault.h
 * @brief default scroll thumb of #ZFUIScroller
 */

#ifndef _ZFI_ZFUIScrollThumbDefault_h_
#define _ZFI_ZFUIScrollThumbDefault_h_

#include "ZFUIScrollThumb.h"
#include "ZFUIScrollView.h"
#include "ZFUIImage.h"
#include "ZFUIImageView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIScrollThumbDefaultPrivate;
/**
 * @brief default scroll thumb for #ZFUIScroller
 */
zfclass ZFLIB_ZFUIKit ZFUIScrollThumbDefault : zfextend ZFStyleableObject, zfimplement ZFUIScrollThumb {
    ZFOBJECT_DECLARE(ZFUIScrollThumbDefault, ZFStyleableObject)
    ZFIMPLEMENT_DECLARE(ZFUIScrollThumb)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollThumbDefault)

public:
    // ============================================================
    // properties
    /**
     * @brief horizontal scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, scrollThumbImageHorizontal, zfRes("ZFUIKit/ZFUIScrollThumbDefault_thumb.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, scrollThumbImageHorizontal)
    /**
     * @brief vertical scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, scrollThumbImageVertical, zfRes("ZFUIKit/ZFUIScrollThumbDefault_thumb.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, scrollThumbImageVertical)

    /**
     * @brief delay to auto hide, #ZFAnimationDurationDefault by default
     */
    ZFPROPERTY_ASSIGN(zftimet, scrollThumbAutoHideDelayHorizontal, ZFAnimationDurationDefault())
    /**
     * @brief delay to auto hide, #ZFAnimationDurationDefault by default
     */
    ZFPROPERTY_ASSIGN(zftimet, scrollThumbAutoHideDelayVertical, ZFAnimationDurationDefault())

    /**
     * @brief time to auto hide, #ZFAnimationDurationDefault by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN(zftimet, scrollThumbAutoHideDurationHorizontal, ZFAnimationDurationDefault())
    /**
     * @brief time to auto hide, #ZFAnimationDurationDefault by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN(zftimet, scrollThumbAutoHideDurationVertical, ZFAnimationDurationDefault())

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    zfoverride
    virtual void scrollThumbInit(void);
    zfoverride
    virtual void scrollThumbDealloc(void);

    zfoverride
    virtual void scrollThumbUpdate(void);

private:
    _ZFP_ZFUIScrollThumbDefaultPrivate *d;
    friend zfclassFwd _ZFP_ZFUIScrollThumbDefaultPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollThumbDefault_h_

