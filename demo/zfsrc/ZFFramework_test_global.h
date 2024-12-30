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
    virtual void testCaseOnStop(ZF_IN ZFResultType testCaseResult);

public:
    virtual void outputSeparator(void);
    virtual const zfstring &testCaseTmpPath(void);
    virtual zfstring testCaseUseTmpFile(ZF_IN const zfchar *fileName);
    virtual void testCaseAddFileToRemove(ZF_IN const zfchar *filePath);

private:
    ZFCoreArray<zfstring> _filesToRemove;
    zfstring _testCaseTmpPath;
};

ZFEXPORT_VAR_DECLARE(ZFLIB_APP, zfbool, ZFFramework_test_asyncTestAvailable)

#define ZFFramework_test_protocolCheck(ProtocolName) \
    do { \
        if(!ZFProtocolIsAvailable(#ProtocolName)) { \
            this->output(#ProtocolName " not available, skip test case"); \
            this->stop(); \
            return; \
        } \
    } while(zffalse)

#define ZFFramework_test_asyncTestCheck() \
    do { \
        if(!ZFThread::implMainThreadTaskAvailable() || !ZFFramework_test_asyncTestAvailable()) { \
            this->output(zfstr("%s not available, skip test case" \
                        , !ZFThread::implMainThreadTaskAvailable() \
                            ? "ZFThread::implMainThreadTaskAvailable()" \
                            : "ZFFramework_test_asyncTestAvailable()" \
                        )); \
            this->stop(); \
            return; \
        } \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFFramework_test_global_h_

