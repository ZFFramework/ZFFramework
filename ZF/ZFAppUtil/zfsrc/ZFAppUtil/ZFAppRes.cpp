#include "ZFAppRes.h"

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
        return ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(
                    ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(packagePathInfo, "zfres"))
                    , packagePwd
                    ));
    }
    else {
        return ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(packagePathInfo, "zfres"));
    }
}

// ============================================================
static void _ZFP_ZFAppRes_localStateSave(void);

zfclass _ZFP_I_ZFAppResData : zfextend ZFStyle {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAppResData, ZFStyle)
public:
    ZFPROPERTY_ASSIGN(zfstring, moduleName)
    ZFPROPERTY_ASSIGN(ZFCoreArray<ZFPathInfo>, packageSrc)
    ZFPROPERTY_ASSIGN(zfstring, packagePwd)
    ZFPROPERTY_ASSIGN(zfindex, fileIndex, zfindexMax()) // downloaded file index of packageSrc, zfindexMax if not done
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
                        ZFResExtPathAdd(_resExtPathInfo(this->fileIndex()));
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
            ZF_IN const ZFCoreArray<ZFPathInfo> &packageSrc
            , ZF_IN const zfstring &packagePwd
            ) {
        if(packagePwd == this->packagePwd()
                && packageSrc.objectCompareValue(this->packageSrc()) == ZFCompareEqual
                && (_downloadTaskId || (this->fileIndex() != zfindexMax() && ZFIOIsExist(_localFilePath(this->fileIndex()))))
                ) {
            return zffalse;
        }
        _cleanup();
        this->packageSrc().copyFrom(packageSrc);
        this->packagePwd(packagePwd);

        zfself *owner = this;
        zfobj<ZFTaskQueue> downloadTask;
        _downloadTaskId = downloadTask;
        for(zfindex i = 0; i < packageSrc.count(); ++i) {
            zfobj<ZFAsyncIOTask> child(
                    ZFOutputForPathInfo(_localCachePath(i))
                    , ZFInputForPathInfo(packageSrc[i])
                    );
            child->objectTag("_ZFP_ZFAppRes_fileIndex", zfobj<v_zfindex>(i));
            downloadTask->child(child);
        }
        ZFLISTENER_1(childOnStop
                , zfweakT<zfself>, owner
                ) {
            ZFTaskQueue *downloadTask = zfargs.sender();
            ZFTask *child = zfargs.param0();
            if(child->success()) {
                v_zfindex *fileIndex = child->objectTag("_ZFP_ZFAppRes_fileIndex");
                if(fileIndex && fileIndex->zfv < owner->packageSrc().count()) {
                    downloadTask->notifySuccess(fileIndex);
                }
            }
            else if(child->failed()) {
                // treat it as success, to try next src
                child->resultType(v_ZFResultType::e_Success);
            }
        } ZFLISTENER_END()
        downloadTask->observerAdd(ZFTaskQueue::E_ChildOnStop(), childOnStop);

        ZFLISTENER_1(downloadOnFinish
                , zfweakT<zfself>, owner
                ) {
            owner->_downloadTaskId = zfnull;
            ZFTaskQueue *downloadTask = zfargs.sender();
            v_zfindex *fileIndex = downloadTask->result();
            if(downloadTask->canceled() || fileIndex == zfnull || fileIndex->zfv >= owner->packageSrc().count()) {
                return;
            }
            ZFPathInfo localFilePath = owner->_localFilePath(fileIndex->zfv);
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
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s", ZFMd5(ZFPathInfoToString(this->packageSrc().get(fileIndex)))));
    }
    // tmp file while downloading, moved to _localFilePath if valid, during next app launch
    ZFPathInfo _localCachePath(ZF_IN zfindex fileIndex) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s.tmp", ZFMd5(ZFPathInfoToString(this->packageSrc().get(fileIndex)))));
    }
    ZFPathInfo _resExtPathInfo(ZF_IN zfindex fileIndex) {
        return ZFAppRes::pathInfoForPackage(_localFilePath(fileIndex), this->packagePwd());
    }
    void _cleanup(void) {
        if(_downloadTaskId) {
            _downloadTaskId->stop();
            _downloadTaskId = zfnull;
        }
        if(!this->packageSrc().isEmpty()) {
            if(this->fileIndex() != zfindexMax()) {
                ZFResExtPathRemove(_resExtPathInfo(this->fileIndex()));
            }
            for(zfindex i = 0; i <  this->packageSrc().count(); ++i) {
                ZFIORemove(_localFilePath(i));
                ZFIORemove(_localCachePath(i));
            }
        }
        this->fileIndex(zfindexMax());
        this->fileSize(zfindexMax());
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAppResData)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAppResDataHolder, ZFLevelZFFrameworkNormal) {
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
    zfautoT<ZFArray> _modules; // [_ZFP_I_ZFAppResData], null when not loaded
    zfbool _modulesChanged;
    zfautoT<ZFTaskId> _modulesSaveDelayId;
public:
    void attach(ZF_IN const zfstring &moduleName) {
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
            , ZF_IN const ZFCoreArray<ZFPathInfo> &packageSrc
            , ZF_IN const zfstring &packagePwd
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
                || data->update(packageSrc, packagePwd);
            _localStateSave();
            return;
        }
        zfobj<_ZFP_I_ZFAppResData> dataHolder;
        data = dataHolder;
        _modules->add(data);
        data->attach();
        data->moduleName(moduleName);
        data->update(packageSrc, packagePwd);
        _modulesChanged = zftrue;
        _localStateSave();
    }
    void localStateSave(void) {
        _modulesChanged = zftrue;
        _localStateSave();
    }
private:
    ZFPathInfo _localStatePath(void) {
        return ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(ZFPathInfo(ZFPathType_storagePath(), "ZFAppRes/state"), "ZFAppRes"));
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
    owner->_ZFP_ZFAppRes_moduleName = moduleName ? moduleName : zftext("default");
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->attach(owner->_ZFP_ZFAppRes_moduleName);
    packageGetter.execute(ZFArgs().sender(owner));
}

ZFMETHOD_DEFINE_2(ZFAppRes, void, notifyFinish
        , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageSrc)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        ) {
    if(!packageSrc.isEmpty()) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->notifyFinish(
                _ZFP_ZFAppRes_moduleName
                , packageSrc
                , packagePwd
                );
    }
}

ZF_NAMESPACE_GLOBAL_END

