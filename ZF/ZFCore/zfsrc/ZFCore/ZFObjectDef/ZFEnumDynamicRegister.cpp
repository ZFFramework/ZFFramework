#include "ZFEnumDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "ZFClassDynamicRegister.h"
#include "ZFMethodUserRegister.h"
#include "ZFTypeIdDynamicRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFEnumDynamicHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnumDynamicHolder, ZFObject)
public:
    _ZFP_ZFEnumData *d;
    ZFCoreArray<const ZFMethod *> userRegMethods;
};
zfclass _ZFP_I_ZFEnumDynamic : zfextend ZFEnum {
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnumDynamic, ZFEnum)

public:
    zfoverride
    virtual zfindex enumCount(void) {
        return _ZFP_ZFEnumDataRef()->enumCount();
    }
    zfoverride
    virtual zfuint enumDefault(void) {
        return _ZFP_ZFEnumDataRef()->enumDefault;
    }
    zfoverride
    virtual zfbool enumIsFlags(void) {
        return _ZFP_ZFEnumDataRef()->enumIsFlags;
    }
    zfoverride
    virtual zfindex enumIndexForValue(ZF_IN zfuint value) {
        return _ZFP_ZFEnumDataRef()->enumIndexForValue(value);
    }
    zfoverride
    virtual zfuint enumValueAt(ZF_IN zfindex index) {
        return _ZFP_ZFEnumDataRef()->enumValueAt(index);
    }
    zfoverride
    virtual const zfstring &enumNameAt(ZF_IN zfindex index) {
        return _ZFP_ZFEnumDataRef()->enumNameAt(index);
    }
    zfoverride
    virtual zfbool enumValueContain(ZF_IN zfuint value) {
        return _ZFP_ZFEnumDataRef()->enumValueContain(value);
    }
    zfoverride
    virtual zfuint enumValueForName(ZF_IN const zfstring &name) {
        return _ZFP_ZFEnumDataRef()->enumValueForName(name);
    }
    zfoverride
    virtual const zfstring &enumNameForValue(ZF_IN zfuint value) {
        return _ZFP_ZFEnumDataRef()->enumNameForValue(value);
    }
public:
    zfoverride
    virtual const zfstring &zfvTypeId(void) {
        return _ZFP_ZFEnumDataRef()->ownerClass->classNameFull();
    }
private:
    const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) {
        return this->classData()->classDynamicRegisterUserData()->to<_ZFP_I_ZFEnumDynamicHolder *>()->d;
    }
};
zfclassNotPOD _ZFP_ZFEnumTypeId : zfextend ZFTypeInfo {
public:
    const ZFClass *enumClass;

public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return zftrue;
    }
    zfoverride
    virtual const zfstring &typeId(void) const {
        return this->enumClass->classNameFull();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return this->enumClass;
    }

public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        obj = enumClass->newInstance();
        ZFEnum *p = obj;
        if(p == zfnull) {
            return zffalse;
        }
        p->enumValue(*(const zfuint *)v);
        return zftrue;
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        ZFEnum *p = obj;
        if(p == zfnull) {
            return zfnull;
        }
        return (void *)zfpoolNew(zfuint, p->enumValue());
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfpoolDelete((zfuint *)v);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfpoolNew(ZFCoreArray<zfuint>);
    }
};

