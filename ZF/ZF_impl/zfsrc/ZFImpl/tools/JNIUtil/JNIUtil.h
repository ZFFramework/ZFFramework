/**
 * @file JNIUtil.h
 * @brief JNI utilities
 */

#ifndef _JNIUtil_h_
#define _JNIUtil_h_

// ============================================================
/** @brief used to export symbols */
#ifndef _JNI_EXPORT
    #ifdef _WIN32
        #define _JNI_EXPORT __declspec(dllimport)
    #else
        #define _JNI_EXPORT __attribute__((visibility("default")))
    #endif
#endif

// ============================================================
#ifndef NEED_JNIUTIL
    #if (defined(ANDROID) || defined(__ANDROID__) || defined(JNI_VERSION_1_1) || defined(JNIEXPORT) || defined(JNICALL))
        /**
         * @brief macro to force enable JNIUtil
         */
        #define NEED_JNIUTIL 1
    #endif
#endif

#if NEED_JNIUTIL
#include <cstdlib>
#include <jni.h>

#if defined(__cplusplus) || defined(c_plusplus)
    #define _JNI_EXTERN_C extern "C"
#else
    #define _JNI_EXTERN_C
#endif

namespace JNIUtil {

// ============================================================
// wrapper
/**
 * @brief workaround to prevent export stl containers
 */
class _JNI_EXPORT JNIString {
public:
    JNIString(void);
    JNIString(JNIString const &ref);
    JNIString(const char *s);
    ~JNIString(void);
    JNIString &operator = (JNIString const &ref);
    JNIString &operator = (const char *s);
    const char *c_str(void) const;
    jsize length(void) const;
    operator const char * (void) const;
    JNIString &operator += (char c);
    JNIString &operator += (const char *s);
    void clear(void);
public:
    void *_d;
};

class JNIType;
/**
 * @brief wrapper for JNIGetMethodSig
 */
class _JNI_EXPORT JNIParamTypeContainer {
public:
    JNIParamTypeContainer(void);
    ~JNIParamTypeContainer(void);
private:
    JNIParamTypeContainer(const JNIParamTypeContainer &ref);
    JNIParamTypeContainer &operator = (const JNIParamTypeContainer &ref);
public:
    /** @brief see #JNIGetMethodSig */
    JNIParamTypeContainer &add(const JNIType &paramType);
public:
    /** @brief see #JNIGetMethodSig */
    void remove(jint i);
    /** @brief see #JNIGetMethodSig */
    void removeAll(void);
public:
    /** @brief see #JNIGetMethodSig */
    jsize count(void) const;
    /** @brief see #JNIGetMethodSig */
    const JNIType &get(jint i) const;
public:
    void *_d;
};

// ============================================================
// common
/**
 * @brief init jni jniEnv, must be called in your JNI_OnLoad
 */
extern _JNI_EXPORT bool JNIInit(JavaVM *javaVM, jint version = JNI_VERSION_1_1);

/**
 * @brief callback for custom impl
 */
typedef JNIEnv *(*JNIGetJNIEnvCallback)(void);
/**
 * @brief init jni jniEnv, must be called in your JNI_OnLoad
 */
extern _JNI_EXPORT bool JNIInit(JNIGetJNIEnvCallback callback);

/**
 * @brief global java vm
 */
extern _JNI_EXPORT JavaVM *JNIGetJavaVM(void);

/**
 * @brief get desired version passed from JNIInit, or -1 if init failed
 */
extern _JNI_EXPORT jint JNIGetDesiredVersion(void);

/**
 * @brief get JNIEnv for current thread
 */
extern _JNI_EXPORT JNIEnv *JNIGetJNIEnv(void);

/**
 * @brief convert class name to JNI class sig
 *
 * for example, convert "com.example.OuterClass$InnerClass"
 * to "Lcom/example/OuterClass$InnerClass;"\n
 * class name can be get by SomeClass.class.getName() in your jave code
 */
extern _JNI_EXPORT void JNIConvertClassNameToClassSig(JNIString &ret, const char *className);
/** @brief see #JNIConvertClassNameToClassSig */
inline JNIString JNIConvertClassNameToClassSig(const char *className) {
    JNIString ret;
    JNIConvertClassNameToClassSig(ret, className);
    return ret;
}

/**
 * @brief convert JNI class sig to class name
 *
 * for example, convert "Lcom/example/OuterClass$InnerClass;"
 * to "com.example.OuterClass$InnerClass"
 */
extern _JNI_EXPORT void JNIConvertClassNameFromClassSig(JNIString &ret, const char *classSig);
/** @brief see #JNIConvertClassNameFromClassSig */
inline JNIString JNIConvertClassNameFromClassSig(const char *className) {
    JNIString ret;
    JNIConvertClassNameFromClassSig(ret, className);
    return ret;
}

/**
 * @brief convert class name for FindClass in JNI
 *
 * for example, convert "com.example.OuterClass$InnerClass"
 * to "com/example/OuterClass$InnerClass"\n
 * class name can be get by SomeClass.class.getName() in your jave code
 */
extern _JNI_EXPORT void JNIConvertClassNameForFindClass(JNIString &ret, const char *className);
/** @brief see #JNIConvertClassNameForFindClass */
inline JNIString JNIConvertClassNameForFindClass(const char *className) {
    JNIString ret;
    JNIConvertClassNameForFindClass(ret, className);
    return ret;
}

/**
 * @brief constructor method name
 */
#define JNIConstructorName "<init>"

// ============================================================
// JNI types
/**
 * @brief JNI type utility
 */
class _JNI_EXPORT JNIType {
public:
    typedef enum {
        T_boolean, /**< jboolean */
        T_byte, /**< jbyte */
        T_char, /**< jchar */
        T_short, /**< jshort */
        T_int, /**< jint */
        T_long, /**< jlong */
        T_float, /**< jfloat */
        T_double, /**< jdouble */
        T_void, /**< void */
        T_object, /**< jobect */
        T_array, /**< jarray */
    } Type;

public:
    /**
     * @brief main constructor with jboolean type
     */
    JNIType(void);
    /**
     * @brief construct from type and classNameOrArrayElementTypeId (if type is object or array)
     *
     * classNameOrArrayElementTypeId is only valid if type is object and array\n
     * if object, it means the class name of the object
     * such as "com.example.OuterClass$InnerClass"\n
     * if array, it means the type id which can be accessed by another #JNIType::getId
     */
    JNIType(JNIType::Type type,
            const char *classNameOrArrayElementTypeId = NULL);
    JNIType(const JNIType &ref);
    JNIType &operator = (const JNIType &ref);

public:
    /**
     * @brief set JNI type
     */
    void setType(JNIType::Type type,
                 const char *classNameOrArrayElementTypeId = NULL);
    /**
     * @brief see #setType
     */
    JNIType::Type getType(void) const;

public:
    /**
     * @brief get type id (e.g. "Z" for jboolean, "V" for void)
     *
     * note: if you set a wrong info about the JNI type
     * (such as a object type with a wrong class name),
     * the returned id may be wrong without any warnings
     */
    const char *getId(void) const;

public:
    /** @brief a static instance for the T_boolean type */
    static const JNIType &S_boolean(void);
    /** @brief a static instance for the T_byte type */
    static const JNIType &S_byte(void);
    /** @brief a static instance for the T_char type */
    static const JNIType &S_char(void);
    /** @brief a static instance for the T_short type */
    static const JNIType &S_short(void);
    /** @brief a static instance for the T_int type */
    static const JNIType &S_int(void);
    /** @brief a static instance for the T_long type */
    static const JNIType &S_long(void);
    /** @brief a static instance for the T_float type */
    static const JNIType &S_float(void);
    /** @brief a static instance for the T_double type */
    static const JNIType &S_double(void);
    /** @brief a static instance for the T_void type */
    static const JNIType &S_void(void);

public:
    /**
     * @brief util method to create object type
     */
    static inline JNIType S_object(const char *className) {
        return JNIType(JNIType::T_object, className);
    }
    /**
     * @brief util method to create array type
     */
    static inline JNIType S_array(const JNIType &type) {
        return JNIType(JNIType::T_array, type.getId());
    }

public:
    /** @brief a static instance for the Class type */
    static const JNIType &S_object_Class(void);
    /** @brief a static instance for the Object type */
    static const JNIType &S_object_Object(void);
    /** @brief a static instance for the String type */
    static const JNIType &S_object_String(void);
public:
    /** @brief a static instance for the Class type */
    static const JNIType &S_array_Class(void);
    /** @brief a static instance for the Object type */
    static const JNIType &S_array_Object(void);
    /** @brief a static instance for the String type */
    static const JNIType &S_array_String(void);

private:
    JNIType::Type _type;
    JNIString *_id;
};

// ============================================================
// signature utilities
/**
 * @brief get method signature, like "(Z)I" for "int func(bool)"
 *
 * you may use JNIParamTypeContainer to create a paramTypeList easily:
 * @code
 *   JNIGetMethodSig(
 *       JNIType::S_boolean(),
 *       JNIParamTypeContainer()
 *           .add(JNIType::S_byte())
 *           .add(JNIType::T_boolean)
 *           .add(JNIType(JNIType::T_object, "java.lang.String"))
 *       );
 * @endcode
 */
extern _JNI_EXPORT JNIString JNIGetMethodSig(
        const JNIType &returnType
        , const JNIParamTypeContainer &paramTypeList
        );
/** @brief see #JNIGetMethodSig */
extern _JNI_EXPORT JNIString JNIGetMethodSig(
        const JNIType &returnType
        , const JNIType *paramTypeList
        , jsize paramCount
        );

/** @cond ZFPrivateDoc */
#define _JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ...) \
    _JNI_EXTERN_C JNIEXPORT ReturnType JNICALL Java_##OwnerClassId##_##MethodName( \
        JNIEnv *jniEnv, jclass jniCls, ##__VA_ARGS__)
/** @endcond */
/**
 * @brief macro to declare a JNI method
 *
 * usage:
 * @code
 *   JNI_METHOD_DECLARE_BEGIN(OwnerClassId
 *           , ReturnType, MethodName
 *           , Param0 param0
 *           , Param1 param1
 *           ) {
 *       // (JNIEnv *)jniEnv and (jclass)jniCls are predefined
 *   }
 *   JNI_METHOD_DECLARE_END()
 * @endcode
 *
 * class name should be like this:\n
 *   com_package_ClassName\n
 * if class name or method name has '_',
 * you should replace it with "_1" instead of '_'\n
 * if class name has inner class,
 * you should separate the inner class by "_00024" instead of '$',
 * such as "com_package_OuterClass_00024InnerClass"\n
 * \n
 * then, the final name would be like this:
 *   com_some_1package_OuterClass_00024InnerClass_method_1name\n
 * which stands for "com.some_package.OuterClass$InnerClass"'s
 * method named "method_name"
 */
#define JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ...) \
    _JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ##__VA_ARGS__)
/** @brief see #JNI_METHOD_DECLARE_BEGIN */
#define JNI_METHOD_DECLARE_END()

/**
 * @brief macro that declare JNI_OnLoad
 */
#define JNI_ONLOAD_ENTRY(paramJavaVM, paramReserved) \
    _JNI_EXTERN_C JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* paramJavaVM, void* paramReserved)

