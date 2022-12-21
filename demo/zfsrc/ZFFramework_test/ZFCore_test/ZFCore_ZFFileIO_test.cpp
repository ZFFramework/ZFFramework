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

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "res tree:";
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_res(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrimT() << "try read content:";
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForRes("test_ZFFileIO/fileExist"));
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForRes("test_ZFFileIO/dirExist/fileExist2"));

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "copy to cache dir, tree:";
        ZFResCopy("test_ZFFileIO", zfstringWithFormat("%s/test_ZFFileIO", ZFPathForCache()));
        ZFPathInfoTreePrint(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrimT() << "try read content:";
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist")));
        ZFInputReadAll(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2")));

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "file type check:";

        zfLogTrimT() << "  res isExist:";
        zfLogTrimT() << "    " << ZFResIsExist("test_ZFFileIO/fileExist");
        zfLogTrimT() << "    " << ZFResIsExist("test_ZFFileIO/dirExist");
        zfLogTrimT() << "    " << ZFResIsExist("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrimT() << "    " << !ZFResIsExist("test_ZFFileIO/fileNotExist");
        zfLogTrimT() << "    " << !ZFResIsExist("test_ZFFileIO/dirNotExist");
        zfLogTrimT() << "    " << !ZFResIsExist("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrimT() << "  res isDir:";
        zfLogTrimT() << "    " << !ZFResIsDir("test_ZFFileIO/fileExist");
        zfLogTrimT() << "    " << ZFResIsDir("test_ZFFileIO/dirExist");
        zfLogTrimT() << "    " << !ZFResIsDir("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrimT() << "    " << !ZFResIsDir("test_ZFFileIO/fileNotExist");
        zfLogTrimT() << "    " << !ZFResIsDir("test_ZFFileIO/dirNotExist");
        zfLogTrimT() << "    " << !ZFResIsDir("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrimT() << "  file isExist:";
        zfLogTrimT() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrimT() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrimT() << "    " << ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << "    " << !ZFPathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        zfLogTrimT() << "  file isDir:";
        zfLogTrimT() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrimT() << "    " << ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrimT() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << "    " << !ZFPathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

