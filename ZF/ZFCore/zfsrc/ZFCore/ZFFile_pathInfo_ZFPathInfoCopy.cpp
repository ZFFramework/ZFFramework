#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFPathInfoCopy_SetErrPos(
        ZF_IN ZFPathInfo *errPos
        , ZF_IN const ZFPathInfo &s
        ) {
    if(errPos != zfnull) {
        if(s) {
            *errPos = s;
        }
        else {
            *errPos = ZFPathInfo("", "<null>");
        }
    }
}

static zfbool _ZFP_ZFPathInfoCopy_copyFile(
        ZF_IN const ZFPathInfo &srcPath
        , ZF_IN const ZFPathInfo &dstPath
        , ZF_IN const ZFPathInfoImpl &srcImpl
        , ZF_IN const ZFPathInfoImpl &dstImpl
        , ZF_IN zfbool isForce
        , ZF_IN_OPT ZFPathInfo *errPos
        ) {
    if(isForce) {
        dstImpl.implRemove(dstPath.pathData(), zffalse, isForce, zfnull);
    }
    void *srcFd = srcImpl.implOpen(srcPath.pathData(), ZFFileOpenOption::e_Read, zffalse);
    if(srcFd == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    ZFPathInfoCloseHolder(srcPath, srcFd);
    void *dstFd = dstImpl.implOpen(dstPath.pathData(), ZFFileOpenOption::e_Write, zftrue);
    if(dstFd == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }
    ZFPathInfoCloseHolder(dstPath, dstFd);

    const zfindex bufSize = 1024;
    zfchar buf[1024];
    zfindex size = 0;
    while((size = srcImpl.implRead(srcFd, buf, bufSize)) > 0) {
        if(dstImpl.implWrite(dstFd, buf, size) < size) {
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
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
        , ZF_IN_OPT ZFPathInfo *errPos
        ) {
    ZFCoreArray<zfstring> stacksDirSrc;
    ZFCoreArray<zfstring> stacksDirDst;
    stacksDirSrc.add(srcPath.pathData());
    stacksDirDst.add(dstPath.pathData());

    ZFPathInfo srcDir(srcPath.pathType(), zfnull);
    ZFPathInfo dstDir(dstPath.pathType(), zfnull);
    zfstring errPosTmp;
    while(!stacksDirSrc.isEmpty()) {
        srcDir.pathData(stacksDirSrc.removeLastAndGet());
        dstDir.pathData(stacksDirDst.removeLastAndGet());

        if(!dstImpl.implPathCreate(dstDir.pathData(), zftrue, &errPosTmp)) {
            dstDir.pathData(errPosTmp);
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstDir);
            return zffalse;
        }

        ZFFileFindData fd;
        if(srcImpl.implFindFirst(fd, srcDir.pathData())) {
            ZFPathInfo srcTmp(srcDir.pathType(), zfnull);
            ZFPathInfo dstTmp(dstDir.pathType(), zfnull);
            do {
                srcTmp.pathData(srcImpl.implToChild(srcDir.pathData(), fd.name()));

                dstTmp.pathData(dstImpl.implToChild(dstDir.pathData(), fd.name()));
                if(srcImpl.implIsDir(srcTmp.pathData())) {
                    stacksDirSrc.add(srcTmp.pathData());
                    stacksDirDst.add(dstTmp.pathData());
                }
                else {
                    if(!_ZFP_ZFPathInfoCopy_copyFile(srcTmp, dstTmp, srcImpl, dstImpl, isForce, errPos)) {
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

ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFPathInfoCopy
        , ZFMP_IN(const ZFPathInfo &, srcPath)
        , ZFMP_IN(const ZFPathInfo &, dstPath)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        , ZFMP_IN_OPT(ZFPathInfo *, errPos, zfnull)
        ) {
    if(srcPath == zfnull || dstPath == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, zfnull);
        return zffalse;
    }
    const ZFPathInfoImpl *srcImpl = ZFPathInfoImplForPathType(srcPath.pathType());
    const ZFPathInfoImpl *dstImpl = ZFPathInfoImplForPathType(dstPath.pathType());
    if(srcImpl == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    if(dstImpl == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }

    if(!srcImpl->implIsExist(srcPath.pathData())) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool srcIsDir = srcImpl->implIsDir(srcPath.pathData());
    if(srcIsDir && !isRecursive) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool dstExist = dstImpl->implIsExist(dstPath.pathData());
    zfbool dstIsDir = dstImpl->implIsDir(dstPath.pathData());
    if(dstExist && srcIsDir != dstIsDir) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }

    {
        ZFPathInfo dstPathParent(dstPath.pathType(), srcIsDir ? dstPath.pathData() : dstImpl->implToParent(dstPath.pathData()));
        zfstring errPosTmp;
        if(!dstImpl->implPathCreate(dstPathParent.pathData(), zftrue, &errPosTmp)) {
            dstPathParent.pathData(errPosTmp);
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPathParent);
            return zffalse;
        }
    }

    if(srcIsDir) {
        return _ZFP_ZFPathInfoCopy_copyDir(srcPath, dstPath, *srcImpl, *dstImpl, isForce, errPos);
    }
    else {
        return _ZFP_ZFPathInfoCopy_copyFile(srcPath, dstPath, *srcImpl, *dstImpl, isForce, errPos);
    }
}

ZF_NAMESPACE_GLOBAL_END

