#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindow.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIRootWindow)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIRootWindow ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIRootWindow)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUIRootWindow, ZFImpl_sys_Android_JNI_NAME_ZFUIRootWindow)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIRootWindowImpl_sys_Android, ZFUIRootWindow, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Activity")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
public:
    zfoverride
    virtual ZFUIRootWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfobjRetain(ZFUIRootWindow::ClassData()->newInstance().to<ZFUIRootWindow *>());

            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_nativeMainWindowCreate",
                JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
                , JNIConvertZFObjectToJNIType(jniEnv, this->_mainWindow)
                );
        }
        return this->_mainWindow;
    }
    zfoverride
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            ZFUIRootWindow *mainWindowTmp = this->_mainWindow;
            this->_mainWindow = zfnull;
            zfobjRelease(mainWindowTmp);
        }
    }
    zfoverride
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    zfoverride
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    zfoverride
    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_nativeWindowRootViewOnAdd",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        nativeParentView = (void *)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            , (jobject)rootWindow->rootView()->nativeView()
            );
    }
    zfoverride
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_nativeWindowRootViewOnRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            , (jobject)rootWindow->rootView()->nativeView()
            );
    }

    zfoverride
    virtual zfauto modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
        if(owner->nativeWindow() == zfnull) {
            return zfnull;
        }
        zfauto modalWindow = ZFUIRootWindow::ClassData()->newInstance();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_modalWindowShow",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)owner->nativeWindow()
            , JNIConvertZFObjectToJNIType(jniEnv, modalWindow)
            );

        return modalWindow;
    }
    zfoverride
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) {
        if(toHide->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_modalWindowHide",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)toHide->nativeWindow()
            );
    }

    zfoverride
    virtual void layoutParamOnInit(ZF_IN ZFUIRootWindow *rootWindow) {
    }
    zfoverride
    virtual void layoutParamOnUpdate(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_layoutParamOnUpdate",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_boolean())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            , (jboolean)rootWindow->preferFullscreen()
            );
    }

    zfoverride
    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return v_ZFUIOrientation::e_Top;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_windowOrientation",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            );
        return (ZFUIOrientation)ret;
    }
    zfoverride
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_windowOrientationFlags",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            , (jint)flags
            );
    }
    zfoverride
    virtual void windowColor(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIColor &color
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), "native_windowColor",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUIRootWindow(), jmId
            , (jobject)rootWindow->nativeWindow()
            , (jint)ZFImpl_sys_Android_ZFUIColorToColor(color)
            );
    }

private:
    ZFUIRootWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIRootWindowImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1mainWindowRegisterForNativeView
        , jobject nativeParent
        ) {
    zfautoT<ZFUIRootWindow> mainWindow = ZFUIRootWindow::nativeWindowEmbedNativeView((void *)nativeParent);
    ZFUIRootWindow::mainWindowRegister(mainWindow);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1nativeWindowEmbedNativeView
        , jobject nativeParent
        , jobject rootWindowName
        ) {
    ZFUIRootWindow::nativeWindowEmbedNativeView((void *)nativeParent, ZFImpl_sys_Android_zfstringFromString(rootWindowName));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyMeasureWindow
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        , jint refWidth
        , jint refHeight
        , jint marginLeft
        , jint marginTop
        , jint marginRight
        , jint marginBottom
        , jintArray resultRect
        ) {
    ZFUIRect result = ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyMeasureWindow(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow)
        , ZFUIRectCreate(0, 0, refWidth, refHeight)
        , ZFUIMarginCreate(marginLeft, marginTop, marginRight, marginBottom)
        );
    jint buf[] = {(jint)result.x, (jint)result.y, (jint)result.width, (jint)result.height};
    JNIUtilSetIntArrayRegion(jniEnv, resultRect, 0, 4, buf);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyOnCreate
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        , jobject nativeWindow
        ) {
    jobject nativeWindowGlobalRef = JNIUtilNewGlobalRef(jniEnv, nativeWindow);
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnCreate(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow),
        (void *)nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyOnDestroy
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        ) {
    ZFUIRootWindow *window = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow);
    jobject nativeWindowGlobalRef = (jobject)window->nativeWindow();
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnDestroy(window);
    JNIUtilDeleteGlobalRef(jniEnv, nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyOnResume
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnResume(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyOnPause
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnPause(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , void, native_1notifyOnRotate
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnRotate(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow));
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIRootWindow
        , jboolean, native_1notifyKeyEvent
        , JNIPointer zfjniPointerOwnerZFUIRootWindow
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
    ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyKeyEvent(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIRootWindow), event);
    return event->eventResolved();
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

