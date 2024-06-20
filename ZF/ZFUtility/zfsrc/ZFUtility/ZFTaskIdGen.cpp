#include "ZFTaskIdGen.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFTaskIdGenPrivate {
public:
    typedef zfstlmap<zfidentity, zfauto> TaskMap;

public:
    zfidentity cur;
    TaskMap m;

public:
    _ZFP_ZFTaskIdGenPrivate(void)
    : cur(0)
    , m()
    {
    }
};

ZFOBJECT_REGISTER(ZFTaskIdGen)

void ZFTaskIdGen::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTaskIdGenPrivate);
}
void ZFTaskIdGen::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFTaskIdGen, zfidentity, attach
        , ZFMP_IN(ZFObject *, taskData)
        ) {
    zfsynchronize(this);
    do {
        ++(d->cur);
        if(d->cur == zfidentityInvalid()) {
            d->cur = 0;
        }
    } while(d->m.find(d->cur) != d->m.end());
    zfidentity taskId = d->cur;
    d->m[taskId] = taskData;
    return taskId;
}
ZFMETHOD_DEFINE_1(ZFTaskIdGen, zfauto, detach
        , ZFMP_IN(zfidentity, taskId)
        ) {
    zfsynchronize(this);
    _ZFP_ZFTaskIdGenPrivate::TaskMap::iterator it = d->m.find(taskId);
    if(it == d->m.end()) {
        return zfnull;
    }
    zfauto ret = it->second;
    d->m.erase(it);
    return ret;
}
ZFMETHOD_DEFINE_0(ZFTaskIdGen, zfautoT<ZFArray>, detachAll) {
    zfsynchronize(this);
    zfobj<ZFArray> ret;
    if(!d->m.empty()) {
        _ZFP_ZFTaskIdGenPrivate::TaskMap m;
        m.swap(d->m);
        for(_ZFP_ZFTaskIdGenPrivate::TaskMap::iterator it = m.begin(); it != m.end(); ++it) {
            ret->add(it->second);
        }
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFTaskIdGen, zfauto, exist
        , ZFMP_IN(zfidentity, taskId)
        ) {
    zfsynchronize(this);
    _ZFP_ZFTaskIdGenPrivate::TaskMap::iterator it = d->m.find(taskId);
    if(it == d->m.end()) {
        return zfnull;
    }
    else {
        return it->second;
    }
}

ZF_NAMESPACE_GLOBAL_END

