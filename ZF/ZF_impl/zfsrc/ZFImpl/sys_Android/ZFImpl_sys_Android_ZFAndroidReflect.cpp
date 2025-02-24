#include "ZFImpl_sys_Android_ZFAndroidReflect.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidReflect, ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect)
ZFImpl_sys_Android_jclass_DEFINE(ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult, ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect_InvokeResult)

jobject ZFAndroidReflect::invokeDetail(
        ZF_IN_OPT jobject obj
        , ZF_IN const zfchar *name
        , ZF_IN_OPT const ZFCoreArray<jobject> &params /* = ZFCoreArray<jobject>() */
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "native_invoke",
        JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_object_Object())
            .add(JNIType::S_object_String())
            .add(JNIType::S_array_Object())
        ).c_str());
    jobject jInvokeResult = NULL;
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "native_obtainInvokeResult",
            JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            ).c_str());
        jInvokeResult = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            );
    }
    jobject jName = ZFImpl_sys_Android_zfstringToString(name);
    jobjectArray jParams = NULL;
    if(!params.isEmpty()) {
        jParams = JNIUtilNewObjectArray(jniEnv
                , (jsize)params.count()
                , ZFImpl_sys_Android_jclassObject()
                , NULL
                );
        for(zfindex i = 0; i < params.count(); ++i) {
            JNIUtilSetObjectArrayElement(jniEnv, jParams, (jsize)i, params[i]);
        }
    }
    JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
        , jInvokeResult
        , obj
        , jName
        , jParams
        );
    JNIUtilDeleteLocalRef(jniEnv, jName);
    if(jParams != NULL) {
        JNIUtilDeleteLocalRef(jniEnv, jParams);
    }

    if(this->debug()) {
        static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(), "success", JNIType::S_boolean().getId());
        zfbool successTmp = (zfbool)JNIUtilGetBooleanField(jniEnv, jInvokeResult, jfId);
        if(success) {
            *success = successTmp;
        }
        if(!successTmp) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(), "errorHint", JNIType::S_object_String().getId());
            jobject jErrorHint = JNIUtilGetObjectField(jniEnv, jInvokeResult, jfId);
            zfstring errorHintTmp;
            if(jErrorHint != NULL) {
                ZFImpl_sys_Android_zfstringFromStringT(errorHintTmp, jErrorHint);
                JNIUtilDeleteLocalRef(jniEnv, jErrorHint);
                if(errorHint) {
                    *errorHint += errorHintTmp;
                }
            }
            ZFCoreLogTrim("[ZFAndroidReflect] invoke fail: %s", errorHintTmp);
        }
    }
    else {
        if(success) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(), "success", JNIType::S_boolean().getId());
            *success = (zfbool)JNIUtilGetBooleanField(jniEnv, jInvokeResult, jfId);
        }
        if(errorHint) {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(), "errorHint", JNIType::S_object_String().getId());
            jobject jErrorHint = JNIUtilGetObjectField(jniEnv, jInvokeResult, jfId);
            if(jErrorHint != NULL) {
                ZFImpl_sys_Android_zfstringFromStringT(*errorHint, jErrorHint);
                JNIUtilDeleteLocalRef(jniEnv, jErrorHint);
            }
        }
    }

    jobject ret = NULL;
    { // result
        static jfieldID jfId = JNIUtilGetFieldID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(), "result", JNIType::S_object_Object().getId());
        ret = JNIUtilGetObjectField(jniEnv, jInvokeResult, jfId);
    }

    // cleanup
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "native_releaseInvokeResult",
            JNIGetMethodSig(JNIType::S_void(), JNIParamTypeContainer()
                .add(JNIType::S_object_Object())
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
                , jInvokeResult
                );
    }
    return ret;
}

