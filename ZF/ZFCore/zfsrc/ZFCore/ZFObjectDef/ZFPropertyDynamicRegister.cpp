#include "ZFPropertyDynamicRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFPropertyDynamicRegisterParamPrivate {
public:
    zfuint refCount;
    zfbool zfunsafe_disableChecker;
    zfauto dynamicRegisterUserData;
    const ZFClass *ownerClass;
    zfstring propertyTypeId;
    zfstring propertyTypeName;
    zfstring propertyName;
    const ZFClass *propertyClassOfRetainProperty;
    ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback;
    ZFMethodAccessType propertySetterType;
    ZFMethodAccessType propertyGetterType;

    const ZFMethod *propertyCustomImplSetterMethod;
    const ZFMethod *propertyCustomImplGetterMethod;
    ZFPropertyCallbackIsValueAccessed propertyCustomImplCallbackIsValueAccessed;
    ZFPropertyCallbackIsInitValue propertyCustomImplCallbackIsInitValue;
    ZFPropertyCallbackValueReset propertyCustomImplCallbackValueReset;

public:
    _ZFP_ZFPropertyDynamicRegisterParamPrivate(void)
    : refCount(1)
    , zfunsafe_disableChecker(zffalse)
    , dynamicRegisterUserData(zfnull)
    , ownerClass()
    , propertyTypeId()
    , propertyTypeName()
    , propertyName()
    , propertyClassOfRetainProperty(zfnull)
    , propertyInitValueCallback(zfnull)
    , propertySetterType(ZFMethodAccessTypePublic)
    , propertyGetterType(ZFMethodAccessTypePublic)

    , propertyCustomImplSetterMethod(zfnull)
    , propertyCustomImplGetterMethod(zfnull)
    , propertyCustomImplCallbackIsValueAccessed(zfnull)
    , propertyCustomImplCallbackIsInitValue(zfnull)
    , propertyCustomImplCallbackValueReset(zfnull)
    {
    }
};

// ============================================================
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::dynamicRegisterUserData(ZF_IN ZFObject *dynamicRegisterUserData) {
    d->dynamicRegisterUserData = dynamicRegisterUserData;
    return *this;
}
zfany ZFPropertyDynamicRegisterParam::dynamicRegisterUserData(void) const {
    return d->dynamicRegisterUserData;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::ownerClass(ZF_IN const ZFClass *ownerClass) {
    d->ownerClass = ownerClass;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::ownerClass(void) const {
    return d->ownerClass;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeId(ZF_IN const zfstring &propertyTypeId) {
    d->propertyTypeId = propertyTypeId;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyTypeId(void) const {
    return d->propertyTypeId;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeName(ZF_IN const zfstring &propertyTypeName) {
    d->propertyTypeName = propertyTypeName;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyTypeName(void) const {
    return (d->propertyTypeName ? d->propertyTypeName : d->propertyTypeId);
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyName(ZF_IN const zfstring &propertyName) {
    d->propertyName = propertyName;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyName(void) const {
    return d->propertyName;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyClassOfRetainProperty(ZF_IN const ZFClass *propertyClassOfRetainProperty) {
    d->propertyClassOfRetainProperty = propertyClassOfRetainProperty;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::propertyClassOfRetainProperty(void) const {
    return d->propertyClassOfRetainProperty;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyInitValueCallback(ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback) {
    d->propertyInitValueCallback = propertyInitValueCallback;
    return *this;
}
ZFPropertyCallbackDynamicRegisterInitValueGetter ZFPropertyDynamicRegisterParam::propertyInitValueCallback(void) const {
    return d->propertyInitValueCallback;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertySetterType(ZF_IN ZFMethodAccessType propertySetterType) {
    d->propertySetterType = propertySetterType;
    return *this;
}
ZFMethodAccessType ZFPropertyDynamicRegisterParam::propertySetterType(void) const {
    return d->propertySetterType;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyGetterType(ZF_IN ZFMethodAccessType propertyGetterType) {
    d->propertyGetterType = propertyGetterType;
    return *this;
}
ZFMethodAccessType ZFPropertyDynamicRegisterParam::propertyGetterType(void) const {
    return d->propertyGetterType;
}

// ============================================================
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyCustomImpl(
        ZF_IN const ZFMethod *propertySetterMethod
        , ZF_IN const ZFMethod *propertyGetterMethod
        , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
        , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
        , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
        ) {
    d->propertyCustomImplSetterMethod = propertySetterMethod;
    d->propertyCustomImplGetterMethod = propertyGetterMethod;
    d->propertyCustomImplCallbackIsValueAccessed = callbackIsValueAccessed;
    d->propertyCustomImplCallbackIsInitValue = callbackIsInitValue;
    d->propertyCustomImplCallbackValueReset = callbackValueReset;
    return *this;
}

const ZFMethod *ZFPropertyDynamicRegisterParam::propertyCustomImplSetterMethod(void) const {
    return d->propertyCustomImplSetterMethod;
}
const ZFMethod *ZFPropertyDynamicRegisterParam::propertyCustomImplGetterMethod(void) const {
    return d->propertyCustomImplGetterMethod;
}
ZFPropertyCallbackIsValueAccessed ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackIsValueAccessed(void) const {
    return d->propertyCustomImplCallbackIsValueAccessed;
}
ZFPropertyCallbackIsInitValue ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackIsInitValue(void) const {
    return d->propertyCustomImplCallbackIsInitValue;
}
ZFPropertyCallbackValueReset ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackValueReset(void) const {
    return d->propertyCustomImplCallbackValueReset;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::zfunsafe_disableChecker(ZF_IN zfbool disableChecker) {
    d->zfunsafe_disableChecker = disableChecker;
    return *this;
}
zfbool ZFPropertyDynamicRegisterParam::zfunsafe_disableChecker(void) const {
    return d->zfunsafe_disableChecker;
}

// ============================================================
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFPropertyDynamicRegisterParamPrivate))
{
}
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(ZF_IN ZFPropertyDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFPropertyDynamicRegisterParam::~ZFPropertyDynamicRegisterParam(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::operator = (ZF_IN ZFPropertyDynamicRegisterParam const &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFPropertyDynamicRegisterParam::operator == (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const {
    return (d == ref.d);
}

void ZFPropertyDynamicRegisterParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFPropertyDynamicRegisterParam ";

    if(this->propertyClassOfRetainProperty() != zfnull) {
        ret += this->propertyClassOfRetainProperty()->classNameFull();
        ret += " ";
    }
    else if(this->propertyTypeName()) {
        ret += this->propertyTypeName();
        ret += " ";
    }
    else if(this->propertyTypeId()) {
        ret += this->propertyTypeId();
        ret += " ";
    }

    if(this->ownerClass() != zfnull) {
        ret += this->ownerClass()->classNameFull();
        ret += "::";
    }

    ret += this->propertyName();

    ret += ">";
}

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyDynamicRegister
        , ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFPropertyDynamicUnregister
        , ZFMP_IN(const ZFProperty *, property)
        )

ZF_NAMESPACE_GLOBAL_END

