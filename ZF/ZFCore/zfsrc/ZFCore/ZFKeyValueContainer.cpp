#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFKeyValueContainer)

ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnUpdate)
ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnAdd)
ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnRemove)

void ZFKeyValueContainer::objectInfoOfContentT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForContainer &token /* = ZFTokenForContainerDefault() */
        ) {
    const ZFTokenForKeyValueContainer &tokenKV = ZFTokenForKeyValueContainerDefault();

    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiter it = this->iter(); it && count < maxCount; ++count, ++it) {
        ZFObject *key = this->iterKey(it);
        ZFObject *value = this->iterValue(it);

        if(count > 0) {
            ret += token.tokenSeparator;
        }

        ret += tokenKV.tokenPairLeft;
        {
            ret += tokenKV.tokenKeyLeft;
            key->objectInfoT(ret);
            ret += tokenKV.tokenKeyRight;
        }
        ret += tokenKV.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            value->objectInfoT(ret);
            ret += token.tokenValueRight;
        }
        ret += tokenKV.tokenPairRight;
    }
    if(count < this->count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}
zfstring ZFKeyValueContainer::objectInfoOfContent(
        ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForContainer &token /* = ZFTokenForContainerDefault() */
        ) {
    zfstring ret;
    this->objectInfoOfContentT(ret, maxCount, token);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFKeyValueContainer, void, objectInfoOfContentT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(zfindex, maxCount)
        , ZFMP_IN(const ZFTokenForKeyValueContainer &, token)
        ) {
    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiter it = this->iter(); it && count < maxCount; ++count, ++it) {
        ZFObject *key = this->iterKey(it);
        ZFObject *value = this->iterValue(it);

        if(count > 0) {
            ret += token.tokenSeparator;
        }

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            key->objectInfoT(ret);
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            value->objectInfoT(ret);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(count < this->count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}
ZFMETHOD_DEFINE_2(ZFKeyValueContainer, zfstring, objectInfoOfContent
        , ZFMP_IN(zfindex, maxCount)
        , ZFMP_IN(const ZFTokenForKeyValueContainer &, token)
        ) {
    zfstring ret;
    this->objectInfoOfContentT(ret, maxCount, token);
    return ret;
}

zfbool ZFKeyValueContainer::serializableOnCheck(void) {
    // completely override ZFContainer's version
    // if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}

    for(zfiter it = this->iter(); it; ++it) {
        if(!ZFObjectIsSerializable(this->iterKey(it))
                || !ZFObjectIsSerializable(this->iterValue(it))
                ) {
            return zffalse;
        }
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    // completely override ZFContainer's version
    // if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->removeAll();

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &nodeData = serializableData.childAt(i);
        if(nodeData.resolved()
                || nodeData.category()
                || nodeData.itemClass() != ZFSerializableKeyword_node
                || nodeData.childCount() != 2
                ) {
            continue;
        }

        zfauto key;
        if(!ZFObjectFromDataT(key, nodeData.childAt(0), outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(key == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "null key");
            return zffalse;
        }

        zfauto value;
        if(!ZFObjectFromDataT(value, nodeData.childAt(1), outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(value == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "null value");
            return zffalse;
        }
        this->iterAdd(key, value);
        nodeData.resolveMark();
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    // completely override ZFContainer's version
    // if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    if(ref == zfnull) {
        for(zfiter it = this->iter(); it; ++it) {
            ZFObject *key = this->iterKey(it);
            ZFObject *value = this->iterValue(it);

            ZFSerializableData keyData;
            if(!ZFObjectToDataT(keyData, key, outErrorHint)) {
                return zffalse;
            }

            ZFSerializableData valueData;
            if(!ZFObjectToDataT(valueData, value, outErrorHint)) {
                return zffalse;
            }

            ZFSerializableData nodeData;
            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.child(keyData);
            nodeData.child(valueData);
            serializableData.child(nodeData);
        }
    }
    else {
        return this->serializableOnSerializeToDataWithRef(serializableData, outErrorHint, ref);
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToDataWithRef(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    zfself *ref = zfcast(zfself *, refOwner);
    if(ref == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "%s not type of %s",
            refOwner->toObject()->objectInfoOfInstance(), ZFKeyValueContainer::ClassData()->classNameFull());
        return zffalse;
    }

    if(ref->count() == 0) {
        for(zfiter it = this->iter(); it; ++it) {
            ZFObject *key = this->iterKey(it);
            ZFObject *value = this->iterValue(it);
            ZFSerializableData keyData;
            if(!ZFObjectToDataT(keyData, key, outErrorHint)) {
                return zffalse;
            }
            ZFSerializableData valueData;
            if(!ZFObjectToDataT(valueData, value, outErrorHint)) {
                return zffalse;
            }

            ZFSerializableData nodeData;
            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.child(keyData);
            nodeData.child(valueData);
            serializableData.child(nodeData);
        }
        return zftrue;
    }

    zfautoT<ZFKeyValueContainer> tmp = this->classData()->newInstance();
    tmp->addFrom(ref);
    for(zfiter it = this->iter(); it; ++it) {
        ZFObject *key = this->iterKey(it);
        ZFObject *value = this->iterValue(it);
        zfiter itTmp = tmp->iterFind(key);
        if(itTmp) {
            if(ZFObjectCompare(value, tmp->iterValue(itTmp)) == ZFCompareEqual) {
                tmp->iterRemove(itTmp);
                continue;
            }
            else {
                tmp->iterRemove(itTmp);
            }
        }

        ZFSerializableData keyData;
        if(!ZFObjectToDataT(keyData, key, outErrorHint)) {
            return zffalse;
        }
        ZFSerializableData valueData;
        if(!ZFObjectToDataT(valueData, value, outErrorHint)) {
            return zffalse;
        }

        ZFSerializableData nodeData;
        nodeData.itemClass(ZFSerializableKeyword_node);
        nodeData.child(keyData);
        nodeData.child(valueData);
        serializableData.child(nodeData);
    }

    if(tmp->count() > 0) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "missing elements from referenced container: %s", tmp->objectInfoOfContent());
        return zffalse;
    }

    return zftrue;
}

zfbool ZFKeyValueContainer::serializableOnSerializeFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!this->classData()->classContainDynamicRegister()) {
        return zffalse;
    }
    const ZFMethod *keyM = this->classData()->methodForName("keyClass");
    if(keyM == zfnull) {
        return zffalse;
    }
    const ZFMethod *valueM = this->classData()->methodForName("valueClass");
    if(valueM == zfnull) {
        return zffalse;
    }
    zfauto keyClsHolder = keyM->methodInvoke(this);
    if(zfcast(v_ZFClass *, keyClsHolder) == zfnull) {
        return zffalse;
    }
    zfauto valueClsHolder = valueM->methodInvoke(this);
    if(zfcast(v_ZFClass *, valueClsHolder) == zfnull) {
        return zffalse;
    }
    const ZFClass *keyCls = keyClsHolder.to<v_ZFClass *>()->zfv;
    const ZFClass *valueCls = valueClsHolder.to<v_ZFClass *>()->zfv;

    ZFCoreArray<ZFIndexRange> pos;
    if(!ZFCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, ",", "{", "}")) {
        return zffalse;
    }
    this->removeAll();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfindex tokenPos = zfstringFind(src + pos[i].start, pos[i].count, "=");
        if(tokenPos == zfindexMax()) {
            return zffalse;
        }

        zfstring tmp;
        ZFCoreDataDecode(tmp, src + pos[i].start, tokenPos);
        zfauto key;
        if(!ZFObjectFromStringOrDataT(key, keyCls, tmp, tmp.length(), errorHint)) {
            return zffalse;
        }

        tmp.removeAll();
        ZFCoreDataDecode(tmp, src + pos[i].start + tokenPos + 1, pos[i].count - tokenPos - 1);
        zfauto value;
        if(!ZFObjectFromStringT(value, valueCls, tmp, tmp.length(), errorHint)) {
            return zffalse;
        }

        this->iterAdd(key, value);
    }
    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(!this->classData()->classContainDynamicRegister()) {
        return zffalse;
    }
    const ZFMethod *keyM = this->classData()->methodForName("keyClass");
    if(keyM == zfnull) {
        return zffalse;
    }
    const ZFMethod *valueM = this->classData()->methodForName("valueClass");
    if(valueM == zfnull) {
        return zffalse;
    }
    zfauto keyClsHolder = keyM->methodInvoke(this);
    if(zfcast(v_ZFClass *, keyClsHolder) == zfnull) {
        return zffalse;
    }
    zfauto valueClsHolder = valueM->methodInvoke(this);
    if(zfcast(v_ZFClass *, valueClsHolder) == zfnull) {
        return zffalse;
    }
    zfstring charMap = ZFCoreDataEncodeCharMapCreate(ZFCoreDataEncodeCharMapAllPrintable()
            , -'%'
            , -'{'
            , -'}'
            , -','
            , -'='
            );

    ret += "{";
    zfbool first = zftrue;
    for(zfiter it = this->iter(); it; ++it) {
        if(first) {
            first = zffalse;
        }
        else {
            ret += ",";
        }

        zfstring tmp;
        if(!ZFObjectToStringOrDataT(tmp, this->iterKey(it), errorHint)) {
            return zffalse;
        }
        ZFCoreDataEncode(ret, tmp, tmp.length());

        ret += "=";

        tmp.removeAll();
        if(!ZFObjectToStringOrDataT(tmp, this->iterValue(it), errorHint)) {
            return zffalse;
        }
        ZFCoreDataEncode(ret, tmp, tmp.length());
    }
    ret += "}";
    return zftrue;
}

void ZFKeyValueContainer::objectOnDeallocPrepare(void) {
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

ZFCompareResult ZFKeyValueContainer::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
            || this->objectHash() != another->objectHash()
            ) {
        return ZFCompareUncomparable;
    }

    for(zfiter it0 = this->iter(); it0; ++it0) {
        ZFObject *key0 = this->iterKey(it0);
        zfiter it1 = another->iterFind(key0);
        if(!it1) {
            return ZFCompareUncomparable;
        }
        if(ZFObjectCompare(this->iterValue(it0), another->iterValue(it1)) != ZFCompareEqual) {
            return ZFCompareUncomparable;
        }
        ++it1;
    }
    return ZFCompareEqual;
}

ZF_NAMESPACE_GLOBAL_END

