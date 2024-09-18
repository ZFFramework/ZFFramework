#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFJson_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFJson_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFJson);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson");

        const zfchar *src =
            "{"
            "    \"k0\" : \"v0\","
            ""
            "    \"object0\" : {},"
            "    \"object1\" : {"
            "        \"k0\" : \"v0\","
            "        \"k1\" : \"v1\""
            "    },"
            "    \"object2\" : {"
            "        \"k0\" : {\"k0\" : \"v0\"}"
            "    },"
            ""
            "    \"array0\" : [],"
            "    \"array1\" : ["
            "        {\"k0\" : \"v0\"},"
            "        {\"k0\" : \"v0\"}"
            "    ],"
            ""
            "    \"k1\" : \"special chars: \\\\ \\\"\""
            "}"
            ;

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson parse from string:");
        ZFJson json;
        ZFJsonFromStringT(json, src);

        this->testCaseOutput("ZFJson parse from string, result:");
        ZFLogTrim() << ZFJsonToString(json);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson add element");
        json.attr("testJsonKey", "testJsonValue");
        this->testCaseOutput("ZFJson add element, result:");
        ZFLogTrim() << ZFJsonToString(json);

        this->performanceTest(json);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFJson &jsonItem) {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeLogBegin("ZFJsonPerformance_test_toData");
        for(zfindex i = 0; i < toDataTimes; ++i) {
            zfstring tmp;
            ZFJsonToOutput(ZFOutputForString(tmp), jsonItem);
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFJsonPerformance_test_toData");

        zfindex fromDataTimes = toDataTimes;
        zfstring jsonString;
        ZFJsonToOutput(ZFOutputForString(jsonString), jsonItem);
        ZFCoreStatistic::invokeTimeLogBegin("ZFJsonPerformance_test_fromData");
        for(zfindex i = 0; i < fromDataTimes; ++i) {
            ZFJsonFromInput(ZFInputForString(jsonString));
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFJsonPerformance_test_fromData");
        ZFJson jsonItemNew = ZFJsonFromInput(ZFInputForString(jsonString));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFJsonPerformance_test_toData");
        this->testCaseOutput(zfstr("serialize to data %s times cost %s seconds"
                    , toDataTimes
                    , ZFTimeValueToStringFriendly(toDataUsedTime)
                    ));

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFJsonPerformance_test_fromData");
        this->testCaseOutput(zfstr("serialize from data %s times cost %s seconds"
                    , fromDataTimes
                    , ZFTimeValueToStringFriendly(fromDataUsedTime)
                    ));

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfstr("content: %s", jsonString));
            this->testCaseOutput(zfstr("newly: %s", jsonItemNew));
        #endif

        ZFCoreStatistic::invokeTimeRemove("ZFJsonPerformance_test_toData");
        ZFCoreStatistic::invokeTimeRemove("ZFJsonPerformance_test_fromData");
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFJson_test)

ZF_NAMESPACE_GLOBAL_END

