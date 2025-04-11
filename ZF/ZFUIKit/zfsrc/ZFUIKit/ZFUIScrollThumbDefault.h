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
zfclass ZFLIB_ZFUIKit ZFUIScrollThumbDefault : zfextend ZFStyle, zfimplement ZFUIScrollThumb {
    ZFOBJECT_DECLARE(ZFUIScrollThumbDefault, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFUIScrollThumb)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollThumbDefault)

public:
    // ============================================================
    // properties
    /**
     * @brief horizontal scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, thumbImageHorizontal, zfres("ZFUIKit/ZFUIScrollThumbDefault_thumb.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, thumbImageHorizontal)
    /**
     * @brief vertical scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImage>, thumbImageVertical, zfres("ZFUIKit/ZFUIScrollThumbDefault_thumb.xml"))
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImage>, thumbImageVertical)

    /**
     * @brief delay to auto hide, -1 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, autoHideDelayHorizontal, -1)
    /**
     * @brief delay to auto hide, -1 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, autoHideDelayVertical, -1)

    /**
     * @brief time to auto hide, -1 by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN(zftimet, autoHideDurationHorizontal, -1)
    /**
     * @brief time to auto hide, -1 by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN(zftimet, autoHideDurationVertical, -1)

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

