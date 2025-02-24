#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUISysWindow ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUISysWindow)
#define ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUISysWindow)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFUISysWindow, ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_Android, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
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
    virtual ZFUISysWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());

            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_nativeMainWindowCreate",
                JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
                , JNIConvertZFObjectToJNIType(jniEnv, this->_mainWindow)
                );
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            ZFUISysWindow *mainWindowTmp = this->_mainWindow;
            this->_mainWindow = zfnull;
            zfRelease(mainWindowTmp);
        }
    }
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_nativeWindowRootViewOnAdd",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        nativeParentView = (void *)JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindow->nativeWindow()
            , (jobject)sysWindow->rootView()->nativeView()
            );
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_nativeWindowRootViewOnRemove",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindow->nativeWindow()
            , (jobject)sysWindow->rootView()->nativeView()
            );
    }

    virtual zfauto modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) {
        if(sysWindowOwner->nativeWindow() == zfnull) {
            return zfnull;
        }
        zfauto modalWindow = ZFUISysWindow::ClassData()->newInstance();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_modalWindowShow",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindowOwner->nativeWindow()
            , JNIConvertZFObjectToJNIType(jniEnv, modalWindow)
            );

        return modalWindow;
    }
    virtual void modalWindowFinish(
            ZF_IN ZFUISysWindow *sysWindowOwner
            , ZF_IN ZFUISysWindow *sysWindowToFinish
            ) {
        if(sysWindowToFinish->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_modalWindowFinish",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindowToFinish->nativeWindow()
            );
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow) {
    }
    virtual void sysWindowLayoutParamOnUpdate(ZF_IN ZFUISysWindow *sysWindow) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_sysWindowLayoutParamOnUpdate",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindow->nativeWindow()
            );
    }

    virtual ZFUIOrientation sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow) {
        if(sysWindow->nativeWindow() == zfnull) {
            return ZFUIOrientation::e_Top;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_sysWindowOrientation",
            JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindow->nativeWindow()
            );
        return (ZFUIOrientation)ret;
    }
    virtual void sysWindowOrientationFlags(
            ZF_IN ZFUISysWindow *sysWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
        if(sysWindow->nativeWindow() == zfnull) {
            return;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), "native_sysWindowOrientationFlags",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
                .add(JNIType::S_int())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFUISysWindow(), jmId
            , (jobject)sysWindow->nativeWindow()
            , (jint)flags
            );
    }

private:
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1mainWindowRegisterForNativeView
        , jobject nativeParent
        ) {
    zfautoT<ZFUISysWindow> mainWindow = ZFUISysWindow::nativeWindowEmbedNativeView((void *)nativeParent);
    ZFUISysWindow::mainWindowRegister(mainWindow);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1nativeWindowEmbedNativeView
        , jobject nativeParent
        , jobject sysWindowName
        ) {
    ZFUISysWindow::nativeWindowEmbedNativeView((void *)nativeParent, ZFImpl_sys_Android_zfstringFromString(sysWindowName));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyMeasureWindow
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        , jint refWidth
        , jint refHeight
        , jintArray resultRect
        ) {
    ZFUIRect result = ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyMeasureWindow(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow),
        ZFUIRectCreate(0, 0, refWidth, refHeight),
        ZFUIMarginZero());
    jint buf[] = {(jint)result.x, (jint)result.y, (jint)result.width, (jint)result.height};
    JNIUtilSetIntArrayRegion(jniEnv, resultRect, 0, 4, buf);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyOnCreate
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        , jobject nativeWindow
        ) {
    jobject nativeWindowGlobalRef = JNIUtilNewGlobalRef(jniEnv, nativeWindow);
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnCreate(
        JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow),
        (void *)nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyOnDestroy
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        ) {
    ZFUISysWindow *window = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow);
    jobject nativeWindowGlobalRef = (jobject)window->nativeWindow();
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnDestroy(window);
    JNIUtilDeleteGlobalRef(jniEnv, nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyOnResume
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyOnPause
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , void, native_1notifyOnRotate
        , JNIPointer zfjniPointerOwnerZFUISysWindow
        ) {
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnRotate(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow));
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow
        , jboolean, native_1notifyKeyEvent
        , JNIPointer zfjniPointerOwnerZFUISysWindow
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
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyKeyEvent(JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow), event);
    return event->eventResolved();
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

