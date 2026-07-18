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
        t->year(),
        t->month() + 1,
        t->day() + 1,
        t->hour(),
        t->minute(),
        t->second(),
        t->miliSecond(),
        t->microSecond(),
        t->dayOfWeek(),
        t->dayOfYear());
    return ret;
}

zfclass ZFCore_ZFTime_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFTime_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTime);

        this->outputSeparator();
        this->output("ZFTime");

        srand((zfuint)ZFTime::currentTimeValue().sec);
        this->output("calc leap year between two years, if nothing wrong, none would be printed:");
        for(zfint i = 0; i < 200; ++i) {
            zfint y1 = rand() % 30000;
            zfint y2 = rand() % 30000;
            zfint n1 = (zfint)_ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(y1, y2);
            zfint n2 = (zfint)ZFTime::leapYearBetween(y1, y2);
            if(n1 != n2 && n1 != -n2) {
                this->output(zfstr("calc LeapYear num error at loop: %s (y1: %6s, y2: %6s) n1: %6s, n2: %6s"
                            , i
                            , y1
                            , y2
                            , n1
                            , n2
                            ));
            }
        }
        this->outputSeparator();

        zfobj<ZFTime> t(ZFTime::currentTimeValue());
        this->output("current time:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t));

        zfobj<ZFTime> t2(ZFTimeValueZero());
        this->output("zero time:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t2));

        zfobj<ZFTime> t3(ZFTimeInfoCreate(
                    t->year(),
                    t->month(),
                    t->day(),
                    t->hour(),
                    t->minute(),
                    t->second(),
                    t->miliSecond(),
                    t->microSecond()
                    ));
        this->output("create a ZFTime:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t3));

        zfobj<ZFTime> t4(ZFTimeInfoCreate(
                    t->year() - 300,
                    t->month(),
                    t->day(),
                    t->hour(),
                    t->minute(),
                    t->second(),
                    t->miliSecond(),
                    t->microSecond()
                    ));
        this->output("time before 1970:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t4));

        zfobj<ZFTime> t5(ZFTimeInfoCreate(
                    t->year() - 3000,
                    t->month(),
                    t->day(),
                    t->hour(),
                    t->minute(),
                    t->second(),
                    t->miliSecond(),
                    t->microSecond()
                    ));
        this->output("time before 0000:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t5));

        zfobj<ZFTime> t6(ZFTimeInfoCreate(
                    t->year() + 3000,
                    t->month(),
                    t->day(),
                    t->hour(),
                    t->minute(),
                    t->second(),
                    t->miliSecond(),
                    t->microSecond()
                    ));
        this->output("time after 3000:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t6));

        zfobj<ZFTime> t7(t->timeValue());
        this->output("copy a ZFTime:");
        this->output(_ZFP_ZFCore_ZFTime_test_printZFTime(t7));

        // ============================================================
        this->outputSeparator();
        this->output("ZFTime performance test");
        zfobj<ZFTime> tCur1;
        zfobj<ZFTime> tCur2;

        this->output("\naccess none");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Year");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->year();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Month");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->month();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Day");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->day();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Hour");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->hour();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Minute");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->minute();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess Second");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->second();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess MiliSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->miliSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess MicroSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->microSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess DayOfWeek");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->dayOfWeek();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->output("\naccess DayOfYear");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue())));
        (void)tCur2->dayOfYear();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->output(zfstr("current timeValue: %s, used: %s"
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue())
                    , _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue()))
                    ));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTime_test)

ZF_NAMESPACE_GLOBAL_END

