#include "ZFFile_impl.h"

#include "ZFCore/ZFPathType_res.h"
#include "ZFCore/ZFPathType_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes
{
public:
    void *fd; // fd returned by ZFFileResOpen or ZFFilePathInfoOpen
    ZFPathInfo resExtPath; // true if fd created by ZFFilePathInfoOpen

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(zfnull)
    , resExtPath()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileResExtPathDataHolder, ZFLevelZFFrameworkStatic)
{
}
public:
    ZFCoreArray<ZFPathInfo> resExtPathList;
ZF_GLOBAL_INITIALIZER_END(ZFFileResExtPathDataHolder)
#define _ZFP_ZFFileResExtPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFFileResExtPathDataHolder)->resExtPathList)

ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResExtPathAdd,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    if(pathInfo.pathType.isEmpty())
    {
        return ;
    }
    _ZFP_ZFFileResExtPathList.add(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResExtPathRemove,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    if(pathInfo.pathType.isEmpty())
    {
        return ;
    }
    _ZFP_ZFFileResExtPathList.removeElement(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFPathInfo>, ZFFileResExtPathList)
{
    return _ZFP_ZFFileResExtPathList;
}
ZFMETHOD_FUNC_DEFINE_1(const ZFPathInfo *, ZFFileResExtPathCheck,
                       ZFMP_IN(const zfchar *, resPath))
{
    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFFileResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i)
    {
        ZFPathInfo t = l[i];
        ZFFilePathInfoToChild(t, t.pathData, resPath);
        if(ZFFilePathInfoIsExist(t))
        {
            return &(l[i]);
        }
    }
    return zfnull;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsExist,
                       ZFMP_IN(const zfchar *, resPath))
{
    const ZFPathInfo *resExtPath = ZFFileResExtPathCheck(resPath);
    if(resExtPath != zfnull)
    {
        return zftrue;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFFileResProcess)->resIsExist(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsDir,
                       ZFMP_IN(const zfchar *, resPath))
{
    const ZFPathInfo *resExtPath = ZFFileResExtPathCheck(resPath);
    if(resExtPath != zfnull)
    {
        ZFPathInfo tmp = *resExtPath;
        ZFFilePathInfoToChild(tmp, tmp.pathData, resPath);
        return ZFFilePathInfoIsDir(tmp);
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFFileResProcess)->resIsDir(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFFileResCopy,
                       ZFMP_IN(const zfchar *, resPath),
                       ZFMP_IN(const zfchar *, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    const ZFPathInfo *resExtPath = ZFFileResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        return _ZFP_ZFFileResProcessImpl->resCopy(resPath, dstPath, isRecursive, isForce, errPos);
    }
    else
    {
        ZFPathInfo errPosTmp;
        zfbool ret = ZFFilePathInfoCopy(
            ZFPathInfo(ZFPathType_res(), resPath),
            ZFPathInfo(ZFPathType_file(), dstPath),
            isRecursive,
            isForce,
            errPos != zfnull ? &errPosTmp : zfnull);
        if(errPos != zfnull && !ret)
        {
            *errPos += errPosTmp.pathData;
        }
        return ret;
    }
}

#define _ZFP_ZFFileFindType_res "ZFFileResFindFirst"

zfclassNotPOD _ZFP_ZFFileResFindData
{
public:
    zfstring resPathSaved;
    ZFFileFindData resExtFd;
    /*
     * if not empty, the file is find from resExtPath
     * ensured end with file separator
     * ensured clear when find task ends from resExtPath
     */
    ZFPathInfo resExtPath;
    zfbool resFindFirstStarted;
public:
    _ZFP_ZFFileResFindData(void)
    : resPathSaved()
    , resExtFd()
    , resExtPath()
    , resFindFirstStarted(zffalse)
    {
    }
public:
    void copyToFd(ZF_OUT ZFFileFindData::Impl &fd) const
    {
        fd.fileName = this->resExtFd.impl().fileName;
        fd.fileIsDir = this->resExtFd.impl().fileIsDir;
    }
};

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileResFindFirst,
                       ZFMP_IN_OUT(ZFFileFindData &, fd),
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return zffalse;
    }
    _ZFP_ZFFileResFindData *implUserData = zfnew(_ZFP_ZFFileResFindData);
    fd.implAttach(_ZFP_ZFFileFindType_res, implUserData);
    implUserData->resPathSaved = resPath;

    const ZFPathInfo *resExtPath = ZFFileResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        if(!_ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), resPath))
        {
            fd.implDetach();
            zfdelete(implUserData);
            return zffalse;
        }
        return zftrue;
    }
    else
    {
        implUserData->resExtPath = *resExtPath;
        ZFPathInfo resPathTmp = implUserData->resExtPath;
        ZFFilePathInfoToChild(resPathTmp, resPathTmp.pathData, resPath);
        if(ZFFilePathInfoFindFirst(resPathTmp, implUserData->resExtFd))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }
        else
        {
            implUserData->resExtPath.pathType.removeAll();
            implUserData->resExtPath.pathData.removeAll();
            if(!_ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), resPath))
            {
                fd.implDetach();
                zfdelete(implUserData);
                return zffalse;
            }
            return zftrue;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResFindNext,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFFileResFindData *implUserData = (_ZFP_ZFFileResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType.isEmpty())
    {
        if(ZFFilePathInfoFindNext(implUserData->resExtPath, implUserData->resExtFd))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }

        ZFFilePathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);
        implUserData->resExtPath.pathType.removeAll();
        implUserData->resExtPath.pathData.removeAll();

        implUserData->resFindFirstStarted = _ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), implUserData->resPathSaved);
        return implUserData->resFindFirstStarted;
    }
    return _ZFP_ZFFileResProcessImpl->resFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResFindClose,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFFileResFindData *implUserData = (_ZFP_ZFFileResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType.isEmpty())
    {
        ZFFilePathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);
        implUserData->resExtPath.pathType.removeAll();
        implUserData->resExtPath.pathData.removeAll();
    }
    if(implUserData->resFindFirstStarted)
    {
        _ZFP_ZFFileResProcessImpl->resFindClose(fd.impl());
    }
    fd.implDetach();
    zfdelete(implUserData);
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFFileResOpen,
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return zfnull;
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    const ZFPathInfo *resExtPath = ZFFileResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        ret->fd = _ZFP_ZFFileResProcessImpl->resOpen(resPath);
    }
    else
    {
        ret->resExtPath = *resExtPath;
        ZFFilePathInfoToChild(ret->resExtPath, ret->resExtPath.pathData, resPath);
        ret->fd = ZFFilePathInfoOpen(ret->resExtPath, ZFFileOpenOption::e_Read);
    }
    if(ret->fd == zfnull)
    {
        zfdelete(ret);
        ret = zfnull;
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResClose,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    zfblockedDelete(resToken);
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resClose(resToken->fd);
    }
    else
    {
        return ZFFilePathInfoClose(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileResTell,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
    }
    else
    {
        return ZFFilePathInfoTell(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileResSeek,
                       ZFMP_IN(void *, token),
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, byteSize, position);
    }
    else
    {
        return ZFFilePathInfoSeek(resToken->resExtPath, resToken->fd, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileResRead,
                       ZFMP_IN(void *, token),
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, maxByteSize))
{
    if(token == zfnull)
    {
        return 0;
    }
    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        if(buf == zfnull)
        {
            zfindex curPos = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
            _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, 0, ZFSeekPosEnd);
            zfindex endPos = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
            _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, curPos, ZFSeekPosBegin);
            return endPos - curPos;
        }
        if(maxByteSize == zfindexMax())
        {
            return 0;
        }
        return _ZFP_ZFFileResProcessImpl->resRead(resToken->fd, buf, maxByteSize);
    }
    else
    {
        if(buf == zfnull)
        {
            zfindex curPos = ZFFilePathInfoTell(resToken->resExtPath, resToken->fd);
            if(curPos == zfindexMax())
            {
                return zfindexMax();
            }
            if(!ZFFilePathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd))
            {
                return zfindexMax();
            }
            zfindex endPos = ZFFilePathInfoTell(resToken->resExtPath, resToken->fd);
            ZFFilePathInfoSeek(resToken->resExtPath, resToken->fd, curPos, ZFSeekPosBegin);
            return endPos - curPos;
        }
        if(maxByteSize == zfindexMax())
        {
            return 0;
        }
        return ZFFilePathInfoRead(resToken->resExtPath, resToken->fd, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsEof,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resIsEof(resToken->fd);
    }
    else
    {
        return ZFFilePathInfoIsEof(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsError,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resIsError(resToken->fd);
    }
    else
    {
        return ZFFilePathInfoIsError(resToken->resExtPath, resToken->fd);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileResSize,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zfindexMax();
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        zfindex saved = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
        _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, 0, ZFSeekPosEnd);
        zfindex size = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
        _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, saved, ZFSeekPosBegin);
        return size;
    }
    else
    {
        zfindex saved = ZFFilePathInfoTell(resToken->resExtPath, resToken->fd);
        if(saved == zfindexMax())
        {
            return zfindexMax();
        }
        if(!ZFFilePathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd))
        {
            return zfindexMax();
        }
        zfindex size = ZFFilePathInfoTell(resToken->resExtPath, resToken->fd);
        ZFFilePathInfoSeek(resToken->resExtPath, resToken->fd, saved, ZFSeekPosBegin);
        return size;
    }
}

ZF_NAMESPACE_GLOBAL_END

