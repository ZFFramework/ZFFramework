#include "ZFImpl_sys_Android_ZFUIWebKit_impl.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIWebView ZFImpl_sys_Android_JNI_ID(ZFUIWebKit_1impl_ZFUIWebView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIWebView ZFImpl_sys_Android_JNI_NAME(ZFUIWebKit_impl.ZFUIWebView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIWebView, ZFImpl_sys_Android_JNI_NAME_ZFUIWebView)

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_Android, ZFUIWebView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:WebView")

public:
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_nativeWebViewCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, webView->toObject())
            );
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return (void *)ret;
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_nativeWebViewDestroy",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject nativeWebView = (jobject)webView->nativeImplView();
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId, nativeWebView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeWebView);
    }

    virtual void loadUrl(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *url
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_loadUrl",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject urlTmp = JNIUtilNewStringUTF(jniEnv, url);
        JNIScopeDeleteLocalRef(urlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            , urlTmp
            );
    }
    virtual void loadHtml(
            ZF_IN ZFUIWebView *webView
            , ZF_IN const zfchar *html
            , ZF_IN_OPT const zfchar *baseUrl = zfnull
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_loadHtml",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        jobject htmlTmp = JNIUtilNewStringUTF(jniEnv, html);
        JNIScopeDeleteLocalRef(htmlTmp);
        jobject baseUrlTmp = ((baseUrl == zfnull) ? NULL : JNIUtilNewStringUTF(jniEnv, baseUrl));
        JNIScopeDeleteLocalRef(baseUrlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            , htmlTmp
            , baseUrlTmp
            );
    }
    virtual void reload(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_reload",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }
    virtual void loadStop(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_loadStop",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }

    virtual void goBack(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_goBack",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }
    virtual void goForward(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_goForward",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }

    virtual zfbool loading(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_loading",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }
    virtual zfbool goBackAvailable(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_goBackAvailable",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }
    virtual zfbool goForwardAvailable(ZF_IN ZFUIWebView *webView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), "native_goForwardAvailable",
            JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIWebView(), jmId
            , (jobject)webView->nativeImplView()
            );
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIWebView
        , void, native_1notifyWebLoadStateOnUpdate
        , JNIPointer zfjniPointerOwnerZFUIWebView
        ) {
    ZFPROTOCOL_ACCESS(ZFUIWebView)->notifyWebLoadStateOnUpdate(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIWebView));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

