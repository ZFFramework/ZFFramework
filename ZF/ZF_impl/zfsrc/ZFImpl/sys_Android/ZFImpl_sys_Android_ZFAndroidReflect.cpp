#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

#define ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidReflect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidReflect)

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlmap<const zfchar *, const ZFClass *, zfcharConst_zfstlComparer> _ZFP_ZFAndroidReflect_ClassMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAndroidReflectDataHolder, ZFLevelZFFrameworkStatic) {
}
public:
    _ZFP_ZFAndroidReflect_ClassMapType clsMap; // <clsNameInJava, cls>
public:
    const ZFClass *clsForNameInJava(ZF_IN const zfchar *clsNameInJava) {
        _ZFP_ZFAndroidReflect_ClassMapType::iterator it = this->clsMap.find(clsNameInJava);
        if(it != this->clsMap.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFAndroidReflectDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAndroidReflectAutoClean, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFAndroidReflectAutoClean) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAndroidReflectDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder);
    if(!d->clsMap.empty()) {
        ZFCoreArrayPOD<const ZFClass *> tmp;
        for(_ZFP_ZFAndroidReflect_ClassMapType::iterator it = d->clsMap.begin(); it != d->clsMap.end(); ++it) {
            tmp.add(it->second);
        }
        for(zfindex i = 0; i < tmp.count(); ++i) {
            ZFClassDynamicUnregister(tmp[i]);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFAndroidReflectAutoClean)

// ============================================================
zfclass _ZFP_ZFAndroidReflect_ClassInfo : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAndroidReflect_ClassInfo, ZFObject)
public:
    ZFSigName clsNameInJava;
    JNIGlobalRef jcls;
protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        if(!this->clsNameInJava.isEmpty()) {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder)->clsMap.erase(this->clsNameInJava);
        }
        zfsuper::objectOnDealloc();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFAndroidReflect_ClassInfo)

// ============================================================
zfclass _ZFP_ZFAndroidReflect_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFAndroidReflect_Object, ZFObject)
public:
    JNIGlobalRef jobj;
protected:
    zfoverride
    virtual void objectOnInitFinish(void) {
        zfsuper::objectOnInitFinish();
        // constructor with params would be registered as dynamic objectOnInit
        if(this->jobj) {
            return;
        }
        _ZFP_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_ZFAndroidReflect_ClassInfo *, this->classData()->classDynamicRegisterUserData());
        if(clsInfo == zfnull) {
            return;
        }
        // default constructor
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jmethodID jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, "<init>"
                , JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()).c_str()
                );
        jobject jobj = JNIUtilNewObject(jniEnv, clsInfo->jcls, jmId);
        this->jobj = jobj;
        JNIUtilDeleteLocalRef(jniEnv, jobj);
        zfCoreAssertWithMessageTrim(this->jobj, "unable to create new instance of Java class \"%s\" for class \"%s\""
                , clsInfo->clsNameInJava
                , this->classData()->classNameFull()
                );
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFAndroidReflect_Object)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , JNIPointer, native_1registerClass
        , jclass cls
        , jstring clsNameInJava
        , jstring clsNameInZF
        ) {
    zfstring clsName;
    ZFImpl_sys_Android_zfstringFromString(clsName, clsNameInZF);
    zfstring errorHint;
    zfblockedAlloc(_ZFP_ZFAndroidReflect_ClassInfo, clsHolder);
    clsHolder->jcls = cls;
    const ZFClass *clsZF = ZFClassDynamicRegister(
            clsName
            , _ZFP_ZFAndroidReflect_Object::ClassData()
            , clsHolder
            , &errorHint
            );
    if(clsZF == zfnull) {
        zfCoreLog("unable to register class \"%s\": %s", clsName, errorHint);
        return JNIPointerInvalid;
    }

    clsHolder->clsNameInJava = ZFImpl_sys_Android_zfstringFromString(clsNameInJava);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder)->clsMap[clsHolder->clsNameInJava] = clsZF;
    return JNIConvertPointerToJNIType(JNIGetJNIEnv(), clsZF);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , void, native_1unregisterClass
        , jstring clsNameInJava
        ) {
    zfstring clsName;
    ZFImpl_sys_Android_zfstringFromString(clsName, clsNameInJava);
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAndroidReflectDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder);
    _ZFP_ZFAndroidReflect_ClassMapType::iterator it = d->clsMap.find(clsName);
    if(it != d->clsMap.end()) {
        ZFClassDynamicUnregister(it->second);
    }
}
JNI_METHOD_DECLARE_END()

