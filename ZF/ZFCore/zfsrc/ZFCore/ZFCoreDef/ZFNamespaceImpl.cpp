#include "../ZFCoreDef.h"

_ZFP_ZFNamespaceHolder::_ZFP_ZFNamespaceHolder(const char *parent, const char *child)
: _ns(_ZFP_ZFNamespaceRegister(parent, child))
{
}
_ZFP_ZFNamespaceHolder::~_ZFP_ZFNamespaceHolder(void) {
    _ZFP_ZFNamespaceUnregister(_ns);
}

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
const zfchar *ZFNamespaceSkipGlobal(ZF_IN const zfchar *ns) {
    static zfstring namePrefix = zftext(ZF_NAMESPACE_GLOBAL_NAME);
    static zfstring abbrNamePrefix = zftext(ZF_NAMESPACE_GLOBAL_ABBR_NAME);
    if(zfstringIsEmpty(ns)) {
        return zfnull;
    }
    else if(zfstringBeginWith(ns, namePrefix)) {
        if(ns[namePrefix.length()] == '\0') {
            return zfnull;
        }
        else if(ns[namePrefix.length()] == '.') {
            if(ns[namePrefix.length() + 1] == '\0') {
                return zfnull;
            }
            else {
                return ns + namePrefix.length() + 1;
            }
        }
        else {
            return ns;
        }
    }
    else if(zfstringBeginWith(ns, abbrNamePrefix)) {
        if(ns[abbrNamePrefix.length()] == '\0') {
            return zfnull;
        }
        else if(ns[abbrNamePrefix.length()] == '.') {
            if(ns[abbrNamePrefix.length() + 1] == '\0') {
                return zfnull;
            }
            else {
                return ns + abbrNamePrefix.length() + 1;
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
zfstring ZFNamespaceSkipGlobal(ZF_IN const zfstring &ns) {
    const zfchar *t = ZFNamespaceSkipGlobal(ns.cString());
    if(t == zfnull) {
        return zfnull;
    }
    else if(t == ns.cString()) {
        return ns;
    }
    else {
        return t;
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
        if(*p == '.') {
            if(p > pL) {
                ret.add(ZFIndexRangeCreate(pL - src, p - pL));
            }
            p = pL = p + 1;
            hasAdd = zftrue;
            continue;
        }
        else {
            ++p;
        }
    }
    if(p > pL) {
        ret.add(ZFIndexRangeCreate(pL - src, p - pL));
    }
    else {
        if(!hasAdd) {
            ret.add(ZFIndexRangeZero());
        }
    }
    return zftrue;
}

// ============================================================
zfclassLikePOD _ZFP_ZFNamespaceTreeType {
public:
    typedef ZFCoreMap<zfstring, _ZFP_ZFNamespaceTreeType> MapType;
public:
    zfuint refCount;
    zfstring ns;
    zfstring nsFull;
    MapType d;
public:
    _ZFP_ZFNamespaceTreeType(void)
    : refCount(1)
    , ns()
    , d()
    {
    }
};

static _ZFP_ZFNamespaceTreeType &_ZFP_ZFNamespaceTree(void) {
    static _ZFP_ZFNamespaceTreeType d;
    return d;
}
zfstring _ZFP_ZFNamespaceRegister(
        ZF_IN const zfchar *parent
        , ZF_IN const zfchar *child
        ) {
    if(child == zfnull) {
        return "";
    }
    ZFCoreMutexLocker();
    zfstring ns;
    parent = ZFNamespaceSkipGlobal(parent);
    if(parent != zfnull) {
        ns += parent;
        ns += ".";
    }
    ns += child;
    ZFCoreArray<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, ns, ns.length());
    _ZFP_ZFNamespaceTreeType *t = &_ZFP_ZFNamespaceTree();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfstring key(ns + pos[i].start, pos[i].count);
        zfiter it = t->d.iterFind(key);
        if(it) {
            t = &(t->d.iterValue(it));
            ++(t->refCount);
        }
        else {
            t = &(t->d.access(key));
            t->ns.append(ns, pos[i].start + pos[i].count);
            t->nsFull = ns;
        }
    }
    return t->nsFull;
}
void _ZFP_ZFNamespaceUnregister(ZF_IN const zfchar *ns) {
    if(ns == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    zfstring nsTmp = ns;
    ZFCoreArray<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, nsTmp, nsTmp.length());
    if(pos.count() < 1) {
        return;
    }
    _ZFP_ZFNamespaceTreeType *t = &_ZFP_ZFNamespaceTree();
    for(zfindex i = 0; i < pos.count(); ++i) {
        zfstring key(nsTmp + pos[i].start, pos[i].count);
        zfiter it = t->d.iterFind(key);
        if(it) {
            if(--(t->d.iterValue(it).refCount) == 0) {
                t->d.iterRemove(it);
                t = zfnull;
                break;
            }
            t = &(t->d.iterValue(it));
        }
        else {
            t = zfnull;
            break;
        }
    }
}

void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret) {
    ZFCoreMutexLocker();
    ZFCoreQueuePOD<_ZFP_ZFNamespaceTreeType *> toCheck;
    toCheck.add(&_ZFP_ZFNamespaceTree());
    do {
        _ZFP_ZFNamespaceTreeType *t = toCheck.take();
        if(!t->ns.isEmpty()) {
            ret.add(t->ns);
        }
        for(zfiter it = t->d.iter(); it; ++it) {
            toCheck.add(&(t->d.iterValue(it)));
        }
    } while(!toCheck.isEmpty());
}

void ZFNamespaceGetAllT(
        ZF_IN_OUT ZFCoreArray<zfstring> &ret
        , ZF_IN const zfchar *parent
        , ZF_IN_OPT zfbool recursive /* = zffalse */
        ) {
    ZFCoreMutexLocker();
    parent = ZFNamespaceSkipGlobal(parent);
    _ZFP_ZFNamespaceTreeType *t = &_ZFP_ZFNamespaceTree();
    if(parent != zfnull) {
        ZFCoreArray<ZFIndexRange> pos;
        ZFNamespaceSplit(pos, parent);
        for(zfindex i = 0; i < pos.count(); ++i) {
            zfstring key(parent + pos[i].start, pos[i].count);
            zfiter it = t->d.iterFind(key);
            if(it) {
                t = &(t->d.iterValue(it));
            }
            else {
                return;
            }
        }
    }

    ZFCoreQueuePOD<_ZFP_ZFNamespaceTreeType *> toCheck;
    for(zfiter it = t->d.iter(); it; ++it) {
        toCheck.add(&(t->d.iterValue(it)));
    }
    do {
        _ZFP_ZFNamespaceTreeType *t = toCheck.take();
        if(!t->ns.isEmpty()) {
            ret.add(t->ns);
        }
        if(recursive) {
            for(zfiter it = t->d.iter(); it; ++it) {
                toCheck.add(&(t->d.iterValue(it)));
            }
        }
    } while(!toCheck.isEmpty());
}

ZF_NAMESPACE_GLOBAL_END

