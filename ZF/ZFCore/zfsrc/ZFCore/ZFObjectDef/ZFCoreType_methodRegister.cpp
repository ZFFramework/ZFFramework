#include "ZFCoreType_methodRegister.h"
#include "ZFTypeId_CoreType.h"
#include "ZFDynamicInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_MAIN, ZF_ENV_ZFVERSION_MAIN())
ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_SUB, ZF_ENV_ZFVERSION_SUB())
ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_MINOR, ZF_ENV_ZFVERSION_MINOR())
ZFEXPORT_VAR_READONLY_DEFINE(const zfchar *, ZF_ENV_ZFVERSION, ZF_ENV_ZFVERSION())

// ============================================================
static zfuint _ZFP_ZFBitCast(ZF_IN ZFObject *v) {
    if(v == zfnull) {
        return 0;
    }
    const ZFClass *cls = v->classData();
    if(cls->classIsTypeOf(ZFEnum::ClassData())) {
        return (zfuint)zfcast(ZFEnum *, v)->enumValue();
    }
    else if(cls->classIsTypeOf(v_zfuint::ClassData())) {
        return (zfuint)zfcast(v_zfuint *, v)->zfv;
    }
    else if(cls->classIsTypeOf(v_zfflags::ClassData())) {
        return (zfuint)zfcast(v_zfflags *, v)->zfv;
    }
    else if(cls->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        zfstring tmp;
        zfcast(ZFTypeIdWrapper *, v)->zfvToString(tmp);
        return zfuintFromString(tmp);
    }
    else if(cls->classIsTypeOf(ZFDI_WrapperBase::ClassData())) {
        return zfuintFromString(zfcast(ZFDI_WrapperBase *, v)->zfv());
    }
    else {
        return 0;
    }
}
static void _ZFP_ZFBitStore(ZF_IN ZFTypeIdWrapper *v, ZF_IN zfuint t) {
    if(v == zfnull) {
        return;
    }
    const ZFClass *cls = v->classData();
    if(cls->classIsTypeOf(ZFEnum::ClassData())) {
        zfcast(ZFEnum *, v)->enumValue(t);
    }
    else if(cls->classIsTypeOf(v_zfuint::ClassData())) {
        zfcast(v_zfuint *, v)->zfv = t;
    }
    else if(cls->classIsTypeOf(v_zfflags::ClassData())) {
        zfcast(v_zfflags *, v)->zfv = (zfflags)t;
    }
    else {
        zfstring tmp;
        zfuintToStringT(tmp, t);
        v->zfvFromString(tmp);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFBitTest
        , ZFMP_IN(ZFObject *, v)
        , ZFMP_IN(ZFObject *, t)
        ) {
    return ZFBitTest(_ZFP_ZFBitCast(v), _ZFP_ZFBitCast(t));
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFBitTestAll
        , ZFMP_IN(ZFObject *, v)
        , ZFMP_IN(ZFObject *, t)
        ) {
    return ZFBitTestAll(_ZFP_ZFBitCast(v), _ZFP_ZFBitCast(t));
}
ZFMETHOD_FUNC_DEFINE_2(zfuint, ZFBitGet
        , ZFMP_IN(ZFObject *, v)
        , ZFMP_IN(ZFObject *, t)
        ) {
    return ZFBitGet(_ZFP_ZFBitCast(v), _ZFP_ZFBitCast(t));
}
ZFMETHOD_FUNC_DEFINE_2(zfuint, ZFBitSet
        , ZFMP_IN(ZFTypeIdWrapper *, v)
        , ZFMP_IN(ZFObject *, t)
        ) {
    zfuint ret = _ZFP_ZFBitCast(v);
    ZFBitSet(ret, _ZFP_ZFBitCast(t));
    _ZFP_ZFBitStore(v, ret);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfuint, ZFBitUnset
        , ZFMP_IN(ZFTypeIdWrapper *, v)
        , ZFMP_IN(ZFObject *, t)
        ) {
    zfuint ret = _ZFP_ZFBitCast(v);
    ZFBitUnset(ret, _ZFP_ZFBitCast(t));
    _ZFP_ZFBitStore(v, ret);
    return ret;
}

// ============================================================
// ZFCoreArgSplit
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFCoreArgSplit
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, result)
        , ZFMP_IN(const zfchar *, cmdLine)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArray<zfstring>, ZFCoreArgSplit
        , ZFMP_IN(const zfchar *, cmdLine)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFCoreArgMerge
        , ZFMP_OUT(zfstring &, result)
        , ZFMP_IN(const ZFCoreArray<zfstring> &, argList)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFCoreArgMerge
        , ZFMP_IN(const ZFCoreArray<zfstring> &, argList)
        )

