#include "ZFImpl_sys_Android_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFHttpRequest.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFHttpRequest ZFImpl_sys_Android_JNI_ID(ZFNet_1impl_ZFHttpRequest)
#define ZFImpl_sys_Android_JNI_NAME_ZFHttpRequest ZFImpl_sys_Android_JNI_NAME(ZFNet_impl.ZFHttpRequest)

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFHttpRequestImpl_sys_Android_Task
{
public:
    v_zfbool *running;
    ZFHttpRequest *request;
    ZFBuffer body;
    jobject nativeTask;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFHttpRequestImpl_sys_Android, ZFHttpRequest, ZFProtocolLevel::e_SystemNormal)

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFHttpRequest).c_str());
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

    // ============================================================
    // for request
public:
    virtual void *nativeTaskCreate(ZF_IN ZFHttpRequest *request,
                                   ZF_IN ZFHttpResponse *response)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeTaskCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_long)
                .add(JNIType::S_long)
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = zfnew(_ZFP_ZFHttpRequestImpl_sys_Android_Task);
        task->running = zfAlloc(v_zfbool, zftrue);
        task->request = request;
        task->nativeTask = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , JNIConvertZFObjectToJNIType(jniEnv, request)
                , JNIConvertZFObjectToJNIType(jniEnv, response)
            );
        JNIBlockedDeleteLocalRefWithEnv((jobject)task->nativeTask, jniEnv);
        task->nativeTask = JNIUtilNewGlobalRef(jniEnv, (jobject)task->nativeTask);
        return task;
    }
    virtual void nativeTaskDestroy(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeTaskDestroy",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        zfCoreMutexLocker();
        task->running->zfv = zffalse;
        zfRelease(task->running);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, (jobject)task->nativeTask);
        JNIUtilDeleteGlobalRef(jniEnv, (jobject)task->nativeTask);
        zfdelete(task);
    }

    virtual void url(ZF_IN void *nativeTask,
                     ZF_IN const zfchar *url)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_url",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
                .add(JNIType::S_int)
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(url)
                , (int)task->request->timeout()
            );
    }

    virtual void httpMethod(ZF_IN void *nativeTask,
                            ZF_IN const zfchar *method)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_httpMethod",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(method)
            );
    }

    virtual void header(ZF_IN void *nativeTask,
                        ZF_IN const zfchar *key,
                        ZF_IN const zfchar *value)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_header",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
                , ZFImpl_sys_Android_zfstringToString(value)
            );
    }

    virtual zfstring header(ZF_IN void *nativeTask,
                            ZF_IN const zfchar *key)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_header",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }

    virtual zfindex headerCount(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerCount",
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return (zfindex)JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
            );
    }

    virtual zfiterator headerIter(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIter",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject nativeIt = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
            );
        JNIBlockedDeleteLocalRefWithEnv(nativeIt, jniEnv);
        return zfiterator((void *)JNIUtilNewGlobalRef(jniEnv, nativeIt), _ZFP_iterDelete, _ZFP_iterCopy);
    }
    virtual zfbool headerIterValid(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterValid",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
    }
    virtual void headerIterNext(ZF_IN void *nativeTask,
                                ZF_IN_OUT zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterNext",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
    }
    virtual zfstring headerIterKey(ZF_IN void *nativeTask,
                                   ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterKey",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }
    virtual zfstring headerIterValue(ZF_IN void *nativeTask,
                                     ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterValue",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }
    virtual void headerIterValue(ZF_IN void *nativeTask,
                                 ZF_IN_OUT zfiterator &it,
                                 ZF_IN const zfchar *value)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterValue",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
                , ZFImpl_sys_Android_zfstringToString(value)
            );
    }
    virtual void headerIterRemove(ZF_IN void *nativeTask,
                                  ZF_IN_OUT zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_headerIterRemove",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
    }

    virtual void body(ZF_IN void *nativeTask,
                      ZF_IN const void *buffer,
                      ZF_IN zfindex byteSize)
    {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        task->body.bufferAppend(buffer, byteSize);
    }
    virtual ZFBuffer body(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return task->body;
    }

    virtual void request(ZF_IN void *nativeTask)
    {
        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        zfautoObjectT<v_zfbool *> running = task->running;
        ZFLISTENER_3(run
                , jclass, jclsOwner
                , zfautoObjectT<v_zfbool *>, running
                , _ZFP_ZFHttpRequestImpl_sys_Android_Task *, task
                ) {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsOwner, "native_request",
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                ).c_str());

            zfCoreMutexLock();
            if(!running->zfv)
            {
                zfCoreMutexUnlock();
                return;
            }
            jobject nativeTask = (jobject)task->nativeTask;
            ZFBuffer bufTmp = task->body;
            jobject nativeBuf = ZFImpl_sys_Android_ZFAndroidBufferToJava(bufTmp.buffer(), bufTmp.bufferSize());
            zfCoreMutexUnlock();

            JNIUtilCallStaticVoidMethod(JNIGetJNIEnv(), jclsOwner, jmId
                    , nativeTask
                    , nativeBuf
                );
        } ZFLISTENER_END(run)
        zfasync(run);
    }

    virtual void requestCancel(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_requestCancel",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
            );
    }

    // ============================================================
    // for response
