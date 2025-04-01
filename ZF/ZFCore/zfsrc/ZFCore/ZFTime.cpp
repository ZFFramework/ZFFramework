#include "ZFTime.h"
#include "protocol/ZFProtocolZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTimeValue
ZFEXPORT_VAR_READONLY_DEFINE(ZFTimeValue, ZFTimeValueZero, ZFTimeValueCreate(0, 0))

ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFTimeValue, ZFTimeValueCreate
        , ZFMP_IN(zftimet, sec)
        , ZFMP_IN(zftimet, usec)
        )

ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFTimeValue, ZFTimeValueFromMiliSeconds
        , ZFMP_IN(zftimet, t)
        )

ZFMETHOD_FUNC_INLINE_DEFINE_1(zftimet, ZFTimeValueToMiliSeconds
        , ZFMP_IN(const ZFTimeValue &, v)
        )

ZFMETHOD_FUNC_DEFINE_1(void, ZFTimeValueNormalize
        , ZFMP_IN_OUT(ZFTimeValue &, v)
        ) {
    while(v.usec >= 1000000LL) {
        ++v.sec;
        v.usec -= 1000000LL;
    }
    while(v.usec <= -1000000LL) {
        --v.sec;
        v.usec += 1000000LL;
    }
    if(v.sec > 0 && v.usec < 0) {
        --v.sec;
        v.usec += 1000000LL;
    }
    else if(v.sec < 0 && v.usec > 0) {
        ++v.sec;
        v.usec -= 1000000LL;
    }
}

ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFTimeValueDec
        , ZFMP_OUT(ZFTimeValue &, result)
        , ZFMP_IN(const ZFTimeValue &, tv1)
        , ZFMP_IN(const ZFTimeValue &, tv2)
        )
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFTimeValue, ZFTimeValueDec
        , ZFMP_IN(const ZFTimeValue &, tv1)
        , ZFMP_IN(const ZFTimeValue &, tv2)
        )

ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFTimeValueInc
        , ZFMP_OUT(ZFTimeValue &, result)
        , ZFMP_IN(const ZFTimeValue &, tv1)
        , ZFMP_IN(const ZFTimeValue &, tv2)
        )
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFTimeValue, ZFTimeValueInc
        , ZFMP_IN(const ZFTimeValue &, tv1)
        , ZFMP_IN(const ZFTimeValue &, tv2)
        )

ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFTimeValueMul
        , ZFMP_OUT(ZFTimeValue &, result)
        , ZFMP_IN(const ZFTimeValue &, tv)
        , ZFMP_IN(zfindex, v)
        )
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFTimeValue, ZFTimeValueMul
        , ZFMP_IN(const ZFTimeValue &, tv)
        , ZFMP_IN(zfindex, v)
        )

ZFMETHOD_FUNC_DEFINE_3(void, ZFTimeValueDiv
        , ZFMP_OUT(ZFTimeValue &, result)
        , ZFMP_IN(const ZFTimeValue &, tv)
        , ZFMP_IN(zfindex, v)
        ) {
    ZFCoreAssertWithMessage(v != 0, "divided by 0");
    result.usec = tv.usec / v;
    result.sec = tv.sec / v;
    result.usec += (zftimet)(((zfdouble)tv.sec / v - result.sec) * 1000000LL);
    ZFTimeValueNormalize(result);
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFTimeValue, ZFTimeValueDiv
        , ZFMP_IN(const ZFTimeValue &, tv)
        , ZFMP_IN(zfindex, v)
        )

