#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFileIO_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFFileIO_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFFile);
        ZFFramework_test_protocolCheck(ZFRes);

        const zfchar *testPath = "test_ZFFileIO";

        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "res tree:";
        ZFIOTreePrint(ZFPathInfo(ZFPathType_res(), testPath), ZFOutputDefault(), "  ");

        ZFLogTrim() << "try read content:";
        ZFInputRead(ZFOutputDefault(),
            ZFInputForRes(zfstr("%s/fileExist", testPath)));
        ZFInputRead(ZFOutputDefault(),
            ZFInputForRes(zfstr("%s/dirExist/fileExist2", testPath)));

        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "copy to cache dir, tree:";
        ZFIOCopy(ZFPathInfo(ZFPathType_res(), testPath), ZFPathInfo(ZFPathType_cachePath(), testPath));
        ZFIOTreePrint(ZFPathInfo(ZFPathType_cachePath(), testPath), ZFOutputDefault(), "  ");

        ZFLogTrim() << "try read content:";
        ZFInputRead(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath))));
        ZFInputRead(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath))));

        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "file type check:";

        ZFLogTrim() << "  res isExist:";
        ZFLogTrim() << "    " << ZFResIsExist(zfstr("%s/fileExist", testPath));
        ZFLogTrim() << "    " << ZFResIsExist(zfstr("%s/dirExist", testPath));
        ZFLogTrim() << "    " << ZFResIsExist(zfstr("%s/dirExist/fileExist2", testPath));
        ZFLogTrim() << "    " << !ZFResIsExist(zfstr("%s/fileNotExist", testPath));
        ZFLogTrim() << "    " << !ZFResIsExist(zfstr("%s/dirNotExist", testPath));
        ZFLogTrim() << "    " << !ZFResIsExist(zfstr("%s/dirExist/fileNotExist", testPath));

        ZFLogTrim() << "  res isDir:";
        ZFLogTrim() << "    " << !ZFResIsDir(zfstr("%s/fileExist", testPath));
        ZFLogTrim() << "    " << ZFResIsDir(zfstr("%s/dirExist", testPath));
        ZFLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirExist/fileExist2", testPath));
        ZFLogTrim() << "    " << !ZFResIsDir(zfstr("%s/fileNotExist", testPath));
        ZFLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirNotExist", testPath));
        ZFLogTrim() << "    " << !ZFResIsDir(zfstr("%s/dirExist/fileNotExist", testPath));

        ZFLogTrim() << "  file isExist:";
        ZFLogTrim() << "    " << ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath)));
        ZFLogTrim() << "    " << ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist", testPath)));
        ZFLogTrim() << "    " << ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileNotExist", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirNotExist", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsExist(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileNotExist", testPath)));

        ZFLogTrim() << "  file isDir:";
        ZFLogTrim() << "    " << !ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileExist", testPath)));
        ZFLogTrim() << "    " << ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileExist2", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/fileNotExist", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirNotExist", testPath)));
        ZFLogTrim() << "    " << !ZFIOIsDir(ZFPathInfo(ZFPathType_cachePath(), zfstr("%s/dirExist/fileNotExist", testPath)));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

