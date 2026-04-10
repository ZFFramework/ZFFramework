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
    extern UTFCodeUtil_EXPORT size_t UTF8GetWordCount(const char *src, size_t len);

    /**
     * @brief convert UTF16 to UTF8
     *
     * when buf is NULL:
     * -  return buffer size including tail '\0', if success
     * -  return (size_t)-1, if failed
     *
     * when buf is not NULL:
     * -  the buf must be large enough, the result would be written,
     *   including tail '\0'
     */
    extern UTFCodeUtil_EXPORT size_t UTF16ToUTF8(char *buf, const wchar_t *src, size_t len);
    /**
     * @brief convert UTF8 to UTF16
     *
     * use space if letter out of UTF16's range
     */
    extern UTFCodeUtil_EXPORT size_t UTF8ToUTF16(wchar_t *buf, const char *src, size_t len);

    /**
     * @brief convert UTF16 to UTF16 big endian
     */
    extern UTFCodeUtil_EXPORT size_t UTF16ToUTF16BE(wchar_t *buf, const wchar_t *src, size_t len);
    /**
     * @brief convert UTF16 big endian to UTF16
     */
    extern UTFCodeUtil_EXPORT size_t UTF16BEToUTF16(wchar_t *buf, const wchar_t *src, size_t len);

} // namespace UTFCodeUtil

#endif // #ifndef _UTFCodeUtil_h_

