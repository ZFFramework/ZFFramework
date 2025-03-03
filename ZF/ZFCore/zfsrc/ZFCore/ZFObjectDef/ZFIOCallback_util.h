/**
 * @file ZFIOCallback_util.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_util_h_
#define _ZFI_ZFIOCallback_util_h_

#include "ZFIOCallback_input.h"
#include "ZFIOCallback_output.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief read single logic char to buf, return count of sizoef(zfchar) if success, or 0 if fail
 *
 * since UTF-8 has variable logic char size, use this method to read a single logic char\n
 * note: result may have more than one char for invalid encoding,
 * and buf size must at least 9 byte size
 */
extern ZFLIB_ZFCore zfindex ZFInputReadChar(
        ZF_OUT zfchar *buf
        , ZF_IN_OUT const ZFInput &input
        );
/**
 * @brief see #ZFInputReadChar
 */
extern ZFLIB_ZFCore zfindex ZFInputReadChar(
        ZF_IN_OUT zfstring &buf
        , ZF_IN_OUT const ZFInput &input
        );

/**
 * @brief util method to skip chars in set
 *
 * return false if all chars are skipped before end,
 * other wise, buf stores the first logic char (see #ZFInputReadChar)
 */
extern ZFLIB_ZFCore zfbool ZFInputSkipChars(
        ZF_OUT zfchar *buf
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT const zfchar *charSet = " \t\r\n"
        );

/**
 * @brief read until any char in charSet shows up, or reached maxCount,
 *   return count read so far
 *
 * the first char matched charSet would be read and discarded,
 * and you may check it by firstMatchedChar,
 * if reached end or maxCount before matched charSet,
 * 0 would be returned to firstMatchedChar
 */
extern ZFLIB_ZFCore zfindex ZFInputReadUntil(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT const zfchar *charSet = " \t\r\n"
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_OUT_OPT zfchar *firstMatchedChar = zfnull
        );

/**
 * @brief util method to check whether the input match the tokens
 *
 * return token's index if match or zfindexMax() if no match or error,
 * header white spaces would be skipped automatically\n
 * if no match, this method would try to restore the callback state by ioSeek to original position
 */
extern ZFLIB_ZFCore zfindex ZFInputCheckMatch(
        ZF_IN const zfchar **tokens
        , ZF_IN zfindex tokenCount
        , ZF_IN_OUT const ZFInput &input
        );

// ============================================================
/**
 * @brief util method to read contents of input to output
 *
 * return size already written to output even if error occurred
 */
extern ZFLIB_ZFCore zfindex ZFInputRead(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT zfindex size = zfindexMax()
        );
/**
 * @brief util method to read contents from input to string
 *
 * return size appended to string
 */
extern ZFLIB_ZFCore zfindex ZFInputRead(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT zfindex size = zfindexMax()
        );

// ============================================================
/**
 * @brief util method to read one line
 *
 * return size read for the line (excluding the endl), or 0 for empty line,
 * or zfindexMax if failed
 */
extern ZFLIB_ZFCore zfindex ZFInputReadLine(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OUT const ZFInput &input
        );
/**
 * @brief util method to read one line
 *
 * return size read for the line (excluding the endl), or 0 for empty line,
 * or zfindexMax if failed\n
 * note the result would be appended to the string
 */
extern ZFLIB_ZFCore zfindex ZFInputReadLine(
        ZF_IN_OUT zfstring &output
        , ZF_IN_OUT const ZFInput &input
        );

// ============================================================
/**
 * @brief repeat output
 */
inline void ZFOutputRepeat(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const zfchar *token
        , ZF_IN zfindex count
        ) {
    if(count != 0 && !zfstringIsEmpty(token)) {
        zfindex len = zfslen(token);
        for(zfindex i = 0; i < count; ++i) {
            output.execute(token, len);
        }
    }
}

// ============================================================
/**
 * @brief util to create a chained output
 *
 * typical usage:
 * @code
 *   ZFOutputDefault(ZFOutputChain(ZFOutputForConsole()
 *           , ZFOutputForFile('/path/to/log/file')
 *       ))
 * @endcode
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputChain(
        ZF_IN const ZFOutput &o0
        , ZF_IN const ZFOutput &o1
        , ZF_IN_OPT const ZFOutput &o2 = zfnull
        , ZF_IN_OPT const ZFOutput &o3 = zfnull
        , ZF_IN_OPT const ZFOutput &o4 = zfnull
        , ZF_IN_OPT const ZFOutput &o5 = zfnull
        , ZF_IN_OPT const ZFOutput &o6 = zfnull
        , ZF_IN_OPT const ZFOutput &o7 = zfnull
        );
/**
 * @brief util to create a chained output
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputChain(
        ZF_IN const ZFCoreArray<ZFOutput> &o
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_util_h_

