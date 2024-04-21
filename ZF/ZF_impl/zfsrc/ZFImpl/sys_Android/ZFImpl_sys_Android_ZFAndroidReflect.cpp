#include "ZFImpl_sys_Android_ZFAndroidReflect.h"

#if ZF_ENV_sys_Android

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

#define ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidReflect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidReflect)

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFCoreArray<_ZFP_ZFAndroidReflect_ToJNITypeCallback> &_ZFP_ZFAndroidReflect_ToJNITypeCallbackList(void) {
    static ZFCoreArray<_ZFP_ZFAndroidReflect_ToJNITypeCallback> d;
    return d;
}
static ZFCoreArray<_ZFP_ZFAndroidReflect_FromJNITypeCallback> &_ZFP_ZFAndroidReflect_FromJNITypeCallbackList(void) {
    static ZFCoreArray<_ZFP_ZFAndroidReflect_FromJNITypeCallback> d;
    return d;
}

void _ZFP_ZFAndroidReflect_ToJNITypeRegister(
        ZF_IN _ZFP_ZFAndroidReflect_ToJNITypeCallback callback
        ) {
    _ZFP_ZFAndroidReflect_ToJNITypeCallbackList().add(callback);
}
void _ZFP_ZFAndroidReflect_ToJNITypeUnregister(
        ZF_IN _ZFP_ZFAndroidReflect_ToJNITypeCallback callback
        ) {
    _ZFP_ZFAndroidReflect_ToJNITypeCallbackList().removeElement(callback);
}
void _ZFP_ZFAndroidReflect_FromJNITypeRegister(
        ZF_IN _ZFP_ZFAndroidReflect_FromJNITypeCallback callback
        ) {
    _ZFP_ZFAndroidReflect_FromJNITypeCallbackList().add(callback);
}
void _ZFP_ZFAndroidReflect_FromJNITypeUnregister(
        ZF_IN _ZFP_ZFAndroidReflect_FromJNITypeCallback callback
        ) {
    _ZFP_ZFAndroidReflect_FromJNITypeCallbackList().removeElement(callback);
}

// ============================================================
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
    this->jclsZFAndroidReflect = JNIUtilFindClass(JNIGetJNIEnv(), JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect).c_str());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFAndroidReflectAutoClean) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFAndroidReflectDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder);
    if(!d->clsMap.empty()) {
        ZFCoreArray<const ZFClass *> tmp;
        for(_ZFP_ZFAndroidReflect_ClassMapType::iterator it = d->clsMap.begin(); it != d->clsMap.end(); ++it) {
            tmp.add(it->second);
        }
        for(zfindex i = 0; i < tmp.count(); ++i) {
            ZFClassDynamicUnregister(tmp[i]);
        }
    }
}
public:
    JNIGlobalRef jclsZFAndroidReflect;
ZF_GLOBAL_INITIALIZER_END(ZFAndroidReflectAutoClean)

// ============================================================
zfclass _ZFP_I_ZFAndroidReflect_ClassInfo : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidReflect_ClassInfo, ZFObject)
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
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidReflect_ClassInfo)

// ============================================================
zfclass _ZFP_I_ZFAndroidReflect_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidReflect_Object, ZFObject)
public:
    zfbool autoAlloc;
    JNIGlobalRef jobj;
protected:
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(void *, jobj))
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->autoAlloc = zftrue;
    }
    zfoverride
    virtual void objectOnInitFinish(void) {
        zfsuper::objectOnInitFinish();
        // constructor with params would be registered as dynamic objectOnInit
        if(this->jobj || !this->autoAlloc) {
            return;
        }
        _ZFP_I_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_I_ZFAndroidReflect_ClassInfo *, this->classData()->classDynamicRegisterUserData());
        if(clsInfo == zfnull) {
            return;
        }
        // default constructor
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jmethodID jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, "<init>"
                , JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()).c_str()
                );
        if(jmId != zfnull) {
            jobject jobj = JNIUtilNewObject(jniEnv, clsInfo->jcls, jmId);
            this->jobj = jobj;
            JNIUtilDeleteLocalRef(jniEnv, jobj);
        }
        zfCoreAssertWithMessageTrim(this->jobj, "unable to create new instance of Java class \"%s\" for class \"%s\""
                , clsInfo->clsNameInJava
                , this->classData()->classNameFull()
                );
    }
