#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFile.h"
#include "ZFCore/zfstringW.h"
#include "ZFCore/ZFIO_file.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#else // #if ZF_ENV_sys_Windows
    #include <unistd.h>
    #include <dirent.h>
    #include <errno.h>
    #include <sys/stat.h>
#endif // #if ZF_ENV_sys_Windows #else

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileImpl_default, ZFFile, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("nativeAPI")
public:
    virtual zfbool fileIsExist(ZF_IN const zfstring &path) {
        if(path == zfnull) {return zffalse;}
        #if ZF_ENV_sys_Windows
            return (GetFileAttributesW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString()) != 0xFFFFFFFF);
        #else // #if ZF_ENV_sys_Windows
            return (access(path, F_OK) != -1);
        #endif // #if ZF_ENV_sys_Windows #else
    }
    virtual zfbool isDir(ZF_IN const zfstring &path) {
        #if ZF_ENV_sys_Windows
            return ((GetFileAttributesW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString())
                & FILE_ATTRIBUTE_DIRECTORY) != 0);
        #else // #if ZF_ENV_sys_Windows
            zfstring tmp = path;
            tmp += '/';
            struct stat statbuf;
            if(lstat(tmp.cString(), &statbuf) < 0) {return zffalse;}
            return S_ISDIR(statbuf.st_mode);
        #endif // #if ZF_ENV_sys_Windows #else
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
            , ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
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
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {
        return this->cp_or_mv(zffalse, srcPath, dstPath, zftrue, isForce);
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
    #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void) {
            zfmemset(&this->fd, 0, sizeof(WIN32_FIND_DATAW));
            this->hFind = zfnull;
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
    #else // #if ZF_ENV_sys_Windows
        _ZFP_ZFFileNativeFd(void) {
            this->pDir = zfnull;
            this->pDirent = zfnull;
            zfmemset(&this->fStat, 0, sizeof(struct stat));
        }
        DIR *pDir;
        struct dirent *pDirent;
        struct stat fStat;
        void setup(ZFIOFindData::Impl &zfd) {
            zfd.name = pDirent->d_name;

            zfstring filePath = this->parentPath;
            filePath += '/';
            filePath += zfd.name;

            zfd.isDir = ZFFileIsDir(filePath);
        }
    #endif // #if ZF_ENV_sys_Windows #else
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
        #if ZF_ENV_sys_Windows
            zfstring tmp;
            tmp += path;
            tmp += "/*";
            nativeFd->hFind = FindFirstFileW(
                zfstringToUTF16(tmp, v_ZFStringEncoding::e_UTF8).cString(),
                &(nativeFd->fd));
            if(nativeFd->hFind == INVALID_HANDLE_VALUE) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #else // #if ZF_ENV_sys_Windows
            nativeFd->pDir = opendir(path);
            if(nativeFd->pDir == zfnull) {break;}
            nativeFd->pDirent = readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {break;}

            nativeFd->setup(fd);
            success = zftrue;
        #endif // #if ZF_ENV_sys_Windows #else
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
        #if ZF_ENV_sys_Windows
            if(!FindNextFileW(nativeFd->hFind, &(nativeFd->fd))) {return zffalse;}
            nativeFd->setup(fd);
        #else // #if ZF_ENV_sys_Windows
            nativeFd->pDirent = readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {return zffalse;}
            nativeFd->setup(fd);
        #endif // #if ZF_ENV_sys_Windows #else
        if(zfstringIsEqual(fd.name, ".")
                || zfstringIsEqual(fd.name, "..")
                ) {
            return this->fileFindNext(fd);
        }
        return zftrue;
    }
    virtual void fileFindClose(ZF_IN_OUT ZFIOFindData::Impl &fd) {
        _ZFP_ZFFileNativeFd *nativeFd = (_ZFP_ZFFileNativeFd *)fd.nativeFd;

        #if ZF_ENV_sys_Windows
            if(nativeFd->hFind != zfnull) {
                FindClose(nativeFd->hFind);
            }
        #else // #if ZF_ENV_sys_Windows
            if(nativeFd->pDir != zfnull) {
                closedir(nativeFd->pDir);
            }
        #endif // #if ZF_ENV_sys_Windows #else

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
        #if ZF_ENV_sys_Windows
            if(!CreateDirectoryW(zfstringToUTF16(path, v_ZFStringEncoding::e_UTF8).cString(), zfnull)) {
                return zffalse;
            }
        #else // #if ZF_ENV_sys_Windows
            if(mkdir(path, 0777) != 0) {
                return zffalse;
            }
        #endif // #if ZF_ENV_sys_Windows #else
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
        #if ZF_ENV_sys_Windows
            if(CopyFileW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    zfstringToUTF16(dstPath, v_ZFStringEncoding::e_UTF8).cString(),
                    !isForce
                ) != TRUE) {
                return zffalse;
            }
            return zftrue;
        #else // #if ZF_ENV_sys_Windows
            if(this->fileIsExist(dstPath) && !isForce) {
                return zffalse;
            }

            FILE *fpSrc = fopen(srcPath, "rb");
            if(fpSrc == zfnull) {
                return zffalse;
            }
            FILE *fpDst = fopen(dstPath, "wb");
            if(fpDst == zfnull) {
                fclose(fpSrc);
                return zffalse;
            }

            size_t readSize = 0;
            #define _copyFile_bufSize 4096
            zfbyte readBuf[_copyFile_bufSize];
            while((readSize = fread(readBuf, 1, _copyFile_bufSize, fpSrc)) > 0) {
                fwrite(readBuf, 1, readSize, fpDst);
            }
            fclose(fpSrc);
            fclose(fpDst);
            #undef _copyFile_bufSize
            return zftrue;
        #endif // #if ZF_ENV_sys_Windows #else
    }
    zfbool moveFile(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            ) {
        #if ZF_ENV_sys_Windows
            if(MoveFileW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    zfstringToUTF16(dstPath, v_ZFStringEncoding::e_UTF8).cString()
                ) != TRUE) {
                return zffalse;
            }
            return zftrue;
        #else // #if ZF_ENV_sys_Windows
            if(rename(srcPath, dstPath) != 0) {
                return zffalse;
            }
            return zftrue;
        #endif // #if ZF_ENV_sys_Windows #else
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
        #if ZF_ENV_sys_Windows
            if(isForce) {
                SetFileAttributesW(
                    zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString(),
                    FILE_ATTRIBUTE_NORMAL);
            }
            if(DeleteFileW(zfstringToUTF16(srcPath, v_ZFStringEncoding::e_UTF8).cString()) != TRUE) {
                return zffalse;
            }
            return zftrue;
        #else // #if ZF_ENV_sys_Windows
            if(isForce) {
                chmod(srcPath, 0777);
            }
            if(remove(srcPath) != 0) {
                return zffalse;
            }
            return zftrue;
        #endif // #if ZF_ENV_sys_Windows #else
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
                #if ZF_ENV_sys_Windows
                    SetFileAttributesW(
                        zfstringToUTF16(dirPath, v_ZFStringEncoding::e_UTF8).cString(),
                        FILE_ATTRIBUTE_NORMAL);
                #else // #if ZF_ENV_sys_Windows
                    chmod(dirPath.cString(), 0777);
                #endif // #if ZF_ENV_sys_Windows #else
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

            #if ZF_ENV_sys_Windows
                if(RemoveDirectoryW(zfstringToUTF16(pathTmp, v_ZFStringEncoding::e_UTF8).cString()) == 0) {
                    return zffalse;
                }
            #else // #if ZF_ENV_sys_Windows
                if(rmdir(pathTmp.cString()) != 0) {
                    return zffalse;
                }
            #endif // #if ZF_ENV_sys_Windows #else
        } // while(!stacksDirToDel.empty())

        return zftrue;
    }

