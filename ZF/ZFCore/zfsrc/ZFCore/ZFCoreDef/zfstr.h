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
inline zfstring zfstr(
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

typedef void (*_ZFP_zfstrFn)(ZF_IN_OUT zfstring &s, ZF_IN const void *p);

template<typename P>
void _ZFP_zfstrIvk(ZF_IN_OUT zfstring &s, ZF_IN const void *p) {
    zftToStringT(s, *(const P *)p);
}

extern ZFLIB_ZFCore void _ZFP_zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN zfindex paramCount
        , ZF_IN const void * const *param
        , ZF_IN _ZFP_zfstrFn *Fn
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
    const void *p[1] = {
        (const void *)&p0,
    };
    _ZFP_zfstrFn Fn[1] = {
        _ZFP_zfstrIvk<P0>,
    };
    _ZFP_zfstringAppend(s, fmt, 1, p, Fn);
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
    const void *p[2] = {
        (const void *)&p0,
        (const void *)&p1,
    };
    _ZFP_zfstrFn Fn[2] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
    };
    _ZFP_zfstringAppend(s, fmt, 2, p, Fn);
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
    const void *p[3] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
    };
    _ZFP_zfstrFn Fn[3] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
    };
    _ZFP_zfstringAppend(s, fmt, 3, p, Fn);
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
    const void *p[4] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
    };
    _ZFP_zfstrFn Fn[4] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
    };
    _ZFP_zfstringAppend(s, fmt, 4, p, Fn);
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
    const void *p[5] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
    };
    _ZFP_zfstrFn Fn[5] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
    };
    _ZFP_zfstringAppend(s, fmt, 5, p, Fn);
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
    const void *p[6] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
    };
    _ZFP_zfstrFn Fn[6] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
    };
    _ZFP_zfstringAppend(s, fmt, 6, p, Fn);
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
    const void *p[7] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
    };
    _ZFP_zfstrFn Fn[7] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
    };
    _ZFP_zfstringAppend(s, fmt, 7, p, Fn);
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
    const void *p[8] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
    };
    _ZFP_zfstrFn Fn[8] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
    };
    _ZFP_zfstringAppend(s, fmt, 8, p, Fn);
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
    const void *p[9] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
    };
    _ZFP_zfstrFn Fn[9] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
    };
    _ZFP_zfstringAppend(s, fmt, 9, p, Fn);
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
    const void *p[10] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
    };
    _ZFP_zfstrFn Fn[10] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
    };
    _ZFP_zfstringAppend(s, fmt, 10, p, Fn);
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
    const void *p[11] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
    };
    _ZFP_zfstrFn Fn[11] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
    };
    _ZFP_zfstringAppend(s, fmt, 11, p, Fn);
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
    const void *p[12] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
        (const void *)&p11,
    };
    _ZFP_zfstrFn Fn[12] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
        _ZFP_zfstrIvk<P11>,
    };
    _ZFP_zfstringAppend(s, fmt, 12, p, Fn);
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
    const void *p[13] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
        (const void *)&p11,
        (const void *)&p12,
    };
    _ZFP_zfstrFn Fn[13] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
        _ZFP_zfstrIvk<P11>,
        _ZFP_zfstrIvk<P12>,
    };
    _ZFP_zfstringAppend(s, fmt, 13, p, Fn);
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
    const void *p[14] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
        (const void *)&p11,
        (const void *)&p12,
        (const void *)&p13,
    };
    _ZFP_zfstrFn Fn[14] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
        _ZFP_zfstrIvk<P11>,
        _ZFP_zfstrIvk<P12>,
        _ZFP_zfstrIvk<P13>,
    };
    _ZFP_zfstringAppend(s, fmt, 14, p, Fn);
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
    const void *p[15] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
        (const void *)&p11,
        (const void *)&p12,
        (const void *)&p13,
        (const void *)&p14,
    };
    _ZFP_zfstrFn Fn[15] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
        _ZFP_zfstrIvk<P11>,
        _ZFP_zfstrIvk<P12>,
        _ZFP_zfstrIvk<P13>,
        _ZFP_zfstrIvk<P14>,
    };
    _ZFP_zfstringAppend(s, fmt, 15, p, Fn);
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
    const void *p[16] = {
        (const void *)&p0,
        (const void *)&p1,
        (const void *)&p2,
        (const void *)&p3,
        (const void *)&p4,
        (const void *)&p5,
        (const void *)&p6,
        (const void *)&p7,
        (const void *)&p8,
        (const void *)&p9,
        (const void *)&p10,
        (const void *)&p11,
        (const void *)&p12,
        (const void *)&p13,
        (const void *)&p14,
        (const void *)&p15,
    };
    _ZFP_zfstrFn Fn[16] = {
        _ZFP_zfstrIvk<P0>,
        _ZFP_zfstrIvk<P1>,
        _ZFP_zfstrIvk<P2>,
        _ZFP_zfstrIvk<P3>,
        _ZFP_zfstrIvk<P4>,
        _ZFP_zfstrIvk<P5>,
        _ZFP_zfstrIvk<P6>,
        _ZFP_zfstrIvk<P7>,
        _ZFP_zfstrIvk<P8>,
        _ZFP_zfstrIvk<P9>,
        _ZFP_zfstrIvk<P10>,
        _ZFP_zfstrIvk<P11>,
        _ZFP_zfstrIvk<P12>,
        _ZFP_zfstrIvk<P13>,
        _ZFP_zfstrIvk<P14>,
        _ZFP_zfstrIvk<P15>,
    };
    _ZFP_zfstringAppend(s, fmt, 16, p, Fn);
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
        zfstringAppend(*s, fmt
                , p0
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                , p11
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                , p11
                , p12
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                , p11
                , p12
                , p13
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                , p11
                , p12
                , p13
                , p14
                );
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
        zfstringAppend(*s, fmt
                , p0
                , p1
                , p2
                , p3
                , p4
                , p5
                , p6
                , p7
                , p8
                , p9
                , p10
                , p11
                , p12
                , p13
                , p14
                , p15
                );
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
    zfstringAppend(s, fmt
            , p0
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            , p11
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            , p11
            , p12
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            , p11
            , p12
            , p13
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            , p11
            , p12
            , p13
            , p14
            );
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
    zfstringAppend(s, fmt
            , p0
            , p1
            , p2
            , p3
            , p4
            , p5
            , p6
            , p7
            , p8
            , p9
            , p10
            , p11
            , p12
            , p13
            , p14
            , p15
            );
    return s;
}

/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfstr_h_

#include "zfstr_CoreType.h"

