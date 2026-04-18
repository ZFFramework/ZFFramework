#include "UTFCodeUtil.h"
#include <cstdlib>
#include <cstring>

namespace UTFCodeUtil {

size_t UTF8GetWordCount(const char *src, size_t len) {
    size_t count = 0;
    const unsigned char *p = (const unsigned char *)src;
    const unsigned char *pEnd = (const unsigned char *)src + len;
    while(p < pEnd) {
        if((*p & 0x80) == 0x00) {
            // 1-byte: 0xxxxxxx
            p += 1;
        }
        else if((*p & 0xE0) == 0xC0) {
            // 2-byte: 110xxxxx 10xxxxxx
            p += 2;
        }
        else if((*p & 0xF0) == 0xE0) {
            // 3-byte: 1110xxxx 10xxxxxx 10xxxxxx
            p += 3;
        }
        else if((*p & 0xF8) == 0xF0) {
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
            if (*src <= 0x007F) {
                ++size;
            }
            else if(*src <= 0x07FF) {
                size += 2;
            }
            else if(*src <= 0xFFFF) {
                size += 3;
            }
            else {
                size += 4;
            }
            ++src;
        }
        ++size;
    }
    else {
        while(src < srcEnd) {
            if (*src <= 0x007F) {
                *buf++ = (unsigned char)(*src);
                ++size;
            }
            else if(*src <= 0x07FF) {
                *buf++ = (unsigned char)(((*src >> 6) & 0x1F) | 0xC0);
                *buf++ = (unsigned char)((*src & 0x3F) | 0x80);
                size += 2;
            }
            else if(*src <= 0xFFFF) {
                *buf++ = (unsigned char)(((*src >> 12) & 0x0F) | 0xE0);
                *buf++ = (unsigned char)(((*src >> 6) & 0x3F) | 0x80);
                *buf++ = (unsigned char)((*src & 0x3F) | 0x80);
                size += 3;
            }
            else {
                *buf++ = (unsigned char)((*src >> 18) | 0xF0);
                *buf++ = (unsigned char)(((*src >> 12) & 0x3F) | 0x80);
                *buf++ = (unsigned char)(((*src >> 6) & 0x3F) | 0x80);
                *buf++ = (unsigned char)((*src & 0x3F) | 0x80);
                size += 4;
            }
            ++src;
        }
        *buf = '\0';
    }
    return size;
}
size_t UTF8ToUTF16(wchar_t *buf, const char *src, size_t len) {
    if(src == NULL) {return 0;}
    if(buf == NULL) {
        return UTF8GetWordCount(src, len) + 1;
    }
    else {
        size_t i = 0;
        const unsigned char *p = (const unsigned char *)src;
        const unsigned char *pEnd = (const unsigned char *)src + len;
        while (*p && p < pEnd) {
            unsigned long cp;
            // 1-byte: 0xxxxxxx
            if((*p & 0x80) == 0x00) {
                cp = *p;
                p += 1;
            }
            // 2-byte: 110xxxxx 10xxxxxx
            else if((*p & 0xE0) == 0xC0) {
                cp = ((*p & 0x1F) << 6) | (p[1] & 0x3F);
                p += 2;
            }
            // 3-byte: 1110xxxx 10xxxxxx 10xxxxxx
            else if((*p & 0xF0) == 0xE0) {
                cp = ((*p & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
                p += 3;
            }
            // 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            else if((*p & 0xF8) == 0xF0) {
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
}
size_t UTF16ToUTF16BE(wchar_t *buf, const wchar_t *src, size_t len) {
    if(src == NULL) {return 0;}
    if(buf != NULL) {
        const wchar_t *srcEnd = src + len;
        while(*src != '\0' && src < srcEnd) {
            *buf = (((*src) << 8) & 0xFF00) | (((*src) >> 8) & 0xFF);
            ++buf;
            ++src;
        }
        *buf = '\0';
    }
    return len;
}
size_t UTF16BEToUTF16(wchar_t *buf, const wchar_t *src, size_t len) {
    return UTFCodeUtil::UTF16ToUTF16BE(buf, src, len);
}

} // namespace UTFCodeUtil
