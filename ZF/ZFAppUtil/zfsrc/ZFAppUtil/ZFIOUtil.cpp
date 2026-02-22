#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFIORemoveOutdate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        , ZFMP_IN_OPT(zftimet, outdate, zftimetOneWeek())
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zfnull;
    }
    ZFLISTENER_4(impl
            , zfautoT<ZFIOImpl>, ioImpl
            , ZFPathInfo, pathInfo
            , ZFListener, filter
            , zftimet, outdate
            ) {
        if(!zfargs.param0()) {
            return;
        }
        zftimet curTime = ZFTime::currentTime();
        ZFArgs zfargsOwner = zfargs;
        ZFLISTENER_5(onFile
                , zfautoT<ZFIOImpl>, ioImpl
                , ZFArgs, zfargsOwner
                , ZFListener, filter
                , zftimet, outdate
                , zftimet, curTime
                ) {
            if(!zfargsOwner.param0()) {
                zfargs.eventFiltered(zftrue);
                return;
            }
            const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
            if(curTime - outdate > ioImpl->ioModTime(pathInfo.pathData())) {
                filter.execute(zfargs);
                if(zfargs.result() == zfnull) {
                    ioImpl->ioRemove(pathInfo.pathData());
                }
                else {
                    zfargs.result(zfnull);
                }
            }
        } ZFLISTENER_END()
        ZFIOForEachFile(pathInfo, onFile);
    } ZFLISTENER_END()
    return zfasync(impl);
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFIORemoveEmptyDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zfnull;
    }
    ZFLISTENER_3(impl
            , zfautoT<ZFIOImpl>, ioImpl
            , ZFPathInfo, pathInfo
            , ZFListener, filter
            ) {
        if(!zfargs.param0()) {
            return;
        }
        ZFArgs zfargsOwner = zfargs;
        ZFLISTENER_3(onFile
                , zfautoT<ZFIOImpl>, ioImpl
                , ZFArgs, zfargsOwner
                , ZFListener, filter
                ) {
            if(!zfargsOwner.param0()) {
                zfargs.eventFiltered(zftrue);
                return;
            }
            const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
            ZFIOFindData fd;
            if(!ioImpl->ioFindFirst(fd, pathInfo.pathData())) {
                filter.execute(zfargs);
                if(zfargs.result() == zfnull) {
                    ioImpl->ioRemove(pathInfo.pathData());
                    zfargs.param0(zfnull);
                }
                else {
                    zfargs.result(zfnull);
                }
            }
            else {
                ioImpl->ioFindClose(fd);
            }
        } ZFLISTENER_END()
        ZFIOForEachDir(pathInfo, onFile);
    } ZFLISTENER_END()
    return zfasync(impl);
}