ZFMETHOD_FUNC_DEFINE_2(ZFCompareResult, ZFTimeValueCompare
        , ZFMP_IN(const ZFTimeValue &, tv1)
        , ZFMP_IN(const ZFTimeValue &, tv2)
        ) {
    if(tv1.sec == tv2.sec) {
        if(tv1.usec == tv2.usec) {
            return ZFCompareEqual;
        }
        else if(tv1.usec < tv2.usec) {
            return ZFCompareSmaller;
        }
        else {
            return ZFCompareGreater;
        }
    }
    else if(tv1.sec < tv2.sec) {
        return ZFCompareSmaller;
    }
    else {
        return ZFCompareGreater;
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFTimeValueToStringFriendlyT
        , ZFMP_IN_OUT(zfstring &, s)
        , ZFMP_IN(ZFTimeValue const &, v)
        ) {
    ZFTimeInfo ti;
    if(ZFTime::timeInfoFromTimeValue(ti, v)) {
        if(ti.year != 1970) {
            zfstringAppend(s, "%02s-", ti.year);
        }
        if(!s.isEmpty() || ti.month != 0 || ti.day != 0) {
            zfstringAppend(s, "%02s-%02s ", ti.month + 1, ti.day + 1);
        }

        if(!s.isEmpty() || ti.hour != 0 || ti.minute != 0) {
            zfstringAppend(s, "%02s:%02s:", ti.hour, ti.minute);
        }

        if(!s.isEmpty()) {
            zfstringAppend(s, "%02s.", ti.second);
        }
        else {
            zfstringAppend(s, "%s.", ti.second);
        }

        zfstringAppend(s, "%03s%03s", ti.miliSecond, ti.microSecond);
    }
    else {
        zfstringAppend(s, "%s.%06s", v.sec, v.usec);
    }
    return zftrue;
}
ZFMETHOD_FUNC_INLINE_DEFINE_1(zfstring, ZFTimeValueToStringFriendly
        , ZFMP_IN(ZFTimeValue const &, v)
        )

zfbool operator < (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return (ZFTimeValueCompare(v0, v1) == ZFCompareSmaller);
}
zfbool operator <= (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return (ZFTimeValueCompare(v0, v1) != ZFCompareGreater);
}
zfbool operator > (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return (ZFTimeValueCompare(v0, v1) == ZFCompareGreater);
}
zfbool operator >= (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return (ZFTimeValueCompare(v0, v1) != ZFCompareSmaller);
}
ZFTimeValue operator + (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return ZFTimeValueInc(v0, v1);
}
ZFTimeValue operator - (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    return ZFTimeValueDec(v0, v1);
}
void operator += (
        ZF_IN_OUT ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    ZFTimeValueInc(v0, v0, v1);
}
void operator -= (
        ZF_IN_OUT ZFTimeValue &v0
        , ZF_IN const ZFTimeValue &v1
        ) {
    ZFTimeValueDec(v0, v0, v1);
}
ZFTimeValue operator * (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN zfindex v1
        ) {
    return ZFTimeValueMul(v0, v1);
}
ZFTimeValue operator / (
        ZF_IN const ZFTimeValue &v0
        , ZF_IN zfindex v1
        ) {
    return ZFTimeValueDiv(v0, v1);
}
void operator *= (
        ZF_IN_OUT ZFTimeValue &v0
        , ZF_IN zfindex v1
        ) {
    ZFTimeValueMul(v0, v0, v1);
}
void operator /= (
        ZF_IN_OUT ZFTimeValue &v0
        , ZF_IN zfindex v1
        ) {
    ZFTimeValueDiv(v0, v0, v1);
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFTimeValue, ZFTimeValue, {
        ZFCoreArray<zftimet> tmp;
        if(!ZFCoreDataPairSplitInt(tmp, 2, src, srcLen)) {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        v.sec = tmp[0];
        v.usec = tmp[1];
        return zftrue;
    }, {
        s += "(";
        zfsFromIntT(s, v.sec);
        s += ", ";
        zfsFromIntT(s, v.usec);
        s += ")";
        return zftrue;
    })

ZFTYPEID_PROGRESS_DEFINE(ZFTimeValue, ZFTimeValue, {
        ret.sec = from.sec + (zft_zftimet)((to.sec - from.sec) * progress);
        zfdouble usec = (to.sec - from.sec) * (zfdouble)progress;
        ret.usec = from.usec + (zft_zftimet)((usec - (zfint)usec) * 1000000LL);
        ZFTimeValueNormalize(ret);
    })

// ============================================================
// ZFTimeInfo
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFTimeInfo, ZFTimeInfo)
ZFOUTPUT_TYPE_DEFINE(ZFTimeInfo, {ZFTimeInfoToStringT(s, v);})

ZFEXPORT_VAR_READONLY_DEFINE(ZFTimeInfo, ZFTimeInfoZero, ZFTimeInfoCreate(0, 0, 0, 0, 0, 0, 0, 0))

ZFMETHOD_FUNC_INLINE_DEFINE_8(ZFTimeInfo, ZFTimeInfoCreate
        , ZFMP_IN(zfint const &, year)
        , ZFMP_IN(zfuint const &, month)
        , ZFMP_IN(zfuint const &, day)
        , ZFMP_IN(zfuint const &, hour)
        , ZFMP_IN(zfuint const &, minute)
        , ZFMP_IN(zfuint const &, second)
        , ZFMP_IN(zfuint const &, miliSecond)
        , ZFMP_IN(zfuint const &, microSecond)
        )

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFTimeInfoToStringT
        , ZFMP_IN_OUT(zfstring &, s)
        , ZFMP_IN(ZFTimeInfo const &, v)
        ) {
    zfstringAppend(s, "%s-%02s-%02s %02s:%02s:%02s.%03s %03s",
        v.year, v.month + 1, v.day + 1,
        v.hour, v.minute, v.second,
        v.miliSecond, v.microSecond);
    return zftrue;
}
ZFMETHOD_FUNC_INLINE_DEFINE_1(zfstring, ZFTimeInfoToString
        , ZFMP_IN(ZFTimeInfo const &, v)
        )

