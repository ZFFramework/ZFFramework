/**
 * @file ZFCoreStringConvert.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreStringConvert_h_
#define _ZFI_ZFCoreStringConvert_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief convert integer number (int, long, etc) to string
 *
 * integer number can be signed or unsigned,
 * whatever your compiler support\n
 * typical usage:
 * @code
 *   zfstring s = ...;
 *   zfint n = ...;
 *
 *   // append result to s and return it as result
 *   zfsFromIntT(s, n);
 *
 *   // return a new zfstring as result
 *   s = zfsFromInt(n);
 * @endcode
 */
template<typename T_Int>
zfbool zfsFromIntT(
        ZF_OUT zfstring &s
        , ZF_IN T_Int n
        , ZF_IN_OPT zfindex radix = 10
        , ZF_IN_OPT zfbool useUpperCase = zftrue
        ) {
    if(radix < 2 || radix > 36) {return zffalse;}

    if(n <= (T_Int)0) {
        // tricks to solve the unsigned type warnings
        if(n != (T_Int)0) {
            s += '-';
            if(n == (T_Int)0 - n) {
                s += '1';
                return zftrue;
            }
            n = (T_Int)0 - n;
        }
        else {
            s += '0';
            return zftrue;
        }
    }
    else if(n == (T_Int)-1) {
        s += '-';
        s += '1';
        return zftrue;
    }

    unsigned long long e = 1;
    while(n / e) {e *= radix;}

    const zfchar *token = useUpperCase
        ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        : "0123456789abcdefghijklmnopqrstuvwxyz";
    while(e /= radix) {
        s += token[n / e];
        n %= e;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromIntT
 */
template<typename T_Int>
zfstring zfsFromInt(
        ZF_IN T_Int n
        , ZF_IN_OPT zfindex radix = 10
        , ZF_IN_OPT zfbool useUpperCase = zftrue
        ) {
    zfstring s;
    zfsFromIntT(s, n, radix, useUpperCase);
    return s;
}

// ============================================================
/**
 * @brief convert string to int, return error position if failed,
 *   or null if success
 *
 * it's your responsibility to make sure the template int type is correct,
 * otherwise, the result may be unexpected
 * (for example, specify a short int type while numbers in src is too long)
 */
template<typename T_Int>
zfbool zfsToIntT(
        ZF_OUT T_Int &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfindex radix = 10
        , ZF_IN_OPT zfbool allowNegative = zftrue
        ) {
    if(src == zfnull || srcLen == 0 || radix < 2 || radix > 36) {
        return zffalse;
    }

    ret = 0;
    const zfchar *p = src;
    const zfchar *pEnd = ((srcLen == zfindexMax()) ? (p - 1) : (p + srcLen));
    zfbool negative = zffalse;
    if(*p == '-') {
        if(!allowNegative) {
            return zffalse;
        }
        negative = zftrue;
        ++p;
    }
    while(*p != '\0' && p != pEnd) {
        zfuint tmp = 0;
        if(*p >= '0' && *p <= '9') {tmp = *p - '0';}
        else if(*p >= 'a' && *p <= 'z') {tmp = 10 + *p - 'a';}
        else if(*p >= 'A' && *p <= 'Z') {tmp = 10 + *p - 'A';}
        else {break;}
        if(tmp >= radix) {break;}
        ret = (T_Int)((ret * (zfuint)radix) + tmp);
        ++p;
    }
    if(negative) {
        ret = (T_Int)0 - ret;
    }

    if(*p == '\0' || p == pEnd) {
        return zftrue;
    }
    else {
        return zffalse;
    }
}
/**
 * @brief see #zfsToIntT
 */
template<typename T_Int>
T_Int zfsToInt(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfindex radix = 10
        , ZF_IN_OPT zfbool allowNegative = zftrue
        ) {
    T_Int ret = 0;
    zfsToIntT(ret, src, srcLen, radix, allowNegative);
    return ret;
}
/**
 * @brief see #zfsToIntT
 */
inline zfint zfsToInt(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfindex radix = 10
        , ZF_IN_OPT zfbool allowNegative = zftrue
        ) {
    return zfsToInt<zfint>(src, srcLen, radix, allowNegative);
}

// ============================================================
/**
 * @brief convert float number (int, long, etc) to string
 *
 * float number can be any type such as float, double, long double,
 * whatever your compiler support\n
 * for typical usage, see #zfsFromIntT
 */
template<typename T_Float>
zfbool zfsFromFloatT(
        ZF_OUT zfstring &s
        , ZF_IN T_Float n
        ) {
    zfchar buf[64] = {0};
    snprintf(buf, sizeof(buf), "%lf", (double)n);
    const zfchar *p = buf;
    while(*p && *p != '.') {++p;}
    if(*p == '\0') {
        s += buf;
        return zftrue;
    }
    const zfchar *pEnd = p + zfslen(p) - 1;
    if(*pEnd == '0') {
        do {--pEnd;} while(*pEnd == '0');
        if(pEnd == p) {
            s.append(buf, p - buf);
        }
        else {
            s.append(buf, pEnd + 1 - buf);
        }
    }
    else {
        s += buf;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromFloatT
 */
template<typename T_Float>
zfstring zfsFromFloat(ZF_IN T_Float n) {
    zfstring s;
    zfsFromFloatT(s, n);
    return s;
}

// ============================================================
/**
 * @brief convert string to float
 *
 * it's your responsibility to make sure int type is correct,
 * otherwise, the result may be unexpected\n
 * for typical usage, see #zfsToIntT
 * @note if src is null, pErrPos would be null
 */
template<typename T_Float>
zfbool zfsToFloatT(
        ZF_OUT T_Float &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    if(src == zfnull || srcLen == 0) {
        return zffalse;
    }

    ret = (T_Float)0;
    const zfchar *p = src;
    const zfchar *pEnd = ((srcLen == zfindexMax()) ? (p - 1) : (p + srcLen));
    zfbool negative = zffalse;
    if(*p == '-') {
        negative = zftrue;
        ++p;
    }

    while(*p != '\0' && *p != '.' && p != pEnd) {
        if(*p < '0' || *p > '9') {break;}
        ret = ret * 10 + (*p - '0');
        ++p;
    }
    if(*p != '\0' && *p != '.' && p != pEnd) {
        return zffalse;
    }

    if(*p == '.') {
        ++p;

        unsigned long e = 10;
        while(*p != '\0' && p != pEnd
                && *p >= '0' && *p <= '9'
                ) {
            ret += (T_Float)(*p - '0') / e;
            e *= 10;
            ++p;
        }
        if(*p != '\0' && p != pEnd) {
            return zffalse;
        }
    }

    // 1.23e+21
    // 1.23e-21
    if(*p == 'e') {
        if(*(p + 1) == '+') {
            zfint e = -1;
            if(zfsToIntT(e, p + 2, pEnd - (p + 2), 10, zffalse)
                    && e > 0
                    ) {
                while(e > 0) {
                    ret *= 10;
                    --e;
                }
                p = pEnd;
            }
        }
        else if(*(p + 1) == '-') {
            zfint e = -1;
            if(zfsToIntT(e, p + 2, pEnd - (p + 2), 10, zffalse)
                    && e > 0
                    ) {
                while(e > 0) {
                    ret /= 10;
                }
                p = pEnd;
            }
        }
    }

    if(negative) {
        ret = (T_Float)(0 - ret);
    }

    if(*p == '\0' || p == pEnd) {
        return zftrue;
    }
    else {
        return zffalse;
    }
}
/**
 * @brief see #zfsToFloatT
 */
template<typename T_Float>
T_Float zfsToFloat(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    T_Float ret = 0;
    zfsToFloatT(ret, src, srcLen);
    return ret;
}
/**
 * @brief see #zfsToFloatT
 */
inline zffloat zfsToFloat(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    return zfsToFloat<zffloat>(src, srcLen);
}


// ============================================================
/**
 * @brief convert pointer value to string
 */
inline zfbool zfsFromPointerT(
        ZF_OUT zfstring &s
        , ZF_IN const void *p
        ) {
    zfchar buf[32] = {0};
    if(p == zfnull) {
        snprintf(buf, sizeof(buf), "<null>");
    }
    else {
        snprintf(buf, sizeof(buf), "%p", p);
    }
    const zfchar *t = buf;
    while(*t) {
        s += *t++;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromPointerT
 */
inline zfstring zfsFromPointer(ZF_IN const void *p) {
    zfstring s;
    zfsFromPointerT(s, p);
    return s;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringConvert_h_

