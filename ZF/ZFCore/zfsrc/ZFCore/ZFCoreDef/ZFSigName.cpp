#include "ZFSigName.h"
#include "zfstr.h"

#include "../ZFSTLWrapper/zfstlmap.h"
#include "../ZFSTLWrapper/zfstlhashmap.h"
#include "../ZFSTLWrapper/zfstlstring.h"
#include "../ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassPOD _ZFP_ZFSigNamePrivate {
public:
    zfuint refCount;
    zfuint length;
    zfchar *s;
    zfidentity sigId;
};

typedef zfstlhashmap<const zfchar *, _ZFP_ZFSigNamePrivate *, zfcharConst_zfstlHasher, zfcharConst_zfstlHashComparer> _ZFP_ZFSigNameMapType;
typedef zfstlmap<zfidentity, zfbool> _ZFP_ZFSigNameIdMapType;
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
static _ZFP_ZFSigNameCacheType &_ZFP_ZFSigNameCache(void) {
    static _ZFP_ZFSigNameCacheType l;
    return l;
}

static _ZFP_ZFSigNamePrivate *_ZFP_ZFSigNameAttach(ZF_IN const zfchar *s, ZF_IN_OPT zfindex len = zfindexMax()) {
    if(zfstringIsEmpty(s, len)) {
        return zfnull;
    }
    _ZFP_ZFSigNameCache();
    _ZFP_ZFSigNameMapType::iterator it = _ZFP_ZFSigNameMap().find(
            len == zfindexMax()
            ? s
            : zfstring(s, len).cString()
            );
    if(it != _ZFP_ZFSigNameMap().end()) {
        _ZFP_ZFSigNamePrivate *d = it->second;
        ++(d->refCount);
        return d;
    }
    else {
        _ZFP_ZFSigNamePrivate *d = zfnew(_ZFP_ZFSigNamePrivate);
        d->refCount = 1;
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
        d->s = zfsCopy(s, len);
        d->length = (zfuint)(len != zfindexMax() ? len : zfslen(d->s));
        _ZFP_ZFSigNameMap()[d->s] = d;
        _ZFP_ZFSigNameIdMap()[d->sigId] = zftrue;
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
                _ZFP_ZFSigNameIdMap().erase(d->sigId);
                _ZFP_ZFSigNameIdUnusedMap()[d->sigId] = zftrue;
                zffree(d->s);
                zfdelete(d);
            }
        }
    }
}

// ============================================================
ZFSigName::ZFSigName(void) : d(zfnull) {
}
ZFSigName::ZFSigName(ZF_IN const ZFSigName &ref) {
    zfCoreMutexLocker();
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
}
ZFSigName::ZFSigName(ZF_IN const zfstring &s) {
    zfCoreMutexLocker();
    d = _ZFP_ZFSigNameAttach(s);
}
ZFSigName::ZFSigName(ZF_IN const zfchar *s, ZF_IN_OPT zfindex len /* = zfindexMax() */) {
    zfCoreMutexLocker();
    d = _ZFP_ZFSigNameAttach(s, len);
}
ZFSigName::~ZFSigName(void) {
    if(d) {
        zfCoreMutexLocker();
        _ZFP_ZFSigNameDetach(d);
    }
}

zfidentity ZFSigName::sigId(void) const {
    return d ? d->sigId : zfidentityInvalid();
}

zfbool ZFSigName::isEmpty(void) const {
    return d == zfnull;
}

const zfchar *ZFSigName::cString(void) const {
    return d ? d->s : "";
}

zfindex ZFSigName::length(void) const {
    return d ? d->length : 0;
}

zfint ZFSigName::compare(ZF_IN const ZFSigName &ref) const {
    if(d == ref.d) {
        return 0;
    }
    else {
        return zfscmp(this->cString(), ref.cString());
    }
}

ZFSigName &ZFSigName::operator = (ZF_IN const ZFSigName &ref) {
    zfCoreMutexLocker();
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
    zfCoreMutexLocker();
    _ZFP_ZFSigNamePrivate *dTmp = d;
    d = _ZFP_ZFSigNameAttach(s);
    if(dTmp) {
        _ZFP_ZFSigNameDetach(dTmp);
    }
    return *this;
}
ZFSigName &ZFSigName::operator = (ZF_IN const zfchar *s) {
    zfCoreMutexLocker();
    _ZFP_ZFSigNamePrivate *dTmp = d;
    d = _ZFP_ZFSigNameAttach(s);
    if(dTmp) {
        _ZFP_ZFSigNameDetach(dTmp);
    }
    return *this;
}

// ============================================================
void ZFSigNameInfo(ZF_OUT zfstring &ret) {
    zfCoreMutexLocker();
    _ZFP_ZFSigNameMapType &m = _ZFP_ZFSigNameMap();
    for(_ZFP_ZFSigNameMapType::iterator it = m.begin(); it != m.end(); ++it) {
        ret += it->first;
        ret += " : ";
        zfsFromIntT(ret, it->second->refCount);
        ret += "\n";
    }
}

ZF_NAMESPACE_GLOBAL_END

