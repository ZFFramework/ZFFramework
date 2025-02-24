#include "ZFUICellViewBasic.h"
#include "ZFUIViewLayout.h"
#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUICellViewBasicPrivate {
public:
    ZFUIViewLayout *cellIconContainer;
    ZFUILinearLayout *cellCenterContainer;
    ZFUIViewLayout *cellLabelMainContainer;
    ZFUIViewLayout *cellLabelSubContainer;
    ZFUIViewLayout *cellAccessoryContainer;
};

// ============================================================
ZFOBJECT_REGISTER(ZFUICellViewBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUICellViewBasic)

ZFPROPERTY_ON_INIT_DEFINE(ZFUICellViewBasic, zfanyT<ZFUIImageView>, cellIcon) {
    zfobj<ZFUIImageView> cellIcon;
    propertyValue = cellIcon;
    cellIcon->viewSizeMax(ZFUISizeCreate(ZFUIGlobalStyle::DefaultStyle()->itemSizeIcon()));
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUIImageView>, cellIcon) {
    this->cellIconContainer()->child(this->cellIcon())->c_alignCenter();
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUIImageView>, cellIcon) {
    this->cellIcon()->removeFromParent();
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelMain) {
    zfobj<ZFUITextView> cellLabelMain;
    propertyValue = cellLabelMain;
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelMain) {
    this->cellLabelMainContainer()->child(this->cellLabelMain())->c_alignLeft();
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelMain) {
    this->cellLabelMain()->removeFromParent();
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelSub) {
    zfobj<ZFUITextView> cellLabelSub;
    propertyValue = cellLabelSub;
    cellLabelSub->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
    cellLabelSub->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelSub) {
    this->cellLabelSubContainer()->child(this->cellLabelSub())->c_alignLeft();
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUICellViewBasic, zfanyT<ZFUITextView>, cellLabelSub) {
    this->cellLabelSub()->removeFromParent();
}

ZFMETHOD_DEFINE_0(ZFUICellViewBasic, zfanyT<ZFUIView>, cellIconContainer) {
    return d->cellIconContainer;
}
ZFMETHOD_DEFINE_0(ZFUICellViewBasic, zfanyT<ZFUIView>, cellCenterContainer) {
    return d->cellCenterContainer;
}
ZFMETHOD_DEFINE_0(ZFUICellViewBasic, zfanyT<ZFUIView>, cellLabelMainContainer) {
    return d->cellLabelMainContainer;
}
ZFMETHOD_DEFINE_0(ZFUICellViewBasic, zfanyT<ZFUIView>, cellLabelSubContainer) {
    return d->cellLabelSubContainer;
}
ZFMETHOD_DEFINE_0(ZFUICellViewBasic, zfanyT<ZFUIView>, cellAccessoryContainer) {
    return d->cellAccessoryContainer;
}

void ZFUICellViewBasic::objectOnInit(void) {
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUICellViewBasicPrivate);

    d->cellIconContainer = zfAlloc(ZFUIViewLayout);

    {
        d->cellCenterContainer = zfAlloc(ZFUILinearLayout);
        d->cellCenterContainer->orientation(ZFUIOrientation::e_Top);

        d->cellLabelMainContainer = zfAlloc(ZFUIViewLayout);
        d->cellCenterContainer->child(d->cellLabelMainContainer)->c_alignLeft();

        d->cellLabelSubContainer = zfAlloc(ZFUIViewLayout);
        d->cellCenterContainer->child(d->cellLabelSubContainer)->c_alignLeft();
    }

    d->cellAccessoryContainer = zfAlloc(ZFUIViewLayout);
}
void ZFUICellViewBasic::objectOnDealloc(void) {
    zfRelease(d->cellIconContainer);
    zfRelease(d->cellCenterContainer);
    zfRelease(d->cellLabelMainContainer);
    zfRelease(d->cellLabelSubContainer);
    zfRelease(d->cellAccessoryContainer);

    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

