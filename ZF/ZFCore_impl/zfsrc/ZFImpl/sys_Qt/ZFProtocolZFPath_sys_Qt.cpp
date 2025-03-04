#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFPath.h"

#if ZF_ENV_sys_Qt
#include <QCoreApplication>
#include <QStandardPaths>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFPathImpl_sys_Qt, ZFPath, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt::applicationDirPath")
public:
    virtual const zfstring &pathForModule(void) {
        if(this->_pathForModule.isEmpty()) {
            this->_pathForModule = QCoreApplication::applicationDirPath().toStdString().c_str();
            if(this->_pathForModule.isEmpty()) {
                this->_pathForModule = "./";
            }
        }
        return this->_pathForModule;
    }
    virtual const zfstring &pathForModuleFile(void) {
        if(this->_pathForModuleFile.isEmpty()) {
            this->_pathForModuleFile = QCoreApplication::applicationFilePath().toStdString().c_str();
            if(this->_pathForModuleFile.isEmpty()) {
                this->_pathForModuleFile = "./unknown";
            }
        }
        return this->_pathForModuleFile;
    }

    virtual const zfstring &pathForSetting(void) {
        if(this->_pathForSetting.isEmpty()) {
            ZFPathFormatT(this->_pathForSetting, QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString().c_str());
            if(this->_pathForSetting.isEmpty()) {
                this->_pathForSetting = this->pathForModule();
            }
            this->_pathForSetting += "/zfsetting";
        }
        return this->_pathForSetting;
    }
    virtual void pathForSetting(ZF_IN const zfstring &path) {
        this->_pathForSetting = path;
    }

    virtual const zfstring &pathForStorage(void) {
        if(this->_pathForStorage.isEmpty()) {
            ZFPathFormatT(this->_pathForStorage, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString().c_str());
            if(this->_pathForStorage.isEmpty()) {
                this->_pathForStorage = this->pathForModule();
            }
            this->_pathForStorage += "/zfstorage";
        }
        return this->_pathForStorage;
    }
    virtual void pathForStorage(ZF_IN const zfstring &path) {
        this->_pathForStorage = path;
    }

    virtual const zfstring &pathForStorageShared(void) {
        if(this->_pathForStorageShared.isEmpty()) {
            ZFPathFormatT(this->_pathForStorageShared, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString().c_str());
            if(this->_pathForStorageShared.isEmpty()) {
                this->_pathForStorageShared = this->pathForModule();
                this->_pathForStorageShared += "/zfstorage";
            }
        }
        return this->_pathForStorageShared;
    }
    virtual void pathForStorageShared(ZF_IN const zfstring &path) {
        this->_pathForStorageShared = path;
    }

    virtual const zfstring &pathForCache(void) {
        if(this->_pathForCache.isEmpty()) {
            ZFPathFormatT(this->_pathForCache, QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdString().c_str());
            if(this->_pathForCache.isEmpty()) {
                this->_pathForCache = this->pathForModule();
            }
            this->_pathForCache += "/zfcache";
        }
        return this->_pathForCache;
    }
    virtual void pathForCache(ZF_IN const zfstring &path) {
        this->_pathForCache = path;
    }

private:
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFPathImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

