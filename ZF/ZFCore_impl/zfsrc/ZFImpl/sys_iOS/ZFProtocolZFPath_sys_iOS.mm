#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFPath.h"

#if ZF_ENV_sys_iOS
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFPathImpl_sys_iOS, ZFPath, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:SandboxPath")
public:
    virtual const zfstring &pathForModule(void) {
        if(this->_pathForModule.isEmpty()) {
            (void)this->pathForModuleFile();
            zfindex pos = zfstringFindReversely(this->_pathForModuleFile, '/');
            if(pos != zfindexMax()) {
                this->_pathForModule.assign(this->_pathForModuleFile, pos);
            }
        }
        return this->_pathForModule;
    }
    virtual const zfstring &pathForModuleFile(void) {
        if(this->_pathForModuleFile.isEmpty()) {
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, [[NSBundle mainBundle] bundlePath]);
            ZFPathFormatT(this->_pathForModuleFile, tmp);
        }
        return this->_pathForModuleFile;
    }

    virtual const zfstring &pathForSetting(void) {
        if(this->_pathForSetting.isEmpty()) {
            this->_pathForSetting = this->nativeDocumentPath();
            this->_pathForSetting += "/zfsetting";
        }
        return this->_pathForSetting;
    }
    virtual void pathForSetting(ZF_IN const zfstring &path) {
        this->_pathForSetting = path;
    }

    virtual const zfstring &pathForStorage(void) {
        if(this->_pathForStorage.isEmpty()) {
            this->_pathForStorage = this->nativeDocumentPath();
            this->_pathForStorage += "/zfstorage";
        }
        return this->_pathForStorage;
    }
    virtual void pathForStorage(ZF_IN const zfstring &path) {
        this->_pathForStorage = path;
    }

    virtual const zfstring &pathForStorageShared(void) {
        if(this->_pathForStorageShared.isEmpty()) {
            this->_pathForStorageShared = this->nativeDocumentPath();
        }
        return this->_pathForStorageShared;
    }
    virtual void pathForStorageShared(ZF_IN const zfstring &path) {
        this->_pathForStorageShared = path;
    }

    virtual const zfstring &pathForCache(void) {
        if(this->_pathForCache.isEmpty()) {
            this->_pathForCache = this->nativeCachePath();
            this->_pathForCache += "/zfcache";
        }
        return this->_pathForCache;
    }
    virtual void pathForCache(ZF_IN const zfstring &path) {
        this->_pathForCache = path;
    }
    virtual void pathForCacheClear(void) {
        ZFFileRemove(this->_pathForCache, zfHint("isRecursive")zftrue, zfHint("isForce")zftrue);
    }

private:
    zfstring _nativeDocumentPath;
    const zfstring &nativeDocumentPath(void) {
        if(_nativeDocumentPath.isEmpty()) {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFPathFormatT(_nativeDocumentPath, tmp);
        }
        return _nativeDocumentPath;
    }
    zfstring _nativeCachePath;
    const zfstring &nativeCachePath(void) {
        if(_nativeCachePath.isEmpty()) {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFPathFormatT(_nativeCachePath, tmp);
        }
        return _nativeCachePath;
    }

private:
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFPathImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

