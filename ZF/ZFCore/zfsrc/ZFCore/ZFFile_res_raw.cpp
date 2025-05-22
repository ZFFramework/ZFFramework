#include "ZFFile.h"

#include "protocol/ZFProtocolZFRes.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFResRaw)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsExist(resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsDir
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsDir(resPath);
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFResCopy
        , ZFMP_IN(const zfchar *, resPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resCopy(resPath, dstPath, isRecursive, isForce);
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resFindFirst(fd.impl(), resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    ZFPROTOCOL_ACCESS(ZFRes)->resFindClose(fd.impl());
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFResOpen
        , ZFMP_IN(const zfchar *, resPath)
        ) {
    return ZFPROTOCOL_ACCESS(ZFRes)->resOpen(resPath);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResClose
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resClose(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResTell
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resTell(token);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFResSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resSeek(token, byteSize, position);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFResRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    if(token == zfnull) {
        return 0;
    }
    if(buf == zfnull) {
        ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
        zfindex curPos = impl->resTell(token);
        impl->resSeek(token, 0, ZFSeekPosEnd);
        zfindex endPos = impl->resTell(token);
        impl->resSeek(token, curPos, ZFSeekPosBegin);
        return endPos - curPos;
    }
    if(maxByteSize == zfindexMax()) {
        return 0;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resRead(token, buf, maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsEof
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsEof(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsError
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resIsError(token);
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
    zfindex saved = impl->resTell(token);
    impl->resSeek(token, 0, ZFSeekPosEnd);
    zfindex size = impl->resTell(token);
    impl->resSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_END(ZFResRaw)
ZF_NAMESPACE_GLOBAL_END

