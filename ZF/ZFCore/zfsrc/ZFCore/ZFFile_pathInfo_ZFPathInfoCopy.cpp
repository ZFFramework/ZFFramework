#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFPathInfoCopy_SetErrPos(
        ZF_IN ZFPathInfo *errPos
        , ZF_IN const ZFPathInfo *s
        ) {
    if(errPos != zfnull) {
        *errPos = ((s != zfnull) ? (*s) : ZFPathInfo("", "<null>"));
    }
}
static void _ZFP_ZFPathInfoCopy_SetErrPos(
        ZF_IN ZFPathInfo *errPos
        , ZF_IN const ZFPathInfo &s
        ) {
    _ZFP_ZFPathInfoCopy_SetErrPos(errPos, &s);
}

static zfbool _ZFP_ZFPathInfoCopy_copyFile(
        ZF_IN const ZFPathInfo &srcPath
        , ZF_IN const ZFPathInfo &dstPath
        , ZF_IN zfbool isForce
        , ZF_IN_OPT ZFPathInfo *errPos
        ) {
    if(isForce) {
        ZFPathInfoRemove(dstPath, zffalse, isForce, zfnull);
    }
    void *srcFd = ZFPathInfoOpen(srcPath, ZFFileOpenOption::e_Read, zffalse);
    if(srcFd == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    ZFPathInfoCloseHolder(srcPath, srcFd);
    void *dstFd = ZFPathInfoOpen(dstPath, ZFFileOpenOption::e_Write, zftrue);
    if(dstFd == zfnull) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }
    ZFPathInfoCloseHolder(dstPath, dstFd);

    const zfindex bufSize = 1024;
    zfchar buf[1024];
    zfindex size = 0;
    while((size = ZFPathInfoRead(srcPath, srcFd, buf, bufSize)) > 0) {
        if(ZFPathInfoWrite(dstPath, dstFd, buf, size) < size) {
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
            return zffalse;
        }
    }
    return zftrue;
}

static zfbool _ZFP_ZFPathInfoCopy_copyDir(
        ZF_IN const ZFPathInfo &srcPath
        , ZF_IN const ZFPathInfo &dstPath
        , ZF_IN zfbool isForce
        , ZF_IN_OPT ZFPathInfo *errPos
        ) {
    ZFCoreArray<zfstring> stacksDirSrc;
    ZFCoreArray<zfstring> stacksDirDst;
    stacksDirSrc.add(srcPath.pathData);
    stacksDirDst.add(dstPath.pathData);

    ZFPathInfo srcDir(srcPath.pathType, zfnull);
    ZFPathInfo dstDir(dstPath.pathType, zfnull);
    zfstring errPosTmp;
    while(!stacksDirSrc.isEmpty()) {
        srcDir.pathData = stacksDirSrc.getLast();
        dstDir.pathData = stacksDirDst.getLast();
        stacksDirSrc.removeLast();
        stacksDirDst.removeLast();

        if(!ZFPathInfoPathCreate(dstDir, zftrue, &errPosTmp)) {
            dstDir.pathData = errPosTmp;
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstDir);
            return zffalse;
        }

        ZFFileFindData fd;
        if(ZFPathInfoFindFirst(srcDir, fd)) {
            ZFPathInfo srcTmp(srcDir.pathType, zfnull);
            ZFPathInfo dstTmp(dstDir.pathType, zfnull);
            do {
                srcTmp.pathData = srcDir.pathData;
                ZFPathInfoToChild(srcTmp, srcTmp.pathData, fd.fileName());

                dstTmp.pathData = dstDir.pathData;
                ZFPathInfoToChild(dstTmp, dstTmp.pathData, fd.fileName());
                if(ZFPathInfoIsDir(srcTmp)) {
                    stacksDirSrc.add(srcTmp.pathData);
                    stacksDirDst.add(dstTmp.pathData);
                }
                else {
                    if(!_ZFP_ZFPathInfoCopy_copyFile(srcTmp, dstTmp, isForce, errPos)) {
                        ZFPathInfoFindClose(srcDir, fd);
                        return zffalse;
                    }
                }
            } while(ZFPathInfoFindNext(srcDir, fd));
            ZFPathInfoFindClose(srcDir, fd);
        } // if(ZFPathInfoFindFirst(srcDir, fd))
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
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, (const ZFPathInfo *)zfnull);
        return zffalse;
    }
    if(!ZFPathInfoIsExist(srcPath)) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool srcIsDir = ZFPathInfoIsDir(srcPath);
    if(srcIsDir && !isRecursive) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool dstExist = ZFPathInfoIsExist(dstPath);
    zfbool dstIsDir = ZFPathInfoIsDir(dstPath);
    if(dstExist && srcIsDir != dstIsDir) {
        _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }

    {
        ZFPathInfo dstPathParent(dstPath.pathType, zfnull);
        if(!srcIsDir) {
            if(!ZFPathInfoToParent(dstPathParent, dstPathParent.pathData)) {
                _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPath);
                return zffalse;
            }
        }
        zfstring errPosTmp;
        if(!ZFPathInfoPathCreate(dstPathParent, zftrue, &errPosTmp)) {
            dstPathParent.pathData = errPosTmp;
            _ZFP_ZFPathInfoCopy_SetErrPos(errPos, dstPathParent);
            return zffalse;
        }
    }

    if(srcIsDir) {
        return _ZFP_ZFPathInfoCopy_copyDir(srcPath, dstPath, isForce, errPos);
    }
    else {
        return _ZFP_ZFPathInfoCopy_copyFile(srcPath, dstPath, isForce, errPos);
    }
}

ZF_NAMESPACE_GLOBAL_END

