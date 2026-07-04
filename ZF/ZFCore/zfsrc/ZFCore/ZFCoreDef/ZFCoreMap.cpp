#include "ZFCoreMap.h"
#include "ZFCoreMapPrivate"
#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFCoreMapImpl : zfextend _ZFP_ZFCoreMap {
public:
    virtual ~_ZFP_ZFCoreMapImpl(void) {
        _ZFP_ZFCoreMapImpl::removeAll();
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
            , ZF_IN const ZFTokenForKeyValueContainer &token
            ) {
        zfindex count = 0;
        if(!this->isEmpty()) {
            for(zfiter it = this->iter(); it && count < maxCount; ++it, ++count) {
                if(count > 0) {
                    ret += token.tokenSeparator;
                }

                ret += token.tokenPairLeft;
                {
                    ret += token.tokenKeyLeft;
                    this->iterKey(it)->implInfo(ret);
                    ret += token.tokenKeyRight;
                }
                ret += token.tokenPairSeparator;
                {
                    ret += token.tokenValueLeft;
                    this->iterValue(it)->implInfo(ret);
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
    }
    zfoverride
    virtual ZFCompareResult objectCompareValue(ZF_IN const _ZFP_ZFCoreMap *ref) {
        MapType const &mRef = ((const _ZFP_ZFCoreMapImpl *)ref)->m;
        if(m.size() != mRef.size()) {
            return ZFCompareUncomparable;
        }
        for(MapType::const_iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            MapType::iterator it = m.find(itRef->first);
            if(it == m.end() || !it->second->implEqual(itRef->second)) {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareEqual;
    }
    zfoverride
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreMap *ref) {
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
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreMap *ref) {
        MapType &mRef = ((_ZFP_ZFCoreMapImpl *)ref)->m;
        if(mRef.empty()) {
            return;
        }
        m.reserve(m.size() + mRef.size());
        for(MapType::iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreMapKey *key = itRef->first->implCopy();
            _ZFP_ZFCoreMapValue *value = itRef->second->implCopy();
            MapType::iterator implIt;
            if(m.iterAccess(implIt, key, value)) {
                _ZFP_ZFCoreMapValue *valueOld = implIt->second;
                implIt->second = value;
                key->implDestroy();
                valueOld->implDestroy();
            }
        }
    }
    zfoverride
    virtual void set(ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN _ZFP_ZFCoreMapValue *value) {
        MapType::iterator implIt;
        if(m.iterAccess(implIt, key, value)) {
            _ZFP_ZFCoreMapValue *valueOld = implIt->second;
            implIt->second = value;
            key->implDestroy();
            valueOld->implDestroy();
        }
    }
    zfoverride
    virtual _ZFP_ZFCoreMapValue *get(ZF_IN _ZFP_ZFCoreMapKey *key) {
        MapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual _ZFP_ZFCoreMapValue *access(ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        MapType::iterator implIt;
        if(m.iterAccess(implIt, key)) {
            key->implDestroy();
        }
        else {
            implIt->second = fn_ValueCreate();
        }
        return implIt->second;
    }
    zfoverride
    virtual void remove(ZF_IN _ZFP_ZFCoreMapKey *key) {
        MapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            _ZFP_ZFCoreMapKey *keyTmp = const_cast<_ZFP_ZFCoreMapKey *>(it->first);
            _ZFP_ZFCoreMapValue *valueTmp = it->second;
            m.erase(it);
            keyTmp->implDestroy();
            valueTmp->implDestroy();
        }
    }
    zfoverride
    virtual void removeAll(void) {
        if(!m.empty()) {
            void **tmp = (void **)zfmalloc(sizeof(void *) * m.size() * 2);
            zfindex nTmp = 0;
            for(MapType::iterator it = m.begin(); it != m.end(); ++it) {
                tmp[nTmp++] = const_cast<_ZFP_ZFCoreMapKey *>(it->first);
                tmp[nTmp++] = it->second;
            }
            m.clear();
            for(zfstlsize i = 0; i < nTmp; ) {
                ((_ZFP_ZFCoreMapKey *)tmp[i++])->implDestroy();
                ((_ZFP_ZFCoreMapValue *)tmp[i++])->implDestroy();
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
    virtual const _ZFP_ZFCoreMapKey *iterKey(ZF_IN const zfiter &it) {
        return m.iterKey(it);
    }
    zfoverride
    virtual _ZFP_ZFCoreMapValue *iterValue(ZF_IN const zfiter &it) {
        return m.iterValue(it);
    }
    zfoverride
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN _ZFP_ZFCoreMapValue *value) {
        _ZFP_ZFCoreMapValue *tmp = m.iterValue(it);
        m.iterValue(it, value);
        tmp->implDestroy();
    }
    zfoverride
    virtual void iterRemove(ZF_IN_OUT zfiter &it) {
        _ZFP_ZFCoreMapKey *key = const_cast<_ZFP_ZFCoreMapKey *>(m.iterKey(it));
        _ZFP_ZFCoreMapValue *value = m.iterValue(it);
        m.iterRemove(it);
        key->implDestroy();
        value->implDestroy();
    }
    zfoverride
    virtual zfiter iterAdd(ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN _ZFP_ZFCoreMapValue *value) {
        MapType::iterator implIt;
        if(m.iterAccess(implIt, key, value)) {
            _ZFP_ZFCoreMapValue *valueOld = implIt->second;
            implIt->second = value;
            key->implDestroy();
            valueOld->implDestroy();
        }
        return m.iter(implIt);
    }
    zfoverride
    virtual zfbool iterAccess(ZF_OUT zfiter &it, ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        _ZFP_ZFCoreMapValue *value = fn_ValueCreate();
        if(m.iterAccess(it, key, value)) {
            key->implDestroy();
            value->implDestroy();
            return zftrue;
        }
        else {
            return zffalse;
        }
    }
};

// ============================================================
_ZFP_ZFCoreMap *_ZFP_ZFCoreMap::create(void) {
    return zfpoolNew(_ZFP_ZFCoreMapImpl);
}
void _ZFP_ZFCoreMap::destroy(ZF_IN _ZFP_ZFCoreMap *d) {
    zfpoolDelete((_ZFP_ZFCoreMapImpl *)d);
}

ZF_NAMESPACE_GLOBAL_END

