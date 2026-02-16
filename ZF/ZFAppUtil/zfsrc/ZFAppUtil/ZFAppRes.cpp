#include "ZFAppRes.h"
#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_DEFINE_4(ZFAppRes, void, build
        , ZFMP_OUT(const ZFPathInfo &, resultPackage)
        , ZFMP_IN(const ZFPathInfo &, sourceDir)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        , ZFMP_IN_OPT(const zfstring &, relPathInPackage, zfnull)
        ) {
    zfautoT<ZFCompressToken> ioToken = ZFCompressOpen(resultPackage, v_ZFIOOpenOption::e_Write);
    zfstring prefix = "zfres/";
    if(relPathInPackage) {
        prefix += relPathInPackage;
        prefix += "/";
    }
    ZFLISTENER_3(impl
            , zfautoT<ZFCompressToken>, ioToken
            , zfstring, packagePwd
            , zfstring, prefix
            ) {
        zfstring itemPath = zfstr("%s%s", prefix, zfargs.param1().to<v_zfstring *>()->zfv);
        if(zfstringEndWith(itemPath, "ZF_PUT_RES_FILES_HERE")) {
            return;
        }
        ZFInput input = ZFInputForPathInfo(zfargs.param0()->to<v_ZFPathInfo *>()->zfv);
        if(packagePwd) {
            ZFInputRead(ZFOutputForEncrypt(ioToken->output(itemPath), packagePwd), input);
        }
        else {
            ZFInputRead(ioToken->output(itemPath), input);
        }
    } ZFLISTENER_END()
    ZFIOForEachFile(sourceDir, impl);
}

ZFMETHOD_DEFINE_2(ZFAppRes, ZFPathInfo, pathInfoForPackage
        , ZFMP_IN(const ZFPathInfo &, packagePathInfo)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        ) {
    if(packagePwd) {
        return ZFPathInfoForEncrypt(ZFPathInfoForCompress(packagePathInfo, "zfres"), packagePwd);
    }
    else {
        return ZFPathInfoForCompress(packagePathInfo, "zfres");
    }
}

// ============================================================
static void _ZFP_ZFAppRes_localStateSave(void);

zfclass _ZFP_I_ZFAppResData : zfextend ZFStyle {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAppResData, ZFStyle)
public:
    ZFPROPERTY_ASSIGN(zfstring, moduleName)
    ZFPROPERTY_ASSIGN(ZFCoreArray<ZFPathInfo>, packageSrcList)
    ZFPROPERTY_ASSIGN(zfstring, packagePwd)
    ZFPROPERTY_ASSIGN(zfstring, packageMd5)
    ZFPROPERTY_ASSIGN(zfindex, fileIndex, zfindexMax()) // downloaded file index of packageSrcList, zfindexMax if not done
    ZFPROPERTY_ASSIGN(zfindex, fileSize, zfindexMax()) // downloaded file size, zfindexMax if not done, also used to verify the file

