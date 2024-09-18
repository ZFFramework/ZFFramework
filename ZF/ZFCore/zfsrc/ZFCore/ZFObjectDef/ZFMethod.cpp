#include "ZFMethod.h"
#include "ZFObjectImpl.h"

#include "ZFMethodDeclare.h"
#include "ZFMethodUserRegister.h"

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"

#include "ZFDynamicInvoker.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

// #define _ZFP_ZFMethod_DEBUG 1

#if _ZFP_ZFMethod_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFMethod_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFMtd] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFMethod_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFListener &_ZFP_ZFMethod_paramDefaultValueCallbackDummy(void) {
    static ZFListener d;
    return d;
}
_ZFP_ZFMethodMP &_ZFP_ZFMethodMP::add(
        ZF_IN const zfstring &paramTypeId
        , ZF_IN const zfstring &paramName
        , ZF_IN const ZFListener &paramDefaultValueCallback
        ) {
    this->paramTypeId[this->paramCount] = paramTypeId;
    this->paramName[this->paramCount] = paramName;
    this->paramDefaultValueCallback[this->paramCount] = zfpoolNew(ZFListener, paramDefaultValueCallback);
    ++this->paramCount;
    return *this;
}
_ZFP_ZFMethodMP::~_ZFP_ZFMethodMP(void) {
    for(zfindex i = 0; i < this->paramCount; ++i) {
        zfpoolDelete(this->paramDefaultValueCallback[i]);
    }
}

// ============================================================
zfclassNotPOD _ZFP_ZFMethodPrivateExt {
public:
    const ZFMethod *methodAliasFrom;
    ZFCoreArray<const ZFMethod *> methodAliasTo;
    ZFFuncAddrType invokerOrig;
    ZFMethodGenericInvoker methodGenericInvokerOrig;
public:
    _ZFP_ZFMethodPrivateExt(void)
    : methodAliasFrom(zfnull)
    , methodAliasTo()
    , invokerOrig(zfnull)
    , methodGenericInvokerOrig(zfnull)
    {
    }
};

const ZFMethod *ZFMethod::methodAliasFrom(void) const {
    return _ZFP_ZFMethod_ext ? _ZFP_ZFMethod_ext->methodAliasFrom : zfnull;
}
const ZFCoreArray<const ZFMethod *> &ZFMethod::methodAliasTo(void) const {
    static ZFCoreArray<const ZFMethod *> dummy;
    return _ZFP_ZFMethod_ext ? _ZFP_ZFMethod_ext->methodAliasTo : dummy;
}

ZFFuncAddrType ZFMethod::methodInvokerOrig(void) const {
    return _ZFP_ZFMethod_ext && _ZFP_ZFMethod_ext->invokerOrig
        ? _ZFP_ZFMethod_ext->invokerOrig
        : _ZFP_ZFMethod_invoker
        ;
}
ZFMethodGenericInvoker ZFMethod::methodGenericInvokerOrig(void) const {
    return _ZFP_ZFMethod_ext && _ZFP_ZFMethod_ext->methodGenericInvokerOrig
        ? _ZFP_ZFMethod_ext->methodGenericInvokerOrig
        : _ZFP_ZFMethod_methodGenericInvoker
        ;
}

// ============================================================
void ZFMethod::methodUserRegisterUserData(ZF_IN ZFObject *methodUserRegisterUserData) const {
    ZFCoreAssertWithMessage(this->methodIsUserRegister(),
        "methodUserRegisterUserData can only be changed for user registered method");
    zfRetainChange(this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodUserData, methodUserRegisterUserData);
}

