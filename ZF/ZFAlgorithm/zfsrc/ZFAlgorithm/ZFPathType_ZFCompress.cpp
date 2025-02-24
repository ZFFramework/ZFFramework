#include "ZFPathType_ZFCompress.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(ZFCompress)

// ============================================================
// ZFCompress
zfclassNotPOD _ZFP_ZFPathType_ZFCompress {
private:
    typedef enum {
        _TaskTypeCompress,
        _TaskTypeDecompress,
    } _TaskType;
    zfclassNotPOD _TaskData {
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
        ~_TaskData(void) {
            if(this->taskToken != zfnull) {
                if(this->taskType == _TaskTypeCompress) {
                    ZFCompressEnd(this->taskToken);
                }
                else {
                    ZFDecompressEnd(this->taskToken);
                }
            }
        }
    };
    typedef zfstlmap<const zfchar *, _TaskData *, zfcharConst_zfstlLess> _TaskMap;

    zfclassNotPOD _Token {
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
        ~_Token(void) {
            if(this->owner != zfnull) {
                _taskIdle(this->owner);
            }
            zfRelease(this->ioBuf);
        }
    };

public:
    static void _taskIdleCleanup(void) {
        ZFCoreArray<_TaskData *> toCleanup;

        {
            ZFCoreMutexLocker();
            _taskIdleTimer() = zfnull;

            _TaskMap &m = _taskMap();
            for(_TaskMap::iterator it = m.begin(); it != m.end(); ) {
                if(it->second->ioCount == 0) {
                    toCleanup.add(it->second);
                    it = m.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        for(zfindex i = 0; i < toCleanup.count(); ++i) {
            zfdelete(toCleanup[i]);
        }
    }
private:
    static zfautoT<ZFTimer> &_taskIdleTimer(void) {
        static zfautoT<ZFTimer> d;
        return d;
    }
    static void _taskIdle(ZF_IN _TaskData *taskData) {
        ZFCoreMutexLocker();
        --(taskData->ioCount);
        if(taskData->ioCount != 0) {
            return;
        }
        zfautoT<ZFTimer> &timer = _taskIdleTimer();
        if(timer == zfnull) {
            ZFLISTENER(delay) {
                _taskIdleCleanup();
            } ZFLISTENER_END();
            timer = ZFTimerOnce(100, delay);
            ZFLISTENER(delayOnStop) {
                ZFCoreMutexLocker();
                _taskIdleTimer() = zfnull;
            } ZFLISTENER_END();
            timer->observerAdd(ZFTimer::E_TimerOnStop(), delayOnStop);
        }
    }

private:
    // <compressFilePathInfo, taskData>
    static _TaskMap &_taskMap(void) {
        static _TaskMap m;
        return m;
    }

    static _TaskData *_taskCreate(
            ZF_OUT zfstring &pathData
            , ZF_IN_OUT const zfchar *pathDataOrig
            , ZF_IN _TaskType taskType
            ) {
        zfstring compressFilePathInfo;
        if(!ZFPathInfoChainDecodeString(pathDataOrig, compressFilePathInfo, &pathData)) {
            return zfnull;
        }

        _TaskData *taskData = zfnull;
        _TaskMap &m = _taskMap();
        {
            ZFCoreMutexLocker();
            _TaskMap::iterator it = m.find(compressFilePathInfo.cString());
            if(it != m.end()) {
                if(it->second->taskType != taskType) {
                    if(it->second->ioCount > 0) {
                        return zfnull;
                    }
                    _TaskData *toCleanup = it->second;
                    m.erase(it);
                    ZFCoreMutexUnlock();
                    zfdelete(toCleanup);
                    ZFCoreMutexLock();
                }
                else {
                    ++(it->second->ioCount);
                    return it->second;
                }
            }
            taskData = zfnew(_TaskData);
            taskData->compressFilePathInfo.swap(compressFilePathInfo);
            m[taskData->compressFilePathInfo.cString()] = taskData;
            taskData->taskType = taskType;
        }

        if(taskType == _TaskTypeCompress) {
            taskData->taskToken = ZFCompressBegin(ZFOutputForPathInfo(ZFPathInfo(taskData->compressFilePathInfo)));
        }
        else {
            taskData->taskToken = ZFDecompressBegin(ZFInputForPathInfo(ZFPathInfo(taskData->compressFilePathInfo)));
        }
        ZFCoreMutexLocker();
        if(taskData->taskToken == zfnull) {
            _taskRelease(taskData);
            return zfnull;
        }
        else {
            return taskData;
        }
    }

    static void _taskRelease(ZF_IN _TaskData *taskData) {
        ZFCoreMutexLock();
        --(taskData->ioCount);
        if(taskData->ioCount == 0) {
            _TaskMap &m = _taskMap();
            m.erase(taskData->compressFilePathInfo.cString());
            ZFCoreMutexUnlock();

            zfdelete(taskData);
        }
        else {
            ZFCoreMutexUnlock();
        }
    }

public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        if(!relPath) {
            return zftrue;
        }
        zfbool exist = (ZFDecompressContentIndex(taskData->taskToken, relPath) != zfindexMax())
            || (ZFDecompressContentIndex(taskData->taskToken, zfstr("%s%s", relPath, '/')) != zfindexMax());
        _taskIdle(taskData);
        return exist;
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        if(!relPath) {
            return zftrue;
        }
        zfbool isDir = ZFDecompressContentIsDir(taskData->taskToken, ZFDecompressContentIndex(taskData->taskToken,
                relPath[relPath.length() - 1] == '/' ? relPath : zfstr("%s/", relPath)
            ));
        _taskIdle(taskData);
        return isDir;
    }
    static zfstring callbackToFileName(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        return ZFPathInfoCallbackToFileNameDefault(pathData, success);
    }
    static zfstring callbackToChild(
            ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        if(zfstringIsEmpty(pathData)) {
            if(success) {
                *success = zffalse;
            }
            return zfnull;
        }

        zfstring ret;
        ret += pathData;
        if(ret[ret.length() - 1] != '|') {
            ret += '/';
        }
        ret += childName;
        if(success) {
            *success = zftrue;
        }
        return ret;
    }
    static zfstring callbackToParent(
            ZF_IN const zfchar *pathData
            , ZF_OUT_OPT zfbool *success = zfnull
            ) {
        zfstring compressFilePathInfo;
        zfstring pathDataTmp;
        if(!ZFPathInfoChainDecodeString(pathData, compressFilePathInfo, &pathDataTmp)) {
            if(success) {
                *success = zffalse;
            }
            return zfnull;
        }
        zfstring ret;
        ZFCoreDataEncode(ret, compressFilePathInfo, compressFilePathInfo.length(), ZFPathInfoChainCharMap());
        zfbool successTmp = zftrue;
        ret += ZFPathInfoCallbackToParentDefault(pathDataTmp, &successTmp);
        if(success) {
            *success = successTmp;
        }
        if(successTmp) {
            return ret;
        }
        else {
            return zfnull;
        }
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            , ZF_OUT_OPT zfstring *errPos
            ) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        zfbool ret = ZFCompressContentDir(taskData->taskToken, relPath);
        _taskIdle(taskData);
        return ret;
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            , ZF_IN_OPT zfstring *errPos
            ) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        zfbool ret = ZFCompressContentRemove(taskData->taskToken, relPath);
        _taskIdle(taskData);
        return ret;
    }
    static zfbool callbackMove(
            ZF_IN const zfchar *pathDataFrom
            , ZF_IN const zfchar *pathDataTo
            , ZF_IN_OPT zfbool isForce
            ) {
        zfstring relPathFrom;
        _TaskData *taskDataFrom = _taskCreate(relPathFrom, pathDataFrom, _TaskTypeCompress);
        zfstring relPathTo;
        _TaskData *taskDataTo = _taskCreate(relPathTo, pathDataTo, _TaskTypeCompress);
        zfbool ret = zffalse;
        if(taskDataFrom != zfnull && taskDataTo != zfnull) {
            ret = ZFCompressContentMove(taskDataFrom->taskToken, relPathFrom, relPathTo, isForce);
        }
        if(taskDataFrom != zfnull) {
            _taskIdle(taskDataFrom);
        }
        if(taskDataTo != zfnull) {
            _taskIdle(taskDataTo);
        }
        return ret;
    }
    static zfbool callbackFindFirst(
            ZF_IN_OUT ZFFileFindData &fd
            , ZF_IN const zfchar *pathData
            ) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        zfbool ret = ZFDecompressContentFindFirst(fd, taskData->taskToken, relPath);
        if(ret) {
            zfobj<ZFValueHolder> taskDataHolder(taskData, ZFValueHolderTypePointerRef());
            fd.implTag("_ZFP_ZFPathType_ZFCompress", taskDataHolder);
        }
        else {
            _taskIdle(taskData);
        }
        return ret;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        return ZFDecompressContentFindNext(fd);
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
        _TaskData *taskData = zfnull;
        ZFValueHolder *taskDataHolder = fd.implTag("_ZFP_ZFPathType_ZFCompress");
        if(taskDataHolder != zfnull) {
            taskData = taskDataHolder->holdedDataPointer<_TaskData *>();
        }
        fd.implTag("_ZFP_ZFPathType_ZFCompress", zfnull);
        ZFDecompressContentFindClose(fd);

        if(taskData != zfnull) {
            _taskIdle(taskData);
        }
    }
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        if(flag == ZFFileOpenOption::e_Read) {
            zfstring relPath;
            _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
            if(taskData == zfnull) {
                return zfnull;
            }
            _Token *d = zfnew(_Token);
            d->owner = taskData;
            if(!ZFDecompressContent(taskData->taskToken, d->ioBuf->output(), relPath)) {
                zfdelete(d);
                return zfnull;
            }
            d->relPath = relPath;
            return d;
        }
        else {
            zfstring relPath;
            _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
            if(taskData == zfnull) {
                return zfnull;
            }
            _Token *d = zfnew(_Token);
            d->owner = taskData;
            d->relPath = relPath;
            return d;
        }
    }
    static zfbool callbackClose(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            zfdelete(d);
            return zftrue;
        }
        else {
            zfbool ret = ZFCompressContent(d->owner->taskToken, d->ioBuf->input(), d->relPath);
            zfdelete(d);
            return ret;
        }
    }
    static zfindex callbackTell(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return d->ioBuf->input().ioTell();
        }
        else {
            return d->ioBuf->output().ioTell();
        }
    }
    static zfbool callbackSeek(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos position
            ) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return d->ioBuf->input().ioSeek(byteSize, position);
        }
        else {
            return d->ioBuf->output().ioSeek(byteSize, position);
        }

    }
    static zfindex callbackRead(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return d->ioBuf->input().execute(buf, maxByteSize);
        }
        else {
            return 0;
        }
    }
    static zfindex callbackWrite(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize
            ) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return 0;
        }
        else {
            return d->ioBuf->output().execute(src, maxByteSize);
        }
    }
    static void callbackFlush(ZF_IN void *token) {
    }
    static zfbool callbackIsEof(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return d->ioBuf->input().ioTell() == d->ioBuf->input().ioSize();
        }
        else {
            return zffalse;
        }
    }
    static zfbool callbackIsError(ZF_IN void *token) {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token) {
        _Token *d = (_Token *)token;
        if(d->owner->taskType == _TaskTypeDecompress) {
            return d->ioBuf->input().ioSize();
        }
        else {
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
        , _ZFP_ZFPathType_ZFCompress::callbackMove
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
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForCompressFile
        , ZFMP_IN(const ZFPathInfo &, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_ZFCompress()
                , ZFPathInfoChainEncode(compressFilePathInfo, relPath)
                ), flags);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForCompressFile
        , ZFMP_IN(const ZFPathInfo &, compressFilePathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_ZFCompress()
                , ZFPathInfoChainEncode(compressFilePathInfo, relPath)
                ), flags);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