public:
    zfbool attached(void) {
        return _attached;
    }
    void attach(void) {
        if(!_attached) {
            _attached = zftrue;

            if(this->fileIndex() != zfindexMax()) {
                ZFPathInfo path = _localFilePath(this->fileIndex());
                if(this->fileSize() != zfindexMax() && ZFIOIsExist(path)) {
                    zfautoT<ZFIOToken> ioToken = ZFIOOpen(path, v_ZFIOOpenOption::e_Read);
                    if(ioToken && ioToken->ioSize() == this->fileSize()) {
                        ioToken = zfnull;
                        ZFPathInfo pathInfo = _resExtPathInfo(this->fileIndex());
                        ZFResExtPathAdd(pathInfo);
                        ZFIOModTime(pathInfo, ZFTime::currentTime());
                    }
                    else {
                        this->fileIndex(zfindexMax());
                        this->fileSize(zfindexMax());
                    }
                }
            }
        }
    }
    void detach(void) {
        _cleanup();
        _attached = zffalse;
    }
    zfbool update(
            ZF_IN const ZFCoreArray<ZFPathInfo> &packageSrcList
            , ZF_IN const zfstring &packagePwd
            , ZF_IN const zfstring &packageMd5
            ) {
        if(packagePwd == this->packagePwd()
                && packageMd5 == this->packageMd5()
                && packageSrcList.objectCompareValue(this->packageSrcList()) == ZFCompareEqual
                && (_downloadTaskId || (this->fileIndex() != zfindexMax() && ZFIOIsExist(_localFilePath(this->fileIndex()))))
                ) {
            return zffalse;
        }
        _cleanup();
        this->packageSrcList().copyFrom(packageSrcList);
        this->packagePwd(packagePwd);
        this->packageMd5(packageMd5);

        zfself *owner = this;
        zfobj<ZFTaskQueue> downloadTask;
        _downloadTaskId = downloadTask;
        for(zfindex fileIndex = 0; fileIndex < packageSrcList.count(); ++fileIndex) {
            zfobj<ZFTaskQueue> childQueue;

            zfobj<ZFAsyncIOTask> child(
                    ZFOutputForIOToken(ZFIOOpen(_localCachePath(fileIndex), v_ZFIOOpenOption::e_Modify))
                    , ZFInputForPathInfo(packageSrcList[fileIndex])
                    );
            child->resumable(ZFObjectFromZFSD(ZFInputForPathInfo(_localCacheResumablePath(fileIndex))));
            child->objectTag("_ZFP_ZFAppRes_fileIndex", zfobj<v_zfindex>(fileIndex));
            childQueue->child(child);

            if(packageMd5) {
                ZFLISTENER_2(childVerify
                        , zfweakT<zfself>, owner
                        , zfindex, fileIndex
                        ) {
                    zfstring md5 = ZFMd5(ZFInputForPathInfo(owner->_localCachePath(fileIndex)), zftrue);
                    ZFTask *ownerTask = zfargs.sender();
                    if(md5 && md5 == zfstringToUpper(owner->packageMd5())) {
                        ownerTask->notifySuccess();
                    }
                    else {
                        ZFIORemove(owner->_localCachePath(fileIndex));
                        ZFIORemove(owner->_localCacheResumablePath(fileIndex));
                        ownerTask->notifyFail(zfstr("md5 mismatch: %s => %s", owner->packageMd5(), md5));
                    }
                } ZFLISTENER_END()
                childQueue->child(zfobj<ZFAsyncTask>(childVerify));
            }

            downloadTask->child(childQueue);
        }
        ZFLISTENER_1(childOnStop
                , zfweakT<zfself>, owner
                ) {
            ZFTaskQueue *downloadTask = zfargs.sender();
            ZFTaskQueue *childQueue = zfargs.param0();
            ZFAsyncIOTask *child = childQueue->childAt(0);
            v_zfindex *fileIndex = child->objectTag("_ZFP_ZFAppRes_fileIndex");
            if(childQueue->success()) {
                if(fileIndex && fileIndex->zfv < owner->packageSrcList().count()) {
                    downloadTask->notifySuccess(fileIndex);
                }
            }
            else if(childQueue->failed()) {
                // treat it as success, to try next src
                childQueue->resultType(v_ZFResultType::e_Success);
            }

            // save resumable, no matter success or not
            if(fileIndex && fileIndex->zfv < owner->packageSrcList().count()) {
                ZFObjectToZFSD(ZFOutputForPathInfo(owner->_localCacheResumablePath(fileIndex->zfv)), child->resumable());
            }
        } ZFLISTENER_END()
        downloadTask->observerAdd(ZFTaskQueue::E_ChildOnStop(), childOnStop);

        ZFLISTENER_1(downloadOnFinish
                , zfweakT<zfself>, owner
                ) {
            owner->_downloadTaskId = zfnull;
            ZFTaskQueue *downloadTask = zfargs.sender();
            v_zfindex *fileIndex = downloadTask->result();
            if(downloadTask->canceled() || fileIndex == zfnull || fileIndex->zfv >= owner->packageSrcList().count()) {
                return;
            }
            ZFPathInfo localFilePath = owner->_localFilePath(fileIndex->zfv);
            ZFIORemove(owner->_localCacheResumablePath(fileIndex->zfv));
            if(ZFIOMove(localFilePath.pathData(), owner->_localCachePath(fileIndex->zfv))) {
                zfautoT<ZFIOToken> token = ZFIOOpen(localFilePath, v_ZFIOOpenOption::e_Read);
                if(token) {
                    zfindex fileSize = token->ioSize();
                    if(fileSize != zfindexMax()) {
                        owner->fileIndex(fileIndex->zfv);
                        owner->fileSize(fileSize);
                        _ZFP_ZFAppRes_localStateSave();
                    }
                }
            }
        } ZFLISTENER_END()
        downloadTask->start(downloadOnFinish);
        return zftrue;
    }

private:
    zfautoT<ZFTaskId> _downloadTaskId;
    zfbool _attached;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _attached = zffalse;
    }
