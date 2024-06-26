#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIFlowLayout_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIFlowLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIFlowLayout> layout;
        container->childAdd(layout)->c_margin(40, 40 + ZFUIGlobalStyle::DefaultStyle()->itemSizeButton(), 40, 40);
        layout->viewBackgroundColor(ZFUIColorRed());

        this->prepareAddRemoveButton(container, layout);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareAddRemoveButton(ZFUIView *container, ZFUIView *layout) {
        zfobj<ZFUIKit_test_Button> addButton;
        container->childAdd(addButton);
        ZFLISTENER_1(addButtonOnClick
                , ZFUIView *, layout
                ) {
            zfobj<ZFUITextView> view;
            layout->childAdd(view);
            view->viewBackgroundColor(ZFUIColorRandom());
            zfindex textLength = zfmRand(1, 10);
            textLength = layout->childCount() + 1;
            zfstring text;
            text.capacity(textLength);
            zfchar c = '0' + ((layout->childCount() - 1) % 10);
            for(zfindex i = 0; i < textLength; ++i) {
                text += c;
            }
            view->text(text);
        } ZFLISTENER_END()
        addButton->observerAdd(ZFUIButton::EventButtonOnClick(), addButtonOnClick);
        addButton->label()->text("add");

        zfobj<ZFUIKit_test_Button> removeButton;
        container->childAdd(removeButton)->c_margin(70, 0, 0, 0);
        ZFLISTENER_1(removeButtonOnClick
                , ZFUIView *, layout
                ) {
            if(layout->childCount() > 0) {
                layout->childRemoveAt(layout->childCount() - 1);
            }
        } ZFLISTENER_END()
        removeButton->observerAdd(ZFUIButton::EventButtonOnClick(), removeButtonOnClick);
        removeButton->label()->text("remove");
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIFlowLayout *layout
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationMain),
            ZFCoreArrayCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationSecondary),
            ZFCoreArrayCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIMargin, ZFPropertyAccess(ZFUIFlowLayout, layoutChildMargin),
            ZFCoreArrayCreate(ZFUIMargin
                , ZFUIMarginZero()
                , ZFUIMarginMake(8)
                , ZFUIMarginMake(12)
                , ZFUIMarginMake(24)
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zffloat, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceX),
            ZFCoreArrayCreate(zffloat
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zffloat, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceY),
            ZFCoreArrayCreate(zffloat
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->layoutParam(), ZFUISizeParam, ZFPropertyAccess(ZFUILayoutParam, sizeParam),
            ZFCoreArrayCreate(ZFUISizeParam
                , ZFUISizeParamWrapWrap()
                , ZFUISizeParamFillWrap()
                , ZFUISizeParamWrapFill()
                , ZFUISizeParamFillFill()
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIFlowLayout_test)

ZF_NAMESPACE_GLOBAL_END

