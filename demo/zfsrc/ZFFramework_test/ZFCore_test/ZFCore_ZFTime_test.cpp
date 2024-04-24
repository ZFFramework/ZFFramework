#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfindex _ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(zfint year1, zfint year2) {
    zfint t1 = year1;
    zfint t2 = year2;
    if(year1 > year2) {
        t1 = year2;
        t2 = year1;
    }
    zfuint ret = 0;
    for( ; t1 <= t2; ++t1) {
        if((t1 % 100 != 0 && t1 % 4 == 0)
                || t1 % 400 == 0
                ) {
            ++ret;
        }
    }
    return ret;
}
static zfstring _ZFP_ZFCore_ZFTime_test_printZFTimeValue(const ZFTimeValue &tv) {
    zfstring s;
    zfstringAppend(s, "\t%s\t%s", tv.sec, tv.usec);
    return s;
}

static zfstring _ZFP_ZFCore_ZFTime_test_printZFTime(ZFTime *t) {
    zfstring ret;
    zfstringAppend(ret, "  %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(t->timeValue()));
    zfstringAppend(ret, "  %6s-%02s-%02s %02s:%02s:%02s.%03s %03s, dayOfWeek: %s, dayOfYear: %s",
        t->timeInfoYear(),
        t->timeInfoMonth() + 1,
        t->timeInfoDay() + 1,
        t->timeInfoHour(),
        t->timeInfoMinute(),
        t->timeInfoSecond(),
        t->timeInfoMiliSecond(),
        t->timeInfoMicroSecond(),
        t->timeInfoDayOfWeek(),
        t->timeInfoDayOfYear());
    return ret;
}

zfclass ZFCore_ZFTime_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFTime_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTime);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTime");

        srand((zfuint)ZFTime::currentTimeValue().sec);
        this->testCaseOutput("calc leap year between two years, if nothing wrong, none would be printed:");
        for(zfint i = 0; i < 200; ++i) {
            zfint y1 = rand() % 30000;
            zfint y2 = rand() % 30000;
            zfint n1 = (zfint)_ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(y1, y2);
            zfint n2 = (zfint)ZFTime::leapYearBetween(y1, y2);
            if(n1 != n2 && n1 != -n2) {
                this->testCaseOutput(zfstr("calc LeapYear num error at loop: %s (y1: %6s, y2: %6s) n1: %6s, n2: %6s"
                            , i
                            , y1
                            , y2
                            , n1
                            , n2
                            ));
            }
        }
        this->testCaseOutputSeparator();

        zfobj<ZFTime> t(ZFTime::currentTimeValue());
        this->testCaseOutput("current time:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t));

        zfobj<ZFTime> t2(ZFTimeValueZero());
        this->testCaseOutput("zero time:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t2));

        zfobj<ZFTime> t3(ZFTimeInfoMake(
                    t->timeInfoYear(),
                    t->timeInfoMonth(),
                    t->timeInfoDay(),
                    t->timeInfoHour(),
                    t->timeInfoMinute(),
                    t->timeInfoSecond(),
                    t->timeInfoMiliSecond(),
                    t->timeInfoMicroSecond()
                    ));
        this->testCaseOutput("create a ZFTime:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t3));

        zfobj<ZFTime> t4(ZFTimeInfoMake(
                    t->timeInfoYear() - 300,
                    t->timeInfoMonth(),
                    t->timeInfoDay(),
                    t->timeInfoHour(),
                    t->timeInfoMinute(),
                    t->timeInfoSecond(),
                    t->timeInfoMiliSecond(),
                    t->timeInfoMicroSecond()
                    ));
        this->testCaseOutput("time before 1970:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t4));

        zfobj<ZFTime> t5(ZFTimeInfoMake(
                    t->timeInfoYear() - 3000,
                    t->timeInfoMonth(),
                    t->timeInfoDay(),
                    t->timeInfoHour(),
                    t->timeInfoMinute(),
                    t->timeInfoSecond(),
                    t->timeInfoMiliSecond(),
                    t->timeInfoMicroSecond()
                    ));
        this->testCaseOutput("time before 0000:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t5));

        zfobj<ZFTime> t6(ZFTimeInfoMake(
                    t->timeInfoYear() + 3000,
                    t->timeInfoMonth(),
                    t->timeInfoDay(),
                    t->timeInfoHour(),
                    t->timeInfoMinute(),
                    t->timeInfoSecond(),
                    t->timeInfoMiliSecond(),
                    t->timeInfoMicroSecond()
                    ));
        this->testCaseOutput("time after 3000:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t6));

        zfobj<ZFTime> t7(t->timeValue());
        this->testCaseOutput("copy a ZFTime:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t7));

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTime performance test");
        zfobj<ZFTime> tCur1;
        zfobj<ZFTime> tCur2;

        this->testCaseOutput("\naccess none");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Year");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoYear();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Month");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoMonth();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Day");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoDay();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Hour");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoHour();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Minute");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoMinute();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess Second");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess MiliSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoMiliSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess MicroSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoMicroSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess DayOfWeek");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoDayOfWeek();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseOutput("\naccess DayOfYear");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->timeInfoDayOfYear();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTime_test)

ZF_NAMESPACE_GLOBAL_END

