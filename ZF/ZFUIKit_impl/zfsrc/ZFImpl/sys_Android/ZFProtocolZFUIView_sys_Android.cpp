#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIView)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIView, ZFImpl_sys_Android_JNI_NAME_ZFUIView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Android, ZFUIView, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")

public:
    zfoverride
    virtual void protocolOnInitFinish(void) {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_nativeViewCacheOnSave",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)nativeView
            );
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeView
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_nativeViewCacheOnRestore",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)nativeView
            , JNIConvertZFObjectToJNIType(jniEnv, view)
            );
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_nativeViewCreate",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , JNIConvertZFObjectToJNIType(jniEnv, view)
            );
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView) {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = (jobject)nativeView;
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

    virtual void nativeImplView(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeImplViewOld
            , ZF_IN void *nativeImplView
            , ZF_IN zfindex virtualIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_nativeImplView",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jobject)nativeImplView
            , (jint)virtualIndex
            );
    }
    virtual void nativeImplViewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_nativeImplViewFrame",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jint)ZFUIRectRoundX(rect)
            , (jint)ZFUIRectRoundY(rect)
            , (jint)ZFUIRectRoundWidth(rect)
            , (jint)ZFUIRectRoundHeight(rect)
            );
    }
    virtual zffloat UIScaleForImpl(ZF_IN void *nativeView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_UIScaleForImpl",
            JNIGetMethodSig(JNIType::S_float(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zffloat)JNIUtilCallStaticFloatMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)nativeView
            );
    }
    virtual zffloat UIScaleForPixel(ZF_IN void *nativeView) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_UIScaleForPixel",
            JNIGetMethodSig(JNIType::S_float(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        return (zffloat)JNIUtilCallStaticFloatMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)nativeView
            );
    }

    // ============================================================
    // properties
public:
    virtual void visible(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool visible
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_visible",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jboolean)visible
            );
    }
    virtual void alpha(
            ZF_IN ZFUIView *view
            , ZF_IN zffloat alpha
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_alpha",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_float())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jfloat)alpha
            );
    }
    virtual void viewUIEnable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnable
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_viewUIEnable",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jboolean)viewUIEnable
            );
    }
    virtual void viewUIEnableTree(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnableTree
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_viewUIEnableTree",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jboolean)viewUIEnableTree
            );
    }
    virtual void bgColor(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIColor &bgColor
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_bgColor",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , ZFImpl_sys_Android_ZFUIColorToColor(bgColor)
            );
    }

public:
    virtual void child(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_child",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)parent->nativeView()
            , (jobject)child->nativeView()
            , (jint)virtualIndex
            , (jint)childLayer
            , (jint)childLayerIndex
            );
    }
    virtual void childRemove(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_childRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)parent->nativeView()
            , (jint)virtualIndex
            , (jint)childLayer
            , (jint)childLayerIndex
            );
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_childRemoveAllForDealloc",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)parent->nativeView()
            );
    }

public:
    virtual void viewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_viewFrame",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            , (jint)ZFUIRectRoundX(rect)
            , (jint)ZFUIRectRoundY(rect)
            , (jint)ZFUIRectRoundWidth(rect)
            , (jint)ZFUIRectRoundHeight(rect)
            );
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_layoutRequest",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)view->nativeView()
            );
    }

    virtual void measureNativeView(
            ZF_OUT ZFUISize &ret
            , ZF_IN void *nativeView
            , ZF_IN const ZFUISize &sizeHint
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), "native_measureNativeView",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int()), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
                .add(JNIType::S_int())
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIView(), jmId
            , (jobject)nativeView
            , (jint)sizeHint.width
            , (jint)sizeHint.height
            );
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ret.width = (zffloat)jarrSize[0];
        ret.height = (zffloat)jarrSize[1];
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIView
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView
        , void, native_1notifyLayoutView
        , JNIPointer zfjniPointerOwnerZFUIView
        , jint rect_x
        , jint rect_y
        , jint rect_width
        , jint rect_height
        ) {
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView),
        ZFUIRectCreate((zffloat)rect_x, (zffloat)rect_y, (zffloat)rect_width, (zffloat)rect_height));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView
        , void, native_1notifyUIEvent_1mouse
        , JNIPointer zfjniPointerOwnerZFUIView
        , jint mouseId
        , jint mouseAction
        , jint mousePointX
        , jint mousePointY
        ) {
    zfobj<ZFUIMouseEvent> event;
    event->eventResolved(zffalse);
    event->mouseId = mouseId;
    event->mouseAction = (ZFUIMouseAction)mouseAction;
    event->mousePoint = ZFUIPointCreate(mousePointX, mousePointY);
    event->mouseButton = v_ZFUIMouseButton::e_Left;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView),
        event);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView
        , jboolean, native_1notifyUIEvent_1key
        , JNIPointer zfjniPointerOwnerZFUIView
        , jint keyId
        , jint keyAction
        , jint keyCode
        , jint keyCodeRaw
        ) {
    zfobj<ZFUIKeyEvent> event;
    event->eventResolved(zffalse);
    event->keyId = keyId;
    event->keyAction = (ZFUIKeyAction)keyAction;
    event->keyCode = (ZFUIKeyCode)keyCode;
    event->keyCodeRaw = keyCodeRaw;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView),
        event);
    return event->eventResolved();
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

