#include "ZFCoreOrderMap.h"
#include "ZFCoreMapPrivate"
#include "../ZFSTLWrapper/zfstlordermap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFCoreOrderMapImpl : zfextend _ZFP_ZFCoreOrderMap {
public:
    virtual ~_ZFP_ZFCoreOrderMapImpl(void) {
        _ZFP_ZFCoreOrderMapImpl::removeAll();
    }
public:
    typedef zfimplordermap<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *, _ZFP_ZFCoreMapKeyHash, _ZFP_ZFCoreMapKeyEqual> MapType;
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
    virtual ZFCompareResult objectCompareValue(ZF_IN const _ZFP_ZFCoreOrderMap *ref) {
        MapType const &mRef = ((const _ZFP_ZFCoreOrderMapImpl *)ref)->m;
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
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMap *ref) {
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
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMap *ref) {
        MapType &mRef = ((_ZFP_ZFCoreOrderMapImpl *)ref)->m;
        if(mRef.empty()) {
            return;
        }
        m.reserve(m.size() + mRef.size());
        for(MapType::iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreMapKey *key = itRef->first->implCopy();
            _ZFP_ZFCoreMapValue *value = itRef->second->implCopy();
            zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, value));
            if(!insertResult.second) {
                _ZFP_ZFCoreMapValue *valueOld = insertResult.first->second;
                insertResult.first->second = value;
                key->implDestroy();
                valueOld->implDestroy();
            }
        }
    }
    zfoverride
    virtual void set(ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN _ZFP_ZFCoreMapValue *value) {
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, value));
        if(!insertResult.second) {
            _ZFP_ZFCoreMapValue *valueOld = insertResult.first->second;
            insertResult.first->second = value;
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
        _ZFP_ZFCoreMapValue *value = fn_ValueCreate();
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, value));
        if(insertResult.second) {
            return value;
        }
        else {
            key->implDestroy();
            value->implDestroy();
            return insertResult.first->second;
        }
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
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, value));
        if(!insertResult.second) {
            _ZFP_ZFCoreMapValue *valueOld = insertResult.first->second;
            insertResult.first->second = value;
            key->implDestroy();
            valueOld->implDestroy();
        }
        return m.iter(insertResult.first);
    }
    zfoverride
    virtual zfiter iterAccess(ZF_IN _ZFP_ZFCoreMapKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        _ZFP_ZFCoreMapValue *value = fn_ValueCreate();
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const _ZFP_ZFCoreMapKey *, _ZFP_ZFCoreMapValue *>(key, value));
        if(!insertResult.second) {
            key->implDestroy();
            value->implDestroy();
        }
        return m.iter(insertResult.first);
    }

    // ============================================================
    // order map spec
    zfoverride
    virtual void move(ZF_IN_OUT zfiter &from, ZF_IN const zfiter &to) {
        m.iterMove(from, to);
    }
};

// ============================================================
_ZFP_ZFCoreOrderMap *_ZFP_ZFCoreOrderMap::create(void) {
    return zfpoolNew(_ZFP_ZFCoreOrderMapImpl);
}
void _ZFP_ZFCoreOrderMap::destroy(ZF_IN _ZFP_ZFCoreOrderMap *d) {
    zfpoolDelete((_ZFP_ZFCoreOrderMapImpl *)d);
}

ZF_NAMESPACE_GLOBAL_END

