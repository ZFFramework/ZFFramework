#include "ZFCoreTypeDef.h"
#include "ZFCoreStringUtil.h"
#include "ZFCoreMutex.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlhashmap<const zfchar *, _zfstrD_RO<zfchar> *, zfcharConst_zfstlHasher, zfcharConst_zfstlHashComparer> _ZFP_zfstrRO_Map;

static _ZFP_zfstrRO_Map &_ZFP_zfstrRO_map(void)
{
    static _ZFP_zfstrRO_Map m;
    return m;
}

_zfstrD_RO<zfchar> *_ZFP_zfstrRO_attach(ZF_IN const zfchar *src)
{
    if(src == zfnull)
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    _ZFP_zfstrRO_Map &m = _ZFP_zfstrRO_map();
    _ZFP_zfstrRO_Map::iterator it = m.find(src);
    if(it != m.end())
    {
        ++(it->second->refCount);
        return it->second;
    }

    _zfstrD_RO<zfchar> *RO = zfnew(_zfstrD_RO<zfchar>);
    RO->refCount = 1;
    RO->length = (zfuint)zfslen(src);
    RO->s = zfsCopy(src, RO->length);
    m[RO->s] = RO;
    return RO;
}
void _ZFP_zfstrRO_detach(ZF_IN const zfchar *src)
{
    if(src == zfnull)
    {
        return;
    }
    zfCoreMutexLocker();
    _ZFP_zfstrRO_Map &m = _ZFP_zfstrRO_map();
    _ZFP_zfstrRO_Map::iterator it = m.find(src);
    --(it->second->refCount);
    if(it->second->refCount == 0)
    {
        _zfstrD_RO<zfchar> *RO = it->second;
        m.erase(it);
        zffree(RO->s);
        zfdelete(RO);
    }
}

ZF_NAMESPACE_GLOBAL_END

