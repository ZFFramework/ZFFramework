#include "ZFCoreSet.h"
#include "ZFCoreMapPrivate"
#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFCoreSetImpl : zfextend _ZFP_ZFCoreSet {
public:
    virtual ~_ZFP_ZFCoreSetImpl(void) {
        _ZFP_ZFCoreSetImpl::removeAll();
    }
public:
    typedef zfimplhashmap<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *, _ZFP_ZFCoreMapKeyHash, _ZFP_ZFCoreMapKeyEqual> MapType;
public:
    MapType m;

public:
    zfoverride
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN zfindex maxCount
            , ZF_IN const ZFTokenForContainer &token
            ) {
        zfindex count = 0;
        ret += token.tokenLeft;
        if(!this->isEmpty()) {
            for(zfiter it = this->iter(); it && count < maxCount; ++it, ++count) {
                if(count > 0) {
                    ret += token.tokenSeparator;
                }
                ret += token.tokenValueLeft;
                this->iterValue(it)->implInfo(ret);
                ret += token.tokenValueRight;
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
    zfoverride
    virtual ZFCompareResult objectCompareValue(ZF_IN const _ZFP_ZFCoreSet *ref) {
        MapType const &mRef = ((const _ZFP_ZFCoreSetImpl *)ref)->m;
        if(m.size() != mRef.size()) {
            return ZFCompareUncomparable;
        }
        for(MapType::const_iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            MapType::iterator it = m.find(itRef->first);
            if(it == m.end() || !it->first->implEqual(itRef->first)) {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareEqual;
    }
    zfoverride
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreSet *ref) {
        this->removeAll();
        this->addFrom(ref);
    }
    zfoverride
    virtual void capacity(ZF_IN zfindex capacity) {
        return m.reserve((zfstlsize)capacity);
    }
    zfoverride
    virtual zfindex count(void) {
        return (zfindex)m.size();
    }
    zfoverride
    virtual zfbool isEmpty(void) {
        return m.empty();
    }
    zfoverride
    virtual zfbool isContain(ZF_IN _ZFP_ZFCoreMapKey *key) {
        if(m.find(key) != m.end()) {
            key->implDestroy();
            return zftrue;
        }
        else {
            key->implDestroy();
            return zffalse;
        }
    }
    zfoverride
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreSet *ref) {
        MapType &mRef = ((_ZFP_ZFCoreSetImpl *)ref)->m;
        if(mRef.empty()) {
            return;
        }
        m.reserve(m.size() + mRef.size());
        for(MapType::iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreMapKey *key = itRef->first->implCopy();
            zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, zfnull));
            if(!insertResult.second) {
                key->implDestroy();
            }
        }
    }
    zfoverride
    virtual void add(ZF_IN _ZFP_ZFCoreMapKey *key) {
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, zfnull));
        if(!insertResult.second) {
            key->implDestroy();
        }
    }
    zfoverride
    virtual void remove(ZF_IN _ZFP_ZFCoreMapKey *key) {
        MapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            _ZFP_ZFCoreMapKey *keyTmp = const_cast<_ZFP_ZFCoreMapKey *>(it->first);
            m.erase(it);
            keyTmp->implDestroy();
        }
    }
    zfoverride
    virtual void removeAll(void) {
        if(!m.empty()) {
            void **tmp = (void **)zfmalloc(sizeof(void *) * m.size() * 2);
            zfindex nTmp = 0;
            for(MapType::iterator it = m.begin(); it != m.end(); ++it) {
                tmp[nTmp++] = const_cast<_ZFP_ZFCoreMapKey *>(it->first);
            }
            m.clear();
            for(zfstlsize i = 0; i < nTmp; ) {
                ((_ZFP_ZFCoreMapKey *)tmp[i++])->implDestroy();
            }
            zffree(tmp);
        }
    }

    zfoverride
    virtual zfiter iter(void) {
        return m.iter();
    }
    zfoverride
    virtual zfiter iterFind(ZF_IN _ZFP_ZFCoreMapKey *key) {
        zfiter it = m.iterFind(key);
        key->implDestroy();
        return it;
    }
    zfoverride
    virtual const _ZFP_ZFCoreMapKey *iterValue(ZF_IN const zfiter &it) {
        return m.iterKey(it);
    }
    zfoverride
    virtual void iterRemove(ZF_IN_OUT zfiter &it) {
        _ZFP_ZFCoreMapKey *key = const_cast<_ZFP_ZFCoreMapKey *>(m.iterKey(it));
        m.iterRemove(it);
        key->implDestroy();
    }
    zfoverride
    virtual zfiter iterAdd(ZF_IN _ZFP_ZFCoreMapKey *key) {
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, zfnull));
        if(!insertResult.second) {
            key->implDestroy();
        }
        return m.iter(insertResult.first);
    }
};

// ============================================================
_ZFP_ZFCoreSet *_ZFP_ZFCoreSet::create(void) {
    return zfpoolNew(_ZFP_ZFCoreSetImpl);
}
void _ZFP_ZFCoreSet::destroy(ZF_IN _ZFP_ZFCoreSet *d) {
    zfpoolDelete((_ZFP_ZFCoreSetImpl *)d);
}

ZF_NAMESPACE_GLOBAL_END

