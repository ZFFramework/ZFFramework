#include "ZFCoreOrderMap.h"

#include "ZFCore/ZFSTLWrapper/zfstlordermap.h"
#include "ZFCore/ZFSTLWrapper/zfstlvector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_ZFCoreOrderMapKey {
public:
    const _ZFP_ZFCoreOrderMapPrivate *owner;
    const void *key;
};

zfclassNotPOD _ZFP_ZFCoreOrderMapHash {
public:
    zfstlsize operator () (ZF_IN const _ZFP_ZFCoreOrderMapKey &key) const {
        return key.owner->fn_Hash(key.key);
    }
};
zfclassNotPOD _ZFP_ZFCoreOrderMapEqual {
public:
    zfbool operator () (ZF_IN const _ZFP_ZFCoreOrderMapKey &key0, ZF_IN const _ZFP_ZFCoreOrderMapKey &key1) const {
        return key0.owner->fn_Equal(key0.key, key1.key);
    }
};

typedef zfimplordermap<_ZFP_ZFCoreOrderMapKey, void *, _ZFP_ZFCoreOrderMapHash, _ZFP_ZFCoreOrderMapEqual> _ZFP_ZFCoreOrderMapType;

zfclassNotPOD _ZFP_ZFCoreOrderMapImpl : zfextend _ZFP_ZFCoreOrderMapPrivate {
public:
    _ZFP_ZFCoreOrderMapImpl(
            ZF_IN Fn_Hash fn_Hash
            , ZF_IN Fn_Equal fn_Equal
            , ZF_IN Fn_KeyCopy fn_KeyCopy
            , ZF_IN Fn_KeyDestroy fn_KeyDestroy
            , ZF_IN Fn_ValueCopy fn_ValueCopy
            , ZF_IN Fn_ValueDestroy fn_ValueDestroy
            )
    : _ZFP_ZFCoreOrderMapPrivate(
            fn_Hash
            , fn_Equal
            , fn_KeyCopy
            , fn_KeyDestroy
            , fn_ValueCopy
            , fn_ValueDestroy
            )
    , m()
    {
    }
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
            , ZF_IN Fn_KeyInfo fn_KeyInfo
            , ZF_IN Fn_ValueInfo fn_ValueInfo
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
                    fn_KeyInfo(ret, this->iterKey(it));
                    ret += token.tokenKeyRight;
                }
                ret += token.tokenPairSeparator;
                {
                    ret += token.tokenValueLeft;
                    fn_ValueInfo(ret, this->iterValue(it));
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
    virtual ZFCompareResult objectCompareValue(
            ZF_IN const _ZFP_ZFCoreOrderMapPrivate *ref
            , ZF_IN Fn_ValueEqual fn_ValueEqual
            ) {
        _ZFP_ZFCoreOrderMapType const &mRef = ((const _ZFP_ZFCoreOrderMapImpl *)ref)->m;
        if(m.size() != mRef.size()) {
            return ZFCompareUncomparable;
        }
        for(_ZFP_ZFCoreOrderMapType::const_iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreOrderMapType::iterator it = m.find(itRef->first);
            if(it == m.end() || !fn_ValueEqual(it->second, itRef->second)) {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareEqual;
    }
    zfoverride
    virtual void copyFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMapPrivate *ref) {
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
    virtual zfbool isContain(ZF_IN const void *key) {
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        return m.find(keyWrap) == m.end();
    }
    zfoverride
    virtual void addFrom(ZF_IN_OUT _ZFP_ZFCoreOrderMapPrivate *ref) {
        _ZFP_ZFCoreOrderMapType &mRef = ((_ZFP_ZFCoreOrderMapImpl *)ref)->m;
        m.reserve(m.size() + mRef.size());
        for(_ZFP_ZFCoreOrderMapType::iterator itRef = mRef.begin(); itRef != mRef.end(); ++itRef) {
            _ZFP_ZFCoreOrderMapType::iterator it = m.find(itRef->first);
            if(it != m.end()) {
                fn_ValueCopy(it->second, itRef->second);
            }
            else {
                _ZFP_ZFCoreOrderMapKey keyWrap;
                keyWrap.owner = this;
                keyWrap.key = fn_KeyCopy(itRef->first.key);
                void *value = fn_ValueCopy(zfnull, itRef->second);
                m.insert(zfstlpair<_ZFP_ZFCoreOrderMapKey, void *>(keyWrap, value));
            }
        }
    }
    zfoverride
    virtual void set(ZF_IN const void *key, ZF_IN const void *value) {
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(keyWrap);
        if(it != m.end()) {
            fn_ValueCopy(it->second, value);
        }
        else {
            _ZFP_ZFCoreOrderMapKey keyWrapNew;
            keyWrapNew.owner = this;
            keyWrapNew.key = fn_KeyCopy(key);
            void *valueNew = fn_ValueCopy(zfnull, value);
            m.insert(zfstlpair<_ZFP_ZFCoreOrderMapKey, void *>(keyWrapNew, valueNew));
        }
    }
    zfoverride
    virtual void *get(ZF_IN const void *key) {
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(keyWrap);
        if(it != m.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual void *access(ZF_IN const void *key) {
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(keyWrap);
        if(it != m.end()) {
            return it->second;
        }
        else {
            _ZFP_ZFCoreOrderMapKey keyWrapNew;
            keyWrapNew.owner = this;
            keyWrapNew.key = fn_KeyCopy(key);
            void *valueNew = fn_ValueCopy(zfnull, zfnull);
            m.insert(zfstlpair<_ZFP_ZFCoreOrderMapKey, void *>(keyWrapNew, valueNew));
            return valueNew;
        }
    }
    zfoverride
    virtual void remove(ZF_IN const void *key) {
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        _ZFP_ZFCoreOrderMapType::iterator it = m.find(keyWrap);
        if(it != m.end()) {
            void *keyTmp = const_cast<void *>(it->first.key);
            void *valueTmp = it->second;
            m.erase(it);
            fn_KeyDestroy(keyTmp);
            fn_ValueDestroy(valueTmp);
        }
    }
    zfoverride
    virtual void removeAll(void) {
        if(!m.empty()) {
            zfstlvector<void *> tmpKey;
            tmpKey.reserve(m.size());
            zfstlvector<void *> tmpValue;
            tmpValue.reserve(m.size());
            for(_ZFP_ZFCoreOrderMapType::iterator it = m.begin(); it != m.end(); ++it) {
                tmpKey.push_back(const_cast<void *>(it->first.key));
                tmpValue.push_back(it->second);
            }
            m.clear();
            for(zfstlsize i = 0; i < tmpKey.size(); ++i) {
                fn_KeyDestroy(tmpKey[i]);
                fn_ValueDestroy(tmpValue[i]);
            }
        }
    }

    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        _ZFP_ZFCoreOrderMapType::iterator it;
        _ZFP_ZFCoreOrderMapType::iterator end;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return it != end;
        }
        zfoverride
        virtual void next(void) {
            ++it;
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->it = it;
            ret->end = end;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return it == t->it;
        }
    };
    zfoverride
    virtual zfiter iter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = m.begin();
        impl->end = m.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfiter iterFind(ZF_IN const void *key) {
        _Iter *impl = zfpoolNew(_Iter);
        _ZFP_ZFCoreOrderMapKey keyWrap;
        keyWrap.owner = this;
        keyWrap.key = key;
        impl->it = m.find(keyWrap);
        impl->end = m.end();
        return zfiter(impl);
    }
    zfoverride
    virtual const void *iterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first.key;
    }
    zfoverride
    virtual void *iterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }
    zfoverride
    virtual void iterValue(ZF_IN_OUT zfiter &it, ZF_IN const void *value) {
        fn_ValueCopy(it.impl<_Iter *>()->it->second, value);
    }
    zfoverride
    virtual void iterRemove(ZF_IN_OUT zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        void *key = const_cast<void *>(impl->it->first.key);
        void *value = impl->it->second;
        m.erase((impl->it)++);
        fn_KeyDestroy(key);
        fn_ValueDestroy(value);
    }

    // ============================================================
    // order map spec
    zfoverride
    virtual void move(
            ZF_IN zfindex from
            , ZF_IN zfindex to
            ) {
        return m.move((zfstlsize)from, (zfstlsize)to);
    }
    zfoverride
    virtual const void *keyAt(ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, (zfindex)m.size());
        return (m.begin() + index)->first.key;
    }
    zfoverride
    virtual void *valueAt(ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, (zfindex)m.size());
        return (m.begin() + index)->second;
    }
    zfoverride
    virtual void removeAt(ZF_IN zfindex index) {
        ZFCoreAssertIndexRange(index, (zfindex)m.size());
        m.erase(m.begin() + index);
    }
    zfoverride
    virtual zfiter iterAt(ZF_IN zfindex index) {
        if(index < (zfindex)m.size()) {
            return m.iterAt((zfstlsize)index);
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual zfindex iterIndex(ZF_IN const zfiter &it) {
        return m.iterIndex(it);
    }
};

// ============================================================
_ZFP_ZFCoreOrderMapPrivate *_ZFP_ZFCoreOrderMapPrivate::create(
        ZF_IN Fn_Hash fn_Hash
        , ZF_IN Fn_Equal fn_Equal
        , ZF_IN Fn_KeyCopy fn_KeyCopy
        , ZF_IN Fn_KeyDestroy fn_KeyDestroy
        , ZF_IN Fn_ValueCopy fn_ValueCopy
        , ZF_IN Fn_ValueDestroy fn_ValueDestroy
        )
{
    return zfpoolNew(_ZFP_ZFCoreOrderMapImpl
            , fn_Hash
            , fn_Equal
            , fn_KeyCopy
            , fn_KeyDestroy
            , fn_ValueCopy
            , fn_ValueDestroy
            );
}
void _ZFP_ZFCoreOrderMapPrivate::destroy(ZF_IN _ZFP_ZFCoreOrderMapPrivate *d) {
    zfpoolDelete((_ZFP_ZFCoreOrderMapImpl *)d);
}

ZF_NAMESPACE_GLOBAL_END

