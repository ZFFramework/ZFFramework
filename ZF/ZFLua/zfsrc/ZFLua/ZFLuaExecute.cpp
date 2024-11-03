#include "ZFLuaExecute.h"
#include "ZFLuaState.h"
#include "ZFLuaGC.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_1(zfauto, ZFLuaExecute
        , ZFMP_IN(const ZFInput &, input)
        ) {
    return ZFLuaExecuteDetail(input, ZFCoreArray<zfauto>());
}
ZFMETHOD_FUNC_DEFINE_1(zfauto, ZFLuaExecute
        , ZFMP_IN(const zfchar *, buf)
        ) {
    return ZFLuaExecuteDetail(buf, ZFCoreArray<zfauto>());
}

ZFMETHOD_FUNC_DEFINE_8(zfauto, ZFLuaExecute
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        ) {
    ZFCoreArray<zfauto> params;
    do {
        if(param0 == ZFMP_DEF()) {break;} else {params.add(param0);}
        if(param1 == ZFMP_DEF()) {break;} else {params.add(param1);}
        if(param2 == ZFMP_DEF()) {break;} else {params.add(param2);}
        if(param3 == ZFMP_DEF()) {break;} else {params.add(param3);}
        if(param4 == ZFMP_DEF()) {break;} else {params.add(param4);}
        if(param5 == ZFMP_DEF()) {break;} else {params.add(param5);}
        if(param6 == ZFMP_DEF()) {break;} else {params.add(param6);}
        // if(param7 == ZFMP_DEF()) {break;} else {params.add(param7);}
    } while(zffalse);
    return ZFLuaExecuteDetail(input, params);
}
ZFMETHOD_FUNC_DEFINE_8(zfauto, ZFLuaExecute
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        ) {
    ZFCoreArray<zfauto> params;
    do {
        if(param0 == ZFMP_DEF()) {break;} else {params.add(param0);}
        if(param1 == ZFMP_DEF()) {break;} else {params.add(param1);}
        if(param2 == ZFMP_DEF()) {break;} else {params.add(param2);}
        if(param3 == ZFMP_DEF()) {break;} else {params.add(param3);}
        if(param4 == ZFMP_DEF()) {break;} else {params.add(param4);}
        if(param5 == ZFMP_DEF()) {break;} else {params.add(param5);}
        if(param6 == ZFMP_DEF()) {break;} else {params.add(param6);}
        // if(param7 == ZFMP_DEF()) {break;} else {params.add(param7);}
    } while(zffalse);
    return ZFLuaExecuteDetail(buf, params);
}

// ============================================================
static zfbool _ZFP_ZFLuaExecute(
        ZF_IN const ZFPathInfo &pathInfoOrNull
        , ZF_IN const ZFInput &input
        , ZF_IN zfauto *luaResult
        , ZF_IN const ZFCoreArray<zfauto> *luaParams
        , ZF_IN void *L
        ) {
    if(!input) {
        return zffalse;
    }
    zfstring errHint;
    if(!ZFPROTOCOL_ACCESS(ZFLua)->luaExecute(L ? L : ZFLuaState(), pathInfoOrNull, input, luaResult, luaParams, &errHint)) {
        if(!errHint.isEmpty()) {
            ZFLuaErrorOccurredTrim("%s", errHint);
        }
        return zffalse;
    }
    else {
        return zftrue;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfauto, ZFLuaExecuteDetail
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, luaParams)
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaState();
    }
    zfauto ret;
    if(_ZFP_ZFLuaExecute(input.pathInfo(), input, &ret, &luaParams, L)) {
        ZFLuaGC(L);
        return ret;
    }
    else {
        return zfnull;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfauto, ZFLuaExecuteDetail
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, luaParams)
        , ZFMP_IN_OPT(void *, L, zfnull)
        ) {
    if(L == zfnull) {
        L = ZFLuaState();
    }
    zfauto ret;
    if(_ZFP_ZFLuaExecute(zfnull, ZFInputForBufferUnsafe(buf), &ret, &luaParams, L)) {
        ZFLuaGC(L);
        return ret;
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

