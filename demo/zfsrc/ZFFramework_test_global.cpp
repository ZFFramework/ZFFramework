#include "ZFFramework_test_global.h"

#include "ZFCore/ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFFramework_test_TestCase)

void ZFFramework_test_TestCase::objectOnInit(void) {
    zfsuper::objectOnInit();
    this->_testCaseTmpPath = zfstr("%s/ZFFramework_test/%s",
        ZFPathForCache(),
        this->classData()->classNameFull());
}
void ZFFramework_test_TestCase::objectOnDealloc(void) {
    zfsuper::objectOnDealloc();
}
void ZFFramework_test_TestCase::objectOnDeallocPrepare(void) {
    ZFFileRemove(this->testCaseTmpPath());
    zfsuper::objectOnDeallocPrepare();
}

void ZFFramework_test_TestCase::testCaseOnStop(ZF_IN ZFResultType testCaseResult) {
    for(zfindex i = 0; i < this->_filesToRemove.count(); ++i) {
        ZFFileRemove(this->_filesToRemove[i]);
    }
    zfsuper::testCaseOnStop(testCaseResult);
}

void ZFFramework_test_TestCase::outputSeparator(void) {
    this->output("----------------------------------------");
}
const zfstring &ZFFramework_test_TestCase::testCaseTmpPath(void) {
    return this->_testCaseTmpPath;
}
zfstring ZFFramework_test_TestCase::testCaseUseTmpFile(ZF_IN const zfchar *fileName) {
    zfstring ret = this->testCaseTmpPath();
    ret += '/';
    ret += fileName;
    this->testCaseAddFileToRemove(ret);
    return ret;
}
void ZFFramework_test_TestCase::testCaseAddFileToRemove(ZF_IN const zfchar *filePath) {
    this->_filesToRemove.add(filePath);
}

// ============================================================
ZFEXPORT_VAR_DEFINE(zfbool, ZFFramework_test_asyncTestAvailable, zftrue)

ZF_NAMESPACE_GLOBAL_END