public:
    virtual zfstring responseHeader(ZF_IN void *nativeTask,
                                    ZF_IN const zfchar *key)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeader",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , ZFImpl_sys_Android_zfstringToString(key)
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }

    virtual zfindex responseHeaderCount(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderCount",
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return (zfindex)JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
            );
    }

    virtual zfiterator responseHeaderIter(ZF_IN void *nativeTask)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderIter",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject nativeIt = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
            );
        JNIBlockedDeleteLocalRefWithEnv(nativeIt, jniEnv);
        return zfiterator((void *)JNIUtilNewGlobalRef(jniEnv, nativeIt), _ZFP_iterDelete, _ZFP_iterCopy);
    }
    virtual zfbool responseHeaderIterValid(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderIterValid",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
    }
    virtual void responseHeaderIterNext(ZF_IN void *nativeTask,
                                        ZF_IN_OUT zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderIterNext",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
    }
    virtual zfstring responseHeaderIterKey(ZF_IN void *nativeTask,
                                           ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderIterKey",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }
    virtual zfstring responseHeaderIterValue(ZF_IN void *nativeTask,
                                             ZF_IN const zfiterator &it)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_responseHeaderIterValue",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        _ZFP_ZFHttpRequestImpl_sys_Android_Task *task = (_ZFP_ZFHttpRequestImpl_sys_Android_Task *)nativeTask;
        jobject jString = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId
                , (jobject)task->nativeTask
                , (jobject)it.data()
            );
        JNIBlockedDeleteLocalRefWithEnv(jString, jniEnv);
        return ZFImpl_sys_Android_zfstringFromString(jString);
    }

private:
    jclass jclsOwner;
private:
    static void _ZFP_iterDelete(void *data)
    {
        JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), (jobject)data);
    }
    static void *_ZFP_iterCopy(void *data)
    {
        return (void *)JNIUtilNewGlobalRef(JNIGetJNIEnv(), (jobject)data);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFHttpRequestImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFHttpRequestImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFHttpRequest,
                         void, native_1ZFHttpRequest_1notifyResponse,
                         JNIPointer zfjniPointerOwnerZFHttpRequest,
                         JNIPointer zfjniPointerOwnerZFHttpResponse,
                         jint code,
                         jstring errorHint,
                         jobject body)
{
    ZFHttpResponse *response = ZFCastZFObject(ZFHttpResponse *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFHttpResponse));
    response->success(code == 200);
    response->code(code);
    response->errorHint(ZFImpl_sys_Android_zfstringFromString(errorHint));
    ZFImpl_sys_Android_Buffer buffer = ZFImpl_sys_Android_ZFAndroidBufferFromJava(body);
    if(buffer.buffer != zfnull)
    {
        response->body().bufferCopy(buffer.buffer, buffer.bufferSize);
    }
    ZFPROTOCOL_ACCESS(ZFHttpRequest)->notifyResponse(
        ZFCastZFObject(ZFHttpRequest *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFHttpRequest)));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

