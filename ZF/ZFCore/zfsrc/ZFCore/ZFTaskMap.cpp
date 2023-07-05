#include "ZFTaskMap.h"
#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFTaskMapPrivate
{
public:
    typedef zfstlmap<zfidentity, zfautoObject> TaskMap;

public:
    zfidentity cur;
    TaskMap m;

public:
    _ZFP_ZFTaskMapPrivate(void)
    : cur(0)
    , m()
    {
    }
};

ZFOBJECT_REGISTER(ZFTaskMap)

void ZFTaskMap::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTaskMapPrivate);
}
void ZFTaskMap::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFTaskMap, zfidentity, attach,
                  ZFMP_IN(ZFObject *, taskData))
{
    zfsynchronize(this);
    do {
        ++(d->cur);
        if(d->cur == zfidentityInvalid())
        {
            d->cur = 0;
        }
    } while(d->m.find(d->cur) != d->m.end());
    zfidentity taskId = d->cur;
    d->m[taskId] = taskData;
    return taskId;
}
ZFMETHOD_DEFINE_1(ZFTaskMap, zfautoObject, detach,
                  ZFMP_IN(zfidentity, taskId))
{
    zfsynchronize(this);
    _ZFP_ZFTaskMapPrivate::TaskMap::iterator it = d->m.find(taskId);
    if(it == d->m.end())
    {
        return zfnull;
    }
    zfautoObject ret = it->second;
    d->m.erase(it);
    return ret;
}
ZFMETHOD_DEFINE_0(ZFTaskMap, zfautoObjectT<ZFArray *>, detachAll)
{
    zfsynchronize(this);
    zfblockedAlloc(ZFArray, ret);
    if(!d->m.empty())
    {
        _ZFP_ZFTaskMapPrivate::TaskMap m;
        m.swap(d->m);
        for(_ZFP_ZFTaskMapPrivate::TaskMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            ret->add(it->second);
        }
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFTaskMap, zfautoObject, exist,
                  ZFMP_IN(zfidentity, taskId))
{
    zfsynchronize(this);
    _ZFP_ZFTaskMapPrivate::TaskMap::iterator it = d->m.find(taskId);
    if(it == d->m.end())
    {
        return zfnull;
    }
    else
    {
        return it->second;
    }
}

ZF_NAMESPACE_GLOBAL_END

