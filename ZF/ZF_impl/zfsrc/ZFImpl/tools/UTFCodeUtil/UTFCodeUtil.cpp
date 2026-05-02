#include "UTFCodeUtil.h"
#include <cstdlib>
#include <cstring>

namespace UTFCodeUtil {

size_t UTF8GetWordCount(const char *src, size_t len) {
    size_t count = 0;
    const unsigned char *p = (const unsigned char *)src;
    const unsigned char *pEnd = p + len;
    while(p < pEnd) {
        if((*p & 0x80) == 0x00) {
            // 1-byte: 0xxxxxxx
            p += 1;
        }
        else if((*p & 0xE0) == 0xC0 && p + 1 < pEnd) {
            // 2-byte: 110xxxxx 10xxxxxx
            p += 2;
        }
        else if((*p & 0xF0) == 0xE0 && p + 2 < pEnd) {
            // 3-byte: 1110xxxx 10xxxxxx 10xxxxxx
            p += 3;
        }
        else if((*p & 0xF8) == 0xF0 && p + 3 < pEnd) {
            // 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            p += 4;
        }
        else {
            // invalid
            p += 1;
        }
        ++count;
    }
    return count;
}

size_t UTF16ToUTF8(char *buf, const wchar_t *src, size_t len) {
    if(src == NULL) {return 0;}
    size_t size = 0;
    const wchar_t *srcEnd = src + len;
    if(buf == NULL) {
        while(src < srcEnd) {
            unsigned int c = *src++;
            if(c >= 0xD800 && c <= 0xDBFF && src < srcEnd) {
                unsigned int low = *src;
                if(low >= 0xDC00 && low <= 0xDFFF) {
                    ++src;
                    c = 0x10000 + ((c - 0xD800) << 10) + (low - 0xDC00);
                }
            }
            if(c <= 0x7F) {size += 1;}
            else if(c <= 0x7FF) {size += 2;}
            else if(c <= 0xFFFF) {size += 3;}
            else {size += 4;}
        }
        return size + 1;
    }

    while(src < srcEnd) {
        unsigned int c = *src++;
        if(c >= 0xD800 && c <= 0xDBFF && src < srcEnd) {
            unsigned int low = *src;
            if(low >= 0xDC00 && low <= 0xDFFF) {
                ++src;
                c = 0x10000 + ((c - 0xD800) << 10) + (low - 0xDC00);
            }
        }
        if(c <= 0x7F) {
            *buf++ = (unsigned char)c;
            size += 1;
        }
        else if(c <= 0x7FF) {
            *buf++ = (unsigned char)((c >> 6) | 0xC0);
            *buf++ = (unsigned char)((c & 0x3F) | 0x80);
            size += 2;
        }
        else if(c <= 0xFFFF) {
            *buf++ = (unsigned char)((c >> 12) | 0xE0);
            *buf++ = (unsigned char)(((c >> 6) & 0x3F) | 0x80);
            *buf++ = (unsigned char)((c & 0x3F) | 0x80);
            size += 3;
        }
        else {
            *buf++ = (unsigned char)((c >> 18) | 0xF0);
            *buf++ = (unsigned char)(((c >> 12) & 0x3F) | 0x80);
            *buf++ = (unsigned char)(((c >> 6) & 0x3F) | 0x80);
            *buf++ = (unsigned char)((c & 0x3F) | 0x80);
            size += 4;
        }
    }
    *buf = '\0';
    return size;
}
size_t UTF8ToUTF16(wchar_t *buf, const char *src, size_t len) {
    if(src == NULL) {return 0;}
    if(buf == NULL) {
        return UTF8GetWordCount(src, len) + 1;
    }

    size_t i = 0;
    const unsigned char *p = (const unsigned char *)src;
    const unsigned char *pEnd = p + len;
    while(p < pEnd && *p) {
        unsigned long cp = 0;
        // 1-byte: 0xxxxxxx
        if((*p & 0x80) == 0x00) {
            cp = *p;
            p += 1;
        }
        // 2-byte: 110xxxxx 10xxxxxx
        else if((*p & 0xE0) == 0xC0
                && p + 1 < pEnd
                && (p[1] & 0xC0) == 0x80
                ) {
            cp = ((*p & 0x1F) << 6) | (p[1] & 0x3F);
            p += 2;
        }
        // 3-byte: 1110xxxx 10xxxxxx 10xxxxxx
        else if((*p & 0xF0) == 0xE0
                && p + 2 < pEnd
                && (p[1] & 0xC0) == 0x80
                && (p[2] & 0xC0) == 0x80
                ) {
            cp = ((*p & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
            p += 3;
        }
        // 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        else if((*p & 0xF8) == 0xF0
                && p + 3 < pEnd
                && (p[1] & 0xC0) == 0x80
                && (p[2] & 0xC0) == 0x80
                && (p[3] & 0xC0) == 0x80
                ) {
            cp = ((*p & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
            p += 4;
        }
        // invalid
        else {
            cp = ' ';
            p += 1;
        }
        if (cp <= 0xFFFF) {
            buf[i++] = (wchar_t)cp;
        }
        else {
            cp -= 0x10000;
            buf[i++] = (wchar_t)(0xFFFF & (0xD800 | (cp >> 10)));
            buf[i++] = (wchar_t)(0xFFFF & (0xDC00 | (cp & 0x3FF)));
        }
    }
    buf[i] = '\0';
    return i;
}
size_t UTF16ToUTF16BE(wchar_t *buf, const wchar_t *src, size_t len) {
    if(src == NULL) {return 0;}
    if(buf != NULL) {
        const wchar_t *srcEnd = src + len;
        while(src < srcEnd && *src) {
            wchar_t c = *src++;
            *buf = (wchar_t)(((c << 8) & 0xFF00) | ((c >> 8) & 0x00FF));
            ++buf;
        }
        *buf = 0;
    }
    return len;
}
size_t UTF16BEToUTF16(wchar_t *buf, const wchar_t *src, size_t len) {
    return UTFCodeUtil::UTF16ToUTF16BE(buf, src, len);
}

} // namespace UTFCodeUtil
