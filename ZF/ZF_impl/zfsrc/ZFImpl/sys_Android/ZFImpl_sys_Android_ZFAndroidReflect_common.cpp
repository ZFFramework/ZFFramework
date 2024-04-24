#include "ZFImpl_sys_Android_ZFAndroidReflect.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFAndroidReflect_ToJNIType_DEFINE(common) {
    if(jniType.getType() != JNIType::T_object) {
        return zffalse;
    }

    if(zfstringIsEqual(jniType.getId(), JNIType::S_object_String().getId())) {
        v_zfstring *t = ZFCastZFObject(v_zfstring *, obj);
        if(t != zfnull) {
            jobj = ZFImpl_sys_Android_zfstringToString(t->zfv);
            return zftrue;
        }
    }

    return zffalse;
}

ZFAndroidReflect_FromJNIType_DEFINE(common) {
    if(jniType.getType() != JNIType::T_object) {
        return zffalse;
    }

    if(zfstringIsEqual(jniType.getId(), JNIType::S_object_String().getId())) {
        zfobj<v_zfstring> ret;
        obj = ret;
        if(jobj != NULL) {
            ZFImpl_sys_Android_zfstringFromStringT(ret->zfv, jobj);
        }
        return zftrue;
    }

    return zffalse;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

