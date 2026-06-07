#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFContainer)

ZFEVENT_REGISTER(ZFContainer, ContentOnUpdate)
ZFEVENT_REGISTER(ZFContainer, ContentOnAdd)
ZFEVENT_REGISTER(ZFContainer, ContentOnRemove)

ZFMETHOD_DEFINE_3(ZFContainer, void, objectInfoOfContentT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        ) {
    zfindex count = 0;
    ret += token.tokenLeft;
    zfiter it = this->iter();
    for(; it && count < maxCount; ++count, ++it) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenValueLeft;
        ZFObjectInfoT(ret, this->iterValue(it));
        ret += token.tokenValueRight;
    }
    if(count < this->count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}
ZFMETHOD_DEFINE_2(ZFContainer, zfstring, objectInfoOfContent
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
        ) {
    zfstring ret;
    this->objectInfoOfContentT(ret, maxCount, token);
    return ret;
}

zfbool ZFContainer::serializableOnCheck(void) {
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}
    for(zfiter it = this->iter(); it; ++it) {
        if(!ZFObjectIsSerializable(this->iterValue(it))) {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->removeAll();

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &elementData = serializableData.childAt(i);
        if(elementData.resolved()
                || elementData.category()
                ) {
            continue;
        }

        zfauto element;
        if(!ZFObjectFromDataT(element, elementData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(element == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, elementData,
                "null element");
            return zffalse;
        }
        this->iterAdd(element);

        elementData.resolveMark();
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    if(ref == zfnull) {
        for(zfiter it = this->iter(); it; ++it) {
            ZFSerializableData elementData;
            if(!ZFObjectToDataT(elementData, this->iterValue(it), outErrorHint)) {
                return zffalse;
            }
            serializableData.child(elementData);
        }
    }
    else {
        return this->serializableOnSerializeToDataWithRef(serializableData, outErrorHint, ref);
    }

    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeToDataWithRef(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    zfself *ref = zfcast(zfself *, refOwner);
    if(ref == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "%s not type of %s",
            refOwner->toObject()->objectInfoOfInstance(), ZFContainer::ClassData()->classNameFull());
        return zffalse;
    }

    if(ref->count() == 0) {
        for(zfiter it = this->iter(); it; ++it) {
            ZFObject *element = this->iterValue(it);
            ZFSerializableData elementData;
            if(!ZFObjectToDataT(elementData, element, outErrorHint)) {
                return zffalse;
            }
            serializableData.child(elementData);
        }
        return zftrue;
    }

    zfautoT<ZFContainer> tmp = this->classData()->newInstance();
    tmp->addFrom(ref);
    for(zfiter it = this->iter(); it; ++it) {
        ZFObject *element = this->iterValue(it);
        zfiter itTmp = tmp->iterFind(element);
        if(itTmp) {
            tmp->iterRemove(itTmp);
            continue;
        }

        ZFSerializableData elementData;
        if(!ZFObjectToDataT(elementData, element, outErrorHint)) {
            return zffalse;
        }
        serializableData.child(elementData);
    }

    if(tmp->count() > 0) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "missing elements from referenced container: %s", tmp->objectInfoOfContent());
        return zffalse;
    }

    return zftrue;
}

zfbool ZFContainer::serializableOnSerializeFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!this->classData()->classContainDynamicRegister()) {
        return zffalse;
    }
    const ZFMethod *m = this->classData()->methodForName("elementClass");
    if(m == zfnull) {
        return zffalse;
    }
    zfauto clsHolder = m->methodInvoke(this);
    if(zfcast(v_ZFClass *, clsHolder) == zfnull) {
        return zffalse;
    }
    const ZFClass *cls = clsHolder.to<v_ZFClass *>()->zfv;
    ZFCoreArray<ZFIndexRange> pos;
    if(!ZFCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, ",", "[", "]", zftrue)) {
        return zffalse;
    }
    this->removeAll();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfauto element;
        zfstring tmp;
        ZFCoreDataDecode(tmp, src + pos[i].start, pos[i].count);
        if(!ZFObjectFromStringOrDataT(element, cls, tmp, tmp.length(), errorHint)) {
            return zffalse;
        }
        this->iterAdd(element);
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!this->classData()->classContainDynamicRegister()) {
        return zffalse;
    }
    const ZFMethod *m = this->classData()->methodForName("elementClass");
    if(m == zfnull) {
        return zffalse;
    }
    zfauto clsHolder = m->methodInvoke(this);
    if(zfcast(v_ZFClass *, clsHolder) == zfnull) {
        return zffalse;
    }
    const ZFClass *cls = clsHolder.to<v_ZFClass *>()->zfv;
    zfstring charMap = ZFCoreDataEncodeCharMapCreate(ZFCoreDataEncodeCharMapAllPrintable()
            , -'%'
            , -'['
            , -']'
            , -','
            );

    ret += "[";
    zfbool first = zftrue;
    for(zfiter it = this->iter(); it; ++it) {
        if(first) {
            first = zffalse;
        }
        else {
            ret += ",";
        }
        zfstring tmp;
        if(!ZFObjectToStringOrDataT(tmp, this->iterValue(it), errorHint)) {
            return zffalse;
        }
        ZFCoreDataEncode(ret, tmp, tmp.length(), charMap);
    }
    ret += "]";
    return zftrue;
}

void ZFContainer::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
    zfself *another = zfcast(zfself *, anotherStyleable);
    if(another != zfnull && this != another) {
        this->removeAll();
        this->addFrom(another);
    }
}

void ZFContainer::objectOnDeallocPrepare(void) {
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

ZFCompareResult ZFContainer::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
            || this->objectHash() != another->objectHash()
            ) {
        return ZFCompareUncomparable;
    }
    for(zfiter it = this->iter(), itRef = another->iter(); it; ++it, ++itRef) {
        if(ZFObjectCompare(this->iterValue(it), another->iterValue(itRef)) != ZFCompareEqual) {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareEqual;
}

ZF_NAMESPACE_GLOBAL_END

