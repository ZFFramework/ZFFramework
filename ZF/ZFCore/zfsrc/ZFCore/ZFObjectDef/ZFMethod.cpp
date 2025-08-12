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
    const ZFMethod *aliasFrom;
    ZFCoreArray<const ZFMethod *> aliasTo;
    ZFFuncAddrType invokerOrig;
    ZFMethodGenericInvoker methodGenericInvokerOrig;
public:
    _ZFP_ZFMethodPrivateExt(void)
    : aliasFrom(zfnull)
    , aliasTo()
    , invokerOrig(zfnull)
    , methodGenericInvokerOrig(zfnull)
    {
    }
};

const ZFMethod *ZFMethod::aliasFrom(void) const {
    return _ZFP_ZFMethod_ext ? _ZFP_ZFMethod_ext->aliasFrom : zfnull;
}
const ZFCoreArray<const ZFMethod *> &ZFMethod::aliasTo(void) const {
    static ZFCoreArray<const ZFMethod *> dummy;
    return _ZFP_ZFMethod_ext ? _ZFP_ZFMethod_ext->aliasTo : dummy;
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
void ZFMethod::userRegisterUserData(ZF_IN ZFObject *userRegisterUserData) const {
    ZFCoreAssertWithMessage(this->isUserRegister(),
        "userRegisterUserData can only be changed for user registered method");
    zfRetainChange(this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_methodUserData, userRegisterUserData);
}

void ZFMethod::_ZFP_ZFMethod_init(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN ZFFuncAddrType invoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    ZFCoreAssert(methodGenericInvoker != zfnull);

    this->_ZFP_ZFMethod_isUserRegister = isUserRegister;
    this->_ZFP_ZFMethod_isDynamicRegister = isDynamicRegister;
    this->_ZFP_ZFMethod_methodUserData = zfRetain(dynamicRegisterUserData);
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
    if(zfstringBeginWith(methodName, "_ZFP_")) {
        this->_ZFP_ZFMethod_isInternal = zftrue;
        this->_ZFP_ZFMethod_isInternalPrivate = zftrue;
    }
    else if(methodName[0] == '_') {
        this->_ZFP_ZFMethod_isInternal = zftrue;
        this->_ZFP_ZFMethod_isInternalPrivate = zffalse;
    }
    else {
        this->_ZFP_ZFMethod_isInternal = zffalse;
        this->_ZFP_ZFMethod_isInternalPrivate = zffalse;
    }
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(
        ZF_IN const ZFClass *ownerClass
        , ZF_IN ZFMethodAccessType accessType
        ) {
    this->_ZFP_ZFMethod_ownerClass = ownerClass;
    this->_ZFP_ZFMethod_accessType = (unsigned short)accessType;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfstring &methodNamespace) {
    if(methodNamespace
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
    }

    this->_ZFP_ZFMethod_ownerClass = zfnull;
    this->_ZFP_ZFMethod_accessType = (unsigned short)ZFMethodAccessTypePublic;
}

ZFMethod::ZFMethod(void)
: _ZFP_ZFMethod_refCount(1)
, _ZFP_ZFMethod_isInternal(zffalse)
, _ZFP_ZFMethod_isInternalPrivate(zffalse)
, _ZFP_ZFMethod_isUserRegister(zffalse)
, _ZFP_ZFMethod_isDynamicRegister(zffalse)
, _ZFP_ZFMethod_methodUserData(zfnull)
, _ZFP_ZFMethod_methodId(zfnull)
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
, _ZFP_ZFMethod_accessType((unsigned short)ZFMethodAccessTypePublic)
, _ZFP_ZFMethod_methodType((unsigned short)ZFMethodTypeNormal)
, _ZFP_ZFMethod_ownerClass(zfnull)
, _ZFP_ZFMethod_ownerProperty(zfnull)
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
        for(zfindex i = 0, count = ZFMETHOD_MAX_PARAM + paramCount(); i < count; ++i) {
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
    switch(this->methodAccessType()) {
        case ZFMethodAccessTypePublic:
            break;
        case ZFMethodAccessTypeProtected:
            ret += ZFTOKEN_ZFMethodAccessTypeProtected;
            ret += ": ";
            break;
        case ZFMethodAccessTypePrivate:
            ret += ZFTOKEN_ZFMethodAccessTypePrivate;
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

    ret += this->returnTypeId();
    ret += " ";

    if(this->isFuncType()) {
        if(this->methodNamespace()) {
            ret += this->methodNamespace();
            ret += "::";
        }
    }
    else {
        ret += this->ownerClass()->classNameFull();
        ret += "::";
    }

    ret += this->methodName();

    if(this->paramCount() > 0) {
        ret += "(";
        this->paramInfoT(ret);
        ret += ")";
    }
    else {
        ret += "(void)";
    }
}

zfbool ZFMethod::paramTypeIdIsMatch(
        ZF_IN_OPT const zfchar *paramTypeId0 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) const {
    #define _ZFP_ZFMethodParamCheck_paramLoop(N) \
        if(zfstringIsEmpty(paramTypeId##N) || zfstringIsEqual(paramTypeId##N, ZFTypeId_void())) {return (this->paramCount() == N);} \
        if(this->paramCount() <= N || !zfstringIsEqual(this->paramTypeIdAt(N), paramTypeId##N)) {return zffalse;}
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
zfbool ZFMethod::paramTypeIdIsMatch(ZF_IN const ZFMethod *method) const {
    if(method->paramCount() != this->paramCount()
            || method->paramDefaultBeginIndex() != this->paramDefaultBeginIndex()
            ) {
        return zffalse;
    }
    ZFSigName *paramTypeIdList = method->_ZFP_ZFMethod_paramTypeIdList();
    return this->paramCount() == 0 || this->paramTypeIdIsMatch(
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
    if(!this->methodInvokeCheck(ownerObjOrNull)) {
        return zfnull;
    }
    ZFArgs zfargs;
    zfargs
        .sender(ownerObjOrNull)
        .ownerMethod(this)
        .paramInit()
        ;
    this->methodGenericInvoker()(zfargs);
    if(zfargs.success()) {
        return zfargs.result();
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
    if(!this->methodInvokeCheck(ownerObjOrNull)) {
        return zfnull;
    }
    ZFArgs zfargs;
    zfargs
        .sender(ownerObjOrNull)
        .ownerMethod(this)
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        ;
    this->methodGenericInvoker()(zfargs);
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        return zfnull;
    }
}
zfbool ZFMethod::methodInvokeT(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN ZFObject *ownerObjOrNull
        , ZF_IN_OPT ZFObject *param0 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) const {
    if(!this->methodInvokeCheck(ownerObjOrNull, errorHint)) {
        return zffalse;
    }
    ZFArgs zfargs;
    zfargs
        .sender(ownerObjOrNull)
        .ownerMethod(this)
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    this->methodGenericInvoker()(zfargs);
    if(zfargs.success()) {
        ret = zfargs.result();
        return zftrue;
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
        return zffalse;
    }
}
zfauto ZFMethod::methodInvokeDetail(
        ZF_IN ZFObject *ownerObjOrNull
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    if(!this->methodInvokeCheck(ownerObjOrNull)) {
        return zfnull;
    }
    ZFArgs zfargs;
    zfargs
        .sender(ownerObjOrNull)
        .ownerMethod(this)
        .paramInit(params)
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    this->methodGenericInvoker()(zfargs);
    if(success != zfnull) {
        *success = zfargs.success();
    }
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
        return zfnull;
    }
}

zfbool ZFMethod::methodInvokeCheck(
        ZF_IN ZFObject *ownerObjOrNull
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    if(this->methodType() != ZFMethodTypeStatic) {
        if(ownerObjOrNull == zfnull) {
            if(errorHint) {
                zfstringAppend(*errorHint, "null object when calling method: %s", this);
            }
            return zffalse;
        }
        else if(!ownerObjOrNull->classData()->classIsTypeOf(this->ownerClass())) {
            if(errorHint) {
                zfstringAppend(*errorHint, "mismatch object %s when calling method: %s", ownerObjOrNull->objectInfoOfInstance(), this);
            }
            return zffalse;
        }
    }
    return zftrue;
}

const ZFListener &ZFMethod::paramDefaultValueCallbackAt(ZF_IN zfindex index) const {
    ZFCoreAssert(index < this->paramCount());
    return this->_ZFP_ZFMethod_paramDefaultValueCallbackList[index];
}
zfauto ZFMethod::paramDefaultValueAt(ZF_IN zfindex index) const {
    ZFCoreAssert(index < this->paramCount());
    if(index < this->paramDefaultBeginIndex()) {
        return zfnull;
    }
    else {
        ZFArgs zfargs;
        zfargs.result(ZFMP_DEF());
        this->paramDefaultValueCallbackAt(index).execute(zfargs);
        return zfargs.result();
    }
}

void ZFMethod::paramInfoT(ZF_IN_OUT zfstring &ret) const {
    for(zfindex i = 0; i < this->paramCount(); ++i) {
        if(i != 0) {
            ret += ", ";
        }
        ret += this->paramTypeIdAt(i);
        ret += " ";
        ret += this->paramNameAt(i);
        if(i >= this->paramDefaultBeginIndex()) {
            ret += " = ";
            zfauto v = this->paramDefaultValueAt(i);
            if(v == zfnull) {
                ret += ZFTOKEN_zfnull;
            }
            else {
                v->objectInfoT(ret);
            }
        }
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
typedef zfstlhashmap<zfstring, ZFMethod *> _ZFP_ZFMethodMapType;
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
    if(methodScope
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

static ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfstring &methodId) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodId);
    if(it != m.end()) {
        ZFMethod *v = it->second;
        ++(v->_ZFP_ZFMethod_refCount);
        return v;
    }
    else {
        ZFMethod *v = zfnew(ZFMethod);
        v->_ZFP_ZFMethod_methodId = methodId;
        m[methodId] = v;
        return v;
    }
}

ZFMethod *_ZFP_ZFMethodRegister(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN ZFMethodAccessType methodAccessType
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    _ZFP_ZFMethod_invokeTimeLogger("reg: %s::%s"
            , ownerClass ? ownerClass->className().cString() : methodNamespace ? methodNamespace.cString() : ""
            , methodName.cString()
            );
    ZFCoreMutexLocker();
    const zfchar *methodNamespaceTmp = methodNamespace;
    methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespaceTmp);

    ZFCoreAssert(methodGenericInvoker != zfnull);
    ZFCoreAssert(ownerClass == zfnull || !methodNamespaceTmp);
    ZFCoreAssert(methodName != zfnull && *methodName != '\0');
    ZFCoreAssert(returnTypeId != zfnull && *returnTypeId != '\0');

    ZFMethod *method = zfnull;

    zfstring methodId;
    ZFSigName methodScope;
    if(ownerClass) {
        methodScope = ownerClass->classNameFull();
    }
    else {
        methodScope = methodNamespaceTmp;
    }
    ZFSigName methodNameSig = methodName;
    _ZFP_ZFMethodInstanceSig(methodId
            , methodScope
            , methodNameSig
            , mp
        );

    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(methodId);
    if(it != m.end()) {
        method = it->second;
        if(method->isUserRegister()) {
            if(ownerClass != zfnull) {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodUserRegister] registering a method that already registered, class: %s, methodName: %s, methodId: %s",
                    ownerClass->classNameFull(),
                    methodName,
                    methodId);
            }
            else {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodFuncUserRegister] registering a method that already registered, namespace: %s, methodName: %s, methodId: %s",
                    methodNamespaceTmp,
                    methodName,
                    methodId);
            }
        }
        else if(method->isDynamicRegister()) {
            if(ownerClass != zfnull) {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, class: %s, methodName: %s, methodId: %s",
                    ownerClass->classNameFull(),
                    methodName,
                    methodId);
            }
            else {
                ZFCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, namespace: %s, methodName: %s, methodId: %s",
                    methodNamespaceTmp,
                    methodName,
                    methodId);
            }
        }

        // param type id may be same for different cpp type
        // strict check type name to prevent redefine
        zfbool isRedefine = ((zfindex)mp.paramCount == method->paramCount());
        if(!isRedefine) {
            for(zfuint i = 0; i < mp.paramCount; ++i) {
                if(!zfstringIsEqual(mp.paramTypeId[i], method->paramTypeIdAt(i))) {
                    isRedefine = zftrue;
                    break;
                }
            }
        }
        ZFCoreAssertWithMessageTrim(!isRedefine,
            "[ZFMethod] redefine of method: %s, existing: %s",
            methodId,
            method);

        ++method->_ZFP_ZFMethod_refCount;
    }
    else {
        method = _ZFP_ZFMethodInstanceAccess(methodId);

        if(method->_ZFP_ZFMethod_methodName == zfnull) {
            method->_ZFP_ZFMethod_init(isUserRegister
                    , isDynamicRegister
                    , dynamicRegisterUserData
                    , methodInvoker
                    , methodGenericInvoker
                    , methodType
                    , methodName
                    , returnTypeId
                    , mp
                );

            if(ownerClass != zfnull) {
                method->_ZFP_ZFMethod_initClassMemberType(ownerClass, methodAccessType);
                ownerClass->_ZFP_ZFClass_methodRegister(method);
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
    _ZFP_ZFMethodMapType::iterator it = m.find(method->_ZFP_ZFMethod_methodId);
    if(it == m.end()) {
        return;
    }

    ZFMethod *v = it->second;
    --(v->_ZFP_ZFMethod_refCount);
    if(v->_ZFP_ZFMethod_refCount != 0) {
        return;
    }
    m.erase(it);

    if(v->isFuncType()) {
        _ZFP_ZFMethodFuncUnregister(v);
    }
    else {
        if(v->isUserRegister() || v->isDynamicRegister()) {
            v->ownerClass()->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodUnregister(v);
        }
    }
    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeDetach, zfnull, zfnull, v);

    zfdelete(v);
}

_ZFP_ZFMethodRegisterHolder::_ZFP_ZFMethodRegisterHolder(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN ZFMethodAccessType methodAccessType
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        )
: method(zfnull)
{
    method = _ZFP_ZFMethodRegister(isUserRegister
            , isDynamicRegister
            , dynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , ownerClass
            , methodAccessType
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
void ZFMethodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    for(_ZFP_ZFMethodMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ZFMethod *v = it->second;
        ret.add(v);
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
        , ZF_IN_OPT const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) {
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull) {
        return cls->methodForName(methodName
                , paramTypeId0
                , paramTypeId1
                , paramTypeId2
                , paramTypeId3
                , paramTypeId4
                , paramTypeId5
                , paramTypeId6
                , paramTypeId7
            );
    }
    else {
        return ZFMethodFuncForName(classNameOrNamespace, methodName
                , paramTypeId0
                , paramTypeId1
                , paramTypeId2
                , paramTypeId3
                , paramTypeId4
                , paramTypeId5
                , paramTypeId6
                , paramTypeId7
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
    if(method == zfnull || !aliasName) {
        return zfnull;
    }
    ZFCoreArray<const ZFMethod *> exist;
    if(method->ownerClass() != zfnull) {
        method->ownerClass()->methodForNameGetAllT(exist, aliasName);
    }
    else {
        ZFMethodFuncForNameGetAllT(exist, method->methodNamespace(), aliasName);
    }
    if(!exist.isEmpty()) {
        for(zfindex i = 0; i < exist.count(); ++i) {
            if(exist[i]->paramTypeIdIsMatch(method)) {
                return exist[i];
            }
        }
    }

    _ZFP_ZFMethodMP mp;
    for(zfindex i = 0; i < method->paramCount(); ++i) {
        mp.add(
                method->paramTypeIdAt(i)
                , method->paramNameAt(i)
                , method->paramDefaultValueCallbackAt(i)
                );
    }
    ZFMethod *ret = _ZFP_ZFMethodRegister(
            method->isUserRegister()
            , method->isDynamicRegister()
            , method->dynamicRegisterUserData()
            , method->methodInvokerOrig()
            , method->methodGenericInvokerOrig()
            , method->methodType()
            , method->ownerClass()
            , method->methodAccessType()
            , method->methodNamespace()
            , aliasName
            , method->returnTypeId()
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
    ret->_ZFP_ZFMethod_ext->aliasFrom = method;
    method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_ext->aliasTo.add(ret);
    return ret;
}
static void _ZFP_ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    if(aliasMethod == zfnull || aliasMethod->aliasFrom() == zfnull) {
        return;
    }
    ZFMethod *from = aliasMethod->aliasFrom()->_ZFP_ZFMethod_removeConst();
    ZFMethod *to = aliasMethod->_ZFP_ZFMethod_removeConst();
    from->_ZFP_ZFMethod_ext->aliasTo.removeElement(to);
    to->_ZFP_ZFMethod_ext->aliasFrom = zfnull;

    if(!to->aliasTo().isEmpty()) {
        ZFCoreArray<const ZFMethod *> chain = to->_ZFP_ZFMethod_ext->aliasTo;
        to->_ZFP_ZFMethod_ext->aliasTo = ZFCoreArray<const ZFMethod *>();

        for(zfindex i = 0; i < chain.count(); ++i) {
            ZFMethod *chainTmp = chain[i]->_ZFP_ZFMethod_removeConst();
            chainTmp->_ZFP_ZFMethod_ext->aliasFrom = zfnull;
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