// ============================================================
// ZFCoreDataEncode
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const zfchar *, ZFCoreDataEncodeCharMapDefault)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(void, ZFCoreDataEncode
        , ZFMP_OUT(zfstring &, result)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(const zfchar *, charMap, ZFCoreDataEncodeCharMapDefault())
        , ZFMP_IN_OPT(zfchar, escapeToken, '%')
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFCoreDataDecode
        , ZFMP_OUT(zfstring &, result)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfchar, escapeToken, '%')
        )

// ============================================================
// ZFCoreDataPairSplit
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, ZFCoreDataPairSplitString
        , ZFMP_IN_OUT(ZFCoreArray<ZFIndexRange> &, outData)
        , ZFMP_IN(zfindex, desiredCountOrIndexMax)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(const zfchar *, separatorTokens, ",")
        , ZFMP_IN_OPT(const zfchar *, leftTokens, "(")
        , ZFMP_IN_OPT(const zfchar *, rightTokens, ")")
        , ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, ZFCoreDataPairSplitInt
        , ZFMP_IN_OUT(ZFCoreArray<zfint> &, outData)
        , ZFMP_IN(zfindex, desiredCountOrIndexMax)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(const zfchar *, separatorTokens, ",")
        , ZFMP_IN_OPT(const zfchar *, leftTokens, "(")
        , ZFMP_IN_OPT(const zfchar *, rightTokens, ")")
        , ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, ZFCoreDataPairSplitFloat
        , ZFMP_IN_OUT(ZFCoreArray<zffloat> &, outData)
        , ZFMP_IN(zfindex, desiredCountOrIndexMax)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(const zfchar *, separatorTokens, ",")
        , ZFMP_IN_OPT(const zfchar *, leftTokens, "(")
        , ZFMP_IN_OPT(const zfchar *, rightTokens, ")")
        , ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse)
        )

// ============================================================
// ZFIdentityGenerator
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, zfidentityCalcString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, zfidentityCalcBuf
        , ZFMP_IN(const void *, src)
        , ZFMP_IN(zfindex, srcLen)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, zfidentityCalc
        , ZFMP_IN(const void *, p)
        )

// ============================================================
// ZFCoreStatistic
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, invokeCountLog
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, invokeCountRemove
        , ZFMP_IN(const zfstring &, key)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, invokeCountRemoveAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfindex, invokeCountGet
        , ZFMP_IN(const zfstring &, key)
        )
ZF_NAMESPACE_END(ZFCoreStatistic)

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, zfstringIsEqual
        , ZFMP_IN(const zfchar *, s1)
        , ZFMP_IN(const zfchar *, s2)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, zfstringIsEqual
        , ZFMP_IN(const zfchar *, s1)
        , ZFMP_IN(zfindex, s1Len)
        , ZFMP_IN(const zfchar *, s2)
        , ZFMP_IN(zfindex, s2Len)
        )
ZFMETHOD_FUNC_USER_REGISTER_2({
        v_zfstring *t1 = zfcast(v_zfstring *, s1);
        v_zfstring *t2 = zfcast(v_zfstring *, s2);
        return ((t1 == zfnull || t1->zfv.isEmpty()) && t2 == zfnull || t2->zfv.isEmpty()) || ZFObjectCompare(t1, t2) == ZFCompareEqual;
    }, zfbool, zfstringIsEqual
    , ZFMP_IN(ZFObject *, s1)
    , ZFMP_IN(ZFObject *, s2)
    )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfbool, zfstringIsEmpty
        , ZFMP_IN(const zfchar *, s1)
        )
ZFMETHOD_FUNC_USER_REGISTER_1({
        v_zfstring *t = zfcast(v_zfstring *, s1);
        return t == zfnull || t->zfv.isEmpty();
    }, zfbool, zfstringIsEmpty
    , ZFMP_IN(ZFObject *, s1)
    )

