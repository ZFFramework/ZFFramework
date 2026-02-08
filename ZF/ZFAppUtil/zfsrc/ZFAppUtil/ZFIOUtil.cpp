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
                    .param0(zfnull)
                    );
            return zffalse;
        }
        if(!_ioImpl->ioIsDir(pathInfo.pathData())) {
            ZFArgs zfargs;
            if(forEachFile) {
                fileCallback.execute(zfargs
                        .param0(zfobj<v_ZFPathInfo>(pathInfo))
                        .param1(zfobj<v_zfstring>())
                        );
            }
            finishCallback.execute(ZFArgs()
                    .param0(zfargs.result())
                    );
            return zffalse;
        }

        ZFIOFindData fd;
        if(!_ioImpl->ioFindFirst(fd, pathInfo.pathData())) {
            finishCallback.execute(ZFArgs()
                    .param0(zfnull)
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

        zfself *owenr = this;
        ZFLISTENER_1(impl
                , zfweakT<zfself>, owenr
                ) {
            if(owenr && owenr->_started) {
                owenr->_next();
            }
        } ZFLISTENER_END()
        _nextCallback = impl;

        _fdStack->add(zfobj<v_ZFIOFindData>(fd));
        _parentRelPathStack->add(zfobj<v_zfstring>());
        zfpost(_nextCallback);
        return zftrue;
    }
    zfoverride
    virtual void stop(void) {
        if(!_started) {
            return;
        }
        _started = zffalse;

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
    void _notifyFinish(void) {
        ZFListener finishCallback = _finishCallback;
        zfauto result = _zfargsCache.result();
        this->stop();
        finishCallback.execute(ZFArgs()
                .param0(result)
                );
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

ZF_NAMESPACE_GLOBAL_END