protected:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) {
        if(!this->jobj) {
            ret += "JNINull";
            return;
        }
        ZFImpl_sys_Android_objectInfoT(ret, this->jobj);
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidReflect_Object)
ZFOBJECT_ON_INIT_DEFINE_1(_ZFP_I_ZFAndroidReflect_Object
        , ZFMP_IN(void *, jobj)) {
    this->objectOnInit();
    this->jobj = (jobject)jobj;
}

// ============================================================
zfclass _ZFP_I_ZFAndroidReflect_GIData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFAndroidReflect_GIData, ZFObject)
public:
    JNIGlobalRef jcls;
    zfbool isStatic;
    jmethodID jmId;
    JNIType returnJNIType;
    zfstring returnTypeNameZF;
    zfstlvector<JNIType> paramJNITypes;
    zfstlvector<JNIGlobalRef> paramJNITypeNames;
};
ZFOBJECT_REGISTER(_ZFP_I_ZFAndroidReflect_GIData)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(const ZFClass *, ZFAndroidReflect_classForNameInJava
        , ZFMP_IN(const zfchar *, clsNameInJava)
        ) {
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder)->clsForNameInJava(clsNameInJava);
}

ZFMETHOD_FUNC_DEFINE_2(const ZFClass *, ZFAndroidReflect_registerClass
        , ZFMP_IN(const zfchar *, clsNameInJava)
        , ZFMP_IN_OPT(const zfchar *, clsNameInZF, zfnull)
        ) {
    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectAutoClean)->jclsZFAndroidReflect;
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "native_registerClass",
        JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
            .add(JNIType::S_object_String())
            .add(JNIType::S_object_String())
        ).c_str());
    JNIPointer zfjniPointerCls = JNIUtilCallStaticLongMethod(jniEnv, jcls, jmId
            , ZFImpl_sys_Android_zfstringToString(clsNameInJava)
            , zfstringIsEmpty(clsNameInZF) ? NULL : ZFImpl_sys_Android_zfstringToString(clsNameInZF)
            );
    return (const ZFClass *)JNIConvertPointerFromJNIType(jniEnv, zfjniPointerCls);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFAndroidReflect_unregisterClass
        , ZFMP_IN(const zfchar *, clsNameInJava)
        ) {
    const ZFClass *cls = ZFAndroidReflect_classForNameInJava(clsNameInJava);
    if(cls == zfnull) {
        return;
    }

    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectAutoClean)->jclsZFAndroidReflect;
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "native_unregisterClass",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_object_String())
            .add(JNIPointerJNIType)
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jcls, jmId
            , ZFImpl_sys_Android_zfstringToString(clsNameInJava)
            , JNIConvertPointerToJNIType(jniEnv, cls)
            );
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFAndroidReflect_registerClassContents
        , ZFMP_IN(const zfchar *, clsNameInJava)
        ) {
    const ZFClass *cls = ZFAndroidReflect_classForNameInJava(clsNameInJava);
    if(cls == zfnull) {
        return;
    }

    jclass jcls = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectAutoClean)->jclsZFAndroidReflect;
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "native_registerClassContents",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_object_String())
            .add(JNIPointerJNIType)
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jcls, jmId
            , ZFImpl_sys_Android_zfstringToString(clsNameInJava)
            , JNIConvertPointerToJNIType(jniEnv, cls)
            );
}

ZF_NAMESPACE_GLOBAL_END

