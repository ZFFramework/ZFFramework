#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIAutoLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIAutoLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIAutoLayout, layout);
        container->childAdd(layout)->c_sizeFill()->c_margin(40);
        layout->viewBackgroundColor(ZFUIColorRed());

        ZFCoreArray<zfautoObject> v;
        for(zfindex i = 0; i < 5; ++i)
        {
            zfblockedAlloc(ZFUITextView, t);
            v.add(t);
            t->text(zfstringWithFormat("text %zi", i));
            t->viewBackgroundColor(ZFUIColorRandom());
        }

        layout->childAdd(v[0])->c_left()->c_toParentLeft()->c_right()->c_toLeft(v[1]);
        layout->childAdd(v[1])->c_left()->c_toRight(v[0])->c_right()->c_toLeft(v[2]);
        layout->childAdd(v[2])->c_left()->c_toRight(v[1])->c_right()->c_toParentRight();

        layout->childAdd(v[3])->c_right()->c_toLeft(v[1])->c_bottom()->c_toTop(v[1])->c_height()->c_toWidth(v[1])->c_weight(2);
        layout->childAdd(v[4])->c_left()->c_toRight(v[1])->c_top()->c_toBottom(v[1])->c_height()->c_toWidth(v[1])->c_weight(3);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIAutoLayout *layout)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->layoutParam(), ZFUISizeParam, ZFPropertyAccess(ZFUILayoutParam, sizeParam),
            ZFCoreArrayPODCreate(ZFUISizeParam
                , ZFUISizeParamWrapWrap()
                , ZFUISizeParamFillWrap()
                , ZFUISizeParamWrapFill()
                , ZFUISizeParamFillFill()
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->childAt(0)->layoutParam(), zffloat, ZFPropertyAccess(ZFUIAutoLayoutParam, biasX),
            ZFCoreArrayPODCreate(zffloat
                , 0.5f
                , 0.1f
                , 0.9f
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIAutoLayout_test)

ZF_NAMESPACE_GLOBAL_END

