#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFPathInfoCopy_copyFile(
        ZF_IN const ZFPathInfo &srcPath
        , ZF_IN const ZFPathInfo &dstPath
        , ZF_IN const ZFPathInfoImpl &srcImpl
        , ZF_IN const ZFPathInfoImpl &dstImpl
        , ZF_IN zfbool isForce
        ) {
    if(isForce) {
        dstImpl.implRemove(dstPath.pathData(), zffalse, isForce);
    }
    void *srcFd = srcImpl.implOpen(srcPath.pathData(), v_ZFFileOpenOption::e_Read, zffalse);
    if(srcFd == zfnull) {
        return zffalse;
    }
    ZFPathInfoCloseHolder(srcPath, srcFd);
    void *dstFd = dstImpl.implOpen(dstPath.pathData(), v_ZFFileOpenOption::e_Write, zftrue);
    if(dstFd == zfnull) {
        return zffalse;
    }
    ZFPathInfoCloseHolder(dstPath, dstFd);

    const zfindex bufSize = 1024;
    zfchar buf[1024];
    zfindex size = 0;
    while((size = srcImpl.implRead(srcFd, buf, bufSize)) > 0) {
        if(dstImpl.implWrite(dstFd, buf, size) < size) {
            return zffalse;
        }
    }
    return zftrue;
}

static zfbool _ZFP_ZFPathInfoCopy_copyDir(
        ZF_IN const ZFPathInfo &srcPath
        , ZF_IN const ZFPathInfo &dstPath
        , ZF_IN const ZFPathInfoImpl &srcImpl
        , ZF_IN const ZFPathInfoImpl &dstImpl
        , ZF_IN zfbool isForce
        ) {
    ZFCoreArray<zfstring> stacksDirSrc;
    ZFCoreArray<zfstring> stacksDirDst;
    stacksDirSrc.add(srcPath.pathData());
    stacksDirDst.add(dstPath.pathData());

    ZFPathInfo srcDir(srcPath.pathType(), zfnull);
    ZFPathInfo dstDir(dstPath.pathType(), zfnull);
    while(!stacksDirSrc.isEmpty()) {
        srcDir.pathData(stacksDirSrc.removeLastAndGet());
        dstDir.pathData(stacksDirDst.removeLastAndGet());

        if(!dstImpl.implPathCreate(dstDir.pathData(), zftrue)) {
            return zffalse;
        }

        ZFFileFindData fd;
        if(srcImpl.implFindFirst(fd, srcDir.pathData())) {
            ZFPathInfo srcTmp(srcDir.pathType(), zfnull);
            ZFPathInfo dstTmp(dstDir.pathType(), zfnull);
            do {
                zfstring srcPathData;
                srcImpl.implToChild(srcPathData, srcDir.pathData(), fd.name());
                srcTmp.pathData(srcPathData);

                zfstring dstPathData;
                dstImpl.implToChild(dstPathData, dstDir.pathData(), fd.name());
                dstTmp.pathData(dstPathData);

                if(srcImpl.implIsDir(srcTmp.pathData())) {
                    stacksDirSrc.add(srcTmp.pathData());
                    stacksDirDst.add(dstTmp.pathData());
                }
                else {
                    if(!_ZFP_ZFPathInfoCopy_copyFile(srcTmp, dstTmp, srcImpl, dstImpl, isForce)) {
                        srcImpl.implFindClose(fd);
                        return zffalse;
                    }
                }
            } while(srcImpl.implFindNext(fd));
            srcImpl.implFindClose(fd);
        }
    } // while(!stacksDirSrc.isEmpty())

    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFPathInfoCopy
        , ZFMP_IN(const ZFPathInfo &, srcPath)
        , ZFMP_IN(const ZFPathInfo &, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        ) {
    if(srcPath == zfnull || dstPath == zfnull) {
        return zffalse;
    }
    const ZFPathInfoImpl *srcImpl = ZFPathInfoImplForPathType(srcPath.pathType());
    const ZFPathInfoImpl *dstImpl = ZFPathInfoImplForPathType(dstPath.pathType());
    if(srcImpl == zfnull) {
        return zffalse;
    }
    if(dstImpl == zfnull) {
        return zffalse;
    }

    if(!srcImpl->implIsExist(srcPath.pathData())) {
        return zffalse;
    }
    zfbool srcIsDir = srcImpl->implIsDir(srcPath.pathData());
    if(srcIsDir && !isRecursive) {
        return zffalse;
    }
    zfbool dstExist = dstImpl->implIsExist(dstPath.pathData());
    zfbool dstIsDir = dstImpl->implIsDir(dstPath.pathData());
    if(dstExist && srcIsDir != dstIsDir) {
        return zffalse;
    }

    {
        zfstring dstPathDataParent;
        if(srcIsDir) {
            dstPathDataParent = dstPath.pathData();
        }
        else {
            dstImpl->implToParent(dstPathDataParent, dstPath.pathData());
        }
        if(!dstImpl->implPathCreate(dstPathDataParent, zftrue)) {
            return zffalse;
        }
    }

    if(srcIsDir) {
        return _ZFP_ZFPathInfoCopy_copyDir(srcPath, dstPath, *srcImpl, *dstImpl, isForce);
    }
    else {
        return _ZFP_ZFPathInfoCopy_copyFile(srcPath, dstPath, *srcImpl, *dstImpl, isForce);
    }
}

ZF_NAMESPACE_GLOBAL_END

