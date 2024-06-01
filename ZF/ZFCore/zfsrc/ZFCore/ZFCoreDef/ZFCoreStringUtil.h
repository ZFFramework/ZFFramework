/**
 * @file ZFCoreStringUtil.h
 * @brief string utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreStringUtil_h_
#define _ZFI_ZFCoreStringUtil_h_

#include "ZFCoreArray.h"
#include "ZFCoreStringConvert.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to copy the contents of src,
 *   return null if src null, or empty string if src is empty string
 */
inline zfchar *zfsCopy(ZF_IN const zfchar *src) {
    zfchar *ret = zfnull;
    if(src) {
        zfindex size = sizeof(zfchar) * (zfslen(src) + 1);
        ret = (zfchar *)zfmalloc(size);
        zfmemcpy(ret, src, size);
    }
    return ret;
}
/**
 * @brief util method to copy the contents of src,
 *   return null if src null, or empty string if src is empty string
 */
inline zfchar *zfsCopy(
        ZF_IN const zfchar *src
        , ZF_IN zfindex size
        ) {
    zfchar *ret = zfnull;
    if(src) {
        if(size == zfindexMax()) {
            size = zfslen(src);
        }
        ret = (zfchar *)zfmalloc(sizeof(zfchar) * (size + 1));
        zfmemcpy(ret, src, sizeof(zfchar) * size);
        ret[size] = '\0';
    }
    return ret;
}
/**
 * @brief util method to free old string then copy and change to new string,
 *   free and set dst to null if src is null,
 *   otherwise, realloc and copy contents to dst
 */
inline zfchar *zfsChange(
        ZF_IN_OUT zfchar *&dst
        , ZF_IN const zfchar *src
        ) {
    if(src) {
        zfindex size = sizeof(zfchar) * (zfslen(src) + 1);
        dst = (zfchar *)zfrealloc(dst, size);
        zfmemcpy(dst, src, size);
    }
    else {
        zffree(dst);
        dst = zfnull;
    }
    return dst;
}
/**
 * @brief util method to free old string then copy and change to new string,
 *   free and set dst to null if src is null,
 *   otherwise, realloc and copy contents to dst
 */
inline zfchar *zfsChange(
        ZF_IN_OUT zfchar *&dst
        , ZF_IN const zfchar *src
        , ZF_IN zfindex size
        ) {
    if(!src) {
        zffree(dst);
        dst = zfnull;
        return dst;
    }

    if(size == zfindexMax()) {
        size = zfslen(src);
    }
    dst = (zfchar *)zfrealloc(dst, sizeof(zfchar) * (size + 1));
    zfmemcpy(dst, src, sizeof(zfchar) * size);
    dst[size] = '\0';
    return dst;
}
/**
 * @brief util method to append string to existing string buffer, realloc if need
 */
inline zfchar *zfsAppend(
        ZF_IN_OUT zfchar *&old
        , ZF_IN const zfchar *append
        ) {
    if(!old) {
        old = zfsCopy(append);
        return old;
    }

    zfindex oldSize = zfslen(old);
    zfindex size = sizeof(zfchar) * (oldSize + zfslen(append) + 1);
    old = (zfchar *)zfrealloc(old, size);
    zfmemcpy(old + oldSize, append, size);
    return old;
}
/**
 * @brief util method to append string to existing string buffer, realloc if need
 */
inline zfchar *zfsAppend(
        ZF_IN_OUT zfchar *&old
        , ZF_IN const zfchar *append
        , ZF_IN zfindex size
        ) {
    if(!old) {
        old = zfsCopy(append, size);
        return old;
    }

    zfindex oldSize = zfslen(old);
    if(size == zfindexMax()) {
        size = zfslen(append);
    }
    old = (zfchar *)zfrealloc(old, sizeof(zfchar) * (oldSize + size + 1));
    zfmemcpy(old + oldSize, append, sizeof(zfchar) * size);
    old[oldSize + size] = '\0';
    return old;
}

#define _ZFP_zfsConnectEndPtr ((void *)-1)
inline zfchar *_ZFP_zfsConnect(ZF_IN const zfchar *src, ...) {
    zfindex len = 0;
    va_list vaList;
    const zfchar *tmp = src;

    va_start(vaList, src);
    do {
        if(tmp != zfnull) {
            len += zfslen(tmp);
        }
        tmp = va_arg(vaList, const zfchar *);
    } while(tmp != _ZFP_zfsConnectEndPtr);
    va_end(vaList);

    zfchar *ret = (zfchar *)zfmalloc(sizeof(zfchar) * (len + 1));
    tmp = src;
    len = 0;
    zfindex tmpLen = 0;
    va_start(vaList, src);
    do {
        if(tmp != zfnull) {
            tmpLen = zfslen(tmp);
            zfmemcpy(ret + len, tmp, sizeof(zfchar) * tmpLen);
            len += tmpLen;
        }
        tmp = va_arg(vaList, const zfchar *);
    } while(tmp != _ZFP_zfsConnectEndPtr);
    va_end(vaList);
    ret[len] = '\0';

    return ret;
}
/**
 * @brief connect multiple strings and return a newly created string,
 *   result must be deleted by #zffree
 *
 * null string would be treated as empty string
 */
