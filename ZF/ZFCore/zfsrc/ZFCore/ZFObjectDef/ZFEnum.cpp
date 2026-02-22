#include "ZFEnum.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const zfstring &_ZFP_ZFEnumNameInvalid(void) {
    static zfstring s = zftext("ZFEnumInvalid");
    return s;
}

// ============================================================
// ZFEnum
ZFOBJECT_REGISTER(ZFEnum)

zfbool ZFEnum::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    zfstring valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(valueString != zfnull) {
        zfuint enumValue = ZFEnumInvalid();
        if(!zfstringIsEqual(valueString, ZFEnumNameInvalid())) {
            if(this->enumIsFlags()) {
                zfflags t = 0;
                if(zfflagsFromStringT(t, this->classData(), valueString)) {
                    enumValue = (zfuint)t;
                }
            }
            else {
                enumValue = this->enumValueForName(valueString);
            }
            if(enumValue == ZFEnumInvalid()) {
                ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                    "invalid value %s for enum %s", valueString, this->classData()->classNameFull());
                return zffalse;
            }
        }
        this->enumValue(enumValue);
    }
    else {
        this->enumValue(ZFEnumInvalid());
    }
    return zftrue;
}
zfbool ZFEnum::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    if((ref == zfnull && this->enumValue() != ZFEnumInvalid())
            || (ref != zfnull && this->enumValue() != ref->enumValue())
            ) {
        if(this->enumIsFlags()) {
            zfstring s;
            if(!zfflagsToStringT(s, this->classData(), (zfflags)this->enumValue())) {
                ZFSerializableUtilErrorOccurred(outErrorHint,
                    "unable convert enum value to string: %s",
                    this);
                return zffalse;
            }
            serializableData.propertyValue(s);
        }
        else {
            serializableData.propertyValue(this->enumName());
        }
    }
    return zftrue;
}

void ZFEnum::objectOnInit(ZF_IN zfuint value) {
    zfself::enumValue(value);
}

void ZFEnum::objectOnInit(ZF_IN ZFEnum *another) {
    if(another != zfnull && another->classData()->classIsTypeOf(this->classData())) {
        zfself::enumValue(another->enumValue());
    }
}

void ZFEnum::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    if(this->enumValue() == ZFEnumInvalid()) {
        ret += "(";
        ret += this->classData()->className();
        ret += ")";
        ret += ZFEnumNameInvalid();
    }
    else {
        if(this->enumIsFlags()) {
            zfflagsToStringT(ret, this->classData(), (zfflags)this->enumValue());
        }
        else {
            ret += this->enumName();
        }
    }
}

ZFCompareResult ZFEnum::objectCompareImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {
        return ZFCompareEqual;
    }
    if(anotherObj == zfnull) {
        return ZFCompareUncomparable;
    }
    if(!anotherObj->classData()->classIsTypeOf(this->classData())
            && !this->classData()->classIsTypeOf(anotherObj->classData())
            ) {
        return ZFCompareUncomparable;
    }

    zfself *another = zfcast(zfself *, anotherObj);
    if(this->enumValue() == ZFEnumInvalid()
            || another->enumValue() == ZFEnumInvalid()
            ) {
        return ZFCompareUncomparable;
    }
    zfint n = this->enumValue() - another->enumValue();
    if(n < 0) {
        return ZFCompareSmaller;
    }
    else if(n == 0) {
        return ZFCompareEqual;
    }
    else {
        return ZFCompareGreater;
    }
}

