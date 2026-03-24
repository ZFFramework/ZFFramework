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
zfclassNotPOD _ZFP_ZFMethodPrivate {
public:
    const ZFMethod *aliasFrom;
    ZFCoreArray<const ZFMethod *> aliasTo;
    ZFMethodGenericInvoker methodGenericInvokerOrig;
    ZFCoreArray<ZFListener> methodImplReplaceList; // latest at head
public:
    _ZFP_ZFMethodPrivate(void)
    : aliasFrom(zfnull)
    , aliasTo()
    , methodGenericInvokerOrig(zfnull)
    , methodImplReplaceList()
    {
    }
};

const ZFMethod *ZFMethod::aliasFrom(void) const {
    return _ext ? _ext->aliasFrom : zfnull;
}
const ZFCoreArray<const ZFMethod *> &ZFMethod::aliasTo(void) const {
    static ZFCoreArray<const ZFMethod *> dummy;
    return _ext ? _ext->aliasTo : dummy;
}

ZFMethodGenericInvoker ZFMethod::methodGenericInvokerOrig(void) const {
    return _ext && _ext->methodGenericInvokerOrig
        ? _ext->methodGenericInvokerOrig
        : _methodGenericInvoker
        ;
}

// ============================================================
void ZFMethod::userRegisterUserData(ZF_IN ZFObject *userRegisterUserData) const {
    ZFCoreAssertWithMessage(this->isUserRegister(),
        "userRegisterUserData can only be changed for user registered method");
    zfobjRetainChange(this->_ZFP_ZFMethod_removeConst()->_methodUserData, userRegisterUserData);
}

void ZFMethod::_ZFP_ZFMethod_init(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        ) {
    ZFCoreAssert(methodGenericInvoker != zfnull);

    if(isUserRegister) {
        ZFBitSet(_stateFlags, _stateFlags_isUserRegister);
    }
    if(isDynamicRegister) {
        ZFBitSet(_stateFlags, _stateFlags_isDynamicRegister);
    }
    this->_methodUserData = zfobjRetain(dynamicRegisterUserData);
    _methodInvoker = methodInvoker;
    _methodGenericInvoker = methodGenericInvoker;
    _methodType = (zfbyte)methodType;
    _methodName = methodName;
    _returnTypeId = returnTypeId;

    // update param count
    for(zfuint i = 0; i < mp.paramCount; ++i) {
        if(*(mp.paramDefaultValueCallback[i]) != zfnull) {
            _paramDefaultBeginIndex = (zfbyte)i;
            break;
        }
    }
    _paramCount = (zfbyte)mp.paramCount;
    if(_paramDefaultBeginIndex == (zfbyte)-1) {
        _paramCountMin = _paramCount;
    }
    else {
        _paramCountMin = _paramDefaultBeginIndex;
    }

    // store param data
    if(mp.paramCount > 0) {
        _paramBuf = (ZFSigName *)zfmalloc((ZFMETHOD_MAX_PARAM + mp.paramCount) * sizeof(ZFSigName));

        ZFSigName *paramTypeIdList = _paramBuf;
        ZFSigName *paramNameList = paramTypeIdList + ZFMETHOD_MAX_PARAM;

        for(zfuint i = 0; i < mp.paramCount; ++i) {
            zfnewPlacement(paramTypeIdList + i, ZFSigName, mp.paramTypeId[i]);
            zfnewPlacement(paramNameList + i, ZFSigName, mp.paramName[i]);
        }
        for(zfuint i = mp.paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
            zfnewPlacement(paramTypeIdList + i, ZFSigName);
        }

        _paramDefaultValueCallbackList = (ZFListener *)zfmalloc(sizeof(ZFListener) * mp.paramCount);
        for(zfindex i = 0; i < mp.paramCount; ++i) {
            zfnewPlacement(_paramDefaultValueCallbackList + i, ZFListener, *(mp.paramDefaultValueCallback[i]));
        }
    }

    // internal
    if(zfstringBeginWith(methodName, "_ZFP_")) {
        ZFBitSet(_stateFlags, _stateFlags_isInternal);
        ZFBitSet(_stateFlags, _stateFlags_isInternalPrivate);
    }
    else if(methodName[0] == '_') {
        ZFBitSet(_stateFlags, _stateFlags_isInternal);
    }
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(
        ZF_IN const ZFClass *ownerClass
        , ZF_IN ZFMethodAccessType accessType
        ) {
    _ownerClass = ownerClass;
    _methodAccessType = (zfbyte)accessType;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfstring &methodNamespace) {
    if(methodNamespace
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME)
    ) {
        _methodNamespace = methodNamespace;
    }

    _ownerClass = zfnull;
    _methodAccessType = (zfbyte)ZFMethodAccessTypePublic;
}

