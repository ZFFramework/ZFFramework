#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKit_test_Window)
ZFOBJECT_REGISTER(ZFUIKit_test_Button)
ZFOBJECT_REGISTER(ZFUIKit_test_ListView)

ZFOBJECT_REGISTER(ZFUIKit_test_SettingData)
ZFEVENT_REGISTER(ZFUIKit_test_SettingData, SettingOnUpdate)

void ZFUIKit_test_prepareTestWindow(
        ZF_OUT ZFUIWindow *&window
        , ZF_OUT ZFUIView *&container
        , ZF_IN ZFTestCase *testCaseToStop
        ) {
    window = zfAlloc(ZFUIKit_test_Window);
    zfblockedRelease(window);
    window->show();

    // close button
    zfobj<ZFUIKit_test_Button> closeButton;
    window->child(closeButton)->c_alignRightTop();
    closeButton->label()->text("close");
    ZFLISTENER_2(onClickCloseButton
            , ZFTestCase *, testCaseToStop
            , ZFUIWindow *, window
            ) {
        testCaseToStop->stop();
        window->hide();
    } ZFLISTENER_END()
    closeButton->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickCloseButton);

    // container
    container = zfAlloc(ZFUIView);
    zfblockedRelease(container);
    window->child(container)->c_sizeFill()->c_margin(0, 50, 0, 0);
}

zfauto ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings) {
    zfobj<ZFUIKit_test_Button> settingsButton;
    settingsButton->label()->text("settings");
    settingsButton->objectTag("settingsHolder", settings);

    zfobj<ZFUIKit_test_Window> window;
    settingsButton->objectTag("setting window", window);
    window->alpha(0.8f);
    ZFLISTENER_1(onClickSetting
            , ZFUIWindow *, window
            ) {
        window->show();
    } ZFLISTENER_END()
    settingsButton->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickSetting);

    zfobj<ZFUIKit_test_Button> closeButton;
    window->child(closeButton)->c_alignTop();
    closeButton->label()->text("done");
    ZFLISTENER_1(onClickCloseButton
            , ZFUIWindow *, window
            ) {
        window->hide();
    } ZFLISTENER_END()
    closeButton->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickCloseButton);

    zfobj<ZFUIKit_test_ListView> listView;
    window->child(listView)->c_sizeFill()->c_margin(0, 50, 0, 0);
    for(zfindex i = 0; i < settings->count(); ++i) {
        ZFUIKit_test_SettingData *setting = settings->get(i);
        ZFCoreAssert(setting->buttonTextGetter());
        ZFCoreAssert(setting->buttonClickListener());

        zfobj<ZFUIKit_test_Button> button;
        listView->child(button);
        ZFLISTENER_1(onButtonClick
                , ZFUIKit_test_SettingData *, setting
                ) {
            ZFUIButtonBasic *button = zfargs.sender();

            setting->buttonClickListener().execute(ZFArgs()
                    .sender(button)
                );
            setting->settingUpdate();
        } ZFLISTENER_END()
        button->observerAdd(ZFUIButton::E_ButtonOnClick(), onButtonClick);

        ZFLISTENER_2(settingOnUpdate
                , ZFUIKit_test_SettingData *, setting
                , ZFUIButtonBasic *, button
                ) {
            zfobj<v_zfstring> buttonText;
            setting->buttonTextGetter().execute(ZFArgs()
                    .sender(button)
                    .param0(buttonText)
                );
            button->label()->text(buttonText->zfv);
        } ZFLISTENER_END()
        setting->observerAdd(ZFUIKit_test_SettingData::E_SettingOnUpdate(), settingOnUpdate);

        zfobj<v_zfstring> buttonText;
        setting->buttonTextGetter().execute(ZFArgs()
                .sender(button)
                .param0(buttonText)
            );
        button->label()->text(buttonText->zfv);
    }

    return settingsButton;
}

void ZFUIKit_test_prepareSettingButtonWithTestWindow(
        ZF_IN ZFUIWindow *window
        , ZF_IN ZFArray *settings
        ) {
    zfauto buttonHolder = ZFUIKit_test_prepareSettingButton(settings);
    ZFUIButton *button = buttonHolder;
    window->child(button)->c_alignTop();
}

void ZFUIKit_test_prepareSettingForProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFListener &nextCallback
        ) {
    ZFCoreAssert(settings != zfnull);
    ZFCoreAssert(nextCallback);

    ZFLISTENER_2(buttonTextGetter
            , ZFObject *, obj
            , const ZFProperty *, property
            ) {
        v_zfstring *text = zfargs.param0();
        text->zfv = zfstr("%s : %s", property->propertyName(), ZFObjectInfo(property->getterMethod()->methodInvoke(obj)));
    } ZFLISTENER_END()
    ZFLISTENER_1(buttonClickListener
            , ZFListener, nextCallback
            ) {
        nextCallback.execute();
    } ZFLISTENER_END()

    settings->add(zfobj<ZFUIKit_test_SettingData>(buttonTextGetter, buttonClickListener));
}

void ZFUIKit_test_prepareSettingForBoolProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        ) {
    ZFCoreAssert(settings != zfnull);
    ZFCoreAssert(obj != zfnull && property != zfnull);
    ZFCoreAssert(obj->classData()->classIsTypeOf(property->ownerClass()));
    ZFCoreAssert(zfstringIsEqual(property->propertyTypeId(), ZFTypeId_zfbool()));

    zfbool value = property->getterMethod()->executeExact<zfbool const &>(obj);
    ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, zfbool, property, ZFCoreArrayCreate(zfbool, value, !value));
}

void ZFUIKit_test_prepareSettingForLayoutRequest(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFUIView *view
        ) {
    ZFCoreAssert(settings != zfnull);
    ZFCoreAssert(view != zfnull);

    ZFLISTENER(buttonTextGetter) {
        v_zfstring *text = zfargs.param0();
        text->zfv = "layoutRequest";
    } ZFLISTENER_END()
    ZFLISTENER_1(buttonClickListener
            , ZFUIView *, view
            ) {
        view->layoutRequest();
    } ZFLISTENER_END()

    settings->add(zfobj<ZFUIKit_test_SettingData>(buttonTextGetter, buttonClickListener));
}

void ZFUIKit_test_prepareSettingForResetProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFCoreArray<const ZFProperty *> &propertyList
        ) {
    zfobj<ZFUIKit_test_SettingData> setting;
    settings->add(setting);

    ZFLISTENER(buttonTextGetter) {
        v_zfstring *text = zfargs.param0();
        text->zfv = "reset setting";
    } ZFLISTENER_END()
    setting->buttonTextGetter(buttonTextGetter);

    ZFLISTENER_3(buttonClickListener
            , ZFArray *, settings
            , ZFObject *, obj
            , ZFCoreArray<const ZFProperty *>, propertyList
            ) {
        const ZFCoreArray<const ZFProperty *> &toReset = propertyList;

        if(obj == zfnull || settings == zfnull || toReset.isEmpty()) {
            return;
        }

        zfauto fromObj = obj->classData()->newInstance();
        for(zfindex i = 0; i < toReset.count(); ++i) {
            ZFPropertyCopy(toReset[i], obj, fromObj);
        }
        for(zfindex i = 0; i < settings->count(); ++i) {
            ZFUIKit_test_SettingData *setting = settings->get(i);
            setting->settingUpdate();
        }
    } ZFLISTENER_END()
    setting->buttonClickListener(buttonClickListener);
}

ZF_NAMESPACE_GLOBAL_END

