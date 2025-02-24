/**
 * @file UTFCodeUtil.h
 * @brief simple UTF code convertor
 */

#ifndef _UTFCodeUtil_h_
#define _UTFCodeUtil_h_

/** @cond ZFPrivateDoc */
#include "../../ZFImpl_ZF_impl.h"
#define UTFCodeUtil_EXPORT ZFLIB_ZF_impl
/** @endcond */

#include <cwchar>

/** @brief used to export symbols */
#ifndef UTFCodeUtil_EXPORT
    #define UTFCodeUtil_EXPORT
#endif

namespace UTFCodeUtil {

    /**
     * @brief get logic word count of UTF8 string instead of byte size
     *
     * return (size_t)-1 if invalid utf8 chars exist
     */
    extern UTFCodeUtil_EXPORT size_t UTF8GetWordCount(const char *src);

    /**
     * @brief convert UTF16 to UTF8
     */
    extern UTFCodeUtil_EXPORT char *UTF16ToUTF8(const wchar_t *src);
    /**
     * @brief convert UTF8 to UTF16
     *
     * use space if letter out of UTF16's range
     */
    extern UTFCodeUtil_EXPORT wchar_t *UTF8ToUTF16(const char *src);

    /**
     * @brief convert UTF16 to UTF16 big endian
     */
    extern UTFCodeUtil_EXPORT wchar_t *UTF16ToUTF16BE(const wchar_t *src);
    /**
     * @brief convert UTF16 big endian to UTF16
     */
    extern UTFCodeUtil_EXPORT wchar_t *UTF16BEToUTF16(const wchar_t *src);

} // namespace UTFCodeUtil

#endif // #ifndef _UTFCodeUtil_h_