#define zfsConnect(src, ...) \
    ZFM_VA_APPEND(_ZFP_zfsConnect, _ZFP_zfsConnectEndPtr, src, ##__VA_ARGS__)

// ============================================================
/**
 * @brief check whether the toCompare matches the tokens, return the index in tokens or zfindexMax() if not matched
 */
extern ZFLIB_ZFCore zfindex zfsCheckMatch(
        ZF_IN const zfchar **tokens
        , ZF_IN zfindex tokenCount
        , ZF_IN const zfchar *toCompare
        , ZF_IN_OPT zfindex toCompareLength = zfindexMax()
        );

// ============================================================
/**
 * @brief util to compare two c strings,
 *   while null is regarded as equal to empty string
 */
inline zfbool zfstringIsEqual(
        ZF_IN const zfchar *s1
        , ZF_IN const zfchar *s2
        ) {
    if(s1 == s2) {
        return zftrue;
    }
    else if(!s1) {
        return (*s2 == '\0');
    }
    else if(!s2) {
        return (*s1 == '\0');
    }
    else {
        return (zfscmp(s1, s2) == 0);
    }
}
/**
 * @brief util to check whether string is empty (null or empty string)
 */
inline zfbool zfstringIsEmpty(ZF_IN const zfchar *s) {
    return (s == zfnull || *s == '\0');
}
/**
 * @brief util to check whether string is empty (null or empty string)
 */
inline zfbool zfstringIsEmpty(ZF_IN const zfchar *s, ZF_IN zfindex len) {
    return (s == zfnull || *s == '\0' || len == 0);
}

// ============================================================
// zfstringFind
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFind(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFind(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFind(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFind(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFind(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFind(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFind(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindReversely
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindReversely(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindReversely(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindReversely(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindReversely(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindCaseInsensitive
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindCaseInsensitive(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitive(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindCaseInsensitive(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindCaseInsensitiveReversely
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindCaseInsensitiveReversely(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitiveReversely(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindCaseInsensitiveReversely(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindFirstOf
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindFirstOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindFirstOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindFirstOf(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindFirstOf(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstOf(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstOf(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindFirstOf(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindFirstNotOf
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindFirstNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindFirstNotOf(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstNotOf(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindFirstNotOf(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindLastOf
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindLastOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindLastOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindLastOf(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindLastOf(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastOf(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastOf(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindLastOf(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringFindLastNotOf
/** @brief find string */
extern ZFLIB_ZFCore zfindex zfstringFindLastNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfindex zfstringFindLastNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN zfchar find
        ) {
    return zfstringFindLastNotOf(src, srcLen, &find, 1);
}
/** @brief find string */
inline zfindex zfstringFindLastNotOf(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastNotOf(src, src ? zfslen(src) : 0, find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfchar find
        ) {
    return zfstringFindLastNotOf(src, src ? zfslen(src) : 0, &find, 1);
}
// ============================================================
// zfstringReplace
/** @brief replace string, return replaced count */
extern ZFLIB_ZFCore zfindex zfstringReplace(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        );
/** @brief replace string, return replaced count */
extern ZFLIB_ZFCore zfindex zfstringReplaceReversely(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        );

// ============================================================
// zfstringSplit
/** @brief split string */
extern ZFLIB_ZFCore void zfstringSplitT(
        ZF_IN_OUT ZFCoreArray<zfstring> &ret
        , ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty = zffalse
        );
/** @brief split string */
inline ZFCoreArray<zfstring> zfstringSplit(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty = zffalse
        ) {
    ZFCoreArray<zfstring> ret;
    zfstringSplitT(ret, src, separator, keepEmpty);
    return ret;
}
/** @brief split string */
extern ZFLIB_ZFCore void zfstringSplitIndexT(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty = zffalse
        );
/** @brief split string */
inline ZFCoreArray<ZFIndexRange> zfstringSplitIndex(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty = zffalse
        ) {
    ZFCoreArray<ZFIndexRange> ret;
    zfstringSplitIndexT(ret, src, separator, keepEmpty);
    return ret;
}

// ============================================================
// other
/** @brief to lower case */
extern ZFLIB_ZFCore void zfstringToLowerT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );
/** @brief to lower case */
inline zfstring zfstringToLower(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    zfstring ret;
    zfstringToLowerT(ret, src, srcLen);
    return ret;
}
/** @brief to upper case */
extern ZFLIB_ZFCore void zfstringToUpperT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );
/** @brief to upper case */
inline zfstring zfstringToUpper(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    zfstring ret;
    zfstringToUpperT(ret, src, srcLen);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringUtil_h_