void *ZFEnum::wrappedValue(void) {
    return &(this->_ZFP_ZFEnum_value); /* ZFTAG_TRICKS: EnumReinterpretCast */
}
void ZFEnum::wrappedValue(ZF_IN const void *v) {
    this->_ZFP_ZFEnum_value = *(const zfuint *)v; /* ZFTAG_TRICKS: EnumReinterpretCast */
}
void ZFEnum::wrappedValueCopy(ZF_IN void *v) {
    *(zfuint *)v = this->_ZFP_ZFEnum_value; /* ZFTAG_TRICKS: EnumReinterpretCast */
}
void ZFEnum::zfvReset(void) {
    this->_ZFP_ZFEnum_value = ZFEnumInvalid();
}
zfbool ZFEnum::zfvIsInit(void) {
    return (this->_ZFP_ZFEnum_value == ZFEnumInvalid());
}
zfbool ZFEnum::zfvFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    return this->serializeFromData(serializableData, outErrorHint, outErrorPos);
}
zfbool ZFEnum::zfvToData(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    return this->serializeToData(serializableData, outErrorHint);
}
zfbool ZFEnum::zfvFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(zfstringIsEqual(ZFEnumNameInvalid(), zfindexMax(), src, srcLen)) {
        this->enumValue(ZFEnumInvalid());
        return zftrue;
    }

    if(this->enumIsFlags()) {
        zfflags t = 0;
        if(zfflagsFromStringT(t, this->classData(), src, srcLen)) {
            this->enumValue((zfuint)t);
            return zftrue;
        }
        else {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
    }
    else {
        zfuint v = this->enumValueForName((srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString());
        if(v == ZFEnumInvalid()) {
            return zffalse;
        }
        else {
            this->enumValue(v);
            return zftrue;
        }
    }
}
zfbool ZFEnum::zfvToString(
        ZF_IN_OUT zfstring &s
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(this->enumIsFlags()) {
        return zfflagsToStringT(s, this->classData(), (zfflags)this->enumValue());
    }
    else {
        s += this->enumName();
        return zftrue;
    }
}

// ============================================================
// zfflags conversion
zfclass _ZFP_I_ZFEnum_stringConverterDataHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnum_stringConverterDataHolder, ZFObject)

public:
    zfindex enumCount;
    zfflags *flagList;
    zfstring *nameListHolder;
    const zfchar **nameList;