void ZFMethod::_ZFP_ZFMethod_init(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType invoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    ZFCoreAssert(methodGenericInvoker != zfnull);

    this->_ZFP_ZFMethod_methodIsUserRegister = methodIsUserRegister;
    this->_ZFP_ZFMethod_methodIsDynamicRegister = methodIsDynamicRegister;
    this->_ZFP_ZFMethod_methodUserData = zfRetain(methodDynamicRegisterUserData);
    this->_ZFP_ZFMethod_invoker = invoker;
    this->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodType = (unsigned short)methodType;
    this->_ZFP_ZFMethod_methodName = methodName;
    this->_ZFP_ZFMethod_returnTypeId = returnTypeId;

    // update param count
    for(zfuint i = 0; i < mp.paramCount; ++i) {
        if(*(mp.paramDefaultValueCallback[i]) != zfnull) {
            _ZFP_ZFMethod_paramDefaultBeginIndex = (zfuint)i;
            break;
        }
    }
    _ZFP_ZFMethod_paramCount = mp.paramCount;
    if(_ZFP_ZFMethod_paramDefaultBeginIndex == (zfuint)-1) {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramCount;
    }
    else {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramDefaultBeginIndex;
    }

    // store param data
    if(mp.paramCount > 0) {
        _ZFP_ZFMethod_paramBuf = (ZFSigName *)zfmalloc((ZFMETHOD_MAX_PARAM + mp.paramCount) * sizeof(ZFSigName));

        ZFSigName *paramTypeIdList = _ZFP_ZFMethod_paramBuf;
        ZFSigName *paramNameList = paramTypeIdList + ZFMETHOD_MAX_PARAM;

        for(zfuint i = 0; i < mp.paramCount; ++i) {
            zfnewPlacement(paramTypeIdList + i, ZFSigName, mp.paramTypeId[i]);
            zfnewPlacement(paramNameList + i, ZFSigName, mp.paramName[i]);
        }
        for(zfuint i = mp.paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
            zfnewPlacement(paramTypeIdList + i, ZFSigName);
        }

        _ZFP_ZFMethod_paramDefaultValueCallbackList = (ZFListener *)zfmalloc(sizeof(ZFListener) * mp.paramCount);
        for(zfindex i = 0; i < mp.paramCount; ++i) {
            zfnewPlacement(_ZFP_ZFMethod_paramDefaultValueCallbackList + i, ZFListener, *(mp.paramDefaultValueCallback[i]));
        }
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
    this->_ZFP_ZFMethod_privilegeType = (unsigned short)privilegeType;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfstring &methodNamespace) {
    if(!zfstringIsEmpty(methodNamespace)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
    }

    this->_ZFP_ZFMethod_methodOwnerClass = zfnull;
    this->_ZFP_ZFMethod_privilegeType = (unsigned short)ZFMethodPrivilegeTypePublic;
}

ZFMethod::ZFMethod(void)
: _ZFP_ZFMethod_refCount(1)
, _ZFP_ZFMethod_methodIsInternal(zffalse)
, _ZFP_ZFMethod_methodIsInternalPrivate(zffalse)
, _ZFP_ZFMethod_methodIsUserRegister(zffalse)
, _ZFP_ZFMethod_methodIsDynamicRegister(zffalse)
, _ZFP_ZFMethod_methodUserData(zfnull)
, _ZFP_ZFMethod_methodInternalId(zfnull)
, _ZFP_ZFMethod_ext(zfnull)
, _ZFP_ZFMethod_invoker(zfnull)
, _ZFP_ZFMethod_methodGenericInvoker(zfnull)
, _ZFP_ZFMethod_methodName(zfnull)
, _ZFP_ZFMethod_returnTypeId(zfnull)
, _ZFP_ZFMethod_paramCount(0)
, _ZFP_ZFMethod_paramCountMin(0)
, _ZFP_ZFMethod_paramBuf(zfnull)
, _ZFP_ZFMethod_paramDefaultValueCallbackList(zfnull)
, _ZFP_ZFMethod_paramDefaultBeginIndex((zfuint)-1)
, _ZFP_ZFMethod_methodOwnerClass(zfnull)
, _ZFP_ZFMethod_methodOwnerProperty(zfnull)
, _ZFP_ZFMethod_privilegeType((unsigned short)ZFMethodPrivilegeTypePublic)
, _ZFP_ZFMethod_methodType((unsigned short)ZFMethodTypeNormal)
, _ZFP_ZFMethod_methodNamespace(zfnull)
{
}
ZFMethod::~ZFMethod(void) {
    zfRelease(this->_ZFP_ZFMethod_methodUserData);

    if(_ZFP_ZFMethod_ext) {
        zfpoolDelete(_ZFP_ZFMethod_ext);
    }

    // paramTypeIdList : ZFMETHOD_MAX_PARAM, fixed length for performance
    // paramNameList : paramCount
    if(_ZFP_ZFMethod_paramBuf != zfnull) {
        for(zfindex i = 0, count = ZFMETHOD_MAX_PARAM + methodParamCount(); i < count; ++i) {
            zfdeletePlacement(_ZFP_ZFMethod_paramBuf + i);
        }
        zffree(_ZFP_ZFMethod_paramBuf);
    }

    if(_ZFP_ZFMethod_paramCount > 0) {
        for(zfindex i = 0; i < _ZFP_ZFMethod_paramCount; ++i) {
            zfdeletePlacement(_ZFP_ZFMethod_paramDefaultValueCallbackList + i);
        }
        zffree(_ZFP_ZFMethod_paramDefaultValueCallbackList);
    }
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
            ZFCoreCriticalShouldNotGoHere();
            break;
    }

    if(this->methodType() == ZFMethodTypeStatic) {
        ret += "static ";
    }
    else if(this->methodType() == ZFMethodTypeVirtual) {
        ret += "virtual ";
    }

    ret += this->methodReturnTypeId();
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
            ret += this->methodParamTypeIdAt(i);
            ret += " ";
            ret += this->methodParamNameAt(i);
            if(i >= this->methodParamDefaultBeginIndex()) {
                ret += " = ";
                zfauto v = this->methodParamDefaultValueAt(i);
                if(v == zfnull) {
                    ret += ZFTOKEN_zfnull;
                }
                else {
                    v->objectInfoT(ret);
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
    ZFSigName *paramTypeIdList = method->_ZFP_ZFMethod_paramTypeIdList();
    return this->methodParamCount() == 0 || this->methodParamTypeIdIsMatch(
              paramTypeIdList[0]
            , paramTypeIdList[1]
            , paramTypeIdList[2]
            , paramTypeIdList[3]
            , paramTypeIdList[4]
            , paramTypeIdList[5]
            , paramTypeIdList[6]
            , paramTypeIdList[7]
        );
}

zfauto ZFMethod::methodInvoke(
        ZF_IN_OPT ZFObject *ownerObjOrNull /* = zfnull */
        ) const {
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfauto ret;
    if(this->methodGenericInvoker()(ret, zfnull, ownerObjOrNull, this, 0, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFMethod::methodInvoke(
        ZF_IN ZFObject *ownerObjOrNull
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) const {
    ZFCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == ZFMP_DEF()) {paramCount = 0; break;} else {paramList[0].zfunsafe_assign(param0);}
        if(param1 == ZFMP_DEF()) {paramCount = 1; break;} else {paramList[1].zfunsafe_assign(param1);}
        if(param2 == ZFMP_DEF()) {paramCount = 2; break;} else {paramList[2].zfunsafe_assign(param2);}
        if(param3 == ZFMP_DEF()) {paramCount = 3; break;} else {paramList[3].zfunsafe_assign(param3);}
        if(param4 == ZFMP_DEF()) {paramCount = 4; break;} else {paramList[4].zfunsafe_assign(param4);}
        if(param5 == ZFMP_DEF()) {paramCount = 5; break;} else {paramList[5].zfunsafe_assign(param5);}
        if(param6 == ZFMP_DEF()) {paramCount = 6; break;} else {paramList[6].zfunsafe_assign(param6);}
        if(param7 == ZFMP_DEF()) {paramCount = 7; break;} else {paramList[7].zfunsafe_assign(param7);}
    } while(zffalse);
    ZFCoreMutexUnlock();
    zfauto ret;
    if(this->methodGenericInvoker()(ret, zfnull, ownerObjOrNull, this, paramCount, paramList)) {
        return ret;
    }
    else {
        return zfnull;
    }
}
zfauto ZFMethod::methodInvokeDetail(
        ZF_IN ZFObject *ownerObjOrNull
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    ZFCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = zfmMin((zfindex)ZFMETHOD_MAX_PARAM, params.count());
    for(zfindex i = 0; i < paramCount; ++i) {
        paramList[i].zfunsafe_assign(params[i]);
    }
    for(zfindex i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
        paramList[i].zfunsafe_assign(ZFMP_DEF());
    }
    ZFCoreMutexUnlock();
    zfauto ret;
    if(this->methodGenericInvoker()(ret, errorHint, ownerObjOrNull, this, paramCount, paramList)) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

const ZFListener &ZFMethod::methodParamDefaultValueCallbackAt(ZF_IN zfindex index) const {
    ZFCoreAssert(index < this->methodParamCount());
    return this->_ZFP_ZFMethod_paramDefaultValueCallbackList[index];
}
zfauto ZFMethod::methodParamDefaultValueAt(ZF_IN zfindex index) const {
    ZFCoreAssert(index < this->methodParamCount());
    if(index < this->methodParamDefaultBeginIndex()) {
        return zfnull;
    }
    else {
        ZFArgs zfargs;
        zfargs.result(ZFMP_DEF());
        this->methodParamDefaultValueCallbackAt(index).execute(zfargs);
        return zfargs.result();
    }
}

void ZFMethod::methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const {
    ZFCoreMutexLocker();
    ZFMethod *owner = this->_ZFP_ZFMethod_removeConst();
    if(methodGenericInvoker != zfnull) {
        if(_ZFP_ZFMethod_ext == zfnull) {
            owner->_ZFP_ZFMethod_ext = zfpoolNew(_ZFP_ZFMethodPrivateExt);
        }
        if(owner->_ZFP_ZFMethod_ext->methodGenericInvokerOrig == zfnull) {
            owner->_ZFP_ZFMethod_ext->methodGenericInvokerOrig = _ZFP_ZFMethod_methodGenericInvoker;
        }
        owner->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    }
    else {
        if(_ZFP_ZFMethod_ext && _ZFP_ZFMethod_ext->methodGenericInvokerOrig) {
            owner->_ZFP_ZFMethod_methodGenericInvoker = _ZFP_ZFMethod_ext->methodGenericInvokerOrig;
        }
    }
}

void ZFMethod::methodInvoker(ZF_IN ZFFuncAddrType methodInvoker) const {
    ZFCoreMutexLocker();
    ZFMethod *owner = this->_ZFP_ZFMethod_removeConst();
    if(methodInvoker != zfnull) {
        if(_ZFP_ZFMethod_ext == zfnull) {
            owner->_ZFP_ZFMethod_ext = zfpoolNew(_ZFP_ZFMethodPrivateExt);
        }
        if(owner->_ZFP_ZFMethod_ext->invokerOrig == zfnull) {
            owner->_ZFP_ZFMethod_ext->invokerOrig = _ZFP_ZFMethod_invoker;
        }
        owner->_ZFP_ZFMethod_invoker = methodInvoker;
    }
    else {
        if(_ZFP_ZFMethod_ext && _ZFP_ZFMethod_ext->invokerOrig) {
            owner->_ZFP_ZFMethod_invoker = _ZFP_ZFMethod_ext->invokerOrig;
        }
    }
}

// ============================================================
typedef zfstlhashmap<zfstring, ZFMethod *, zfstring_zfstlHash, zfstring_zfstlEqual> _ZFP_ZFMethodMapType;
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
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    if(!zfstringIsEmpty(methodScope)
        && !zfstringIsEqual(methodScope, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodScope, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        zfindexToStringT(ret, methodScope.sigId());
    }
    ret += ':';
    zfindexToStringT(ret, methodName.sigId());

    for(zfuint i = 0; i < mp.paramCount; ++i) {
        ret += '+';
        zfidentityToStringT(ret, mp.paramTypeId[i].sigId());
    }
}

static ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfstring &methodInternalId) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodInternalId);
    if(it != m.end()) {
        ZFMethod *v = it->second;
        ++(v->_ZFP_ZFMethod_refCount);
        return v;
    }
    else {
        ZFMethod *v = zfnew(ZFMethod);
        v->_ZFP_ZFMethod_methodInternalId = methodInternalId;
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
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    _ZFP_ZFMethod_invokeTimeLogger("reg: %s::%s"
            , methodOwnerClass ? methodOwnerClass->className().cString() : methodNamespace ? methodNamespace.cString() : ""
            , methodName.cString()
            );
    ZFCoreMutexLocker();
    const zfchar *methodNamespaceTmp = methodNamespace;
    methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespaceTmp);

    ZFCoreAssert(methodGenericInvoker != zfnull);
    ZFCoreAssert(methodOwnerClass == zfnull || zfstringIsEmpty(methodNamespaceTmp));
    ZFCoreAssert(methodName != zfnull && *methodName != '\0');
    ZFCoreAssert(returnTypeId != zfnull && *returnTypeId != '\0');

    ZFMethod *method = zfnull;

    zfstring methodInternalId;
    ZFSigName methodScope;
    if(methodOwnerClass) {
        methodScope = methodOwnerClass->classNameFull();
    }
    else {
        methodScope = methodNamespaceTmp;
    }
    ZFSigName methodNameSig = methodName;
    _ZFP_ZFMethodInstanceSig(methodInternalId
            , methodScope
            , methodNameSig
            , mp
        );

    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodInternalId);
    if(it != m.end()) {
        method = it->second;
        if(method->methodIsUserRegister()) {
            if(methodOwnerClass != zfnull) {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodUserRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId);
            }
            else {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodFuncUserRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespaceTmp,
                    methodName,
                    methodInternalId);
            }
        }
        else if(method->methodIsDynamicRegister()) {
            if(methodOwnerClass != zfnull) {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId);
            }
            else {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespaceTmp,
                    methodName,
                    methodInternalId);
            }
        }

        // param type id may be same for different cpp type
        // strict check type name to prevent redefine
        zfbool isRedefine = ((zfindex)mp.paramCount == method->methodParamCount());
        if(!isRedefine) {
            for(zfuint i = 0; i < mp.paramCount; ++i) {
                if(!zfstringIsEqual(mp.paramTypeId[i], method->methodParamTypeIdAt(i))) {
                    isRedefine = zftrue;
                    break;
                }
            }
        }
        ZFCoreAssertWithMessageTrim(!isRedefine,
            "[ZFMethod] redefine of method: %s, existing: %s",
            methodInternalId,
            method);

        ++method->_ZFP_ZFMethod_refCount;
    }
    else {
        method = _ZFP_ZFMethodInstanceAccess(methodInternalId);

        if(method->_ZFP_ZFMethod_methodName == zfnull) {
            method->_ZFP_ZFMethod_init(methodIsUserRegister
                    , methodIsDynamicRegister
                    , methodDynamicRegisterUserData
                    , methodInvoker
                    , methodGenericInvoker
                    , methodType
                    , methodName
                    , returnTypeId
                    , mp
                );

            if(methodOwnerClass != zfnull) {
                method->_ZFP_ZFMethod_initClassMemberType(methodOwnerClass, methodPrivilegeType);
                methodOwnerClass->_ZFP_ZFClass_methodRegister(method);
                _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeAttach, zfnull, zfnull, method);
            }
            else {
                method->_ZFP_ZFMethod_initFuncType(methodNamespaceTmp);
                _ZFP_ZFMethodFuncRegister(method);
                _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeAttach, zfnull, zfnull, method);
            }
        }
    }

    return method;
}
void _ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method) {
    _ZFP_ZFMethod_invokeTimeLogger("unreg: %s", method->methodName().cString());
    ZFCoreMutexLocker();
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
    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeDetach, zfnull, zfnull, v);

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
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        )
: method(zfnull)
{
    method = _ZFP_ZFMethodRegister(methodIsUserRegister
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
            , mp
        );
}
_ZFP_ZFMethodRegisterHolder::~_ZFP_ZFMethodRegisterHolder(void) {
    _ZFP_ZFMethodUnregister(this->method);
}

