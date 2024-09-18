#include "ZFSerializable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFPropertyUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cache in class's meta data for performance
zfclassNotPOD _ZFP_ZFSerializable_PropertyTypeData {
public:
    const ZFProperty *property;
    ZFSerializablePropertyType propertyType;
};
zfclass _ZFP_I_ZFSerializablePropertyTypeHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFSerializablePropertyTypeHolder, ZFObject)
public:
    ZFCoreArray<_ZFP_ZFSerializable_PropertyTypeData *> serializableProperty;
    ZFCoreMap serializablePropertyMap; // _ZFP_ZFSerializable_PropertyTypeData *

public:
    void addData(
            ZF_IN const ZFProperty *property
            , ZF_IN ZFSerializablePropertyType propertyType
            ) {
        _ZFP_ZFSerializable_PropertyTypeData *typeData = zfnew(_ZFP_ZFSerializable_PropertyTypeData);
        typeData->property = property;
        typeData->propertyType = propertyType;

        this->serializableProperty.add(typeData);
        this->serializablePropertyMap.set(property->propertyName(),
            ZFCorePointerForObject<_ZFP_ZFSerializable_PropertyTypeData *>(typeData));
    }
};

// ============================================================
// ZFSerializable
zfbool ZFSerializable::serializable(void) {
    return this->serializableOnCheck();
}
zfbool ZFSerializable::serializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
            , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
            ) {
    // style logic
    ZFStyleable *styleable = zfcast(ZFStyleable *, this);
    {
        zfstring styleKey = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleKey);
        if(styleKey != zfnull) {
            if(styleable == zfnull) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "styleKey exists but the object is not ZFStyleable: %s",
                    this->toObject()->objectInfoOfInstance());
                return zffalse;
            }
            styleable->styleKey(styleKey);
        }
    }

    // property with wrappedValuePreferStringConverter
    {
        for(zfiter it = serializableData.attrIter(); it; ++it) {
            zfstring name = serializableData.attrIterKey(it);
            if(serializableData.attrIterResolved(it)) {
                continue;
            }
            const ZFProperty *property = this->classData()->propertyForName(name);
            if(property == zfnull) {
                continue;
            }
            const ZFTypeInfo *typeId = ZFTypeInfoForName(property->propertyTypeId());
            if(typeId == zfnull) {
                continue;
            }
            const ZFClass *propertyClass = typeId->typeIdClass();
            if(propertyClass == zfnull || propertyClass->classIsAbstract()) {
                continue;
            }
            zfauto propertyValue = propertyClass->newInstance();
            if(!zfcast(ZFTypeIdWrapper *, propertyValue)->wrappedValueFromString(serializableData.attrIterValue(it))) {
                continue;
            }
            property->setterMethod()->methodInvoke(this->toObject(), propertyValue);
            serializableData.attrIterResolveMark(it);
        }
    }

    // property
    {
        const ZFCoreMap &propertyMap = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializablePropertyMap;
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            const ZFSerializableData &element = serializableData.childAt(i);
            if(element.resolved() || element.category() != zfnull) {
                continue;
            }
            zfstring propertyName = ZFSerializableUtil::checkPropertyName(element);
            if(propertyName == zfnull) {
                continue;
            }
            _ZFP_ZFSerializable_PropertyTypeData *data = propertyMap.get<_ZFP_ZFSerializable_PropertyTypeData *>(propertyName);
            if(data == zfnull) {
                continue;
            }

            // serialize the property
            switch(data->propertyType) {
                case ZFSerializablePropertyTypeSerializable: {
                        zfstring styleKey = ZFSerializableUtil::checkAttribute(element, ZFSerializableKeyword_styleKey);
                        if(styleKey != zfnull) {
                            if(styleable == zfnull) {
                                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                                    "styleKey exists in property %s but the object is not ZFStyleable: %s",
                                    data->property,
                                    this->toObject()->objectInfoOfInstance());
                                return zffalse;
                            }
                            styleable->styleKeyForProperty(data->property, styleKey);
                            break;
                        }
                    }
                    if(!this->serializableOnSerializePropertyFromData(
                                element,
                                data->property,
                                outErrorHint,
                                outErrorPos)
                                ) {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbeded:
                    if(!this->serializableOnSerializeEmbededPropertyFromData(
                                element,
                                data->property,
                                outErrorHint,
                                outErrorPos)
                                ) {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                case ZFSerializablePropertyTypeUnspecified:
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {
        return zffalse;
    }

    // mark self as resolved
    serializableData.resolveMark();

    // dynamic
    ZFCoreArray<const ZFMethod *> dynamicMethod = this->classData()->methodForNameGetAll("serializableOnSerializeFromData");
    for(zfindex i = 0; i < dynamicMethod.count(); ++i) {
        if(!dynamicMethod[i]->execute<zfbool, const ZFSerializableData &, zfstring *, ZFSerializableData *>(
                    this->toObject(), serializableData, outErrorHint, outErrorPos)
                    ) {
            return zffalse;
        }
    }

    // check whether all resoved
    if(ZFSerializableDataResolveCheckEnable) {
        zfstring tmp;
        if(ZFSerializableUtil::printResolveStatus(serializableData, ZFOutputForString(tmp))) {
            #if ZF_ENV_DEBUG
                ZFCoreCriticalMessageTrim(tmp);
            #endif
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializeToData(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */
        ) {
    zfauto referencedObjectHolder;
    ZFSerializable *referencedObject = zfnull;
    ZFStyleable *styleable = zfcast(ZFStyleable *, this);
    if(styleable != zfnull && styleable->styleKey() != zfnull) {
        referencedObjectHolder = ZFStyleGet(styleable->styleKey());
        referencedObject = referencedObjectHolder;
        serializableData.attr(ZFSerializableKeyword_styleKey, styleable->styleKey());
    }
    if(referencedObject == zfnull) {
        referencedObject = referencedOwnerOrNull;
    }

    if(referencedObject != zfnull
            && !referencedObject->classData()->classIsTypeOf(this->classData())
            && !this->classData()->classIsTypeOf(referencedObject->classData())
            ) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "serialize with a reference style object whose type mismatch, self: %s, style: %s",
            this->classData()->classNameFull(), referencedObject->classData()->classNameFull());
        return zffalse;
    }

    // property
    {
        const ZFCoreArray<_ZFP_ZFSerializable_PropertyTypeData *> &allProperty = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
        for(zfindex i = 0; i < allProperty.count(); ++i) {
            _ZFP_ZFSerializable_PropertyTypeData *data = allProperty[i];
            switch(data->propertyType) {
                case ZFSerializablePropertyTypeSerializable:
                    if(styleable != zfnull) {
                        const zfstring &styleKey = styleable->styleKeyForProperty(data->property);
                        if(styleKey != zfnull) {
                            ZFSerializableData propertyData;
                            propertyData.itemClass(data->property->propertyTypeId());
                            propertyData.attr(ZFSerializableKeyword_styleKey, styleKey);
                            propertyData.propertyName(data->property->propertyName());
                            serializableData.childAdd(propertyData);
                            break;
                        }
                    }
                    if(!this->serializableOnSerializePropertyToData(
                                serializableData
                                , data->property
                                , referencedObject
                                , outErrorHint
                                )) {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbeded:
                    if(!this->serializableOnSerializeEmbededPropertyToData(
                                serializableData
                                , data->property
                                , referencedObject
                                , outErrorHint
                                )) {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                    break;
                case ZFSerializablePropertyTypeUnspecified:
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeToData(serializableData, referencedObject, outErrorHint)) {
        return zffalse;
    }

    serializableData.itemClass(this->classData()->classNameFull());

    // dynamic
    ZFCoreArray<const ZFMethod *> dynamicMethod = this->classData()->methodForNameGetAll("serializableOnSerializeToData");
    for(zfindex i = 0; i < dynamicMethod.count(); ++i) {
        if(!dynamicMethod[i]->execute<zfbool, ZFSerializableData &, ZFSerializable *, zfstring *>(
                    this->toObject(), serializableData, referencedObject, outErrorHint)
                    ) {
            return zffalse;
        }
    }

    return zftrue;
}

zfbool ZFSerializable::serializeFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeFromString");
    if(dynamicMethod != zfnull
            && !dynamicMethod->execute<zfbool, const zfchar *>(this->toObject(), src)
            ) {
        return zffalse;
    }
    return this->serializableOnSerializeFromString(src, srcLen, errorHint);
}
zfbool ZFSerializable::serializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeToString");
    if(dynamicMethod != zfnull
            && !dynamicMethod->execute<zfbool, zfstring &>(this->toObject(), ret)
            ) {
        return zffalse;
    }
    return this->serializableOnSerializeToString(ret, errorHint);
}

_ZFP_I_ZFSerializablePropertyTypeHolder *ZFSerializable::_ZFP_ZFSerializable_getPropertyTypeHolder(void) {
    ZFCoreMutexLocker();
    _ZFP_I_ZFSerializablePropertyTypeHolder *holder = this->classData()->classTag(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->classNameFull());
    if(holder == zfnull) {
        zfobj<_ZFP_I_ZFSerializablePropertyTypeHolder> holderTmp;
        ZFCoreArray<const ZFProperty *> allProperty;
        {
            // the order affects the serialization step's order,
            // sort parent's property first for human-readable
            ZFCoreArray<const ZFClass *> allClass;
            const ZFClass *tmpCls = this->classData();
            allClass.add(tmpCls);
            do {
                for(zfindex i = tmpCls->dynamicInterfaceCount() - 1; i != zfindexMax(); --i) {
                    allClass.add(tmpCls->dynamicInterfaceAt(i));
                }
                for(zfindex i = tmpCls->implementedInterfaceCount() - 1; i != zfindexMax(); --i) {
                    allClass.add(tmpCls->implementedInterfaceAt(i));
                }
                if(tmpCls->classParent() != zfnull) {
                    allClass.add(tmpCls->classParent());
                }
                tmpCls = tmpCls->classParent();
            } while(tmpCls != zfnull);
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i) {
                tmpCls = allClass[i];
                for(zfindex iProperty = 0; iProperty < tmpCls->propertyCount(); ++iProperty) {
                    allProperty.add(tmpCls->propertyAt(iProperty));
                }
            }
        }

        for(zfindex i = 0; i < allProperty.count(); ++i) {
            const ZFProperty *property = allProperty[i];

            ZFSerializablePropertyType propertyType = ZFSerializablePropertyTypeUnspecified;
            ZFCoreArray<const ZFMethod *> dynamicMethod = this->classData()->methodForNameGetAll("serializableOnCheckPropertyType");
            for(zfindex i = 0; i < dynamicMethod.count(); ++i) {
                ZFSerializablePropertyType propertyTypeTmp = dynamicMethod[i]->execute<ZFSerializablePropertyType, const ZFProperty *>(this->toObject(), property);
                if(propertyTypeTmp == ZFSerializablePropertyTypeNotSerializable) {
                    propertyType = propertyTypeTmp;
                    break;
                }
                else if(propertyTypeTmp != ZFSerializablePropertyTypeUnspecified) {
                    propertyType = propertyTypeTmp;
                }
            }
            if(propertyType == ZFSerializablePropertyTypeUnspecified) {
                propertyType = this->serializableOnCheckPropertyType(property);
            }

            switch(propertyType) {
                case ZFSerializablePropertyTypeUnspecified:
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                    break;
                case ZFSerializablePropertyTypeSerializable:
                    holderTmp->addData(property, ZFSerializablePropertyTypeSerializable);
                    break;
                case ZFSerializablePropertyTypeEmbeded:
                    holderTmp->addData(property, ZFSerializablePropertyTypeEmbeded);
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }

        holder = holderTmp;
        this->classData()->classTag(
            _ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->classNameFull(),
            holderTmp);
    }
    return holder;
}
void ZFSerializable::serializableGetAllSerializablePropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) {
    const ZFCoreArray<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i) {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeSerializable) {
            ret.add(tmp[i]->property);
        }
    }
}
void ZFSerializable::serializableGetAllSerializableEmbededPropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) {
    const ZFCoreArray<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i) {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeEmbeded) {
            ret.add(tmp[i]->property);
        }
    }
}