private:
    // downloaded file
    ZFPathInfo _localFilePath(ZF_IN zfindex fileIndex) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s", ZFMd5(ZFPathInfoToString(this->packageSrcList().get(fileIndex)))));
    }
    // tmp file while downloading, moved to _localFilePath if valid, during next app launch
    ZFPathInfo _localCachePath(ZF_IN zfindex fileIndex) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s.tmp", ZFMd5(ZFPathInfoToString(this->packageSrcList().get(fileIndex)))));
    }
    // store resumable for async io
    ZFPathInfo _localCacheResumablePath(ZF_IN zfindex fileIndex) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s.state", ZFMd5(ZFPathInfoToString(this->packageSrcList().get(fileIndex)))));
    }
    ZFPathInfo _resExtPathInfo(ZF_IN zfindex fileIndex) {
        return ZFAppRes::pathInfoForPackage(_localFilePath(fileIndex), this->packagePwd());
    }
    void _cleanup(void) {
        if(_downloadTaskId) {
            _downloadTaskId->stop();
            _downloadTaskId = zfnull;
        }
        if(!this->packageSrcList().isEmpty()) {
            if(this->fileIndex() != zfindexMax()) {
                ZFResExtPathRemove(_resExtPathInfo(this->fileIndex()));
            }
            for(zfindex i = 0; i <  this->packageSrcList().count(); ++i) {
                ZFIORemove(_localFilePath(i));
                ZFIORemove(_localCachePath(i));
                ZFIORemove(_localCacheResumablePath(i));
            }
        }
        this->fileIndex(zfindexMax());
        this->fileSize(zfindexMax());
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAppResData)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAppResDataHolder, ZFLevelZFFrameworkNormal) {
    _cleanupHasRun = zffalse;
    _modulesChanged = zffalse;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFAppResDataHolder) {
    if(_modules) {
        for(zfindex i = 0; i < _modules->count(); ) {
            _ZFP_I_ZFAppResData *data = _modules->get(i);
            if(!data->attached()) {
                data->detach();
                _modules->remove(i);
                _modulesChanged = zftrue;
            }
            else {
                ++i;
            }
        }
        _localStateSave(zftrue);
    }
}
private:
    zfbool _cleanupHasRun;
    zfbool _modulesChanged;
    zfautoT<ZFArray> _modules; // [_ZFP_I_ZFAppResData], null when not loaded
    zfautoT<ZFTaskId> _modulesSaveDelayId;
public:
    void attach(ZF_IN const zfstring &moduleName) {
        if(!_cleanupHasRun) {
            _cleanupHasRun = zftrue;
            ZFIORemoveOutdate(ZFPathInfo(ZFPathType_storagePath(), "ZFAppRes"));
        }
        _localStateLoad();
        _ZFP_I_ZFAppResData *data = zfnull;
        for(zfindex i = 0; i < _modules->count(); ++i) {
            _ZFP_I_ZFAppResData *t = _modules->get(i);
            if(t->moduleName() == moduleName) {
                data = t;
                break;
            }
        }
        if(data != zfnull) {
            if(data->attached()) {
                ZFCoreCriticalMessageTrim("ZFAppRes:%s module already started", moduleName);
                return;
            }
            else {
                data->attach();
            }
        }
    }
    void notifyFinish(
            ZF_IN const zfstring &moduleName
            , ZF_IN const ZFCoreArray<ZFPathInfo> &packageSrcList
            , ZF_IN const zfstring &packagePwd
            , ZF_IN const zfstring &packageMd5
            ) {
        _localStateLoad();
        _ZFP_I_ZFAppResData *data = zfnull;
        for(zfindex i = 0; i < _modules->count(); ++i) {
            _ZFP_I_ZFAppResData *t = _modules->get(i);
            if(t->moduleName() == moduleName) {
                data = t;
                break;
            }
        }
        if(data != zfnull) {
            _modulesChanged = _modulesChanged
                || data->update(packageSrcList, packagePwd, packageMd5);
            _localStateSave();
            return;
        }
        zfobj<_ZFP_I_ZFAppResData> dataHolder;
        data = dataHolder;
        _modules->add(data);
        data->attach();
        data->moduleName(moduleName);
        data->update(packageSrcList, packagePwd, packageMd5);
        _modulesChanged = zftrue;
        _localStateSave();
    }
    void localStateSave(void) {
        _modulesChanged = zftrue;
        _localStateSave();
    }
