#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFPath_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFPath_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFPath);

        this->output("ZFPathForModule:");
        this->output(ZFPathForModule());

        this->output("ZFPathForModuleFile:");
        this->output(ZFPathForModuleFile());

        this->output("ZFPathForSetting:");
        this->output(ZFPathForSetting());

        this->output("ZFPathForStorage:");
        this->output(ZFPathForStorage());

        this->output("ZFPathForStorageShared:");
        this->output(ZFPathForStorageShared());

        this->output("ZFPathForCache:");
        this->output(ZFPathForCache());

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPath_test)

ZF_NAMESPACE_GLOBAL_END

