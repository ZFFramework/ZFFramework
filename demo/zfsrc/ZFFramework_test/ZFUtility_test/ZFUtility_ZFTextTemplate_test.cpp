#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUtility_ZFTextTemplate_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUtility_ZFTextTemplate_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->outputSeparator();
        this->output("ZFTextTemplate");

        const zfchar *src =
            "========== test replace begin ==========\n"
            "<exist>{ZFTT_R_replace_exist}<exist>\n"
            "<not_exist>{ZFTT_R_replace_not_exist}<not_exist>\n"
            "---------- test replace  end  ----------\n"
            "\n"
            "========== test enableif begin ==========\n"
            "<enableif_true>{ZFTT_C_enableif_true}enableif_true{ZFTT_CE}<enableif_true>\n"
            "<enableif_false>{ZFTT_C_enableif_false}enableif_false{ZFTT_CE}<enableif_false>\n"
            "\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}enableif{ZFTT_C_enableif_false}enableif_false{ZFTT_CE}_true{ZFTT_CE}<enableif_embeded_0>\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}{ZFTT_R_replace_exist}{ZFTT_CE}<enableif_embeded_0>\n"
            "<enableif_embeded_0>{ZFTT_C_enableif_true}{ZFTT_R_replace_not_exist}{ZFTT_CE}<enableif_embeded_0>\n"
            "\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}enableif{ZFTT_C_enableif_true}enableif_true{ZFTT_CE}_false{ZFTT_CE}<enableif_embeded_1>\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}{ZFTT_R_replace_exist}{ZFTT_CE}<enableif_embeded_1>\n"
            "<enableif_embeded_1>{ZFTT_C_enableif_false}{ZFTT_R_replace_not_exist}{ZFTT_CE}<enableif_embeded_1>\n"
            "---------- test enableif  end  ----------\n"
            "\n"
            "========== test index begin ==========\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "reset{ZFTT_IR_test_index}\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "this is {ZFTT_I_test_index} item\n"
            "---------- test index  end  ----------\n"
            ;

        zfstring buf;
        ZFTextTemplateParam param;
        param.replaceData("replace_exist", "_replace_exist_");
        param.enableData("enableif_true", zftrue);
        ZFTextTemplateApply(param, ZFOutputForString(buf), src);

        this->outputSeparator();
        this->output(zfstr("src:\n%s", src));

        this->outputSeparator();
        this->output(zfstr("applied:\n%s", buf));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFTextTemplate_test)

ZF_NAMESPACE_GLOBAL_END

