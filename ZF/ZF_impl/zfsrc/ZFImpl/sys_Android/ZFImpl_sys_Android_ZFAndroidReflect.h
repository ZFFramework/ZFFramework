#ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_
#define _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_

#include "ZFImpl_sys_Android_def.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidReflect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidReflect)
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFAndroidReflect(void);

#define ZFImpl_sys_Android_JNI_ID_ZFAndroidReflect_InvokeResult ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidReflect_00024InvokeResult)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidReflect_InvokeResult ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidReflect$InvokeResult)
extern ZFLIB_ZF_impl jclass ZFImpl_sys_Android_jclassZFAndroidReflect_InvokeResult(void);

zfclass ZFLIB_ZF_impl ZFAndroidProxyInfo : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAndroidProxyInfo, ZFObject)
    ZFPROPERTY_ASSIGN(JNIGlobalRef, owner)
    ZFPROPERTY_ASSIGN(zfstring, methodName)
    ZFPROPERTY_ASSIGN(JNIGlobalRef, method)
    ZFPROPERTY_ASSIGN(ZFCoreArray<JNIGlobalRef>, params)
    ZFPROPERTY_ASSIGN(JNIGlobalRef, result)
    ZFPROPERTY_ASSIGN(zfstring, errorHint)
};

zfclassLikePOD ZFLIB_ZF_impl ZFAndroidReflect {
    ZFCORE_PARAM_DECLARE_SELF(ZFAndroidReflect)
public:
    ZFCORE_PARAM(zfbool, debug, zffalse)
public:
    jobject invokeDetail(
            ZF_IN_OPT jobject obj
            , ZF_IN const zfchar *name
            , ZF_IN_OPT const ZFCoreArray<jobject> &params = ZFCoreArray<jobject>()
            , ZF_OUT_OPT zfbool *success = zfnull
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) const;
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name) {return this->invokeDetail(obj, name);}
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            , ZF_IN jobject p3
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            , ZF_IN jobject p3
            , ZF_IN jobject p4
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            , ZF_IN jobject p3
            , ZF_IN jobject p4
            , ZF_IN jobject p5
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            , ZF_IN jobject p3
            , ZF_IN jobject p4
            , ZF_IN jobject p5
            , ZF_IN jobject p6
            );
    jobject invoke(ZF_IN_OPT jobject obj, ZF_IN const zfchar *name
            , ZF_IN jobject p0
            , ZF_IN jobject p1
            , ZF_IN jobject p2
            , ZF_IN jobject p3
            , ZF_IN jobject p4
            , ZF_IN jobject p5
            , ZF_IN jobject p6
            , ZF_IN jobject p7
            );

    zfint asInt(ZF_IN jobject v, ZF_IN_OPT zfint def = -1) const;
    zfuint asUInt(ZF_IN jobject v, ZF_IN_OPT zfuint def = (zfuint)-1) const;
    zfindex asIndex(ZF_IN jobject v, ZF_IN_OPT zfindex def = zfindexMax()) const;
    zflong asLong(ZF_IN jobject v, ZF_IN_OPT zflong def = -1) const;
    zffloat asFloat(ZF_IN jobject v, ZF_IN_OPT zffloat def = -1) const;
    zfdouble asDouble(ZF_IN jobject v, ZF_IN_OPT zfdouble def = -1) const;

    void asStringT(ZF_IN_OUT zfstring &ret, ZF_IN jobject v, ZF_IN_OPT const zfchar *def = zfnull) const;
    zfstring asString(ZF_IN jobject v, ZF_IN_OPT const zfchar *def = zfnull) const;
    void toStringT(ZF_IN_OUT zfstring &ret, ZF_IN jobject v) const;
    zfstring toString(ZF_IN jobject v) const;

    // create callback via java.lang.reflect.Proxy
    // impl's param0 would be a ZFAndroidProxyInfo, and impl must:
    // * set none empty ZFAndroidProxyInfo.errorHint if invoke fail
    // * store ZFAndroidProxyInfo.result for method result
    jobject implInterface(ZF_IN const zfchar *interfaceClass, ZF_IN const ZFListener &impl) const;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZF_impl, ZFAndroidReflect, ZFAndroidReflect)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZF_impl, ZFAndroidReflect, ZFAndroidReflect)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFAndroidReflect_h_