private:
    ZFPathInfo _localStatePath(void) {
        return ZFPathInfoForEncrypt(ZFPathInfo(ZFPathType_storagePath(), "ZFAppRes/state"), "ZFAppRes");
    }
    void _localStateLoad(void) {
        if(_modules != zfnull) {
            return;
        }
        zfstring data;
        ZFInputRead(data, ZFInputForPathInfo(_localStatePath()));
        _modules = ZFObjectFromStringOrData(ZFArray::ClassData(), data, data.length());
        if(!_modules) {
            _modules = zfobj<ZFArray>();
        }
    }
    void _localStateSave(ZF_IN_OPT zfbool immediately = zffalse) {
        if(_modules == zfnull || !_modulesChanged
                || (_modulesSaveDelayId && !immediately)
                ) {
            return;
        }
        if(_modulesSaveDelayId) {
            _modulesSaveDelayId->stop();
        }
        zfself *owner = this;
        ZFLISTENER_1(onSave
                , zfself *, owner
                ) {
            owner->_modulesSaveDelayId = zfnull;
            owner->_modulesChanged = zffalse;
            zfstring data = ZFObjectToStringOrData(owner->_modules);
            if(data) {
                ZFOutputForPathInfo(owner->_localStatePath()).output(data, data.length());
            }
        } ZFLISTENER_END()
        _modulesSaveDelayId = ZFTimerOnce(2000, onSave);
    }
ZF_GLOBAL_INITIALIZER_END(ZFAppResDataHolder)
static void _ZFP_ZFAppRes_localStateSave(void) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->localStateSave();
}

ZFMETHOD_DEFINE_2(ZFAppRes, void, start
        , ZFMP_IN(const ZFListener &, packageGetter)
        , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
        ) {
    zfobj<ZFAppRes> owner;
    zfobj<v_zfstring> moduleNameTmp(moduleName ? moduleName : zftext("default"));
    owner->objectTag("_ZFP_ZFAppRes_moduleName", moduleNameTmp);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->attach(moduleNameTmp->zfv);
    packageGetter.execute(ZFArgs().sender(owner));
}

ZFMETHOD_DEFINE_3(ZFAppRes, void, notifyFinish
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageSrcList)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        , ZFMP_IN_OPT(const zfstring &, packageMd5, zfnull)
        ) {
    if(!packageSrcList.isEmpty()) {
        v_zfstring *moduleName = this->objectTag("_ZFP_ZFAppRes_moduleName");
        if(moduleName) {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->notifyFinish(
                    moduleName->zfv
                    , packageSrcList
                    , packagePwd
                    , packageMd5
                    );
        }
    }
}

// ============================================================
ZFMETHOD_DEFINE_3(ZFAppRes, void, start
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageInfoList)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
        ) {
    if(packageInfoList.isEmpty()) {
        return;
    }
    ZFLISTENER_2(packageGetter
            , ZFCoreArray<ZFPathInfo>, packageInfoList
            , zfstring, packagePwd
            ) {
        zfautoT<ZFAppRes> o = zfargs.sender();

        zfobj<ZFTaskGroup> task;
        ZFCoreArray<ZFPathInfo> packageSrcList;
        zfobj<v_zfstring> packageMd5;
        for(zfindex i = 0; i < packageInfoList.count(); ++i) {
            const ZFPathInfo &pathInfo = packageInfoList[i];
            ZFLISTENER_3(childTask
                    , ZFCoreArray<ZFPathInfo>, packageSrcList
                    , zfautoT<v_zfstring>, packageMd5
                    , ZFPathInfo, pathInfo
                    ) {
                ZFJson info = ZFJsonFromInput(ZFInputForPathInfo(pathInfo));
                zfstring src = info.attrValue("src");
                if(!src) {
                    return;
                }
                ZFPathInfo srcPathInfo;
                if(!ZFPathInfoFromStringT(srcPathInfo, src)) {
                    return;
                }
                zfstring md5 = info.attrValue("md5");
                {
                    ZFObjectLocker(packageMd5);
                    if(packageMd5->zfv && md5 && packageMd5->zfv != md5) {
                        return;
                    }
                    if(md5) {
                        packageMd5->zfv = md5;
                    }
                    packageSrcList.add(srcPathInfo);
                }
            } ZFLISTENER_END()
            task->child(zfobj<ZFAsyncTask>()->c_impl(childTask));
        }
        ZFLISTENER_4(taskOnStop
                , zfautoT<ZFAppRes>, o
                , zfstring, packagePwd
                , ZFCoreArray<ZFPathInfo>, packageSrcList
                , zfautoT<v_zfstring>, packageMd5
                ) {
            o->notifyFinish(packageSrcList, packagePwd, packageMd5->zfv);
        } ZFLISTENER_END()
        task->start(taskOnStop);
    } ZFLISTENER_END()
    zfself::start(packageGetter, moduleName);
}

ZF_NAMESPACE_GLOBAL_END

