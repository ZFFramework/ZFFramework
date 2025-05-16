#include "ZFFile.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileOpenOption
ZFENUM_DEFINE_FLAGS(ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
// ZFFileFindData
zfclassNotPOD _ZFP_ZFFileFindDataPrivate {
public:
    typedef zfstlhashmap<zfstring, zfauto> ImplTagMapType;

public:
    zfuint refCount;
    zfstring implName; // empty shows find not started
    void *implUserData;
    ZFFileFindData::Impl impl;
    ImplTagMapType *implTagMap;
public:
    _ZFP_ZFFileFindDataPrivate(void)
    : refCount(1)
    , implName()
    , implUserData(zfnull)
    , impl()
    , implTagMap(zfnull)
    {
    }
    ~_ZFP_ZFFileFindDataPrivate(void) {
        ZFCoreAssertWithMessage(this->implName.isEmpty(),
            "have you forgot to close find? module: %s",
            this->implName);
        if(this->implTagMap != zfnull) {
            zfdelete(this->implTagMap);
        }
    }
};

ZFFileFindData::ZFFileFindData(void)
: d(zfnew(_ZFP_ZFFileFindDataPrivate))
{
}
ZFFileFindData::ZFFileFindData(ZF_IN ZFFileFindData const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFFileFindData &ZFFileFindData::operator = (ZF_IN ZFFileFindData const &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFFileFindData::operator == (ZF_IN ZFFileFindData const &ref) const {
    return (d->impl.nativeFd == ref.d->impl.nativeFd);
}
ZFFileFindData::~ZFFileFindData(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}

void ZFFileFindData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    if(this->isDir()) {
        ret += "(dir)";
    }
    else {
        ret += "(file)";
    }
    ret += this->name();
}

ZFFileFindData::Impl &ZFFileFindData::impl(void) const {
    return d->impl;
}
const zfstring &ZFFileFindData::implName(void) const {
    return d->implName;
}
void *ZFFileFindData::implUserData(void) const {
    return d->implUserData;
}
void ZFFileFindData::implAttach(
        ZF_IN const zfstring &implName
        , ZF_IN_OPT void *implUserData /* = zfnull */
        ) {
    ZFCoreAssertWithMessage(d->implName.isEmpty(),
        "have you forgot to close find? current module: %s",
        d->implName);
    ZFCoreAssert(implName);
    d->implName = implName;
    d->implUserData = implUserData;
}
void ZFFileFindData::implDetach(void) {
    d->implName.removeAll();
    d->implUserData = zfnull;
}
void *ZFFileFindData::implCheck(ZF_IN const zfstring &implName) const {
    ZFCoreAssertWithMessage(!d->implName.isEmpty(), "find not started");
    ZFCoreAssertWithMessage(zfstringIsEqual(d->implName, implName),
        "have you forgot to close find? desired, %s, got: %s",
        implName, d->implName);
    return d->implUserData;
}

void ZFFileFindData::implTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *value
        ) const {
    if(!key) {
        return;
    }
    if(value == zfnull) {
        if(d->implTagMap != zfnull) {
            d->implTagMap->erase(key);
        }
    }
    else {
        if(d->implTagMap == zfnull) {
            d->implTagMap = zfnew(_ZFP_ZFFileFindDataPrivate::ImplTagMapType);
        }
        (*(d->implTagMap))[key] = value;
    }
}
zfany ZFFileFindData::implTag(ZF_IN const zfstring &key) const {
    if(!key || d->implTagMap) {
        return zfnull;
    }
    else {
        _ZFP_ZFFileFindDataPrivate::ImplTagMapType::iterator it = d->implTagMap->find(key);
        if(it != d->implTagMap->end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFileFindData, ZFFileFindData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, const zfstring &, name)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, zfbool, isDir)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, const zfstring &, implName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, void *, implUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFileFindData, void, implAttach
        , ZFMP_IN(const zfstring &, implName)
        , ZFMP_IN_OPT(void *, implUserData, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, void, implDetach)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFileFindData, void *, implCheck
        , ZFMP_IN(const zfstring &, implName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFileFindData, void, implTag
        , ZFMP_IN(const zfstring &, key)
        , ZFMP_IN(ZFObject *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFileFindData, zfany, implTag
        , ZFMP_IN(const zfstring &, key)
        )

ZF_NAMESPACE_GLOBAL_END

