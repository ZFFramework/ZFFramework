#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFMd5_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFMd5_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        const zfchar *testString = "123abc";
        zfstring testValue = "A906449D5769FA7361D7ECC6AA3F6D28"; // testString's MD5 to verify
        zfstring value;

        value = ZFMd5((const zfbyte *)testString, zfslen(testString));
        this->output(zfstr("MD5 of array \"%s\": %s", testString, value));
        ZFTestCaseAssert(value == testValue);

        ZFPathInfo tmpFilePath = this->testCaseUseTmpFile("ZFMd5_Md5.txt");
        zfautoT<ZFIOToken> fp = ZFIOOpen(tmpFilePath, v_ZFIOOpenOption::e_Write);
        if(fp != zfnull) {
            fp->ioWrite(testString);
            fp = zfnull;
        }
        this->output(zfstr("write it to file %s, file's MD5: %s"
                    , tmpFilePath
                    , ZFMd5(ZFInputForPathInfo(tmpFilePath))
                    ));

        this->outputSeparator();
        tmpFilePath = this->testCaseUseTmpFile("ZFMd5_Md5_big.txt");
        fp = ZFIOOpen(tmpFilePath, v_ZFIOOpenOption::e_Write);
        zfindex fileSize = 0;
        if(fp != zfnull) {
            for(zfindex i = 0; i < 1000; i++) {
                for(zfindex j = 0; j < 1000; j++) {
                    fp->ioWrite(testString);
                }
            }
            fileSize = fp->ioTell();
            fp = zfnull;
        }
        ZFTimeValue tv1 = ZFTime::currentTimeValue();
        zfstring MD5BigFile = ZFMd5(ZFInputForPathInfo(tmpFilePath));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->output(zfstr("write it 1000*1000 times to file %s, file's size: %s, MD5: %s, time: %s.%03s %03s"
                    , tmpFilePath
                    , fileSize
                    , MD5BigFile
                    , tv2.sec
                    , tv2.usec / 1000
                    , tv2.usec % 1000
                    ));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFMd5_test)

ZF_NAMESPACE_GLOBAL_END

