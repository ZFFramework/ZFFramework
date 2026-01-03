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
    if(toCompareLength == zfindexMax()) {
        toCompareLength = zfslen(toCompare);
    }
    for(zfindex i = 0; i < tokenCount; ++i) {
        if(zfstringIsEqual(tokens[i], zfslen(tokens[i]), toCompare, toCompareLength)) {
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
zfstring zfstringReplace(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_OUT_OPT zfindex *replacedCount /* = zfnull */
        ) {
    zfindex srcLen = zfslen(src);
    zfindex replacedCountTmp = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex replaceToLen = zfslen(replaceTo);
    zfindex posStart = 0;
    zfstring ret;
    while(maxCount == zfindexMax() || replacedCountTmp < maxCount) {
        zfindex pos = zfstringFind(src + posStart, srcLen - posStart, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        ret.append(src + posStart, pos);
        ret.append(replaceTo, replaceToLen);
        posStart += pos + replaceFromLen;
        ++replacedCountTmp;
    }
    if(posStart < srcLen) {
        ret.append(src + posStart, srcLen - posStart);
    }
    if(replacedCount != zfnull) {
        *replacedCount = replacedCountTmp;
    }
    return ret;
}
zfstring zfstringReplaceReversely(
        ZF_IN const zfchar *src
        , ZF_IN const zfchar *replaceFrom
        , ZF_IN const zfchar *replaceTo
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_OUT_OPT zfindex *replacedCount /* = zfnull */
        ) {
    zfindex srcLen = zfslen(src);
    zfindex replacedCountTmp = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex replaceToLen = zfslen(replaceTo);
    zfindex posStart = srcLen;
    zfstring ret;
    while(maxCount == zfindexMax() || replacedCountTmp < maxCount) {
        zfindex pos = zfstringFindReversely(src, posStart, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        ret.insert(0, src + pos + replaceFromLen, posStart - pos - replaceFromLen);
        ret.insert(0, replaceTo, replaceToLen);
        posStart = pos;
        ++replacedCountTmp;
    }
    if(posStart > 0) {
        ret.insert(0, src, posStart);
    }
    if(replacedCount != zfnull) {
        *replacedCount = replacedCountTmp;
    }
    return ret;
}


// ============================================================
// zfstringBeginWith
zfbool zfstringBeginWith(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(findLen == zfindexMax()) {
        findLen = zfslen(find);
    }
    return zfsncmp(src, find, findLen) == 0;
}
// ============================================================
// zfstringEndWith
zfbool zfstringEndWith(
        ZF_IN const zfchar *src
        , ZF_IN zfindex srcLen
        , ZF_IN const zfchar *find
        , ZF_IN_OPT zfindex findLen /* = zfindexMax() */
        ) {
    if(srcLen == zfindexMax()) {
        srcLen = zfslen(src);
    }
    if(findLen == zfindexMax()) {
        findLen = zfslen(find);
    }
    return srcLen >= findLen && zfsncmp(src + srcLen - findLen, find, findLen) == 0;
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
                        ret.add(ZFIndexRangeCreate(src - srcOrig, p - src));
                        src = p;
                    }
                    p += len;
                    src = p;

                    if(p == srcEnd && keepEmpty) {
                        ret.add(ZFIndexRangeCreate(srcEnd - srcOrig, 0));
                    }
                }
                else {
                    ++p;
                }
            }
            if(p > src) {
                ret.add(ZFIndexRangeCreate(src - srcOrig, p - src));
            }
        }
        else if(*src || keepEmpty) {
            ret.add(ZFIndexRangeCreate(src - srcOrig, srcEnd - src));
        }
    }
    else if(src != zfnull) {
        if(*src || keepEmpty) {
            ret.add(ZFIndexRangeCreate(0, zfslen(src)));
        }
    }
}

// ============================================================
// other
void zfstringToLowerT(ZF_IN_OUT zfstring &ret) {
    for(zfindex i = 0; i < ret.length(); i += zfcharGetSize(ret + i)) {
        if(ret.get(i) >= 'A' && ret.get(i) <= 'Z') {
            ret.set(i, ret.get(i) + 32);
        }
    }
}
void zfstringToLowerT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {
        return;
    }
    if(srcLen == zfindexMax()) {
        srcLen = zfslen(src);
    }
    for(zfindex i = 0; i < srcLen; ++i) {
        if(src[i] >= 'A' && src[i] <= 'Z') {
            ret.append((zfchar)(src[i] + 32));
        }
        else {
            ret.append(src[i]);
        }
    }
}
void zfstringToUpperT(ZF_IN_OUT zfstring &ret) {
    for(zfindex i = 0; i < ret.length(); i += zfcharGetSize(ret + i)) {
        if(ret.get(i) >= 'a' && ret.get(i) <= 'z') {
            ret.set(i, ret.get(i) - 32);
        }
    }
}
void zfstringToUpperT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {
        return;
    }
    if(srcLen == zfindexMax()) {
        srcLen = zfslen(src);
    }
    for(zfindex i = 0; i < srcLen; ++i) {
        if(src[i] >= 'a' && src[i] <= 'z') {
            ret.append((zfchar)(src[i] - 32));
        }
        else {
            ret.append(src[i]);
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

