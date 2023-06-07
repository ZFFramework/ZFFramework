#include "ZFFile.h"

#include "ZFPathType_res.h"
#include "ZFPathType_file.h"

#include "protocol/ZFProtocolZFRes.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes
{
public:
    void *fd; // fd returned by ZFResOpen or ZFPathInfoOpen
    ZFPathInfo resExtPath; // true if fd created by ZFPathInfoOpen

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(zfnull)
    , resExtPath()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFResExtPathDataHolder, ZFLevelZFFrameworkStatic)
{
}
public:
    ZFCoreArray<ZFPathInfo> resExtPathList;
ZF_GLOBAL_INITIALIZER_END(ZFResExtPathDataHolder)
#define _ZFP_ZFResExtPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFResExtPathDataHolder)->resExtPathList)

ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathAdd,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    if(pathInfo.pathType.isEmpty())
    {
        return;
    }
    _ZFP_ZFResExtPathList.add(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResExtPathRemove,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    if(pathInfo.pathType.isEmpty())
    {
        return;
    }
    _ZFP_ZFResExtPathList.removeElement(pathInfo);
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFPathInfo>, ZFResExtPathList)
{
    return _ZFP_ZFResExtPathList;
}
ZFMETHOD_FUNC_DEFINE_1(const ZFPathInfo *, ZFResExtPathCheck,
                       ZFMP_IN(const zfchar *, resPath))
{
    ZFCoreArray<ZFPathInfo> &l = _ZFP_ZFResExtPathList;
    for(zfindex i = 0; i < l.count(); ++i)
    {
        ZFPathInfo t = l[i];
        ZFPathInfoToChild(t, t.pathData, resPath);
        if(ZFPathInfoIsExist(t))
        {
            return &(l[i]);
        }
    }
    return zfnull;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsExist,
                       ZFMP_IN(const zfchar *, resPath))
{
    const ZFPathInfo *resExtPath = ZFResExtPathCheck(resPath);
    if(resExtPath != zfnull)
    {
        return zftrue;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resIsExist(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsDir,
                       ZFMP_IN(const zfchar *, resPath))
{
    const ZFPathInfo *resExtPath = ZFResExtPathCheck(resPath);
    if(resExtPath != zfnull)
    {
        ZFPathInfo tmp = *resExtPath;
        ZFPathInfoToChild(tmp, tmp.pathData, resPath);
        return ZFPathInfoIsDir(tmp);
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resIsDir(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFResCopy,
                       ZFMP_IN(const zfchar *, resPath),
                       ZFMP_IN(const zfchar *, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zftrue),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    const ZFPathInfo *resExtPath = ZFResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resCopy(resPath, dstPath, isRecursive, isForce, errPos);
    }
    else
    {
        ZFPathInfo errPosTmp;
        zfbool ret = ZFPathInfoCopy(
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

#define _ZFP_ZFFileFindType_res "ZFResFindFirst"

zfclassNotPOD _ZFP_ZFResFindData
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
    _ZFP_ZFResFindData(void)
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

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFResFindFirst,
                       ZFMP_IN_OUT(ZFFileFindData &, fd),
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return zffalse;
    }
    _ZFP_ZFResFindData *implUserData = zfnew(_ZFP_ZFResFindData);
    fd.implAttach(_ZFP_ZFFileFindType_res, implUserData);
    implUserData->resPathSaved = resPath;

    const ZFPathInfo *resExtPath = ZFResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        if(!ZFPROTOCOL_ACCESS(ZFRes)->resFindFirst(fd.impl(), resPath))
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
        ZFPathInfoToChild(resPathTmp, resPathTmp.pathData, resPath);
        if(ZFPathInfoFindFirst(resPathTmp, implUserData->resExtFd))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }
        else
        {
            implUserData->resExtPath.removeAll();
            if(!ZFPROTOCOL_ACCESS(ZFRes)->resFindFirst(fd.impl(), resPath))
            {
                fd.implDetach();
                zfdelete(implUserData);
                return zffalse;
            }
            return zftrue;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResFindNext,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType.isEmpty())
    {
        if(ZFPathInfoFindNext(implUserData->resExtPath, implUserData->resExtFd))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }

        ZFPathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);
        implUserData->resExtPath.removeAll();

        implUserData->resFindFirstStarted = ZFPROTOCOL_ACCESS(ZFRes)->resFindFirst(fd.impl(), implUserData->resPathSaved);
        return implUserData->resFindFirstStarted;
    }
    return ZFPROTOCOL_ACCESS(ZFRes)->resFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFResFindClose,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFResFindData *implUserData = (_ZFP_ZFResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resExtPath.pathType.isEmpty())
    {
        ZFPathInfoFindClose(implUserData->resExtPath, implUserData->resExtFd);
        implUserData->resExtPath.removeAll();
    }
    if(implUserData->resFindFirstStarted)
    {
        ZFPROTOCOL_ACCESS(ZFRes)->resFindClose(fd.impl());
    }
    fd.implDetach();
    zfdelete(implUserData);
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFResOpen,
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return zfnull;
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    const ZFPathInfo *resExtPath = ZFResExtPathCheck(resPath);
    if(resExtPath == zfnull)
    {
        ret->fd = ZFPROTOCOL_ACCESS(ZFRes)->resOpen(resPath);
    }
    else
    {
        ret->resExtPath = *resExtPath;
        ZFPathInfoToChild(ret->resExtPath, ret->resExtPath.pathData, resPath);
        ret->fd = ZFPathInfoOpen(ret->resExtPath, ZFFileOpenOption::e_Read);
    }
    if(ret->fd == zfnull)
    {
        zfdelete(ret);
        ret = zfnull;
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResClose,
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
        return ZFPROTOCOL_ACCESS(ZFRes)->resClose(resToken->fd);
    }
    else
    {
        return ZFPathInfoClose(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResTell,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resTell(resToken->fd);
    }
    else
    {
        return ZFPathInfoTell(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFResSeek,
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
        return ZFPROTOCOL_ACCESS(ZFRes)->resSeek(resToken->fd, byteSize, position);
    }
    else
    {
        return ZFPathInfoSeek(resToken->resExtPath, resToken->fd, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFResRead,
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
            ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
            zfindex curPos = impl->resTell(resToken->fd);
            impl->resSeek(resToken->fd, 0, ZFSeekPosEnd);
            zfindex endPos = impl->resTell(resToken->fd);
            impl->resSeek(resToken->fd, curPos, ZFSeekPosBegin);
            return endPos - curPos;
        }
        if(maxByteSize == zfindexMax())
        {
            return 0;
        }
        return ZFPROTOCOL_ACCESS(ZFRes)->resRead(resToken->fd, buf, maxByteSize);
    }
    else
    {
        if(buf == zfnull)
        {
            zfindex curPos = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
            if(curPos == zfindexMax())
            {
                return zfindexMax();
            }
            if(!ZFPathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd))
            {
                return zfindexMax();
            }
            zfindex endPos = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
            ZFPathInfoSeek(resToken->resExtPath, resToken->fd, curPos, ZFSeekPosBegin);
            return endPos - curPos;
        }
        if(maxByteSize == zfindexMax())
        {
            return 0;
        }
        return ZFPathInfoRead(resToken->resExtPath, resToken->fd, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsEof,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resIsEof(resToken->fd);
    }
    else
    {
        return ZFPathInfoIsEof(resToken->resExtPath, resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFResIsError,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        return ZFPROTOCOL_ACCESS(ZFRes)->resIsError(resToken->fd);
    }
    else
    {
        return ZFPathInfoIsError(resToken->resExtPath, resToken->fd);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFResSize,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zfindexMax();
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resExtPath.pathType.isEmpty())
    {
        ZFPROTOCOL_INTERFACE_CLASS(ZFRes) *impl = ZFPROTOCOL_ACCESS(ZFRes);
        zfindex saved = impl->resTell(resToken->fd);
        impl->resSeek(resToken->fd, 0, ZFSeekPosEnd);
        zfindex size = impl->resTell(resToken->fd);
        impl->resSeek(resToken->fd, saved, ZFSeekPosBegin);
        return size;
    }
    else
    {
        zfindex saved = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
        if(saved == zfindexMax())
        {
            return zfindexMax();
        }
        if(!ZFPathInfoSeek(resToken->resExtPath, resToken->fd, 0, ZFSeekPosEnd))
        {
            return zfindexMax();
        }
        zfindex size = ZFPathInfoTell(resToken->resExtPath, resToken->fd);
        ZFPathInfoSeek(resToken->resExtPath, resToken->fd, saved, ZFSeekPosBegin);
        return size;
    }
}

ZF_NAMESPACE_GLOBAL_END

