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
zfclassNotPOD _ZFP_ZFCoreMapPrivate
{
public:
#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    typedef zfimplhashmap<zfstringRO, ZFCorePointerBase *, zfstring_zfstlHasher, zfstring_zfstlHashComparer> MapType;
#else
    typedef zfimplmap<zfstringRO, ZFCorePointerBase *> MapType;
#endif

public:
    zfuint refCount;
    _ZFP_ZFCoreMapPrivate::MapType m;

public:
    void removeAll(void)
    {
        if(!this->m.empty())
        {
            _ZFP_ZFCoreMapPrivate::MapType tmp;
            tmp.swap(this->m);
            for(_ZFP_ZFCoreMapPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it)
            {
                zfdelete(it->second);
            }
        }
    }

public:
    _ZFP_ZFCoreMapPrivate(void)
    : refCount(1)
    {
    }
    ~_ZFP_ZFCoreMapPrivate(void)
    {
        this->removeAll();
    }
};

// ============================================================
ZFCoreMap::ZFCoreMap(void)
: d(zfnew(_ZFP_ZFCoreMapPrivate))
{
}
ZFCoreMap::ZFCoreMap(ZF_IN const ZFCoreMap &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFCoreMap &ZFCoreMap::operator = (ZF_IN const ZFCoreMap &ref)
{
    _ZFP_ZFCoreMapPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFCoreMap::operator == (ZF_IN const ZFCoreMap &ref) const
{
    return (d == ref.d);
}
ZFCoreMap::~ZFCoreMap(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFCoreMap::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->objectInfoOfContentT(ret, 5);
}
void ZFCoreMap::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                     ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                                     ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */) const
{
    zfindex count = 0;
    ret += token.tokenLeft;
    if(!this->isEmpty())
    {
        for(zfiterator it = this->iter();
            this->iterValid(it) && count < maxCount;
            this->iterNext(it), ++count)
        {
            if(count > 0)
            {
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
                ZFCorePointerBase *value = this->iterValue(it);
                if(value == zfnull)
                {
                    ret += ZFTOKEN_zfnull;
                }
                else
                {
                    value->objectInfoOfContentT(ret);
                }
                ret += token.tokenValueRight;
            }
            ret += token.tokenPairRight;
        }
    }
    if(count < this->count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

void ZFCoreMap::swap(ZF_IN_OUT ZFCoreMap &ref)
{
    _ZFP_ZFCoreMapPrivate *dTmp = d;
    d = ref.d;
    ref.d = dTmp;
}

void ZFCoreMap::copyFrom(ZF_IN const ZFCoreMap &ref)
{
    if(d != ref.d)
    {
        this->removeAll();
        this->addFrom(ref);
    }
}

zfindex ZFCoreMap::objectRetainCount(void) const
{
    return d->refCount;
}

zfindex ZFCoreMap::count(void) const
{
    return d->m.size();
}

zfbool ZFCoreMap::isEmpty(void) const
{
    return d->m.empty();
}

zfbool ZFCoreMap::isContain(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    return (d->m.find(key) != d->m.end());
}

void ZFCoreMap::addFrom(ZF_IN const ZFCoreMap &ref)
{
    if(d != ref.d && !ref.isEmpty())
    {
        for(zfiterator it = ref.iter(); this->iterValid(it); this->iterNext(it))
        {
            this->set(this->iterKey(it), *(this->iterValue(it)));
        }
    }
}

void ZFCoreMap::set(ZF_IN const zfchar *key,
                    ZF_IN const ZFCorePointerBase &value)
{
    if(key == zfnull)
    {
        key = "";
    }

    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it == d->m.end())
    {
        d->m[key] = value.refNew();
    }
    else
    {
        ZFCorePointerBase *toDelete = it->second;
        it->second = value.refNew();
        toDelete->refDelete();
    }
}
ZFCorePointerBase *ZFCoreMap::get(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it == d->m.end())
    {
        return zfnull;
    }
    return it->second;
}

void ZFCoreMap::allKeyT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret) const
{
    ret.capacity(ret.count() + this->count());
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->first);
    }
}
void ZFCoreMap::allValueT(ZF_IN_OUT ZFCoreArray<ZFCorePointerBase *> &ret) const
{
    ret.capacity(ret.count() + this->count());
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->second);
    }
}

void ZFCoreMap::remove(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        key = "";
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it != d->m.end())
    {
        ZFCorePointerBase *savedValue = it->second;
        d->m.erase(it);
        savedValue->refDelete();
    }
}

void ZFCoreMap::removeAll(void)
{
    d->removeAll();
}

// ============================================================
// iterator
zfiterator ZFCoreMap::iter(void) const
{
    return d->m.iter();
}

zfiterator ZFCoreMap::iterFind(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    return d->m.iterFind(key);
}

zfbool ZFCoreMap::iterValid(ZF_IN const zfiterator &it) const
{
    return d->m.iterValid(it);
}

void ZFCoreMap::iterNext(ZF_IN_OUT zfiterator &it) const
{
    d->m.iterNext(it);
}

const zfchar *ZFCoreMap::iterKey(ZF_IN const zfiterator &it) const
{
    return d->m.iterKey(it);
}
ZFCorePointerBase *ZFCoreMap::iterValue(ZF_IN const zfiterator &it) const
{
    return d->m.iterValue(it);
}

void ZFCoreMap::iterValue(ZF_IN_OUT zfiterator &it,
                          ZF_IN const ZFCorePointerBase &newValue)
{
    ZFCorePointerBase *old = d->m.iterValue(it);
    d->m.iterValue(it, newValue.refNew());
    old->refDelete();
}
void ZFCoreMap::iterRemove(ZF_IN_OUT zfiterator &it)
{
    ZFCorePointerBase *old = d->m.iterValue(it);
    d->m.iterRemove(it);
    old->refDelete();
}

void ZFCoreMap::iterAdd(ZF_IN const zfchar *key,
                        ZF_IN const ZFCorePointerBase &value)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

