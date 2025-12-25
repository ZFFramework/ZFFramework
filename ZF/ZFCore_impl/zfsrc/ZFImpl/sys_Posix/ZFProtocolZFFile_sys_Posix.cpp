#include "ZFImpl_sys_Posix_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFile.h"
#include "ZFCore/zfstringW.h"
#include "ZFCore/ZFIO_file.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Posix
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileImpl_sys_Posix, ZFFile, v_ZFProtocolLevel::e_SystemLow)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("nativeAPI")
public:
    virtual zfbool fileIsExist(ZF_IN const zfstring &path) {
        if(path == zfnull) {return zffalse;}
        return (access(path, F_OK) != -1);
    }
    virtual zfbool isDir(ZF_IN const zfstring &path) {
        zfstring tmp = path;
        tmp += '/';
        struct stat statbuf;
        if(lstat(tmp.cString(), &statbuf) < 0) {return zffalse;}
        return S_ISDIR(statbuf.st_mode);
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
    };

    virtual zfbool fileFindFirst(
            ZF_IN_OUT ZFIOFindData::Impl &fd
            , ZF_IN const zfstring &path
            ) {
        if(path == zfnull) {return zffalse;}
        _ZFP_ZFFileNativeFd *nativeFd = zfpoolNew(_ZFP_ZFFileNativeFd);
        nativeFd->parentPath = path;
        fd.nativeFd = nativeFd;

        zfbool success = zffalse;
        do {
            nativeFd->pDir = opendir(path);
            if(nativeFd->pDir == zfnull) {break;}
            nativeFd->pDirent = readdir(nativeFd->pDir);
            if(nativeFd->pDirent == zfnull) {break;}

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
        nativeFd->pDirent = readdir(nativeFd->pDir);
        if(nativeFd->pDirent == zfnull) {return zffalse;}
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
        if(nativeFd->pDir != zfnull) {
            closedir(nativeFd->pDir);
        }
        zfpoolDelete(nativeFd);
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
        if(mkdir(path, 0777) != 0) {
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
    }
    zfbool moveFile(
            ZF_IN const zfstring &srcPath
            , ZF_IN const zfstring &dstPath
            ) {
        if(rename(srcPath, dstPath) != 0) {
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
            chmod(srcPath, 0777);
        }
        if(remove(srcPath) != 0) {
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
                chmod(dirPath.cString(), 0777);
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
            if(rmdir(pathTmp.cString()) != 0) {
                return zffalse;
            }
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

        if(ZFBitTest(flags, v_ZFIOOpenOption::e_Write | v_ZFIOOpenOption::e_Modify)) {
            int fd = -1;
            {
                zfsynchronize(_lock);
                fd = open(
                        filePath
                        , O_RDWR | O_CREAT
                        , 0644
                        );
                if(fd == -1) {
                    return zfnull;
                }
                if(flock(fd, LOCK_EX | LOCK_NB) == -1) {
                    close(fd);
                    return zfnull;
                }

                if(!ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
                    close(fd);
                    int fd = open(
                            filePath
                            , O_RDWR | O_CREAT | O_TRUNC
                            , 0644
                            );
                    if(fd == -1) {
                        return zfnull;
                    }
                    if(flock(fd, LOCK_EX | LOCK_NB) == -1) {
                        close(fd);
                        return zfnull;
                    }
                }
            }

            FILE *fp = fdopen(fd, sFlag);
            if(fp == NULL) {
                close(fd);
                return zfnull;
            }
            return fp;
        }
        else {
            return fopen(filePath, sFlag);
        }
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
private:
    zfobj<ZFObject> _lock;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileImpl_sys_Posix)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Posix

