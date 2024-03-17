#include "ZFMethod.h"
#include "ZFObjectImpl.h"

#include "ZFMethodDeclare.h"
#include "ZFMethodUserRegister.h"

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFMethod::methodUserRegisterUserData(ZF_IN ZFObject *methodUserRegisterUserData) const {
    zfCoreAssertWithMessage(this->methodIsUserRegister(),
        "methodUserRegisterUserData can only be changed for user registered method");
    zfRetainChange(this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodUserRegisterUserData, methodUserRegisterUserData);
}

void ZFMethod::_ZFP_ZFMethod_init(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType invoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        , ...
        ) {
    zfCoreAssert(methodGenericInvoker != zfnull);

    this->_ZFP_ZFMethod_methodIsUserRegister = methodIsUserRegister;
    this->_ZFP_ZFMethod_methodUserRegisterUserData = zfnull;
    this->_ZFP_ZFMethod_methodIsDynamicRegister = methodIsDynamicRegister;
    this->_ZFP_ZFMethod_methodDynamicRegisterUserData = zfRetain(methodDynamicRegisterUserData);
    this->_ZFP_ZFMethod_invoker = invoker;
    this->_ZFP_ZFMethod_invokerOrg = invoker;
    this->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodGenericInvokerOrg = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodType = methodType;
    this->_ZFP_ZFMethod_methodName = methodName;
    this->_ZFP_ZFMethod_returnTypeId = returnTypeId;
    this->_ZFP_ZFMethod_returnTypeName = returnTypeName;

    const zfchar *paramTypeIdList[ZFMETHOD_MAX_PARAM];
    const zfchar *paramTypeNameList[ZFMETHOD_MAX_PARAM];
    const zfchar *paramNameList[ZFMETHOD_MAX_PARAM];
    ZFMethodParamDefaultValueCallback paramDefaultValueCallbackList[ZFMETHOD_MAX_PARAM];

    zfuint paramCount = 0;
    va_list vaList;
    va_start(vaList, returnTypeName);
    do {
        const zfchar *paramTypeId = va_arg(vaList, const zfchar *);
        if(paramTypeId == zfnull || paramTypeId[0] == '\0') {
            break;
        }
        paramTypeIdList[paramCount] = paramTypeId;
        paramTypeNameList[paramCount] = va_arg(vaList, const zfchar *);
        paramNameList[paramCount] = va_arg(vaList, const zfchar *);

        paramDefaultValueCallbackList[paramCount] = va_arg(vaList, ZFMethodParamDefaultValueCallback);
        if(paramDefaultValueCallbackList[paramCount] != zfnull) {
            if(this->_ZFP_ZFMethod_paramDefaultBeginIndex == (zfuint)-1) {
                this->_ZFP_ZFMethod_paramDefaultBeginIndex = paramCount;
            }
        }

        ++paramCount;
    } while(zftrue);
    va_end(vaList);

    _ZFP_ZFMethod_paramCount = paramCount;
    if(_ZFP_ZFMethod_paramDefaultBeginIndex == (zfuint)-1) {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramCount;
    }
    else {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramDefaultBeginIndex;
    }

    // store param data
    if(paramCount > 0) {
        _ZFP_ZFMethod_paramBuf = (ZFSigName *)zfmalloc((ZFMETHOD_MAX_PARAM + paramCount * 2) * sizeof(ZFSigName));

        _ZFP_ZFMethod_paramTypeIdList = _ZFP_ZFMethod_paramBuf;
        _ZFP_ZFMethod_paramTypeNameList = _ZFP_ZFMethod_paramTypeIdList + ZFMETHOD_MAX_PARAM;
        _ZFP_ZFMethod_paramNameList = _ZFP_ZFMethod_paramTypeNameList + paramCount;

        for(zfuint i = 0; i < paramCount; ++i) {
            zfnewPlacement(_ZFP_ZFMethod_paramTypeIdList + i, ZFSigName, paramTypeIdList[i]);
            zfnewPlacement(_ZFP_ZFMethod_paramTypeNameList + i, ZFSigName, paramTypeNameList[i]);
            zfnewPlacement(_ZFP_ZFMethod_paramNameList + i, ZFSigName, paramNameList[i]);
        }
        for(zfuint i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
            zfnewPlacement(_ZFP_ZFMethod_paramTypeIdList + i, ZFSigName);
        }

        _ZFP_ZFMethod_paramDefaultValueCallbackList = (ZFMethodParamDefaultValueCallback *)zfmalloc(sizeof(ZFMethodParamDefaultValueCallback) * paramCount);
        zfmemcpy(_ZFP_ZFMethod_paramDefaultValueCallbackList, paramDefaultValueCallbackList, sizeof(ZFMethodParamDefaultValueCallback) * paramCount);
    }

    // internal
    static const zfchar *_ZFP_ = "_ZFP_";
    static zfindex _ZFP_len = zfslen(_ZFP_);
    static const zfchar *_ZFP_I_ = "_ZFP_I_";
    static zfindex _ZFP_I_len = zfslen(_ZFP_I_);
    if(zfsncmp(methodName, _ZFP_I_, _ZFP_I_len) == 0) {
        this->_ZFP_ZFMethod_methodIsInternal = zftrue;
        this->_ZFP_ZFMethod_methodIsInternalPrivate = zftrue;
    }
    else if(zfsncmp(methodName, _ZFP_, _ZFP_len) == 0) {
        this->_ZFP_ZFMethod_methodIsInternal = zftrue;
        this->_ZFP_ZFMethod_methodIsInternalPrivate = zffalse;
    }
    else {
        this->_ZFP_ZFMethod_methodIsInternal = zffalse;
        this->_ZFP_ZFMethod_methodIsInternalPrivate = zffalse;
    }
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(
        ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType privilegeType
        ) {
    this->_ZFP_ZFMethod_methodOwnerClass = methodOwnerClass;
    this->_ZFP_ZFMethod_privilegeType = privilegeType;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfchar *methodNamespace) {
    if(!zfstringIsEmpty(methodNamespace)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
    }

    this->_ZFP_ZFMethod_methodOwnerClass = zfnull;
    this->_ZFP_ZFMethod_privilegeType = ZFMethodPrivilegeTypePublic;
}

ZFMethod::ZFMethod(void)
: _ZFP_ZFMethodNeedInit(zftrue)
, _ZFP_ZFMethod_refCount(1)
, _ZFP_ZFMethod_methodAliasFrom(zfnull)
, _ZFP_ZFMethod_methodAliasTo()
, _ZFP_ZFMethod_methodInternalId(zfnull)
, _ZFP_ZFMethod_methodIsUserRegister(zffalse)
, _ZFP_ZFMethod_methodUserRegisterUserData(zfnull)
, _ZFP_ZFMethod_methodIsDynamicRegister(zffalse)
, _ZFP_ZFMethod_methodDynamicRegisterUserData(zfnull)
, _ZFP_ZFMethod_invoker(zfnull)
, _ZFP_ZFMethod_invokerOrg(zfnull)
, _ZFP_ZFMethod_methodGenericInvoker(zfnull)
, _ZFP_ZFMethod_methodGenericInvokerOrg(zfnull)
, _ZFP_ZFMethod_methodName(zfnull)
, _ZFP_ZFMethod_returnTypeId(zfnull)
, _ZFP_ZFMethod_returnTypeName(zfnull)
, _ZFP_ZFMethod_paramCount(0)
, _ZFP_ZFMethod_paramCountMin(0)
, _ZFP_ZFMethod_paramBuf(zfnull)
, _ZFP_ZFMethod_paramTypeIdList(zfnull)
, _ZFP_ZFMethod_paramTypeNameList(zfnull)
, _ZFP_ZFMethod_paramNameList(zfnull)
, _ZFP_ZFMethod_paramDefaultValueCallbackList(zfnull)
, _ZFP_ZFMethod_paramDefaultValueList()
, _ZFP_ZFMethod_paramDefaultBeginIndex((zfuint)-1)
, _ZFP_ZFMethod_methodOwnerClass(zfnull)
, _ZFP_ZFMethod_methodOwnerProperty(zfnull)
, _ZFP_ZFMethod_privilegeType(ZFMethodPrivilegeTypePublic)
, _ZFP_ZFMethod_methodType(ZFMethodTypeNormal)
, _ZFP_ZFMethod_methodNamespace(zfnull)
, _ZFP_ZFMethod_methodIsInternal(zffalse)
, _ZFP_ZFMethod_methodIsInternalPrivate(zffalse)
{
}
ZFMethod::~ZFMethod(void) {
    zfRelease(this->_ZFP_ZFMethod_methodUserRegisterUserData);
    zfRelease(this->_ZFP_ZFMethod_methodDynamicRegisterUserData);

    zffree(this->_ZFP_ZFMethod_methodInternalId);

    // registered by ZFSigName, no need to free
    // _ZFP_ZFMethod_methodName;
    // _ZFP_ZFMethod_returnTypeId;
    // _ZFP_ZFMethod_returnTypeName;
    // _ZFP_ZFMethod_methodNamespace;
    // _ZFP_ZFMethod_paramTypeIdList;
    // _ZFP_ZFMethod_paramTypeNameList;

    if(_ZFP_ZFMethod_paramBuf != zfnull) {
        for(zfindex i = 0, count = ZFMETHOD_MAX_PARAM + methodParamCount() * 2; i < count; ++i) {
            zfdeletePlacement(_ZFP_ZFMethod_paramBuf + i);
        }
        zffree(_ZFP_ZFMethod_paramBuf);
    }
    // _ZFP_ZFMethod_paramTypeIdList; // fixed lengh for performance, ZFMETHOD_MAX_PARAM
    // _ZFP_ZFMethod_paramTypeNameList; // depends on paramCount
    // _ZFP_ZFMethod_paramNameList; // depends on paramCount

    zffree(_ZFP_ZFMethod_paramDefaultValueCallbackList);
}

void ZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    switch(this->methodPrivilegeType()) {
        case ZFMethodPrivilegeTypePublic:
            break;
        case ZFMethodPrivilegeTypeProtected:
            ret += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
            ret += ": ";
            break;
        case ZFMethodPrivilegeTypePrivate:
            ret += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
            ret += ": ";
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }

    if(this->methodType() == ZFMethodTypeStatic) {
        ret += "static ";
    }
    else if(this->methodType() == ZFMethodTypeVirtual) {
        ret += "virtual ";
    }

    ret += this->methodReturnTypeName();
    ret += " ";

    if(this->methodIsFunctionType()) {
        if(!zfstringIsEmpty(this->methodNamespace())) {
            ret += this->methodNamespace();
            ret += "::";
        }
    }
    else {
        ret += this->methodOwnerClass()->classNameFull();
        ret += "::";
    }

    ret += this->methodName();

    if(this->methodParamCount() > 0) {
        ret += "(";
        for(zfindex i = 0; i < this->methodParamCount(); ++i) {
            if(i != 0) {
                ret += ", ";
            }
            ret += this->methodParamTypeNameAt(i);
            ret += " ";
            ret += this->methodParamNameAt(i);
            if(i >= this->methodParamDefaultBeginIndex()) {
                ret += " = ";
                zfauto v = this->methodParamDefaultValueAt(i);
                if(v == zfnull) {
                    ret += ZFTOKEN_zfnull;
                }
                else {
                    v.toObject()->objectInfoT(ret);
                }
            }
        }
        ret += ")";
    }
    else {
        ret += "(void)";
    }
}

