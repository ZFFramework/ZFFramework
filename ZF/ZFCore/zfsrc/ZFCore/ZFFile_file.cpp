#include "ZFFile.h"

#include "protocol/ZFProtocolZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsExist
        , ZFMP_IN(const zfchar *, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileIsExist(path);
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsDir
        , ZFMP_IN(const zfchar *, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->isDir(path);
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFPathCreate
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    if(*path == '\0') {
        return zftrue;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->filePathCreate(path, autoMakeParent);
}

ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFileCopy
        , ZFMP_IN(const zfchar *, srcPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(srcPath == zfnull || dstPath == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileCopy(srcPath, dstPath, isRecursive, isForce);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileMove
        , ZFMP_IN(const zfchar *, srcPath)
        , ZFMP_IN(const zfchar *, dstPath)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(srcPath == zfnull || dstPath == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileMove(srcPath, dstPath, isForce);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileRemove
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileRemove(path, isRecursive, isForce);
}

#define _ZFP_ZFFileFindType_file "ZFFileFindFirst"

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileFindFirst
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        , ZFMP_IN(const zfchar *, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    fd.implAttach(_ZFP_ZFFileFindType_file);
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindFirst(fd.impl(), path)) {
        fd.implDetach();
        return zffalse;
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFindNext
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    fd.implCheck(_ZFP_ZFFileFindType_file);
    return ZFPROTOCOL_ACCESS(ZFFile)->fileFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileFindClose
        , ZFMP_IN_OUT(ZFFileFindData &, fd)
        ) {
    fd.implCheck(_ZFP_ZFFileFindType_file);
    ZFPROTOCOL_ACCESS(ZFFile)->fileFindClose(fd.impl());
    fd.implDetach();
}

ZFMETHOD_FUNC_DEFINE_3(void *, ZFFileOpen
        , ZFMP_IN(const zfchar *, filePath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, v_ZFFileOpenOption::e_Read)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    if(autoCreateParent && (zffalse
                || ZFBitTest(flag, v_ZFFileOpenOption::e_Create)
                || ZFBitTest(flag, v_ZFFileOpenOption::e_Write)
                || ZFBitTest(flag, v_ZFFileOpenOption::e_Append)
                )) {
        zfstring parentPath;
        if(ZFPathParentOfT(parentPath, filePath)) {
            ZFPathCreate(parentPath);
        }
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileOpen(filePath, flag);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileClose
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileClose(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileTell
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileTell(token);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileSeek(token, byteSize, position);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileRead
        , ZFMP_IN(void *, token)
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, maxByteSize)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFile) *impl = ZFPROTOCOL_ACCESS(ZFFile);
    if(buf == zfnull) {
        zfindex curPos = impl->fileTell(token);
        impl->fileSeek(token, 0, ZFSeekPosEnd);
        zfindex endPos = impl->fileTell(token);
        impl->fileSeek(token, curPos, ZFSeekPosBegin);
        return endPos - curPos;
    }
    if(maxByteSize == zfindexMax()) {
        return 0;
    }
    return impl->fileRead(token, buf, maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileWrite
        , ZFMP_IN(void *, token)
        , ZFMP_IN(const void *, src)
        , ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax())
        ) {
    if(src == zfnull) {
        return 0;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileWrite(token, src,
        (maxByteSize == zfindexMax()) ? zfslen((const zfchar *)src) : maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileFlush
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileFlush(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsEof
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileIsEof(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsError
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileIsError(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileSize
        , ZFMP_IN(void *, token)
        ) {
    if(token == zfnull) {
        return zfindexMax();
    }
    zfindex saved = ZFFileTell(token);
    if(saved == zfindexMax()) {
        return zfindexMax();
    }
    if(!ZFFileSeek(token, 0, ZFSeekPosEnd)) {
        return zfindexMax();
    }
    zfindex size = ZFFileTell(token);
    ZFFileSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_GLOBAL_END

