#include "ZFPathType_compress.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(compress)

// ============================================================
// ZFCompress
zfclassNotPOD _ZFP_ZFPathType_compress {
private:
    typedef enum {
        _TaskTypeCompress,
        _TaskTypeDecompress,
    } _TaskType;
    zfclassNotPOD _TaskData {
    public:
        zfstring refPathInfo;
        _TaskType taskType;
        zfauto taskToken; // ZFCompress / ZFDecompress token
        zfuint ioCount; // input / output task count
                        // maybe zero when in idle state (_taskIdle)
                        // would be cleaned by _taskIdleCleanup
    public:
        _TaskData(void)
        : refPathInfo()
        , taskType()
        , taskToken()
        , ioCount(1)
        {
        }
    };
    typedef zfstlhashmap<const zfchar *, _TaskData *, zfcharConst_zfstlHash, zfcharConst_zfstlEqual> _TaskMap;

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
                    m.erase(it++);
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
    // <refPathInfo, taskData>
    static _TaskMap &_taskMap(void) {
        static _TaskMap m;
        return m;
    }

    static _TaskData *_taskCreate(
            ZF_OUT zfstring &pathData
            , ZF_IN_OUT const zfchar *pathDataOrig
            , ZF_IN _TaskType taskType
            ) {
        zfstring refPathInfo;
        if(!ZFPathInfoChainDecodeString(refPathInfo, pathData, pathDataOrig)) {
            return zfnull;
        }

        _TaskData *taskData = zfnull;
        _TaskMap &m = _taskMap();
        {
            ZFCoreMutexLocker();
            _TaskMap::iterator it = m.find(refPathInfo.cString());
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
            taskData->refPathInfo.swap(refPathInfo);
            m[taskData->refPathInfo.cString()] = taskData;
            taskData->taskType = taskType;
        }

        zfbool success = zffalse;
        if(taskType == _TaskTypeCompress) {
            zfobj<ZFCompress> t;
            if(t->open(ZFOutputForPathInfo(ZFPathInfo(taskData->refPathInfo)))) {
                taskData->taskToken = t;
                success = zftrue;
            }
        }
        else {
            zfobj<ZFDecompress> t;
            if(t->open(ZFInputForPathInfo(ZFPathInfo(taskData->refPathInfo)))) {
                taskData->taskToken = t;
                success = zftrue;
            }
        }
        ZFCoreMutexLocker();
        if(!success) {
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
            m.erase(taskData->refPathInfo.cString());
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
        ZFDecompress *t = taskData->taskToken;
        zfbool exist = (zffalse
                || t->contentIndex(relPath) != zfindexMax()
                || t->contentIndex(zfstr("%s%s", relPath, '/')) != zfindexMax()
                );
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
        ZFDecompress *t = taskData->taskToken;
        zfbool isDir = t->isDir(t->contentIndex(
                    relPath[relPath.length() - 1] == '/' ? relPath : zfstr("%s/", relPath)
                    ));
        _taskIdle(taskData);
        return isDir;
    }
    static zfbool callbackToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        zfstring refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecodeString(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        return ZFPathInfoCallbackToFileNameDefault(ret, selfPathData);
    }
    static zfbool callbackToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            , ZF_IN const zfchar *childName
            ) {
        if(zfstringIsEmpty(childName)) {
            return zftrue;
        }

        zfstring refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecodeString(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        if(!(pathData >= ret.cString() && pathData < ret.cString() + ret.length())) {
            ret += pathData;
        }
        if(selfPathData) {
            ret += "/";
        }
        ret += childName;
        return zftrue;
    }
    static zfbool callbackToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfchar *pathData
            ) {
        ZFPathInfo refPathInfo;
        zfstring selfPathData;
        if(!ZFPathInfoChainDecode(refPathInfo, selfPathData, pathData)) {
            return zffalse;
        }
        ZFPathInfoCallbackToParentDefault(selfPathData, selfPathData);
        if(pathData >= ret.cString() && pathData < ret.cString() + ret.length()) {
            ret.removeAll();
        }
        ZFPathInfoChainEncodeT(ret, refPathInfo, selfPathData);
        return zftrue;
    }
    static zfbool callbackPathCreate(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool autoMakeParent
            ) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        ZFCompress *t = taskData->taskToken;
        zfbool ret = t->createDir(relPath);
        _taskIdle(taskData);
        return ret;
    }
    static zfbool callbackRemove(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT zfbool isRecursive
            , ZF_IN_OPT zfbool isForce
            ) {
        zfstring relPath;
        _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeCompress);
        if(taskData == zfnull) {
            return zffalse;
        }
        ZFCompress *t = taskData->taskToken;
        zfbool ret = t->remove(relPath);
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
            ZFCompress *t = taskDataFrom->taskToken;
            ret = t->move(relPathFrom, relPathTo, isForce);
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
        ZFDecompress *t = taskData->taskToken;
        zfbool ret = t->findFirst(fd, relPath);
        if(ret) {
            zfobj<ZFValueHolder> taskDataHolder(taskData, ZFValueHolderTypePointerRef());
            fd.implTag("_ZFP_ZFPathType_compress", taskDataHolder);
        }
        else {
            _taskIdle(taskData);
        }
        return ret;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd) {
        _TaskData *taskData = zfnull;
        ZFValueHolder *taskDataHolder = fd.implTag("_ZFP_ZFPathType_compress");
        if(taskDataHolder != zfnull) {
            taskData = taskDataHolder->holdedDataPointer<_TaskData *>();
        }
        ZFDecompress *t = taskData->taskToken;
        return t->findNext(fd);
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd) {
        _TaskData *taskData = zfnull;
        ZFValueHolder *taskDataHolder = fd.implTag("_ZFP_ZFPathType_compress");
        if(taskDataHolder != zfnull) {
            taskData = taskDataHolder->holdedDataPointer<_TaskData *>();
        }
        ZFDecompress *t = taskData->taskToken;
        t->findClose(fd);

        if(taskData != zfnull) {
            _taskIdle(taskData);
        }

        fd.implTag("_ZFP_ZFPathType_compress", zfnull);
    }
    static void *callbackOpen(
            ZF_IN const zfchar *pathData
            , ZF_IN_OPT ZFFileOpenOptionFlags flag
            , ZF_IN_OPT zfbool autoCreateParent
            ) {
        if(flag == v_ZFFileOpenOption::e_Read) {
            zfstring relPath;
            _TaskData *taskData = _taskCreate(relPath, pathData, _TaskTypeDecompress);
            if(taskData == zfnull) {
                return zfnull;
            }
            _Token *d = zfnew(_Token);
            d->owner = taskData;
            ZFDecompress *t = taskData->taskToken;
            if(!t->output(d->ioBuf->output(), relPath)) {
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
            ZFCompress *t = d->owner->taskToken;
            zfbool ret = t->output(d->relPath, d->ioBuf->input());
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
ZFPATHTYPE_FILEIO_REGISTER(compress, ZFPathType_compress()
        , _ZFP_ZFPathType_compress::callbackIsExist
        , _ZFP_ZFPathType_compress::callbackIsDir
        , _ZFP_ZFPathType_compress::callbackToFileName
        , _ZFP_ZFPathType_compress::callbackToChild
        , _ZFP_ZFPathType_compress::callbackToParent
        , _ZFP_ZFPathType_compress::callbackPathCreate
        , _ZFP_ZFPathType_compress::callbackRemove
        , _ZFP_ZFPathType_compress::callbackMove
        , _ZFP_ZFPathType_compress::callbackFindFirst
        , _ZFP_ZFPathType_compress::callbackFindNext
        , _ZFP_ZFPathType_compress::callbackFindClose
        , _ZFP_ZFPathType_compress::callbackOpen
        , _ZFP_ZFPathType_compress::callbackClose
        , _ZFP_ZFPathType_compress::callbackTell
        , _ZFP_ZFPathType_compress::callbackSeek
        , _ZFP_ZFPathType_compress::callbackRead
        , _ZFP_ZFPathType_compress::callbackWrite
        , _ZFP_ZFPathType_compress::callbackFlush
        , _ZFP_ZFPathType_compress::callbackIsEof
        , _ZFP_ZFPathType_compress::callbackIsError
        , _ZFP_ZFPathType_compress::callbackSize
    )

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Read)
        ) {
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_compress()
                , ZFPathInfoChainEncode(refPathInfo, relPath)
                ), flags);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForCompress
        , ZFMP_IN(const ZFPathInfo &, refPathInfo)
        , ZFMP_IN(const zfchar *, relPath)
        , ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, v_ZFFileOpenOption::e_Create)
        ) {
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, ZFPathInfo(
                ZFPathType_compress()
                , ZFPathInfoChainEncode(refPathInfo, relPath)
                ), flags);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

