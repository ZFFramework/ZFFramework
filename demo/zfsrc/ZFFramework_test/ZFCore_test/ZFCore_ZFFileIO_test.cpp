#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFileIO_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFFileIO_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFFile);
        ZFFramework_test_protocolCheck(ZFRes);

        const zfchar *testPath = "test_ZFFileIO";

        zfLogTrim() << "============================================================";
        zfLogTrim() << "res tree:";
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_res(), testPath), ZFOutputDefault(), "  ");

        zfLogTrim() << "try read content:";
        ZFInputRead(ZFOutputDefault(),
            ZFInputForRes(zfstr("%s/fileExist", testPath)));
        ZFInputRead(ZFOutputDefault(),
            ZFInputForRes(zfstr("%s/dirExist/fileExist2", testPath)));

        zfLogTrim() << "============================================================";
        zfLogTrim() << "copy to cache dir, tree:";
        ZFResCopy(testPath, zfstr("%s/%s", ZFPathForCache(), testPath));
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_cachePath(), testPath), ZFOutputDefault(), "  ");

        zfLogTrim() << "try read content:";
        ZFInputRead(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath))));
        ZFInputRead(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath))));

        zfLogTrim() << "============================================================";
        zfLogTrim() << "file type check:";

        zfLogTrim() << "  res isExist:";
        zfLogTrim() << "    " << ZFResIsExist(zfstr("%s/fileExist", testPath));
        zfLogTrim() << "    " << ZFResIsExist(zfstr("%s/dirExist", testPath));
        zfLogTrim() << "    " << ZFResIsExist(zfstr("%s/dirExist/fileExist2", testPath));
        zfLogTrim() << "    " << !ZFResIsExist(zfstr("%s/fileNotExist", testPath));
        zfLogTrim() << "    " << !ZFResIsExist(zfstr("%s/dirNotExist", testPath));
        zfLogTrim() << "    " << !ZFResIsExist(zfstr("%s/dirExist/fileNotExist", testPath));

        zfLogTrim() << "  res isDir:";
        zfLogTrim() << "    " << !ZFResIsDir(zfstr("%s/fileExist", testPath));
        zfLogTrim() << "    " << ZFResIsDir(zfstr("%s/dirExist", testPath));
        zfLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirExist/fileExist2", testPath));
        zfLogTrim() << "    " << !ZFResIsDir(zfstr("%s/fileNotExist", testPath));
        zfLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirNotExist", testPath));
        zfLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirExist/fileNotExist", testPath));

        zfLogTrim() << "  file isExist:";
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath)));
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist", testPath)));
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileNotExist", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirNotExist", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileNotExist", testPath)));

        zfLogTrim() << "  file isDir:";
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath)));
        zfLogTrim() << "    " << ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileNotExist", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirNotExist", testPath)));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileNotExist", testPath)));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

