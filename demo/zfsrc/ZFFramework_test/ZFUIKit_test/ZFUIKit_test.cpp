#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKit_test_Window)
ZFOBJECT_REGISTER(ZFUIKit_test_Button)
ZFOBJECT_REGISTER(ZFUIKit_test_ListView)

void ZFUIKit_test_prepareTestWindow(
        ZF_OUT ZFUIWindow *&window
        , ZF_OUT ZFUIView *&container
        , ZF_IN ZFTestCase *testCaseToStop
        ) {
    window = zfAlloc(ZFUIKit_test_Window);
    zfblockedRelease(window);
    window->windowShow();

    // close button
    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton)->c_alignRightTop();
    closeButton->label()->text("close");
    ZFLISTENER_2(onClickCloseButton
            , ZFTestCase *, testCaseToStop
            , ZFUIWindow *, window
            ) {
        testCaseToStop->testCaseStop();
        window->windowHide();
    } ZFLISTENER_END()
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton);

    // container
    container = zfAlloc(ZFUIView);
    zfblockedRelease(container);
    window->childAdd(container)->c_sizeFill()->c_margin(0, 50, 0, 0);
}

zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings) {
    zfblockedAlloc(ZFUIKit_test_Button, settingsButton);
    settingsButton->label()->text("settings");
    settingsButton->objectTag("settingsHolder", settings);

    zfblockedAlloc(ZFUIKit_test_Window, window);
    settingsButton->objectTag("setting window", window);
    window->viewAlpha(0.8f);
    ZFLISTENER_1(onClickSetting
            , ZFUIWindow *, window
            ) {
        window->windowShow();
    } ZFLISTENER_END()
    settingsButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickSetting);

    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton)->c_alignTop();
    closeButton->label()->text("done");
    ZFLISTENER_1(onClickCloseButton
            , ZFUIWindow *, window
            ) {
        window->windowHide();
    } ZFLISTENER_END()
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton);

    zfblockedAlloc(ZFUIKit_test_ListView, listView);
    window->childAdd(listView)->c_sizeFill()->c_margin(0, 50, 0, 0);
    for(zfindex i = 0; i < settings->count(); ++i) {
        ZFUIKit_test_SettingData *setting = settings->get<ZFUIKit_test_SettingData *>(i);
        zfCoreAssert(setting->buttonTextGetter());
        zfCoreAssert(setting->buttonClickListener());

        zfblockedAlloc(ZFUIKit_test_Button, button);
        listView->childAdd(button);
        ZFLISTENER_1(onButtonClick
                , ZFUIKit_test_SettingData *, setting
                ) {
            ZFUIButtonBasic *button = ZFAny(zfargs.sender());

            setting->buttonClickListener().execute(ZFArgs()
                    .sender(button)
                    .userData(setting->buttonClickListener().userData())
                );
            setting->settingUpdate();
        } ZFLISTENER_END()
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onButtonClick);

        ZFLISTENER_2(settingOnChange
                , ZFUIKit_test_SettingData *, setting
                , ZFUIButtonBasic *, button
                ) {
            zfblockedAlloc(v_zfstring, buttonText);
            setting->buttonTextGetter().execute(ZFArgs()
                    .sender(button)
                    .param0(buttonText)
                    .userData(setting->buttonTextGetter().userData())
                );
            button->label()->text(buttonText->zfv);
        } ZFLISTENER_END()
        setting->observerAdd(ZFUIKit_test_SettingData::EventSettingOnChange(), settingOnChange);

        zfblockedAlloc(v_zfstring, buttonText);
        setting->buttonTextGetter().execute(ZFArgs()
                .sender(button)
                .param0(buttonText)
                .userData(setting->buttonTextGetter().userData())
            );
        button->label()->text(buttonText->zfv);
    }

    return settingsButton;
}

void ZFUIKit_test_prepareSettingButtonWithTestWindow(
        ZF_IN ZFUIWindow *window
        , ZF_IN ZFArray *settings
        ) {
    zfautoObject buttonHolder = ZFUIKit_test_prepareSettingButton(settings);
    ZFUIButton *button = buttonHolder;
    window->childAdd(button)->c_alignTop();
}

void ZFUIKit_test_prepareSettingForProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFListener &nextCallback
        ) {
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(nextCallback);

    ZFLISTENER_2(buttonTextGetter
            , ZFObject *, obj
            , const ZFProperty *, property
            ) {
        v_zfstring *text = zfargs.param0T();
        text->zfv = zfstringWithFormat("%s : %s", property->propertyName(), ZFPropertyGetInfo(property, obj).cString());
    } ZFLISTENER_END()
    ZFLISTENER_1(buttonClickListener
            , ZFListener, nextCallback
            ) {
        nextCallback.execute();
    } ZFLISTENER_END()

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener));
}

void ZFUIKit_test_prepareSettingForBoolProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        ) {
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(obj != zfnull && property != zfnull);
    zfCoreAssert(obj->classData()->classIsTypeOf(property->propertyOwnerClass()));
    zfCoreAssert(zfstringIsEqual(property->propertyTypeId(), ZFTypeId_zfbool()));

    zfbool value = property->getterMethod()->execute<zfbool const &>(obj);
    ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, zfbool, property, ZFCoreArrayPODCreate(zfbool, value, !value));
}

void ZFUIKit_test_prepareSettingForLayoutRequest(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFUIView *view
        ) {
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(view != zfnull);

    ZFLISTENER(buttonTextGetter) {
        v_zfstring *text = zfargs.param0T();
        text->zfv = "layoutRequest";
    } ZFLISTENER_END()
    ZFLISTENER_1(buttonClickListener
            , ZFUIView *, view
            ) {
        view->layoutRequest();
    } ZFLISTENER_END()

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener));
}

void ZFUIKit_test_prepareSettingForResetProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFCoreArrayPOD<const ZFProperty *> &propertyList
        ) {
    zfblockedAlloc(ZFUIKit_test_SettingData, setting);
    settings->add(setting);

    ZFLISTENER(buttonTextGetter) {
        v_zfstring *text = zfargs.param0T();
        text->zfv = "reset setting";
    } ZFLISTENER_END()
    setting->buttonTextGetter(buttonTextGetter);

    ZFLISTENER_3(buttonClickListener
            , ZFArray *, settings
            , ZFObject *, obj
            , ZFCoreArrayPOD<const ZFProperty *>, propertyList
            ) {
        const ZFCoreArrayPOD<const ZFProperty *> &toReset = propertyList;

        if(obj == zfnull || settings == zfnull || toReset.isEmpty()) {
            return;
        }

        zfautoObject fromObj = obj->classData()->newInstance();
        for(zfindex i = 0; i < toReset.count(); ++i) {
            ZFPropertyCopy(toReset[i], obj, fromObj);
        }
        for(zfindex i = 0; i < settings->count(); ++i) {
            settings->get<ZFUIKit_test_SettingData *>(i)->settingUpdate();
        }
    } ZFLISTENER_END()
    setting->buttonClickListener(buttonClickListener);
}

ZF_NAMESPACE_GLOBAL_END

