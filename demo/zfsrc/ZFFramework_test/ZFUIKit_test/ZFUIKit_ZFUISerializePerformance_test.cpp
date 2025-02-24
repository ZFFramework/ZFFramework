#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFUISerializePerformance_test_TestObject : zfextend ZFObject, zfimplement ZFSerializable {
    ZFOBJECT_DECLARE(_ZFP_ZFUISerializePerformance_test_TestObject, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable)

    ZFPROPERTY_RETAIN(zfanyT<_ZFP_ZFUISerializePerformance_test_TestObject>, vObj)

    ZFPROPERTY_ASSIGN(zfstring, v0)
    ZFPROPERTY_ASSIGN(zfstring, v1)
    ZFPROPERTY_ASSIGN(zfstring, v2)
    ZFPROPERTY_ASSIGN(zfstring, v3)
    ZFPROPERTY_ASSIGN(zfstring, v4)
    ZFPROPERTY_ASSIGN(zfstring, v5)
    ZFPROPERTY_ASSIGN(zfstring, v6)
    ZFPROPERTY_ASSIGN(zfstring, v7)
    ZFPROPERTY_ASSIGN(zfstring, v8)
    ZFPROPERTY_ASSIGN(zfstring, v9)
    ZFPROPERTY_ASSIGN(zfstring, v10)
    ZFPROPERTY_ASSIGN(zfstring, v11)
    ZFPROPERTY_ASSIGN(zfstring, v12)
    ZFPROPERTY_ASSIGN(zfstring, v13)
    ZFPROPERTY_ASSIGN(zfstring, v14)
    ZFPROPERTY_ASSIGN(zfstring, v15)

public:
    void modifyProperty(void) {
        for(zfindex i = 0; i < 16; ++i) {
            zfstring v = zfstr("v%s", i);
            zfself::ClassData()->propertyForName(v)
                ->setterMethod()->executeExact<void, zfstring const &>(this, v);
        }
        this->vObj(zfobj<_ZFP_ZFUISerializePerformance_test_TestObject>());
        this->vObj()->v0("dummy");
    }
};

zfclass ZFUIKit_ZFUISerializePerformance_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUISerializePerformance_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUITextView);
        ZFFramework_test_protocolCheck(ZFUIImage);
        ZFFramework_test_protocolCheck(ZFUIImageView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIView> outputViewContainer;
        container->child(outputViewContainer)->c_sizeFill()->c_margin(0, 50, 0, 0);
        outputViewContainer->bgColor(ZFUIColorBlack());

        zfobj<ZFUITextView> outputView;
        outputViewContainer->child(outputView)->c_widthFill();
        outputView->singleLine(zffalse);
        outputView->textColor(ZFUIColorWhite());
        outputView->text("press start");

        zfobj<ZFUIKit_test_Button> startButton;
        container->child(startButton)->c_alignTop();
        startButton->label()->text("start");

        zfself *owner = this;
        ZFLISTENER_2(onStart
                , zfself *, owner
                , zfautoT<ZFUITextView>, outputView
                ) {
            zfauto testObject = owner->prepareTestObject();
            outputView->text("running...");
            ZFSerializableData data;
            ZFObjectToDataT(data, testObject);

            zfindex toDataTimes = 1000;
            ZFCoreStatistic::invokeTimeLogBegin("ZFUISerializePerformance_test_toData");
            for(zfindex i = 0; i < toDataTimes; ++i) {
                ZFObjectToData(testObject);
            }
            ZFCoreStatistic::invokeTimeLogEnd("ZFUISerializePerformance_test_toData");

            zfindex fromDataTimes = toDataTimes;
            ZFCoreStatistic::invokeTimeLogBegin("ZFUISerializePerformance_test_fromData");
            for(zfindex i = 0; i < fromDataTimes; ++i) {
                ZFObjectFromData(data);
            }
            ZFCoreStatistic::invokeTimeLogEnd("ZFUISerializePerformance_test_fromData");

            zfstring result;
            ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFUISerializePerformance_test_toData");
            zfstringAppend(result, "serialize %s object to data cost %s seconds",
                toDataTimes,
                ZFTimeValueToStringFriendly(toDataUsedTime));
            result += "\n";
            ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFUISerializePerformance_test_fromData");
            zfstringAppend(result, "serialize %s object from data cost %s seconds",
                fromDataTimes,
                ZFTimeValueToStringFriendly(fromDataUsedTime));
            result += "\ndata:\n";
            ZFSerializableDataToXml(ZFOutputForString(result), data);
            outputView->text(result);

            ZFCoreStatistic::invokeTimeRemove("ZFUISerializePerformance_test_toData");
            ZFCoreStatistic::invokeTimeRemove("ZFUISerializePerformance_test_fromData");
        } ZFLISTENER_END()
        startButton->observerAdd(ZFUIButton::E_ButtonOnClick(), onStart);

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window) {
        zfobj<ZFArray> settings;

        { // auto scroll x
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0();
                text->zfv = "change test object";
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            zfself *owner = this;
            ZFLISTENER_1(buttonClickListener
                    , zfself *, owner
                    ) {
                owner->testObjectType = ((owner->testObjectType + 1) % owner->testObjectTypeCount);
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->testObjectType = 0;
        this->testObjectTypeCount = 3;
    }

public:
    zfindex testObjectType;
    zfindex testObjectTypeCount;
    zfauto prepareTestObject(void) {
        switch(this->testObjectType) {
            case 0: {
                zfobj<ZFUIKit_test_Button> v;
                v->bg()->bgColor(ZFUIColorRandom());
                return v;
            }
            case 1: {
                zfobj<ZFUIImageView> v;
                v->image(ZFUIImageFromColor(ZFUIColorRed()));
                return v;
            }
            case 2:
            default: {
                zfobj<_ZFP_ZFUISerializePerformance_test_TestObject> v;
                v->modifyProperty();
                return v;
            }
        }
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUISerializePerformance_test)

ZF_NAMESPACE_GLOBAL_END

