#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFCompress_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCompress_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFCompress);

        this->outputSeparator();
        this->output("compress buffer");
        {
            zfstring buf;
            ZFInputRead(ZFOutputForCompress(ZFOutputForString(buf)), ZFInputForString("uncompressed text"));
            ZFInputRead(ZFOutputDefault(), ZFInputForCompress(ZFInputForString(buf)));
            ZFOutputDefault() << "\n";
        }

        this->outputSeparator();
        this->output("compress tree");
        {
            ZFPathInfo pathInfoSrc(ZFPathType_res(), ".");
            ZFPathInfo pathInfoDst(ZFPathType_cachePath(), "ZFCompress_test");
            ZFPathInfo pathInfoZip(ZFPathType_cachePath(), "ZFCompress_test.zip");

            this->output("original src tree:");
            ZFIOTreePrint(pathInfoSrc, ZFOutputDefault(), "    ");
            this->output("original dst tree:");
            ZFIOTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            ZFCompressDir(pathInfoZip, pathInfoSrc, "");

            ZFDecompressDir(pathInfoDst, pathInfoDst);
            this->output("decompressed dst tree:");
            ZFIOTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            ZFIORemove(pathInfoDst);
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCompress_test)

ZF_NAMESPACE_GLOBAL_END