zfbool ZFMethod::methodParamTypeIdIsMatch(
        ZF_IN_OPT const zfchar *methodParamTypeId0 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
        ) const {
    #define _ZFP_ZFMethodParamCheck_paramLoop(N) \
        if(zfstringIsEmpty(methodParamTypeId##N) || zfstringIsEqual(methodParamTypeId##N, ZFTypeId_void())) {return (this->methodParamCount() == N);} \
        if(this->methodParamCount() <= N || !zfstringIsEqual(this->methodParamTypeIdAt(N), methodParamTypeId##N)) {return zffalse;}
    _ZFP_ZFMethodParamCheck_paramLoop(0)
    _ZFP_ZFMethodParamCheck_paramLoop(1)
    _ZFP_ZFMethodParamCheck_paramLoop(2)
    _ZFP_ZFMethodParamCheck_paramLoop(3)
    _ZFP_ZFMethodParamCheck_paramLoop(4)
    _ZFP_ZFMethodParamCheck_paramLoop(5)
    _ZFP_ZFMethodParamCheck_paramLoop(6)
    _ZFP_ZFMethodParamCheck_paramLoop(7)
    #undef _ZFP_ZFMethodParamCheck_paramLoop
    return zftrue;
}
zfbool ZFMethod::methodParamTypeIdIsMatch(ZF_IN const ZFMethod *method) const {
    if(method->methodParamCount() != this->methodParamCount()
            || method->methodParamDefaultBeginIndex() != this->methodParamDefaultBeginIndex()
            ) {
        return zffalse;
    }
    return this->methodParamCount() == 0 || this->methodParamTypeIdIsMatch(
            method->_ZFP_ZFMethod_paramTypeIdList[0]
            , method->_ZFP_ZFMethod_paramTypeIdList[1]
            , method->_ZFP_ZFMethod_paramTypeIdList[2]
            , method->_ZFP_ZFMethod_paramTypeIdList[3]
            , method->_ZFP_ZFMethod_paramTypeIdList[4]
            , method->_ZFP_ZFMethod_paramTypeIdList[5]
            , method->_ZFP_ZFMethod_paramTypeIdList[6]
            , method->_ZFP_ZFMethod_paramTypeIdList[7]
        );
}

zfauto ZFMethod::methodGenericInvoke(
        ZF_IN_OPT ZFObject *ownerObjOrNull /* = zfnull */
        , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    zfauto ret;
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    paramList[0].zfunsafe_assign(param0);
    paramList[1].zfunsafe_assign(param1);
    paramList[2].zfunsafe_assign(param2);
    paramList[3].zfunsafe_assign(param3);
    paramList[4].zfunsafe_assign(param4);
    paramList[5].zfunsafe_assign(param5);
    paramList[6].zfunsafe_assign(param6);
    paramList[7].zfunsafe_assign(param7);

    zfbool t = this->methodGenericInvoker()(this, ownerObjOrNull, errorHint, ret, paramList);
    if(success != zfnull) {
        *success = t;
    }
    return ret;
}

void ZFMethod::methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const {
    zfCoreMutexLocker();
    ZFMethod *m = this->_ZFP_ZFMethod_removeConst();
    if(methodGenericInvoker != zfnull) {
        m->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    }
    else {
        m->_ZFP_ZFMethod_methodGenericInvoker = m->_ZFP_ZFMethod_methodGenericInvokerOrg;
    }
}

void ZFMethod::methodInvoker(ZF_IN ZFFuncAddrType methodInvoker) const {
    this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_invoker =
        ((methodInvoker != zfnull) ? methodInvoker : this->_ZFP_ZFMethod_invokerOrg);
}

// ============================================================
typedef zfstlhashmap<const zfchar *, ZFMethod *, zfcharConst_zfstlHasher, zfcharConst_zfstlHashComparer> _ZFP_ZFMethodMapType;
static _ZFP_ZFMethodMapType &_ZFP_ZFMethodMap(void) {
    static _ZFP_ZFMethodMapType m;
    return m;
}

void _ZFP_ZFMethodDataHolderInit(void) {
    _ZFP_ZFMethodMap();
}

// ============================================================
static void _ZFP_ZFMethodInstanceSig(
        ZF_OUT zfstring &ret
        , ZF_IN const ZFSigName &methodScope
        , ZF_IN const ZFSigName &methodName
        , ZF_IN_OPT const ZFSigName &methodParamTypeId0 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId1 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId2 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId3 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId4 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId5 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId6 /* = zfnull */
        , ZF_IN_OPT const ZFSigName &methodParamTypeId7 /* = zfnull */
        ) {
    if(!zfstringIsEmpty(methodScope)
        && !zfstringIsEqual(methodScope, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodScope, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        zfindexToString(ret, methodScope.sigId());
    }
    ret += ':';
    zfindexToString(ret, methodName.sigId());

    if(zfstringIsEmpty(methodParamTypeId0)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId0.sigId());
    if(zfstringIsEmpty(methodParamTypeId1)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId1.sigId());
    if(zfstringIsEmpty(methodParamTypeId2)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId2.sigId());
    if(zfstringIsEmpty(methodParamTypeId3)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId3.sigId());
    if(zfstringIsEmpty(methodParamTypeId4)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId4.sigId());
    if(zfstringIsEmpty(methodParamTypeId5)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId5.sigId());
    if(zfstringIsEmpty(methodParamTypeId6)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId6.sigId());
    if(zfstringIsEmpty(methodParamTypeId7)) {return;} ret += '+'; zfindexToString(ret, methodParamTypeId7.sigId());
}

static ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfchar *methodInternalId) {
    zfCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodInternalId);
    if(it != m.end()) {
        ZFMethod *v = it->second;
        ++(v->_ZFP_ZFMethod_refCount);
        return v;
    }
    else {
        ZFMethod *v = zfnew(ZFMethod);
        v->_ZFP_ZFMethod_methodInternalId = zfsCopy(methodInternalId);
        m[v->methodInternalId()] = v;
        return v;
    }
}

ZFMethod *_ZFP_ZFMethodRegister(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ...
        ) {
    va_list vaList;
    va_start(vaList, returnTypeName);
    ZFMethod *method = _ZFP_ZFMethodRegisterV(methodIsUserRegister
            , methodIsDynamicRegister
            , methodDynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , methodOwnerClass
            , methodPrivilegeType
            , methodNamespace
            , methodName
            , returnTypeId
            , returnTypeName
            , vaList
        );
    va_end(vaList);
    return method;
}
ZFMethod *_ZFP_ZFMethodRegisterV(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ZF_IN va_list vaList
        ) {
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);

    zfCoreAssert(methodGenericInvoker != zfnull);
    zfCoreAssert(methodOwnerClass == zfnull || zfstringIsEmpty(methodNamespace));
    zfCoreAssert(methodName != zfnull && *methodName != '\0');
    zfCoreAssert(returnTypeId != zfnull && *returnTypeId != '\0');
    zfCoreAssert(returnTypeName != zfnull && *returnTypeName != '\0');

    ZFMethod *method = zfnull;

    zfindex paramCount = 0;
    ZFSigName paramTypeId[ZFMETHOD_MAX_PARAM];
    const zfchar *paramTypeName[ZFMETHOD_MAX_PARAM] = {0};
    const zfchar *paramName[ZFMETHOD_MAX_PARAM] = {0};
    ZFMethodParamDefaultValueCallback paramDefaultValueAccess[ZFMETHOD_MAX_PARAM] = {0};
    paramTypeId[paramCount] = va_arg(vaList, const zfchar *);
    while(paramTypeId[paramCount] != zfnull) {
        paramTypeName[paramCount] = va_arg(vaList, const zfchar *);
        paramName[paramCount] = va_arg(vaList, const zfchar *);
        paramDefaultValueAccess[paramCount] = va_arg(vaList, ZFMethodParamDefaultValueCallback);
        ++paramCount;
        if(paramCount >= ZFMETHOD_MAX_PARAM) {
            break;
        }
        paramTypeId[paramCount] = va_arg(vaList, const zfchar *);
    }

    zfstring methodInternalId;
    ZFSigName methodScope = methodOwnerClass ? methodOwnerClass->classNameFull() : methodNamespace;
    ZFSigName methodNameSig = methodName;
    _ZFP_ZFMethodInstanceSig(methodInternalId
            , methodScope
            , methodNameSig
            , paramTypeId[0]
            , paramTypeId[1]
            , paramTypeId[2]
            , paramTypeId[3]
            , paramTypeId[4]
            , paramTypeId[5]
            , paramTypeId[6]
            , paramTypeId[7]
        );

    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodInternalId);
    if(it != m.end()) {
        method = it->second;
        if(method->methodIsUserRegister()) {
            if(methodOwnerClass != zfnull) {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodUserRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId);
            }
            else {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodFuncUserRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespace,
                    methodName,
                    methodInternalId);
            }
        }
        else if(method->methodIsDynamicRegister()) {
            if(methodOwnerClass != zfnull) {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId);
            }
            else {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespace,
                    methodName,
                    methodInternalId);
            }
        }

        // param type id may be same for different cpp type
        // strict check type name to prevent redefine
        zfbool isRedefine = (paramCount != method->methodParamCount());
        if(!isRedefine) {
            for(zfindex i = 0; i < paramCount; ++i) {
                if(!zfstringIsEqual(paramTypeName[i], method->methodParamTypeNameAt(i))) {
                    isRedefine = zftrue;
                    break;
                }
            }
        }
        zfCoreAssertWithMessageTrim(!isRedefine,
            "[ZFMethod] redefine of method: %s, existing: %s",
            methodInternalId,
            method);

        ++method->_ZFP_ZFMethod_refCount;
    }
    else {
        method = _ZFP_ZFMethodInstanceAccess(methodInternalId);
    }

    if(method->_ZFP_ZFMethodNeedInit) {
        method->_ZFP_ZFMethodNeedInit = zffalse;
        method->_ZFP_ZFMethod_init(methodIsUserRegister
                , methodIsDynamicRegister
                , methodDynamicRegisterUserData
                , methodInvoker
                , methodGenericInvoker
                , methodType
                , methodName
                , returnTypeId
                , returnTypeName
                , paramTypeId[0].cString(), paramTypeName[0], paramName[0], paramDefaultValueAccess[0]
                , paramTypeId[1].cString(), paramTypeName[1], paramName[1], paramDefaultValueAccess[1]
                , paramTypeId[2].cString(), paramTypeName[2], paramName[2], paramDefaultValueAccess[2]
                , paramTypeId[3].cString(), paramTypeName[3], paramName[3], paramDefaultValueAccess[3]
                , paramTypeId[4].cString(), paramTypeName[4], paramName[4], paramDefaultValueAccess[4]
                , paramTypeId[5].cString(), paramTypeName[5], paramName[5], paramDefaultValueAccess[5]
                , paramTypeId[6].cString(), paramTypeName[6], paramName[6], paramDefaultValueAccess[6]
                , paramTypeId[7].cString(), paramTypeName[7], paramName[7], paramDefaultValueAccess[7]
                , (const zfchar *)zfnull
            );

        if(methodOwnerClass != zfnull) {
            method->_ZFP_ZFMethod_initClassMemberType(methodOwnerClass, methodPrivilegeType);
            methodOwnerClass->_ZFP_ZFClass_methodRegister(method);
            _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, zfnull, zfnull, method);
        }
        else {
            method->_ZFP_ZFMethod_initFuncType(methodNamespace);
            _ZFP_ZFMethodFuncRegister(method);
            _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, zfnull, zfnull, method);
        }
    }

    return method;
}
void _ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method) {
    zfCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(method->methodInternalId());
    if(it == m.end()) {
        return;
    }

    ZFMethod *v = it->second;
    --(v->_ZFP_ZFMethod_refCount);
    if(v->_ZFP_ZFMethod_refCount != 0) {
        return;
    }
    m.erase(it);

    if(v->methodIsFunctionType()) {
        _ZFP_ZFMethodFuncUnregister(v);
    }
    else {
        if(v->methodIsUserRegister() || v->methodIsDynamicRegister()) {
            v->methodOwnerClass()->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodUnregister(v);
        }
    }
    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeDetach, zfnull, zfnull, v);

    zfdelete(v);
}

