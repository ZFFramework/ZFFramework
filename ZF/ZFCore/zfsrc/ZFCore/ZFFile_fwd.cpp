#include "ZFFile.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileSeparator
const zfchar _ZFP_ZFFileSeparator = '/';
const zfchar *_ZFP_ZFFileSeparatorString = "/";
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(zfchar, ZFFileSeparator, _ZFP_ZFFileSeparator)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(const zfchar *, ZFFileSeparatorString, _ZFP_ZFFileSeparatorString)

// ============================================================
// ZFFileOpenOption
ZFENUM_DEFINE_FLAGS(ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
// ZFFileFindData
zfclassNotPOD _ZFP_ZFFileFindDataPrivate {
public:
    typedef zfstlmap<zfstring, zfauto> ImplTagMapType;

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
        zfCoreAssertWithMessage(this->implName.isEmpty(),
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
    if(this->fileIsDir()) {
        ret += "(dir)";
    }
    else {
        ret += "(file)";
    }
    ret += this->fileName();
}

ZFFileFindData::Impl &ZFFileFindData::impl(void) const {
    return d->impl;
}
const zfchar *ZFFileFindData::implName(void) const {
    return (d->implName.isEmpty() ? zfnull : d->implName.cString());
}
void *ZFFileFindData::implUserData(void) const {
    return d->implUserData;
}
void ZFFileFindData::implAttach(
        ZF_IN const zfchar *implName
        , ZF_IN_OPT void *implUserData /* = zfnull */
        ) {
    zfCoreAssertWithMessage(d->implName.isEmpty(),
        "have you forgot to close find? current module: %s",
        d->implName);
    zfCoreAssert(!zfstringIsEmpty(implName));
    d->implName = implName;
    d->implUserData = implUserData;
}
void ZFFileFindData::implDetach(void) {
    d->implName.removeAll();
    d->implUserData = zfnull;
}
void *ZFFileFindData::implCheck(ZF_IN const zfchar *implName) const {
    zfCoreAssertWithMessage(!d->implName.isEmpty(), "find not started");
    zfCoreAssertWithMessage(zfstringIsEqual(d->implName.cString(), implName),
        "have you forgot to close find? desired, %s, got: %s",
        implName, d->implName);
    return d->implUserData;
}

void ZFFileFindData::implTag(
        ZF_IN const zfchar *key
        , ZF_IN ZFObject *value
        ) const {
    if(zfstringIsEmpty(key)) {
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
ZFObject *ZFFileFindData::implTag(ZF_IN const zfchar *key) const {
    if(zfstringIsEmpty(key) || d->implTagMap) {
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, const zfchar *, fileName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, zfbool, fileIsDir)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, const zfchar *, implName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, void *, implUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFileFindData, void, implAttach
        , ZFMP_IN(const zfchar *, implName)
        , ZFMP_IN_OPT(void *, implUserData, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, void, implDetach)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFileFindData, void *, implCheck
        , ZFMP_IN(const zfchar *, implName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFileFindData, void, implTag
        , ZFMP_IN(const zfchar *, key)
        , ZFMP_IN(ZFObject *, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFileFindData, ZFObject *, implTag
        , ZFMP_IN(const zfchar *, key)
        )

ZF_NAMESPACE_GLOBAL_END

