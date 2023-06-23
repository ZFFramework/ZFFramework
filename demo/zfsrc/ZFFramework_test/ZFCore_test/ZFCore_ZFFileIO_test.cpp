#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFileIO_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFileIO_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFFile);
        ZFFramework_test_protocolCheck(ZFRes);

        zfLogTrim() << "============================================================";
        zfLogTrim() << "res tree:";
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_res(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrim() << "try read content:";
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForRes("test_ZFFileIO/fileExist"));
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForRes("test_ZFFileIO/dirExist/fileExist2"));

        zfLogTrim() << "============================================================";
        zfLogTrim() << "copy to cache dir, tree:";
        ZFResCopy("test_ZFFileIO", zfstringWithFormat("%s/test_ZFFileIO", ZFPathForCache()));
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrim() << "try read content:";
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist")));
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2")));

        zfLogTrim() << "============================================================";
        zfLogTrim() << "file type check:";

        zfLogTrim() << "  res isExist:";
        zfLogTrim() << "    " << ZFResIsExist("test_ZFFileIO/fileExist");
        zfLogTrim() << "    " << ZFResIsExist("test_ZFFileIO/dirExist");
        zfLogTrim() << "    " << ZFResIsExist("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrim() << "    " << !ZFResIsExist("test_ZFFileIO/fileNotExist");
        zfLogTrim() << "    " << !ZFResIsExist("test_ZFFileIO/dirNotExist");
        zfLogTrim() << "    " << !ZFResIsExist("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrim() << "  res isDir:";
        zfLogTrim() << "    " << !ZFResIsDir("test_ZFFileIO/fileExist");
        zfLogTrim() << "    " << ZFResIsDir("test_ZFFileIO/dirExist");
        zfLogTrim() << "    " << !ZFResIsDir("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrim() << "    " << !ZFResIsDir("test_ZFFileIO/fileNotExist");
        zfLogTrim() << "    " << !ZFResIsDir("test_ZFFileIO/dirNotExist");
        zfLogTrim() << "    " << !ZFResIsDir("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrim() << "  file isExist:";
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrim() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrim() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        zfLogTrim() << "  file isDir:";
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrim() << "    " << ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrim() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

