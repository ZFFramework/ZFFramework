#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFEnvInfo_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFEnvInfo_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->output("current env info:");
        this->output(ZFEnvInfo::envSummary());
        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFEnvInfo_test)

ZF_NAMESPACE_GLOBAL_END

