#include "ZFImpl_sys_Windows_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFile.h"
#include "ZFCore/zfstringW.h"
#include "ZFCore/ZFIO_file.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileImpl_sys_Windows, ZFFile, v_ZFProtocolLevel::e_SystemLow)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("nativeAPI")
public:
    virtual zfbool fileIsExist(ZF_IN const zfstring &path) {
        if(path == zfnull) {return zffalse;}
        return (GetFileAttributesW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString()) != 0xFFFFFFFF);
    }
    virtual zfbool isDir(ZF_IN const zfstring &path) {
        return ((GetFileAttributesW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString())
                    & FILE_ATTRIBUTE_DIRECTORY) != 0);
    }

    virtual zfbool filePathCreate(
            ZF_IN const zfstring &path
            , ZF_IN_OPT zfbool autoCreateParent = zffalse
            ) {
        if(autoCreateParent) {
            return this->makePath(path);
        }
        else {
            return this->makeDir(path);
        }
    }
private:
    zfbool cp_or_mv(
            ZF_IN zfbool isCopy
            , ZF_IN const zfstring &dstPath
            , ZF_IN const zfstring &srcPath
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            ) {
        if(srcPath == zfnull || dstPath == zfnull) {
            return zffalse;
        }
        if(!this->fileIsExist(srcPath)) {
            return zffalse;
        }
        zfbool srcIsDir = this->isDir(srcPath);
        if(srcIsDir && !isRecursive) {
            return zffalse;
        }
        zfbool dstExist = this->fileIsExist(dstPath);
        zfbool dstIsDir = this->isDir(dstPath);
        if(dstExist && srcIsDir != dstIsDir) {
            return zffalse;
        }

        if(!this->makePath(dstPath, !srcIsDir)) {
            return zffalse;
        }
        if(srcIsDir) {
            if(isCopy) {
                return this->copyDir(srcPath, dstPath, isForce);
            }
            else {
                if(isForce) {
                    this->removeDir(dstPath, isForce);
                }
                return this->moveDir(srcPath, dstPath);
            }
        }
        else {
            if(isCopy) {
                return this->copyFile(srcPath, dstPath, isForce);
            }
            else {
                if(isForce) {
                    this->removeFile(dstPath, isForce);
                }
                return this->moveFile(srcPath, dstPath);
            }
        }
    }
public:
    virtual zfbool fileMove(
            ZF_IN const zfstring &dstPath
            , ZF_IN const zfstring &srcPath
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return this->cp_or_mv(zffalse, dstPath, srcPath, zftrue, isForce);
    }
    virtual zfbool fileRemove(
            ZF_IN const zfstring &path
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        if(!this->fileIsExist(path)) {return zftrue;}
        if(this->isDir(path)) {
            if(!isRecursive) {
                return zffalse;
            }
            return this->removeDir(path, isForce);
        }
        else {
            return this->removeFile(path, isForce);
        }
    }

    zfclassNotPOD _ZFP_ZFFileNativeFd {
    public:
        zfstring parentPath;
        _ZFP_ZFFileNativeFd(void) {
            zfmemset(&this->fd, 0, sizeof(WIN32_FIND_DATAW));
            this->hFind = INVALID_HANDLE_VALUE;
        }
        WIN32_FIND_DATAW fd;
        HANDLE hFind;
        void setup(ZFIOFindData::Impl &zfd) {
            zfd.name.removeAll();
            zfstringToUTF8(zfd.name, fd.cFileName, v_ZFStringEncoding::e_UTF16);

            zfstring filePath = this->parentPath;
            filePath += '/';
            filePath += zfd.name;

            zfd.isDir = ZFFileIsDir(filePath);
        }
    };

    virtual zfbool fileFindFirst(
            ZF_IN_OUT ZFIOFindData::Impl &fd
            , ZF_IN const zfstring &path
            ) {
        if(path == zfnull) {return zffalse;}
        _ZFP_ZFFileNativeFd *nativeFd = zfnew(_ZFP_ZFFileNativeFd);
        nativeFd->parentPath = path;
        fd.nativeFd = nativeFd;

        zfbool success = zffalse;
        do {
            zfstring tmp;
            tmp += path;
            tmp += "/*";
            nativeFd->hFind = FindFirstFileW(
                zfstringToUTF16(tmp, v_ZFStringEncoding::e_UTF8).cString(),
                &(nativeFd->fd));
            if(nativeFd->hFind == INVALID_HANDLE_VALUE) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        } while(zffalse);
        if(success) {
            if(zfstringIsEqual(fd.name, ".")
                    || zfstringIsEqual(fd.name, "..")
                    ) {
                if(!this->fileFindNext(fd)) {
                    this->fileFindClose(fd);
                    return zffalse;
                }
            }
            return zftrue;
        }
        else {
            this->fileFindClose(fd);
            return zffalse;
        }
    }
    virtual zfbool fileFindNext(ZF_IN_OUT ZFIOFindData::Impl &fd) {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;
        if(!FindNextFileW(nativeFd->hFind, &(nativeFd->fd))) {return zffalse;}
        nativeFd->setup(fd);
        if(zfstringIsEqual(fd.name, ".")
                || zfstringIsEqual(fd.name, "..")
                ) {
            return this->fileFindNext(fd);
        }
        return zftrue;
    }
    virtual void fileFindClose(ZF_IN_OUT ZFIOFindData::Impl &fd) {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;
        if(nativeFd->hFind != INVALID_HANDLE_VALUE) {
            FindClose(nativeFd->hFind);
        }
        zfdelete(nativeFd);
        fd.nativeFd = zfnull;
    }

