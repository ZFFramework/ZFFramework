#include "ZFMethodDynamicRegister.h"
#include "ZFObjectImpl.h"
#include "ZFDynamicInvoker.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFMethodDynamicRegisterParamPrivate {
public:
    zfuint refCount;
    zfbool zfunsafe_disableChecker;
    zfauto dynamicRegisterUserData;
    const ZFClass *ownerClass;
    zfstring methodNamespace;
    ZFMethodGenericInvoker methodGenericInvoker;
    ZFListener methodImpl;
    ZFMethodType methodType;
    ZFMethodAccessType methodAccessType;
    zfstring methodName;
    zfstring returnTypeId;
    zfindex paramCount;
    zfstring paramTypeId[ZFMETHOD_MAX_PARAM];
    zfstring paramName[ZFMETHOD_MAX_PARAM];
    ZFListener paramDefaultValueCallback[ZFMETHOD_MAX_PARAM];

public:
    _ZFP_ZFMethodDynamicRegisterParamPrivate(void)
    : refCount(1)
    , zfunsafe_disableChecker(zffalse)
    , dynamicRegisterUserData()
    , ownerClass(zfnull)
    , methodNamespace()
    , methodGenericInvoker(zfnull)
    , methodImpl()
    , methodType(ZFMethodTypeVirtual)
    , methodAccessType(ZFMethodAccessTypePublic)
    , methodName()
    , returnTypeId(ZFTypeId_void())
    , paramCount(0)
    , paramTypeId()
    , paramName()
    , paramDefaultValueCallback()
    {
    }
};