// ============================================================
zfclass _ZFP_ZFIOForEachAsyncTask : zfextend ZFObject, zfimplement ZFTaskId {
    ZFOBJECT_DECLARE(_ZFP_ZFIOForEachAsyncTask, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFTaskId)
public:
    zfbool start(
            ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN const ZFListener &fileCallback
            , ZF_IN const ZFListener &finishCallback
            , ZF_IN zfbool isRecursive
            , ZF_IN zfbool forEachFile
            , ZF_IN zfbool forEachDir
            ) {
        _ioImpl = ZFIOImplForPathType(pathInfo.pathType());
        if(!_ioImpl
                || !_ioImpl->ioIsExist(pathInfo.pathData())
                ) {
            finishCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                    .param1(zfnull)
                    );
            return zffalse;
        }
        if(!_ioImpl->ioIsDir(pathInfo.pathData())) {
            ZFArgs zfargs;
            if(forEachFile) {
                ZFIOFindData fd;
                _ioImpl->ioToFileName(fd.impl().name, pathInfo.pathData());
                fd.impl().isDir = zffalse;
                fileCallback.execute(zfargs
                        .sender(zfobj<v_ZFIOFindData>(fd))
                        .param0(zfobj<v_ZFPathInfo>(pathInfo))
                        .param1(zfobj<v_zfstring>())
                        );
            }
            finishCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                    .param1(zfargs.result())
                    );
            return zffalse;
        }

        ZFIOFindData fd;
        if(!_ioImpl->ioFindFirst(fd, pathInfo.pathData())) {
            finishCallback.execute(ZFArgs()
                    .param0(zfobj<v_ZFResultType>(v_ZFResultType::e_Success))
                    .param1(zfnull)
                    );
            return zffalse;
        }
        _pathDataBase = pathInfo.pathData();
        _fdStack = zfobj<ZFArray>();
        _parentRelPathStack = zfobj<ZFArray>();
        _zfargsCacheReset();
        _fileCallback = fileCallback;
        _finishCallback = finishCallback;
        _isRecursive = isRecursive;
        _forEachFile = forEachFile;
        _forEachDir = forEachDir;
        _started = zftrue;

        zfself *owner = this;
        ZFLISTENER_1(impl
                , zfweakT<zfself>, owner
                ) {
            if(owner && owner->_started) {
                owner->_next();
            }
        } ZFLISTENER_END()
        _nextCallback = impl;

        _fdStack->add(zfobj<v_ZFIOFindData>(fd));
        _parentRelPathStack->add(zfobj<v_zfstring>());
        zfpost(_nextCallback);

        zfobjRetain(this); // release in _notifyFinish
        return zftrue;
    }
    zfoverride
    virtual void stop(void) {
        _notifyFinish(v_ZFResultType::e_Cancel);
    }
private:
    zfautoT<ZFIOImpl> _ioImpl;
    zfstring _pathDataBase;
    zfautoT<ZFArray> _fdStack; // v_ZFIOFindData
    zfautoT<ZFArray> _parentRelPathStack; // v_zfstring
    ZFArgs _zfargsCache;
    ZFListener _fileCallback;
    ZFListener _finishCallback;
    ZFListener _nextCallback;
    zfautoT<ZFTask> _customTask;
    zfbool _isRecursive;
    zfbool _forEachFile;
    zfbool _forEachDir;
    zfbool _started;