public:
    static _ZFP_I_ZFEnum_stringConverterDataHolder *setup(ZF_IN const ZFClass *enumClass) {
        _ZFP_I_ZFEnum_stringConverterDataHolder *ret = enumClass->dataCache(_ZFP_I_ZFEnum_stringConverterDataHolder::ClassData()->classNameFull());
        if(ret == zfnull) {
            const ZFMethod *enumCountMethod = enumClass->methodForName("EnumCount");
            const ZFMethod *enumValueAtMethod = enumClass->methodForName("EnumValueAt");
            const ZFMethod *enumNameAtMethod = enumClass->methodForName("EnumNameAt");
            ZFCoreAssert(enumCountMethod != zfnull && enumValueAtMethod != zfnull && enumNameAtMethod != zfnull);

            ret = zfobjAlloc(_ZFP_I_ZFEnum_stringConverterDataHolder);
            enumClass->dataCache(_ZFP_I_ZFEnum_stringConverterDataHolder::ClassData()->classNameFull(), ret);
            zfobjRelease(ret);

            ret->enumCount = enumCountMethod->methodInvoke().to<v_zfindex *>()->zfv;
            ZFCoreAssert(ret->enumCount > 0);

            zfbyte *buf = (zfbyte *)zfmalloc((0
                        + sizeof(zfflags)
                        + sizeof(zfstring)
                        + sizeof(const zfchar *)
                        ) * ret->enumCount);

            ret->flagList = (zfflags *)buf;
            buf += sizeof(zfflags) * ret->enumCount;

            ret->nameListHolder = (zfstring *)buf;
            buf += sizeof(zfstring) * ret->enumCount;

            ret->nameList = (const zfchar **)buf;

            for(zfindex i = 0; i < ret->enumCount; ++i) {
                zfobj<v_zfindex> iHolder(i);
                ret->flagList[i] = enumValueAtMethod->methodInvoke(zfnull, iHolder).to<v_zfuint *>()->zfv;
                zfnewPlacement(ret->nameListHolder + i, zfstring, enumNameAtMethod->methodInvoke(zfnull, iHolder).to<v_zfstring *>()->zfv);
                ret->nameList[i] = ret->nameListHolder[i];
            }
        }
        return ret;
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void) {
        for(zfindex i = 0; i < this->enumCount; ++i) {
            zfdeletePlacement(this->nameListHolder + i);
        }
        zffree((zfbyte *)this->flagList);
        zfsuper::objectOnDealloc();
    }
};
zfbool zfflagsToStringT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const ZFClass *enumClass
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted /* = zftrue */
        , ZF_IN_OPT zfbool exclusiveMode /* = zffalse */
        , ZF_OUT_OPT zfflags *notConverted /* = zfnull */
        , ZF_IN_OPT zfchar separatorToken /* = '|' */
        ) {
    ZFCoreMutexLocker();
    if(enumClass != zfnull && enumClass->classIsTypeOf(ZFEnum::ClassData())) {
        _ZFP_I_ZFEnum_stringConverterDataHolder *d = _ZFP_I_ZFEnum_stringConverterDataHolder::setup(enumClass);
        return zfflagsToStringT(ret,
            d->flagList, d->nameList, d->enumCount,
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
    }
    else {
        return zfflagsToStringT(ret,
            zfnull, zfnull, 0,
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
    }
}
zfbool zfflagsFromStringT(
        ZF_OUT zfflags &ret
        , ZF_IN const ZFClass *enumClass
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_IN_OPT zfchar separatorToken /* = '|' */
        ) {
    ZFCoreMutexLocker();
    if(enumClass != zfnull && enumClass->classIsTypeOf(ZFEnum::ClassData())) {
        _ZFP_I_ZFEnum_stringConverterDataHolder *d = _ZFP_I_ZFEnum_stringConverterDataHolder::setup(enumClass);
        return zfflagsFromStringT(
                ret
                , d->flagList
                , d->nameList
                , d->enumCount
                , src
                , srcLen
                , separatorToken
                );
    }
    else {
        return zfflagsFromStringT(
                ret
                , zfnull
                , zfnull
                , 0
                , src
                , srcLen
                , separatorToken
                );
    }
}

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFEnumDataHolder) {
}
ZFCoreMap enumDataMap;
ZF_STATIC_INITIALIZER_END(ZFEnumDataHolder)
#define _ZFP_ZFEnumDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFEnumDataHolder)->enumDataMap)

// ============================================================
zfclassNotPOD _ZFP_ZFEnumDataPrivate {
public:
    typedef zfstlhashmap<zfuint, ZFCoreArray<zfstring> > ValueMapType;
    ValueMapType valueMap;
    typedef zfstlhashmap<zfstring, zfuint> NameMapType;
    NameMapType nameMap;
    ZFCoreArray<zfuint> vl; // ensured no duplicated value
    ZFCoreArray<zfstring> nl; // for duplicated value, only first stored
};
_ZFP_ZFEnumData::_ZFP_ZFEnumData(void)
: needInitFlag(zftrue)
, ownerClass(zfnull)
, enumDefault(0)
, enumIsFlags(zffalse)
, d(zfnew(_ZFP_ZFEnumDataPrivate))
{
}
_ZFP_ZFEnumData::~_ZFP_ZFEnumData(void) {
    zfdelete(d);
    d = zfnull;
}
void _ZFP_ZFEnumData::add(
        ZF_IN zfbool isEnableDuplicateValue
        , ZF_IN zfuint value
        , ZF_IN const zfstring &name
        ) {
    ZFCoreAssert(value != ZFEnumInvalid());
    ZFCoreAssert(name);
    _ZFP_ZFEnumDataPrivate::ValueMapType::iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end()) {
        ZFCoreAssertWithMessageTrim(isEnableDuplicateValue,
            "[ZFEnum] duplicate value %s (new: %s, old: %s) when define %s",
            value,
            name,
            it->second[0],
            this->ownerClass->classNameFull());
        it->second.add(name);
    }
    else {
        d->valueMap[value].add(name);
        d->vl.add(value);
        d->nl.add(name);
    }
    if(d->nameMap.find(name) == d->nameMap.end()) {
        d->nameMap[name] = value;
    }
}
zfindex _ZFP_ZFEnumData::enumCount(void) const {
    return d->vl.count();
}
zfindex _ZFP_ZFEnumData::enumIndexForValue(ZF_IN zfuint value) const {
    return d->vl.find(value);
}
zfuint _ZFP_ZFEnumData::enumValueAt(ZF_IN zfindex index) const {
    if(index >= d->vl.count()) {
        return ZFEnumInvalid();
    }
    else {
        return d->vl[index];
    }
}
const zfstring &_ZFP_ZFEnumData::enumNameAt(ZF_IN zfindex index) const {
    if(index >= d->nl.count()) {
        return zfstring::Empty();
    }
    else {
        return d->nl[index];
    }
}
zfbool _ZFP_ZFEnumData::enumValueContain(ZF_IN zfuint value) const {
    return (d->valueMap.find(value) != d->valueMap.end());
}
zfuint _ZFP_ZFEnumData::enumValueForName(ZF_IN const zfstring &name) const {
    if(name) {
        _ZFP_ZFEnumDataPrivate::NameMapType::iterator it = d->nameMap.find(name);
        if(it != d->nameMap.end()) {
            return it->second;
        }
    }
    return ZFEnumInvalid();
}
const zfstring &_ZFP_ZFEnumData::enumNameForValue(ZF_IN zfuint value) const {
    _ZFP_ZFEnumDataPrivate::ValueMapType::const_iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end()) {
        return it->second[0];
    }
    else {
        return ZFEnumNameInvalid();
    }
}

_ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass) {
    ZFCoreMutexLocker();
    _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(ownerClass->classNameFull());
    if(d != zfnull) {
        return d;
    }
    d = zfnew(_ZFP_ZFEnumData);
    _ZFP_ZFEnumDataMap.set(ownerClass->classNameFull(), ZFCorePointerForObject<_ZFP_ZFEnumData *>(d));
    d->ownerClass = ownerClass;
    return d;
}
void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass) {
    _ZFP_ZFEnumDataMap.remove(ownerClass->classNameFull());
}

// ============================================================
const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass) {
    return _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(enumClass->classNameFull());
}
void _ZFP_ZFEnumMethodReg(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const _ZFP_ZFEnumData *d
        ) {
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumIsFlags;
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfbool, zftext("EnumIsFlags"));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumDefault;
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, zftext("EnumDefault"));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumCount();
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfindex, zftext("EnumCount"));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumIndexForValue(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfindex, zftext("EnumIndexForValue"),
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumValueAt(index);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, zftext("EnumValueAt"),
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumNameAt(index);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            const zfstring &, zftext("EnumNameAt"),
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumValueContain(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfbool, zftext("EnumValueContain"),
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumValueForName(name);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, zftext("EnumValueForName"),
            ZFMP_IN(const zfstring &, name));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->ownerClass())->enumNameForValue(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            const zfstring &, zftext("EnumNameForValue"),
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
}

// ============================================================
ZFOBJECT_ON_INIT_USER_REGISTER_1(ZFEnum
    , ZFMP_IN(zfflags, value)
    ) {
    invokerObject->to<ZFEnum *>()->_ZFP_enumValue((zfuint)value);
}
ZFOBJECT_ON_INIT_USER_REGISTER_1(ZFEnum
    , ZFMP_IN(zfuint, value)
    ) {
    invokerObject->to<ZFEnum *>()->_ZFP_enumValue(value);
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfindex, enumCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfindex, enumIndexForValue
        , ZFMP_IN(zfuint, value)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfuint, enumValueAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, const zfstring &, enumNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfbool, enumValueContain
        , ZFMP_IN(zfuint, value)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfuint, enumValueForName
        , ZFMP_IN(const zfstring &, name)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, const zfstring &, enumNameForValue
        , ZFMP_IN(zfuint, value)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfbool, enumIsFlags)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfuint, enumDefault)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfuint const &, enumValue)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, const zfstring &, enumName)
ZFMETHOD_USER_REGISTER_DETAIL_1(ZFEnum,
    protected, ZFMethodTypeVirtual, ZF_CALLER_LINE,
    void, enumValue
    , ZFMP_IN(zfuint const &, value)
    ) {
    invokerObject->to<ZFEnum *>()->_ZFP_ZFEnum_value = value;
}
ZFMETHOD_USER_REGISTER_DETAIL_1(ZFEnum,
    protected, ZFMethodTypeVirtual, ZF_CALLER_LINE,
    void, enumValue
    , ZFMP_IN(ZFEnum *, value)
    ) {
    if(value != zfnull) {
        invokerObject->to<ZFEnum *>()->_ZFP_ZFEnum_value = value->enumValue();
    }
}

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(zfbool, zfflagsFromStringT
        , ZFMP_OUT(zfflags &, ret)
        , ZFMP_IN(const ZFClass *, enumClass)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfchar, separatorToken, '|')
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfflags, zfflagsFromString
        , ZFMP_IN(const ZFClass *, enumClass)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        , ZFMP_IN_OPT(zfchar, separatorToken, '|')
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_7(zfbool, zfflagsToStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFClass *, enumClass)
        , ZFMP_IN(zfflags const &, value)
        , ZFMP_IN_OPT(zfbool, includeNotConverted, zftrue)
        , ZFMP_IN_OPT(zfbool, exclusiveMode, zffalse)
        , ZFMP_OUT_OPT(zfflags *, notConverted, zfnull)
        , ZFMP_IN_OPT(zfchar, separatorToken, '|')
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_6(zfstring, zfflagsToString
        , ZFMP_IN(const ZFClass *, enumClass)
        , ZFMP_IN(zfflags const &, value)
        , ZFMP_IN_OPT(zfbool, includeNotConverted, zftrue)
        , ZFMP_IN_OPT(zfbool, exclusiveMode, zffalse)
        , ZFMP_OUT_OPT(zfflags *, notConverted, zfnull)
        , ZFMP_IN_OPT(zfchar, separatorToken, '|')
        )

ZF_NAMESPACE_GLOBAL_END

