#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFKeyValueContainer)

ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnChange)
ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnAdd)
ZFEVENT_REGISTER(ZFKeyValueContainer, ContentOnRemove)

ZFMETHOD_DEFINE_3(ZFKeyValueContainer, void, objectInfoOfContentT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        ) {
    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiterator it = this->iter(); this->iterValid(it) && count < maxCount; ++count, this->iterNext(it)) {
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
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        ) {
    zfstring ret;
    this->objectInfoOfContentT(ret, maxCount, token);
    return ret;
}

zfbool ZFKeyValueContainer::serializableOnCheck(void) {
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}

    for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
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
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->removeAll();

    zfauto key;
    zfauto value;
    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        zfstring category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfstringIsEqual(category, ZFSerializableKeyword_ZFKeyValueContainer_key)) {
            if(key != zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "missing value for key %s (%s)",
                    key->objectInfoOfInstance(),
                    key);
                return zffalse;
            }
            if(!ZFObjectFromDataT(key, categoryData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(key == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "null key");
                return zffalse;
            }
        }
        else if(zfstringIsEqual(category, ZFSerializableKeyword_ZFKeyValueContainer_value)) {
            if(key == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "missing key");
                return zffalse;
            }
            if(!ZFObjectFromDataT(value, categoryData, outErrorHint, outErrorPos)) {
                return zffalse;
            }
            if(value == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData, "null value");
                return zffalse;
            }
            this->iterAdd(key, value);
            key = zfnull;
            value = zfnull;
        }
    }
    if(key != zfnull) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "missing value for key %s (%s)",
            key->objectInfoOfInstance(),
            key);
        return zffalse;
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    if(ref == zfnull) {
        for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
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

            keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
            serializableData.childAdd(keyData);
            valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
            serializableData.childAdd(valueData);
        }
    }
    else {
        return this->serializableOnSerializeToDataWithRef(serializableData, ref, outErrorHint);
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToDataWithRef(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);
    if(ref == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "%s not type of %s",
            referencedOwnerOrNull->toObject()->objectInfoOfInstance(), ZFKeyValueContainer::ClassData()->classNameFull());
        return zffalse;
    }

    if(ref->count() == 0) {
        for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
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

            keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
            serializableData.childAdd(keyData);
            valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
            serializableData.childAdd(valueData);
        }
        return zftrue;
    }

    ZFKeyValueContainer *tmp = this->classData()->newInstance();
    zfblockedRelease(tmp);
    tmp->addFrom(ref);
    for(zfiterator it = this->iter(); this->iterValid(it); this->iterNext(it)) {
        ZFObject *key = this->iterKey(it);
        ZFObject *value = this->iterValue(it);
        zfiterator itTmp = tmp->iterFind(key);
        if(ref->iterValid(itTmp)) {
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

        keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
        serializableData.childAdd(keyData);
        valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
        serializableData.childAdd(valueData);
    }

    if(tmp->count() > 0) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "missing elements from referenced container: %s", tmp->objectInfoOfContent());
        return zffalse;
    }

    return zftrue;
}

void ZFKeyValueContainer::copyableOnCopyFrom(ZF_IN ZFObject *anotherObj) {
    zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
    zfself *another = zfcast(zfself *, anotherObj);
    if(another != zfnull && this != another) {
        this->removeAll();
        this->addFrom(another);
    }
}

void ZFKeyValueContainer::objectOnDeallocPrepare(void) {
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFKeyValueContainer::objectHash(void) {
    zfiterator it = this->iter();
    if(this->iterValid(it)) {
        return (zfidentity)(~((zft_zfidentity)this->count()
            | ((ZFObjectHash(this->iterKey(it)) << 16) & 0x00FF0000)
            | ((ZFObjectHash(this->iterValue(it)) << 24) & 0xFF000000)));
    }
    else {
        return 0;
    }
}
ZFCompareResult ZFKeyValueContainer::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
            || this->objectHash() != another->objectHash()
            ) {
        return ZFCompareUncomparable;
    }

    for(zfiterator it0 = this->iter(); this->iterValid(it0); this->iterNext(it0)) {
        ZFObject *key0 = this->iterKey(it0);
        zfiterator it1 = another->iterFind(key0);
        if(!another->iterValid(it1)) {
            return ZFCompareUncomparable;
        }
        if(ZFObjectCompare(this->iterValue(it0), another->iterValue(it1)) != ZFCompareEqual) {
            return ZFCompareUncomparable;
        }
        another->iterNext(it1);
    }
    return ZFCompareEqual;
}

ZF_NAMESPACE_GLOBAL_END

