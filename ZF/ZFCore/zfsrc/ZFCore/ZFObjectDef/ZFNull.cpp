#include "ZFNull.h"
#include "ZFObjectImpl.h"
#include "ZFCopyable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFNull */
zfclass ZFNullObject : zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable {
    ZFOBJECT_DECLARE(ZFNullObject, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable)

public:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
        ret += "ZFNull";
    }
    zfoverride
    virtual zfidentity objectHashImpl(void) {
        return zfidentityInvalid();
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        if(this == anotherObj || (anotherObj != zfnull && anotherObj->classData() == ZFNullObject::ClassData())) {
            return ZFCompareEqual;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void objectTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) {
        ZFCoreCriticalMessageTrim("setting tag to ZFNull is not allowed, key: %s", key);
    }

private:
    ZFMETHOD_DECLARE_DETAIL_0(private, ZFMethodTypeStatic, zfauto, ZFSerializableKeyword_serializableNewInstanceId)

public:
    zfoverride
    virtual zfauto copy(void) {
        return ZFNull();
    }
};

ZFOBJECT_REGISTER(ZFNullObject)

ZFMETHOD_DEFINE_0(ZFNullObject, zfauto, ZFSerializableKeyword_serializableNewInstanceId) {
    return ZFNull();
}

ZFObject *_ZFP_ZFNull = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfnullObjHolder, ZFLevelZFFrameworkStatic) {
    _ZFP_ZFNull = zfAlloc(ZFNullObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfnullObjHolder) {
    zfRelease(_ZFP_ZFNull);
    _ZFP_ZFNull = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(zfnullObjHolder)

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_0({return ZFNull();}, ZFObject *, ZFNull)

ZF_NAMESPACE_GLOBAL_END