_ZFP_ZFMethodRegisterHolder::_ZFP_ZFMethodRegisterHolder(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ...
        )
: method(zfnull)
{
    va_list vaList;
    va_start(vaList, returnTypeName);
    method = _ZFP_ZFMethodRegisterV(methodIsUserRegister
            , methodIsDynamicRegister
            , methodDynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , methodOwnerClass
            , methodPrivilegeType
            , methodNamespace
            , methodName
            , returnTypeId
            , returnTypeName
            , vaList
        );
    va_end(vaList);
}
_ZFP_ZFMethodRegisterHolder::_ZFP_ZFMethodRegisterHolder(
        ZF_IN zfbool dummy
        , ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ZF_IN va_list vaList
        )
: method(_ZFP_ZFMethodRegisterV(
            methodIsUserRegister
            , methodIsDynamicRegister
            , methodDynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , methodOwnerClass
            , methodPrivilegeType
            , methodNamespace
            , methodName
            , returnTypeId
            , returnTypeName
            , vaList
            )) {
}
_ZFP_ZFMethodRegisterHolder::~_ZFP_ZFMethodRegisterHolder(void) {
    _ZFP_ZFMethodUnregister(this->method);
}

// ============================================================
void ZFMethodGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN_OPT const ZFFilterForZFMethod *methodFilter /* = zfnull */
        ) {
    zfCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    if(methodFilter != zfnull) {
        for(_ZFP_ZFMethodMapType::iterator it = m.begin(); it != m.end(); ++it) {
            ZFMethod *v = it->second;
            if(methodFilter->filterCheckActive(v)) {
                ret.add(v);
            }
        }
    }
    else {
        for(_ZFP_ZFMethodMapType::iterator it = m.begin(); it != m.end(); ++it) {
            ZFMethod *v = it->second;
            ret.add(v);
        }
    }
}