// ============================================================
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        , ZF_IN jobject p3
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    params.add(p3);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        , ZF_IN jobject p3
        , ZF_IN jobject p4
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    params.add(p3);
    params.add(p4);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        , ZF_IN jobject p3
        , ZF_IN jobject p4
        , ZF_IN jobject p5
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    params.add(p3);
    params.add(p4);
    params.add(p5);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        , ZF_IN jobject p3
        , ZF_IN jobject p4
        , ZF_IN jobject p5
        , ZF_IN jobject p6
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    params.add(p3);
    params.add(p4);
    params.add(p5);
    params.add(p6);
    return this->invokeDetail(obj, name, params);
}
jobject ZFAndroidReflect::invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
        , ZF_IN jobject p0
        , ZF_IN jobject p1
        , ZF_IN jobject p2
        , ZF_IN jobject p3
        , ZF_IN jobject p4
        , ZF_IN jobject p5
        , ZF_IN jobject p6
        , ZF_IN jobject p7
        ) {
    ZFCoreArray<jobject> params;
    params.add(p0);
    params.add(p1);
    params.add(p2);
    params.add(p3);
    params.add(p4);
    params.add(p5);
    params.add(p6);
    params.add(p7);
    return this->invokeDetail(obj, name, params);
}

zfint ZFAndroidReflect::asInt(ZF_IN jobject v, ZF_IN_OPT zfint def /* = -1 */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asInt",
        JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_int())
        ).c_str());
    return (zfint)JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , (jint)def
            );
}
zfuint ZFAndroidReflect::asUInt(ZF_IN jobject v, ZF_IN_OPT zfuint def /* = (zfuint)-1 */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asInt",
        JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_int())
        ).c_str());
    jint ret = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , def == (zfuint)-1 ? (jint)-1 : (jint)def
            );
    return (ret == (jint)-1) ? (zfuint)-1 : (zfuint)ret;
}
zfindex ZFAndroidReflect::asIndex(ZF_IN jobject v, ZF_IN_OPT zfindex def /* = zfindexMax() */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asInt",
        JNIGetMethodSig(JNIType::S_int(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_int())
        ).c_str());
    jint ret = JNIUtilCallStaticIntMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , def == zfindexMax() ? (jint)-1 : (jint)def
            );
    return (ret == (jint)-1) ? zfindexMax() : (zfindex)ret;
}
zflong ZFAndroidReflect::asLong(ZF_IN jobject v, ZF_IN_OPT zflong def /* = -1 */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asLong",
        JNIGetMethodSig(JNIType::S_long(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_long())
        ).c_str());
    return (zflong)JNIUtilCallStaticLongMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , (jlong)def
            );
}
zffloat ZFAndroidReflect::asFloat(ZF_IN jobject v, ZF_IN_OPT zffloat def /* = -1 */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asFloat",
        JNIGetMethodSig(JNIType::S_float(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_float())
        ).c_str());
    return (zffloat)JNIUtilCallStaticFloatMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , (jfloat)def
            );
}
zfdouble ZFAndroidReflect::asDouble(ZF_IN jobject v, ZF_IN_OPT zfdouble def /* = -1 */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "asDouble",
        JNIGetMethodSig(JNIType::S_double(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
            .add(JNIType::S_double())
        ).c_str());
    return (zfdouble)JNIUtilCallStaticDoubleMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            , (jdouble)def
            );
}

void ZFAndroidReflect::asStringT(ZF_IN_OUT zfstring &ret, ZF_IN jobject v, ZF_IN_OPT const zfchar *def /* = zfnull */) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    if(JNIUtilIsInstanceOf(jniEnv, v, ZFImpl_sys_Android_jclassString())) {
        ZFImpl_sys_Android_zfstringFromStringT(ret, v);
    }
    else {
        ret += def;
    }
}
zfstring ZFAndroidReflect::asString(ZF_IN jobject v, ZF_IN_OPT const zfchar *def /* = zfnull */) const {
    zfstring ret;
    this->asStringT(ret, v, def);
    return ret;
}
void ZFAndroidReflect::toStringT(ZF_IN_OUT zfstring &ret, ZF_IN jobject v) const {
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "toString",
        JNIGetMethodSig(JNIType::S_object_String(), JNIParamTypeContainer()
            .add(JNIType::S_object_Object())
        ).c_str());
    jobject jStr = JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , v
            );
    ZFImpl_sys_Android_zfstringFromStringT(ret, jStr);
    JNIUtilDeleteLocalRef(jniEnv, jStr);
}
zfstring ZFAndroidReflect::toString(ZF_IN jobject v) const {
    zfstring ret;
    this->toStringT(ret, v);
    return ret;
}

