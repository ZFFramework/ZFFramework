#include "ZFTypeId_ZFCoreArray.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_ZFTYPEID_ID_DATA_REGISTER(ZFCoreArray, ZFCoreArray<zfauto>)

void v_ZFCoreArray::objectInfoT(ZF_IN_OUT zfstring &ret) {
    this->zfv.objectInfoT(ret);
}

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, void, copyFrom
        , ZFMP_IN(const ZFCoreArray<zfauto> &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, ZFCompareResult, objectCompare
        , ZFMP_IN(const ZFCoreArray<zfauto> &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFCoreArray, void, objectInfoOfContentT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCoreArray, zfstring, objectInfoOfContent
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfbool, isPODType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, void, capacity
        , ZFMP_IN(zfindex, newCapacity)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, void, capacityTrim)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfindex, capacity)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, void, add
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCoreArray, void, add
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, void, addFrom
        , ZFMP_IN(const ZFCoreArray<zfauto> &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfindex, find
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfindex, findReversely
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfbool, removeElement
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfbool, removeElementReversely
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfindex, removeElementAll
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, void, remove
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCoreArray, void, remove
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(zfindex, count)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, void, removeFirst)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfauto, removeFirstAndGet)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, void, removeLast)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfauto, removeLastAndGet)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCoreArray, void, move
        , ZFMP_IN(zfindex, fromIndex)
        , ZFMP_IN(zfindex, toIndexOrIndexMax)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCoreArray, void, set
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(zfauto const &, e)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCoreArray, zfauto &, get
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfauto const &, getFirst)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfauto const &, getLast)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfindex, count)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCoreArray, zfbool, isEmpty)

ZF_NAMESPACE_GLOBAL_END

