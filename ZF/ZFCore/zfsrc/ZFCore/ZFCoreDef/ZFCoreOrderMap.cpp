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

zfclassNotPOD _ZFP_ZFCoreOrderMapImpl : zfextend _ZFP_ZFCoreOrderMap {
public:
    virtual ~_ZFP_ZFCoreOrderMapImpl(void) {
        _ZFP_ZFCoreOrderMapImpl::removeAll();
    }
public:
    typedef zfimplordermap<const BaseKey *, BaseValue *, _ZFP_ZFCoreOrderMapHash, _ZFP_ZFCoreOrderMapEqual> MapType;
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
    virtual zfindex count(void) {
        return (zfindex)m.size();
    }
    zfoverride
    virtual zfbool isEmpty(void) {
        return m.empty();
    }
    zfoverride
    virtual zfbool isContain(ZF_IN BaseKey *key) {
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
            BaseKey *key = itRef->first->implCopy();
            BaseValue *value = itRef->second->implCopy();
            zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const BaseKey *, BaseValue *>(key, value));
            if(!insertResult.second) {
                BaseValue *valueOld = insertResult.first->second;
                insertResult.first->second = value;
                key->implDestroy();
                valueOld->implDestroy();
            }
        }
    }
    zfoverride
    virtual void set(ZF_IN BaseKey *key, ZF_IN BaseValue *value) {
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const BaseKey *, BaseValue *>(key, value));
        if(!insertResult.second) {
            BaseValue *valueOld = insertResult.first->second;
            insertResult.first->second = value;
            key->implDestroy();
            valueOld->implDestroy();
        }
    }
    zfoverride
    virtual BaseValue *get(ZF_IN BaseKey *key) {
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
    virtual BaseValue *access(ZF_IN BaseKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        BaseValue *value = fn_ValueCreate();
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const BaseKey *, BaseValue *>(key, value));
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
    virtual void remove(ZF_IN BaseKey *key) {
        MapType::iterator it = m.find(key);
        key->implDestroy();
        if(it != m.end()) {
            BaseKey *keyTmp = const_cast<BaseKey *>(it->first);
            BaseValue *valueTmp = it->second;
            m.erase(it);
            keyTmp->implDestroy();
            valueTmp->implDestroy();
        }
    }
    zfoverride
    virtual void removeAll(void) {
        if(!m.empty()) {
            zfstlvector<BaseKey *> tmpKey;
            tmpKey.reserve(m.size());
            zfstlvector<BaseValue *> tmpValue;
            tmpValue.reserve(m.size());
            for(MapType::iterator it = m.begin(); it != m.end(); ++it) {
                tmpKey.push_back(const_cast<BaseKey *>(it->first));
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
    virtual zfiter iterFind(ZF_IN BaseKey *key) {
        zfiter it = m.iterFind(key);
        key->implDestroy();
        return it;
    }
    zfoverride
    virtual const BaseKey *iterKey(ZF_IN const zfiter &it) {
        return m.iterKey(it);
    }
    zfoverride
    virtual BaseValue *iterValue(ZF_IN const zfiter &it) {
        return m.iterValue(it);
    }
    zfoverride
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN BaseValue *value) {
        BaseValue *tmp = m.iterValue(it);
        m.iterValue(it, value);
        tmp->implDestroy();
    }
    zfoverride
    virtual void iterRemove(ZF_IN_OUT zfiter &it) {
        BaseKey *key = const_cast<BaseKey *>(m.iterKey(it));
        BaseValue *value = m.iterValue(it);
        m.iterRemove(it);
        key->implDestroy();
        value->implDestroy();
    }
    zfoverride
    virtual zfiter iterAdd(ZF_IN BaseKey *key, ZF_IN BaseValue *value) {
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const BaseKey *, BaseValue *>(key, value));
        if(!insertResult.second) {
            BaseValue *valueOld = insertResult.first->second;
            insertResult.first->second = value;
            key->implDestroy();
            valueOld->implDestroy();
        }
        return m.iter(insertResult.first);
    }
    zfoverride
    virtual zfiter iterAccess(ZF_IN BaseKey *key, ZF_IN Fn_ValueCreate fn_ValueCreate) {
        BaseValue *value = fn_ValueCreate();
        zfstlpair<MapType::iterator, bool> insertResult = m.insert(zfstlpair<const BaseKey *, BaseValue *>(key, value));
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

