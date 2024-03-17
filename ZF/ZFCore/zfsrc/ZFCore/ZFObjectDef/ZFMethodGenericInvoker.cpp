#include "ZFMethodGenericInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
zfclass _ZFP_I_ZFMethodGenericInvokerDefaultParamType : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFMethodGenericInvokerDefaultParamType, ZFObject)
};
ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodGenericInvokerDefaultParamInit, ZFLevelZFFrameworkEssential) {
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfAlloc(_ZFP_I_ZFMethodGenericInvokerDefaultParamType);
    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = _ZFP_ZFMethodGenericInvokerDefaultParamRef;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodGenericInvokerDefaultParamInit) {
    zfRelease(_ZFP_ZFMethodGenericInvokerDefaultParamRef);
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;

    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodGenericInvokerDefaultParamInit)

zfauto _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;

// ============================================================
static void _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch(
        ZF_OUT zfstring *errorHint
        , ZF_IN zfindex paramCount
        , ZF_IN zfindex paramCountMin
        , ZF_IN_OPT const ZFClass *paramType0 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType1 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType2 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType3 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType4 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType5 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType6 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType7 /* = zfnull */
        ) {
    zfindex paramCountMax = ZFMETHOD_MAX_PARAM;
    do {
        if(paramType0 == zfnull) {paramCountMax = 0; break;}
        if(paramType1 == zfnull) {paramCountMax = 1; break;}
        if(paramType2 == zfnull) {paramCountMax = 2; break;}
        if(paramType3 == zfnull) {paramCountMax = 3; break;}
        if(paramType4 == zfnull) {paramCountMax = 4; break;}
        if(paramType5 == zfnull) {paramCountMax = 5; break;}
        if(paramType6 == zfnull) {paramCountMax = 6; break;}
        if(paramType7 == zfnull) {paramCountMax = 7; break;}
    } while(zffalse);
    if(paramCountMin == paramCountMax) {
        zfstringAppend(errorHint
                , "expect %s param, got %s"
                , paramCountMin
                , paramCount
                );
    }
    else {
        zfstringAppend(errorHint
                , "expect %s~%s param, got %s"
                , paramCountMin
                , paramCountMax
                , paramCount
                );
    }
}
zfbool ZFMethodGenericInvokerParamsCheck(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_IN zfindex paramCountMin
        , ZF_IN_OPT const ZFClass *paramType0 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType1 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType2 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType3 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType4 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType5 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType6 /* = zfnull */
        , ZF_IN_OPT const ZFClass *paramType7 /* = zfnull */
        ) {
    if(paramCount < paramCountMin) {
        if(errorHint != zfnull) {
            _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch(
                    errorHint
                    , paramCount
                    , paramCountMin
                    , paramType0
                    , paramType1
                    , paramType2
                    , paramType3
                    , paramType4
                    , paramType5
                    , paramType6
                    , paramType7
                    );
        }
        return zffalse;
    }
#define _ZFP_ZFMethodGenericInvokerParamsCheck_loop(N) \
    if(paramType##N != zfnull) { \
        if(paramCount <= N) { \
            return zftrue; \
        } \
        ZFObject *p = paramList[N]; \
        if(p != zfnull && !p->classData()->classIsTypeOf(paramType##N)) { \
            if(errorHint != zfnull) { \
                zfstringAppend(errorHint \
                        , "unable to access param%s as type (%s): (%s)%s" \
                        , N \
                        , paramType##N->className() \
                        , p->classData()->className() \
                        , p \
                        ); \
            } \
            return zffalse; \
        } \
    } \
    else { \
        if(paramCount <= N) { \
            return zftrue; \
        } \
        if(errorHint != zfnull) { \
            _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch( \
                    errorHint \
                    , paramCount \
                    , paramCountMin \
                    , paramType0 \
                    , paramType1 \
                    , paramType2 \
                    , paramType3 \
                    , paramType4 \
                    , paramType5 \
                    , paramType6 \
                    , paramType7 \
                    ); \
        } \
        return zffalse; \
    }
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(0)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(1)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(2)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(3)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(4)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(5)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(6)
    _ZFP_ZFMethodGenericInvokerParamsCheck_loop(7)
#undef _ZFP_ZFMethodGenericInvokerParamsCheck_loop
    return zftrue;
}
zfbool ZFMethodGenericInvokerParamsCheckWithMethod(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_IN const ZFMethod *method
        ) {
    if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount()) {
        if(errorHint != zfnull) {
            if(method->methodParamCountMin() == method->methodParamCount()) {
                zfstringAppend(errorHint
                        , "expect %s param, got %s"
                        , method->methodParamCount()
                        , paramCount
                        );
            }
            else {
                zfstringAppend(errorHint
                        , "expect %s~%s param, got %s"
                        , method->methodParamCountMin()
                        , method->methodParamCount()
                        , paramCount
                        );
            }
        }
        return zffalse;
    }
    const ZFClass *paramTypes[ZFMETHOD_MAX_PARAM];
    zfmemset(paramTypes, 0, sizeof(const ZFClass *) * ZFMETHOD_MAX_PARAM);
    for(zfindex i = 0; i < paramCount; ++i) {
        const ZFClass *cls = ZFClass::classForName(method->methodParamTypeIdAt(i));
        if(cls == zfnull) {
            const ZFTypeInfo *typeInfo = ZFTypeInfoForName(method->methodParamTypeIdAt(i));
            if(typeInfo == zfnull || typeInfo->typeIdClass() == zfnull) {
                zfstringAppend(errorHint
                        , "unable to access param%s's typeInfo, typeId: %s, typeInfo: typeInfo"
                        , i
                        , method->methodParamTypeIdAt(i)
                        );
                return zffalse;
            }
            cls = typeInfo->typeIdClass();
        }
        paramTypes[i] = cls;
    }
    return ZFMethodGenericInvokerParamsCheck(
            errorHint
            , paramCount
            , paramList
            , method->methodParamCountMin()
            , paramTypes[0]
            , paramTypes[1]
            , paramTypes[2]
            , paramTypes[3]
            , paramTypes[4]
            , paramTypes[5]
            , paramTypes[6]
            , paramTypes[7]
            );
}

