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
        zfindex size = (zfslen(src) + 1);
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
        ret = (zfchar *)zfmalloc(size + 1);
        zfmemcpy(ret, src, size);
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
        zfindex size = zfslen(src) + 1;
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
    dst = (zfchar *)zfrealloc(dst, size + 1);
    zfmemcpy(dst, src, size);
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
    zfindex size = oldSize + zfslen(append) + 1;
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
    old = (zfchar *)zfrealloc(old, oldSize + size + 1);
    zfmemcpy(old + oldSize, append, size);
    old[oldSize + size] = '\0';
    return old;
}

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
 * @brief util to compare two c strings,
 *   while null is regarded as equal to empty string
 */
inline zfbool zfstringIsEqual(
        ZF_IN const zfchar *s1
        , ZF_IN zfindex s1Len
        , ZF_IN const zfchar *s2
        , ZF_IN zfindex s2Len
        ) {
    if(s1 == s2) {
        return zftrue;
    }
    else if(!s1 || s1Len == 0) {
        return (s2Len == 0 || *s2 == '\0');
    }
    else if(!s2) {
        return (s1Len == 0 || *s1 == '\0');
    }
    else {
        if(s1Len == zfindexMax()) {
            s1Len = zfslen(s1);
        }
        if(s2Len == zfindexMax()) {
            s2Len = zfslen(s2);
        }
        return (s1Len == s2Len && zfsncmp(s1, s2, s1Len) == 0);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFind(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFind(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFind(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindReversely(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindReversely(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindReversely(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitive(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitive(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitiveReversely(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindCaseInsensitiveReversely(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstOf(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstOf(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstOf(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstNotOf(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindFirstNotOf(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastOf(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastOf(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastOf(src, src.length(), find, findLen);
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
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastNotOf(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastNotOf(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringFindLastNotOf(src, src.length(), find, findLen);
}
// ============================================================
// zfstringReplace
/** @brief replace string */
extern ZFLIB_ZFCore zfstring zfstringReplace(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_OUT_OPT zfindex *replacedCount = zfnull
        );
/** @brief replace string */
extern ZFLIB_ZFCore zfstring zfstringReplaceReversely(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_OUT_OPT zfindex *replacedCount = zfnull
        );

// ============================================================
// zfstringBeginWith
/** @brief find string */
extern ZFLIB_ZFCore zfbool zfstringBeginWith(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfbool zfstringBeginWith(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringBeginWith(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfbool zfstringBeginWith(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringBeginWith(src, src.length(), find, findLen);
}
// ============================================================
// zfstringEndWith
/** @brief find string */
extern ZFLIB_ZFCore zfbool zfstringEndWith(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        );
/** @brief find string */
inline zfbool zfstringEndWith(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringEndWith(src, zfindexMax(), find, findLen);
}
/** @brief find string */
inline zfbool zfstringEndWith(
        ZF_IN const zfstring &src
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen = zfindexMax()
        ) {
    return zfstringEndWith(src, src.length(), find, findLen);
}

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
extern ZFLIB_ZFCore void zfstringToLowerT(ZF_IN_OUT zfstring &ret);
/** @brief to lower case */
inline zfstring zfstringToLower(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    zfstring ret(src, srcLen);
    zfstringToLowerT(ret);
    return ret;
}
/** @brief to upper case */
extern ZFLIB_ZFCore void zfstringToUpperT(ZF_IN_OUT zfstring &ret);
/** @brief to upper case */
inline zfstring zfstringToUpper(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        ) {
    zfstring ret(src, srcLen);
    zfstringToUpperT(ret);
    return ret;
}

/** @brief repeat string */
inline void zfstringRepeatT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *token
        , ZF_IN zfindex count
        ) {
    if(count != 0 && !zfstringIsEmpty(token)) {
        zfindex len = zfslen(token);
        for(zfindex i = 0; i < count; ++i) {
            ret.append(token, len);
        }
    }
}
/** @brief repeat string */
inline zfstring zfstringRepeat(
        ZF_IN const zfchar *token
        , ZF_IN zfindex count
        ) {
    zfstring ret;
    zfstringRepeatT(ret, token, count);
    return ret;
}

/**
 * @brief util to switch-case on string
 *
 * @code
 *   switch(zfstringSwitch(v
 *           , "111"
 *           , "222"
 *           )) {
 *       case 0:
 *           ZFLog("case 111");
 *           break;
 *       case 1:
 *           ZFLog("case 222");
 *           break;
 *       default:
 *           ZFLog("default");
 *           break;
 *   }
 * @endcode
 */
#define zfstringSwitch(v, c0, ...) _ZFP_zfstringSwitch(v, c0 \
        ZFM_FIX_PARAM(_ZFP_zfstringSwitchExpand, ZFM_EMPTY, ##__VA_ARGS__) \
        , zfnull)
#define _ZFP_zfstringSwitchExpand(arg) , ((const zfchar *)(arg))
extern ZFLIB_ZFCore zfindex _ZFP_zfstringSwitch(
        ZF_IN const zfchar *v
        , ZF_IN const zfchar *c0
        , ...
        );

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringUtil_h_

