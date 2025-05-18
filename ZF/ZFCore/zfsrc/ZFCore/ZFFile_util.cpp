#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFPathFormat(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *src
        , ZF_IN const zfchar *srcEnd
        ) {
    const zfchar *p = src;

    while(p < srcEnd) {
        if(*p == '/') {
            if(p > src && *(p-1) == ':' && *(p+1) == '/') {
                ret += "//";
                p += 2;
                while(p < srcEnd && *p == '/') {
                    ++p;
                }
            }
            else {
                do {
                    ++p;
                } while(p < srcEnd && *p == '/');
                if(ret.isEmpty() || ret[ret.length() - 1] != '/') {
                    ret += '/';
                }
            }
        }
        else if(*p == '\\') {
            ++p;
            if(*p == ' '
                    || *p == '"'
                    ) {
                ret += *p;
                ++p;
            }
            else {
                while(p < srcEnd && (*p == '/' || *p == '\\')) {
                    ++p;
                }
                if(ret.isEmpty() || ret[ret.length() - 1] != '/') {
                    ret += '/';
                }
            }
        }
        else if(*p == '.') {
            if(p == src || *(p-1) == '/') {
                if(p + 1 == srcEnd) {
                    if(p != src) { // "[xxx]/."
                        ret.remove(ret.length() - 1);
                        ++p;
                    }
                    else { // "."
                        ret += '.';
                    }
                    break;
                }
                else if(p[1] == '/'
                        || (p[1] == '\\' && p[1] != ' ' && p[1] != '"')
                        ) { // "[xxx]/./"
                    ++p;
                    while(p < srcEnd && (*p == '/' || *p == '\\')) {++p;}
                    continue;
                }
            }
            zfcharAppendAndMoveNext(ret, p);
        }
        else {
            zfcharAppendAndMoveNext(ret, p);
        }
    }
    while(!ret.isEmpty()) {
        zfchar last = ret[ret.length() - 1];
        if(last == '/') {
            if(ret.length() >= 3
                    && ret[ret.length() - 2] == '/'
                    && ret[ret.length() - 3] == ':'
                    ) {
                break;
            }
            ret.remove(ret.length() - 1);
        }
        else if(last == ' ' || last == '\t') {
            ret.remove(ret.length() - 1);
        }
        else {
            break;
        }
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathFormatT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        ) {
    if(src == zfnull) {return zffalse;}
    while(*src == ' ' || *src == '\t') {++src;}
    const zfchar *srcEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen);
    if(srcEnd == src) {return zftrue;}
    while(*(srcEnd-1) == ' ' || *(srcEnd - 1) == '\t') {--srcEnd;}
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        zfstring tmp;
        ret.swap(tmp);
        return _ZFP_ZFPathFormat(ret, tmp.cString(), tmp.cString() + tmp.length());
    }
    else {
        return _ZFP_ZFPathFormat(ret, src, srcEnd);
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfstring, ZFPathFormat
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathFormatRelativeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        ) {
    zfindex start = ret.length();
    if(src >= ret && src < ret + ret.length()) {
        ZFPathFormatT(ret, src, srcLen);
        if(ret.isEmpty()) {
            return zftrue;
        }
        start = 0;
    }
    else {
        ZFPathFormatT(ret, src, srcLen);
        if(ret.isEmpty()) {
            return zftrue;
        }
    }

    zfindex p = start;
    do {
        if(p + 2 >= ret.length()) {
            return zftrue;
        }
        if(!(ret[p] == '/' && ret[p + 1] == '.' && ret[p + 2] == '.')) {
            ++p;
            continue;
        }
        if(!(p + 3 >= ret.length() || ret[p + 3] == '/')) {
            p += 4;
            continue;
        }
        zfindex pL = zfstringFindReversely(ret.cString() + start, p - start, '/');
        if(pL == zfindexMax()) {
            if(ret[start] == '.' && ret[start + 1] == '.' && ret[start + 2] == '/') { // ../..
                p += 3;
            }
            else if(ret[p + 3] == '/') { // xx/../yy  => yy
                ret.remove(start, p + 4 - start);
                p = start;
            }
            else { // xx/..  =>
                ret.remove(start);
                p = start;
            }
        }
        else {
            if(p == pL + 3 && ret[pL + 1] == '.' && ret[pL + 2] == '.') { // xx/../..
                p += 3;
            }
            else if(ret[p + 3] == '/') { // xx/yy/../zz  => xx/zz
                ret.remove(pL, p + 3 - pL);
                p = pL;
            }
            else { // xx/yy/.. => xx
                ret.remove(pL);
                p = pL;
            }
        }
    } while(zftrue);
    return zftrue;
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfstring, ZFPathFormatRelative
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileNameOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), '/');
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        if(pos != zfindexMax()) {
            ret.remove(0, (src + pos + 1) - ret.cString());
        }
        else {
            // nothing to do
        }
        return zftrue;
    }
    else {
        if(pos != zfindexMax()) {
            ret += (src + pos + 1);
        }
        else {
            ret += src;
        }
        return zftrue;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileNameOf
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFFileNameOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileNameOfWithoutExt
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex len = zfslen(src);
    zfindex pos = zfstringFindReversely(src, len, '/');
    if(pos != zfindexMax()) {
        ++pos;
    }
    else {
        pos = 0;
    }
    zfindex dotPos = zfstringFindReversely(src + pos, len - pos, ".", 1);
    if(dotPos < pos) {
        dotPos = zfindexMax();
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        pos += src - ret.cString();
        if(dotPos != zfindexMax()) {
            ret.remove(src - ret.cString() + dotPos);
        }
        if(pos != 0) {
            ret.remove(0, pos);
        }
        return zftrue;
    }
    else {
        if(dotPos != zfindexMax()) {
            ret.append(src + pos, dotPos - pos);
        }
        else {
            ret += (src + pos);
        }
        return zftrue;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileNameOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFFileNameOfWithoutExt(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileExtOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), '/');
    zfindex dotPos = zfstringFindReversely(src, zfindexMax(), ".", 1);
    if(pos != zfindexMax() && dotPos < pos) {
        dotPos = zfindexMax();
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        if(dotPos != zfindexMax()) {
            ret.remove(0, (src + dotPos + 1) - ret.cString());
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
    else {
        if(dotPos != zfindexMax()) {
            ret += (src + dotPos + 1);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileExtOf
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFFileExtOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathOfWithoutExt
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src)) {
        return zffalse;
    }
    zfindex dotPos = zfslen(src) - 1;
    for( ; dotPos != zfindexMax(); --dotPos) {
        if(src[dotPos] == '.') {
            break;
        }
        else if(src[dotPos] == '/' || src[dotPos] == '\\') {
            dotPos = zfindexMax();
            break;
        }
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        if(dotPos != zfindexMax()) {
            ret.remove(src - ret.cString() + dotPos, zfslen(src) - dotPos);
        }
    }
    else {
        ret.append(src, dotPos);
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFPathOfWithoutExt
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFPathOfWithoutExt(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathParentOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), '/');
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        if(pos != zfindexMax() && !(pos >= 3 && src[pos-1] == '/' && src[pos-2] == ':')) {
            ret.remove(pos);
            return zftrue;
        }
        else {
            ret.removeAll();
            return zffalse;
        }
    }
    else {
        if(pos != zfindexMax() && !(pos >= 3 && src[pos-1] == '/' && src[pos-2] == ':')) {
            ret.append(src, pos);
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFPathParentOf
        , ZFMP_IN(const zfchar *, src)
        ) {
    zfstring ret;
    ZFPathParentOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathComponentsOf
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex len = zfslen(src);
    zfindex posL = 0;
    zfindex posR = len;
    do {
        posL = zfstringFindReversely(src, posR, '/');
        if(posL == zfindexMax()) {
            if(posR > 0) {
                ret.add(zfstring(src, posR));
            }
            break;
        }
        else {
            if(posL != posR - 1) {
                ret.add(zfstring(src, posL + 1, posR - posL - 1));
            }
            posR = posL;
        }
    } while(zftrue);
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(ZFCoreArray<zfstring>, ZFPathComponentsOf
        , ZFMP_IN(const zfchar *, src)
        ) {
    ZFCoreArray<zfstring> ret;
    ZFPathComponentsOf(ret, src);
    return ret;
}

// ============================================================
static void _ZFP_ZFFileTreePrint(
        ZF_IN const zfchar *pathData
        , ZF_IN const ZFOutput &outputCallback
        , ZF_IN const zfchar *headToken
        , ZF_IN const zfchar *indentToken
        , ZF_IN zfindex indentLevel
        , ZF_IN ZFPathInfoImpl const &fileImpl
        ) {
    ZFFileFindData fd;
    if(fileImpl.implFindFirst(fd, pathData)) {
        do {
            if(headToken != zfnull) {
                outputCallback << headToken;
            }
            if(indentToken != zfnull) {
                for(zfindex i = 0; i < indentLevel; i++) {
                    outputCallback << indentToken;
                }
            }

            if(fd.isDir()) {
                outputCallback << fd.name() << "/\n";
                zfstring pathDataChild = fileImpl.implToChild(pathData, fd.name());
                if(pathDataChild) {
                    _ZFP_ZFFileTreePrint(pathDataChild, outputCallback, headToken, indentToken, indentLevel + 1, fileImpl);
                }
            }
            else {
                outputCallback << fd.name();
                outputCallback << "\n";
            }
        } while(fileImpl.implFindNext(fd));
        fileImpl.implFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_4(void, ZFPathInfoTreePrint
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        , ZFMP_IN_OPT(const zfchar *, headToken, zfnull)
        , ZFMP_IN_OPT(const zfchar *, indentToken, "  ")
        ) {
    const ZFPathInfoImpl *data = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(data != zfnull) {
        _ZFP_ZFFileTreePrint(pathInfo.pathData(), outputCallback, headToken, indentToken, 0, *data);
    }
}

// ============================================================
template<bool isRecursive, bool forEachFile, bool forEachDir>
zfclassNotPOD _ZFP_ZFPathInfoForEach {
public:
    static zfbool action(
            ZF_IN const ZFPathInfoImpl &impl
            , ZF_IN const ZFListener &fileCallback
            , ZF_IN v_ZFPathInfo *pathInfo
            ) {
        zfobj<v_ZFFileFindData> fd;
        if(impl.implFindFirst(fd->zfv, pathInfo->zfv.pathData())) {
            do {
                pathInfo->zfv.pathData(impl.implToChild(pathInfo->zfv.pathData(), fd->zfv.name()));
                if(!pathInfo->zfv.pathData()) {
                    return zffalse;
                }

                if((fd->zfv.isDir() && forEachDir) || (!fd->zfv.isDir() && forEachFile)) {
                    ZFArgs zfargs;
                    fileCallback.execute(zfargs
                            .param0(pathInfo)
                            .param1(fd)
                        );
                    if(zfargs.eventFiltered()) {
                        break;
                    }
                }
                if(isRecursive && fd->zfv.isDir()) {
                    action(impl, fileCallback, pathInfo);
                }

                pathInfo->zfv.pathData(impl.implToParent(pathInfo->zfv.pathData()));
                if(!pathInfo->zfv.pathData()) {
                    return zffalse;
                }
            } while(impl.implFindNext(fd->zfv));
            impl.implFindClose(fd->zfv);
        }
        return zftrue;
    }
};
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForEach
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return zffalse;
    }
    zfobj<v_ZFPathInfo> pathInfoHolder;
    pathInfoHolder->zfv = pathInfo;
    if(isRecursive) {
        return _ZFP_ZFPathInfoForEach<true, true, true>::action(*impl, fileCallback, pathInfoHolder);
    }
    else {
        return _ZFP_ZFPathInfoForEach<false, true, true>::action(*impl, fileCallback, pathInfoHolder);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForEachFile
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return zffalse;
    }
    zfobj<v_ZFPathInfo> pathInfoHolder;
    pathInfoHolder->zfv = pathInfo;
    if(isRecursive) {
        return _ZFP_ZFPathInfoForEach<true, true, false>::action(*impl, fileCallback, pathInfoHolder);
    }
    else {
        return _ZFP_ZFPathInfoForEach<false, true, false>::action(*impl, fileCallback, pathInfoHolder);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForEachDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
    if(impl == zfnull) {
        return zffalse;
    }
    zfobj<v_ZFPathInfo> pathInfoHolder;
    pathInfoHolder->zfv = pathInfo;
    if(isRecursive) {
        return _ZFP_ZFPathInfoForEach<true, false, true>::action(*impl, fileCallback, pathInfoHolder);
    }
    else {
        return _ZFP_ZFPathInfoForEach<false, false, true>::action(*impl, fileCallback, pathInfoHolder);
    }
}

ZF_NAMESPACE_GLOBAL_END

