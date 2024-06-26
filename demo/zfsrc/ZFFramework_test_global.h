#ifndef _ZFI_ZFFramework_test_global_h_
#define _ZFI_ZFFramework_test_global_h_

#include "ZFUtility/ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfabstract ZFFramework_test_TestCase : zfextend ZFTestCase {
    ZFOBJECT_DECLARE_ABSTRACT(ZFFramework_test_TestCase, ZFTestCase)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult);

public:
    virtual void testCaseOutputSeparator(void);
    virtual const zfchar *testCaseTmpPath(void);
    virtual zfstring testCaseUseTmpFile(ZF_IN const zfchar *fileName);
    virtual void testCaseAddFileToRemove(ZF_IN const zfchar *filePath);

private:
    ZFCoreArray<zfstring> _filesToRemove;
    zfstring _testCaseTmpPath;
};

#define ZFFramework_test_protocolCheck(ProtocolName) \
    do { \
        if(!ZFProtocolIsAvailable(#ProtocolName)) { \
            this->testCaseOutput(#ProtocolName " not available, skip test case"); \
            this->testCaseStop(); \
            return; \
        } \
    } while(zffalse)

#define ZFFramework_test_asyncTestCheck() \
    do { \
        if(!ZFThread::implMainThreadTaskAvailable()) { \
            this->testCaseOutput("ZFThread::implMainThreadTaskAvailable() not available, skip test case"); \
            this->testCaseStop(); \
            return; \
        } \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFFramework_test_global_h_