// ============================================================
void ZFMethodGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN_OPT const ZFFilterForZFMethod *methodFilter /* = zfnull */
        ) {
    ZFCoreMutexLocker();
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
        ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
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
        ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
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
        , ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
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
        , ZF_IN const zfstring &aliasName
        ) {
    ZFCoreMutexLocker();
    if(method == zfnull || zfstringIsEmpty(aliasName)) {
        return zfnull;
    }
    ZFCoreArray<const ZFMethod *> exist;
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

    _ZFP_ZFMethodMP mp;
    for(zfindex i = 0; i < method->methodParamCount(); ++i) {
        mp.add(
                method->methodParamTypeIdAt(i)
                , method->methodParamNameAt(i)
                , method->methodParamDefaultValueCallbackAt(i)
                );
    }
    ZFMethod *ret = _ZFP_ZFMethodRegister(
            method->methodIsUserRegister()
            , method->methodIsDynamicRegister()
            , method->methodDynamicRegisterUserData()
            , method->methodInvokerOrig()
            , method->methodGenericInvokerOrig()
            , method->methodType()
            , method->methodOwnerClass()
            , method->methodPrivilegeType()
            , method->methodNamespace()
            , aliasName
            , method->methodReturnTypeId()
            , mp
            );
    if(ret == zfnull) {
        return zfnull;
    }
    if(ret->_ZFP_ZFMethod_ext == zfnull) {
        ret->_ZFP_ZFMethod_ext = zfpoolNew(_ZFP_ZFMethodPrivateExt);
    }
    if(method->_ZFP_ZFMethod_ext == zfnull) {
        method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_ext = zfpoolNew(_ZFP_ZFMethodPrivateExt);
    }
    ret->_ZFP_ZFMethod_ext->methodAliasFrom = method;
    method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_ext->methodAliasTo.add(ret);
    return ret;
}
static void _ZFP_ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    if(aliasMethod == zfnull || aliasMethod->methodAliasFrom() == zfnull) {
        return;
    }
    ZFMethod *from = aliasMethod->methodAliasFrom()->_ZFP_ZFMethod_removeConst();
    ZFMethod *to = aliasMethod->_ZFP_ZFMethod_removeConst();
    from->_ZFP_ZFMethod_ext->methodAliasTo.removeElement(to);
    to->_ZFP_ZFMethod_ext->methodAliasFrom = zfnull;

    if(!to->methodAliasTo().isEmpty()) {
        ZFCoreArray<const ZFMethod *> chain = to->_ZFP_ZFMethod_ext->methodAliasTo;
        to->_ZFP_ZFMethod_ext->methodAliasTo = ZFCoreArray<const ZFMethod *>();

        for(zfindex i = 0; i < chain.count(); ++i) {
            ZFMethod *chainTmp = chain[i]->_ZFP_ZFMethod_removeConst();
            chainTmp->_ZFP_ZFMethod_ext->methodAliasFrom = zfnull;
            _ZFP_ZFMethodAliasRemove(chainTmp);
        }
    }

    _ZFP_ZFMethodUnregister(aliasMethod);
}
void ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    ZFCoreMutexLocker();
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
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArray<const ZFMethod *>, ZFMethodGetAll
        , ZFMP_IN_OPT(const ZFFilterForZFMethod *, methodFilter, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodForName
        , ZFMP_IN(const zfstring &, classNameOrNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodForName
        , ZFMP_IN(const zfstring &, classNameOrNamespace)
        , ZFMP_IN(const zfstring &, methodName)
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
        , ZFMP_IN(const zfstring &, classNameOrNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<const ZFMethod *>, ZFMethodForNameGetAll
        , ZFMP_IN(const zfstring &, classNameOrNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodAlias
        , ZFMP_IN(const ZFMethod *, method)
        , ZFMP_IN(const zfstring &, aliasName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodAliasRemove
        , ZFMP_IN(const ZFMethod *, aliasMethod)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

