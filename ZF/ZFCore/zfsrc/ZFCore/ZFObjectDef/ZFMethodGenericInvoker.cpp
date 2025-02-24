#include "ZFMethodGenericInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
zfclass _ZFP_I_ZFMP_DEF_Type : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFMP_DEF_Type, ZFObject)
};
ZFObject *_ZFP_ZFMP_DEF = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMP_DEF_Init, ZFLevelZFFrameworkEssential) {
    _ZFP_ZFMP_DEF = zfAlloc(_ZFP_I_ZFMP_DEF_Type);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMP_DEF_Init) {
    zfRelease(_ZFP_ZFMP_DEF);
    _ZFP_ZFMP_DEF = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFMP_DEF_Init)

// ============================================================
static void _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN zfindex paramCount
        ) {
    if(paramCount == zfindexMax()) {
        paramCount = ZFMETHOD_MAX_PARAM;
        for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) {
            if(zfargs.paramAt(i) == ZFMP_DEF()) {
                paramCount = i;
                break;
            }
        }
    }
    zfargs.success(zffalse);
    if(!zfargs.ignoreError()) {
        if(zfargs.ownerMethod()->paramDefaultBeginIndex() == zfindexMax()) {
            zfargs.errorHint(zfstr("expect %s param, got %s"
                        , zfargs.ownerMethod()->paramCount()
                        , paramCount
                        ));
        }
        else {
            zfargs.errorHint(zfstr("expect %s~%s param, got %s"
                        , zfargs.ownerMethod()->paramCountMin()
                        , zfargs.ownerMethod()->paramCount()
                        , paramCount
                        ));
        }
    }
}
static void _ZFP_ZFMethodGenericInvokerParamsCheck_paramTypeMismatch(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN zfindex N
        ) {
    zfargs.success(zffalse);
    if(!zfargs.ignoreError()) {
        ZFObject *p = zfargs.paramAt(N);
        zfargs.errorHint(zfstr("unable to access param%s as type (%s): (%s)%s"
                    , N
                    , zfargs.ownerMethod()->paramTypeIdAt(N)
                    , p ? p->classData()->classNameFull().cString() : ZFTOKEN_zfnull
                    , p
                    ));
    }
}
zfbool ZFMethodGenericInvokerParamsCheck(ZF_IN_OUT const ZFArgs &zfargs) {
#define _ZFP_ZFMethodGenericInvokerParamsCheck_loop(N) \
    { \
        ZFObject *p = zfargs.paramAt(N); \
        if(p == ZFMP_DEF()) { \
            if(N >= zfargs.ownerMethod()->paramCountMin()) { \
                return zftrue; \
            } \
            else { \
                _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch(zfargs, N); \
                return zffalse; \
            } \
        } \
        else if(N >= zfargs.ownerMethod()->paramCount()) { \
            _ZFP_ZFMethodGenericInvokerParamsCheck_paramCountMismatch(zfargs, zfindexMax()); \
            return zffalse; \
        } \
        else if(p != zfnull) { \
            const ZFClass *desired = ZFClass::classForName(zfargs.ownerMethod()->paramTypeIdAt(N)); \
            if(!p->classData()->classIsTypeOf(desired)) { \
                _ZFP_ZFMethodGenericInvokerParamsCheck_paramTypeMismatch(zfargs, N); \
                return zffalse; \
            } \
        } \
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

// ============================================================
zfbool _ZFP_MtdGIParamCheck(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN zfbool zfvAccessAvailable
        , ZF_IN zfindex paramIndex
        , ZF_IN const zfchar *paramType
        ) {
    ZFObject *param = zfargs.paramAt(paramIndex);
    if((param != ZFMP_DEF() && !zfvAccessAvailable)
            || (param == ZFMP_DEF() && paramIndex < zfargs.ownerMethod()->paramDefaultBeginIndex())
            ) {
        zfargs.success(zffalse);
        if(!zfargs.ignoreError()) {
            zfargs.errorHint(zfstr("unable to access param%s as type (%s): (%s)%s"
                        , paramIndex
                        , paramType
                        , (param != zfnull ? param->classData()->className().cString() : ZFTOKEN_zfnull)
                        , param
                        ));
        }
        return zffalse;
    }
    else {
        return zftrue;
    }
}

// ============================================================
void _ZFP_ZFMethodGenericInvoke(ZF_IN_OUT const ZFArgs &zfargs) {
    zfargs.ownerMethod()->methodGenericInvoker()(zfargs);
}
void _ZFP_ZFMethodGenericInvokeError(
        ZF_IN zfint pos
        , ZF_IN const ZFArgs &zfargs
        ) {
    if(pos >= 0) {
        ZFCoreCriticalMessageTrim("[ZFMethodDynamicRegister] method %s unable to convert param %s"
                , zfargs.ownerMethod()
                , pos
                );
    }
    else if(pos == -1) {
        ZFCoreCriticalMessageTrim("[ZFMethodDynamicRegister] method %s unable to perform generic invoker, reason: %s"
                , zfargs.ownerMethod()
                , zfargs.errorHint()
                );
    }
    else {
        ZFCoreCriticalMessageTrim("[ZFMethodDynamicRegister] method %s unable to convert return value"
                , zfargs.ownerMethod()
                );
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return ZFMP_DEF();}, ZFObject *, ZFMP_DEF)

ZF_NAMESPACE_GLOBAL_END
#endif

