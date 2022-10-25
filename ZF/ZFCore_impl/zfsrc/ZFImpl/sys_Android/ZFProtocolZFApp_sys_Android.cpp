#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFApp ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFApp)
#define ZFImpl_sys_Android_JNI_NAME_ZFApp ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFApp)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_sys_Android, ZFApp, ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFApp).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void appRestart(ZF_IN zftimet delay)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "appRestart",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_long)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, (jlong)delay);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAppImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFApp,
                         void, native_1appRestart)
{
    ZFCoreArray<zfstring> appParamsSaved;
    appParamsSaved.copyFrom(ZFApp::appParams());
    ZFFrameworkCleanup();
    ZFFrameworkInit();
    ZFMainExecute(appParamsSaved);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