ZFSerializablePropertyType ZFSerializable::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property) {
    if(property->propertyIsRetainProperty()) {
        if(property->setterMethod()->methodIsPrivate()) {
            if(property->getterMethod()->methodIsPrivate()
                    || !property->propertyClassOfRetainProperty()->classIsTypeOf(ZFSerializable::ClassData())
                    ) {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else {
                return ZFSerializablePropertyTypeEmbeded;
            }
        }
        else {
            if(property->getterMethod()->methodIsPrivate()) {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else {
                return ZFSerializablePropertyTypeSerializable;
            }
        }
    }
    else {
        if(!property->propertySerializable()
                || property->getterMethod()->methodIsPrivate()
                ) {
            return ZFSerializablePropertyTypeNotSerializable;
        }
        else {
            return ZFSerializablePropertyTypeSerializable;
        }
    }
}

zfbool ZFSerializable::serializableOnSerializePropertyFromData(
        ZF_IN const ZFSerializableData &propertyData
        , ZF_IN const ZFProperty *property
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(property->propertyIsRetainProperty()) {
        zfauto obj;
        if(!ZFObjectFromDataT(obj, propertyData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(obj != zfnull && !obj->classData()->classIsTypeOf(property->propertyClassOfRetainProperty())) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, propertyData,
                "object %s not type of %s",
                obj->objectInfoOfInstance(), property->propertyClassOfRetainProperty()->classNameFull());
            return zffalse;
        }
        property->setterMethod()->methodInvoke(this->toObject(), obj);
        return zftrue;
    }
    else {
        const ZFTypeInfo *typeId = ZFTypeInfoForName(property->propertyTypeId());
        if(typeId == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, propertyData,
                "property %s is not serializable",
                property);
            return zffalse;
        }

        const ZFClass *propertyClass = typeId->typeIdClass();
        if(propertyClass == zfnull || propertyClass->classIsAbstract()) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, propertyData,
                "property %s is not serializable",
                property->propertyName());
            return zffalse;
        }
        zfauto propertyValue = propertyClass->newInstance();
        if(!zfcast(ZFTypeIdWrapper *, propertyValue)->wrappedValueFromData(propertyData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        property->setterMethod()->methodInvoke(this->toObject(), propertyValue);
        return zftrue;
    }
}
zfbool ZFSerializable::serializableOnSerializePropertyToData(
        ZF_OUT ZFSerializableData &ownerData
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(referencedOwnerOrNull != zfnull
            && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareEqual
            ) {
        return zftrue;
    }
    if(property->callbackIsInitValue(property, this->toObject(), zfnull)) {
        return zftrue;
    }

    zfauto propertyValue = property->getterMethod()->methodInvoke(this->toObject());
    ZFTypeIdWrapper *wrapper = propertyValue;
    if(wrapper != zfnull && wrapper->wrappedValuePreferStringConverter()) {
        zfstring value;
        if(wrapper->wrappedValueToString(value)) {
            ownerData.attr(property->propertyName(), value);
            return zftrue;
        }
    }

    if(property->propertyIsRetainProperty()) {
        ZFSerializableData propertyData;
        if(!ZFObjectToDataT(propertyData, propertyValue, outErrorHint)) {
            return zffalse;
        }
        propertyData.propertyName(property->propertyName());
        ownerData.childAdd(propertyData);
        return zftrue;
    }
    else {
        if(propertyValue == zfnull) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "property %s is not serializable",
                property->propertyName());
            return zffalse;
        }
        ZFSerializableData propertyData;
        if(!zfcast(ZFTypeIdWrapper *, propertyValue)->wrappedValueToData(propertyData, outErrorHint)) {
            return zffalse;
        }
        propertyData.propertyName(property->propertyName());
        ownerData.childAdd(propertyData);
        return zftrue;
    }
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyFromData(
        ZF_IN const ZFSerializableData &propertyData
        , ZF_IN const ZFProperty *property
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    zfauto obj = property->getterMethod()->methodInvoke(this->toObject());
    if(obj == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "embeded property %s is null while serializing \"%s\"",
            property->propertyName(),
            this->classData()->classNameFull());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj)) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "not serializable object %s while serializing \"%s\"",
            obj->objectInfoOfInstance(),
            this->classData()->classNameFull());
        return zffalse;
    }
    else {
        const ZFClass *cls = ZFClass::classForName(propertyData.itemClass());
        if(cls == zfnull) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "no class named %s while serializing \"%s\"'s property %s",
                propertyData.itemClass(),
                this->classData()->classNameFull(),
                property->propertyName());
            return zffalse;
        }
        else if(!cls->classIsTypeOf(property->propertyClassOfRetainProperty())) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "node %s is not type of %s while serializing \"%s\"'s property %s",
                propertyData.itemClass(),
                property->propertyClassOfRetainProperty()->classNameFull(),
                this->classData()->classNameFull(),
                property->propertyName());
            return zffalse;
        }
    }
    return zfcast(zfself *, obj)->serializeFromData(propertyData, outErrorHint, outErrorPos);
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyToData(
        ZF_OUT ZFSerializableData &ownerData
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(referencedOwnerOrNull != zfnull
            && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareEqual
            ) {
        return zftrue;
    }
    zfauto initValue;
    if(property->callbackIsInitValue(property, this->toObject(), &initValue)) {
        return zftrue;
    }

    zfauto objHolder = property->getterMethod()->methodInvoke(this->toObject());
    ZFSerializable *obj = objHolder;
    if(obj == zfnull || !ZFObjectIsSerializable(obj->toObject())) {
        return zftrue;
    }

    ZFSerializable *propertyRef = zfnull;
    if(referencedOwnerOrNull != zfnull) {
        zfauto t = property->getterMethod()->methodInvoke(referencedOwnerOrNull->toObject());
        if(t != zfnull) {
            propertyRef = t;
        }
    }
    if(propertyRef == zfnull) {
        propertyRef = initValue;
    }

    ZFSerializableData propertyData;
    if(!obj->serializeToData(propertyData, outErrorHint, propertyRef)) {
        return zffalse;
    }

    if(propertyData.childCount() == 0
            && propertyData.attrCount() == 0
            ) {
        propertyData.itemClass(zfnull);
    }
    propertyData.propertyName(property->propertyName());
    ownerData.childAdd(propertyData);
    return zftrue;
}