private:
    void _zfargsCacheReset(void) {
        _zfargsCache
            .sender(zfnull)
            .param0(zfnull)
            .param1(zfnull)
            .result(zfnull)
            .eventFiltered(zffalse)
            ;
    }
    void _notifyFinish(ZF_IN_OPT ZFResultType resultType = v_ZFResultType::e_Success) {
        if(!_started) {
            return;
        }
        _started = zffalse;
        ZFListener finishCallback = _finishCallback;
        zfauto result = _zfargsCache.result();

        if(_customTask) {
            _customTask->stop();
            _customTask = zfnull;
        }
        for(zfindex i = _fdStack->count() - 1; i != zfindexMax(); --i) {
            v_ZFIOFindData *fd = _fdStack->get(i);
            _ioImpl->ioFindClose(fd->zfv);
        }

        _ioImpl = zfnull;
        _pathDataBase = zfnull;
        _fdStack->removeAll();
        _parentRelPathStack->removeAll();
        _zfargsCacheReset();
        _fileCallback = zfnull;
        _finishCallback = zfnull;

        finishCallback.execute(ZFArgs()
                .param0(zfobj<v_ZFResultType>(resultType))
                .param1(result)
                );

        zfobjRelease(this); // retain in start
    }
    void _next(void) {
        v_ZFIOFindData *fd = _fdStack->getLast();
        v_zfstring *parentRelPath = _parentRelPathStack->getLast();
        {
            zfstring relPath;
            _ioImpl->ioToChild(relPath, parentRelPath->zfv, fd->zfv.name());
            zfstring pathDataAbs;
            _ioImpl->ioToChild(pathDataAbs, _pathDataBase, relPath);
            _zfargsCache
                .sender(fd)
                .param0(zfobj<v_ZFPathInfo>(ZFPathInfo(
                                _ioImpl->pathType()
                                , pathDataAbs
                                )))
                .param1(zfobj<v_zfstring>(relPath))
                ;
        }

        if(fd->zfv.isDir()) {
            if(_forEachDir) {
                _fileCallback.execute(_zfargsCache);
                if(_zfargsCache.eventFiltered()) {
                    _notifyFinish();
                    return;
                }
                if(_customTaskCheck()) {
                    return;
                }
            }
        }
        else {
            if(_forEachFile) {
                _fileCallback.execute(_zfargsCache);
                if(_zfargsCache.eventFiltered()) {
                    _notifyFinish();
                    return;
                }
                if(_customTaskCheck()) {
                    return;
                }
            }
        }
        _fdStackNext(fd->zfv);
    }
    zfbool _customTaskCheck(void) {
        _customTask = zfcast(ZFTask *, _zfargsCache.param0());
        if(!_customTask) {
            return zffalse;
        }
        zfself *owner = this;
        ZFLISTENER_1(onStop
                , zfweakT<zfself>, owner
                ) {
            ZFTask *task = zfargs.sender();
            if(!task || task->resultType() == v_ZFResultType::e_Cancel) {
                return;
            }
            owner->_customTask = zfnull;
            v_ZFIOFindData *fd = owner->_fdStack->getLast();
            owner->_fdStackNext(fd->zfv);
        } ZFLISTENER_END()
        _customTask->start();
        return zftrue;
    }
    void _fdStackNext(ZF_IN_OUT ZFIOFindData &fd) {
        if(!fd.isDir()
                || !_isRecursive
                ) {
            if(_ioImpl->ioFindNext(fd)) {
                zfpost(_nextCallback);
            }
            else {
                _fdStackPop();
            }
            return;
        }
        {
            v_ZFPathInfo *tmp = _zfargsCache.param0();
            if(!tmp || !tmp->zfv) { // skip dir
                if(_ioImpl->ioFindNext(fd)) {
                    zfpost(_nextCallback);
                }
                else {
                    _fdStackPop();
                }
                return;
            }
        }

        // dir, and search child
        ZFIOFindData fdChild;
        zfstring parentRelPath;
        _ioImpl->ioToChild(parentRelPath, _parentRelPathStack->getLast().to<v_zfstring *>()->zfv, fd.name());
        zfstring pathDataAbs;
        _ioImpl->ioToChild(pathDataAbs, _pathDataBase, parentRelPath);
        if(!_ioImpl->ioFindFirst(fdChild, pathDataAbs)) {
            _fdStackPop();
            return;
        }

        _fdStack->add(zfobj<v_ZFIOFindData>(fdChild));
        _parentRelPathStack->add(zfobj<v_zfstring>(parentRelPath));
        zfpost(_nextCallback);
    }
    void _fdStackPop(void) {
        {
            zfautoT<v_ZFIOFindData> fd = _fdStack->removeLastAndGet();
            _parentRelPathStack->removeLast();
            _ioImpl->ioFindClose(fd->zfv);
        }
        if(_fdStack->isEmpty()) {
            _notifyFinish();
            return;
        }
        ZFIOFindData &fd = _fdStack->getLast().to<v_ZFIOFindData *>()->zfv;
        if(_ioImpl->ioFindNext(fd)) {
            zfpost(_nextCallback);
        }
        else {
            _fdStackPop();
        }
    }
};
ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFTaskId>, ZFIOForEachAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    zfobj<_ZFP_ZFIOForEachAsyncTask> task;
    if(task->start(pathInfo, fileCallback, finishCallback, isRecursive, zftrue, zftrue)) {
        return task;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFTaskId>, ZFIOForEachFileAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    zfobj<_ZFP_ZFIOForEachAsyncTask> task;
    if(task->start(pathInfo, fileCallback, finishCallback, isRecursive, zftrue, zffalse)) {
        return task;
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfautoT<ZFTaskId>, ZFIOForEachDirAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFListener &, fileCallback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        ) {
    zfobj<_ZFP_ZFIOForEachAsyncTask> task;
    if(task->start(pathInfo, fileCallback, finishCallback, isRecursive, zffalse, zftrue)) {
        return task;
    }
    else {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFStyleLoadAsync
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zfnull;
    }
    ZFStyleUpdateBegin();
    ZFLISTENER_1(impl
            , zfautoT<ZFIOImpl>, ioImpl
            ) {
        const ZFIOFindData &fd = zfargs.sender().to<v_ZFIOFindData *>()->zfv;
        const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
        const zfstring &relPath = zfargs.param1().to<v_zfstring *>()->zfv;
        if(!ZFStyleLoadCheck(fd.name())) {
            zfargs.param0(zfnull);
            return;
        }
        if(!fd.isDir()) {
            ZFStyleLoadItem(ioImpl, pathInfo.pathData(), relPath);
        }
    } ZFLISTENER_END()
    ZFLISTENER_1(onFinish
            , ZFListener, finishCallback
            ) {
        ZFStyleUpdateEnd();
        finishCallback.execute(ZFArgs()
                .param0(zfargs.param0())
                );
    } ZFLISTENER_END()
    return ZFIOForEachAsync(pathInfo, impl, onFinish);
}

// ============================================================
ZFOBJECT_REGISTER(ZFStyleLoadAsyncTask)

ZFMETHOD_DEFINE_1(ZFStyleLoadAsyncTask, void, child
        , ZFMP_IN(const ZFPathInfo &, child)
        ) {
    if(child) {
        if(!this->taskList()) {
            this->taskList(zfobj<ZFArray>());
        }
        this->taskList()->add(zfobj<v_ZFPathInfo>(child));
    }
}
ZFMETHOD_DEFINE_1(ZFStyleLoadAsyncTask, void, child
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, child)
        ) {
    if(!this->taskList()) {
        this->taskList(zfobj<ZFArray>());
    }
    for(zfindex i = 0; i < child.count(); ++i) {
        this->taskList()->add(zfobj<v_ZFPathInfo>(child[i]));
    }
}
ZFMETHOD_DEFINE_1(ZFStyleLoadAsyncTask, void, child
        , ZFMP_IN(ZFArray *, child)
        ) {
    if(!this->taskList()) {
        this->taskList(zfobj<ZFArray>());
    }
    this->taskList()->addFrom(child);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFStyleLoadAsyncTask
        , ZFMP_IN(const ZFPathInfo &, child)
        ) {
    this->child(child);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFStyleLoadAsyncTask
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, child)
        ) {
    this->child(child);
}
ZFOBJECT_ON_INIT_DEFINE_1(ZFStyleLoadAsyncTask
        , ZFMP_IN(ZFArray *, child)
        ) {
    this->child(child);
}

