#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFCrc32_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCrc32_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        const zfchar *testString = "123abc";
        zfflags testValue = (zfflags)0x8DFE51D2; // testString's CRC32 to verify
        zfflags value = 0;

        value = ZFCrc32((const zfbyte *)testString, zfslen(testString));
        this->output(zfstr("CRC32 of array \"%s\": %s", testString, value));
        ZFTestCaseAssert(value == testValue);

        zfstring tmpFilePath = this->testCaseUseTmpFile("ZFCrc32_Crc32.txt");
        void *fp = ZFFileOpen(tmpFilePath, v_ZFFileOpenOption::e_Write);
        if(fp != zfnull) {
            ZFFileWrite(fp, testString);
            ZFFileClose(fp);
            fp = zfnull;
        }
        this->output(zfstr("write it to file %s, file's CRC32: %s"
                    , tmpFilePath
                    , ZFCrc32(ZFInputForFile(tmpFilePath))
                    ));

        this->outputSeparator();
        tmpFilePath = this->testCaseUseTmpFile("ZFCrc32_Crc32_big.txt");
        fp = ZFFileOpen(tmpFilePath, v_ZFFileOpenOption::e_Write);
        zfindex fileSize = 0;
        if(fp != zfnull) {
            for(zfindex i = 0; i < 1000; i++) {
                for(zfindex j = 0; j < 1000; j++) {
                    ZFFileWrite(fp, testString);
                }
            }
            fileSize = ZFFileTell(fp);
            ZFFileClose(fp);
            fp = zfnull;
        }
        ZFTimeValue tv1 = ZFTime::currentTimeValue();
        zfflags CRC32BigFile = ZFCrc32(ZFInputForFile(tmpFilePath));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->output(zfstr("write it 1000*1000 times to file %s, file's size: %s, CRC32: %X, time: %s.%03s %03s"
                    , tmpFilePath
                    , fileSize
                    , CRC32BigFile
                    , tv2.sec
                    , tv2.usec / 1000
                    , tv2.usec % 1000
                    ));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCrc32_test)

ZF_NAMESPACE_GLOBAL_END

