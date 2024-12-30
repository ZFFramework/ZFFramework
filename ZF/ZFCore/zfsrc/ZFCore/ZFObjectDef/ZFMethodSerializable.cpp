#include "ZFMethodSerializable.h"
#include "ZFObjectImpl.h"

#include "ZFMethodDeclare.h"
#include "ZFMethodUserRegister.h"

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFMethod, const ZFMethod *, {
        ZFCoreArray<ZFIndexRange> pos;
        if(!ZFMethodSigSplit(pos, src, srcLen)) {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        v = ZFMethodFromSig(src, pos);
        return (v != zfnull);
    }, {
        if(v->isFuncType()) {
            if(v->methodNamespace()) {
                s += v->methodNamespace();
                s += ".";
            }
        }
        else {
            s += v->ownerClass()->classNameFull();
            s += ".";
        }

        s += v->methodName();

        for(zfindex i = 0; i < v->paramCount(); ++i) {
            s += ":";
            s += v->paramTypeIdAt(i);
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, const zfstring &, methodId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isUserRegister)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isDynamicRegister)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfany, dynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, const zfstring &, methodName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, const zfstring &, returnTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfindex, paramCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfindex, paramCountMin)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFMethod, zfbool, paramTypeIdIsMatch
        , ZFMP_IN_OPT(const zfchar *, paramTypeId0, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId5, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId6, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId7, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, zfbool, paramTypeIdIsMatch
        , ZFMP_IN(const ZFMethod *, method)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, const zfstring &, paramTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, const zfstring &, paramNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, const ZFListener &, paramDefaultValueCallbackAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, zfauto, paramDefaultValueAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, void, paramInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfstring, paramInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfindex, paramDefaultBeginIndex)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFFuncAddrType, methodInvoker)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, void, methodInvoker
        , ZFMP_IN(ZFFuncAddrType, methodInvoker)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFFuncAddrType, methodInvokerOrig)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFMethodGenericInvoker, methodGenericInvoker)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, zfauto, methodInvoke
        , ZFMP_IN_OPT(ZFObject *, ownerObjOrNull, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFMethod, zfauto, methodInvoke
        , ZFMP_IN(ZFObject *, ownerObjOrNull)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFMethod, zfbool, methodInvokeT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_OUT(zfstring *, errorHint)
        , ZFMP_IN(ZFObject *, ownerObjOrNull)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        // , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFMethod, zfauto, methodInvokeDetail
        , ZFMP_IN(ZFObject *, ownerObjOrNull)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, params)
        , ZFMP_OUT_OPT(zfbool *, success, zfnull)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFMethodGenericInvoker, methodGenericInvokerOrig)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethod, void, methodGenericInvoker
        , ZFMP_IN(ZFMethodGenericInvoker, methodGenericInvoker)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, const ZFClass *, ownerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFMethodAccessType, methodAccessType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isPublic)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isProtected)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isPrivate)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, ZFMethodType, methodType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isNormal)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isStatic)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isVirtual)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, zfbool, isFuncType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethod, const zfstring &, methodNamespace)

/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFMethodFromSig(
        ZF_IN const zfstring &classOrNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT const zfchar *paramTypeId0 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) {
    return ZFMethodForName(classOrNamespace, methodName
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
const ZFMethod *ZFMethodFromSig(
        ZF_IN const zfstring &methodSig
        , ZF_IN const ZFCoreArray<ZFIndexRange> &methodSigPos
        ) {
    if(methodSig == zfnull || methodSigPos.count() != ZFMETHOD_MAX_PARAM + 2) {
        return zfnull;
    }

    zfstring classOrNamespace(methodSig, methodSigPos[0].start, methodSigPos[0].count);
    zfstring methodName(methodSig, methodSigPos[1].start, methodSigPos[1].count);

    if(methodSigPos[2].count == 0) {
        return ZFMethodForName(classOrNamespace, methodName);
    }
    else {
        return ZFMethodForName(classOrNamespace, methodName
                , zfstring(methodSig, methodSigPos[2].start, methodSigPos[2].count)
                , zfstring(methodSig, methodSigPos[3].start, methodSigPos[3].count)
                , zfstring(methodSig, methodSigPos[4].start, methodSigPos[4].count)
                , zfstring(methodSig, methodSigPos[5].start, methodSigPos[5].count)
                , zfstring(methodSig, methodSigPos[6].start, methodSigPos[6].count)
                , zfstring(methodSig, methodSigPos[7].start, methodSigPos[7].count)
                , zfstring(methodSig, methodSigPos[8].start, methodSigPos[8].count)
                , zfstring(methodSig, methodSigPos[9].start, methodSigPos[9].count)
            );
    }
}

zfbool ZFMethodSigSplit(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfstring &src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    ret.removeAll();
    ret.add(ZFIndexRangeZero()); // add placeholder for method scope
    if(!ZFCoreDataPairSplitString(
                ret,
                zfHint("desiredCountOrIndexMax")zfindexMax(),
                src, srcLen,
                zfHint("separatorTokens")":",
                zfHint("leftToken")zfnull, zfHint("rightToken")zfnull,
                zfHint("allowEmptyItem")zftrue)
            || ret.count() > 1 + ZFMETHOD_MAX_PARAM
            || ret.count() <= 1
            || ret[1].count == 0
            ) {
        return zffalse;
    }

    // [Scope0.Scope1.]methodName[:paramTypeId0]
    zfindex dotPos = zfstringFindReversely(src, ret[1].count, ".");
    if(dotPos == 0) { // .methodName
        ret[1].start += 1;
        ret[1].count -= 1;
    }
    else if(dotPos != zfindexMax()) { // [Scope0.]Scope1.methodName
        ret[0].start = 0;
        ret[0].count = dotPos;
        zfindex offset = dotPos + 1 - ret[1].start;
        ret[1].start += offset;
        ret[1].count -= offset;
    }

    while(ret.count() < (ZFMETHOD_MAX_PARAM + 2)) {
        ret.add(ZFIndexRangeZero());
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodFromSig
        , ZFMP_IN(const zfstring &, classOrNamespace)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const zfchar *, paramTypeId0)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId1, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId2, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId3, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId4, zfnull)
        , ZFMP_IN_OPT(const zfchar *, paramTypeId5, zfnull)
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, paramTypeId6, zfnull) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, paramTypeId7, zfnull) */
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodFromSig
        , ZFMP_IN(const zfstring &, methodSig)
        , ZFMP_IN(const ZFCoreArray<ZFIndexRange> &, methodSigPos)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFMethodSigSplit
        , ZFMP_IN_OUT(ZFCoreArray<ZFIndexRange> &, ret)
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )

ZF_NAMESPACE_GLOBAL_END
#endif