ZFMethod::ZFMethod(void)
: _refCount(1)
, _stateFlags(0)
, _methodUserData(zfnull)
, _methodId(zfnull)
, _ext(zfnull)
, _methodInvoker(zfnull)
, _methodGenericInvoker(zfnull)
, _methodName(zfnull)
, _returnTypeId(zfnull)
, _paramBuf(zfnull)
, _paramDefaultValueCallbackList(zfnull)
, _paramCount(0)
, _paramCountMin(0)
, _paramDefaultBeginIndex((zfbyte)-1)
, _methodAccessType((zfbyte)ZFMethodAccessTypePublic)
, _methodType((zfbyte)ZFMethodTypeNormal)
, _ownerClass(zfnull)
, _ownerProperty(zfnull)
, _methodNamespace(zfnull)
{
}
ZFMethod::~ZFMethod(void) {
    zfobjRelease(this->_methodUserData);

    if(_ext) {
        zfpoolDelete(_ext);
    }

    // paramTypeIdList : ZFMETHOD_MAX_PARAM, fixed length for performance
    // paramNameList : paramCount
    if(_paramBuf != zfnull) {
        for(zfindex i = 0, count = ZFMETHOD_MAX_PARAM + paramCount(); i < count; ++i) {
            zfdeletePlacement(_paramBuf + i);
        }
        zffree(_paramBuf);
    }

    if(_paramCount > 0) {
        for(zfindex i = 0; i < (zfindex)_paramCount; ++i) {
            zfdeletePlacement(_paramDefaultValueCallbackList + i);
        }
        zffree(_paramDefaultValueCallbackList);
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
    return _paramDefaultValueCallbackList[index];
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

// ============================================================
typedef zfstlhashmap<zfstring, ZFMethod *> _ZFP_ZFMethodMapType;
static _ZFP_ZFMethodMapType &_ZFP_ZFMethodMap(void) {
    static _ZFP_ZFMethodMapType m;
    return m;
}

void _ZFP_ZFMethodDataHolderInit(void) {
    _ZFP_ZFMethodMap();
}

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

ZFMethod *ZFMethod::_ZFP_ZFMethodRegister(
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
    zfstring methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespace);

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

        ++(method->_refCount);
    }
    else {
        {
            _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
            _ZFP_ZFMethodMapType::iterator it = m.find(methodId);
            if(it != m.end()) {
                method = it->second;
                ++(method->_refCount);
            }
            else {
                method = zfnew(ZFMethod);
                method->_methodId = methodId;
                m[methodId] = method;
            }
        }

        if(method->_methodName == zfnull) {
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
void ZFMethod::_ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method) {
    _ZFP_ZFMethod_invokeTimeLogger("unreg: %s", method->methodName().cString());
    ZFCoreMutexLocker();
    _ZFP_ZFMethodMapType &m = _ZFP_ZFMethodMap();
    _ZFP_ZFMethodMapType::iterator it = m.find(method->_methodId);
    if(it == m.end()) {
        return;
    }

    ZFMethod *v = it->second;
    --(v->_refCount);
    if(v->_refCount != 0) {
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
    method = ZFMethod::_ZFP_ZFMethodRegister(isUserRegister
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
    ZFMethod::_ZFP_ZFMethodUnregister(this->method);
}

// ============================================================
_ZFP_ZFMethodPrivate *ZFMethod::_ZFP_ZFMethod_extInit(void) {
    if(_ext == zfnull) {
        _ext = zfpoolNew(_ZFP_ZFMethodPrivate);
    }
    return _ext;
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
    ZFMethod *ret = ZFMethod::_ZFP_ZFMethodRegister(
            method->isUserRegister()
            , method->isDynamicRegister()
            , method->dynamicRegisterUserData()
            , method->methodInvoker()
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
    ret->_ZFP_ZFMethod_extInit()->aliasFrom = method;
    method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_extInit()->aliasTo.add(ret);
    return ret;
}
static void _ZFP_ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    if(aliasMethod == zfnull || aliasMethod->aliasFrom() == zfnull) {
        return;
    }
    ZFMethod *from = aliasMethod->aliasFrom()->_ZFP_ZFMethod_removeConst();
    ZFMethod *to = aliasMethod->_ZFP_ZFMethod_removeConst();
    from->_ZFP_ZFMethod_ext()->aliasTo.removeElement(to);
    to->_ZFP_ZFMethod_ext()->aliasFrom = zfnull;

    if(!to->aliasTo().isEmpty()) {
        ZFCoreArray<const ZFMethod *> chain = to->_ZFP_ZFMethod_ext()->aliasTo;
        to->_ZFP_ZFMethod_ext()->aliasTo = ZFCoreArray<const ZFMethod *>();

        for(zfindex i = 0; i < chain.count(); ++i) {
            ZFMethod *chainTmp = chain[i]->_ZFP_ZFMethod_removeConst();
            chainTmp->_ZFP_ZFMethod_ext()->aliasFrom = zfnull;
            _ZFP_ZFMethodAliasRemove(chainTmp);
        }
    }

    ZFMethod::_ZFP_ZFMethodUnregister(aliasMethod);
}
void ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethodAliasRemove(aliasMethod);
}

// ============================================================
static void _ZFP_ZFMethodImplReplace_GI(ZF_IN const ZFArgs &zfargs) {
    if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
        return;
    }
    zfargs._ZFP_ZFMethodImplReplace_index() = 0;
    zfargs.ownerMethod()->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethodImplReplace_invoke(zfargs);
}
void ZFMethod::_ZFP_ZFMethodImplReplace_invoke(ZF_IN const ZFArgs &zfargs) {
    ZFCoreMutexLock();
    zfuint &index = zfargs._ZFP_ZFMethodImplReplace_index();
    if(index >= _ext->methodImplReplaceList.count()) {
        ZFCoreMutexUnlock();
        _ext->methodGenericInvokerOrig(zfargs);
        return;
    }
    ZFListener impl = _ext->methodImplReplaceList.get(index++);
    ZFCoreMutexUnlock();

    impl.execute(zfargs);
}
void ZFMethod::_ZFP_ZFMethodImplReplace_attach(ZF_IN const ZFListener &impl) {
    ZFCoreMutexLocker();
    _ZFP_ZFMethod_extInit();
    _ext->methodImplReplaceList.add(impl, 0);
    ZFBitSet(_stateFlags, _stateFlags_preferGenericInvoker);
    if(_ext->methodGenericInvokerOrig == zfnull) {
        _ext->methodGenericInvokerOrig = _methodGenericInvoker;
        _methodGenericInvoker = _ZFP_ZFMethodImplReplace_GI;
    }
}
void ZFMethod::_ZFP_ZFMethodImplReplace_detach(ZF_IN const ZFListener &impl) {
    ZFCoreMutexLocker();
    _ext->methodImplReplaceList.removeElement(impl);
    if(_ext->methodImplReplaceList.isEmpty()) {
        _methodGenericInvoker = _ext->methodGenericInvokerOrig;
        _ext->methodGenericInvokerOrig = zfnull;
        if(_methodInvoker) {
            ZFBitUnset(_stateFlags, _stateFlags_preferGenericInvoker);
        }
    }
}
ZFListener ZFMethodImplReplace(
        ZF_IN const ZFMethod *method
        , ZF_IN const ZFListener &impl
        ) {
    if(method && impl) {
        method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethodImplReplace_attach(impl);
        return impl;
    }
    else {
        return zfnull;
    }
}
void ZFMethodImplRestore(
        ZF_IN const ZFMethod *method
        , ZF_IN const ZFListener &impl
        ) {
    if(method && impl) {
        method->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethodImplReplace_detach(impl);
    }
}

ZF_NAMESPACE_GLOBAL_END

