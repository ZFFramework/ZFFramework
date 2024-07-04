#include "ZFCoreStringUtil.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex zfsCheckMatch(
        ZF_IN const zfchar **tokens
        , ZF_IN zfindex tokenCount
        , ZF_IN const zfchar *toCompare
        , ZF_IN_OPT zfindex toCompareLength /* = zfindexMax() */
        ) {
    if(toCompare == zfnull) {
        return zfindexMax();
    }

    zfindex tmpLen = 0;
    for(zfindex i = 0; i < tokenCount; ++i) {
        tmpLen = zfslen(tokens[i]);
        if(tmpLen <= toCompareLength && zfsncmp(tokens[i], toCompare, tmpLen) == 0) {
            return i;
        }
    }
    return zfindexMax();
}

// ============================================================
zfindex zfstringFind(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen) {
            const zfchar *p = src;
            const zfchar *pEnd = src + srcLen - findLen;
            do {
                if(zfsncmp(p, find, findLen) == 0) {
                    return (p - src);
                }
                zfcharMoveNext(p);
            } while(p <= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen) {
            const zfchar *p = src + srcLen - findLen;
            const zfchar *pEnd = src;
            do {
                if(zfsncmp(p, find, findLen) == 0) {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindCaseInsensitive(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen) {
            const zfchar *p = src;
            const zfchar *pEnd = src + srcLen - findLen;
            do {
                if(zfsnicmp(p, find, findLen) == 0) {
                    return (p - src);
                }
                zfcharMoveNext(p);
            } while(p <= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindCaseInsensitiveReversely(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen) {
            const zfchar *p = src + srcLen - findLen;
            const zfchar *pEnd = src;
            do {
                if(zfsnicmp(p, find, findLen) == 0) {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindFirstOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        const zfchar *p = src;
        const zfchar *pEnd = src + srcLen;
        while(p < pEnd) {
            for(zfindex i = 0; i < findLen; ++i) {
                if(*p == find[i]) {
                    return (p - src);
                }
            }
            zfcharMoveNext(p);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindFirstNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        const zfchar *p = src;
        const zfchar *pEnd = src + srcLen;
        while(p < pEnd) {
            zfindex i = 0;
            for( ; i < findLen; ++i) {
                if(*p == find[i]) {
                    break;
                }
            }
            if(i >= findLen) {
                return (p - src);
            }
            zfcharMoveNext(p);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindLastOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        const zfchar *p = src + srcLen - 1;
        const zfchar *pEnd = src - 1;
        while(p > pEnd) {
            for(zfindex i = 0; i < findLen; ++i) {
                if(*p == find[i]) {
                    return (p - src);
                }
            }
            --p;
        }
    }
    return zfindexMax();
}
zfindex zfstringFindLastNotOf(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(src && find) {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax()) {
            findLen = zfslen(find);
        }
        const zfchar *p = src + srcLen - 1;
        const zfchar *pEnd = src - 1;
        while(p > pEnd) {
            zfindex i = 0;
            for( ; i < findLen; ++i) {
                if(*p == find[i]) {
                    break;
                }
            }
            if(i >= findLen) {
                return (p - src);
            }
            --p;
        }
    }
    return zfindexMax();
}
zfindex zfstringReplace(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        ) {
    zfindex replacedCount = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex replaceToLen = zfslen(replaceTo);
    zfindex pos = 0;
    zfindex posStart = pos;
    while(maxCount == zfindexMax() || replacedCount < maxCount) {
        pos = zfstringFind(s + posStart, s.length() - posStart, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        s.replace(posStart + pos, replaceFromLen, replaceTo);
        posStart = posStart + pos + replaceToLen;
        ++replacedCount;
    }
    return replacedCount;
}
zfindex zfstringReplaceReversely(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        ) {
    zfindex replacedCount = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex pos = s.length();
    zfindex posStart = pos;
    while(maxCount == zfindexMax() || replacedCount < maxCount) {
        pos = zfstringFindReversely(s, posStart, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        s.replace(pos, replaceFromLen, replaceTo);
        posStart = pos;
        ++replacedCount;
    }
    return replacedCount;
}

// ============================================================
// zfstringSplit
void zfstringSplitT(
        ZF_IN_OUT ZFCoreArray<zfstring> &ret
        , ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty /* = zffalse */
        ) {
    if(src != zfnull && separator != zfnull && *separator) {
        zfindex len = zfslen(separator);
        const zfchar *srcEnd = src + zfslen(src);
        if(src <= srcEnd - len) {
            const zfchar *p = src;
            while(p < srcEnd) {
                if(zfsncmp(p, separator, len) == 0) {
                    if(p > src || keepEmpty) {
                        ret.add(zfstring(src, p - src));
                        src = p;
                    }
                    p += len;
                    src = p;

                    if(p == srcEnd && keepEmpty) {
                        ret.add(zfstring());
                    }
                }
                else {
                    ++p;
                }
            }
            if(p > src) {
                ret.add(zfstring(src, p - src));
            }
        }
        else if(*src || keepEmpty) {
            ret.add(src);
        }
    }
    else if(src != zfnull) {
        if(*src || keepEmpty) {
            ret.add(src);
        }
    }
}
void zfstringSplitIndexT(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfchar *src
        , ZF_IN const zfchar *separator
        , ZF_IN_OPT zfbool keepEmpty /* = zffalse */
        ) {
    const zfchar *srcOrig = src;
    if(src != zfnull && separator != zfnull && *separator) {
        zfindex len = zfslen(separator);
        const zfchar *srcEnd = src + zfslen(src);
        if(src <= srcEnd - len) {
            const zfchar *p = src;
            while(p < srcEnd) {
                if(zfsncmp(p, separator, len) == 0) {
                    if(p > src || keepEmpty) {
                        ret.add(ZFIndexRangeMake(src - srcOrig, p - src));
                        src = p;
                    }
                    p += len;
                    src = p;

                    if(p == srcEnd && keepEmpty) {
                        ret.add(ZFIndexRangeMake(srcEnd - srcOrig, 0));
                    }
                }
                else {
                    ++p;
                }
            }
            if(p > src) {
                ret.add(ZFIndexRangeMake(src - srcOrig, p - src));
            }
        }
        else if(*src || keepEmpty) {
            ret.add(ZFIndexRangeMake(src - srcOrig, srcEnd - src));
        }
    }
    else if(src != zfnull) {
        if(*src || keepEmpty) {
            ret.add(ZFIndexRangeMake(0, zfslen(src)));
        }
    }
}

// ============================================================
// other
void zfstringToLowerT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(zfstringIsEmpty(src)) {
        return;
    }
    ;
    for(const zfchar *srcEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen); src < srcEnd; ++src) {
        if(*src >= 'A' && *src <= 'Z') {
            ret += (zfchar)(*src + 32);
        }
        else {
            ret += *src;
        }
    }
}
void zfstringToUpperT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(zfstringIsEmpty(src)) {
        return;
    }
    for(const zfchar *srcEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen); src < srcEnd; ++src) {
        if(*src >= 'a' && *src <= 'z') {
            ret += (zfchar)(*src - 32);
        }
        else {
            ret += *src;
        }
    }
}

zfindex _ZFP_zfstringSwitch(
        ZF_IN const zfchar *v
        , ZF_IN const zfchar *c0
        , ...
        ) {
    if(zfstringIsEmpty(v) || zfstringIsEmpty(c0)) {
        return zfindexMax();
    }

    typedef zfstlmap<const zfchar *, zfindex, zfcharConst_zfstlLess> MapType;
    MapType m;
    zfindex p = 0;
    m[c0] = p++;
    va_list vaList;
    va_start(vaList, c0);
    const zfchar *c = va_arg(vaList, const zfchar *);
    while(!zfstringIsEmpty(c)) {
        m[c] = p++;
        c = va_arg(vaList, const zfchar *);
    }
    va_end(vaList);

    MapType::iterator it = m.find(v);
    if(it == m.end()) {
        return zfindexMax();
    }
    else {
        return it->second;
    }
}

ZF_NAMESPACE_GLOBAL_END

