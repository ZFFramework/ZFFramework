/**
 * @file ZFCoreDataEncode.h
 * @brief util to encode data to printable string
 */

#ifndef _ZFI_ZFCoreDataEncode_h_
#define _ZFI_ZFCoreDataEncode_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore const zfchar _ZFP_ZFCoreDataEncodeCharMapDefault[256];
/**
 * @brief see #ZFCoreDataEncode, default is all numbers and all lower and upper letters
 *
 * here's a list of contents:
 * @code
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    // 0x40 ~ 0x4F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x60 ~ 0x6F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 * @endcode
 */
#define ZFCoreDataEncodeCharMapDefault() _ZFP_ZFCoreDataEncodeCharMapDefault

extern ZFLIB_ZFCore const zfchar _ZFP_ZFCoreDataEncodeCharMapAllPrintable[256];
/**
 * @brief see #ZFCoreDataEncode, all printable chars 0x20 ~ 0x7E
 *
 * here's a list of contents:
 * @code
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x40 ~ 0x4F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x60 ~ 0x6F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 * @endcode
 */
#define ZFCoreDataEncodeCharMapAllPrintable() _ZFP_ZFCoreDataEncodeCharMapAllPrintable

/**
 * @brief util to encode data to printable string
 *
 * use charMap to declare the printable chars,
 * it's a 256 size zfchar array,
 * zero element means the char isn't printable and non-zero means printable\n
 * not printable chars would be escaped by escapeToken
 */
extern ZFLIB_ZFCore void ZFCoreDataEncode(
        ZF_OUT zfstring &result
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT const zfchar *charMap = ZFCoreDataEncodeCharMapDefault()
        , ZF_IN_OPT zfchar escapeToken = '%'
        );
/**
 * @brief see #ZFCoreDataEncode
 */
extern ZFLIB_ZFCore void ZFCoreDataDecode(
        ZF_OUT zfstring &result
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfchar escapeToken = '%'
        );

extern ZFLIB_ZFCore zfstring _ZFP_ZFCoreDataEncodeCharMapCreate(ZF_IN const zfchar *base, ZF_IN zfint c, ...);
/**
 * @brief util to create char map for #ZFCoreDataEncode
 *
 * the char map would be initialized by base,
 * each param is an index value which can be:
 * -  [0, 255] : extend the char map by `(zfchar)c`, so that the specified char won't be escaped
 * -  [-255, 0) : remove the char map by `(zfchar)(1 - c)`, so that the specified char should be escaped
 */
#define ZFCoreDataEncodeCharMapCreate(base, c, ...) _ZFP_ZFCoreDataEncodeCharMapCreate(base, c, ##__VA_ARGS__, (zfint)256)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreDataEncode_h_

