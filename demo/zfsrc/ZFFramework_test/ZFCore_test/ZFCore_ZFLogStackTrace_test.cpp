#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFLogStackTrace_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFLogStackTrace_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutput(zfstr("stack trace:\n%s", ZFLogStackTrace()));

        this->testCaseOutput(zfstr("caller info: %s", ZFLogCallerInfo()));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLogStackTrace_test)

ZF_NAMESPACE_GLOBAL_END

