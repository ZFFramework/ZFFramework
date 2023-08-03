#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFRegExp_test : zfextends ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFRegExp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFRegExp);

        zfblockedAlloc(ZFRegExp, regexp);
        const zfchar *patternFrom = "(ab*c)(?<n0>ab*c)";
        const zfchar *patternTo = "{$1}";
        const zfchar *stringFrom = "zacabcabbcabbbcz";

        regexp->regExpCompile(patternFrom);

        {
            ZFRegExpResult result;
            this->testCaseOutputSeparator();
            this->testCaseOutput("find:");
            regexp->regExpMatch(result, stringFrom);
            this->testCaseOutput(zfstr("  pattern    : %s", patternFrom));
            this->testCaseOutput(zfstr("  string     : %s", stringFrom));
            this->testCaseOutput(zfstr("  result     : %s", result));
            this->testCaseOutput(zfstr("  named group: %s", regexp->regExpNamedGroupIndexForName("n0")));
        }

        {
            ZFRegExpResult result;
            zfstring stringTo;
            this->testCaseOutputSeparator();
            this->testCaseOutput("replace:");
            regexp->regExpReplace(stringTo, result, patternTo, stringFrom);
            this->testCaseOutput(zfstr("  pattern from: %s", patternFrom));
            this->testCaseOutput(zfstr("  pattern to  : %s", patternTo));
            this->testCaseOutput(zfstr("  string  from: %s", stringFrom));
            this->testCaseOutput(zfstr("  string  to  : %s", stringTo));
            this->testCaseOutput(zfstr("  match result: %s", result));
            this->testCaseOutput(zfstr("  named group: %s", regexp->regExpNamedGroupIndexForName("n0")));
        }
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFRegExp_test)

ZF_NAMESPACE_GLOBAL_END

