#include "ZFAppRes.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_DEFINE_3(ZFAppRes, void, build
        , ZFMP_OUT(const ZFPathInfo &, resultPackage)
        , ZFMP_IN(const ZFPathInfo &, sourceDir)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        ) {
    zfautoT<ZFCompressToken> ioToken = ZFCompressOpen(resultPackage, v_ZFIOOpenOption::e_Write);
    ZFLISTENER_2(impl
            , zfautoT<ZFCompressToken>, ioToken
            , zfstring, packagePwd
            ) {
        zfstring itemPath = zfstr("zfres/%s", zfargs.param1().to<v_zfstring *>()->zfv);
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

// ============================================================
zfclass _ZFP_I_ZFAppResData : zfextend ZFStyle {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAppResData, ZFStyle)
public:
    ZFPROPERTY_ASSIGN(zfstring, moduleName)
    ZFPROPERTY_ASSIGN(ZFPathInfo, packageSrc)
    ZFPROPERTY_ASSIGN(zfstring, packagePwd)
    ZFPROPERTY_ASSIGN(zfindex, fileSize, zfindexMax()) // downloaded file size, zfindexMax if not done, also used to verify the file

public:
    zfbool attached(void) {
        return _attached;
    }
    void attach(void) {
        if(!_attached) {
            _attached = zftrue;

            ZFPathInfo path = _localFilePath();
            if(this->fileSize() != zfindexMax() && ZFIOIsExist(path)) {
                zfautoT<ZFIOToken> ioToken = ZFIOOpen(path, v_ZFIOOpenOption::e_Read);
                if(ioToken && ioToken->ioSize() == this->fileSize()) {
                    ioToken = zfnull;
                    ZFResExtPathAdd(_resExtPathInfo());
                }
                else {
                    this->fileSize(zfindexMax());
                }
            }
        }
    }
    void detach(void) {
        if(this->packageSrc()) {
            ZFResExtPathRemove(_resExtPathInfo());
            ZFIORemove(_localFilePath());
            ZFIORemove(_localCachePath());
        }
        _attached = zffalse;
    }
    zfbool update(
            ZF_IN const ZFPathInfo &packageSrc
            , ZF_IN const zfstring &packagePwd
            ) {
        if(packageSrc == this->packageSrc() && packagePwd == this->packagePwd()
                && (_downloadTaskId || (this->fileSize() != zfindexMax() && ZFIOIsExist(_localFilePath())))
                ) {
            return zffalse;
        }
        if(_downloadTaskId) {
            _downloadTaskId->stop();
            _downloadTaskId = zfnull;
        }
        ZFIORemove(_localFilePath());
        ZFIORemove(_localCachePath());
        this->packageSrc(packageSrc);
        this->packagePwd(packagePwd);
        this->fileSize(zfindexMax());

        zfself *owner = this;
        ZFLISTENER_1(downloadFinish
                , zfweakT<zfself>, owner
                ) {
            owner->_downloadTaskId = zfnull;
            ZFTask *task = zfargs.sender();
            if(task->success()) {
                v_zfindex *result = task->result();
                if(result != zfnull && result->zfv != zfindexMax()
                        && owner
                        ) {
                    if(ZFIOMove(owner->_localCachePath(), owner->_localFilePath().pathData())) {
                        owner->fileSize(result->zfv);
                    }
                }
            }
        } ZFLISTENER_END()
        _downloadTaskId = zfobj<ZFAsyncIOTask>(
                ZFOutputForPathInfo(_localCachePath())
                , ZFInputForPathInfo(packageSrc)
                )->c_start(downloadFinish);

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
    // valid file to use
    ZFPathInfo _localFilePath(void) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s", ZFMd5(ZFPathInfoToString(this->packageSrc()))));
    }
    // tmp file while downloading, moved to _localFilePath if valid, during next app launch
    ZFPathInfo _localCachePath(void) {
        return ZFPathInfo(ZFPathType_storagePath(), zfstr("ZFAppRes/%s.tmp", ZFMd5(ZFPathInfoToString(this->packageSrc()))));
    }
    ZFPathInfo _resExtPathInfo(void) {
        if(this->packagePwd()) {
            return ZFPathInfo(ZFPathType_encrypt(), ZFPathInfoChainEncode(
                        ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(_localFilePath(), "zfres"))
                        , this->packagePwd()
                        ));
        }
        else {
            return ZFPathInfo(ZFPathType_compress(), ZFPathInfoChainEncode(_localFilePath(), "zfres"));
        }
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
        _localStateSave();
    }
}
private:
    zfautoT<ZFArray> _modules; // [_ZFP_I_ZFAppResData], null when not loaded
    zfbool _modulesChanged;
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
            , ZF_IN const ZFPathInfo &packageSrc
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
    void _localStateSave(void) {
        if(_modules == zfnull || !_modulesChanged) {
            return;
        }
        zfstring data = ZFObjectToStringOrData(_modules);
        if(data) {
            ZFOutputForPathInfo(_localStatePath()).output(data, data.length());
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFAppResDataHolder)

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
        , ZFMP_IN(const ZFPathInfo &, packageSrc)
        , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
        ) {
    if(packageSrc) {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAppResDataHolder)->notifyFinish(
                _ZFP_ZFAppRes_moduleName
                , packageSrc
                , packagePwd
                );
    }
}

ZF_NAMESPACE_GLOBAL_END

