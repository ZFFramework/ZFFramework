#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUILinearLayout_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUILinearLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUILinearLayout> layout;
        container->child(layout)->c_margin(40);
        layout->bgColor(ZFUIColorRed());

        this->prepareChildren(layout);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareChildren(ZF_IN ZFUILinearLayout *layout) {
        for(zfindex i = 0; i < 3; ++i) {
            zfobj<ZFUITextView> child;
            layout->child(child);
            child->text(zfstr("text %s", i));
            child->textAlign(v_ZFUIAlign::e_Center);
            child->bgColor(ZFUIColorRandom());
        }

        layout->childAt(1)->layoutParam().to<ZFUILinearLayoutParam *>()->weight(1);
        layout->childAt(2)->layoutParam().to<ZFUILinearLayoutParam *>()->weight(2);

        zfobj<ZFUILinearLayout> l;
        layout->child(l);
        l->orientation(v_ZFUIOrientation::e_Bottom);
        for(zfindex i = 0; i < 3; ++i) {
            zfobj<ZFUITextView> child;
            l->child(child);
            child->text(zfstr("text %s", 3 + i));
            child->textAlign(v_ZFUIAlign::e_Center);
            child->bgColor(ZFUIColorRandom());
        }
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUILinearLayout *layout
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientation, ZFPropertyAccess(ZFUILinearLayout, orientation),
            ZFCoreArrayCreate(ZFUIOrientation
                , v_ZFUIOrientation::e_Right
                , v_ZFUIOrientation::e_Bottom
                , v_ZFUIOrientation::e_Left
                , v_ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIMargin, ZFPropertyAccess(ZFUILinearLayout, childMargin),
            ZFCoreArrayCreate(ZFUIMargin
                , ZFUIMarginZero()
                , ZFUIMarginCreate(8)
                , ZFUIMarginCreate(12)
                , ZFUIMarginCreate(24)
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zffloat, ZFPropertyAccess(ZFUILinearLayout, childSpace),
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
ZFOBJECT_REGISTER(ZFUIWidget_ZFUILinearLayout_test)

ZF_NAMESPACE_GLOBAL_END