// ============================================================
// zfstringFind
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFind
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFind
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindReversely
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindReversely
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindCaseInsensitive
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindCaseInsensitive
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindCaseInsensitiveReversely
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindCaseInsensitiveReversely
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindFirstOf
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindFirstOf
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindFirstNotOf
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindFirstNotOf
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindLastOf
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindLastOf
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindLastNotOf
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindLastNotOf
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(zfstring, zfstringReplace
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, replaceFrom)
        , ZFMP_IN(const zfchar *, replaceTo)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_OUT_OPT(zfindex *, replacedCount, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(zfstring, zfstringReplaceReversely
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, replaceFrom)
        , ZFMP_IN(const zfchar *, replaceTo)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_OUT_OPT(zfindex *, replacedCount, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, zfstringBeginWith
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, zfstringBeginWith
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, zfstringEndWith
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(zfindex, srcLen)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, zfstringEndWith
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN(const zfchar *, find)
        , ZFMP_IN_OPT(zfindex, findLen, zfindexMax())
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, zfstringSplitT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, separator)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCoreArray<zfstring>, zfstringSplit
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, separator)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, zfstringSplitIndexT
        , ZFMP_OUT(ZFCoreArray<ZFIndexRange> &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, separator)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCoreArray<ZFIndexRange>, zfstringSplitIndex
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfchar *, separator)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfstringToLowerT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, zfstringToLowerT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, zfstringToLower
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, zfstringToUpperT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, zfstringToUpperT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, zfstringToUpper
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, zfstringRepeatT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, token)
        , ZFMP_IN(zfindex, count)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, zfstringRepeat
        , ZFMP_IN(const zfchar *, token)
        , ZFMP_IN(zfindex, count)
        )

// ============================================================
// namespace
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(const zfchar *, ZFNamespaceSkipGlobal
        , ZFMP_IN(const zfchar *, ns)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFNamespaceSplit
        , ZFMP_IN_OUT(ZFCoreArray<ZFIndexRange> &, ret)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFNamespaceGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<zfstring>, ZFNamespaceGetAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFNamespaceGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
        , ZFMP_IN(const zfchar *, parent)
        , ZFMP_IN_OPT(zfbool, recursive, zffalse)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<zfstring>, ZFNamespaceGetAll
        , ZFMP_IN(const zfchar *, parent)
        , ZFMP_IN_OPT(zfbool, recursive, zffalse)
        )

// ============================================================
// core type
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, zfindexMax, zfindexMax())
ZFEXPORT_VAR_READONLY_DEFINE(zftimet, zftimetInvalid, zftimetInvalid())
ZFEXPORT_VAR_READONLY_DEFINE(zfidentity, zfidentityInvalid, zfidentityInvalid())

ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFIndexRange, ZFIndexRangeZero)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFIndexRange, ZFIndexRangeMax)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFIndexRange, ZFIndexRangeCreate
        , ZFMP_IN(zfindex, start)
        , ZFMP_IN(zfindex, count)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFIndexRangeContain
        , ZFMP_IN(const ZFIndexRange &, range)
        , ZFMP_IN(zfindex, index)
        )