// ============================================================
#define _ZFP_ZFTimeImpl ZFPROTOCOL_ACCESS(ZFTime)

// _ZFP_ZFTimePrivate
zfclassNotPOD _ZFP_ZFTimePrivate {
public:
    ZFTimeValue tv;
    ZFTimeValue tz;
    ZFTimeValue tvAppliedTimeZone;
    ZFTimeInfo ti;
public:
    _ZFP_ZFTimePrivate(void)
    : tv(ZFTimeValueZero())
    , tz(ZFTime::timeZoneLocal())
    , tvAppliedTimeZone(tz)
    , ti(ZFTimeInfoZero())
    {
        _ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone);
    }
    void clear(void) {
        this->tv = ZFTimeValueZero();
        this->tz = ZFTime::timeZoneLocal();
        this->tvAppliedTimeZone = this->tz;
        _ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone);
    }
    void copyFrom(_ZFP_ZFTimePrivate *p) {
        this->tv = p->tv;
        this->tz = p->tz;
        this->tvAppliedTimeZone = p->tvAppliedTimeZone;
        this->ti = p->ti;
    }
    zfbool timeValue(const ZFTimeValue &tv) {
        this->tv = tv;
        ZFTimeValueInc(this->tvAppliedTimeZone, this->tv, this->tz);
        if(!_ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone)) {
            this->clear();
            return zffalse;
        }

        return zftrue;
    }
    zfbool timeZone(const ZFTimeValue &tz) {
        this->tz = tz;
        ZFTimeValueInc(this->tvAppliedTimeZone, this->tv, this->tz);
        if(!_ZFP_ZFTimeImpl->timeInfoFromTimeValue(this->ti, this->tvAppliedTimeZone)) {
            this->clear();
            return zffalse;
        }

        return zftrue;
    }
    zfbool timeInfo(
            ZF_IN const ZFTimeInfo &ti
            , ZF_IN const ZFTimeValue &tz
            ) {
        if(!_ZFP_ZFTimeImpl->timeInfoToTimeValue(this->tvAppliedTimeZone, ti)) {
            this->clear();
            return zffalse;
        }

        this->ti = ti;
        this->tz = tz;
        ZFTimeValueDec(this->tv, this->tvAppliedTimeZone, this->tz);
        return zftrue;
    }
};

// ============================================================
// ZFTime
ZFOBJECT_REGISTER(ZFTime)

