#include "ZFCoreOrderMap.h"

#include "ZFCore/ZFSTLWrapper/zfstlordermap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFCoreOrderMapHash {
public:
    zfstlsize operator () (ZF_IN const _ZFP_ZFCoreOrderMap::BaseKey *key) const {
        return key->implHash();
    }
};
zfclassNotPOD _ZFP_ZFCoreOrderMapEqual {
public:
    zfbool operator () (ZF_IN const _ZFP_ZFCoreOrderMap::BaseKey *key0, ZF_IN const _ZFP_ZFCoreOrderMap::BaseKey *key1) const {
        return key0->implEqual(key1);
    }
};

typedef zfimplordermap<const _ZFP_ZFCoreOrderMap::BaseKey *, _ZFP_ZFCoreOrderMap::BaseValue *, _ZFP_ZFCoreOrderMapHash, _ZFP_ZFCoreOrderMapEqual> _ZFP_ZFCoreOrderMapType;

zfclassNotPOD _ZFP_ZFCoreOrderMapImpl : zfextend _ZFP_ZFCoreOrderMap {
public:
    virtual ~_ZFP_ZFCoreOrderMapImpl(void) {
        _ZFP_ZFCoreOrderMapImpl::removeAll();
    }
public:
    _ZFP_ZFCoreOrderMapType m;

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
        _ZFP_ZFCoreOrderMapType const &mRef = ((const _ZFP_ZFCoreOrderMapImpl *)ref)->m;
        if(m.size() != mRef.size()) {
            return ZFCompareUncomparable;
        }
        for(_ZFP_ZFCoreOrderMapType::const_iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreOrderMapType::iterator it = m.find(itRef->first);
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
    virtual zfindex count(void) {
        return (zfindex)m.size();
    }
    zfoverride
    virtual zfbool isEmpty(void) {
        return m.empty();
    }
    zfoverride
    virtual zfbool isContain(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key) {
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
        _ZFP_ZFCoreOrderMapType &mRef = ((_ZFP_ZFCoreOrderMapImpl *)ref)->m;
        m.reserve(m.size() + mRef.size());
        for(_ZFP_ZFCoreOrderMapType::iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreOrderMapType::iterator it = m.find(itRef->first);
            if(it != m.end()) {
                it->second->implCopy(itRef->second);
            }
            else {
                m[itRef->first->implCopy()] = itRef->second->implCopy();
            }
        }
    }
    zfoverride
    virtual void set(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key, ZF_IN _ZFP_ZFCoreOrderMap::BaseValue *value) {
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(key);
        if(it != m.end()) {
            _ZFP_ZFCoreOrderMap::BaseValue *tmp = it->second;
            it->second = value;
            tmp->implDestroy();
        }
        else {
            m[key] = value;
        }
    }
    zfoverride
    virtual _ZFP_ZFCoreOrderMap::BaseValue *get(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key) {
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual _ZFP_ZFCoreOrderMap::BaseValue *access(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(key);
        if(it != m.end()) {
            key->implDestroy();
            return it->second;
        }
        else {
            return (m[key] = fn_ValueCreate());
        }
    }
    zfoverride
    virtual void remove(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key) {
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            _ZFP_ZFCoreOrderMap::BaseKey *keyTmp = const_cast<_ZFP_ZFCoreOrderMap::BaseKey *>(it->first);
            _ZFP_ZFCoreOrderMap::BaseValue *valueTmp = it->second;
            m.erase(it);
            keyTmp->implDestroy();
            valueTmp->implDestroy();
        }
    }
    zfoverride
    virtual void removeAll(void) {
        if(!m.empty()) {
            zfstlvector<_ZFP_ZFCoreOrderMap::BaseKey *> tmpKey;
            tmpKey.reserve(m.size());
            zfstlvector<_ZFP_ZFCoreOrderMap::BaseValue *> tmpValue;
            tmpValue.reserve(m.size());
            for(_ZFP_ZFCoreOrderMapType::iterator it = m.begin(); it != m.end(); ++it) {
                tmpKey.push_back(const_cast<_ZFP_ZFCoreOrderMap::BaseKey *>(it->first));
                tmpValue.push_back(it->second);
            }
            m.clear();
            for(zfstlsize i = 0; i < tmpKey.size(); ++i) {
                tmpKey[i]->implDestroy();
                tmpValue[i]->implDestroy();
            }
        }
    }

    zfoverride
    virtual zfiter iter(void) {
        return m.iter();
    }
    zfoverride
    virtual zfiter iterFind(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key) {
        zfiter it = m.iterFind(key);
        key->implDestroy();
        return it;
    }
    zfoverride
    virtual const _ZFP_ZFCoreOrderMap::BaseKey *iterKey(ZF_IN const zfiter &it) {
        return m.iterKey(it);
    }
    zfoverride
    virtual _ZFP_ZFCoreOrderMap::BaseValue *iterValue(ZF_IN const zfiter &it) {
        return m.iterValue(it);
    }
    zfoverride
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN _ZFP_ZFCoreOrderMap::BaseValue *value) {
        _ZFP_ZFCoreOrderMap::BaseValue *tmp = m.iterValue(it);
        m.iterValue(it, value);
        tmp->implDestroy();
    }
    zfoverride
    virtual void iterRemove(ZF_IN_OUT zfiter &it) {
        _ZFP_ZFCoreOrderMap::BaseKey *key = const_cast<_ZFP_ZFCoreOrderMap::BaseKey *>(m.iterKey(it));
        _ZFP_ZFCoreOrderMap::BaseValue *value = m.iterValue(it);
        m.iterRemove(it);
        key->implDestroy();
        value->implDestroy();
    }
    zfoverride
    virtual zfiter iterAdd(ZF_IN _ZFP_ZFCoreOrderMap::BaseKey *key, ZF_IN _ZFP_ZFCoreOrderMap::BaseValue *value) {
        zfiter it = m.iterFind(key);
        if(it) {
            _ZFP_ZFCoreOrderMap::BaseValue *tmp = m.iterValue(it);
            m.iterValue(it, value);
            key->implDestroy();
            tmp->implDestroy();
            return it;
        }
        else {
            return m.iterAdd(key, value);
        }
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