// ============================================================
// ZFCoreUtilMath
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmMax
        , ZFMP_IN(zfint const &, n0)
        , ZFMP_IN(zfint const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfuint, zfmMax
        , ZFMP_IN(zfuint const &, n0)
        , ZFMP_IN(zfuint const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfmMax
        , ZFMP_IN(zfindex const &, n0)
        , ZFMP_IN(zfindex const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zffloat, zfmMax
        , ZFMP_IN(zffloat const &, n0)
        , ZFMP_IN(zffloat const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zftimet, zfmMax
        , ZFMP_IN(zftimet const &, n0)
        , ZFMP_IN(zftimet const &, n1)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmMin
        , ZFMP_IN(zfint const &, n0)
        , ZFMP_IN(zfint const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfuint, zfmMin
        , ZFMP_IN(zfuint const &, n0)
        , ZFMP_IN(zfuint const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfmMin
        , ZFMP_IN(zfindex const &, n0)
        , ZFMP_IN(zfindex const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zffloat, zfmMin
        , ZFMP_IN(zffloat const &, n0)
        , ZFMP_IN(zffloat const &, n1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zftimet, zfmMin
        , ZFMP_IN(zftimet const &, n0)
        , ZFMP_IN(zftimet const &, n1)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfint, zfmAbs
        , ZFMP_IN(zfint const &, n0)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfuint, zfmAbs
        , ZFMP_IN(zfuint const &, n0)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfindex, zfmAbs
        , ZFMP_IN(zfindex const &, n0)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmAbs
        , ZFMP_IN(zffloat const &, n0)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zftimet, zfmAbs
        , ZFMP_IN(zftimet const &, n0)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfint, zfmApplyRange
        , ZFMP_IN(zfint const &, n)
        , ZFMP_IN(zfint const &, nMin)
        , ZFMP_IN(zfint const &, nMax)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfuint, zfmApplyRange
        , ZFMP_IN(zfuint const &, n)
        , ZFMP_IN(zfuint const &, nMin)
        , ZFMP_IN(zfuint const &, nMax)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfmApplyRange
        , ZFMP_IN(zfindex const &, n)
        , ZFMP_IN(zfindex const &, nMin)
        , ZFMP_IN(zfindex const &, nMax)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zffloat, zfmApplyRange
        , ZFMP_IN(zffloat const &, n)
        , ZFMP_IN(zffloat const &, nMin)
        , ZFMP_IN(zffloat const &, nMax)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zftimet, zfmApplyRange
        , ZFMP_IN(zftimet const &, n)
        , ZFMP_IN(zftimet const &, nMin)
        , ZFMP_IN(zftimet const &, nMax)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfint, zfmApplyProgress
        , ZFMP_IN(zfint const &, start)
        , ZFMP_IN(zfint const &, end)
        , ZFMP_IN(zffloat const &, progress)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfuint, zfmApplyProgress
        , ZFMP_IN(zfuint const &, start)
        , ZFMP_IN(zfuint const &, end)
        , ZFMP_IN(zffloat const &, progress)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfmApplyProgress
        , ZFMP_IN(zfindex const &, start)
        , ZFMP_IN(zfindex const &, end)
        , ZFMP_IN(zffloat const &, progress)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zffloat, zfmApplyProgress
        , ZFMP_IN(zffloat const &, start)
        , ZFMP_IN(zffloat const &, end)
        , ZFMP_IN(zffloat const &, progress)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zftimet, zfmApplyProgress
        , ZFMP_IN(zftimet const &, start)
        , ZFMP_IN(zftimet const &, end)
        , ZFMP_IN(zffloat const &, progress)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRoundUp
        , ZFMP_IN(zffloat const &, v)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRoundDown
        , ZFMP_IN(zffloat const &, v)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRound
        , ZFMP_IN(zffloat const &, v)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zffloat, zfmRandFloat)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zfint, zfmRandMax)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zfint, zfmRand)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfint, zfmRand
        , ZFMP_IN(zfint const &, range)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmRand
        , ZFMP_IN(zfint const &, start)
        , ZFMP_IN(zfint const &, end)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfuint, zfmRand
        , ZFMP_IN(zfuint const &, range)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfuint, zfmRand
        , ZFMP_IN(zfuint const &, start)
        , ZFMP_IN(zfuint const &, end)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfindex, zfmRand
        , ZFMP_IN(zfindex const &, range)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfmRand
        , ZFMP_IN(zfindex const &, start)
        , ZFMP_IN(zfindex const &, end)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRand
        , ZFMP_IN(zffloat const &, range)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zffloat, zfmRand
        , ZFMP_IN(zffloat const &, start)
        , ZFMP_IN(zffloat const &, end)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zftimet, zfmRand
        , ZFMP_IN(zftimet const &, range)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zftimet, zfmRand
        , ZFMP_IN(zftimet const &, start)
        , ZFMP_IN(zftimet const &, end)
        )

// ============================================================
// ZFVersion
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFVersionGet
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const zfchar *, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFVersionGet
        , ZFMP_IN(const zfchar *, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFVersionGetInt
        , ZFMP_OUT(zfuint &, ret)
        , ZFMP_IN(const zfchar *, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfuint, ZFVersionGetInt
        , ZFMP_IN(const zfchar *, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFVersionSet
        , ZFMP_IN_OUT(zfstring &, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        , ZFMP_IN(const zfchar *, subVersion)
        , ZFMP_IN_OPT(const zfchar *, emptySubVersion, "0")
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFVersionSetInt
        , ZFMP_IN_OUT(zfstring &, version)
        , ZFMP_IN(zfindex, subVersionIndex)
        , ZFMP_IN(zfuint, subVersion)
        , ZFMP_IN_OPT(const zfchar *, emptySubVersion, "0")
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, ZFVersionCompare
        , ZFMP_IN(const zfchar *, version0)
        , ZFMP_IN(const zfchar *, version1)
        )
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_MAIN, ZFVERSION_MAIN())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_SUB, ZFVERSION_SUB())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_MINOR, ZFVERSION_MINOR())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_BUILD, ZFVERSION_BUILD())

// ============================================================
// ZFTokenForContainer
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerTrim)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerDetail)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerPlainList)
ZFEXPORT_VAR_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerDefault)

ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerTrim)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDetail)
ZFEXPORT_VAR_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDefault)

ZF_NAMESPACE_GLOBAL_END

