#include "ZFCoreMap.h"

#ifndef ZF_ENV_ZFCOREMAP_USE_HASHMAP
    #define ZF_ENV_ZFCOREMAP_USE_HASHMAP 1
#endif

#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    #include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"
#else
    #include "ZFCore/ZFSTLWrapper/zfstlmap.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFCoreMapPrivate {
public:
#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    typedef zfimplhashmap<zfstring, const ZFCorePointerBase *, zfstring_zfstlHash, zfstring_zfstlEqual> MapType;
#else
    typedef zfimplmap<zfstring, const ZFCorePointerBase *> MapType;
#endif

public:
    zfuint refCount;
    _ZFP_ZFCoreMapPrivate::MapType m;

public:
    void removeAll(void) {
        if(!this->m.empty()) {
            _ZFP_ZFCoreMapPrivate::MapType tmp;
            tmp.swap(this->m);
            for(_ZFP_ZFCoreMapPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
                it->second->refDelete();
            }
        }
    }

public:
    _ZFP_ZFCoreMapPrivate(void)
    : refCount(1)
    {
    }
    ~_ZFP_ZFCoreMapPrivate(void) {
        this->removeAll();
    }
};

// ============================================================
ZFCoreMap::ZFCoreMap(void)
: d(zfnull)
{
}
ZFCoreMap::ZFCoreMap(ZF_IN const ZFCoreMap &ref)
: d(ref.d)
{
    if(d) {
        ++(d->refCount);
    }
}
ZFCoreMap &ZFCoreMap::operator = (ZF_IN const ZFCoreMap &ref) {
    _ZFP_ZFCoreMapPrivate *dTmp = d;
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFCoreMap::operator == (ZF_IN const ZFCoreMap &ref) const {
    return (d == ref.d);
}
ZFCoreMap::~ZFCoreMap(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
}

void ZFCoreMap::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    this->objectInfoOfContentT(ret, 5);
}
void ZFCoreMap::objectInfoOfContentT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */
        ) const {
    zfindex count = 0;
    ret += token.tokenLeft;
    if(!this->isEmpty()) {
        for(zfiter it = this->iter(); it && count < maxCount; ++it, ++count) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }

            ret += token.tokenPairLeft;
            {
                ret += token.tokenKeyLeft;
                ret += this->iterKey(it);
                ret += token.tokenKeyRight;
            }
            ret += token.tokenPairSeparator;
            {
                ret += token.tokenValueLeft;
                const ZFCorePointerBase *value = this->iterValue(it);
                if(value == zfnull) {
                    ret += ZFTOKEN_zfnull;
                }
                else {
                    value->objectInfoOfContentT(ret);
                }
                ret += token.tokenValueRight;
            }
            ret += token.tokenPairRight;
        }
    }
    if(count < this->count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

void ZFCoreMap::swap(ZF_IN_OUT ZFCoreMap &ref) {
    _ZFP_ZFCoreMapPrivate *dTmp = d;
    d = ref.d;
    ref.d = dTmp;
}

void ZFCoreMap::copyFrom(ZF_IN const ZFCoreMap &ref) {
    if(d != ref.d) {
        this->removeAll();
        this->addFrom(ref);
    }
}

zfindex ZFCoreMap::objectRetainCount(void) const {
    return (zfindex)(d ? d->refCount : 0);
}

zfindex ZFCoreMap::count(void) const {
    return (zfindex)(d ? d->m.size() : 0);
}

zfbool ZFCoreMap::isEmpty(void) const {
    return d == zfnull || d->m.empty();
}

zfbool ZFCoreMap::isContain(ZF_IN const zfstring &key) const {
    return (d && d->m.find(key) != d->m.end());
}

void ZFCoreMap::addFrom(ZF_IN const ZFCoreMap &ref) {
    if(d != ref.d && !ref.isEmpty()) {
        for(zfiter it = ref.iter(); it; ++it) {
            this->set(this->iterKey(it), *(this->iterValue(it)));
        }
    }
}

void ZFCoreMap::set(
        ZF_IN const zfstring &key
        , ZF_IN const ZFCorePointerBase &value
        ) {
    if(d == zfnull) {
        d = zfpoolNew(_ZFP_ZFCoreMapPrivate);
    }

    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it == d->m.end()) {
        d->m[key] = value.refNew();
    }
    else {
        const ZFCorePointerBase *toDelete = it->second;
        if(toDelete != &value) {
            it->second = value.refNew();
            toDelete->refDelete();
        }
    }
}
const ZFCorePointerBase *ZFCoreMap::get(ZF_IN const zfstring &key) const {
    if(d) {
        _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
        if(it != d->m.end()) {
            return it->second;
        }
    }
    return zfnull;
}

void ZFCoreMap::allKeyT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) const {
    if(d) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
                it != d->m.end();
                ++it
                ) {
            ret.add(it->first);
        }
    }
}
void ZFCoreMap::allValueT(ZF_IN_OUT ZFCoreArray<const ZFCorePointerBase *> &ret) const {
    if(d) {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
                it != d->m.end();
                ++it
                ) {
            ret.add(it->second);
        }
    }
}

void ZFCoreMap::remove(ZF_IN const zfstring &key) {
    if(d) {
        _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
        if(it != d->m.end()) {
            const ZFCorePointerBase *savedValue = it->second;
            d->m.erase(it);
            savedValue->refDelete();
        }
    }
}

void ZFCoreMap::removeAll(void) {
    if(d) {
        d->removeAll();
    }
}

// ============================================================
// iterator
zfiter ZFCoreMap::iter(void) const {
    return d ? d->m.iter() : zfnull;
}

zfiter ZFCoreMap::iterFind(ZF_IN const zfstring &key) const {
    return d ? d->m.iterFind(key) : zfnull;
}

zfstring ZFCoreMap::iterKey(ZF_IN const zfiter &it) const {
    return d ? d->m.iterKey(it) : zfnull;
}
const ZFCorePointerBase *ZFCoreMap::iterValue(ZF_IN const zfiter &it) const {
    return d ? d->m.iterValue(it) : zfnull;
}

void ZFCoreMap::iterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN const ZFCorePointerBase &newValue
        ) {
    if(d) {
        const ZFCorePointerBase *old = d->m.iterValue(it);
        d->m.iterValue(it, newValue.refNew());
        old->refDelete();
    }
}
void ZFCoreMap::iterRemove(ZF_IN_OUT zfiter &it) {
    if(d) {
        const ZFCorePointerBase *old = d->m.iterValue(it);
        d->m.iterRemove(it);
        old->refDelete();
    }
}

void ZFCoreMap::iterAdd(
        ZF_IN const zfstring &key
        , ZF_IN const ZFCorePointerBase &value
        ) {
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

