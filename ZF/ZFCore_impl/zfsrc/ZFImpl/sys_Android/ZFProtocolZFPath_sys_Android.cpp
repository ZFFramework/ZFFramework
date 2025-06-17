#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFPath.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFPath ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFPath)
#define ZFImpl_sys_Android_JNI_NAME_ZFPath ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFPath)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFPath, ZFImpl_sys_Android_JNI_NAME_ZFPath)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFPathImpl_sys_Android, ZFPath, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:FileSystem")
public:
    virtual const zfstring &pathForModule(void) {
        if(this->_pathForModule.isEmpty()) {
            (void)this->pathForModuleFile();
            zfindex pos = zfstringFindReversely(this->_pathForModuleFile, "/");
            ZFCoreAssert(pos != zfindexMax());
            this->_pathForModule.assign(this->_pathForModuleFile, pos);
        }
        return this->_pathForModule;
    }
    virtual const zfstring &pathForModuleFile(void) {
        if(this->_pathForModuleFile.isEmpty()) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFPath(), "native_pathForModuleFile",
                JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFPath(), jmId);
            ZFImpl_sys_Android_zfstringFromStringT(this->_pathForModuleFile, path);
            ZFCoreAssert(!this->_pathForModuleFile.isEmpty());
        }
        return this->_pathForModuleFile;
    }

    virtual const zfstring &pathForSetting(void) {
        if(this->_pathForSetting.isEmpty()) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFPath(), "native_pathForSetting",
                JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFPath(), jmId);
            ZFImpl_sys_Android_zfstringFromStringT(this->_pathForSetting, path);
            ZFCoreAssert(!this->_pathForSetting.isEmpty());
        }
        return this->_pathForSetting;
    }
    virtual void pathForSetting(ZF_IN const zfstring &path) {
        this->_pathForSetting = path;
    }

    virtual const zfstring &pathForStorage(void) {
        if(this->_pathForStorage.isEmpty()) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFPath(), "native_pathForStorage",
                JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFPath(), jmId);
            ZFImpl_sys_Android_zfstringFromStringT(this->_pathForStorage, path);
            ZFCoreAssert(!this->_pathForStorage.isEmpty());
        }
        return this->_pathForStorage;
    }
    virtual void pathForStorage(ZF_IN const zfstring &path) {
        this->_pathForStorage = path;
    }

    virtual const zfstring &pathForStorageShared(void) {
        if(this->_pathForStorageShared.isEmpty()) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFPath(), "native_pathForStorageShared",
                JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFPath(), jmId);
            ZFImpl_sys_Android_zfstringFromStringT(this->_pathForStorageShared, path);
            ZFCoreAssert(!this->_pathForStorageShared.isEmpty());
        }
        return this->_pathForStorageShared;
    }
    virtual void pathForStorageShared(ZF_IN const zfstring &path) {
        this->_pathForStorageShared = path;
    }

    virtual const zfstring &pathForCache(void) {
        if(this->_pathForCache.isEmpty()) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFPath(), "native_pathForCache",
                JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFPath(), jmId);
            ZFImpl_sys_Android_zfstringFromStringT(this->_pathForCache, path);
            ZFCoreAssert(!this->_pathForCache.isEmpty());
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFPathImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

