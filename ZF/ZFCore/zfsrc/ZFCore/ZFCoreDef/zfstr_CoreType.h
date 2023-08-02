/**
 * @file zfstr_CoreType.h
 * @brief declare output for core types
 */

#ifndef _ZFI_zfstr_CoreType_h_
#define _ZFI_zfstr_CoreType_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreStringConvert.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOUTPUT_TYPE(zfbool, {s += (v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);})
ZFOUTPUT_TYPE(zfbyte, {zfsFromIntT(s, v, 16);})
ZFOUTPUT_TYPE(zfchar, {s += v;})
ZFOUTPUT_TYPE(zfstring, {s += v;})
ZFOUTPUT_TYPE(const zfchar *, {s += (v ? v : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(zfchar *, {s += (v ? v : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(const void *, {
    if(v) {
        zfsFromPointerT(s, v);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFOUTPUT_TYPE(void *, {
    if(v) {
        zfsFromPointerT(s, v);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFOUTPUT_TYPE(zfint, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(zfuint, {
    if(v == (zfuint)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v);
    }
})
ZFOUTPUT_TYPE(zfindex, {
    if(v == (zfindex)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v);
    }
})
ZFOUTPUT_TYPE(zffloat, {zfsFromFloatT(s, v);})
ZFOUTPUT_TYPE(zfdouble, {zfsFromFloatT(s, v);})
ZFOUTPUT_TYPE(zflongdouble, {zfsFromFloatT(s, v);})
ZFOUTPUT_TYPE(zftimet, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(zfflags, {
    if(v == (zfflags)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v, 16);
    }
})
ZFOUTPUT_TYPE(zfidentity, {
    if(v == (zfidentity)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v);
    }
})
ZFOUTPUT_TYPE(ZFFuncAddrType, {zfsFromPointerT(s, (const void *)v);})
ZFOUTPUT_TYPE(ZFCompareResult, {
    switch(v) {
        case ZFCompareUncomparable:
            s += ZFTOKEN_ZFCompareUncomparable;
            break;
        case ZFCompareSmaller:
            s += ZFTOKEN_ZFCompareSmaller;
            break;
        case ZFCompareTheSame:
            s += ZFTOKEN_ZFCompareTheSame;
            break;
        case ZFCompareGreater:
            s += ZFTOKEN_ZFCompareGreater;
            break;
        default:
            break;
    }
})
ZFOUTPUT_TYPE(ZFSeekPos, {
    switch(v) {
        case ZFSeekPosBegin:
            s += ZFTOKEN_ZFSeekPosBegin;
            break;
        case ZFSeekPosCur:
            s += ZFTOKEN_ZFSeekPosCur;
            break;
        case ZFSeekPosCurReversely:
            s += ZFTOKEN_ZFSeekPosCurReversely;
            break;
        case ZFSeekPosEnd:
            s += ZFTOKEN_ZFSeekPosEnd;
            break;
        default:
            break;
    }
})
ZFOUTPUT_TYPE(ZFIndexRange, {
    s += "(";
    zfsFromIntT(s, v.start);
    s += ", ";
    zfsFromIntT(s, v.count);
    s += ")";
})

// ============================================================
ZFOUTPUT_TYPE(long long, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(unsigned long long, {
    if(v == (unsigned long long)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v);
    }
})
ZFOUTPUT_TYPE(double, {zfsFromFloatT(s, v);})
ZFOUTPUT_TYPE(long double, {zfsFromFloatT(s, v);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfstr_CoreType_h_

