#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test_Container : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test_Container, ZFUIView)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImageView *, imageViewSmall, zflineAlloc(ZFUIImageView))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImageView *, imageViewLarge, zflineAlloc(ZFUIImageView))

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->viewBackgroundColor(ZFUIColorYellow());

        zfautoObject imageSmall = zfRes("test_normal.png");
        zfautoObject imageLarge = ZFUIImageScale(imageSmall, ZFUISizeMake(600));

        this->childAdd(this->imageViewSmall());
        this->imageViewSmall()->image(imageSmall);
        this->imageViewSmall()->viewBackgroundColor(ZFUIColorRed());

        this->childAdd(this->imageViewLarge());
        this->imageViewLarge()->image(imageLarge);
        this->imageViewLarge()->viewBackgroundColor(ZFUIColorBlue());
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        zfsuper::layoutOnLayout(bounds);
        if(bounds.height >= bounds.width)
        {
            this->imageViewSmall()->viewFrame(ZFUIRectApplyMargin(
                ZFUIRectMake(0, 0, bounds.width, bounds.height / 2),
                ZFUIMarginMake(5)));
            this->imageViewLarge()->viewFrame(ZFUIRectApplyMargin(
                ZFUIRectMake(0, bounds.height / 2, bounds.width, bounds.height / 2),
                ZFUIMarginMake(5)));
        }
        else
        {
            this->imageViewSmall()->viewFrame(ZFUIRectMake(0, 0, bounds.width, bounds.height / 2));
            this->imageViewLarge()->viewFrame(ZFUIRectMake(bounds.width / 2, 0, bounds.width, bounds.height / 2));
        }
    }
};

zfclass ZFUIKit_ZFUIImageView_imageScaleType_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageView_imageScaleType_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIImage);
        ZFFramework_test_protocolCheck(ZFUIImageView);

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIKit_ZFUIImageView_imageScaleType_test_Container, layout);
        container->childAdd(layout)->c_sizeFill();

        zfblockedAlloc(ZFArray, imageViews);
        imageViews->add(layout->imageViewSmall());
        imageViews->add(layout->imageViewLarge());
        this->prepareSettingButton(window, imageViews);
    }

private:
    static void _nextValue(ZF_IN_OUT ZFUIContentScaleTypeEnum &value)
    {
        zfindex index = ZFUIContentScaleType::EnumIndexForValue(value);
        zfCoreAssert(index != zfindexMax());
        ++index;
        if(index >= ZFUIContentScaleType::EnumCount())
        {
            index = 0;
        }
        value = (ZFUIContentScaleTypeEnum)ZFUIContentScaleType::EnumValueAt(index);
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFArray *imageViews)
    {
        zfblockedAlloc(ZFArray, settings);

        { // imageScaleType
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , zfautoObjectT<ZFArray *>, imageViews
                    ) {
                v_zfstring *text = zfargs.param0T();

                ZFUIImageView *imageView = imageViews->getFirst<ZFUIImageView *>();
                text->zfv = ZFUIContentScaleType::EnumNameForValue(imageView->imageScaleType());
            } ZFLISTENER_END(buttonTextGetter)
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , zfautoObjectT<ZFArray *>, imageViews
                    ) {
                ZFUIContentScaleTypeEnum value = imageViews->getFirst<ZFUIImageView *>()->imageScaleType();
                _nextValue(value);
                for(zfindex i = 0; i < imageViews->count(); ++i)
                {
                    imageViews->get<ZFUIImageView *>(i)->imageScaleType(value);
                }
            } ZFLISTENER_END(buttonClickListener)
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageView_imageScaleType_test)

ZF_NAMESPACE_GLOBAL_END

