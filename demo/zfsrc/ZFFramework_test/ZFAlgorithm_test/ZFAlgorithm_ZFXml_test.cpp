#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFXml_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFXml_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFXml);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml");

        const zfchar *src =
            "<?xml type=\"declaration\"?>"
            "<!DOCTYPE TYPE \"DOCTYPE\">"
            "<?pi ProcessingInstruction?>"
            "<element0 attribute0=\"attribute value0\">"
            "<element1 attribute0=\"attribute value0\" />"
            "<element2 attribute0=\"attribute value0\" attribute1=\"attribute value1\""
            " attribute2=\"attribute value2\" attribute3=\"attribute value3\" />"
            "<element3>text0</element3>"
            "<element4><![CDATA[cdata text]]></element4>"
            "<element5>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>"
            "</element0>"
            ;

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml parse from string:");
        ZFXml doc = ZFXmlFromString(src);

        this->testCaseOutput("ZFXml parse from string, result:");
        zfLogTrim() << ZFXmlToString(doc);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml add element");
        ZFXml element(ZFXmlType::e_XmlElement);
        element.xmlName("testElement");
        doc.childAdd(element);
        this->testCaseOutput("ZFXml add element, result:");
        zfLogTrim() << ZFXmlToString(doc);

        this->performanceTest(doc);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFXml &xmlItem) {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeLogBegin("ZFXmlPerformance_test_toData");
        for(zfindex i = 0; i < toDataTimes; ++i) {
            zfstring tmp;
            ZFXmlToOutput(ZFOutputForString(tmp), xmlItem);
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFXmlPerformance_test_toData");

        zfindex fromDataTimes = toDataTimes;
        zfstring xmlString;
        ZFXmlToOutput(ZFOutputForString(xmlString), xmlItem);
        ZFCoreStatistic::invokeTimeLogBegin("ZFXmlPerformance_test_fromData");
        for(zfindex i = 0; i < fromDataTimes; ++i) {
            ZFXmlFromInput(ZFInputForBufferUnsafe(xmlString));
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFXmlPerformance_test_fromData");
        ZFXml xmlItemNew = ZFXmlFromInput(ZFInputForBufferUnsafe(xmlString));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFXmlPerformance_test_toData");
        this->testCaseOutput(zfstr("serialize to data %s times cost %s seconds"
                    , toDataTimes
                    , ZFTimeValueToStringFriendly(toDataUsedTime)
                    ));

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFXmlPerformance_test_fromData");
        this->testCaseOutput(zfstr("serialize from data %s times cost %s seconds"
                    , fromDataTimes
                    , ZFTimeValueToStringFriendly(fromDataUsedTime)
                    ));

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfstr("content: %s", xmlString));
            this->testCaseOutput(zfstr("newly: %s", xmlItemNew));
        #endif

        ZFCoreStatistic::invokeTimeRemove("ZFXmlPerformance_test_toData");
        ZFCoreStatistic::invokeTimeRemove("ZFXmlPerformance_test_fromData");
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFXml_test)

ZF_NAMESPACE_GLOBAL_END

