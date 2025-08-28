#include "ZFImpl_sys_Android_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFHttpRequest ZFImpl_sys_Android_JNI_ID(ZFNet_1impl_ZFHttpRequest)
#define ZFImpl_sys_Android_JNI_NAME_ZFHttpRequest ZFImpl_sys_Android_JNI_NAME(ZFNet_impl.ZFHttpRequest)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFHttpRequest, ZFImpl_sys_Android_JNI_NAME_ZFHttpRequest)

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFHttpRequestImpl_sys_Android_Task {
public:
    zfobj<v_zfbool> running;
    ZFHttpRequest *ownerRequest;
    zfautoT<ZFHttpResponse> ownerResponse;
    zfstring body;
    jobject nativeTask;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_sys_Android, ZFHttpRequest, v_ZFProtocolLevel::e_SystemNormal)
    // ============================================================
    // for request
public:
    virtual zfbool httpsAvailable(void) {
        return zftrue;
    }

    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_nativeTaskCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = zfnew(_ZFP_ZFHttpRequestImpl_sys_Android_Task);
        task->running->zfv = zftrue;
        task->ownerRequest = request;
        task->nativeTask = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , JNIConvertZFObjectToJNIType(jniEnv, request)
                );
        JNIScopeDeleteLocalRef((jobject)task->nativeTask);
        task->nativeTask = JNIUtilNewGlobalRef(jniEnv, (jobject)task->nativeTask);
        return task;
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_nativeTaskDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        ZFCoreMutexLocker();
        task->running->zfv = zffalse;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId, (jobject)task->nativeTask);
        JNIUtilDeleteGlobalRef(jniEnv, (jobject)task->nativeTask);
        zfdelete(task);
    }

    virtual void url(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &url
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_url",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
                .add(JNIType::S_int())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(url)
                , (jint)task->ownerRequest->timeout()
            );
    }

    virtual void httpMethod(
            ZF_IN void *nativeTask
            , ZF_IN ZFHttpMethod httpMethod
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_httpMethod",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(ZFHttpMethodToString(httpMethod))
            );
    }

    virtual void header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_header",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
                .add(JNIType::S_object_String())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
                , ZFImpl_sys_Android_zfstringToString(value)
            );
    }

    virtual void headerRemove(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_headerRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
            );
    }

    virtual zfstring header(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_header",
            JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
            );
        JNIScopeDeleteLocalRef(jString);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_headerCount",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return (zfindex)JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
            );
    }

    virtual zfiter headerIter(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_headerMap",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject nativeMap = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
            );
        JNIScopeDeleteLocalRef(nativeMap);
        return ZFImpl_sys_Android_MapIter(nativeMap);
    }
    virtual zfstring headerIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        jobject nativeKey = ZFImpl_sys_Android_MapIterKey(it);
        JNIScopeDeleteLocalRef(nativeKey);
        return ZFImpl_sys_Android_zfstringFromString(nativeKey);
    }
    virtual zfstring headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        jobject nativeKey = ZFImpl_sys_Android_MapIterValue(it);
        JNIScopeDeleteLocalRef(nativeKey);
        return ZFImpl_sys_Android_zfstringFromString(nativeKey);
    }
    virtual void headerIterValue(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            ) {
        ZFImpl_sys_Android_MapIterValue(it, ZFImpl_sys_Android_zfstringToString(value));
    }
    virtual void headerIterRemove(
            ZF_IN void *nativeTask
            , ZF_IN_OUT zfiter &it
            ) {
        ZFImpl_sys_Android_MapIterRemove(it);
    }

    virtual void body(
            ZF_IN void *nativeTask
            , ZF_IN const void *buffer
            , ZF_IN zfindex byteSize
            ) {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        task->body.append(buffer, byteSize);
    }
    virtual zfstring body(ZF_IN void *nativeTask) {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return task->body;
    }

    virtual void request(ZF_IN void *nativeTask, ZF_IN ZFHttpResponse *response) {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        task->ownerResponse = response;
        zfautoT<v_zfbool> running = task->running;
        ZFLISTENER_2(run
                , zfautoT<v_zfbool>, running
                , _ZFP_ZFHttpRequestImpl_sys_Android_Task *, task
                ) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_request",
                JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                    .add(JNIType::S_object_Object())
                    .add(JNIType::S_object_Object())
                    .add(JNIPointerJNIType)
                ).c_str());

            ZFCoreMutexLock();
            if(!running->zfv) {
                ZFCoreMutexUnlock();
                return;
            }
            jobject nativeTask = (jobject)task->nativeTask;
            jobject nativeInput = task->body.length() > 0
                ? ZFImpl_sys_Android_ZFInputWrapperFromZFInput(ZFInputForString(task->body))
                : NULL;
            ZFCoreMutexUnlock();

            JNIUtilCallStaticVoidMethod(JNIGetJNIEnv(), ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                    , nativeTask
                    , nativeInput
                    , JNIConvertZFObjectToJNIType(jniEnv, task->ownerResponse)
                );
        } ZFLISTENER_END()
        zfasync(run);
    }

    virtual void requestCancel(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_requestCancel",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
            );
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(
            ZF_IN void *nativeTask
            , ZF_IN const zfstring &key
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_responseHeader",
            JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_String())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
            );
        JNIScopeDeleteLocalRef(jString);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_responseHeaderCount",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return (zfindex)JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
            );
    }

    virtual zfiter responseHeaderIter(ZF_IN void *nativeTask) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), "native_responseHeaderMap",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject nativeMap = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFHttpRequest(), jmId
                , (jobject)task->nativeTask
            );
        JNIScopeDeleteLocalRef(nativeMap);
        return ZFImpl_sys_Android_MapIter(nativeMap);
    }
    virtual zfstring responseHeaderIterKey(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        jobject nativeKey = ZFImpl_sys_Android_MapIterKey(it);
        JNIScopeDeleteLocalRef(nativeKey);
        return ZFImpl_sys_Android_zfstringFromString(nativeKey);
    }
    virtual zfstring responseHeaderIterValue(
            ZF_IN void *nativeTask
            , ZF_IN const zfiter &it
            ) {
        jobject nativeKey = ZFImpl_sys_Android_MapIterKey(it);
        JNIScopeDeleteLocalRef(nativeKey);
        return ZFImpl_sys_Android_zfstringFromString(nativeKey);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFHttpRequest
        , void, native_1notifyResponse
        , JNIPointer zfjniPointerOwnerZFHttpRequest
        , JNIPointer zfjniPointerOwnerZFHttpResponse
        , jint code
        , jstring errorHint
        , jobject nativeBodyInput
        ) {
    ZFHttpRequest *request = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFHttpRequest);
    ZFHttpResponse *response = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFHttpResponse);
    response->success(code >= 200 && code < 300);
    response->code(code);
    response->errorHint(ZFImpl_sys_Android_zfstringFromString(errorHint));
    if(nativeBodyInput != NULL) {
        ZFInput bodyInput = ZFImpl_sys_Android_ZFInputFromZFAndroidInput(nativeBodyInput);
        zfstring body;
        if(ZFInputRead(body, bodyInput) == zfindexMax()) {
            response->success(zffalse);
        }
        response->body(body);
    }
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(request, response);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

