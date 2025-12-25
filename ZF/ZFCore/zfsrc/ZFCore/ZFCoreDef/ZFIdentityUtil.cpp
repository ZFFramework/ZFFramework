#include "ZFIdentityUtil.h"
#include "ZFCoreArray.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIdentityGeneratorPrivate {
public:
    zfuint refCount;
    zfidentity cur;
    zfstlhashmap<zfidentity, zfbool> used;

public:
    _ZFP_ZFIdentityGeneratorPrivate(void)
    : refCount(1)
    , cur(zfidentityInvalid())
    , used()
    {
    }
};

// ============================================================
ZFIdentityGenerator::ZFIdentityGenerator(void)
: d(zfpoolNew(_ZFP_ZFIdentityGeneratorPrivate))
{
}
ZFIdentityGenerator::ZFIdentityGenerator(ZF_IN ZFIdentityGenerator const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFIdentityGenerator &ZFIdentityGenerator::operator = (ZF_IN ZFIdentityGenerator const &ref) {
    _ZFP_ZFIdentityGeneratorPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfpoolDelete(dTmp);
    }
    return *this;
}
zfbool ZFIdentityGenerator::operator == (ZF_IN ZFIdentityGenerator const &ref) const {
    return (d == ref.d);
}
ZFIdentityGenerator::~ZFIdentityGenerator(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfpoolDelete(d);
    }
}

zfidentity ZFIdentityGenerator::idAcquire(void) {
    do {
        ++(d->cur);
    } while(d->cur == zfidentityInvalid() || d->used.find(d->cur) != d->used.end());
    d->used[d->cur] = zftrue;
    return d->cur;
}
zfbool ZFIdentityGenerator::idRelease(ZF_IN zfidentity identity) {
    return d->used.erase(identity) != 0;
}

zfbool ZFIdentityGenerator::idExist(ZF_IN zfidentity identity) const {
    return (d->used.find(identity) != d->used.end());
}
void ZFIdentityGenerator::idExistGetAll(ZF_IN_OUT ZFCoreArray<zfidentity> &ret) const {
    ret.capacity(ret.capacity() + (zfindex)d->used.size());
    for(zfstlhashmap<zfidentity, zfbool>::iterator it = d->used.begin(); it != d->used.end(); ++it) {
        ret.add(it->first);
    }
}

void ZFIdentityGenerator::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFIdentityGenerator %s (%s)>", (const void *)this, (zfindex)d->used.size());
}

// ============================================================
zfidentity zfidentityCalcString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    // FNV-1a
    // https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
    zft_zfidentity hash = 0x811c9dc5;
    if(src) {
        if(srcLen == zfindexMax()) {
            while(*src) {
                hash = (hash ^ *src) * 0x01000193;
                ++src;
            }
        }
        else {
            const zfchar *srcEnd = src + srcLen;
            while(src < srcEnd) {
                hash = (hash ^ *src) * 0x01000193;
                ++src;
            }
        }
    }
    return hash;
}
zfidentity zfidentityCalcBuf(
        ZF_IN const void *src
        , ZF_IN zfindex srcLen
        ) {
    const zfbyte *p = (const zfbyte *)src;
    const zfbyte *pEnd = p + srcLen;
    zft_zfidentity hash = 0x811c9dc5;
    if(p) {
        while(p < pEnd) {
            hash = (hash ^ *p) * 0x01000193;
            ++p;
        }
    }
    return hash;
}

ZF_NAMESPACE_GLOBAL_END