private:
    zfbool makeDir(
            ZF_IN const zfstring &path
            ) {
        if(this->fileIsExist(path)) {
            if(!this->isDir(path)) {
                return zffalse;
            }
            return zftrue;
        }
        if(!CreateDirectoryW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString(), zfnull)) {
            return zffalse;
        }
        return zftrue;
    }
    zfbool makePath(
            ZF_IN const zfstring &path
            , ZF_IN_OPT zfbool excludeLastLevel = zffalse
            ) {
        zfstring pathTmp = path;
        zfindex indexL = zfstringFindReversely(pathTmp, "/");
        if(indexL == zfindexMax()) {
            if(!excludeLastLevel) {
                return this->makeDir(pathTmp);
            }
            return zftrue;
        }
        ZFCoreArray<zfstring> dirToCreate;
        if(!excludeLastLevel) {
            if(this->fileIsExist(pathTmp)) {
                if(!this->isDir(pathTmp)) {
                    return zffalse;
                }
                return zftrue;
            }
            dirToCreate.add(pathTmp);
        }
        pathTmp.remove(indexL);
        do {
            if(this->fileIsExist(pathTmp)) {
                if(!this->isDir(pathTmp)) {
                    return zffalse;
                }
                break;
            }
            dirToCreate.add(pathTmp);
            if(indexL == zfindexMax()) {
                break;
            }
            indexL = zfstringFindReversely(pathTmp, "/");
            if(indexL != zfindexMax()) {
                pathTmp.remove(indexL);
            }
        } while(zftrue);
        for(zfindex i = dirToCreate.count() - 1; i != zfindexMax(); --i) {
            if(!this->makeDir(dirToCreate[i])) {
                return zffalse;
            }
        }
        return zftrue;
    }
    zfbool copyFile(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            , ZF_IN zfbool isForce
            ) {
        if(isForce) {
            this->removeFile(dstPath, isForce);
        }
        if(CopyFileW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    zfstringToUTF16(dstPath, v_ZFStringEncoding::e_UTF8).cString(),
                    !isForce
                    ) != TRUE) {
            return zffalse;
        }
        return zftrue;
    }
    zfbool moveFile(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            ) {
        if(MoveFileW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    zfstringToUTF16(dstPath, v_ZFStringEncoding::e_UTF8).cString()
                    ) != TRUE) {
            return zffalse;
        }
        return zftrue;
    }
    zfbool moveDir(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            ) {
        return this->moveFile(srcPath, dstPath);
    }
    zfbool copyDir(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            , ZF_IN zfbool isForce
            ) {
        ZFCoreArray<zfstring> stacksDirSrc;
        ZFCoreArray<zfstring> stacksDirDst;
        stacksDirSrc.add(srcPath);
        stacksDirDst.add(dstPath);

        while(stacksDirSrc.count() > 0) {
            zfstring srcDir = stacksDirSrc.removeLastAndGet();
            zfstring dstDir = stacksDirDst.removeLastAndGet();

            if(!this->makePath(dstDir)) {
                return zffalse;
            }

            ZFIOFindData::Impl fd;
            if(this->fileFindFirst(fd, srcDir)) {
                do {
                    zfstring srcTmp;
                    srcTmp += srcDir;
                    srcTmp += '/';
                    srcTmp += fd.name;

                    zfstring dstTmp;
                    dstTmp += dstDir;
                    dstTmp += '/';
                    dstTmp += fd.name;
                    if(this->isDir(srcTmp)) {
                        stacksDirSrc.add(srcTmp);
                        stacksDirDst.add(dstTmp);
                    }
                    else {
                        if(!this->copyFile(srcTmp, dstTmp, isForce)) {
                            this->fileFindClose(fd);
                            return zffalse;
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            } // if(this->fileFindFirst(fd, srcDir))
        } // while(!stacksDirSrc.empty())

        return zftrue;
    }
    zfbool removeFile(
            ZF_IN const zfstring &srcPath
            , ZF_IN zfbool isForce
            ) {
        if(isForce) {
            SetFileAttributesW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    FILE_ATTRIBUTE_NORMAL);
        }
        if(DeleteFileW(zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString()) != TRUE) {
            return zffalse;
        }
        return zftrue;
    }
    zfbool removeDir(
            ZF_IN const zfstring &srcPath
            , ZF_IN zfbool isForce
            ) {
        ZFCoreArray<zfstring> dirsToCheck;
        dirsToCheck.add(srcPath);
        ZFCoreArray<zfstring> emptyDirsToDel;

        // delete all child files
        while(dirsToCheck.count() > 0) {
            zfstring dirPath = dirsToCheck.removeLastAndGet();
            emptyDirsToDel.add(dirPath);

            if(isForce) {
                SetFileAttributesW(
                        zfstringToUTF16(dirPath, v_ZFStringEncoding::e_UTF8).cString(),
                        FILE_ATTRIBUTE_NORMAL);
            }

            ZFIOFindData::Impl fd;
            if(this->fileFindFirst(fd, dirPath)) {
                do {
                    zfstring filePath;
                    filePath += dirPath;
                    filePath += '/';
                    filePath += fd.name;
                    if(this->isDir(filePath)) {
                        dirsToCheck.add(filePath);
                    }
                    else {
                        if(!this->removeFile(filePath, isForce)) {
                            return zffalse;
                        }
                    }
                } while(this->fileFindNext(fd));
                this->fileFindClose(fd);
            }
        } // while(!dirsToCheck.empty())

        // delete all empty dir
        while(emptyDirsToDel.count() > 0) {
            zfstring pathTmp = emptyDirsToDel.removeLastAndGet();
            if(RemoveDirectoryW(zfstringToUTF16(pathTmp, v_ZFStringEncoding::e_UTF8).cString()) == 0) {
                return zffalse;
            }
        } // while(!stacksDirToDel.empty())

        return zftrue;
    }

private:
    zfclassPOD _FileToken {
    public:
        HANDLE h;
        zfbool locked;
    };
public:
    virtual void *fileOpen(
            ZF_IN const zfstring &filePath
            , ZF_IN ZFIOOpenOptionFlags flags
            ) {
        zfstringW filePathTmp;
        zfstringToUTF16(filePathTmp, filePath, v_ZFStringEncoding::e_UTF8);

        _FileToken token;
        zfmemset(&token, 0, sizeof(_FileToken));

        if(zffalse) {
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
            if(this->fileIsExist(filePath)) { // r+b
                token.h = CreateFileW(
                        filePathTmp
                        , GENERIC_READ | GENERIC_WRITE
                        , FILE_SHARE_READ
                        , NULL
                        , OPEN_EXISTING
                        , FILE_ATTRIBUTE_NORMAL
                        , NULL
                        );
            }
            else { // w+b
                token.h = CreateFileW(
                        filePathTmp
                        , GENERIC_READ | GENERIC_WRITE
                        , 0
                        , NULL
                        , CREATE_ALWAYS
                        , FILE_ATTRIBUTE_NORMAL
                        , NULL
                        );
            }
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(ZFBitTest(flags, v_ZFIOOpenOption::e_Read)) { // w+b
                token.h = CreateFileW(
                        filePathTmp
                        , GENERIC_READ | GENERIC_WRITE
                        , 0
                        , NULL
                        , CREATE_ALWAYS
                        , FILE_ATTRIBUTE_NORMAL
                        , NULL
                        );
            }
            else { // wb
                token.h = CreateFileW(
                        filePathTmp
                        , GENERIC_WRITE
                        , 0
                        , NULL
                        , CREATE_ALWAYS
                        , FILE_ATTRIBUTE_NORMAL
                        , NULL
                        );
            }
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Read)) { // rb
            token.h = CreateFileW(
                    filePathTmp
                    , GENERIC_READ
                    , FILE_SHARE_READ
                    , NULL
                    , OPEN_EXISTING
                    , FILE_ATTRIBUTE_NORMAL
                    , NULL
                    );
        }
        else {
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
        }
        if(token.h == INVALID_HANDLE_VALUE) {
            return zfnull;
        }

        if(ZFBitTest(flags, v_ZFIOOpenOption::e_Write | v_ZFIOOpenOption::e_Modify)) {
            OVERLAPPED overlapped;
            zfmemset(&overlapped, 0, sizeof(OVERLAPPED));
            token.locked = LockFileEx(
                    token.h
                    , LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY
                    , 0
                    , 0
                    , 0
                    , &overlapped
                    );
            if(!token.locked) {
                CloseHandle(token.h);
                return zfnull;
            }
        }

        _FileToken *impl = (_FileToken *)zfmalloc(sizeof(_FileToken));
        zfmemcpy(impl, &token, sizeof(_FileToken));
        return impl;
    }
    virtual zfbool fileClose(ZF_IN void *token) {
        if(token == zfnull) {
            return zftrue;
        }
        zfbool ret = zftrue;
        _FileToken *impl = (_FileToken *)token;
        if(impl->locked) {
            OVERLAPPED overlapped;
            zfmemset(&overlapped, 0, sizeof(OVERLAPPED));
            ret = ret && UnlockFileEx(
                    impl->h
                    , 0
                    , 0
                    , 0
                    , &overlapped
                    );
        }
        ret = ret && CloseHandle(impl->h);
        zffree(impl);
        return ret;
    }

    virtual zfindex fileTell(ZF_IN void *token) {
        if(token == zfnull) {
            return zfindexMax();
        }
        _FileToken *impl = (_FileToken *)token;
        LARGE_INTEGER li;
        li.QuadPart = 0;
        li.LowPart = SetFilePointer(impl->h, li.LowPart, &li.HighPart, FILE_CURRENT);
        if(li.LowPart != INVALID_SET_FILE_POINTER || GetLastError() == NO_ERROR) {
            return (zfindex)li.QuadPart;
        }
        else {
            return zfindexMax();
        }
    }
    virtual zfbool fileSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        if(token == zfnull) {
            return zffalse;
        }
        _FileToken *impl = (_FileToken *)token;
        LARGE_INTEGER li;
        li.QuadPart = (LONGLONG)byteSize;

        DWORD tmpPos = FILE_BEGIN;
        switch(seekPos) {
            case ZFSeekPosBegin:
                tmpPos = FILE_BEGIN;
                break;
            case ZFSeekPosCur:
                tmpPos = FILE_CURRENT;
                break;
            case ZFSeekPosCurReversely:
                tmpPos = FILE_CURRENT;
                li.QuadPart = -li.QuadPart;
                break;
            case ZFSeekPosEnd:
                tmpPos = FILE_END;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
        li.LowPart = SetFilePointer(impl->h, li.LowPart, &li.HighPart, tmpPos);
        return (li.LowPart != INVALID_SET_FILE_POINTER || GetLastError() == NO_ERROR);
    }

    virtual zfindex fileRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(token == zfnull) {
            return 0;
        }
        _FileToken *impl = (_FileToken *)token;
        DWORD read = 0;
        if(!ReadFile(impl->h, (LPVOID)buf, (DWORD)maxByteSize, &read, NULL)) {
            return 0;
        }
        else {
            return (zfindex)read;
        }
    }

    virtual zfindex fileWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN zfindex maxByteSize
            ) {
        if(token == zfnull) {
            return 0;
        }
        _FileToken *impl = (_FileToken *)token;
        DWORD written = 0;
        if(!WriteFile(impl->h, (LPCVOID)src, (DWORD)maxByteSize, (LPDWORD)&written, NULL)) {
            return 0;
        }
        else {
            return (zfindex)written;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileImpl_sys_Windows)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Windows

