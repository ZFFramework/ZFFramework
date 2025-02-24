#include "ZFEnumDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFEnumDataHolder) {
}
ZFCoreMap enumDataMap;
ZF_STATIC_INITIALIZER_END(ZFEnumDataHolder)
#define _ZFP_ZFEnumDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFEnumDataHolder)->enumDataMap)

// ============================================================
zfclassNotPOD _ZFP_ZFEnumDataPrivate {
public:
    typedef zfstlmap<zfuint, ZFCoreArray<zfstring> > ValueMapType;
    ValueMapType valueMap;
    typedef zfstlmap<zfstring, zfuint> NameMapType;
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

ZF_NAMESPACE_GLOBAL_END

