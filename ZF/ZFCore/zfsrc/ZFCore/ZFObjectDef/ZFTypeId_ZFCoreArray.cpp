#include "ZFTypeId_ZFCoreArray.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool _ZFP_ZFCoreArrayFromStringT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFCoreArray<ZFIndexRange> pos;
    if(!ZFCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, ",", "[", "]", zftrue)) {
        return zffalse;
    }
    if(elementType.typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        for(zfindex i = 0; i < pos.count(); ++i) {
            zfstring elementString;
            ZFCoreDataDecode(elementString, zfstring(src + pos[i].start, pos[i].count));
            zfauto e = elementType.typeIdClass()->newInstance();
            ZFTypeIdWrapper *eTmp = e;
            if(eTmp == zfnull || !eTmp->zfvFromString(elementString, elementString.length(), errorHint)) {
                return zffalse;
            }
            void *eGeneric = elementType.genericAccess(e);
            if(eGeneric == zfnull) {
                return zffalse;
            }
            v.genericAdd(eGeneric);
            elementType.genericAccessFinish(e, eGeneric);
        }
    }
    else {
        for(zfindex i = 0; i < pos.count(); ++i) {
            zfstring elementString;
            ZFCoreDataDecode(elementString, zfstring(src + pos[i].start, pos[i].count));
            zfauto e = elementType.typeIdClass()->newInstance();
            ZFSerializable *eTmp = e;
            if(eTmp == zfnull || !eTmp->serializeFromString(elementString, elementString.length(), errorHint)) {
                return zffalse;
            }
            void *eGeneric = elementType.genericAccess(e);
            if(eGeneric == zfnull) {
                return zffalse;
            }
            v.genericAdd(eGeneric);
            elementType.genericAccessFinish(e, eGeneric);
        }
    }
    return zftrue;
}
zfbool _ZFP_ZFCoreArrayToStringT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_OUT zfstring &s
        , ZF_IN ZFCoreArrayBase const &v
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    s += "[";
    if(elementType.typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        for(zfindex i = 0; i < v.count(); ++i) {
            if(i != 0) {
                s += ",";
            }
            zfauto e;
            if(!elementType.genericValueStore(e, v.genericGet(i))) {
                return zffalse;
            }
            ZFTypeIdWrapper *eTmp = e;
            if(eTmp == zfnull) {
                return zffalse;
            }
            zfstring elementString;
            if(!eTmp->zfvToString(elementString, errorHint)) {
                return zffalse;
            }
            ZFCoreDataEncode(s, elementString.cString(), elementString.length());
        }
    }
    else {
        for(zfindex i = 0; i < v.count(); ++i) {
            if(i != 0) {
                s += ",";
            }
            zfauto e;
            if(!elementType.genericValueStore(e, v.genericGet(i))) {
                return zffalse;
            }
            ZFSerializable *eTmp = e;
            if(eTmp == zfnull) {
                return zffalse;
            }
            zfstring elementString;
            if(!eTmp->serializeToString(elementString, errorHint)) {
                return zffalse;
            }
            ZFCoreDataEncode(s, elementString.cString(), elementString.length());
        }
    }
    s += "]";
    return zftrue;
}
zfbool _ZFP_ZFCoreArrayFromDataT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_IN_OUT ZFCoreArrayBase &v
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    v.removeAll();
    if(!ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCoreArray(), outErrorHint, outErrorPos)) {
        return zffalse;
    }
    if(elementType.typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            const ZFSerializableData &element = serializableData.childAt(i);
            if(element.resolved()) {
                continue;
            }
            zfauto e = elementType.typeIdClass()->newInstance();
            ZFTypeIdWrapper *eTmp = e;
            if(eTmp == zfnull || !eTmp->zfvFromData(element, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            void *eGeneric = elementType.genericAccess(e);
            if(eGeneric == zfnull) {
                return zffalse;
            }
            v.genericAdd(eGeneric);
            elementType.genericAccessFinish(e, eGeneric);
        }
    }
    else {
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            const ZFSerializableData &element = serializableData.childAt(i);
            if(element.resolved()) {
                continue;
            }
            zfauto e = elementType.typeIdClass()->newInstance();
            ZFSerializable *eTmp = e;
            if(eTmp == zfnull || !eTmp->serializeFromData(element, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            void *eGeneric = elementType.genericAccess(e);
            if(eGeneric == zfnull) {
                return zffalse;
            }
            v.genericAdd(eGeneric);
            elementType.genericAccessFinish(e, eGeneric);
        }
    }
    return zftrue;
}
zfbool _ZFP_ZFCoreArrayToDataT(
        ZF_IN const ZFTypeInfo &elementType
        , ZF_OUT ZFSerializableData &serializableData
        , ZF_IN ZFCoreArrayBase const &v
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    serializableData.itemClass(ZFTypeId_ZFCoreArray());
    if(elementType.typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
        for(zfindex i = 0; i < v.count(); ++i) {
            zfauto e;
            if(!elementType.genericValueStore(e, v.genericGet(i))) {
                return zffalse;
            }
            ZFTypeIdWrapper *eTmp = e;
            if(eTmp == zfnull) {
                return zffalse;
            }
            ZFSerializableData element;
            if(!eTmp->zfvToData(element, outErrorHint)) {
                return zffalse;
            }
            serializableData.child(element);
        }
    }
    else {
        for(zfindex i = 0; i < v.count(); ++i) {
            zfauto e;
            if(!elementType.genericValueStore(e, v.genericGet(i))) {
                return zffalse;
            }
            ZFSerializable *eTmp = e;
            if(eTmp == zfnull) {
                return zffalse;
            }
            ZFSerializableData element;
            if(!eTmp->serializeToData(element, outErrorHint)) {
                return zffalse;
            }
            serializableData.child(element);
        }
    }
    return zftrue;
}

// ============================================================
zfbool v_ZFCoreArray::elementTypeInit(ZF_IN const zfstring &elementTypeId) {
    if(this->elementType != zfnull) {
        return zffalse;
    }

    const ZFTypeInfo *typeInfo = ZFTypeInfoForName(elementTypeId);
    if(typeInfo != zfnull) {
        this->zfv = typeInfo->genericArrayNew();
        if(this->zfv == zfnull) {
            return zffalse;
        }
        this->elementType = typeInfo;
        this->_ZFP_elementTypeHolder = zfnull;
        return zftrue;
    }
    else if(ZFClass::classForName(elementTypeId) != zfnull) {
        this->zfv = ZFCoreArray<zfauto>().refNew();
        ZFTypeInfo *t = zfpoolNew(ZFTypeId<zfauto>);
        this->_ZFP_elementTypeHolder = zfpoolNew(ZFCorePointerForPoolObject<ZFTypeInfo *>, t);
        this->elementType = this->_ZFP_elementTypeHolder->pointerValueT<const ZFTypeInfo *>();
        return zftrue;
    }
    else {
        return zffalse;
    }
}
zfauto v_ZFCoreArray::_ZFP_elementTypeCheck(ZF_IN ZFObject *element) {
    if(element == zfnull) {
        return zfnull;
    }
    if(this->elementType == zfnull) {
        ZFTypeIdWrapper *v = zfcast(ZFTypeIdWrapper *, element);
        if(v != zfnull) {
            if(!this->elementTypeInit(v->zfvTypeId())) {
                return zfnull;
            }
        }
        else {
            if(!this->elementTypeInit(ZFObject::ClassData()->classNameFull())) {
                return zfnull;
            }
        }
        return element;
    }
    if(this->elementType->typeIdClass() == zfnull) {
        return zfnull;
    }
    if(element->classData()->classIsTypeOf(this->elementType->typeIdClass())) {
        return element;
    }
    if(element->classData()->classIsTypeOf(v_zfstring::ClassData())
            && this->elementType->typeIdClass()->classIsTypeOf(ZFTypeIdWrapper::ClassData())
            ) {
        zfauto ret = this->elementType->typeIdClass()->newInstance();
        ZFTypeIdWrapper *retTmp = ret;
        v_zfstring *src = zfcast(v_zfstring *, element);
        if(retTmp->zfvFromString(src->zfv, src->zfv.length())) {
            return ret;
        }
    }
    return zfnull;
}

// ============================================================
typedef ZFCoreArray<zfauto> _ZFP_PropTypeW_ZFCoreArray;
_ZFP_ZFTYPEID_ID_DATA_REGISTER(ZFCoreArray, ZFCoreArray<zfauto>)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
    invokerObject->to<v_ZFCoreArray *>()->elementTypeInit(elementTypeId);
}, v_ZFCoreArray
, ZFMP_IN(const zfstring &, elementTypeId)
)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, swap
        , ZFMP_IN(v_ZFCoreArray *, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, copyFrom
        , ZFMP_IN(v_ZFCoreArray *, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(v_ZFCoreArray, void, objectInfoOfContentT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, zfstring, objectInfoOfContent
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, capacity
        , ZFMP_IN(zfindex, newCapacity)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, void, capacityTrim)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfindex, capacity)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, add
        , ZFMP_IN(ZFObject *, e)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, add
        , ZFMP_IN(ZFObject *, e)
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, addFrom
        , ZFMP_IN(v_ZFCoreArray *, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, zfindex, find
        , ZFMP_IN(ZFObject *, e)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, zfindex, findReversely
        , ZFMP_IN(ZFObject *, e)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, void, remove
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, remove
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(zfindex, count)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, zfauto, removeAndGet
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, void, removeFirst)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfauto, removeFirstAndGet)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, void, removeLast)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfauto, removeLastAndGet)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, move
        , ZFMP_IN(zfindex, fromIndex)
        , ZFMP_IN(zfindex, toIndexOrIndexMax)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, set
        , ZFMP_IN(zfindex, index)
        , ZFMP_IN(ZFObject *, e)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, zfauto, get
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfauto, getFirst)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfauto, getLast)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfindex, count)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(v_ZFCoreArray, zfbool, isEmpty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(v_ZFCoreArray, zfindex, isContain
        , ZFMP_IN(ZFObject *, e)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, sort
        , ZFMP_IN_OPT(zfindex, start, 0)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(v_ZFCoreArray, void, sortReversely
        , ZFMP_IN_OPT(zfindex, start, 0)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )

ZF_NAMESPACE_GLOBAL_END