void ZFSerializable::serializableInfoT(ZF_IN_OUT zfstring &ret) {
    ZFSerializableData serializableData;
    if(!this->serializable()
            || !this->serializeToData(serializableData)
            ) {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->toObject()->objectInfoOfInstanceT(ret);
        ret += ": not serializable";
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    else {
        serializableData.objectInfoT(ret);
    }
}

// ============================================================
zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj) {
    if(obj == zfnull) {
        return zftrue;
    }
    else {
        ZFSerializable *tmp = zfcast(ZFSerializable *, obj);
        return (tmp != zfnull && tmp->serializable());
    }
}

// ============================================================
zfbool ZFObjectFromDataT(
        ZF_OUT zfauto &result
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    result = zfnull;

    zfstring serializableClass = ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_none(), outErrorHint, outErrorPos);
    if(serializableClass == zfnull) {
        return zffalse;
    }
    if(zfstringIsEqual(serializableClass, ZFSerializableKeyword_null)) {
        serializableData.resolveMark();
        return zftrue;
    }

    const ZFClass *cls = ZFClass::classForName(serializableClass);
    if(cls == zfnull) {
        if(cls == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "no class named \"%s\"",
                serializableData.itemClass());
            return zffalse;
        }
    }
    zfauto obj;
    {
        const ZFMethod *overridedCreateMethod = cls->methodForNameIgnoreParent(ZFSerializableKeyword_serializableNewInstance);
        if(overridedCreateMethod != zfnull) {
            obj = overridedCreateMethod->execute<zfauto>(zfnull);
        }
        else {
            obj = cls->newInstance();
        }
    }
    ZFSerializable *tmp = obj;
    if(tmp == zfnull || !tmp->serializable()) {
        ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
            "object %s not serializable",
            ZFObjectInfoOfInstance(obj));
        return zffalse;
    }
    if(!tmp->serializeFromData(serializableData, outErrorHint, outErrorPos)) {
        return zffalse;
    }
    result = obj;
    return zftrue;
}
zfauto ZFObjectFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    zfauto result;
    ZFObjectFromDataT(result, serializableData, outErrorHint, outErrorPos);
    return result;
}
zfbool ZFObjectToDataT(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */
        ) {
    if(obj == zfnull) {
        serializableData.itemClass(ZFSerializableKeyword_null);
        return zftrue;
    }
    ZFSerializable *tmp = zfcast(ZFSerializable *, obj);
    if(tmp == zfnull || !tmp->serializable()) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "object %s not serializable",
            obj->objectInfoOfInstance());
        return zffalse;
    }
    return tmp->serializeToData(serializableData, outErrorHint, referencedOwnerOrNull);
}
ZFSerializableData ZFObjectToData(
        ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfbool *outSuccess /* = zfnull */
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */
        ) {
    ZFSerializableData serializableData;
    zfbool success = ZFObjectToDataT(serializableData, obj, outErrorHint, referencedOwnerOrNull);
    if(outSuccess != zfnull) {
        *outSuccess = success;
    }
    if(success) {
        return serializableData;
    }
    return ZFSerializableData();
}