zfbool ZFTime::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    ZFTimeValue timeValue = ZFTimeValueZero();
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFTime_timeValue, ZFTimeValue, timeValue, {
                return zffalse;
            });
    this->timeValue(timeValue);

    ZFTimeValue timeZone = ZFTime::timeZoneLocal();
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFTime_timeZone, ZFTimeValue, timeZone, {
                return zffalse;
            });
    this->timeZone(timeZone);

    return zftrue;
}
zfbool ZFTime::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref,
            ZFSerializableKeyword_ZFTime_timeValue, ZFTimeValue, this->timeValue(), ref->timeValue(), ZFTimeValueZero(), {
                return zffalse;
            });

    ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref,
            ZFSerializableKeyword_ZFTime_timeZone, ZFTimeValue, this->timeZone(), ref->timeZone(), ZFTime::timeZoneLocal(), {
                return zffalse;
            });

    return zftrue;
}

ZFMETHOD_DEFINE_1(ZFTime, zfbool, leapYear
        , ZFMP_IN(zfint, year)
        ) {
    return _ZFP_ZFTimeImpl->leapYear(year);
}
ZFMETHOD_DEFINE_2(ZFTime, zfint, leapYearBetween
        , ZFMP_IN(zfint, year1)
        , ZFMP_IN(zfint, year2)
        ) {
    return _ZFP_ZFTimeImpl->leapYearBetween(year1, year2);
}
ZFMETHOD_DEFINE_0(ZFTime, zftimet, timestamp) {
    return _ZFP_ZFTimeImpl->timestamp();
}
ZFMETHOD_DEFINE_0(ZFTime, ZFTimeValue, currentTimeValue) {
    ZFTimeValue tv;
    _ZFP_ZFTimeImpl->currentTimeValue(tv);
    return tv;
}
ZFMETHOD_DEFINE_0(ZFTime, zftimet, currentTime) {
    return ZFTimeValueToMiliSeconds(ZFTime::currentTimeValue());
}
ZFMETHOD_DEFINE_1(ZFTime, ZFTimeInfo, currentTimeInfo
        , ZFMP_IN_OPT(const ZFTimeValue &, localTimeZone, ZFTime::timeZoneLocal())
        ) {
    ZFTimeInfo ti;
    ZFTime::timeInfoFromTimeValue(ti,
        ZFTimeValueInc(ZFTime::currentTimeValue(), localTimeZone));
    return ti;
}
ZFMETHOD_DEFINE_2(ZFTime, zfbool, timeInfoFromTimeValue
        , ZFMP_OUT(ZFTimeInfo &, ti)
        , ZFMP_IN(const ZFTimeValue &, tv)
        ) {
    return _ZFP_ZFTimeImpl->timeInfoFromTimeValue(ti, tv);
}
ZFMETHOD_DEFINE_2(ZFTime, zfbool, timeInfoToTimeValue
        , ZFMP_OUT(ZFTimeValue &, tv)
        , ZFMP_IN(const ZFTimeInfo &, ti)
        ) {
    return _ZFP_ZFTimeImpl->timeInfoToTimeValue(tv, ti);
}
zfbool ZFTime::timeInfoToTimeValue(
        ZF_OUT ZFTimeValue &tv
        , ZF_IN zfint year
        , ZF_IN zfuint month
        , ZF_IN zfuint day
        , ZF_IN zfuint hour
        , ZF_IN zfuint minute
        , ZF_IN zfuint second
        , ZF_IN zfuint miliSecond
        , ZF_IN zfuint microSecond
        ) {
    return _ZFP_ZFTimeImpl->timeInfoToTimeValue(
        tv,
        ZFTimeInfoCreate(year, month, day, hour, minute, second, miliSecond, microSecond));
}

ZFMETHOD_DEFINE_0(ZFTime, const ZFTimeValue &, timeZoneLocal) {
    return _ZFP_ZFTimeImpl->timeZoneLocal();
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFTime
        , ZFMP_IN(ZFTime *, time)
        ) {
    this->objectOnInit();
    if(time != zfnull) {
        d->copyFrom(time->d);
    }
}
ZFOBJECT_ON_INIT_DEFINE_2(ZFTime
        , ZFMP_IN(const ZFTimeValue &, tv)
        , ZFMP_IN_OPT(const ZFTimeValue &, timeZone, ZFTime::timeZoneLocal())
        ) {
    this->objectOnInit();
    zfself::timeZone(timeZone);
    zfself::timeValue(tv);
}
ZFOBJECT_ON_INIT_DEFINE_2(ZFTime
        , ZFMP_IN(const ZFTimeInfo &, ti)
        , ZFMP_IN_OPT(const ZFTimeValue &, timeZone, ZFTime::timeZoneLocal())
        ) {
    this->objectOnInit();
    zfself::timeInfo(ti, timeZone);
}

