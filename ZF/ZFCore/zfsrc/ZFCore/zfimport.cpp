#include "zfimport.h"
#include "ZFMap.h"
#include "ZFFile.h"
#include "ZFPathType_res.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfstlmap<zfstring, zfautoObject> _ZFP_zfimportCacheMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfimportDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfimportDataHolder)
{
}
public:
    _ZFP_zfimportCacheMapType cacheMap;
ZF_GLOBAL_INITIALIZER_END(zfimportDataHolder)

static zfautoObject _ZFP_zfimportFile(ZF_IN const ZFInput &input)
{
    if(!input)
    {
        return zfnull;
    }
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(input.callbackId() != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_zfimportCacheMapType::iterator it = cacheMap.find(input.callbackId());
        if(it != cacheMap.end())
        {
            return it->second;
        }
    }

    zfautoObject ret = ZFObjectIOLoad(input);
    if(ret == zfnull)
    {
        return zfnull;
    }

    if(input.callbackId() != zfnull)
    {
        zfCoreMutexLocker();
        cacheMap[input.callbackId()] = ret;
    }

    return ret;
}
static void _ZFP_zfimportDir(ZF_IN_OUT ZFMap *ret,
                             ZF_IN const ZFPathInfoImpl &impl,
                             ZF_IN const ZFPathInfo &pathInfoRoot,
                             ZF_IN const zfchar *path)
{
    zfstring pathData;
    if(!impl.callbackToChild(pathInfoRoot.pathData, pathData, path))
    {
        return;
    }
    ZFFileFindData fd;
    if(impl.callbackFindFirst(fd, pathData))
    {
        do {
            zfstring relPath;
            zfstringAppend(relPath, "%s/%s", path, fd.fileName());
            if(fd.fileIsDir())
            {
                _ZFP_zfimportDir(ret, impl, pathInfoRoot, relPath);
            }
            else
            {
                zfautoObject obj = _ZFP_zfimportFile(ZFInputForLocal(relPath, pathInfoRoot));
                if(obj != zfnull)
                {
                    zfblockedAlloc(v_zfstring, key);
                    key->zfv = relPath;
                    ret->set(key, obj);
                }
            }
        } while(impl.callbackFindNext(fd));
        impl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfautoObject, zfimport,
                       ZFMP_IN(const zfchar *, path),
                       ZFMP_IN_OPT(const ZFPathInfo *, pathInfo, zfnull))
{
    zfstring pathFormated;
    if(!ZFPathFormat(pathFormated, path))
    {
        return zfnull;
    }

    if(pathInfo == zfnull || pathInfo->isEmpty())
    {
        if(ZFResIsDir(pathFormated))
        {
            const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(ZFPathType_res());
            if(impl == zfnull)
            {
                return zfnull;
            }
            zfblockedAlloc(ZFMap, ret);
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(ZFPathType_res(), ""), pathFormated);
            return ret;
        }
        else
        {
            return _ZFP_zfimportFile(ZFInputForRes(pathFormated));
        }
    }
    else
    {
        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo->pathType);
        if(impl == zfnull)
        {
            return zfnull;
        }
        zfstring pathData;
        if(!impl->callbackToChild(pathInfo->pathData, pathData, pathFormated))
        {
            return zfnull;
        }
        if(impl->callbackIsDir(pathData))
        {
            zfblockedAlloc(ZFMap, ret);
            _ZFP_zfimportDir(ret, *impl, *pathInfo, pathFormated);
            return ret;
        }
        else
        {
            return _ZFP_zfimportFile(ZFInputForLocal(pathFormated, pathInfo));
        }
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfautoObject, zfimportCacheRemove,
                       ZFMP_IN(const zfchar *, callbackId))
{
    if(callbackId == zfnull)
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    _ZFP_zfimportCacheMapType::iterator it = cacheMap.find(callbackId);
    if(it != cacheMap.end())
    {
        zfautoObject ret = it->second;
        cacheMap.erase(it);
        return ret;
    }
    else
    {
        return zfnull;
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, zfimportCacheRemoveAll)
{
    zfCoreMutexLock();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(cacheMap.empty())
    {
        zfCoreMutexUnlock();
    }
    else
    {
        _ZFP_zfimportCacheMapType tmp;
        tmp.swap(cacheMap);
        zfCoreMutexUnlock();
    }
}

ZF_NAMESPACE_GLOBAL_END

