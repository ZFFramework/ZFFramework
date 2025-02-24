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
ZFOUTPUT_TYPE(zfbyte, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(zfchar, {s += v;})
ZFOUTPUT_TYPE(zfstring, {s += v;})
ZFOUTPUT_TYPE(zfstring::Char, {s += (zfchar)v;})
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
ZFOUTPUT_TYPE(zflong, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(zfulong, {
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
ZFOUTPUT_TYPE(zftimet, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(zfflags, {
    if(v == (zfflags)-1) {
        s += "-1";
    }
    else {
        zfsFromIntT(s, v);
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
        case ZFCompareEqual:
            s += ZFTOKEN_ZFCompareEqual;
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
// pointers fallback to reference
template<typename T_Type>
zfclassNotPOD _ZFP_zftToStringFallback<T_Type *> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Type * const &v
            ) {
        if(v == zfnull) {
            s += ZFTOKEN_zfnull;
        }
        else {
            zftToStringT(s, *v);
        }
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_zftToStringFallback<const T_Type *> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN const T_Type * const &v
            ) {
        if(v == zfnull) {
            s += ZFTOKEN_zfnull;
        }
        else {
            zftToStringT(s, *v);
        }
    }
};

// ============================================================
// function addr fallback to raw pointer
template<typename T_Ret>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(void)> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(void)
            ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
, typename T_P11
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        , T_P11
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                , T_P11
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
, typename T_P11
, typename T_P12
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        , T_P11
        , T_P12
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                , T_P11
                , T_P12
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
, typename T_P11
, typename T_P12
, typename T_P13
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        , T_P11
        , T_P12
        , T_P13
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                , T_P11
                , T_P12
                , T_P13
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
, typename T_P11
, typename T_P12
, typename T_P13
, typename T_P14
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        , T_P11
        , T_P12
        , T_P13
        , T_P14
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                , T_P11
                , T_P12
                , T_P13
                , T_P14
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};
template<typename T_Ret
, typename T_P0
, typename T_P1
, typename T_P2
, typename T_P3
, typename T_P4
, typename T_P5
, typename T_P6
, typename T_P7
, typename T_P8
, typename T_P9
, typename T_P10
, typename T_P11
, typename T_P12
, typename T_P13
, typename T_P14
, typename T_P15
>
zfclassNotPOD _ZFP_zftToStringFallback<T_Ret (*)(
        T_P0
        , T_P1
        , T_P2
        , T_P3
        , T_P4
        , T_P5
        , T_P6
        , T_P7
        , T_P8
        , T_P9
        , T_P10
        , T_P11
        , T_P12
        , T_P13
        , T_P14
        , T_P15
        )> {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Ret (* const &v)(
                T_P0
                , T_P1
                , T_P2
                , T_P3
                , T_P4
                , T_P5
                , T_P6
                , T_P7
                , T_P8
                , T_P9
                , T_P10
                , T_P11
                , T_P12
                , T_P13
                , T_P14
                , T_P15
                )
                ) {
        zfsFromPointerT(s, (const void *)v);
    }
};

// ============================================================
// special number types
ZFOUTPUT_TYPE(long, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(long long, {zfsFromIntT(s, v);})
ZFOUTPUT_TYPE(double, {zfsFromFloatT(s, v);})
ZFOUTPUT_TYPE(long double, {zfsFromFloatT(s, v);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfstr_CoreType_h_