static const zfstlmap<zfstring, JNIType::Type> &_ZFP_ZFAndroidReflect_TypeMap(void) {
    static zfstlmap<zfstring, JNIType::Type> m;
    if(m.empty()) {
        m["boolean"] = JNIType::T_boolean;
        m["byte"] = JNIType::T_byte;
        m["char"] = JNIType::T_char;
        m["short"] = JNIType::T_short;
        m["int"] = JNIType::T_int;
        m["long"] = JNIType::T_long;
        m["float"] = JNIType::T_float;
        m["double"] = JNIType::T_double;
        m["void"] = JNIType::T_void;
    }
    return m;
}

static zfbool _ZFP_ZFAndroidReflect_typeCheck(ZF_OUT JNIType &jniType, ZF_OUT const zfchar *&typeIdZF, ZF_IN_OUT zfstring &typeName) {
    zfint arrayCount = 0;
    while(typeName.length() > 2 && zfstringIsEqual(typeName.cString() + typeName.length() - 2, "[]")) {
        ++arrayCount;
        typeName.remove(typeName.length() - 2);
    }

    const zfstlmap<zfstring, JNIType::Type> &typeMap = _ZFP_ZFAndroidReflect_TypeMap();
    zfstlmap<zfstring, JNIType::Type>::const_iterator it = typeMap.find(typeName);
    if(it != typeMap.end()) {
        jniType.setType(it->second);
        switch(it->second) {
            case JNIType::T_boolean:
                typeIdZF = ZFTypeId_zfbool();
                break;
            case JNIType::T_byte:
                typeIdZF = ZFTypeId_zfbyte();
                break;
            case JNIType::T_char:
                typeIdZF = ZFTypeId_zfchar();
                break;
            case JNIType::T_short:
                typeIdZF = ZFTypeId_zfint();
                break;
            case JNIType::T_int:
                typeIdZF = ZFTypeId_zfint();
                break;
            case JNIType::T_long:
                typeIdZF = ZFTypeId_zftimet();
                break;
            case JNIType::T_float:
                typeIdZF = ZFTypeId_zffloat();
                break;
            case JNIType::T_double:
                typeIdZF = ZFTypeId_zfdouble();
                break;
            case JNIType::T_void:
                typeIdZF = ZFTypeId_void();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    else {
        jniType.setType(JNIType::T_object, typeName);

        if(arrayCount == 0) {
            const ZFClass *cls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder)->clsForNameInJava(typeName);
            if (cls != zfnull) {
                typeIdZF = cls->classNameFull();
            } else {
                typeIdZF = _ZFP_ZFAndroidReflect_Object::ClassData()->classNameFull();
            }
        }
        else {
            typeIdZF = _ZFP_ZFAndroidReflect_Object::ClassData()->classNameFull();
        }
    }

    while(arrayCount > 0) {
        --arrayCount;
        jniType.setType(JNIType::T_array, jniType.getId());
    }

    return zftrue;
}

static ZFCoreArrayPOD<jvalue> _ZFP_ZFAndroidReflect_paramConvert(
        ZF_IN ZFMethodInvokeData *ivk
        , ZF_IN ZFCoreArray<JNIType> const &paramJNITypes
        ) {
    ZFCoreArrayPOD<jvalue> params;
    for(zfindex i = 0; i < paramJNITypes.count(); ++i) {
        const JNIType &paramJNIType = paramJNITypes[i];
        switch(paramJNIType.getType()) {
            case JNIType::T_boolean: {
                v_zfbool *p = ivk->paramAt(i);
                jvalue t;
                t.z = (jboolean)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_byte: {
                v_zfbyte *p = ivk->paramAt(i);
                jvalue t;
                t.b = (jbyte)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_char: {
                v_zfchar *p = ivk->paramAt(i);
                jvalue t;
                t.c = (jchar)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_short: {
                v_zfint *p = ivk->paramAt(i);
                jvalue t;
                t.s = (jshort)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_int: {
                v_zfint *p = ivk->paramAt(i);
                jvalue t;
                t.i = (jint)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_long: {
                v_zftimet *p = ivk->paramAt(i);
                jvalue t;
                t.j = (jlong)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_float: {
                v_zffloat *p = ivk->paramAt(i);
                jvalue t;
                t.f = (jfloat)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_double: {
                v_zfdouble *p = ivk->paramAt(i);
                jvalue t;
                t.d = (jdouble)p->zfv;
                params.add(t);
                break;
            }
            case JNIType::T_void:
                zfCoreCriticalShouldNotGoHere();
                break;
            case JNIType::T_object: {
                _ZFP_ZFAndroidReflect_Object *p = ivk->paramAt(i);
                jvalue t;
                t.l = p->jobj;
                params.add(t);
                break;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }
    return params;
}

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , void, native_1registerConstructor
        , JNIPointer zfjniPointerCls
        , jobjectArray paramTypeNames
        ) {
    const ZFClass *cls = (const ZFClass *)JNIConvertPointerFromJNIType(JNIGetJNIEnv(), zfjniPointerCls);
    _ZFP_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_ZFAndroidReflect_ClassInfo *, cls->classDynamicRegisterUserData());
    zfCoreAssert(clsInfo != zfnull);

    ZFMP mpZF;
    ZFCoreArray<JNIType> paramJNITypes;
    if(paramTypeNames != NULL) {
        jsize paramCount = JNIUtilGetArrayLength(jniEnv, paramTypeNames);
        zfstring paramTypeName;
        JNIType paramJNIType;
        const zfchar *typeIdZF;
        for(jsize i = 0; i < paramCount; ++i) {
            paramTypeName.removeAll();
            ZFImpl_sys_Android_zfstringFromString(paramTypeName, JNIUtilGetObjectArrayElement(jniEnv, paramTypeNames, i));
            if(!_ZFP_ZFAndroidReflect_typeCheck(paramJNIType, typeIdZF, paramTypeName)) {
                zfCoreCriticalMessage("unknown type name: %s", paramTypeName);
            }
            mpZF.mp(typeIdZF);
            paramJNITypes.add(paramJNIType);
        }
    }
    jmethodID jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, "<init>"
            , JNIGetMethodSig(JNIType::S_void(), paramJNITypes.arrayBuf(), (jsize)paramJNITypes.count()).c_str()
            );
    zfCoreAssertWithMessageTrim(jmId != NULL, "unable to find Java constructor with sig: %s"
            , JNIGetMethodSig(JNIType::S_void(), paramJNITypes.arrayBuf(), (jsize)paramJNITypes.count()).c_str()
            );

    ZFLISTENER_3(methodImpl
            , zfweakT<_ZFP_ZFAndroidReflect_ClassInfo *>, clsInfo
            , jmethodID, jmId
            , ZFCoreArray<JNIType>, paramJNITypes
            ) {
        ZFMethodInvokeData *ivk = zfargs.param0();
        _ZFP_ZFAndroidReflect_Object *obj = ZFCastZFObject(_ZFP_ZFAndroidReflect_Object *, ivk->invokerObject);
        JNIEnv *jniEnv = JNIGetJNIEnv();
        ZFCoreArrayPOD<jvalue> params = _ZFP_ZFAndroidReflect_paramConvert(ivk, paramJNITypes);
        jobject jobj = JNIUtilNewObjectA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf());
        zfCoreAssert(jobj != NULL);
        obj->jobj = jobj;
        JNIUtilDeleteLocalRef(jniEnv, jobj);
    } ZFLISTENER_END()
    ZFObjectOnInitDynamicRegister(cls, mpZF, methodImpl);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , void, native_1registerMethod
        , JNIPointer zfjniPointerCls
        , jstring methodName
        , jboolean isStatic
        , jstring returnTypeName
        , jobjectArray paramTypeNames
        ) {
    const ZFClass *cls = (const ZFClass *)JNIConvertPointerFromJNIType(JNIGetJNIEnv(), zfjniPointerCls);
    _ZFP_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_ZFAndroidReflect_ClassInfo *, cls->classDynamicRegisterUserData());
    zfCoreAssert(clsInfo != zfnull);

    zfstring methodNameZF;
    ZFImpl_sys_Android_zfstringFromString(methodNameZF, methodName);

    // return type
    JNIType returnJNIType;
    const zfchar *returnTypeIdZF = zfnull;
    {
        zfstring returnTypeNameZF;
        ZFImpl_sys_Android_zfstringFromString(returnTypeNameZF, returnTypeName);
        if(!_ZFP_ZFAndroidReflect_typeCheck(returnJNIType, returnTypeIdZF, returnTypeNameZF)) {
            zfCoreCriticalMessage("unknown type name: %s", returnTypeNameZF);
        }
    }

    // param type
    ZFMP mpZF;
    ZFCoreArray<JNIType> paramJNITypes;
    if(paramTypeNames != NULL) {
        jsize paramCount = JNIUtilGetArrayLength(jniEnv, paramTypeNames);
        zfstring paramTypeName;
        JNIType paramJNIType;
        const zfchar *typeIdZF;
        for(jsize i = 0; i < paramCount; ++i) {
            paramTypeName.removeAll();
            ZFImpl_sys_Android_zfstringFromString(paramTypeName, JNIUtilGetObjectArrayElement(jniEnv, paramTypeNames, i));
            if(!_ZFP_ZFAndroidReflect_typeCheck(paramJNIType, typeIdZF, paramTypeName)) {
                zfCoreCriticalMessage("unknown type name: %s", paramTypeName);
            }
            mpZF.mp(typeIdZF);
            paramJNITypes.add(paramJNIType);
        }
    }
    jmethodID jmId = NULL;
    if(isStatic) {
        jmId = JNIUtilGetStaticMethodID(jniEnv, clsInfo->jcls, methodNameZF
                , JNIGetMethodSig(returnJNIType, paramJNITypes.arrayBuf(), (jsize)paramJNITypes.count()).c_str()
                );
    }
    else {
        jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, methodNameZF
                , JNIGetMethodSig(returnJNIType, paramJNITypes.arrayBuf(), (jsize)paramJNITypes.count()).c_str()
                );
    }
    zfCoreAssert(jmId != NULL);

    ZFLISTENER_5(methodImpl
            , zfweakT<_ZFP_ZFAndroidReflect_ClassInfo *>, clsInfo
            , jboolean, isStatic
            , jmethodID, jmId
            , JNIType, returnJNIType
            , ZFCoreArray<JNIType>, paramJNITypes
            ) {
        ZFMethodInvokeData *ivk = zfargs.param0();
        _ZFP_ZFAndroidReflect_Object *obj = ZFCastZFObject(_ZFP_ZFAndroidReflect_Object *, ivk->invokerObject);
        JNIEnv *jniEnv = JNIGetJNIEnv();
        ZFCoreArrayPOD<jvalue> params = _ZFP_ZFAndroidReflect_paramConvert(ivk, paramJNITypes);
        switch(returnJNIType.getType()) {
            case JNIType::T_boolean: {
                jboolean ret = isStatic
                    ? JNIUtilCallStaticBooleanMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallBooleanMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfbool, (zfbool)ret);
                break;
            }
            case JNIType::T_byte: {
                jbyte ret = isStatic
                    ? JNIUtilCallStaticByteMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallByteMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfbyte, (zfbyte)ret);
                break;
            }
            case JNIType::T_char: {
                jchar ret = isStatic
                    ? JNIUtilCallStaticCharMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallCharMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfchar, (zfchar)ret);
                break;
            }
            case JNIType::T_short: {
                jshort ret = isStatic
                    ? JNIUtilCallStaticShortMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallShortMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfint, (zfint)ret);
                break;
            }
            case JNIType::T_int: {
                jint ret = isStatic
                    ? JNIUtilCallStaticIntMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallIntMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfint, (zfint)ret);
                break;
            }
            case JNIType::T_long: {
                jlong ret = isStatic
                    ? JNIUtilCallStaticLongMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallLongMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zftimet, (zftimet)ret);
                break;
            }
            case JNIType::T_float: {
                jfloat ret = isStatic
                    ? JNIUtilCallStaticFloatMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallFloatMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zffloat, (zffloat)ret);
                break;
            }
            case JNIType::T_double: {
                jdouble ret = isStatic
                    ? JNIUtilCallStaticDoubleMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallDoubleMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                ivk->ret = zflineAlloc(v_zfdouble, (zfdouble)ret);
                break;
            }
            case JNIType::T_void: {
                if(isStatic) {
                    JNIUtilCallStaticVoidMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf());
                }
                else {
                    JNIUtilCallVoidMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf());
                }
                break;
            }
            case JNIType::T_array:
            case JNIType::T_object: {
                jobject ret = isStatic
                    ? JNIUtilCallStaticObjectMethodA(jniEnv, clsInfo->jcls, jmId, params.arrayBuf())
                    : JNIUtilCallObjectMethodA(jniEnv, obj->jobj, jmId, params.arrayBuf())
                    ;
                zfblockedAlloc(_ZFP_ZFAndroidReflect_Object, wrapper);
                wrapper->jobj = ret;
                ivk->ret = wrapper;
                JNIUtilDeleteLocalRef(jniEnv, ret);
                break;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    } ZFLISTENER_END()
    ZFMethodDynamicRegister(cls, returnTypeIdZF, methodNameZF, mpZF, methodImpl);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

