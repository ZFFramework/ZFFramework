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
            zfobj<ZFIOBufferByMemory> io;
            ZFCompressContent(io->output(), ZFInputForBufferUnsafe("uncompressed text"));
            ZFDecompressContent(ZFOutputDefault(), io->input());
            ZFOutputDefault() << "\n";
        }

        this->outputSeparator();
        this->output("compress tree");
        {
            ZFPathInfo pathInfoSrc(ZFPathType_res(), ".");
            ZFPathInfo pathInfoDst(ZFPathType_cachePath(), "ZFCompress_test");

            this->output("original src tree:");
            ZFPathInfoTreePrint(pathInfoSrc, ZFOutputDefault(), "    ");
            this->output("original dst tree:");
            ZFPathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            zfobj<ZFIOBufferByCacheFile> io;
            ZFCompressDir(io->output(), pathInfoSrc, "");

            ZFDecompressDir(pathInfoDst, io->input());
            this->output("decompressed dst tree:");
            ZFPathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            ZFPathInfoRemove(pathInfoDst);
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCompress_test)

ZF_NAMESPACE_GLOBAL_END

