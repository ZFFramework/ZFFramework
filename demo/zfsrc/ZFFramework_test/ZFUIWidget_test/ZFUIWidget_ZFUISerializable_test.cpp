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
            this->testCaseOutputSeparator();
            this->testCaseOutput("xml:");
            ZFObjectToXml(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("xml re-serialized:");
            ZFObjectToXml(ZFOutputDefault(), ZFObjectFromXml(ZFInputForBufferUnsafe(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput("json:");
            ZFObjectToJson(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("json re-serialized:");
            ZFObjectToJson(ZFOutputDefault(), ZFObjectFromJson(ZFInputForBufferUnsafe(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput("zfsd:");
            ZFObjectToZfsd(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("zfsd re-serialized:");
            ZFObjectToZfsd(ZFOutputDefault(), ZFObjectFromZfsd(ZFInputForBufferUnsafe(s)));
        }

        this->testCaseStop();
    }

private:
    zfauto prepareTestObject(void) {
        zfblockedAlloc(ZFUIAutoLayout, parent);
        parent->viewAlpha(0.5f);

        zfblockedAlloc(ZFUIView, child0);
        child0->viewUIEnableTree(zffalse);
        parent->childAdd(child0)->c_left()->c_toParent();

        zfblockedAlloc(ZFUITextView, child1);
        child1->text("special chars: \r\n\t\"'-_=+<>()[]{}");
        parent->childAdd(child1)->c_right()->c_toParentRight();

        return parent;
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUISerializable_test)

ZF_NAMESPACE_GLOBAL_END

