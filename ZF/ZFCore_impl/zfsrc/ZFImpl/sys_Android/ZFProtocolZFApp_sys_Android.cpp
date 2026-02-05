#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/protocol/ZFProtocolZFApp.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFApp ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFApp)
#define ZFImpl_sys_Android_JNI_NAME_ZFApp ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFApp)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFApp, ZFImpl_sys_Android_JNI_NAME_ZFApp)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAppImpl_sys_Android, ZFApp, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual void appRestart(void) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFApp(), "appRestart",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFApp(), jmId);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAppImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

