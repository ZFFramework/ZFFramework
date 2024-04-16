#include "../ZFCoreDef.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
const zfchar *ZFNamespaceSkipGlobal(ZF_IN const zfchar *ns) {
    static zfstring namePrefix(ZF_NAMESPACE_GLOBAL_NAME);
    static zfstring abbrNamePrefix(ZF_NAMESPACE_GLOBAL_ABBR_NAME);
    if(zfstringIsEmpty(ns)) {
        return zfnull;
    }
    else if(zfsncmp(ns, ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0) {
        if(ns[ZFNamespaceSeparatorLen()] == '\0') {
            return zfnull;
        }
        else {
            return ns + ZFNamespaceSeparatorLen();
        }
    }
    else if(zfsncmp(ns, namePrefix, namePrefix.length()) == 0) {
        if(ns[namePrefix.length()] == '\0') {
            return zfnull;
        }
        else if(zfsncmp(ns + namePrefix.length(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0) {
            if(ns[namePrefix.length() + ZFNamespaceSeparatorLen()] == '\0') {
                return zfnull;
            }
            else {
                return ns + namePrefix.length() + ZFNamespaceSeparatorLen();
            }
        }
        else {
            return ns;
        }
    }
    else if(zfsncmp(ns, abbrNamePrefix, abbrNamePrefix.length()) == 0) {
        if(ns[abbrNamePrefix.length()] == '\0') {
            return zfnull;
        }
        else if(zfsncmp(ns + abbrNamePrefix.length(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0) {
            if(ns[abbrNamePrefix.length() + ZFNamespaceSeparatorLen()] == '\0') {
                return zfnull;
            }
            else {
                return ns + abbrNamePrefix.length() + ZFNamespaceSeparatorLen();
            }
        }
        else {
            return ns;
        }
    }
    else {
        return ns;
    }
}

zfbool ZFNamespaceSplit(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        ) {
    if(src == zfnull) {
        ret.add(ZFIndexRangeZero());
        return zftrue;
    }
    const zfchar *pL = src;
    const zfchar *p = src;
    const zfchar *pEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen);
    zfbool hasAdd = zffalse;
    while(p < pEnd) {
        if(zfsncmp(p, ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0) {
            if(p > pL) {
                ret.add(ZFIndexRangeMake(pL - src, p - pL));
            }
            p = pL = p + ZFNamespaceSeparatorLen();
            hasAdd = zftrue;
            continue;
        }
        else {
            ++p;
        }
    }
    if(p > pL) {
        ret.add(ZFIndexRangeMake(pL - src, p - pL));
    }
    else {
        if(!hasAdd) {
            ret.add(ZFIndexRangeZero());
        }
    }
    return zftrue;
}

// ============================================================
zfclassLikePOD _ZFP_ZFNamespaceMapType {
public:
    zfuint refCount;
    zfstring ns;
    zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> > d;
public:
    _ZFP_ZFNamespaceMapType(void)
        : refCount(1)
          , ns()
          , d()
    {
    }
};

static _ZFP_ZFNamespaceMapType &_ZFP_ZFNamespaceMap(void) {
    static _ZFP_ZFNamespaceMapType d;
    return d;
}
zfstring _ZFP_ZFNamespaceRegister(
        ZF_IN const zfchar *parent
        , ZF_IN const zfchar *child
        ) {
    if(child == zfnull) {
        return "";
    }
    zfCoreMutexLocker();
    zfstring ns;
    if(parent != zfnull) {
        ns += parent;
        ns += ZFNamespaceSeparator();
    }
    ns += child;
    ZFCoreArrayPOD<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, ns, ns.length());
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfstring key(ns + pos[i].start, pos[i].count);
        zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
        if(it == t->d.end()) {
            _ZFP_ZFNamespaceMapType *tNew = zfnew(_ZFP_ZFNamespaceMapType);
            t->d[key] = ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *>(tNew);
            t = tNew;
            t->ns.append(ns, pos[i].start + pos[i].count);
        }
        else {
            t = it->second.pointerValue();
            (++t->refCount);
        }
    }
    return ns;
}
void _ZFP_ZFNamespaceUnregister(ZF_IN const zfchar *ns) {
    if(ns == zfnull) {
        return;
    }
    zfCoreMutexLocker();
    zfstring nsTmp = ns;
    ZFCoreArrayPOD<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, nsTmp, nsTmp.length());
    if(pos.count() < 1) {
        return;
    }
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfstring key(nsTmp + pos[i].start, pos[i].count);
        zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
        if(it == t->d.end()) {
            t = zfnull;
            break;
        }
        else {
            --(it->second->refCount);
            if(it->second->refCount == 0) {
                t->d.erase(it);
                t = zfnull;
                break;
            }
            t = it->second.pointerValue();
        }
    }
}

void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    zfCoreMutexLocker();
    ZFCoreQueuePOD<_ZFP_ZFNamespaceMapType *> toCheck;
    toCheck.add(&_ZFP_ZFNamespaceMap());
    do {
        _ZFP_ZFNamespaceMapType *t = toCheck.take();
        if(!t->ns.isEmpty()) {
            ret.add(t->ns);
        }
        for(zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it) {
            toCheck.add(it->second.pointerValue());
        }
    } while(!toCheck.isEmpty());
}

void ZFNamespaceGetAllT(
        ZF_IN_OUT ZFCoreArray<zfstring> &ret
        , ZF_IN const zfchar *parent
        , ZF_IN_OPT zfbool recursive /* = zffalse */
        ) {
    zfCoreMutexLocker();
    parent = ZFNamespaceSkipGlobal(parent);
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    if(parent != zfnull) {
        ZFCoreArrayPOD<ZFIndexRange> pos;
        ZFNamespaceSplit(pos, parent);
        for(zfindex i = 0; i < pos.count(); ++i) {
            zfstring key(parent + pos[i].start, pos[i].count);
            zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
            if(it == t->d.end()) {
                return;
            }
            else {
                t = it->second.pointerValue();
            }
        }
    }

    ZFCoreQueuePOD<_ZFP_ZFNamespaceMapType *> toCheck;
    for(zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it) {
        toCheck.add(it->second.pointerValue());
    }
    do {
        _ZFP_ZFNamespaceMapType *t = toCheck.take();
        if(!t->ns.isEmpty()) {
            ret.add(t->ns);
        }
        if(recursive) {
            for(zfstlmap<zfstring, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it) {
                toCheck.add(it->second.pointerValue());
            }
        }
    } while(!toCheck.isEmpty());
}

ZF_NAMESPACE_GLOBAL_END

