#include "ZFSigName.h"
#include "zfstr.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"
#include "../ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFSigNamePrivate {
public:
    zfuint refCount;
    zfstring s;
    zfidentity sigId;
};

typedef zfstlhashmap<const zfchar *, _ZFP_ZFSigNamePrivate *, zfcharConst_zfstlHash, zfcharConst_zfstlEqual> _ZFP_ZFSigNameMapType;
typedef zfstlhashmap<zfidentity, _ZFP_ZFSigNamePrivate *, zfpod_zfstlHash<zfidentity>, zfpod_zfstlEqual<zfidentity> > _ZFP_ZFSigNameIdMapType;
typedef zfstldeque<_ZFP_ZFSigNamePrivate *> _ZFP_ZFSigNameCacheType;

static zfidentity &_ZFP_ZFSigNameId(void) {
    static zfidentity d = zfidentityInvalid();
    return d;
}
static _ZFP_ZFSigNameMapType &_ZFP_ZFSigNameMap(void) {
    static _ZFP_ZFSigNameMapType m;
    return m;
}
static _ZFP_ZFSigNameIdMapType &_ZFP_ZFSigNameIdMap(void) {
    static _ZFP_ZFSigNameIdMapType m;
    return m;
}
static _ZFP_ZFSigNameIdMapType &_ZFP_ZFSigNameIdUnusedMap(void) {
    static _ZFP_ZFSigNameIdMapType m;
    return m;
}
// delay delete, to prevent sigId increase for creating tmp ZFSigName
static _ZFP_ZFSigNameCacheType &_ZFP_ZFSigNameCache(void) {
    static _ZFP_ZFSigNameCacheType l;
    return l;
}

static void _ZFP_ZFSigIdAttach(ZF_IN _ZFP_ZFSigNamePrivate *d) {
    if(_ZFP_ZFSigNameIdUnusedMap().empty()) {
        do {
            ++_ZFP_ZFSigNameId();
            if(_ZFP_ZFSigNameId() == zfidentityInvalid()) {
                ++_ZFP_ZFSigNameId();
            }
        } while(_ZFP_ZFSigNameIdMap().find(_ZFP_ZFSigNameId()) != _ZFP_ZFSigNameIdMap().end());
        d->sigId = _ZFP_ZFSigNameId();
    }
    else {
        d->sigId = _ZFP_ZFSigNameIdUnusedMap().begin()->first;
        _ZFP_ZFSigNameIdUnusedMap().erase(_ZFP_ZFSigNameIdUnusedMap().begin());
    }
    _ZFP_ZFSigNameIdMap()[d->sigId] = d;
}
static _ZFP_ZFSigNamePrivate *_ZFP_ZFSigNameAttach(ZF_IN const zfstring &s) {
    if(s == zfnull) {
        return zfnull;
    }

    // ensure init order
    static zfbool dummy = (
            (void)_ZFP_ZFSigNameCache(),
            (void)_ZFP_ZFSigNameIdMap(),
            (void)_ZFP_ZFSigNameIdUnusedMap(),
            zffalse
            );
    ZFUNUSED(dummy);

    _ZFP_ZFSigNameMapType::iterator it = _ZFP_ZFSigNameMap().find(s);
    if(it != _ZFP_ZFSigNameMap().end()) {
        _ZFP_ZFSigNamePrivate *d = it->second;
        ++(d->refCount);
        return d;
    }
    else {
        _ZFP_ZFSigNamePrivate *d = zfnew(_ZFP_ZFSigNamePrivate);
        d->refCount = 1;
        d->sigId = zfidentityInvalid();
        d->s = s;
        _ZFP_ZFSigNameMap()[d->s] = d;
        return d;
    }
}
static void _ZFP_ZFSigNameDetach(ZF_IN _ZFP_ZFSigNamePrivate *d) {
    --(d->refCount);
    if(d->refCount == 0) {
        _ZFP_ZFSigNameCacheType &cache = _ZFP_ZFSigNameCache();
        ++(d->refCount);
        cache.push_back(d);
        if(cache.size() >= 32) {
            d = cache[0];
            cache.pop_front();

            --(d->refCount);
            if(d->refCount == 0) {
                _ZFP_ZFSigNameMap().erase(d->s);
                if(d->sigId != zfidentityInvalid()) {
                    _ZFP_ZFSigNameIdMap().erase(d->sigId);
                    _ZFP_ZFSigNameIdUnusedMap()[d->sigId] = zfnull;
                }
                zfdelete(d);
            }
        }
    }
}