jobject ZFAndroidReflect::implInterface(ZF_IN const zfchar *interfaceClass, ZF_IN const ZFListener &impl) const {
    if(zfstringIsEmpty(interfaceClass) || !impl) {
        return NULL;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), "native_implInterface",
        JNIGetMethodSig(JNIType::S_object_Object(), JNIParamTypeContainer()
            .add(JNIType::S_object_String())
            .add(JNIPointerJNIType)
        ).c_str());

    jstring jInterfaceClass = JNIUtilNewStringUTF(jniEnv, interfaceClass);
    JNIBlockedDeleteLocalRef(jInterfaceClass);

    // release in native_implInterface
    v_ZFListener *implHolder = zfAlloc(v_ZFListener, impl);

    return JNIUtilCallStaticObjectMethod(jniEnv, ZFImpl_sys_Android_jclassZFAndroidReflect(), jmId
            , jInterfaceClass
            , JNIConvertZFObjectToJNIType(jniEnv, implHolder)
            );
}

// ============================================================
ZFOBJECT_REGISTER(ZFAndroidProxyInfo)

ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFAndroidReflect, ZFAndroidReflect)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFAndroidReflect, zfbool const &, debug
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAndroidReflect, void, debug
        , ZFMP_IN(zfbool const &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFAndroidReflect, jobject, invokeDetail
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN_OPT(const ZFCoreArray<jobject> &, params, ZFCoreArray<jobject>())
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        , ZFMP_IN(jobject, p1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        , ZFMP_IN(jobject, p1)
        , ZFMP_IN(jobject, p2)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        , ZFMP_IN(jobject, p1)
        , ZFMP_IN(jobject, p2)
        , ZFMP_IN(jobject, p3)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_7(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        , ZFMP_IN(jobject, p1)
        , ZFMP_IN(jobject, p2)
        , ZFMP_IN(jobject, p3)
        , ZFMP_IN(jobject, p4)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFAndroidReflect, jobject, invoke
        , ZFMP_IN(jobject, obj)
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN(jobject, p0)
        , ZFMP_IN(jobject, p1)
        , ZFMP_IN(jobject, p2)
        , ZFMP_IN(jobject, p3)
        , ZFMP_IN(jobject, p4)
        , ZFMP_IN(jobject, p5)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zfint, asInt
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zfint, def, -1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zfuint, asUInt
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zfuint, def, (zfuint)-1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zfindex, asIndex
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zfindex, def, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zflong, asLong
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zflong, def, -1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zffloat, asFloat
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zffloat, def, -1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zfdouble, asDouble
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(zfdouble, def, -1)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFAndroidReflect, void, asStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(const zfchar *, def, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, zfstring, asString
        , ZFMP_IN(jobject, v)
        , ZFMP_IN_OPT(const zfchar *, def, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, void, toStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(jobject, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFAndroidReflect, zfstring, toString
        , ZFMP_IN(jobject, v)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFAndroidReflect, jobject, implInterface
        , ZFMP_IN(const zfchar *, interfaceClass)
        , ZFMP_IN(const ZFListener &, impl)
        )

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect
        , jobject, native_1implInterfaceCallback
        , JNIPointer zfjniPointerImpl
        , jobject proxy
        , jstring methodName
        , jobject method
        , jobjectArray args
        ) {
    v_ZFListener *implHolder = JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerImpl);
    ZFListener impl = implHolder->zfv;

    zfobj<ZFAndroidProxyInfo> info;
    info->owner(proxy);
    info->methodName(ZFImpl_sys_Android_zfstringFromString(methodName));
    info->method(method);

    ZFArgs zfargs;
    zfargs.param0(info);
    impl.execute(zfargs);
    v_ZFAndroid_jobject *retHolder = zfargs.result();
    if(retHolder) {
        return retHolder->zfv;
    }
    else {
        return NULL;
    }
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

