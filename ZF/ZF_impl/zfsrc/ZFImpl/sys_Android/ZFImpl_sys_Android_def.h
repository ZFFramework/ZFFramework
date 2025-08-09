#ifndef _ZFI_ZFImpl_sys_Android_def_h_
#define _ZFI_ZFImpl_sys_Android_def_h_

#include "../ZFImpl_ZF_impl.h"
#include "ZFImpl_sys_Android.h"
#include "ZFCore.h"

#if ZF_ENV_sys_Android

// #define _ZFP_ZFImpl_sys_Android_JNI_DEBUG 1

#if _ZFP_ZFImpl_sys_Android_JNI_DEBUG
    #include <jni.h>
    #include <android/log.h>
    #define JNIUtilWrap_Log(fmt, ...) \
        ((void)__android_log_print(ANDROID_LOG_ERROR, "test", fmt, ##__VA_ARGS__))

    #define JNIUtilWrap_Enable 1

    inline const char *_JNIUtilWrap_file(const char *filePath) {
        const char *ret = filePath;
        while(*filePath) {
            if(*filePath == '/' || *filePath == '\\') {
                ret = filePath + 1;
            }
            ++filePath;
        }
        return ret;
    }
    template<typename T_Obj>
    T_Obj _JNIUtilWrap_LogAction(
            T_Obj obj
            , const char *callerFile, const char *callerFunction, int callerLine
            , const char *actionName, const char *type
            ) {
        JNIUtilWrap_Log("[%s %s (%s)] %s %s %s",
            _JNIUtilWrap_file(callerFile), callerFunction, callerLine,
            type, actionName, (const void *)obj);
        return obj;
    }

    #if 1
        #define JNIUtilWrap_Created(obj, callerFile, callerFunction, callerLine, actionName) \
            _JNIUtilWrap_LogAction(obj, callerFile, callerFunction, callerLine, actionName, "Create")
    #endif
    #ifndef JNIUtilWrap_Released
        #define JNIUtilWrap_Released(obj, callerFile, callerFunction, callerLine, actionName) \
            _JNIUtilWrap_LogAction(obj, callerFile, callerFunction, callerLine, actionName, "Release")
    #endif
    #ifndef JNIUtilWrap_FuncCalled
        #if 1
            #include <ctime>
            class _JNIUtilWrap_FuncCalledTimeLog {
            public:
                _JNIUtilWrap_FuncCalledTimeLog(const char *callerFunction)
                : _callerFunction(callerFunction)
                , _startTime(clock())
                {
                }
                ~_JNIUtilWrap_FuncCalledTimeLog(void) {
                    JNIUtilWrap_Log("[%s] %s",
                        _callerFunction,
                        (double)((double)(clock() - _startTime) / CLOCKS_PER_SEC));
                }
            private:
                const char *_callerFunction;
                clock_t _startTime;
            };
            #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName)  \
                _JNIUtilWrap_FuncCalledTimeLog(callerFunction), \
                _JNIUtilWrap_LogAction(NULL, callerFile, callerFunction, callerLine, actionName, "FuncCall")
        #else
            #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName)  \
                _JNIUtilWrap_LogAction(NULL, callerFile, callerFunction, callerLine, actionName, "FuncCall")
        #endif
    #endif
    #ifndef JNIUtilWrap_CallbackCalled
        #define JNIUtilWrap_CallbackCalled(className, actionName) \
            JNIUtilWrap_Log("[%s %s (%s)] Callback %s %s", _JNIUtilWrap_file(__FILE__), __FUNCTION__, __LINE__, className, actionName);
    #endif
#endif // #if _ZFP_ZFImpl_sys_Android_JNI_DEBUG

#include "ZFImpl/tools/JNIUtil/AndroidJNIUtil.h"
using namespace JNIUtil;
using namespace AndroidJNIUtil;

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_ROOT_JNI_ID com_ZFFramework
#define ZFImpl_sys_Android_ROOT_JNI_NAME "com.ZFFramework"

// usually used when declare JNI method
// note: when class name contains `_`, it must be replaced with `_1`
#define ZFImpl_sys_Android_JNI_ID(clsId) ZFM_CAT(ZFM_CAT(ZFImpl_sys_Android_ROOT_JNI_ID, _), clsId)
// usually used when find class
#define ZFImpl_sys_Android_JNI_NAME(clsName) ZFImpl_sys_Android_ROOT_JNI_NAME "." ZFM_TOSTRING(clsName)

// ============================================================
// typeid
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZF_impl, ZFAndroid_JNIGlobalRef, JNIGlobalRef)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZF_impl, ZFAndroid_JNIGlobalRef, JNIGlobalRef)
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZF_impl, JNIGlobalRef)

ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZF_impl, ZFAndroid_JNIGlobalRef, JNIGlobalRef, ZFAndroid_jobject, jobject)
ZFTYPEID_ALIAS_REG(ZFLIB_ZF_impl, ZFAndroid_JNIGlobalRef, JNIGlobalRef, ZFAndroid_jobject, jobject)

// ============================================================
// Object
#define ZFImpl_sys_Android_JNI_ID_Object java_lang_Object
#define ZFImpl_sys_Android_JNI_NAME_Object "java.lang.Object"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassObject(void);