// ============================================================
ZFSigName::ZFSigName(void) : d(zfnull) {
}
ZFSigName::ZFSigName(ZF_IN const ZFSigName &ref) {
    ZFCoreMutexLocker();
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
}
ZFSigName::ZFSigName(ZF_IN const zfstring &s) {
    ZFCoreMutexLocker();
    d = _ZFP_ZFSigNameAttach(s);
}
ZFSigName::ZFSigName(ZF_IN const zfchar *s) {
    ZFCoreMutexLocker();
    d = _ZFP_ZFSigNameAttach(s);
}
ZFSigName::ZFSigName(ZF_IN const zfnullT &dummy) : d(zfnull) {
}
ZFSigName::~ZFSigName(void) {
    if(d) {
        ZFCoreMutexLocker();
        _ZFP_ZFSigNameDetach(d);
    }
}

zfidentity ZFSigName::sigId(void) const {
    if(d) {
        if(d->sigId == zfidentityInvalid()) {
            ZFCoreMutexLocker();
            _ZFP_ZFSigIdAttach(d);
        }
        return d->sigId;
    }
    else {
        return zfidentityInvalid();
    }
}

zfbool ZFSigName::isEmpty(void) const {
    return d == zfnull;
}

const zfchar *ZFSigName::cString(void) const {
    return d ? d->s.cString() : "";
}

zfindex ZFSigName::length(void) const {
    return d ? d->s.length() : 0;
}

zfint ZFSigName::compare(ZF_IN const ZFSigName &ref) const {
    if(d == ref.d) {
        return 0;
    }
    else {
        return zfscmp(this->cString(), ref.cString());
    }
}

ZFSigName::operator const zfstring & (void) const {
    return d ? d->s : zfstring::Empty();
}

ZFSigName &ZFSigName::operator = (ZF_IN const ZFSigName &ref) {
    ZFCoreMutexLocker();
    if(d != ref.d) {
        _ZFP_ZFSigNamePrivate *dTmp = d;
        d = ref.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp) {
            _ZFP_ZFSigNameDetach(dTmp);
        }
    }
    return *this;
}
ZFSigName &ZFSigName::operator = (ZF_IN const zfstring &s) {
    ZFCoreMutexLocker();
    _ZFP_ZFSigNamePrivate *dTmp = d;
    d = _ZFP_ZFSigNameAttach(s);
    if(dTmp) {
        _ZFP_ZFSigNameDetach(dTmp);
    }
    return *this;
}
ZFSigName &ZFSigName::operator = (ZF_IN const zfchar *s) {
    ZFCoreMutexLocker();
    _ZFP_ZFSigNamePrivate *dTmp = d;
    d = _ZFP_ZFSigNameAttach(s);
    if(dTmp) {
        _ZFP_ZFSigNameDetach(dTmp);
    }
    return *this;
}

// ============================================================
void ZFSigNameInfoT(ZF_OUT zfstring &ret) {
    ZFCoreMutexLocker();
    _ZFP_ZFSigNameMapType &m = _ZFP_ZFSigNameMap();
    for(_ZFP_ZFSigNameMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret += it->first;
        ret += " : ";
        zfsFromIntT(ret, it->second->refCount);
        ret += "\n";
    }
}
zfstring ZFSigNameInfo(void) {
    zfstring ret;
    ZFSigNameInfoT(ret);
    return ret;
}
zfidentity ZFSigNameToId(ZF_IN const zfstring &name) {
    ZFCoreMutexLocker();
    _ZFP_ZFSigNameMapType &m = _ZFP_ZFSigNameMap();
    _ZFP_ZFSigNameMapType::iterator it = m.find(name);
    if(it != m.end()) {
        return it->second->sigId;
    }
    else {
        return zfidentityInvalid();
    }
}
zfstring ZFSigNameFromId(ZF_IN zfidentity sigId) {
    ZFCoreMutexLocker();
    _ZFP_ZFSigNameIdMapType &m = _ZFP_ZFSigNameIdMap();
    _ZFP_ZFSigNameIdMapType::iterator it = m.find(sigId);
    if(it != m.end()) {
        return it->second->s;
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

