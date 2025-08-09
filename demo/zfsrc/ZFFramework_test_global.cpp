#include "ZFFramework_test_global.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFFramework_test_TestCase)

void ZFFramework_test_TestCase::objectOnDeallocPrepare(void) {
    for(zfindex i = 0; i < this->_filesToRemove.count(); ++i) {
        ZFIORemove(this->_filesToRemove[i]);
    }
    this->_filesToRemove.removeAll();
    zfsuper::objectOnDeallocPrepare();
}

void ZFFramework_test_TestCase::testCaseOnStop(ZF_IN ZFResultType testCaseResult) {
    for(zfindex i = 0; i < this->_filesToRemove.count(); ++i) {
        ZFIORemove(this->_filesToRemove[i]);
    }
    this->_filesToRemove.removeAll();
    zfsuper::testCaseOnStop(testCaseResult);
}

void ZFFramework_test_TestCase::outputSeparator(void) {
    this->output("----------------------------------------");
}
ZFPathInfo ZFFramework_test_TestCase::testCaseUseTmpFile(ZF_IN const zfchar *fileName) {
    ZFPathInfo ret(ZFPathType_cachePath(), zfstr("ZFFramework_test/%s", fileName));
    this->_filesToRemove.add(ret);
    return ret;
}

// ============================================================
ZFEXPORT_VAR_DEFINE(zfbool, ZFFramework_test_asyncTestAvailable, zftrue)

ZF_NAMESPACE_GLOBAL_END

