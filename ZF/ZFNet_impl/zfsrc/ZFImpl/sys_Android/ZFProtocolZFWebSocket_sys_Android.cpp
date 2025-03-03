#include "ZFImpl_sys_Android_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFWebSocket.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFWebSocket ZFImpl_sys_Android_JNI_ID(ZFNet_1impl_ZFWebSocket)
#define ZFImpl_sys_Android_JNI_NAME_ZFWebSocket ZFImpl_sys_Android_JNI_NAME(ZFNet_impl.ZFWebSocket)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFWebSocket, ZFImpl_sys_Android_JNI_NAME_ZFWebSocket)

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFWebSocketImpl_sys_Android, ZFWebSocket, v_ZFProtocolLevel::e_SystemNormal)
public:
    virtual void *open(
            ZF_IN ZFWebSocket *owner
            , ZF_IN const zfstring &url
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), "native_open",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
                .add(JNIType::S_object_String())
            ).c_str());
        jobject nativeWebSocket = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), jmId
                , JNIConvertZFObjectToJNIType(jniEnv, owner)
                , ZFImpl_sys_Android_zfstringToString(url)
            );
        JNIBlockedDeleteLocalRef(nativeWebSocket);
        return JNIUtilNewGlobalRef(jniEnv, nativeWebSocket);
    }
    virtual void close(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), "native_close",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeWebSocketTmp = (jobject)nativeWebSocket;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), jmId
                , nativeWebSocketTmp
            );
        JNIUtilDeleteGlobalRef(jniEnv, nativeWebSocketTmp);
    }
    virtual void send(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), "native_send",
                JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                    .add(JNIType::S_object_Object())
                    .add(JNIType::S_object_Object())
                    ).c_str());
        jbyteArray nativeBuf = JNIUtilNewByteArray(jniEnv, size);
        JNIBlockedDeleteLocalRef(nativeBuf);
        JNIUtilSetByteArrayRegion(jniEnv, nativeBuf, 0, (jint)size, (const jbyte *)data);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), jmId
                , (jobject)nativeWebSocket
                , nativeBuf
                );
    }
    virtual void sendBin(
            ZF_IN ZFWebSocket *owner
            , ZF_IN void *nativeWebSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), "native_sendBin",
                JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                    .add(JNIType::S_object_Object())
                    .add(JNIType::S_object_Object())
                    ).c_str());
        jbyteArray nativeBuf = JNIUtilNewByteArray(jniEnv, size);
        JNIBlockedDeleteLocalRef(nativeBuf);
        JNIUtilSetByteArrayRegion(jniEnv, nativeBuf, 0, (jint)size, (const jbyte *)data);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFWebSocket(), jmId
                , (jobject)nativeWebSocket
                , nativeBuf
                );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFWebSocketImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFWebSocket
        , void, native_1notifyOnOpen
        , JNIPointer zfjniPointerOwnerZFWebSocket
        ) {
    ZFWebSocket *owner = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFWebSocket);
    ZFPROTOCOL_ACCESS(ZFWebSocket)->notifyOnOpen(owner);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFWebSocket
        , void, native_1notifyOnClose
        , JNIPointer zfjniPointerOwnerZFWebSocket
        , jint result
        , jstring reasonHint
        ) {
    ZFWebSocket *owner = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFWebSocket);
    ZFPROTOCOL_ACCESS(ZFWebSocket)->notifyOnClose(owner
            , (ZFResultType)result
            , ZFImpl_sys_Android_zfstringFromString(reasonHint)
            );
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFWebSocket
        , void, native_1notifyOnRecv
        , JNIPointer zfjniPointerOwnerZFWebSocket
        , jobject data
        ) {
    ZFWebSocket *owner = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFWebSocket);
    jbyte *nativeData = JNIUtilGetByteArrayElements(jniEnv, (jbyteArray)data, NULL);
    ZFPROTOCOL_ACCESS(ZFWebSocket)->notifyOnRecv(owner
            , zfstring::shared((const zfchar *)nativeData, (zfindex)JNIUtilGetArrayLength(jniEnv, (jbyteArray)data))
            );
    JNIUtilReleaseByteArrayElements(jniEnv, (jbyteArray)data, nativeData, 0);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFWebSocket
        , void, native_1notifyOnRecvBin
        , JNIPointer zfjniPointerOwnerZFWebSocket
        , jobject data
        ) {
    ZFWebSocket *owner = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFWebSocket);
    jbyte *nativeData = JNIUtilGetByteArrayElements(jniEnv, (jbyteArray)data, NULL);
    ZFPROTOCOL_ACCESS(ZFWebSocket)->notifyOnRecvBin(owner
            , zfstring::shared((const zfchar *)nativeData, (zfindex)JNIUtilGetArrayLength(jniEnv, (jbyteArray)data))
            );
    JNIUtilReleaseByteArrayElements(jniEnv, (jbyteArray)data, nativeData, 0);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

