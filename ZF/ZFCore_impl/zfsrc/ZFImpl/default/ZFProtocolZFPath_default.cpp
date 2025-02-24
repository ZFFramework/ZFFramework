#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFPath.h"
#include "ZFCore/zfstringW.h"

#include "ZFImpl/ZFImpl_env.h"

#ifndef _ZFP_ZFPathImpl_default_whereami
    #define _ZFP_ZFPathImpl_default_whereami 1
#endif

#if _ZFP_ZFPathImpl_default_whereami
    #define WHEREAMI_H
    #ifndef WAI_FUNCSPEC
        #define WAI_FUNCSPEC
    #endif
    #ifndef WAI_PREFIX
        #define WAI_PREFIX(function) wai_##function
    #endif
    #include "../../../zf3rd/_repo/whereami/src/whereami.c"
#else
    #if ZF_ENV_sys_Windows
        #include <Windows.h>
    #else // #if ZF_ENV_sys_Windows
        #include <unistd.h>
    #endif // #if ZF_ENV_sys_Windows #else
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFPathImpl_default, ZFPath, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ModuleLocalPath")
public:
    virtual const zfstring &pathForModule(void) {
        if(this->_pathForModule.isEmpty()) {
            (void)this->pathForModuleFile();
            zfindex pos = zfstringFindReversely(this->_pathForModuleFile, '/');
            ZFCoreAssert(pos != zfindexMax());
            this->_pathForModule.assign(this->_pathForModuleFile, pos);
        }
        return this->_pathForModule;
    }
    virtual const zfstring &pathForModuleFile(void) {
        if(this->_pathForModuleFile.isEmpty()) {
            #if _ZFP_ZFPathImpl_default_whereami
                zfstring tmp;
                {
                    int len = wai_getExecutablePath(NULL, 0, NULL);
                    if(len > 0) {
                        char *buf = (char *)zfmalloc(len + 1);
                        wai_getExecutablePath(buf, len, NULL);
                        buf[len] = '\0';
                        tmp += buf;
                    }
                }
            #else
                #if ZF_ENV_sys_Windows
                    zfstring tmp;
                    zfcharW buf[1024] = {0};
                    GetModuleFileNameW(zfnull, buf, 1024);
                    zfstringToUTF8(tmp, buf, ZFStringEncoding::e_UTF16);
                #else // #if ZF_ENV_sys_Windows
                    zfchar tmp[1024] = {0};
                    zfint len = (zfint)readlink("/proc/self/exe", tmp, 1024);
                    if(len > 0) {
                        tmp[len] = '\0';
                    }
                    else {
                        zfscpy(tmp, "./unknown");
                    }
                #endif // #if ZF_ENV_sys_Windows #else
            #endif
            zfbool result = ZFPathFormatT(this->_pathForModuleFile, tmp);
            ZFCoreAssert(result);
        }
        return this->_pathForModuleFile;
    }

    virtual const zfstring &pathForSetting(void) {
        if(this->_pathForSetting.isEmpty()) {
            this->_pathForSetting = this->pathForModule();
            this->_pathForSetting += "/zfsetting";
        }
        return this->_pathForSetting;
    }
    virtual void pathForSetting(ZF_IN const zfstring &path) {
        this->_pathForSetting = path;
    }

    virtual const zfstring &pathForStorage(void) {
        if(this->_pathForStorage.isEmpty()) {
            this->_pathForStorage = this->pathForModule();
            this->_pathForStorage += "/zfstorage";
        }
        return this->_pathForStorage;
    }
    virtual void pathForStorage(ZF_IN const zfstring &path) {
        this->_pathForStorage = path;
    }

    virtual const zfstring &pathForStorageShared(void) {
        if(this->_pathForStorageShared.isEmpty()) {
            this->_pathForStorageShared = this->pathForModule();
            this->_pathForStorageShared += "/zfstorageshared";
        }
        return this->_pathForStorageShared;
    }
    virtual void pathForStorageShared(ZF_IN const zfstring &path) {
        this->_pathForStorageShared = path;
    }

    virtual const zfstring &pathForCache(void) {
        if(this->_pathForCache.isEmpty()) {
            this->_pathForCache = this->pathForModule();
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
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFPathImpl_default)

ZF_NAMESPACE_GLOBAL_END