void ZFTime::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimePrivate);
}
void ZFTime::objectOnDealloc(void) {
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFTime::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfstringAppend(ret, "%04s-%02s-%02s %02s:%02s:%02s.%03s %03s",
        this->year(),
        this->month() + 1,
        this->day() + 1,
        this->hour(),
        this->minute(),
        this->second(),
        this->miliSecond(),
        this->microSecond());
}

zfclassPOD _ZFP_ZFTime_hash {
public:
    ZFTimeValue tv;
    ZFTimeValue tz;
};
zfidentity ZFTime::objectHashImpl(void) {
    _ZFP_ZFTime_hash tmp = {d->tv, d->tz};
    return zfidentityCalcPOD(tmp);
}

ZFCompareResult ZFTime::objectCompareImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFTimeValueCompare(this->timeValueAppliedTimeZone(), another->timeValueAppliedTimeZone());
}

ZFMETHOD_DEFINE_1(ZFTime, zfbool, timeValue
        , ZFMP_IN(const ZFTimeValue &, tv)
        ) {
    d->timeValue(tv);
    return _ZFP_ZFTimeImpl->timeInfoFromTimeValue(d->ti, d->tvAppliedTimeZone);
}
ZFMETHOD_DEFINE_0(ZFTime, const ZFTimeValue &, timeValue) {
    return d->tv;
}

ZFMETHOD_DEFINE_1(ZFTime, zfbool, timeZone
        , ZFMP_IN(const ZFTimeValue &, timeZone)
        ) {
    return d->timeZone(timeZone);
}
ZFMETHOD_DEFINE_0(ZFTime, const ZFTimeValue &, timeZone) {
    return d->tz;
}
ZFMETHOD_DEFINE_0(ZFTime, const ZFTimeValue &, timeValueAppliedTimeZone) {
    return d->tvAppliedTimeZone;
}

zfbool ZFTime::timeInfo(
        ZF_IN zfint year
        , ZF_IN zfuint month
        , ZF_IN zfuint day
        , ZF_IN zfuint hour
        , ZF_IN zfuint minute
        , ZF_IN zfuint second
        , ZF_IN zfuint miliSecond
        , ZF_IN zfuint microSecond
        , ZF_IN_OPT const ZFTimeValue &tz /* = ZFTime::timeZoneLocal() */
        ) {
    return d->timeInfo(ZFTimeInfoCreate(year, month, day, hour, minute, second, miliSecond, microSecond), tz);
}
ZFMETHOD_DEFINE_2(ZFTime, zfbool, timeInfo
        , ZFMP_IN(const ZFTimeInfo &, ti)
        , ZFMP_IN_OPT(const ZFTimeValue &, tz, ZFTime::timeZoneLocal())
        ) {
    return d->timeInfo(ti, tz);
}
ZFMETHOD_DEFINE_0(ZFTime, const ZFTimeInfo &, timeInfo) {
    return d->ti;
}

ZFMETHOD_DEFINE_0(ZFTime, zfint, year) {
    return d->ti.year;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, month) {
    return d->ti.month;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, day) {
    return d->ti.day;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, dayOfWeek) {
    return _ZFP_ZFTimeImpl->dayOfWeek(d->tvAppliedTimeZone, d->ti);
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, dayOfYear) {
    return _ZFP_ZFTimeImpl->dayOfYear(d->tvAppliedTimeZone, d->ti);
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, hour) {
    return d->ti.hour;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, minute) {
    return d->ti.minute;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, second) {
    return d->ti.second;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, miliSecond) {
    return d->ti.miliSecond;
}
ZFMETHOD_DEFINE_0(ZFTime, zfuint, microSecond) {
    return d->ti.microSecond;
}

ZF_NAMESPACE_GLOBAL_END