zfbool ZFSerializeFromString(
        ZF_OUT zfauto &result
        , ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(cls == zfnull || !cls->classIsTypeOf(ZFSerializable::ClassData())) {
        return zffalse;
    }
    const ZFMethod *createMethod = cls->methodForName(ZFSerializableKeyword_serializableNewInstance);
    if(createMethod == zfnull) {
        result = cls->newInstance();
    }
    else {
        result = createMethod->execute<zfauto>(zfnull);
    }
    ZFSerializable *serializable = result;
    if(serializable == zfnull) {
        result = zfnull;
        return zffalse;
    }
    if(!serializable->serializeFromString(src, srcLen, errorHint)) {
        result = zfnull;
        return zffalse;
    }
    else {
        return zftrue;
    }
}
zfauto ZFSerializeFromString(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    zfauto ret;
    ZFSerializeFromString(ret, cls, src, srcLen, errorHint);
    return ret;
}
zfbool ZFSerializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(obj == zfnull) {
        return zftrue;
    }
    ZFSerializable *t = zfcast(ZFSerializable *, obj);
    if(t == zfnull) {
        return zffalse;
    }
    return t->serializeToString(ret, errorHint);
}
zfstring ZFSerializeToString(
        ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    zfstring ret;
    ZFSerializeToString(ret, obj, errorHint);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfbool, serializable)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeFromData
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeToData
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFSerializable, zfbool, serializeToString
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializablePropertyT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArray<const ZFProperty *>, serializableGetAllSerializableProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializableEmbededPropertyT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArray<const ZFProperty *>, serializableGetAllSerializableEmbededProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfstring, serializableInfo)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfbool, ZFObjectIsSerializable
        , ZFMP_IN(ZFObject *, obj)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromDataT
        , ZFMP_OUT(zfauto &, result)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfauto, ZFObjectFromData
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectToDataT
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFSerializableData, ZFObjectToData
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfbool *, outSuccess, zfnull)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(zfbool, ZFSerializeFromString
        , ZFMP_OUT(zfauto &, result)
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfauto, ZFSerializeFromString
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializeToString
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFSerializeToString
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

