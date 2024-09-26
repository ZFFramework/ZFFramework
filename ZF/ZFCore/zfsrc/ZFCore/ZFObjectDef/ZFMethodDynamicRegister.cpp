#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFMethodInvokeData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfbool, success)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, const ZFMethod *, invokerMethod)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfany, invokerObject)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfstring, errorHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, ret)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param0)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param1)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param2)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param3)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param4)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param5)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param6)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfauto, param7)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMethodInvokeData, zfauto, callSuper)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFMethodInvokeData, const zfauto &, paramAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFMethodInvokeData, ZFMethodInvokeData *, paramSet
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(ZFObject *, param)
        )

zfauto ZFMethodInvokeData::callSuper(void) {
    ZFCoreAssertWithMessage(invokerMethod->isDynamicRegister(),
        "ZFMethodInvokeData::callSuper() only works for dynamic registered method");
    ZFCoreAssertWithMessage(invokerMethod->ownerClass() != zfnull && invokerMethod->methodType() == ZFMethodTypeVirtual,
        "ZFMethodInvokeData::callSuper() only works for class virtual method");

    ZFCoreQueuePOD<const ZFClass *> toCheck;
    const ZFClass *cls = invokerMethod->ownerClass();
    ZFCoreArray<const ZFMethod *> buf;
    do {
        if(cls->classParent() != zfnull) {
            toCheck.add(cls->classParent());
        }
        for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
            toCheck.add(cls->implementedInterfaceAt(i));
        }
        for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
            toCheck.add(cls->dynamicInterfaceAt(i));
        }
        if(toCheck.isEmpty()) {
            break;
        }

        cls = toCheck.take();
        const ZFMethod *chain = invokerMethod;
        do {
            cls->methodForNameIgnoreParentGetAllT(buf, chain->methodName());
            for(zfindex i = 0; i < buf.count(); ++i) {
                const ZFMethod *m = buf[i];
                if(m->paramTypeIdIsMatch(invokerMethod)) {
                    return m->methodInvoke(invokerObject
                            , param0
                            , param1
                            , param2
                            , param3
                            , param4
                            , param5
                            , param6
                            , param7
                        );
                }
            }
            buf.removeAll();
            chain = chain->aliasFrom();
        } while(chain != zfnull);
    } while(zftrue);
    return zfnull;
}

const zfauto &ZFMethodInvokeData::paramAt(ZF_IN zfindex index) {
    switch(index) {
        case 0: return this->param0;
        case 1: return this->param1;
        case 2: return this->param2;
        case 3: return this->param3;
        case 4: return this->param4;
        case 5: return this->param5;
        case 6: return this->param6;
        case 7: return this->param7;
        default:
            ZFCoreCriticalIndexOutOfRange(index, 8);
            return this->param7;
    } /* ZFMETHOD_MAX_PARAM */
}
ZFMethodInvokeData *ZFMethodInvokeData::paramSet(
        ZF_IN zfindex index
        , ZF_IN ZFObject *param
        ) {
    switch(index) {
        case 0: this->param0 = param; break;
        case 1: this->param1 = param; break;
        case 2: this->param2 = param; break;
        case 3: this->param3 = param; break;
        case 4: this->param4 = param; break;
        case 5: this->param5 = param; break;
        case 6: this->param6 = param; break;
        case 7: this->param7 = param; break;
        default:
            ZFCoreCriticalIndexOutOfRange(index, 8);
            break;
    } /* ZFMETHOD_MAX_PARAM */
    return this;
}

void ZFMethodInvokeData::objectInfoT(ZF_IN_OUT zfstring &ret) {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += this->classData()->className();
    if(!this->success) {
        ret += "(fail)";
    }

    // return type
    ret += " ";
    if(this->ret) {
        this->ret->objectInfoT(ret);
    }
    else if(this->invokerMethod != zfnull) {
        ret += this->invokerMethod->returnTypeId();
    }
    else {
        ret += "?";
    }

    // scope
    ret += " ";
    if(this->invokerObject != zfnull) {
        this->invokerObject->objectInfoOfInstanceT(ret);
        ret += ".";
    }
    else if(this->invokerMethod != zfnull) {
        if(this->invokerMethod->methodNamespace()) {
            ret += this->invokerMethod->methodNamespace();
            ret += "::";
        }
    }

    // method name
    if(this->invokerMethod != zfnull) {
        ret += this->invokerMethod->methodName();
    }
    else {
        ret += "?";
    }

    // params
    ret += "(";
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
        const zfauto &param = this->paramAt(i);
        if(!param || param == ZFMP_DEF()) {
            break;
        }
        if(i > 0) {
            ret += ", ";
        }
        param->objectInfoT(ret);
    }
    ret += ")";

    ret += ZFTOKEN_ZFObjectInfoRight;
}

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
    ZFMethodPrivilegeType methodPrivilegeType;
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
    , methodPrivilegeType(ZFMethodPrivilegeTypePublic)
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

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodPrivilegeType(ZF_IN ZFMethodPrivilegeType methodPrivilegeType) {
    d->methodPrivilegeType = methodPrivilegeType;
    return *this;
}
ZFMethodPrivilegeType ZFMethodDynamicRegisterParam::methodPrivilegeType(void) const {
    return d->methodPrivilegeType;
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

    if(this->methodPrivilegeType() != ZFMethodPrivilegeTypePublic) {
        ZFMethodPrivilegeTypeToStringT(ret, this->methodPrivilegeType());
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
        , ZF_IN_OPT const ZFListener &paramDefaultValueCallback /* = _ZFP_ZFMethod_paramDefaultValueCallbackDummy() */
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

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

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
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, methodPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodDynamicRegister
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, returnTypeId)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_IN_OPT(ZFMethodType, methodType, ZFMethodTypeVirtual)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, methodPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodDynamicUnregister
        , ZFMP_IN(const ZFMethod *, method)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