// ============================================================
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::dynamicRegisterUserData(ZF_IN ZFObject *dynamicRegisterUserData) {
    d->dynamicRegisterUserData = dynamicRegisterUserData;
    return *this;
}
zfany ZFMethodDynamicRegisterParam::dynamicRegisterUserData(void) const {
    return d->dynamicRegisterUserData;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::ownerClass(ZF_IN const ZFClass *ownerClass) {
    d->ownerClass = ownerClass;
    return *this;
}
const ZFClass *ZFMethodDynamicRegisterParam::ownerClass(void) const {
    return d->ownerClass;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodNamespace(ZF_IN const zfstring &methodNamespace) {
    d->methodNamespace = methodNamespace;
    return *this;
}
const zfstring &ZFMethodDynamicRegisterParam::methodNamespace(void) const {
    return d->methodNamespace;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) {
    d->methodGenericInvoker = methodGenericInvoker;
    return *this;
}
ZFMethodGenericInvoker ZFMethodDynamicRegisterParam::methodGenericInvoker(void) const {
    return d->methodGenericInvoker;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodImpl(ZF_IN const ZFListener &methodImpl) {
    d->methodImpl = methodImpl;
    return *this;
}
const ZFListener &ZFMethodDynamicRegisterParam::methodImpl(void) const {
    return d->methodImpl;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodType(ZF_IN ZFMethodType methodType) {
    d->methodType = methodType;
    return *this;
}
ZFMethodType ZFMethodDynamicRegisterParam::methodType(void) const {
    return d->methodType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodAccessType(ZF_IN ZFMethodAccessType methodAccessType) {
    d->methodAccessType = methodAccessType;
    return *this;
}
ZFMethodAccessType ZFMethodDynamicRegisterParam::methodAccessType(void) const {
    return d->methodAccessType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodName(ZF_IN const zfstring &methodName) {
    d->methodName = methodName;
    return *this;
}
const zfstring &ZFMethodDynamicRegisterParam::methodName(void) const {
    return d->methodName;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::returnTypeId(ZF_IN const zfstring &returnTypeId) {
    d->returnTypeId = returnTypeId;
    return *this;
}
const zfstring &ZFMethodDynamicRegisterParam::returnTypeId(void) const {
    return d->returnTypeId;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodParam(
        ZF_IN const zfstring &paramTypeId
        , ZF_IN_OPT const zfstring &paramName /* = zfnull */
        , ZF_IN_OPT const ZFListener &paramDefaultValueCallback /* = _ZFP_ZFMethod_paramDefaultValueCallbackDummy() */
        ) {
    ZFCoreAssert(d->paramCount < ZFMETHOD_MAX_PARAM);
    if(paramTypeId != zfnull) {
        d->paramTypeId[d->paramCount] = paramTypeId;
        if(paramName) {
            d->paramName[d->paramCount] = paramName;
        }
        else {
            d->paramName[d->paramCount] = zfstr("p%s", d->paramCount);
        }
        d->paramDefaultValueCallback[d->paramCount] = paramDefaultValueCallback;
        ++(d->paramCount);
    }
    return *this;
}
zfindex ZFMethodDynamicRegisterParam::paramCount(void) const {
    return d->paramCount;
}
const zfstring &ZFMethodDynamicRegisterParam::paramTypeIdAt(ZF_IN zfindex index) const {
    return (index < d->paramCount ? d->paramTypeId[index] : zfstring::Empty());
}
const zfstring &ZFMethodDynamicRegisterParam::paramNameAt(ZF_IN zfindex index) const {
    return (index < d->paramCount ? d->paramName[index] : zfstring::Empty());
}
const ZFListener &ZFMethodDynamicRegisterParam::paramDefaultValueCallbackAt(ZF_IN zfindex index) const {
    return (index < d->paramCount ? d->paramDefaultValueCallback[index] : _ZFP_ZFMethod_paramDefaultValueCallbackDummy());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::zfunsafe_disableChecker(ZF_IN zfbool disableChecker) {
    d->zfunsafe_disableChecker = disableChecker;
    return *this;
}
zfbool ZFMethodDynamicRegisterParam::zfunsafe_disableChecker(void) const {
    return d->zfunsafe_disableChecker;
}

ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFMethodDynamicRegisterParamPrivate))
{
}
ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(ZF_IN ZFMethodDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFMethodDynamicRegisterParam::~ZFMethodDynamicRegisterParam(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::operator = (ZF_IN ZFMethodDynamicRegisterParam const &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFMethodDynamicRegisterParam::operator == (ZF_IN ZFMethodDynamicRegisterParam const &ref) const {
    return (d == ref.d);
}

void ZFMethodDynamicRegisterParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFMethodDynamicRegisterParam ";

    if(this->methodAccessType() != ZFMethodAccessTypePublic) {
        ZFMethodAccessTypeToStringT(ret, this->methodAccessType());
        ret += ":";
    }

    switch(this->methodType()) {
        case ZFMethodTypeStatic:
            ret += "static ";
            break;
        case ZFMethodTypeVirtual:
            ret += "virtual ";
            break;
        default:
            break;
    }

    if(this->returnTypeId()) {
        ret += this->returnTypeId();
        ret += " ";
    }

    if(this->ownerClass() != zfnull) {
        ret += this->ownerClass()->classNameFull();
        ret += "::";
    }
    else {
        const zfchar *ns = ZFNamespaceSkipGlobal(this->methodNamespace());
        if(ns) {
            ret += ns;
            ret += "::";
        }
    }

    ret += this->methodName();
    ret += "(";
    if(this->paramCount() == 0) {
        ret += "void";
    }
    else {
        for(zfindex i = 0; i < this->paramCount(); ++i) {
            if(i > 0) {
                ret += ", ";
            }
            if(this->paramTypeIdAt(i)) {
                ret += this->paramTypeIdAt(i);
                ret += " ";
            }

            ret += this->paramNameAt(i);

            if(this->paramDefaultValueCallbackAt(i) != zfnull) {
                ZFArgs zfargs;
                zfargs.result(ZFMP_DEF());
                this->paramDefaultValueCallbackAt(i).execute(zfargs);
                if(zfargs.result() != ZFMP_DEF()) {
                    ret += " = ";
                    ZFObjectInfoT(ret, zfargs.result());
                }
            }
        }
    }
    ret += ")";

    ret += ">";
}

// ============================================================
zfclassNotPOD _ZFP_ZFMPPrivate {
public:
    zfuint refCount;
    zfindex paramCount;
    zfstring paramTypeId[ZFMETHOD_MAX_PARAM];
    zfstring paramName[ZFMETHOD_MAX_PARAM];
    ZFListener paramDefaultValueCallback[ZFMETHOD_MAX_PARAM];
public:
    _ZFP_ZFMPPrivate(void)
    : refCount(1)
    , paramCount(0)
    , paramTypeId()
    , paramName()
    , paramDefaultValueCallback()
    {
    }
};

ZFMP &ZFMP::mp(
        ZF_IN const zfstring &paramTypeId
        , ZF_IN_OPT const zfstring &paramName /* = zfnull */
        ) {
    ZFCoreAssert(d->paramCount <= ZFMETHOD_MAX_PARAM);
    d->paramTypeId[d->paramCount] = paramTypeId;
    d->paramName[d->paramCount] = paramName;
    ++(d->paramCount);
    return *this;
}
ZFMP &ZFMP::mp(
        ZF_IN const zfstring &paramTypeId
        , ZF_IN const zfstring &paramName
        , ZF_IN ZFObject *paramDefaultValue
        ) {
    zfauto wrap;
    if(!zfconvT(wrap, paramTypeId, paramDefaultValue)) {
        ZFCoreLogTrim("invalid param default value: \"%s\", desired: %s"
                , paramDefaultValue
                , paramTypeId
                );
    }
    ZFLISTENER_1(paramDefaultValueCallback
            , zfauto, wrap
            ) {
        zfargs.result(wrap);
    } ZFLISTENER_END()
    return this->mpWithInit(paramTypeId, paramName, paramDefaultValueCallback);
}
ZFMP &ZFMP::mpWithInit(
        ZF_IN const zfstring &paramTypeId
        , ZF_IN const zfstring &paramName
        , ZF_IN const ZFListener &paramDefaultValueCallback
        ) {
    ZFCoreAssert(d->paramCount <= ZFMETHOD_MAX_PARAM);
    d->paramTypeId[d->paramCount] = paramTypeId;
    d->paramName[d->paramCount] = paramName;
    d->paramDefaultValueCallback[d->paramCount] = paramDefaultValueCallback;
    ++(d->paramCount);
    return *this;
}

zfindex ZFMP::paramCount(void) const {
    return d->paramCount;
}
const zfstring &ZFMP::paramTypeIdAt(ZF_IN zfindex index) const {
    return d->paramTypeId[index];
}
const zfstring &ZFMP::paramNameAt(ZF_IN zfindex index) const {
    return d->paramName[index];
}
const ZFListener &ZFMP::paramDefaultValueCallbackAt(ZF_IN zfindex index) const {
    return d->paramDefaultValueCallback[index];
}

ZFMP::ZFMP(void)
: d(zfnew(_ZFP_ZFMPPrivate))
{
}
ZFMP::ZFMP(ZF_IN const ZFMP &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFMP::~ZFMP(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFMP &ZFMP::operator = (ZF_IN const ZFMP &ref) {
    _ZFP_ZFMPPrivate *dTmp = d;
    d = ref.d;
    ++(d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfdelete(dTmp);
    }
    return *this;
}

void ZFMP::methodParamListInfoT(ZF_IN_OUT zfstring &ret) const {
    for(zfindex i = 0; i < d->paramCount; ++i) {
        if(i > 0) {
            ret += ", ";
        }
        ret += d->paramTypeId[i];
        ret += " ";
        if(d->paramName[i].isEmpty()) {
            zfstringAppend(ret, "p%s", i);
        }
        else {
            ret += d->paramName[i];
        }
        ZFArgs zfargs;
        zfargs.result(ZFMP_DEF());
        d->paramDefaultValueCallback[i].execute(zfargs);
        if(zfargs.result() != ZFMP_DEF()) {
            ret += " = ";
            ZFObjectInfoT(ret, zfargs.result());
        }
    }
}

void ZFMP::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "ZFMP(";
    this->methodParamListInfoT(ret);
    ret += ")";
}

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodDynamicRegister
        , ZFMP_IN(const ZFMethodDynamicRegisterParam &, param)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodDynamicRegister
        , ZFMP_IN(const ZFClass *, ownerClass)
        , ZFMP_IN(const zfstring &, returnTypeId)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_IN_OPT(ZFMethodType, methodType, ZFMethodTypeVirtual)
        , ZFMP_IN_OPT(ZFMethodAccessType, methodAccessType, ZFMethodAccessTypePublic)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodDynamicRegister
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, returnTypeId)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_IN_OPT(ZFMethodType, methodType, ZFMethodTypeVirtual)
        , ZFMP_IN_OPT(ZFMethodAccessType, methodAccessType, ZFMethodAccessTypePublic)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodDynamicUnregister
        , ZFMP_IN(const ZFMethod *, method)
        )

ZF_NAMESPACE_GLOBAL_END

