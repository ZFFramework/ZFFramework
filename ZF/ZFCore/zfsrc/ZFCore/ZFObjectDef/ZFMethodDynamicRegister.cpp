#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFMethodInvokeData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfbool, invokeSuccess)
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
    ZFCoreAssertWithMessage(invokerMethod->methodIsDynamicRegister(),
        "ZFMethodInvokeData::callSuper() only works for dynamic registered method");
    ZFCoreAssertWithMessage(invokerMethod->methodOwnerClass() != zfnull && invokerMethod->methodType() == ZFMethodTypeVirtual,
        "ZFMethodInvokeData::callSuper() only works for class virtual method");

    ZFCoreQueuePOD<const ZFClass *> toCheck;
    const ZFClass *cls = invokerMethod->methodOwnerClass();
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
                if(m->methodParamTypeIdIsMatch(invokerMethod)) {
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
            chain = chain->methodAliasFrom();
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
    if(!this->invokeSuccess) {
        ret += "(fail)";
    }

    // return type
    ret += " ";
    if(this->ret) {
        this->ret->objectInfoT(ret);
    }
    else if(this->invokerMethod != zfnull) {
        ret += this->invokerMethod->methodReturnTypeId();
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
        if(!zfstringIsEmpty(this->invokerMethod->methodNamespace())) {
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
    zfauto methodDynamicRegisterUserData;
    const ZFClass *methodOwnerClass;
    zfstring methodNamespace;
    ZFMethodGenericInvoker methodGenericInvoker;
    ZFListener methodImpl;
    ZFMethodType methodType;
    ZFMethodPrivilegeType methodPrivilegeType;
    zfstring methodName;
    zfstring methodReturnTypeId;
    zfindex methodParamCount;
    zfstring methodParamTypeId[ZFMETHOD_MAX_PARAM];
    zfstring methodParamName[ZFMETHOD_MAX_PARAM];
    ZFListener methodParamDefaultValueCallback[ZFMETHOD_MAX_PARAM];

public:
    _ZFP_ZFMethodDynamicRegisterParamPrivate(void)
    : refCount(1)
    , zfunsafe_disableChecker(zffalse)
    , methodDynamicRegisterUserData()
    , methodOwnerClass(zfnull)
    , methodNamespace()
    , methodGenericInvoker(zfnull)
    , methodImpl()
    , methodType(ZFMethodTypeVirtual)
    , methodPrivilegeType(ZFMethodPrivilegeTypePublic)
    , methodName()
    , methodReturnTypeId(ZFTypeId_void())
    , methodParamCount(0)
    , methodParamTypeId()
    , methodParamName()
    , methodParamDefaultValueCallback()
    {
    }
};

// ============================================================
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodDynamicRegisterUserData(ZF_IN ZFObject *methodDynamicRegisterUserData) {
    d->methodDynamicRegisterUserData = methodDynamicRegisterUserData;
    return *this;
}
zfany ZFMethodDynamicRegisterParam::methodDynamicRegisterUserData(void) const {
    return d->methodDynamicRegisterUserData;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodOwnerClass(ZF_IN const ZFClass *methodOwnerClass) {
    d->methodOwnerClass = methodOwnerClass;
    return *this;
}
const ZFClass *ZFMethodDynamicRegisterParam::methodOwnerClass(void) const {
    return d->methodOwnerClass;
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

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodReturnTypeId(ZF_IN const zfstring &methodReturnTypeId) {
    d->methodReturnTypeId = methodReturnTypeId;
    return *this;
}
const zfstring &ZFMethodDynamicRegisterParam::methodReturnTypeId(void) const {
    return d->methodReturnTypeId;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodParamAdd(
        ZF_IN const zfstring &methodParamTypeId
        , ZF_IN_OPT const zfstring &methodParamName /* = zfnull */
        , ZF_IN_OPT const ZFListener &methodParamDefaultValueCallback /* = _ZFP_ZFMethod_paramDefaultValueCallbackDummy() */
        ) {
    ZFCoreAssert(d->methodParamCount < ZFMETHOD_MAX_PARAM);
    if(methodParamTypeId != zfnull) {
        d->methodParamTypeId[d->methodParamCount] = methodParamTypeId;
        if(!zfstringIsEmpty(methodParamName)) {
            d->methodParamName[d->methodParamCount] = methodParamName;
        }
        else {
            d->methodParamName[d->methodParamCount] = zfstr("p%s", d->methodParamCount);
        }
        d->methodParamDefaultValueCallback[d->methodParamCount] = methodParamDefaultValueCallback;
        ++(d->methodParamCount);
    }
    return *this;
}
zfindex ZFMethodDynamicRegisterParam::methodParamCount(void) const {
    return d->methodParamCount;
}
const zfstring &ZFMethodDynamicRegisterParam::methodParamTypeIdAt(ZF_IN zfindex index) const {
    return (index < d->methodParamCount ? d->methodParamTypeId[index] : zfstring::Empty());
}
const zfstring &ZFMethodDynamicRegisterParam::methodParamNameAt(ZF_IN zfindex index) const {
    return (index < d->methodParamCount ? d->methodParamName[index] : zfstring::Empty());
}
const ZFListener &ZFMethodDynamicRegisterParam::methodParamDefaultValueCallbackAt(ZF_IN zfindex index) const {
    return (index < d->methodParamCount ? d->methodParamDefaultValueCallback[index] : _ZFP_ZFMethod_paramDefaultValueCallbackDummy());
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

    if(!zfstringIsEmpty(this->methodReturnTypeId())) {
        ret += this->methodReturnTypeId();
        ret += " ";
    }

    if(this->methodOwnerClass() != zfnull) {
        ret += this->methodOwnerClass()->classNameFull();
        ret += "::";
    }
    else {
        const zfchar *ns = ZFNamespaceSkipGlobal(this->methodNamespace());
        if(!zfstringIsEmpty(ns)) {
            ret += ns;
            ret += "::";
        }
    }

    ret += this->methodName();
    ret += "(";
    if(this->methodParamCount() == 0) {
        ret += "void";
    }
    else {
        for(zfindex i = 0; i < this->methodParamCount(); ++i) {
            if(i > 0) {
                ret += ", ";
            }
            if(!zfstringIsEmpty(this->methodParamTypeIdAt(i))) {
                ret += this->methodParamTypeIdAt(i);
                ret += " ";
            }

            ret += this->methodParamNameAt(i);

            if(this->methodParamDefaultValueCallbackAt(i) != zfnull) {
                ZFArgs zfargs;
                zfargs.result(ZFMP_DEF());
                this->methodParamDefaultValueCallbackAt(i).execute(zfargs);
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
    zfindex methodParamCount;
    zfstring methodParamTypeId[ZFMETHOD_MAX_PARAM];
    zfstring methodParamName[ZFMETHOD_MAX_PARAM];
    ZFListener methodParamDefaultValueCallback[ZFMETHOD_MAX_PARAM];
public:
    _ZFP_ZFMPPrivate(void)
    : refCount(1)
    , methodParamCount(0)
    , methodParamTypeId()
    , methodParamName()
    , methodParamDefaultValueCallback()
    {
    }
};

ZFMP &ZFMP::mp(
        ZF_IN const zfstring &methodParamTypeId
        , ZF_IN_OPT const zfstring &methodParamName /* = zfnull */
        , ZF_IN_OPT const ZFListener &methodParamDefaultValueCallback /* = _ZFP_ZFMethod_paramDefaultValueCallbackDummy() */
        ) {
    ZFCoreAssert(d->methodParamCount <= ZFMETHOD_MAX_PARAM);
    d->methodParamTypeId[d->methodParamCount] = methodParamTypeId;
    d->methodParamName[d->methodParamCount] = methodParamName;
    d->methodParamDefaultValueCallback[d->methodParamCount] = methodParamDefaultValueCallback;
    ++(d->methodParamCount);
    return *this;
}

zfindex ZFMP::methodParamCount(void) const {
    return d->methodParamCount;
}
const zfstring &ZFMP::methodParamTypeIdAt(ZF_IN zfindex index) const {
    return d->methodParamTypeId[index];
}
const zfstring &ZFMP::methodParamNameAt(ZF_IN zfindex index) const {
    return d->methodParamName[index];
}
const ZFListener &ZFMP::methodParamDefaultValueCallbackAt(ZF_IN zfindex index) const {
    return d->methodParamDefaultValueCallback[index];
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
    for(zfindex i = 0; i < d->methodParamCount; ++i) {
        if(i > 0) {
            ret += ", ";
        }
        ret += d->methodParamTypeId[i];
        ret += " ";
        if(d->methodParamName[i].isEmpty()) {
            zfstringAppend(ret, "p%s", i);
        }
        else {
            ret += d->methodParamName[i];
        }
        ZFArgs zfargs;
        zfargs.result(ZFMP_DEF());
        d->methodParamDefaultValueCallback[i].execute(zfargs);
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
        , ZFMP_IN(const ZFClass *, methodOwnerClass)
        , ZFMP_IN(const zfstring &, methodReturnTypeId)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_IN_OPT(ZFMethodType, methodType, ZFMethodTypeVirtual)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, methodPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodDynamicRegister
        , ZFMP_IN(const zfstring &, methodNamespace)
        , ZFMP_IN(const zfstring &, methodReturnTypeId)
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

