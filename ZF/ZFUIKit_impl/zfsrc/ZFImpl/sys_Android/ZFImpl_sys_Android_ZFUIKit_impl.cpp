#include "ZFImpl_sys_Android_ZFUIKit_impl.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// Point
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidPoint, ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint)
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(
        ZF_IN const ZFUIPoint &point
        , ZF_IN_OUT jobject jobjPoint
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, "x", JNIType::S_int().getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, "y", JNIType::S_int().getId());
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdX, (jint)point.x);
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdY, (jint)point.y);
    return jobjPoint;
}
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsPoint, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    jobject jobjPoint = JNIUtilNewObject(jniEnv, jclsPoint, jmId);
    return ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(point, jobjPoint);
}
void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPointT(
        ZF_OUT ZFUIPoint &ret
        , ZF_IN jobject jobjPoint
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, "x", JNIType::S_int().getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, "y", JNIType::S_int().getId());
    ret.x = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdX);
    ret.y = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdY);
}

// ============================================================
// Size
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidSize, ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize)
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(
        ZF_IN const ZFUISize &size
        , ZF_IN_OUT jobject jobjSize
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, "width", JNIType::S_int().getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, "height", JNIType::S_int().getId());
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdWidth, (jint)size.width);
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdHeight, (jint)size.height);
    return jobjSize;
}
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsSize, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    jobject jobjSize = JNIUtilNewObject(jniEnv, jclsSize, jmId);
    return ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(size, jobjSize);
}
void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSizeT(
        ZF_OUT ZFUISize &ret
        , ZF_IN jobject jobjSize
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, "width", JNIType::S_int().getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, "height", JNIType::S_int().getId());
    ret.width = JNIUtilGetIntField(jniEnv, jobjSize, jfIdWidth);
    ret.height = JNIUtilGetIntField(jniEnv, jobjSize, jfIdHeight);
}

// ============================================================
// Margin
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidMargin, ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin)
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(
        ZF_IN const ZFUIMargin &margin
        , ZF_IN_OUT jobject jobjMargin
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, "left", JNIType::S_int().getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, "top", JNIType::S_int().getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, "right", JNIType::S_int().getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, "bottom", JNIType::S_int().getId());
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdLeft, (jint)margin.left);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdTop, (jint)margin.top);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdRight, (jint)margin.right);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdBottom, (jint)margin.bottom);
    return jobjMargin;
}
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsMargin, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    jobject jobjMargin = JNIUtilNewObject(jniEnv, jclsMargin, jmId);
    return ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(margin, jobjMargin);
}
void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMarginT(
        ZF_OUT ZFUIMargin &ret
        , ZF_IN jobject jobjMargin
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, "left", JNIType::S_int().getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, "top", JNIType::S_int().getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, "right", JNIType::S_int().getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, "bottom", JNIType::S_int().getId());
    ret.left = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdLeft);
    ret.top = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdTop);
    ret.right = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdRight);
    ret.bottom = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdBottom);
}

// ============================================================
// Rect
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidRect, ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect)
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(
        ZF_IN const ZFUIRect &rect
        , ZF_IN_OUT jobject jobjRect
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, "x", JNIType::S_int().getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, "y", JNIType::S_int().getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, "width", JNIType::S_int().getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, "height", JNIType::S_int().getId());
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdX, (jint)rect.x);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdY, (jint)rect.y);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdWidth, (jint)rect.width);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdHeight, (jint)rect.height);
    return jobjRect;
}
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsRect, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
        ).c_str());
    jobject jobjRect = JNIUtilNewObject(jniEnv, jclsRect, jmId);
    return ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(rect, jobjRect);
}
void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRectT(
        ZF_OUT ZFUIRect &ret
        , ZF_IN jobject jobjRect
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, "x", JNIType::S_int().getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, "y", JNIType::S_int().getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, "width", JNIType::S_int().getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, "height", JNIType::S_int().getId());
    ret.x = JNIUtilGetIntField(jniEnv, jobjRect, jfIdX);
    ret.y = JNIUtilGetIntField(jniEnv, jobjRect, jfIdY);
    ret.width = JNIUtilGetIntField(jniEnv, jobjRect, jfIdWidth);
    ret.height = JNIUtilGetIntField(jniEnv, jobjRect, jfIdHeight);
}

jint ZFImpl_sys_Android_ZFUIColorToColor(ZF_IN const ZFUIColor &color) {
    return (jint)(0
            | ((zfuint)(ZFUIColorGetA(color) * 0xFF) << 24)
            | ((zfuint)(ZFUIColorGetR(color) * 0xFF) << 16)
            | ((zfuint)(ZFUIColorGetG(color) * 0xFF) << 8)
            | ((zfuint)(ZFUIColorGetB(color) * 0xFF) << 0)
        );
}
void ZFImpl_sys_Android_ZFUIColorFromColorT(
        ZF_OUT ZFUIColor &ret
        , ZF_IN jint jColor
        ) {
    ret = ZFUIColorCreate(
            (zfuint)((jColor >> 16) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 8) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 0) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 24) & 0xFF) / 255.0f
        );
}

// ============================================================
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidUI, ZFImpl_sys_Android_JNI_NAME_ZFAndroidUI)

void ZFImpl_sys_Android_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN jobject nativeView
        ) {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidUI(), "native_viewTreePrint",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidUI(), jmId, nativeView);
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, (jstring)tmp, NULL);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, (jstring)tmp, utf);
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUISysWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_Android_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUISysWindow *sysWindow = zfargs.sender();
            zfstring s;
            ZFImpl_sys_Android_viewTreePrintT(s, (jobject)sysWindow->rootView()->nativeView());
            ZFLogTrim() << s;
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::E_SysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_Android