// ============================================================
zfbool _ZFP_MtdGIParamCheck(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfbool zfvAccessAvailable
        , ZF_IN const ZFMethod *invokerMethod
        , ZF_IN zfindex paramIndex
        , ZF_IN const zfchar *paramType
        , ZF_IN ZFObject *param
        ) {
    if((param != ZFMethodGenericInvokerDefaultParam() && !zfvAccessAvailable)
            || (param == ZFMethodGenericInvokerDefaultParam() && paramIndex < invokerMethod->methodParamDefaultBeginIndex())
            ) {
        if(errorHint != zfnull) {
            zfstringAppend(errorHint,
                    "unable to access param%s as type (%s): (%s)%s",
                    paramIndex,
                    paramType,
                    param != zfnull ? param->classData()->className() : ZFTOKEN_zfnull,
                    param
                    );
        }
        return zffalse;
    }
    else {
        return zftrue;
    }
}
void _ZFP_MtdGIRetError(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnValueInfo
        ) {
    zfstringAppend(errorHint,
        "unable to convert return value as type %s: %s",
        returnTypeId,
        returnValueInfo);
}

// ============================================================
zfbool _ZFP_ZFMethodGenericInvoke(
        ZF_IN const ZFMethod *invokerMethod
        , ZF_IN ZFObject *invokerObject
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_OUT zfauto &ret
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        ) {
    return invokerMethod->methodGenericInvoker()(
            invokerMethod
            , invokerObject
            , errorHint
            , ret
            , paramCount
            , paramList
        );
}
void _ZFP_ZFMethodGenericInvokeError(
        ZF_IN const ZFMethod *method
        , ZF_IN ZFObject *obj
        , ZF_IN zfint pos
        , ZF_IN_OPT const zfchar *errorHint /* = zfnull */
        ) {
    if(pos >= 0) {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to convert param %s",
            method,
            pos);
    }
    else if(pos == -1) {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to perform generic invoker, reason: %s",
            method,
            errorHint);
    }
    else {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to convert return value",
            method);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return ZFMethodGenericInvokerDefaultParam();}, ZFObject *, ZFMethodGenericInvokerDefaultParam)

ZF_NAMESPACE_GLOBAL_END
#endif

