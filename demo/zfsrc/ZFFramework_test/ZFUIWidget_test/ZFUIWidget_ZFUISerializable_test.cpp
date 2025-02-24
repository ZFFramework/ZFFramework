#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUISerializable_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUISerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);

        zfauto testObject = this->prepareTestObject();

        {
            zfstring s;
            this->outputSeparator();
            this->output("xml:");
            ZFObjectToXml(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->output("xml re-serialized:");
            ZFObjectToXml(ZFOutputDefault(), ZFObjectFromXml(ZFInputForString(s)));
        }

        {
            zfstring s;
            this->outputSeparator();
            this->output("json:");
            ZFObjectToJson(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->output("json re-serialized:");
            ZFObjectToJson(ZFOutputDefault(), ZFObjectFromJson(ZFInputForString(s)));
        }

        {
            zfstring s;
            this->outputSeparator();
            this->output("zfsd:");
            ZFObjectToZFSD(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->output("zfsd re-serialized:");
            ZFObjectToZFSD(ZFOutputDefault(), ZFObjectFromZFSD(ZFInputForString(s)));
        }

        this->stop();
    }

private:
    zfauto prepareTestObject(void) {
        zfobj<ZFUIAutoLayout> parent;
        parent->alpha(0.5f);

        zfobj<ZFUIView> child0;
        child0->viewUIEnableTree(zffalse);
        parent->child(child0)->c_left()->c_toParent();

        zfobj<ZFUITextView> child1;
        child1->text("special chars: \r\n\t\"'-_=+<>()[]{}");
        parent->child(child1)->c_right()->c_toParentRight();

        return parent;
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUISerializable_test)

ZF_NAMESPACE_GLOBAL_END