// ============================================================
// String
#define ZFImpl_sys_Android_JNI_ID_String java_lang_String
#define ZFImpl_sys_Android_JNI_NAME_String "java.lang.String"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassString(void);

// ============================================================
// Number
#define ZFImpl_sys_Android_JNI_ID_Number java_lang_Number
#define ZFImpl_sys_Android_JNI_NAME_Number "java.lang.Number"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassNumber(void);

// ============================================================
// Boolean
#define ZFImpl_sys_Android_JNI_ID_Boolean java_lang_Boolean
#define ZFImpl_sys_Android_JNI_NAME_Boolean "java.lang.Boolean"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassBoolean(void);

extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_jobjectBooleanTRUE(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_jobjectBooleanFALSE(void);
inline jobject ZFImpl_sys_Android_newBoolean(ZF_IN jboolean v) {
    if(v) {
        return ZFImpl_sys_Android_jobjectBooleanTRUE();
    }
    else {
        return ZFImpl_sys_Android_jobjectBooleanFALSE();
    }
}

// ============================================================
// Byte
#define ZFImpl_sys_Android_JNI_ID_Byte java_lang_Byte
#define ZFImpl_sys_Android_JNI_NAME_Byte "java.lang.Byte"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassByte(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newByte(ZF_IN jbyte v);

// ============================================================
// Character
#define ZFImpl_sys_Android_JNI_ID_Character java_lang_Character
#define ZFImpl_sys_Android_JNI_NAME_Character "java.lang.Character"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassCharacter(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newCharacter(ZF_IN jchar v);

// ============================================================
// Short
#define ZFImpl_sys_Android_JNI_ID_Short java_lang_Short
#define ZFImpl_sys_Android_JNI_NAME_Short "java.lang.Short"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassShort(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newShort(ZF_IN jshort v);

// ============================================================
// Integer
#define ZFImpl_sys_Android_JNI_ID_Integer java_lang_Integer
#define ZFImpl_sys_Android_JNI_NAME_Integer "java.lang.Integer"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassInteger(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newInteger(ZF_IN jint v);

// ============================================================
// Long
#define ZFImpl_sys_Android_JNI_ID_Long java_lang_Long
#define ZFImpl_sys_Android_JNI_NAME_Long "java.lang.Long"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassLong(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newLong(ZF_IN jlong v);

// ============================================================
// Float
#define ZFImpl_sys_Android_JNI_ID_Float java_lang_Float
#define ZFImpl_sys_Android_JNI_NAME_Float "java.lang.Float"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassFloat(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newFloat(ZF_IN jfloat v);

// ============================================================
// Double
#define ZFImpl_sys_Android_JNI_ID_Double java_lang_Double
#define ZFImpl_sys_Android_JNI_NAME_Double "java.lang.Double"
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassDouble(void);
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_newDouble(ZF_IN jdouble v);

// ============================================================
// ZFAndroidLog
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidLog ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidLog)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidLog)

// ============================================================
// jclass util
zfclassLikePOD ZFLIB_ZF_impl _ZFP_ZFImpl_sys_Android_jclass {
public:
    _ZFP_ZFImpl_sys_Android_jclass(
            ZF_IN const zfchar *funcName
            , ZF_IN const zfchar *className
            ) {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        cls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(className).c_str());
        ZFCoreAssertWithMessage(cls != NULL, "no class named: %s, for jclass: %s", className, funcName);
        JNIScopeDeleteLocalRef(cls);
        cls = (jclass)JNIUtilNewGlobalRef(jniEnv, cls);
    }
    ~_ZFP_ZFImpl_sys_Android_jclass(void) {
        if(cls != NULL) {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), cls);
        }
    }
public:
    jclass cls;
};
#define ZFImpl_sys_Android_jclass_DEFINE(funcName, className) \
    jclass funcName(void) { \
        static _ZFP_ZFImpl_sys_Android_jclass d(#funcName, className); \
        return d.cls; \
    }

// ============================================================
// utils
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectToJNIType
#define JNIConvertZFObjectToJNIType(jniEnv, p) JNIConvertPointerToJNIType((jniEnv), zfcast(ZFObject *, (p)))
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectFromJNIType
#define JNIConvertZFObjectFromJNIType(jniEnv, d) zfany((ZFObject *)JNIConvertPointerFromJNIType((jniEnv), (d)))
#endif

// ============================================================
extern ZFLIB_ZF_impl void ZFImpl_sys_Android_zfstringFromStringT(
        ZF_IN_OUT zfstring &s
        , ZF_IN jobject jstr
        );
inline zfstring ZFImpl_sys_Android_zfstringFromString(ZF_IN jobject jstr) {
    zfstring ret;
    ZFImpl_sys_Android_zfstringFromStringT(ret, jstr);
    return ret;
}
// note returned object must be deleted by DeleteLocalRef
extern ZFLIB_ZF_impl jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s);

// ============================================================
extern ZFLIB_ZF_impl zfstring ZFImpl_sys_Android_stackTrace(void);
extern ZFLIB_ZF_impl void ZFImpl_sys_Android_stackTracePrint(void);
extern ZFLIB_ZF_impl void ZFImpl_sys_Android_objectInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN jobject nativeObject
        );
inline zfstring ZFImpl_sys_Android_objectInfo(ZF_IN jobject nativeObject) {
    zfstring ret;
    ZFImpl_sys_Android_objectInfoT(ret, nativeObject);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_def_h_