// ============================================================
static ZFEnum *_ZFP_ZFEnumDynamic_e(
        ZF_IN const ZFMethod *invokerMethod
        , ZF_IN zfany const &invokerObject
        ) {
    const zfchar *enumName = invokerMethod->methodName() + zfslen("e_");
    zfstring tagKey = zfstr("_ZFP_EnumDyn_%s", enumName);
    ZFEnum *ret = invokerMethod->ownerClass()->classTag(tagKey);
    if(ret != zfnull) {
        return ret;
    }
    const _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataFind(invokerMethod->ownerClass());
    zfauto retHolder = invokerMethod->ownerClass()->newInstance();
    ret = retHolder;
    ret->_ZFP_enumValue(d->enumValueForName(enumName));
    invokerMethod->ownerClass()->classTag(tagKey, retHolder);
    return ret;
}
const ZFClass *ZFEnumDynamicRegister(
        ZF_IN const zfstring &enumClassName
        , ZF_IN const ZFCoreArray<zfuint> &enumValues
        , ZF_IN const ZFCoreArray<zfstring> &enumNames
        , ZF_IN zfuint enumDefault /* = ZFEnumInvalid() */
        , ZF_IN zfbool enumIsFlags /* = zffalse */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(enumValues.count() != enumNames.count()) {
        zfstringAppend(errorHint, "enum values and names mismatch");
        return zfnull;
    }
    if(enumValues.isEmpty()) {
        zfstringAppend(errorHint, "empty enum value");
        return zfnull;
    }

    if(enumDefault == ZFEnumInvalid()) {
        enumDefault = enumValues[0];
    }
    else {
        if(enumValues.find(enumDefault) == zfindexMax()) {
            zfstringAppend(errorHint,
                "default enum value %s doesn't exists in enum value list: %s",
                enumDefault,
                enumValues.objectInfoOfContent());
            return zfnull;
        }
    }

    // class
    zfobj<_ZFP_I_ZFEnumDynamicHolder> d;
    const ZFClass *enumClass = ZFClassDynamicRegister(
        enumClassName, _ZFP_I_ZFEnumDynamic::ClassData(),
        d, errorHint);
    if(enumClass == zfnull) {
        return zfnull;
    }

    // internal data
    d->d = _ZFP_ZFEnumDataAccess(enumClass);
    for(zfindex i = 0; i < enumValues.count(); ++i) {
        d->d->add(zftrue, enumValues[i], enumNames[i]);
    }
    d->d->needInitFlag = zffalse;
    d->d->enumDefault = enumDefault;
    d->d->enumIsFlags = enumIsFlags;

    // typeid
    _ZFP_ZFEnumTypeId *typeIdData = zfnew(_ZFP_ZFEnumTypeId);
    ZFCorePointerForObject<ZFTypeInfo *> typeIdDataHolder(typeIdData);
    typeIdData->enumClass = enumClass;
    if(!ZFTypeIdDynamicRegister(enumClassName, typeIdDataHolder, errorHint)) {
        ZFClassDynamicUnregister(enumClass);
        return zfnull;
    }

    // method
    _ZFP_ZFEnumMethodReg(d->userRegMethods, d->d);
    for(zfindex i = 0; i < d->d->enumCount(); ++i) {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDynamic_e(invokerMethod, invokerObject);
            }, enumClass, public, ZFMethodTypeStatic,
            ZFEnum *, zfstr("e_%s", d->d->enumNameAt(i)));
        d->userRegMethods.add(resultMethod);
    }

    return d->d->ownerClass;
}
void ZFEnumDynamicUnregister(ZF_IN const ZFClass *enumClass) {
    if(enumClass == zfnull) {
        return;
    }

    _ZFP_I_ZFEnumDynamicHolder *d = enumClass->classDynamicRegisterUserData();
    if(d == zfnull) {
        return;
    }
    for(zfindex i = 0; i < d->userRegMethods.count(); ++i) {
        ZFMethodUserUnregister(d->userRegMethods[i]);
    }
    ZFTypeIdDynamicUnregister(enumClass->classNameFull());
    _ZFP_ZFEnumDataCleanup(enumClass);
    ZFClassDynamicUnregister(enumClass);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_6(const ZFClass *, ZFEnumDynamicRegister
        , ZFMP_IN(const zfstring &, enumClassName)
        , ZFMP_IN(const ZFCoreArray<zfuint> &, enumValues)
        , ZFMP_IN(const ZFCoreArray<zfstring> &, enumNames)
        , ZFMP_IN_OPT(zfuint, enumDefault, ZFEnumInvalid())
        , ZFMP_IN_OPT(zfbool, enumIsFlags, zffalse)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFEnumDynamicUnregister
        , ZFMP_IN(const ZFClass *, enumClass)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

