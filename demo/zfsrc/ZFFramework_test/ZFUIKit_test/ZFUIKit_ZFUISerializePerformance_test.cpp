#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFUISerializePerformance_test_TestObject : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFUISerializePerformance_test_TestObject, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

    ZFPROPERTY_RETAIN(_ZFP_ZFUISerializePerformance_test_TestObject *, vObj)

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
    void modifyProperty(void)
    {
        for(zfindex i = 0; i < 16; ++i)
        {
            zfstring v = zfstringWithFormat("v%zi", i);
            zfself::ClassData()->propertyForName(v)
                ->setterMethod()->execute<void, zfstring const &>(this, v);
        }
        this->vObj(zflineAlloc(_ZFP_ZFUISerializePerformance_test_TestObject));
        this->vObj()->v0("dummy");
    }
};

zfclass ZFUIKit_ZFUISerializePerformance_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUISerializePerformance_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUITextView);
        ZFFramework_test_protocolCheck(ZFUIImage);
        ZFFramework_test_protocolCheck(ZFUIImageView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, outputViewContainer);
        container->childAdd(outputViewContainer)->c_sizeFill()->c_margin(0, 50, 0, 0);
        outputViewContainer->viewBackgroundColor(ZFUIColorBlack());

        zfblockedAlloc(ZFUITextView, outputView);
        outputViewContainer->childAdd(outputView)->c_widthFill();
        outputView->textSingleLine(zffalse);
        outputView->textColor(ZFUIColorWhite());
        outputView->text("press start");

        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton)->c_alignTop();
        startButton->label()->text("start");

        zfself *owner = this;
        ZFLISTENER_2(onStart
                , zfself *, owner
                , zfautoObjectT<ZFUITextView *>, outputView
                ) {
            zfautoObject testObject = owner->prepareTestObject();
            outputView->text("running...");
            ZFSerializableData data = ZFObjectToData(testObject);

            zfindex toDataTimes = 1000;
            ZFCoreStatistic::invokeTimeLogBegin("ZFUISerializePerformance_test_toData");
            for(zfindex i = 0; i < toDataTimes; ++i)
            {
                ZFObjectToData(testObject);
            }
            ZFCoreStatistic::invokeTimeLogEnd("ZFUISerializePerformance_test_toData");

            zfindex fromDataTimes = toDataTimes;
            ZFCoreStatistic::invokeTimeLogBegin("ZFUISerializePerformance_test_fromData");
            for(zfindex i = 0; i < fromDataTimes; ++i)
            {
                ZFObjectFromData(data);
            }
            ZFCoreStatistic::invokeTimeLogEnd("ZFUISerializePerformance_test_fromData");

            zfstring result;
            ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFUISerializePerformance_test_toData");
            zfstringAppend(result, "serialize %zi object to data cost %s seconds",
                toDataTimes,
                ZFTimeValueToStringFriendly(toDataUsedTime).cString());
            result += "\n";
            ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFUISerializePerformance_test_fromData");
            zfstringAppend(result, "serialize %zi object from data cost %s seconds",
                fromDataTimes,
                ZFTimeValueToStringFriendly(fromDataUsedTime).cString());
            result += "\ndata:\n";
            ZFSerializableDataToXml(ZFOutputForString(result), data);
            outputView->text(result);

            ZFCoreStatistic::invokeTimeRemove("ZFUISerializePerformance_test_toData");
            ZFCoreStatistic::invokeTimeRemove("ZFUISerializePerformance_test_fromData");
        } ZFLISTENER_END(onStart)
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), onStart);

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArray, settings);

        { // auto scroll x
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);

            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0T();
                text->zfv = "change test object";
            } ZFLISTENER_END(buttonTextGetter)
            setting->buttonTextGetter(buttonTextGetter);

            zfself *owner = this;
            ZFLISTENER_1(buttonClickListener
                    , zfself *, owner
                    ) {
                owner->testObjectType = ((owner->testObjectType + 1) % owner->testObjectTypeCount);
            } ZFLISTENER_END(buttonClickListener)
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->testObjectType = 0;
        this->testObjectTypeCount = 3;
    }

public:
    zfindex testObjectType;
    zfindex testObjectTypeCount;
    zfautoObject prepareTestObject(void)
    {
        switch(this->testObjectType)
        {
            case 0:
            {
                zfblockedAlloc(ZFUIKit_test_Button, v);
                v->background()->viewBackgroundColor(ZFUIColorRandom());
                return v;
            }
            case 1:
            {
                zfblockedAlloc(ZFUIImageView, v);
                v->image(ZFUIImageLoadFromColor(ZFUIColorRed()));
                return v;
            }
            case 2:
            default:
            {
                zfblockedAlloc(_ZFP_ZFUISerializePerformance_test_TestObject, v);
                v->modifyProperty();
                return v;
            }
        }
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUISerializePerformance_test)

ZF_NAMESPACE_GLOBAL_END

