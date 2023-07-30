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
            do {
                ++p;
            } while(p < srcEnd && *p == '/');
            if(ret.isEmpty() || ret[ret.length() - 1] != ZFFileSeparator()) {
                ret += ZFFileSeparator();
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
                if(ret.isEmpty() || ret[ret.length() - 1] != ZFFileSeparator()) {
                    ret += ZFFileSeparator();
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
        if(last == '/' || last == ' ' || last == '\t') {
            ret.remove(ret.length() - 1);
        }
        else {
            break;
        }
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathFormat
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

ZFMETHOD_FUNC_DEFINE_1(void, ZFPathFormatRelative
        , ZFMP_IN_OUT(zfstring &, ret)
        ) {
    if(ret.isEmpty()) {
        return;
    }

    zfindex p = 0;
    do {
        if(p + 2 >= ret.length()) {
            return;
        }
        if(!(ret[p] == ZFFileSeparator() && ret[p + 1] == '.' && ret[p + 2] == '.')) {
            ++p;
            continue;
        }
        if(!(p + 3 >= ret.length() || ret[p + 3] == ZFFileSeparator())) {
            p += 4;
            continue;
        }
        zfindex pL = zfstringFindReversely(ret.cString(), p, ZFFileSeparatorString());
        if(pL == zfindexMax()) {
            if(p == 3 && ret[0] == '.' && ret[1] == '.') {
                return;
            }
            if(p + 3 >= ret.length()) {
                ret.removeAll();
                return;
            }
            else {
                ret.remove(0, p + 4);
            }
            p = 0;
            continue;
        }
        if(p + 3 >= ret.length()) {
            ret.remove(pL);
            return;
        }
        else {
            ret.remove(pL, p + 3 - pL);
        }
        p = pL;
    } while(zftrue);
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileNameOf
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        ) {
    if(zfstringIsEmpty(src) || zfstringIsEqual(src, ".")) {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
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
    zfindex pos = zfstringFindReversely(src, len, ZFFileSeparatorString());
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
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
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
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
    if(src >= ret.cString() && src < ret.cString() + ret.length()) {
        if(pos != zfindexMax() && !(pos >= 3 && src[pos-1] == ZFFileSeparator() && src[pos-2] == ':')) {
            ret.remove(pos);
            return zftrue;
        }
        else {
            ret.removeAll();
            return zffalse;
        }
    }
    else {
        if(pos != zfindexMax() && !(pos >= 3 && src[pos-1] == ZFFileSeparator() && src[pos-2] == ':')) {
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
        posL = zfstringFindReversely(src, posR, ZFFileSeparatorString());
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
    if(fileImpl.callbackFindFirst(fd, pathData)) {
        do {
            if(headToken != zfnull) {
                outputCallback << headToken;
            }
            if(indentToken != zfnull) {
                for(zfindex i = 0; i < indentLevel; i++) {
                    outputCallback << indentToken;
                }
            }

            if(fd.fileIsDir()) {
                outputCallback << fd.fileName() << "/\n";
                zfstring pathDataChild;
                if(fileImpl.callbackToChild(pathData, pathDataChild, fd.fileName())) {
                    _ZFP_ZFFileTreePrint(pathDataChild, outputCallback, headToken, indentToken, indentLevel + 1, fileImpl);
                }
            }
            else {
                outputCallback << fd.fileName();
                outputCallback << "\n";
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_4(void, ZFPathInfoTreePrint
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        , ZFMP_IN_OPT(const zfchar *, headToken, zfnull)
        , ZFMP_IN_OPT(const zfchar *, indentToken, "  ")
        ) {
    const ZFPathInfoImpl *data = ZFPathInfoImplForPathType(pathInfo.pathType);
    if(data != zfnull
            && data->callbackFindFirst
            && data->callbackFindNext
            && data->callbackFindClose
            ) {
        _ZFP_ZFFileTreePrint(pathInfo.pathData, outputCallback, headToken, indentToken, 0, *data);
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
        zfblockedAlloc(v_ZFFileFindData, fd);
        if(impl.callbackFindFirst(fd->zfv, pathInfo->zfv.pathData)) {
            do {
                if(!impl.callbackToChild(pathInfo->zfv.pathData, pathInfo->zfv.pathData, fd->zfv.fileName())) {
                    return zffalse;
                }

                if((fd->zfv.fileIsDir() && forEachDir) || (!fd->zfv.fileIsDir() && forEachFile)) {
                    fileCallback.execute(ZFArgs()
                            .param0(pathInfo)
                            .param1(fd)
                        );
                }
                if(isRecursive && fd->zfv.fileIsDir()) {
                    action(impl, fileCallback, pathInfo);
                }

                if(!impl.callbackToParent(pathInfo->zfv.pathData, pathInfo->zfv.pathData)) {
                    return zffalse;
                }
            } while(impl.callbackFindNext(fd->zfv));
            impl.callbackFindClose(fd->zfv);
        }
        return zftrue;
    }
};
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForEach
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType);
    if(impl == zfnull) {
        return zffalse;
    }
    zfblockedAlloc(v_ZFPathInfo, pathInfoHolder);
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
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType);
    if(impl == zfnull) {
        return zffalse;
    }
    zfblockedAlloc(v_ZFPathInfo, pathInfoHolder);
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
    const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType);
    if(impl == zfnull) {
        return zffalse;
    }
    zfblockedAlloc(v_ZFPathInfo, pathInfoHolder);
    pathInfoHolder->zfv = pathInfo;
    if(isRecursive) {
        return _ZFP_ZFPathInfoForEach<true, false, true>::action(*impl, fileCallback, pathInfoHolder);
    }
    else {
        return _ZFP_ZFPathInfoForEach<false, false, true>::action(*impl, fileCallback, pathInfoHolder);
    }
}

ZF_NAMESPACE_GLOBAL_END