// ============================================================
// pointer converter
/**
 * @brief define to true to use long as #JNIPointer type
 */
#ifndef JNIPointerUseLong
    #define JNIPointerUseLong 1
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointer
    #if JNIPointerUseLong
        #define JNIPointer jlong
    #else
        #define JNIPointer jbyteArray
    #endif
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointerInvalid
    #if JNIPointerUseLong
        #define JNIPointerInvalid 0
    #else
        #define JNIPointerInvalid NULL
    #endif
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointerJNIType
    #if JNIPointerUseLong
        #define JNIPointerJNIType JNIType::S_long()
    #else
        #define JNIPointerJNIType JNIType::S_array(JNIType::S_byte())
    #endif
#endif

jbyteArray _JNIConvertPointerToJNITypeAction(JNIEnv *jniEnv, void *p);
void *_JNIConvertPointerFromJNITypeAction(JNIEnv *jniEnv, jbyteArray d);

#if JNIPointerUseLong
    #define _JNIConvertPointerToJNIType(jniEnv, p) (JNIPointer)(p)
    #define _JNIConvertPointerFromJNIType(jniEnv, p) (void *)(p)
#else
    #define _JNIConvertPointerToJNIType(jniEnv, p) _JNIConvertPointerToJNITypeAction(jniEnv, p)
    #define _JNIConvertPointerFromJNIType(jniEnv, d) _JNIConvertPointerFromJNITypeAction(jniEnv, d)