// ============================================================
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , JNIPointer, native_1registerClass
        , jclass cls
        , jstring clsNameInJava
        , jstring clsNameInZF
        ) {
    zfstring clsName;
    ZFImpl_sys_Android_zfstringFromStringT(clsName, clsNameInZF);
    zfstring errorHint;
    zfblockedAlloc(_ZFP_I_ZFAndroidReflect_ClassInfo, clsInfo);
    clsInfo->jcls = cls;
    const ZFClass *clsZF = ZFClassDynamicRegister(
            clsName
            , _ZFP_I_ZFAndroidReflect_Object::ClassData()
            , clsInfo
            , &errorHint
            );
    if(clsZF == zfnull) {
        zfCoreLog("unable to register class \"%s\": %s", clsName, errorHint);
        return JNIPointerInvalid;
    }

    clsInfo->clsNameInJava = ZFImpl_sys_Android_zfstringFromString(clsNameInJava);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectDataHolder)->clsMap[clsInfo->clsNameInJava] = clsZF;
    return JNIConvertPointerToJNIType(JNIGetJNIEnv(), clsZF);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , void, native_1unregisterClass
        , jstring clsNameInJava
        ) {
    zfstring clsName;
    ZFImpl_sys_Android_zfstringFromStringT(clsName, clsNameInJava);
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
        if(arrayCount == 0) {
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
            typeIdZF = _ZFP_I_ZFAndroidReflect_Object::ClassData()->classNameFull();
        }
    }
    else {
        jniType.setType(JNIType::T_object, typeName);

        if(arrayCount == 0) {
            const ZFClass *cls = ZFAndroidReflect_classForNameInJava(typeName);
            if (cls != zfnull) {
                typeIdZF = cls->classNameFull();
            } else {
                typeIdZF = _ZFP_I_ZFAndroidReflect_Object::ClassData()->classNameFull();
            }
        }
        else {
            typeIdZF = _ZFP_I_ZFAndroidReflect_Object::ClassData()->classNameFull();
        }
    }

    while(arrayCount > 0) {
        --arrayCount;
        jniType.setType(JNIType::T_array, jniType.getId());
    }

    return zftrue;
}

