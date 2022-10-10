#include "ZFPathType_ZFCompress.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(ZFCompress)

// ============================================================
// ZFCompress
zfclassNotPOD _ZFP_ZFPathType_ZFCompress
{
private:
    typedef enum {
        _TaskTypeCompress,
        _TaskTypeDecompress,
    } _TaskType;
    zfclassNotPOD _TaskData
    {
    public:
        zfstring compressFilePathInfo;
        _TaskType taskType;
        void *taskToken; // compress / decompress token
        zfuint ioCount; // input / output task count
                        // maybe zero when in idle state (_taskIdle)
                        // would be cleaned by _taskIdleCleanup
    public:
        _TaskData(void)
        : compressFilePathInfo()
        , taskType()
        , taskToken(zfnull)
        , ioCount(1)
        {
        }
        ~_TaskData(void)
        {
            if(this->taskToken != zfnull)
            {
                if(this->taskType == _TaskTypeCompress)
                {
                    ZFCompressEnd(this->taskToken);
                }
                else
                {
                    ZFDecompressEnd(this->taskToken);
                }
            }
        }
    };
    typedef zfstlmap<const zfchar *, _TaskData *, zfcharConst_zfstlComparer> _TaskMap;

    zfclassNotPOD _Token
    {
    public:
        _TaskData *owner;
        zfstring relPath;
        ZFIOBuffer *ioBuf;
    public:
        _Token(void)
        : owner(zfnull)
        , relPath()
        , ioBuf(zfAlloc(ZFIOBufferByCacheFile))
        {
        }
        ~_Token(void)
        {
            if(this->owner != zfnull)
            {
                _taskIdle(this->owner);
            }
            zfRelease(this->ioBuf);
        }
    };

public:
    static void _taskIdleCleanup(void)
    {
        ZFCoreArrayPOD<_TaskData *> toCleanup;

        {
            zfCoreMutexLocker();
            _taskIdleTimer() = zfnull;

            _TaskMap &m = _taskMap();
            for(_TaskMap::iterator it = m.begin(); it != m.end(); )
            {
                if(it->second->ioCount == 0)
                {
                    toCleanup.add(it->second);
                    it = m.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        for(zfindex i = 0; i < toCleanup.count(); ++i)
        {
            zfdelete(toCleanup[i]);
        }
    }
private:
    static zfautoObjectT<ZFTimer *> &_taskIdleTimer(void)
    {
        static zfautoObjectT<ZFTimer *> d;
        return d;
    }
    static void _taskIdle(ZF_IN _TaskData *taskData)
    {
        zfCoreMutexLocker();
        --(taskData->ioCount);
        if(taskData->ioCount != 0)
        {
            return;
        }
        zfautoObjectT<ZFTimer *> &timer = _taskIdleTimer();
        if(timer == zfnull)
        {
            ZFLISTENER(delay) {
                _taskIdleCleanup();
            } ZFLISTENER_END(delay);
            timer = ZFTimerOnce(100, delay);
            ZFLISTENER(delayOnStop) {
                zfCoreMutexLocker();
                _taskIdleTimer() = zfnull;
            } ZFLISTENER_END(delayOnStop);
            timer->observerAdd(ZFTimer::EventTimerOnStop(), delayOnStop);
        }
    }

private:
    // <compressFilePathInfo, taskData>
    static _TaskMap &_taskMap(void)
    {
        static _TaskMap m;
        return m;
    }

    static _TaskData *_taskCreate(ZF_OUT const zfchar *&pathData,
                                  ZF_IN_OUT const zfchar *pathDataOrig,
                                  ZF_IN _TaskType taskType)
    {
        zfstring compressFilePathInfo;
        if(!ZFPathInfoChainParse(compressFilePathInfo, pathData, pathDataOrig))
        {
            return zfnull;
        }

        _TaskData *taskData = zfnull;
        _TaskMap &m = _taskMap();
        {
            zfCoreMutexLocker();
            _TaskMap::iterator it = m.find(compressFilePathInfo.cString());
            if(it != m.end())
            {
                if(it->second->taskType != taskType)
                {
                    if(it->second->ioCount > 0)
                    {
                        return zfnull;
                    }
                    _TaskData *toCleanup = it->second;
                    m.erase(it);
                    zfCoreMutexUnlock();
                    zfdelete(toCleanup);
                    zfCoreMutexLock();
                }
                else
                {
                    ++(it->second->ioCount);
                    return it->second;
                }
            }
            taskData = zfnew(_TaskData);
            taskData->compressFilePathInfo.swap(compressFilePathInfo);
            m[taskData->compressFilePathInfo.cString()] = taskData;
            taskData->taskType = taskType;
        }

        if(taskType == _TaskTypeCompress)
        {
            taskData->taskToken = ZFCompressBegin(ZFOutputForPathInfoString(taskData->compressFilePathInfo));
        }
        else
        {
            taskData->taskToken = ZFDecompressBegin(ZFInputForPathInfoString(taskData->compressFilePathInfo));
        }
        zfCoreMutexLocker();
        if(taskData->taskToken == zfnull)
        {
            _taskRelease(taskData);
            return zfnull;
        }
        else
        {
            return taskData;
        }
    }

    static void _taskRelease(ZF_IN _TaskData *taskData)
    {
        zfCoreMutexLock();
        --(taskData->ioCount);
        if(taskData->ioCount == 0)
        {
            _TaskMap &m = _taskMap();
            m.erase(taskData->compressFilePathInfo.cString());
            zfCoreMutexUnlock();

            zfdelete(taskData);
        }
        else
        {
            zfCoreMutexUnlock();
        }
    }

public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData)
    {
        const zfchar *relPath = zfnull;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull)
        {
            return zffalse;
        }
        while(*relPath == ZFFileSeparator())
        {
            ++relPath;
        }
        if(zfsIsEmpty(relPath))
        {
            return zftrue;
        }
        zfbool exist = (ZFDecompressContentIndex(taskData->taskToken, relPath) != zfindexMax())
            || (ZFDecompressContentIndex(taskData->taskToken, zfstringWithFormat("%s%c", relPath, ZFFileSeparator())) != zfindexMax());
        _taskIdle(taskData);
        return exist;
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData)
    {
        const zfchar *relPath = zfnull;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull)
        {
            return zffalse;
        }
        while(*relPath == ZFFileSeparator())
        {
            ++relPath;
        }
        if(zfsIsEmpty(relPath))
        {
            return zftrue;
        }
        zfbool isDir = ZFDecompressContentIsDir(taskData->taskToken, ZFDecompressContentIndex(taskData->taskToken,
                relPath[zfslen(relPath) - 1] == ZFFileSeparator() ? relPath : zfstringWithFormat("%s%c", relPath, ZFFileSeparator()).cString()
            ));
        _taskIdle(taskData);
        return isDir;
    }
    static zfbool callbackToFileName(ZF_IN const zfchar *pathData,
                                     ZF_IN_OUT zfstring &fileName)
    {
        return ZFFilePathInfoCallbackToFileNameDefault(pathData, fileName);
    }
    static zfbool callbackToChild(ZF_IN const zfchar *pathData,
                                  ZF_IN_OUT zfstring &pathDataChild,
                                  ZF_IN const zfchar *childName)
    {
        zfindex pathDataLen = zfslen(pathData);
        if(pathDataLen <= 0)
        {
            return zffalse;
        }

        if(!(pathData >= pathDataChild.cString() && pathData < pathDataChild.cString() + pathDataChild.length()))
        {
            pathDataChild += pathData;
        }
        if(pathData[pathDataLen - 1] != '|')
        {
            pathDataChild += ZFFileSeparator();
        }
        pathDataChild += childName;

        return zftrue;
    }
    static zfbool callbackToParent(ZF_IN const zfchar *pathData,
                                   ZF_IN_OUT zfstring &pathDataParent)
    {
        zfstring compressFilePathInfo;
        const zfchar *pathDataTmp = zfnull;
        if(!ZFPathInfoChainParse(compressFilePathInfo, pathDataTmp, pathData))
        {
            return zffalse;
        }
        zfCoreDataEncode(pathDataParent, compressFilePathInfo, compressFilePathInfo.length(), ZFPathInfoChainCharMap());
        return ZFFilePathInfoCallbackToParentDefault(pathDataTmp, pathDataParent);
    }
    static zfbool callbackPathCreate(ZF_IN const zfchar *pathData,
                                     ZF_IN_OPT zfbool autoMakeParent,
                                     ZF_OUT_OPT zfstring *errPos)
    {
        const zfchar *relPath = zfnull;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull)
        {
            return zffalse;
        }
        zfbool ret = ZFCompressContentDir(taskData->taskToken, relPath);
        _taskIdle(taskData);
        return ret;
    }
    static zfbool callbackRemove(ZF_IN const zfchar *pathData,
                                 ZF_IN_OPT zfbool isRecursive,
                                 ZF_IN_OPT zfbool isForce,
                                 ZF_IN_OPT zfstring *errPos)
    {
        const zfchar *relPath = zfnull;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull)
        {
            return zffalse;
        }
        zfbool ret = ZFCompressContentRemove(taskData->taskToken, relPath);
        _taskIdle(taskData);
        return ret;
    }
    static zfbool callbackFindFirst(ZF_IN_OUT ZFFileFindData &fd,
                                    ZF_IN const zfchar *pathData)
    {
        const zfchar *relPath = zfnull;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull)
        {
            return zffalse;
        }
        zfbool ret = ZFDecompressContentFindFirst(fd, taskData->taskToken, relPath);
        if(ret)
        {
            zfblockedAlloc(ZFValueHolder, taskDataHolder, taskData, ZFValueHolderTypePointerRef);
            fd.implTag("_ZFP_ZFPathType_ZFCompress", taskDataHolder);
        }
        else
        {
            _taskIdle(taskData);
        }
        return ret;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd)
    {
        return ZFDecompressContentFindNext(fd);
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd)
    {
        _TaskData *taskData = zfnull;
        ZFValueHolder *taskDataHolder = ZFCastZFObject(ZFValueHolder *, fd.implTag("_ZFP_ZFPathType_ZFCompress"));
        if(taskDataHolder != zfnull)
        {
            taskData = taskDataHolder->holdedDataPointer<_TaskData *>();
        }
        fd.implTag("_ZFP_ZFPathType_ZFCompress", zfnull);
        ZFDecompressContentFindClose(fd);

        if(taskData != zfnull)
        {
            _taskIdle(taskData);
        }
    }
    static void *callbackOpen(ZF_IN const zfchar *pathData,
                              ZF_IN_OPT ZFFileOpenOptionFlags flag,
                              ZF_IN_OPT zfbool autoCreateParent)
    {
        if(flag == ZFFileOpenOption::e_Read)
        {
            const zfchar *relPath = zfnull;
            _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
            if(taskData == zfnull)
            {
                return zfnull;
            }
            _Token *d = zfnew(_Token);
            d->owner = taskData;
            if(!ZFDecompressContent(taskData->taskToken, d->ioBuf->output(), relPath))
            {
                zfdelete(d);
                return zfnull;
            }
            d->relPath = relPath;
            return d;
        }
        else
        {
            const zfchar *relPath = zfnull;
            _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
            if(taskData == zfnull)
            {
                return zfnull;
            }
            _Token *d = zfnew(_Token);
            d->owner = taskData;
            d->relPath = relPath;
            return d;
        }
    }
    static zfbool callbackClose(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            zfdelete(d);
            return zftrue;
        }
        else
        {
            zfbool ret = ZFCompressContent(d->owner->taskToken, d->ioBuf->input(), d->relPath);
            zfdelete(d);
            return ret;
        }
    }
    static zfindex callbackTell(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return d->ioBuf->input().ioTell();
        }
        else
        {
            return d->ioBuf->output().ioTell();
        }
    }
    static zfbool callbackSeek(ZF_IN void *token,
                               ZF_IN zfindex byteSize,
                               ZF_IN_OPT ZFSeekPos position)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return d->ioBuf->input().ioSeek(byteSize, position);
        }
        else
        {
            return d->ioBuf->output().ioSeek(byteSize, position);
        }

    }
    static zfindex callbackRead(ZF_IN void *token,
                                ZF_IN void *buf,
                                ZF_IN zfindex maxByteSize)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return d->ioBuf->input().execute(buf, maxByteSize);
        }
        else
        {
            return 0;
        }
    }
    static zfindex callbackWrite(ZF_IN void *token,
                                 ZF_IN const void *src,
                                 ZF_IN_OPT zfindex maxByteSize)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return 0;
        }
        else
        {
            return d->ioBuf->output().execute(src, maxByteSize);
        }
    }
    static void callbackFlush(ZF_IN void *token)
    {
    }
    static zfbool callbackIsEof(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return d->ioBuf->input().ioTell() == d->ioBuf->input().ioSize();
        }
        else
        {
            return zffalse;
        }
    }
    static zfbool callbackIsError(ZF_IN void *token)
    {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress)
        {
            return d->ioBuf->input().ioSize();
        }
        else
        {
            return d->ioBuf->output().ioSize();
        }
    }
};
ZFPATHTYPE_FILEIO_REGISTER(ZFCompress, ZFPathType_ZFCompress()
        , _ZFP_ZFPathType_ZFCompress::callbackIsExist
        , _ZFP_ZFPathType_ZFCompress::callbackIsDir
        , _ZFP_ZFPathType_ZFCompress::callbackToFileName
        , _ZFP_ZFPathType_ZFCompress::callbackToChild
        , _ZFP_ZFPathType_ZFCompress::callbackToParent
        , _ZFP_ZFPathType_ZFCompress::callbackPathCreate
        , _ZFP_ZFPathType_ZFCompress::callbackRemove
        , _ZFP_ZFPathType_ZFCompress::callbackFindFirst
        , _ZFP_ZFPathType_ZFCompress::callbackFindNext
        , _ZFP_ZFPathType_ZFCompress::callbackFindClose
        , _ZFP_ZFPathType_ZFCompress::callbackOpen
        , _ZFP_ZFPathType_ZFCompress::callbackClose
        , _ZFP_ZFPathType_ZFCompress::callbackTell
        , _ZFP_ZFPathType_ZFCompress::callbackSeek
        , _ZFP_ZFPathType_ZFCompress::callbackRead
        , _ZFP_ZFPathType_ZFCompress::callbackWrite
        , _ZFP_ZFPathType_ZFCompress::callbackFlush
        , _ZFP_ZFPathType_ZFCompress::callbackIsEof
        , _ZFP_ZFPathType_ZFCompress::callbackIsError
        , _ZFP_ZFPathType_ZFCompress::callbackSize
    )

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForCompressFile,
                       ZFMP_IN(const zfchar *, compressFilePathInfo),
                       ZFMP_IN(const zfchar *, relPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    zfstring pathData;
    if(ZFPathInfoVerify(compressFilePathInfo))
    {
        zfCoreDataEncode(pathData, compressFilePathInfo, zfindexMax(), ZFPathInfoChainCharMap());
    }
    else
    {
        pathData += ZFPathType_file();
        pathData += ZFSerializableKeyword_ZFPathInfo_separator;
        zfCoreDataEncode(pathData, compressFilePathInfo, zfindexMax(), ZFPathInfoChainCharMap());
    }
    pathData += '|';
    pathData += relPath;

    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathType_ZFCompress(), pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForCompressFile,
                       ZFMP_IN(const ZFPathInfo &, compressFilePathInfo),
                       ZFMP_IN(const zfchar *, relPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    zfstring pathData;
    pathData += compressFilePathInfo.pathType;
    pathData += ZFSerializableKeyword_ZFPathInfo_separator;
    zfCoreDataEncode(pathData, compressFilePathInfo.pathData, zfindexMax(), ZFPathInfoChainCharMap());
    pathData += '|';
    pathData += relPath;

    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathType_ZFCompress(), pathData, flags);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForCompressFile,
                       ZFMP_IN(const zfchar *, compressFilePathInfo),
                       ZFMP_IN(const zfchar *, relPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfstring pathData;
    if(ZFPathInfoVerify(compressFilePathInfo))
    {
        zfCoreDataEncode(pathData, compressFilePathInfo, zfindexMax(), ZFPathInfoChainCharMap());
    }
    else
    {
        pathData += ZFPathType_file();
        pathData += ZFSerializableKeyword_ZFPathInfo_separator;
        zfCoreDataEncode(pathData, compressFilePathInfo, zfindexMax(), ZFPathInfoChainCharMap());
    }
    pathData += '|';
    pathData += relPath;

    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathType_ZFCompress(), pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForCompressFile,
                       ZFMP_IN(const ZFPathInfo &, compressFilePathInfo),
                       ZFMP_IN(const zfchar *, relPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfstring pathData;
    pathData += compressFilePathInfo.pathType;
    pathData += ZFSerializableKeyword_ZFPathInfo_separator;
    zfCoreDataEncode(pathData, compressFilePathInfo.pathData, zfindexMax(), ZFPathInfoChainCharMap());
    pathData += '|';
    pathData += relPath;

    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathType_ZFCompress(), pathData, flags);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

