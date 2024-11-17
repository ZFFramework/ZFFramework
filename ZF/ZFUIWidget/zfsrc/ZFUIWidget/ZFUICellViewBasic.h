/**
 * @file ZFUICellViewBasic.h
 * @brief basic cell view with icon and label
 */

#ifndef _ZFI_ZFUICellViewBasic_h_
#define _ZFI_ZFUICellViewBasic_h_

#include "ZFUICellView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUICellViewBasic
zfclassFwd _ZFP_ZFUICellViewBasicPrivate;
/**
 * @brief basic cell view with icon and label
 */
zfclass ZFLIB_ZFUIWidget ZFUICellViewBasic : zfextend ZFUICellView {
    ZFOBJECT_DECLARE(ZFUICellViewBasic, ZFUICellView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUICellViewBasic)

public:
    // ============================================================
    // properties
    /**
     * @brief cell's icon
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUIImageView>, cellIcon, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUIImageView>, cellIcon)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUIImageView>, cellIcon)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUIImageView>, cellIcon)

    /**
     * @brief cell's main label
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, cellLabelMain, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, cellLabelMain)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, cellLabelMain)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, cellLabelMain)

    /**
     * @brief cell's sub label
     */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFUITextView>, cellLabelSub, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(zfanyT<ZFUITextView>, cellLabelSub)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfanyT<ZFUITextView>, cellLabelSub)
    ZFPROPERTY_ON_DETACH_DECLARE(zfanyT<ZFUITextView>, cellLabelSub)

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, cellIconContainer)
    /**
     * @brief container for center item, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, cellCenterContainer)
    /**
     * @brief container for main label, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, cellLabelMainContainer)
    /**
     * @brief container for sub label, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, cellLabelSubContainer)
    /**
     * @brief container for accessory, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, cellAccessoryContainer)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUICellViewBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellViewBasic_h_