public:
    virtual void *fileOpen(
            ZF_IN const zfstring &filePath
            , ZF_IN ZFIOOpenOptionFlags flags
            ) {
        const zfchar *sFlag = zfnull;
        if(zffalse) {
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
            if(this->fileIsExist(filePath)) {
                sFlag = "r+b";
            }
            else {
                sFlag = "w+b";
            }
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
            if(ZFBitTest(flags, v_ZFIOOpenOption::e_Read)) {
                sFlag = "w+b";
            }
            else {
                sFlag = "wb";
            }
        }
        else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Read)) {
            sFlag = "rb";
        }
        else {
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
        }

        #if ZF_ENV_sys_Windows
            zfstringW _filePathSaved;
            zfstringToUTF16(_filePathSaved, filePath, v_ZFStringEncoding::e_UTF8);
            DWORD _fileAttrSaved = GetFileAttributesW(_filePathSaved.cString());
            SetFileAttributesW(_filePathSaved.cString(), FILE_ATTRIBUTE_NORMAL);
        #endif

        void *ret = fopen(filePath, sFlag);

        #if ZF_ENV_sys_Windows
            SetFileAttributesW(_filePathSaved.cString(), _fileAttrSaved);
        #endif

        return ret;
    }
    virtual zfbool fileClose(ZF_IN void *token) {
        if(token == zfnull) {
            return zftrue;
        }
        return (fclose((FILE *)token) == 0);
    }

    virtual zfindex fileTell(ZF_IN void *token) {
        if(token == zfnull) {
            return zfindexMax();
        }
        long result = ftell((FILE *)token);
        if(result == -1) {
            return zfindexMax();
        }
        return (zfindex)result;
    }
    virtual zfbool fileSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        if(token == zfnull) {
            return zffalse;
        }
        int tmpPos = 0;
        long seekSize = (long)byteSize;
        switch(seekPos) {
            case ZFSeekPosBegin:
                tmpPos = SEEK_SET;
                break;
            case ZFSeekPosCur:
                tmpPos = SEEK_CUR;
                break;
            case ZFSeekPosCurReversely:
                tmpPos = SEEK_CUR;
                seekSize = -seekSize;
                break;
            case ZFSeekPosEnd:
                tmpPos = SEEK_END;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }
        return (fseek((FILE *)token, seekSize, tmpPos) == 0);
    }

    virtual zfindex fileRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(token == zfnull) {
            return 0;
        }
        else {
            return (zfindex)fread(buf, 1, (size_t)maxByteSize, (FILE *)token);
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
        else {
            return (zfindex)fwrite(src, 1, (size_t)maxByteSize, (FILE *)token);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileImpl_default)

ZF_NAMESPACE_GLOBAL_END

