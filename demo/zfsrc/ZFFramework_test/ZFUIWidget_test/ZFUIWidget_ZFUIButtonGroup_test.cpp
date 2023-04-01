#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIButtonGroup_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIButtonGroup_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIButtonGroupBasic, layout);
        container->childAdd(layout)->c_margin(40);
        layout->buttonGroupType(ZFUIButtonGroupType::e_Tab);
        layout->layoutOrientation(ZFUIOrientation::e_Top);

        this->prepareChildren(layout);

        this->prepareSettingButton(window, layout);

        ZFLISTENER(buttonGroupAction) {
            zfLogTrimT()
                << zfargs.sender()
                << ZFIdMapNameForId(zfargs.eventId())
                << ", checked:" << zfargs.param1();
        } ZFLISTENER_END()
        layout->observerAdd(ZFUIButtonGroup::EventButtonTabOnClickChecked(), buttonGroupAction);
        layout->observerAdd(ZFUIButtonGroup::EventButtonTabOnChange(), buttonGroupAction);
    }

private:
    void prepareChildren(ZF_IN ZFUIButtonGroup *layout)
    {
        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(ZFUIButtonRatio, child);
            layout->buttonAdd(child);
            child->label()->text(zfstringWithFormat("btn %zi", i));
        }
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIButtonGroup *layout)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout->toObject(), ZFPropertyAccess(ZFUIButtonGroup, buttonTabAllowUnchecked));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIButtonGroup_test)

ZF_NAMESPACE_GLOBAL_END

