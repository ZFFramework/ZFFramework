/**
 * @file zfstr.h
 * @brief string format util
 */

#ifndef _ZFI_zfstr_h_
#define _ZFI_zfstr_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief string format util
 *
 * usage:
 * @code
 *   zfstring s = zfstr("test %s", sth);
 *   zfstringAppend(s, ", test %s %s", sth1, sth2);
 * @endcode
 *
 * it's ensured type safe, you may simply use `%s` everywhere,
 * but for convenient, we also support these tokens:
 * -  "b": zfbool (e.g. true)
 * -  "zi": zfindex (e.g. 13579)
 * -  "d" or "i": zfint (e.g. -13579)
 * -  "u": zfuint (e.g. 13579)
 * -  "o": zfuint, in octal (e.g. 01234567)
 * -  "x": zfuint, in heximal, lowercase (e.g. 0x1234abcd)
 * -  "X": zfuint, in heximal, uppercase (e.g. 0x1234ABCD)
 * -  "f": zffloat, accuracy is not ensured (e.g. 13.579)
 * -  "p": pointer value (e.g. 0x1234ABCD)
 * -  "c" or "C": char
 * -  "s" or "S": zfchar string
 * -  "%": % itself
 *
 * extra format tokens can be added before any of those listed above (except "%"),
 * similar to printf, they are:
 * -  "-": left justify
 * -  "0": leading zero fill
 * -  "+": print plus if positive number
 * -  " ": print space if positive number
 * -  "m": minimum width
 * -  ".n": decimal width for float types or max width for string types
 *
 * note:
 * -  support up to 16 params
 * -  it's safe to pass any type that registered by #ZFOUTPUT_TYPE,
 *   and you must register you custom type before calling this method
 *   with your custom type
 * -  params are converted to string before extra parsing (such as convert to `x`),
 *   when not able to perform conversion,
 *   it's ensured no error would occur for safety and convenient
 * -  when params less than `%s` token,
 *   the exceed `%s` token would be processed as empty string was passed
 * -  when params more than `%s` token,
 *   the exceed param would be discarded
 */
inline const zfchar *zfstr(
        ZF_IN const zfchar *fmt = zfnull
        ) {
    return fmt;
}

/** @brief see #zfstr */
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        ) {
    s += fmt;
}

/** @brief see #zfstr */
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        ) {
    if(s) {
        (*s) += fmt;
    }
}

extern ZFLIB_ZFCore void _ZFP_zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN const zfstring *param
        , ZF_IN zfindex paramCount
        );

/** @cond ZFPrivateDoc */

// ============================================================

template<
typename P0
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        ) {
    zfstring p[1];
    zftToString(p[0], p0);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        ) {
    zfstring p[2];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        ) {
    zfstring p[3];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        ) {
    zfstring p[4];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        ) {
    zfstring p[5];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        ) {
    zfstring p[6];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        ) {
    zfstring p[7];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        ) {
    zfstring p[8];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        ) {
    zfstring p[9];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        ) {
    zfstring p[10];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        ) {
    zfstring p[11];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        ) {
    zfstring p[12];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        ) {
    zfstring p[13];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        ) {
    zfstring p[14];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        ) {
    zfstring p[15];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    zftToString(p[14], p14);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
, typename P15
>
inline void zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        , ZF_IN P15 const &p15
        ) {
    zfstring p[16];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    zftToString(p[14], p14);
    zftToString(p[15], p15);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
}

// ============================================================

