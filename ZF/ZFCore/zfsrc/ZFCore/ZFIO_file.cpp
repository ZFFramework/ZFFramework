#include "ZFIO_file.h"

#include "protocol/ZFProtocolZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFIO_DEFINE(file, _ZFP_I_ZFIOImpl_file)

zfclass _ZFP_I_ZFIOToken_file : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_file, ZFIOToken)
public:
    zfstring _pathData;
    ZFIOOpenOptionFlags _flags;
    void *_ioToken;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _ioToken = zfnull;
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_file();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return _pathData;
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return _flags;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_ioToken) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        _pathData = zfnull;
        void *ioToken = _ioToken;
        _ioToken = zfnull;
        ZFFileClose(ioToken);
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {return ZFFileRead(_ioToken, buf, maxByteSize);}
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {return ZFFileWrite(_ioToken, src, maxByteSize);}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {return ZFFileSeek(_ioToken, byteSize, seekPos);}
    zfoverride
    virtual zfindex ioTell(void) {return ZFFileTell(_ioToken);}
    zfoverride
    virtual zfindex ioSize(void) {return ZFFileSize(_ioToken);}
};

zfautoT<ZFIOToken> _ZFP_I_ZFIOImpl_file::ioOpen(
        ZF_IN const zfstring &pathData
        , ZF_IN ZFIOOpenOptionFlags flags
        , ZF_IN_OPT zfbool autoCreateParent /* = zftrue */
        ) {
    void *token = ZFFileOpen(pathConvert(pathData), flags, autoCreateParent);
    if(token == zfnull) {
        return zfnull;
    }
    zfobj<_ZFP_I_ZFIOToken_file> ioToken;
    ioToken->_pathData = pathData;
    ioToken->_flags = flags;
    ioToken->_ioToken = token;
    return ioToken;
}

// ============================================================
// ZFInputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForFile
        , ZFMP_IN(const zfstring &, filePath)
        )

// ============================================================
// ZFOutputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFOutput, ZFOutputForFile
        , ZFMP_IN(const zfstring &, filePath)
        )

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsExist
        , ZFMP_IN(const zfstring &, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileIsExist(path);
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileIsDir
        , ZFMP_IN(const zfstring &, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->isDir(path);
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathCreate
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    if(*path == '\0') {
        return zftrue;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->filePathCreate(path, autoCreateParent);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileRemove
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileRemove(path, isRecursive, isForce);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileMove
        , ZFMP_IN(const zfstring &, srcPath)
        , ZFMP_IN(const zfstring &, dstPath)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(srcPath == zfnull || dstPath == zfnull) {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileMove(srcPath, dstPath, isForce);
}

#define _ZFP_ZFIOFindType_file "ZFFileFindFirst"

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileFindFirst
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        , ZFMP_IN(const zfstring &, path)
        ) {
    if(path == zfnull) {
        return zffalse;
    }
    fd.implAttach(_ZFP_ZFIOFindType_file);
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindFirst(fd.impl(), path)) {
        fd.implDetach();
        return zffalse;
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFindNext
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    fd.implCheck(_ZFP_ZFIOFindType_file);
    return ZFPROTOCOL_ACCESS(ZFFile)->fileFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileFindClose
        , ZFMP_IN_OUT(ZFIOFindData &, fd)
        ) {
    fd.implCheck(_ZFP_ZFIOFindType_file);
    ZFPROTOCOL_ACCESS(ZFFile)->fileFindClose(fd.impl());
    fd.implDetach();
}

ZFMETHOD_FUNC_DEFINE_3(void *, ZFFileOpen
        , ZFMP_IN(const zfstring &, filePath)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        ) {
    if(autoCreateParent && (zffalse
                || ZFBitTest(flags, v_ZFIOOpenOption::e_Write)
                || ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)
                )) {
        zfstring parentPath;
        if(ZFPathParentOfT(parentPath, filePath)) {
            ZFFilePathCreate(parentPath);
        }
    }
    return ZFPROTOCOL_ACCESS(ZFFile)->fileOpen(filePath, flags);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileClose
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileClose(token);
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
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileSeek
        , ZFMP_IN(void *, token)
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN_OPT(ZFSeekPos, seekPos, ZFSeekPosBegin)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileSeek(token, byteSize, seekPos);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileTell
        , ZFMP_IN(void *, token)
        ) {
    return ZFPROTOCOL_ACCESS(ZFFile)->fileTell(token);
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

