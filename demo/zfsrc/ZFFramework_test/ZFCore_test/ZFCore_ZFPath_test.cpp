#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFPath_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFPath_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFPath);

        this->testCaseOutput("ZFPathForModule:");
        this->testCaseOutput(ZFPathForModule());

        this->testCaseOutput("ZFPathForModuleFile:");
        this->testCaseOutput(ZFPathForModuleFile());

        this->testCaseOutput("ZFPathForSetting:");
        this->testCaseOutput(ZFPathForSetting());

        this->testCaseOutput("ZFPathForStorage:");
        this->testCaseOutput(ZFPathForStorage());

        this->testCaseOutput("ZFPathForStorageShared:");
        this->testCaseOutput(ZFPathForStorageShared());

        this->testCaseOutput("ZFPathForCache:");
        this->testCaseOutput(ZFPathForCache());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPath_test)

ZF_NAMESPACE_GLOBAL_END