static zfbool _ZFP_ZFAndroidReflect_paramConvert(
        ZF_IN JNIEnv *jniEnv
        , ZF_OUT zfstlvector<jvalue> &params
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_IN const zfstlvector<JNIType> &paramJNITypes
        , ZF_IN const zfstlvector<JNIGlobalRef> &paramJNITypeNames
        ) {
    params.reserve(paramJNITypes.size());
    const ZFClass *expectedType = zfnull;
    for(zfstlsize iParam = 0; iParam < paramJNITypes.size(); ++iParam) {
        const JNIType &paramJNIType = paramJNITypes[iParam];
        switch(paramJNIType.getType()) {
            case JNIType::T_boolean: {
                v_zfbool *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfbool::ClassData(); break;}
                jvalue t;
                t.z = (jboolean)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_byte: {
                v_zfbyte *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfbyte::ClassData(); break;}
                jvalue t;
                t.b = (jbyte)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_char: {
                v_zfchar *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfchar::ClassData(); break;}
                jvalue t;
                t.c = (jchar)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_short: {
                v_zfint *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfint::ClassData(); break;}
                jvalue t;
                t.s = (jshort)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_int: {
                v_zfint *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfint::ClassData(); break;}
                jvalue t;
                t.i = (jint)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_long: {
                v_zftimet *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zftimet::ClassData(); break;}
                jvalue t;
                t.j = (jlong)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_float: {
                v_zffloat *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zffloat::ClassData(); break;}
                jvalue t;
                t.f = (jfloat)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_double: {
                v_zfdouble *p = paramList[iParam];
                if(p == zfnull) {expectedType = v_zfdouble::ClassData(); break;}
                jvalue t;
                t.d = (jdouble)p->zfv;
                params.push_back(t);
                break;
            }
            case JNIType::T_void:
                zfCoreCriticalShouldNotGoHere();
                break;
            case JNIType::T_object:
            case JNIType::T_array: {
                _ZFP_I_ZFAndroidReflect_Object *p = paramList[iParam];
                if(p != zfnull) {
                    jvalue t;
                    t.l = p->jobj;
                    params.push_back(t);

                    if(p->jobj) {
                        jclass jclsZFAndroidReflect = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFAndroidReflectAutoClean)->jclsZFAndroidReflect;
                        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsZFAndroidReflect, "native_typeCheck"
                                , JNIGetMethodSig(JNIType::S_boolean(), JNIParamTypeContainer()
                                    .add(JNIType::S_object_String())
                                    .add(JNIType::S_object_Object())
                                    ).c_str());
                        jboolean match = JNIUtilCallStaticBooleanMethod(jniEnv, jclsZFAndroidReflect, jmId
                                , (jobject)paramJNITypeNames[iParam]
                                , (jobject)p->jobj
                                );
                        if(!match) {
                            if(errorHint != zfnull) {
                                zfstringAppend(errorHint, "param%s type mismatch, expect: %s, got: %s"
                                        , (zfindex)iParam
                                        , ZFImpl_sys_Android_zfstringFromString(paramJNITypeNames[iParam])
                                        , ZFImpl_sys_Android_objectInfo(p->jobj)
                                        );
                            }
                            return zffalse;
                        }
                    }
                    break;
                }

                jvalue t;
                zfbool implSuccess = zffalse;
                ZFCoreArray<_ZFP_ZFAndroidReflect_ToJNITypeCallback> &impl = _ZFP_ZFAndroidReflect_ToJNITypeCallbackList();
                for(zfindex i = 0; i < impl.count(); ++i) {
                    if(impl[i](t.l, paramList[i], paramJNIType)) {
                        params.push_back(t);
                        implSuccess = zftrue;
                        break;
                    }
                }
                if(!implSuccess) {
                    if(errorHint != zfnull) {
                        zfstringAppend(errorHint, "param%s type mismatch, expect %s, got: %s"
                                , iParam
                                , paramJNIType.getId()
                                , paramList[iParam]
                                );
                    }
                    return zffalse;
                }
                break;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        if(expectedType != zfnull) {
            if(errorHint != zfnull) {
                zfstringAppend(errorHint, "param%s type mismatch, expect: %s, got: %s"
                        , iParam
                        , expectedType->className()
                        , paramList[iParam]
                        );
            }
            return zffalse;
        }
    }
    return zftrue;
}

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , void, native_1registerConstructor
        , JNIPointer zfjniPointerCls
        , jobjectArray paramTypeNames
        ) {
    const ZFClass *cls = (const ZFClass *)JNIConvertPointerFromJNIType(JNIGetJNIEnv(), zfjniPointerCls);
    _ZFP_I_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_I_ZFAndroidReflect_ClassInfo *, cls->classDynamicRegisterUserData());
    zfCoreAssert(clsInfo != zfnull);

    ZFMethodDynamicRegisterParam regParam;
    zfstlvector<JNIType> paramJNITypes;
    zfstlvector<JNIGlobalRef> paramJNITypeNames;
    if(paramTypeNames != NULL) {
        jsize paramCount = JNIUtilGetArrayLength(jniEnv, paramTypeNames);
        zfstring paramTypeName;
        JNIType paramJNIType;
        const zfchar *typeIdZF;
        for(jsize i = 0; i < paramCount; ++i) {
            paramTypeName.removeAll();
            jobject paramJNITypeName = JNIUtilGetObjectArrayElement(jniEnv, paramTypeNames, i);
            ZFImpl_sys_Android_zfstringFromStringT(paramTypeName, paramJNITypeName);
            if(!_ZFP_ZFAndroidReflect_typeCheck(paramJNIType, typeIdZF, paramTypeName)) {
                zfCoreCriticalMessage("unknown type name: %s", paramTypeName);
            }
            regParam.methodParamAdd(typeIdZF);
            paramJNITypes.push_back(paramJNIType);
            if(paramJNIType.getType() == JNIType::T_object || paramJNIType.getType() == JNIType::T_array) {
                paramJNITypeNames.push_back(paramJNITypeName);
            }
            else {
                paramJNITypeNames.push_back(JNIGlobalRef());
            }
            JNIUtilDeleteLocalRef(jniEnv, paramJNITypeName);
        }
    }
    jmethodID jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, "<init>"
            , JNIGetMethodSig(JNIType::S_void(), paramJNITypes.data(), (jsize)paramJNITypes.size()).c_str()
            );
    zfCoreAssertWithMessageTrim(jmId != NULL, "unable to find Java constructor with sig: %s"
            , JNIGetMethodSig(JNIType::S_void(), paramJNITypes.data(), (jsize)paramJNITypes.size()).c_str()
            );

    zfblockedAlloc(_ZFP_I_ZFAndroidReflect_GIData, GIData);
    GIData->jcls = clsInfo->jcls;
    GIData->jmId = jmId;
    GIData->paramJNITypes = paramJNITypes;
    GIData->paramJNITypeNames = paramJNITypeNames;
    zfclassNotPOD _ZFP_GIWrap {
    public:
        static zfbool GI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
            ZFArray *userData = ZFCastZFObject(ZFArray *, invokerMethod->methodDynamicRegisterUserData());
            if(userData == zfnull) {
                return zffalse;
            }
            _ZFP_I_ZFAndroidReflect_Object *obj = ZFCastZFObject(_ZFP_I_ZFAndroidReflect_Object *, invokerObject);
            obj->autoAlloc = zffalse;
            for(zfindex iGIData = 0; iGIData < userData->count(); ++iGIData) {
                _ZFP_I_ZFAndroidReflect_GIData *GIData = userData->get(iGIData)->toAny();
                if(paramCount != (zfindex)GIData->paramJNITypes.size()) {
                    zfstringAppend(errorHint, "expect %s param, got %s", (zfindex)GIData->paramJNITypes.size(), paramCount);
                    continue;
                }
                JNIEnv *jniEnv = JNIGetJNIEnv();
                zfstlvector<jvalue> params;
                if(errorHint != zfnull && iGIData > 0) {
                    *errorHint += ", ";
                }
                if(!_ZFP_ZFAndroidReflect_paramConvert(jniEnv, params, errorHint, paramList, GIData->paramJNITypes, GIData->paramJNITypeNames)) {
                    continue;
                }
                jobject jobj = JNIUtilNewObjectA(jniEnv, GIData->jcls, GIData->jmId, params.data());
                zfCoreAssert(jobj != NULL);
                obj->jobj = jobj;
                JNIUtilDeleteLocalRef(jniEnv, jobj);
                return zftrue;
            }
            return zffalse;
        }
    };
    const ZFMethod *exist = cls->methodForName("objectOnInit"
            , regParam.methodParamTypeIdAt(0)
            , regParam.methodParamTypeIdAt(1)
            , regParam.methodParamTypeIdAt(2)
            , regParam.methodParamTypeIdAt(3)
            , regParam.methodParamTypeIdAt(4)
            , regParam.methodParamTypeIdAt(5)
            , regParam.methodParamTypeIdAt(6)
            , regParam.methodParamTypeIdAt(7)
            );
    if(exist != zfnull) {
        ZFArray *userData = ZFCastZFObject(ZFArray *, exist->methodDynamicRegisterUserData());
        zfCoreAssert(userData != zfnull);
        userData->add(GIData);
    }
    else {
        zfblockedAlloc(ZFArray, userData);
        userData->add(GIData);
        if(ZFObjectOnInitDynamicRegister(cls, regParam
                    .methodGenericInvoker(_ZFP_GIWrap::GI)
                    .methodDynamicRegisterUserData(userData)
                    ) == zfnull) {
            zfCoreLog("unable to register constructor: %s", regParam);
        }
    }
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
    _ZFP_I_ZFAndroidReflect_ClassInfo *clsInfo = ZFCastZFObject(_ZFP_I_ZFAndroidReflect_ClassInfo *, cls->classDynamicRegisterUserData());
    zfCoreAssert(clsInfo != zfnull);

    zfstring methodNameZF;
    ZFImpl_sys_Android_zfstringFromStringT(methodNameZF, methodName);

    // return type
    JNIType returnJNIType;
    zfstring returnTypeNameZF;
    const zfchar *returnTypeIdZF = zfnull;
    {
        ZFImpl_sys_Android_zfstringFromStringT(returnTypeNameZF, returnTypeName);
        if(!_ZFP_ZFAndroidReflect_typeCheck(returnJNIType, returnTypeIdZF, returnTypeNameZF)) {
            zfCoreCriticalMessage("unknown type name: %s", returnTypeNameZF);
        }
    }

    // param type
    ZFMethodDynamicRegisterParam regParam;
    zfstlvector<JNIType> paramJNITypes;
    zfstlvector<JNIGlobalRef> paramJNITypeNames;
    if(paramTypeNames != NULL) {
        jsize paramCount = JNIUtilGetArrayLength(jniEnv, paramTypeNames);
        zfstring paramTypeName;
        JNIType paramJNIType;
        const zfchar *typeIdZF;
        for(jsize i = 0; i < paramCount; ++i) {
            paramTypeName.removeAll();
            jobject paramJNITypeName = JNIUtilGetObjectArrayElement(jniEnv, paramTypeNames, i);
            ZFImpl_sys_Android_zfstringFromStringT(paramTypeName, paramJNITypeName);
            if(!_ZFP_ZFAndroidReflect_typeCheck(paramJNIType, typeIdZF, paramTypeName)) {
                zfCoreCriticalMessage("unknown type name: %s", paramTypeName);
            }
            regParam.methodParamAdd(typeIdZF);
            paramJNITypes.push_back(paramJNIType);
            if(paramJNIType.getType() == JNIType::T_object || paramJNIType.getType() == JNIType::T_array) {
                paramJNITypeNames.push_back(paramJNITypeName);
            }
            else {
                paramJNITypeNames.push_back(JNIGlobalRef());
            }
            JNIUtilDeleteLocalRef(jniEnv, paramJNITypeName);
        }
    }
    jmethodID jmId = NULL;
    if(isStatic) {
        jmId = JNIUtilGetStaticMethodID(jniEnv, clsInfo->jcls, methodNameZF
                , JNIGetMethodSig(returnJNIType, paramJNITypes.data(), (jsize)paramJNITypes.size()).c_str()
                );
    }
    else {
        jmId = JNIUtilGetMethodID(jniEnv, clsInfo->jcls, methodNameZF
                , JNIGetMethodSig(returnJNIType, paramJNITypes.data(), (jsize)paramJNITypes.size()).c_str()
                );
    }
    zfCoreAssertWithMessageTrim(jmId != NULL, "unable to find Java method with sig: %s"
            , JNIGetMethodSig(JNIType::S_void(), paramJNITypes.data(), (jsize)paramJNITypes.size()).c_str()
            );

    zfblockedAlloc(_ZFP_I_ZFAndroidReflect_GIData, GIData);
    GIData->jcls = clsInfo->jcls;
    GIData->isStatic = (zfbool)isStatic;
    GIData->jmId = jmId;
    GIData->returnJNIType = returnJNIType;
    GIData->returnTypeNameZF = returnTypeNameZF;
    GIData->paramJNITypes = paramJNITypes;
    GIData->paramJNITypeNames = paramJNITypeNames;
    zfclassNotPOD _ZFP_GIWrap {
    public:
        static zfbool GI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
            ZFArray *userData = ZFCastZFObject(ZFArray *, invokerMethod->methodDynamicRegisterUserData());
            if(userData == zfnull) {
                return zffalse;
            }
            _ZFP_I_ZFAndroidReflect_Object *obj = ZFCastZFObject(_ZFP_I_ZFAndroidReflect_Object *, invokerObject);
            for(zfindex iGIData = 0; iGIData < userData->count(); ++iGIData) {
                _ZFP_I_ZFAndroidReflect_GIData *GIData = userData->get(iGIData)->toAny();
                if(paramCount != (zfindex)GIData->paramJNITypes.size()) {
                    zfstringAppend(errorHint, "expect %s param, got %s", (zfindex)GIData->paramJNITypes.size(), paramCount);
                    continue;
                }
                JNIEnv *jniEnv = JNIGetJNIEnv();
                zfstlvector<jvalue> params;
                if(errorHint != zfnull && iGIData > 0) {
                    *errorHint += ", ";
                }
                if(!_ZFP_ZFAndroidReflect_paramConvert(jniEnv, params, errorHint, paramList, GIData->paramJNITypes, GIData->paramJNITypeNames)) {
                    continue;
                }
                switch(GIData->returnJNIType.getType()) {
                    case JNIType::T_boolean: {
                        jboolean tmp = GIData->isStatic
                            ? JNIUtilCallStaticBooleanMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallBooleanMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfbool, (zfbool)tmp);
                        break;
                    }
                    case JNIType::T_byte: {
                        jbyte tmp = GIData->isStatic
                            ? JNIUtilCallStaticByteMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallByteMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfbyte, (zfbyte)tmp);
                        break;
                    }
                    case JNIType::T_char: {
                        jchar tmp = GIData->isStatic
                            ? JNIUtilCallStaticCharMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallCharMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfchar, (zfchar)tmp);
                        break;
                    }
                    case JNIType::T_short: {
                        jshort tmp = GIData->isStatic
                            ? JNIUtilCallStaticShortMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallShortMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfint, (zfint)tmp);
                        break;
                    }
                    case JNIType::T_int: {
                        jint tmp = GIData->isStatic
                            ? JNIUtilCallStaticIntMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallIntMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfint, (zfint)tmp);
                        break;
                    }
                    case JNIType::T_long: {
                        jlong tmp = GIData->isStatic
                            ? JNIUtilCallStaticLongMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallLongMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zftimet, (zftimet)tmp);
                        break;
                    }
                    case JNIType::T_float: {
                        jfloat tmp = GIData->isStatic
                            ? JNIUtilCallStaticFloatMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallFloatMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zffloat, (zffloat)tmp);
                        break;
                    }
                    case JNIType::T_double: {
                        jdouble tmp = GIData->isStatic
                            ? JNIUtilCallStaticDoubleMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallDoubleMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        ret = zflineAlloc(v_zfdouble, (zfdouble)tmp);
                        break;
                    }
                    case JNIType::T_void: {
                        if(GIData->isStatic) {
                            JNIUtilCallStaticVoidMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data());
                        }
                        else {
                            JNIUtilCallVoidMethodA(jniEnv, obj->jobj, GIData->jmId, params.data());
                        }
                        break;
                    }
                    case JNIType::T_array:
                    case JNIType::T_object: {
                        jobject tmp = GIData->isStatic
                            ? JNIUtilCallStaticObjectMethodA(jniEnv, GIData->jcls, GIData->jmId, params.data())
                            : JNIUtilCallObjectMethodA(jniEnv, obj->jobj, GIData->jmId, params.data())
                            ;
                        const ZFClass *tmpCls = ZFAndroidReflect_classForNameInJava(GIData->returnTypeNameZF);
                        if(tmpCls != zfnull && tmpCls->classIsTypeOf(_ZFP_I_ZFAndroidReflect_Object::ClassData())) {
                            ret = tmpCls->newInstance(zflineAlloc(v_ZFPtr, (void *)tmp));
                        }
                        else {
                            zfbool implSuccess = zffalse;
                            ZFCoreArray<_ZFP_ZFAndroidReflect_FromJNITypeCallback> &impl = _ZFP_ZFAndroidReflect_FromJNITypeCallbackList();
                            for(zfindex i = 0; i < impl.count(); ++i) {
                                if(impl[i](ret, tmp, GIData->returnJNIType)) {
                                    implSuccess = zftrue;
                                    break;
                                }
                            }
                            if(!implSuccess) {
                                zfblockedAlloc(_ZFP_I_ZFAndroidReflect_Object, wrapper);
                                wrapper->jobj = tmp;
                                ret = wrapper;
                            }
                        }
                        JNIUtilDeleteLocalRef(jniEnv, tmp);
                        break;
                    }
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        break;
                }
                return zftrue;
            }
            return zffalse;
        }
    };
    const ZFMethod *exist = cls->methodForName(methodNameZF
            , regParam.methodParamTypeIdAt(0)
            , regParam.methodParamTypeIdAt(1)
            , regParam.methodParamTypeIdAt(2)
            , regParam.methodParamTypeIdAt(3)
            , regParam.methodParamTypeIdAt(4)
            , regParam.methodParamTypeIdAt(5)
            , regParam.methodParamTypeIdAt(6)
            , regParam.methodParamTypeIdAt(7)
            );
    if(exist != zfnull) {
        ZFArray *userData = ZFCastZFObject(ZFArray *, exist->methodDynamicRegisterUserData());
        zfCoreAssert(userData != zfnull);
        userData->add(GIData);
    }
    else {
        zfblockedAlloc(ZFArray, userData);
        userData->add(GIData);
        if(ZFMethodDynamicRegister(regParam
                    .methodOwnerClass(cls)
                    .methodReturnTypeId(returnTypeIdZF)
                    .methodName(methodNameZF)
                    .methodGenericInvoker(_ZFP_GIWrap::GI)
                    .methodDynamicRegisterUserData(userData)
                    ) == zfnull) {
            zfCoreLog("unable to register method: %s", regParam);
        }
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