void ZFStyleLoadAsyncTask::taskOnStart(void) {
    zfsuper::taskOnStart();
    if(this->taskList() == zfnull || this->taskList()->isEmpty()) {
        this->notifySuccess();
        return;
    }
    zfobj<ZFArray> taskList(this->taskList());
    zfclassNotPOD _Impl {
    public:
        static void next(
                ZF_IN const zfweakT<zfself> &owner
                , ZF_IN const zfautoT<ZFArray> &taskList
                , ZF_IN zfindex index
                ) {
            if(index >= taskList->count()) {
                owner->stop(v_ZFResultType::e_Success);
                return;
            }
            v_ZFPathInfo *pathInfo = taskList->get(index);
            if(pathInfo == zfnull || pathInfo->zfv.isEmpty()) {
                next(owner, taskList, index + 1);
                return;
            }

            ZFLISTENER_3(onFinish
                    , zfweakT<zfself>, owner
                    , zfautoT<ZFArray>, taskList
                    , zfindex, index
                    ) {
                ZFResultType resultType = zfargs.param0().to<v_ZFResultType *>()->zfv();
                owner->_task = zfnull;
                if(resultType != v_ZFResultType::e_Cancel) {
                    next(owner, taskList, index + 1);
                }
            } ZFLISTENER_END()
            owner->_task = ZFStyleLoadAsync(pathInfo->zfv, onFinish);
        }
    };
    _Impl::next(this, taskList, 0);
}
void ZFStyleLoadAsyncTask::taskOnStop(void) {
    if(_task) {
        _task->stop();
        _task = zfnull;
    }
    zfsuper::taskOnStop();
}

ZF_NAMESPACE_GLOBAL_END

