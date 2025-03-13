#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test_Container : zfextend ZFUIView {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test_Container, ZFUIView)

    ZFPROPERTY_RETAIN(zfanyT<ZFUIImageView>, imageViewSmall, zfobj<ZFUIImageView>())
    ZFPROPERTY_RETAIN(zfanyT<ZFUIImageView>, imageViewLarge, zfobj<ZFUIImageView>())

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->bgColor(ZFUIColorYellow());

        zfauto imageSmall = zfres("test_normal.png");
        zfauto imageLarge = ZFUIImageScale(imageSmall, ZFUISizeCreate(600));

        this->child(this->imageViewSmall());
        this->imageViewSmall()->image(imageSmall);
        this->imageViewSmall()->bgColor(ZFUIColorRed());

        this->child(this->imageViewLarge());
        this->imageViewLarge()->image(imageLarge);
        this->imageViewLarge()->bgColor(ZFUIColorBlue());
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds) {
        zfsuper::layoutOnLayout(bounds);
        if(bounds.height >= bounds.width) {
            this->imageViewSmall()->viewFrame(ZFUIRectApplyMargin(
                ZFUIRectCreate(0, 0, bounds.width, bounds.height / 2),
                ZFUIMarginCreate(5)));
            this->imageViewLarge()->viewFrame(ZFUIRectApplyMargin(
                ZFUIRectCreate(0, bounds.height / 2, bounds.width, bounds.height / 2),
                ZFUIMarginCreate(5)));
        }
        else {
            this->imageViewSmall()->viewFrame(ZFUIRectCreate(0, 0, bounds.width, bounds.height / 2));
            this->imageViewLarge()->viewFrame(ZFUIRectCreate(bounds.width / 2, 0, bounds.width, bounds.height / 2));
        }
    }
};

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIImage);
        ZFFramework_test_protocolCheck(ZFUIImageView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIKit_ZFUIImageView_imageScaleType_test_Container> layout;
        container->child(layout)->c_sizeFill();

        zfobj<ZFArray> imageViews;
        imageViews->add(layout->imageViewSmall());
        imageViews->add(layout->imageViewLarge());
        this->prepareSettingButton(window, imageViews);
    }

private:
    static void _nextValue(ZF_IN_OUT ZFUIScaleType &value) {
        zfindex index = v_ZFUIScaleType::EnumIndexForValue(value);
        ZFCoreAssert(index != zfindexMax());
        ++index;
        if(index >= v_ZFUIScaleType::EnumCount()) {
            index = 0;
        }
        value = (ZFUIScaleType)v_ZFUIScaleType::EnumValueAt(index);
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFArray *imageViews
            ) {
        zfobj<ZFArray> settings;

        { // imageScaleType
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , zfautoT<ZFArray>, imageViews
                    ) {
                v_zfstring *text = zfargs.param0();

                ZFUIImageView *imageView = imageViews->getFirst();
                text->zfv = v_ZFUIScaleType::EnumNameForValue(imageView->imageScaleType());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , zfautoT<ZFArray>, imageViews
                    ) {
                ZFUIImageView *first = imageViews->getFirst();
                ZFUIScaleType value = first->imageScaleType();
                _nextValue(value);
                for(zfindex i = 0; i < imageViews->count(); ++i) {
                    ZFUIImageView *child = imageViews->get(i);
                    child->imageScaleType(value);
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageView_imageScaleType_test)

ZF_NAMESPACE_GLOBAL_END