// ============================================================
/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFMethodForName(
        ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        ) {
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull) {
        return cls->methodForName(methodName);
    }
    else {
        return ZFMethodFuncForName(classNameOrNamespace, methodName);
    }
}
const ZFMethod *ZFMethodForName(
        ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN_OPT const zfchar *methodParamTypeId0
        , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
        ) {
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull) {
        return cls->methodForName(methodName
                , methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            );
    }
    else {
        return ZFMethodFuncForName(classNameOrNamespace, methodName
                , methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            );
    }
}
void ZFMethodForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        ) {
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull) {
        return cls->methodForNameGetAllT(ret, methodName);
    }
    else {
        return ZFMethodFuncForNameGetAllT(ret, classNameOrNamespace, methodName);
    }
}

// ============================================================
const ZFMethod *ZFMethodAlias(
        ZF_IN const ZFMethod *method
        , ZF_IN const zfchar *aliasName
        ) {
    zfCoreMutexLocker();
    if(method == zfnull || zfstringIsEmpty(aliasName)) {
        return zfnull;
    }
    ZFCoreArrayPOD<const ZFMethod *> exist;
    if(method->methodOwnerClass() != zfnull) {
        method->methodOwnerClass()->methodForNameGetAllT(exist, aliasName);
    }
    else {
        ZFMethodFuncForNameGetAllT(exist, method->methodNamespace(), aliasName);
    }
    if(!exist.isEmpty()) {
        for(zfindex i = 0; i < exist.count(); ++i) {
            if(exist[i]->methodParamTypeIdIsMatch(method)) {
                return exist[i];
            }
        }
    }

    const zfchar *paramTypeId[ZFMETHOD_MAX_PARAM] = {0};
    const zfchar *paramTypeName[ZFMETHOD_MAX_PARAM] = {0};
    const zfchar *paramName[ZFMETHOD_MAX_PARAM] = {0};
    ZFMethodParamDefaultValueCallback paramDefaultValueCallback[ZFMETHOD_MAX_PARAM] = {0};
    for(zfindex i = 0; i < method->methodParamCount(); ++i) {
        paramTypeId[i] = method->methodParamTypeIdAt(i);
        paramTypeName[i] = method->methodParamTypeNameAt(i);
        paramName[i] = method->methodParamNameAt(i);
        paramDefaultValueCallback[i] = method->methodParamDefaultValueCallbackAt(i);
    }
    if(method->methodParamCount() != ZFMETHOD_MAX_PARAM) {
        paramTypeId[method->methodParamCount()] = zfnull;
    }
    ZFMethod *ret = _ZFP_ZFMethodRegister(
            method->methodIsUserRegister()
            , method->methodIsDynamicRegister()
            , method->methodDynamicRegisterUserData()
            , method->methodInvokerOrg()
            , method->methodGenericInvokerOrg()
            , method->methodType()
            , method->methodOwnerClass()
            , method->methodPrivilegeType()
            , method->methodNamespace()
            , aliasName
            , method->methodReturnTypeId()
            , method->methodReturnTypeName()
            , paramTypeId[0], paramTypeName[0], paramName[0], paramDefaultValueCallback[0]
            , paramTypeId[1], paramTypeName[1], paramName[1], paramDefaultValueCallback[1]
            , paramTypeId[2], paramTypeName[2], paramName[2], paramDefaultValueCallback[2]
            , paramTypeId[3], paramTypeName[3], paramName[3], paramDefaultValueCallback[3]
            , paramTypeId[4], paramTypeName[4], paramName[4], paramDefaultValueCallback[4]
            , paramTypeId[5], paramTypeName[5], paramName[5], paramDefaultValueCallback[5]
            , paramTypeId[6], paramTypeName[6], paramName[6], paramDefaultValueCallback[6]
            , paramTypeId[7], paramTypeName[7], paramName[7], paramDefaultValueCallback[7]
            , zfnull
            );
    if(ret == zfnull) {
        return zfnull;
    }
    ret->_ZFP_ZFMethod_methodAliasFrom = method;
    method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodAliasTo.add(ret);
    return ret;
}
static void _ZFP_ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    if(aliasMethod == zfnull || aliasMethod->methodAliasFrom() == zfnull) {
        return;
    }
    ZFMethod *from = aliasMethod->methodAliasFrom()->_ZFP_ZFMethod_removeConst();
    ZFMethod *to = aliasMethod->_ZFP_ZFMethod_removeConst();
    from->_ZFP_ZFMethod_methodAliasTo.removeElement(to);
    to->_ZFP_ZFMethod_methodAliasFrom = zfnull;

    if(!to->methodAliasTo().isEmpty()) {
        ZFCoreArrayPOD<const ZFMethod *> chain = to->_ZFP_ZFMethod_methodAliasTo;
        to->_ZFP_ZFMethod_methodAliasTo = ZFCoreArrayPOD<const ZFMethod *>();

        for(zfindex i = 0; i < chain.count(); ++i) {
            ZFMethod *chainTmp = chain[i]->_ZFP_ZFMethod_removeConst();
            chainTmp->_ZFP_ZFMethod_methodAliasFrom = zfnull;
            _ZFP_ZFMethodAliasRemove(chainTmp);
        }
    }

    _ZFP_ZFMethodUnregister(aliasMethod);
}
void ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    zfCoreMutexLocker();
    _ZFP_ZFMethodAliasRemove(aliasMethod);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFMethodGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        , ZFMP_IN_OPT(const ZFFilterForZFMethod *, methodFilter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodGetAll
        , ZFMP_IN_OPT(const ZFFilterForZFMethod *, methodFilter, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodForName
        , ZFMP_IN(const zfchar *, classNameOrNamespace)
        , ZFMP_IN(const zfchar *, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodForName
        , ZFMP_IN(const zfchar *, classNameOrNamespace)
        , ZFMP_IN(const zfchar *, methodName)
        , ZFMP_IN(const zfchar *, methodParamTypeId0)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull) */
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFMethodForNameGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        , ZFMP_IN(const zfchar *, classNameOrNamespace)
        , ZFMP_IN(const zfchar *, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodForNameGetAll
        , ZFMP_IN(const zfchar *, classNameOrNamespace)
        , ZFMP_IN(const zfchar *, methodName)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodAlias
        , ZFMP_IN(const ZFMethod *, method)
        , ZFMP_IN(const zfchar *, aliasName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodAliasRemove
        , ZFMP_IN(const ZFMethod *, aliasMethod)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

