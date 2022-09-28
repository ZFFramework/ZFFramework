#include "ZFFile_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFFilePathInfoCopy_SetErrPos(ZF_IN ZFPathInfo *errPos, ZF_IN const ZFPathInfo *s)
{
    if(errPos != zfnull)
    {
        *errPos = ((s != zfnull) ? (*s) : ZFPathInfo("", "<null>"));
    }
}
static void _ZFP_ZFFilePathInfoCopy_SetErrPos(ZF_IN ZFPathInfo *errPos, ZF_IN const ZFPathInfo &s)
{
    _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, &s);
}

static zfbool _ZFP_ZFFilePathInfoCopy_copyFile(ZF_IN const ZFPathInfo &srcPath,
                                               ZF_IN const ZFPathInfo &dstPath,
                                               ZF_IN zfbool isForce,
                                               ZF_IN_OPT ZFPathInfo *errPos)
{
    if(isForce)
    {
        ZFFilePathInfoRemove(dstPath, zffalse, isForce, zfnull);
    }
    void *srcFd = ZFFilePathInfoOpen(srcPath, ZFFileOpenOption::e_Read, zffalse);
    if(srcFd == zfnull)
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    ZFFilePathInfoCloseHolder(srcPath, srcFd);
    void *dstFd = ZFFilePathInfoOpen(dstPath, ZFFileOpenOption::e_Write, zftrue);
    if(dstFd == zfnull)
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }
    ZFFilePathInfoCloseHolder(dstPath, dstFd);

    const zfindex bufSize = 1024;
    zfchar buf[1024];
    zfindex size = 0;
    while((size = ZFFilePathInfoRead(srcPath, srcFd, buf, bufSize)) > 0)
    {
        if(ZFFilePathInfoWrite(dstPath, dstFd, buf, size) < size)
        {
            _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstPath);
            return zffalse;
        }
    }
    return zftrue;
}

static zfbool _ZFP_ZFFilePathInfoCopy_copyDir(ZF_IN const ZFPathInfo &srcPath,
                                              ZF_IN const ZFPathInfo &dstPath,
                                              ZF_IN zfbool isForce,
                                              ZF_IN_OPT ZFPathInfo *errPos)
{
    ZFCoreArray<zfstring> stacksDirSrc;
    ZFCoreArray<zfstring> stacksDirDst;
    stacksDirSrc.add(srcPath.pathData);
    stacksDirDst.add(dstPath.pathData);

    ZFPathInfo srcDir(srcPath.pathType);
    ZFPathInfo dstDir(dstPath.pathType);
    zfstring errPosTmp;
    while(!stacksDirSrc.isEmpty())
    {
        srcDir.pathData = stacksDirSrc.getLast();
        dstDir.pathData = stacksDirDst.getLast();
        stacksDirSrc.removeLast();
        stacksDirDst.removeLast();

        if(!ZFFilePathInfoPathCreate(dstDir, zftrue, &errPosTmp))
        {
            dstDir.pathData = errPosTmp;
            _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstDir);
            return zffalse;
        }

        ZFFileFindData fd;
        if(ZFFilePathInfoFindFirst(srcDir, fd))
        {
            ZFPathInfo srcTmp(srcDir.pathType);
            ZFPathInfo dstTmp(dstDir.pathType);
            do
            {
                srcTmp.pathData = srcDir.pathData;
                ZFFilePathInfoToChild(srcTmp, srcTmp.pathData, fd.fileName());

                dstTmp.pathData = dstDir.pathData;
                ZFFilePathInfoToChild(dstTmp, dstTmp.pathData, fd.fileName());
                if(ZFFilePathInfoIsDir(srcTmp))
                {
                    stacksDirSrc.add(srcTmp.pathData);
                    stacksDirDst.add(dstTmp.pathData);
                }
                else
                {
                    if(!_ZFP_ZFFilePathInfoCopy_copyFile(srcTmp, dstTmp, isForce, errPos))
                    {
                        ZFFilePathInfoFindClose(srcDir, fd);
                        return zffalse;
                    }
                }
            } while(ZFFilePathInfoFindFirst(srcDir, fd));
            ZFFilePathInfoFindClose(srcDir, fd);
        } // if(ZFFilePathInfoFindFirst(srcDir, fd))
    } // while(!stacksDirSrc.isEmpty())

    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFFilePathInfoCopy,
                       ZFMP_IN(const ZFPathInfo &, srcPath),
                       ZFMP_IN(const ZFPathInfo &, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(ZFPathInfo *, errPos, zfnull))
{
    if(srcPath == zfnull || dstPath == zfnull)
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, zfnull);
        return zffalse;
    }
    if(!ZFFilePathInfoIsExist(srcPath))
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool srcIsDir = ZFFilePathInfoIsDir(srcPath);
    if(srcIsDir && !isRecursive)
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, srcPath);
        return zffalse;
    }
    zfbool dstExist = ZFFilePathInfoIsExist(dstPath);
    zfbool dstIsDir = ZFFilePathInfoIsDir(dstPath);
    if(dstExist && srcIsDir != dstIsDir)
    {
        _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstPath);
        return zffalse;
    }

    {
        ZFPathInfo dstPathParent(dstPath.pathType);
        if(!srcIsDir)
        {
            if(!ZFFilePathInfoToParent(dstPathParent, dstPathParent.pathData))
            {
                _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstPath);
                return zffalse;
            }
        }
        zfstring errPosTmp;
        if(!ZFFilePathInfoPathCreate(dstPathParent, zftrue, &errPosTmp))
        {
            dstPathParent.pathData = errPosTmp;
            _ZFP_ZFFilePathInfoCopy_SetErrPos(errPos, dstPathParent);
            return zffalse;
        }
    }

    if(srcIsDir)
    {
        return _ZFP_ZFFilePathInfoCopy_copyDir(srcPath, dstPath, isForce, errPos);
    }
    else
    {
        return _ZFP_ZFFilePathInfoCopy_copyFile(srcPath, dstPath, isForce, errPos);
    }
}

ZF_NAMESPACE_GLOBAL_END

