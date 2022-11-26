#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlhashmap<zfstlstringZ, zfindex> _ZFP_ZFSigMapType;
zfindex _ZFP_ZFSigForName(ZF_IN const zfchar *name)
{
    static _ZFP_ZFSigMapType m;
    static zfindex idx = 0;
    _ZFP_ZFSigMapType::iterator it = m.find(name);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return (m[name] = ++idx);
    }
}

ZF_NAMESPACE_GLOBAL_END

