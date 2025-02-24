#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFRegExp_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFRegExp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFRegExp);

        zfobj<ZFRegExp> regexp;
        const zfchar *patternFrom = "(ab*c)(?<n0>ab*c)";
        const zfchar *patternTo = "{$1}";
        const zfchar *stringFrom = "zacabcabbcabbbcz";

        regexp->pattern(patternFrom);

        {
            ZFRegExpResult result;
            this->outputSeparator();
            this->output("find:");
            regexp->find(result, stringFrom);
            this->output(zfstr("  pattern    : %s", patternFrom));
            this->output(zfstr("  string     : %s", stringFrom));
            this->output(zfstr("  result     : %s", result));
            this->output(zfstr("  named group: %s", regexp->namedGroupIndexForName("n0")));
        }

        {
            ZFRegExpResult result;
            zfstring stringTo;
            this->outputSeparator();
            this->output("replace:");
            regexp->replace(stringTo, result, patternTo, stringFrom);
            this->output(zfstr("  pattern from: %s", patternFrom));
            this->output(zfstr("  pattern to  : %s", patternTo));
            this->output(zfstr("  string  from: %s", stringFrom));
            this->output(zfstr("  string  to  : %s", stringTo));
            this->output(zfstr("  match result: %s", result));
            this->output(zfstr("  named group: %s", regexp->namedGroupIndexForName("n0")));
        }
        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFRegExp_test)

ZF_NAMESPACE_GLOBAL_END