#endif

/**
 * @brief safely convert a C pointer type to JNI type
 *
 * @see JNIConvertPointerFromJNIType
 * @note this method only ensure safe to convert as raw pointer type,
 *   you should take care when processing pointers with inheritance,
 *   it's recommended to convert to base class' pointer first,
 *   and always use base class's pointer when access
 */
#ifndef JNIConvertPointerToJNIType
    #define JNIConvertPointerToJNIType(jniEnv, p) _JNIConvertPointerToJNIType(jniEnv, p)
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertPointerFromJNIType
    #define JNIConvertPointerFromJNIType(jniEnv, d) _JNIConvertPointerFromJNIType(jniEnv, d)
#endif

// ============================================================
// local/global ref cleaner
/** @cond ZFPrivateDoc */
namespace JNIUtilPrivate {
    class _JNI_EXPORT LocalRefDel {
    public:
        LocalRefDel(jobject obj)
        : objSaved(obj)
        {
        }
        ~LocalRefDel(void) {
            if(this->objSaved) {
                JNIGetJNIEnv()->DeleteLocalRef(this->objSaved);
            }
        }
    public:
        jobject objSaved;
    };
    class _JNI_EXPORT GlobalRefDel {
    public:
        GlobalRefDel(jobject obj)
        : objSaved(obj)
        {
        }
        ~GlobalRefDel(void) {
            if(this->objSaved) {
                JNIGetJNIEnv()->DeleteGlobalRef(this->objSaved);
            }
        }
    public:
        jobject objSaved;
    };
    #define _JNIUtil_uniqueName(name) _JNIUtil_uniqueName2(name, __LINE__)
    #define _JNIUtil_uniqueName2(name, line) _JNIUtil_uniqueName3(name, line)
    #define _JNIUtil_uniqueName3(name, line) _JNIUtil_##name##_##line
} // namespace JNIUtilPrivate
/** @endcond */

/** @brief util macro to delete object after end line for JNI local object */
#define JNILineDeleteLocalRef(obj_) \
    (JNIUtilPrivate::LocalRefDel(obj_).objSaved)
/** @brief util macro to delete object after end line for JNI global object */
#define JNILineDeleteGlobalRef(obj_) \
    (JNIUtilPrivate::GlobalRefDel(obj_).objSaved)

/** @brief util macro to delete object after code block for JNI local object */
#define JNIBlockedDeleteLocalRef(obj_) \
    JNIUtilPrivate::LocalRefDel _JNIUtil_uniqueName(jniRef)(obj_)
/** @brief util macro to delete object after code block for JNI global object */
#define JNIBlockedDeleteGlobalRef(obj_) \
    JNIUtilPrivate::GlobalRefDel _JNIUtil_uniqueName(jniRef)(obj_)

} // namespace JNIUtil
#endif // #if NEED_JNIUTIL

#endif // #ifndef _JNIUtil_h_

