#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// used to reduce memory usage for static names (class/method/property's name/internalId/...)
typedef zfstlhashmap<zfstlstringZ, zfindex> _ZFP_ZFSigMapType;
static _ZFP_ZFSigMapType &_ZFP_ZFSigMap(void)
{
    static _ZFP_ZFSigMapType m;
    return m;
}
static zfindex &_ZFP_ZFSigIdx(void)
{
    static zfindex d = 0;
    return d;
}

static _ZFP_ZFSigMapType::iterator _ZFP_ZFSigAccess(ZF_IN const zfchar *name)
{
    static _ZFP_ZFSigMapType &m = _ZFP_ZFSigMap();
    static zfindex &idx = _ZFP_ZFSigIdx();
    _ZFP_ZFSigMapType::iterator it = m.find(name);
    if(it != m.end())
    {
        return it;
    }
    else
    {
        ++idx;
        m[name] = idx;
        it = m.find(name);
        return it;
    }
}

zfindex _ZFP_ZFSigForName(ZF_IN const zfchar *name)
{
    return _ZFP_ZFSigAccess(name)->second;
}

const zfchar *_ZFP_ZFSigNameAddr(ZF_IN const zfchar *name)
{
    return _ZFP_ZFSigAccess(name)->first.c_str();
}

ZF_NAMESPACE_GLOBAL_END