template<
typename P0
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        ) {
    if(s) {
        zfstring p[1];
        zftToString(p[0], p0);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        ) {
    if(s) {
        zfstring p[2];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        ) {
    if(s) {
        zfstring p[3];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        ) {
    if(s) {
        zfstring p[4];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        ) {
    if(s) {
        zfstring p[5];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        ) {
    if(s) {
        zfstring p[6];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        ) {
    if(s) {
        zfstring p[7];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        ) {
    if(s) {
        zfstring p[8];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        ) {
    if(s) {
        zfstring p[9];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        ) {
    if(s) {
        zfstring p[10];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        ) {
    if(s) {
        zfstring p[11];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        ) {
    if(s) {
        zfstring p[12];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        zftToString(p[11], p11);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        ) {
    if(s) {
        zfstring p[13];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        zftToString(p[11], p11);
        zftToString(p[12], p12);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        ) {
    if(s) {
        zfstring p[14];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        zftToString(p[11], p11);
        zftToString(p[12], p12);
        zftToString(p[13], p13);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        ) {
    if(s) {
        zfstring p[15];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        zftToString(p[11], p11);
        zftToString(p[12], p12);
        zftToString(p[13], p13);
        zftToString(p[14], p14);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
, typename P15
>
inline void zfstringAppend(
        ZF_IN_OUT_OPT zfstring *s
        , ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        , ZF_IN P15 const &p15
        ) {
    if(s) {
        zfstring p[16];
        zftToString(p[0], p0);
        zftToString(p[1], p1);
        zftToString(p[2], p2);
        zftToString(p[3], p3);
        zftToString(p[4], p4);
        zftToString(p[5], p5);
        zftToString(p[6], p6);
        zftToString(p[7], p7);
        zftToString(p[8], p8);
        zftToString(p[9], p9);
        zftToString(p[10], p10);
        zftToString(p[11], p11);
        zftToString(p[12], p12);
        zftToString(p[13], p13);
        zftToString(p[14], p14);
        zftToString(p[15], p15);
        _ZFP_zfstringAppend(*s, fmt, p, sizeof(p) / sizeof(p[0]));
    }
}

// ============================================================

template<
typename P0
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        ) {
    zfstring s;
    zfstring p[1];
    zftToString(p[0], p0);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        ) {
    zfstring s;
    zfstring p[2];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        ) {
    zfstring s;
    zfstring p[3];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        ) {
    zfstring s;
    zfstring p[4];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        ) {
    zfstring s;
    zfstring p[5];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        ) {
    zfstring s;
    zfstring p[6];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        ) {
    zfstring s;
    zfstring p[7];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        ) {
    zfstring s;
    zfstring p[8];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        ) {
    zfstring s;
    zfstring p[9];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        ) {
    zfstring s;
    zfstring p[10];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        ) {
    zfstring s;
    zfstring p[11];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        ) {
    zfstring s;
    zfstring p[12];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        ) {
    zfstring s;
    zfstring p[13];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        ) {
    zfstring s;
    zfstring p[14];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        ) {
    zfstring s;
    zfstring p[15];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    zftToString(p[14], p14);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}
template<
typename P0
, typename P1
, typename P2
, typename P3
, typename P4
, typename P5
, typename P6
, typename P7
, typename P8
, typename P9
, typename P10
, typename P11
, typename P12
, typename P13
, typename P14
, typename P15
>
inline zfstring zfstr(
        ZF_IN const zfchar *fmt
        , ZF_IN P0 const &p0
        , ZF_IN P1 const &p1
        , ZF_IN P2 const &p2
        , ZF_IN P3 const &p3
        , ZF_IN P4 const &p4
        , ZF_IN P5 const &p5
        , ZF_IN P6 const &p6
        , ZF_IN P7 const &p7
        , ZF_IN P8 const &p8
        , ZF_IN P9 const &p9
        , ZF_IN P10 const &p10
        , ZF_IN P11 const &p11
        , ZF_IN P12 const &p12
        , ZF_IN P13 const &p13
        , ZF_IN P14 const &p14
        , ZF_IN P15 const &p15
        ) {
    zfstring s;
    zfstring p[16];
    zftToString(p[0], p0);
    zftToString(p[1], p1);
    zftToString(p[2], p2);
    zftToString(p[3], p3);
    zftToString(p[4], p4);
    zftToString(p[5], p5);
    zftToString(p[6], p6);
    zftToString(p[7], p7);
    zftToString(p[8], p8);
    zftToString(p[9], p9);
    zftToString(p[10], p10);
    zftToString(p[11], p11);
    zftToString(p[12], p12);
    zftToString(p[13], p13);
    zftToString(p[14], p14);
    zftToString(p[15], p15);
    _ZFP_zfstringAppend(s, fmt, p, sizeof(p) / sizeof(p[0]));
    return s;
}

/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfstr_h_

#include "zfstr_CoreType.h"

