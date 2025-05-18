#include "zfimport.h"
#include "ZFMap.h"
#include "ZFFile.h"
#include "ZFPathType_res.h"

#include "ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFImportBegin)
ZFEVENT_GLOBAL_REGISTER(ZFImportEnd)
ZF_NAMESPACE_END(ZFGlobalEvent)

typedef zfstlhashmap<zfstring, zfauto> _ZFP_zfimportCacheMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfimportDataHolder, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfimportDataHolder) {
}
public:
    _ZFP_zfimportCacheMapType cacheMap;
ZF_GLOBAL_INITIALIZER_END(zfimportDataHolder)

static zfbool _ZFP_zfimportFile(ZF_OUT zfauto &ret, ZF_IN const ZFInput &input) {
    if(!input) {
        return zffalse;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(zfimportDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder);
    _ZFP_zfimportCacheMapType &cacheMap = d->cacheMap;
    zfstring callbackId = input.callbackId();
    if(callbackId) {
        ZFCoreMutexLocker();
        _ZFP_zfimportCacheMapType::iterator it = cacheMap.find(callbackId);
        if(it != cacheMap.end()) {
            if(it->second == ZFNull()) {
                ret = zfnull;
            }
            else {
                ret = it->second;
            }
            return zftrue;
        }

        // mark loading
        cacheMap[callbackId] = ZFNull();
    }

    zfobj<v_ZFInput> inputHolder;
    inputHolder->zfv = input;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFImportBegin(), inputHolder);
    zfbool success = ZFObjectIOLoadT(ret, input);
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFImportEnd(), inputHolder, ret);

    if(callbackId) {
        ZFCoreMutexLocker();
        if(success) {
            cacheMap[callbackId] = ret;
        }
        else {
            // unmark loading
            cacheMap.erase(callbackId);
        }
    }

    return success;
}
static void _ZFP_zfimportDir(
        ZF_IN_OUT ZFMap *ret
        , ZF_IN const ZFPathInfoImpl &impl
        , ZF_IN const ZFPathInfo &pathInfoRoot
        , ZF_IN const zfchar *path
        ) {
    zfstring pathData = impl.implToChild(pathInfoRoot.pathData(), path);
    if(!pathData) {
        return;
    }
    ZFFileFindData fd;
    if(impl.implFindFirst(fd, pathData)) {
        do {
            zfstring relPath;
            if(!zfstringIsEmpty(path)) {
                relPath += path;
                relPath += "/";
            }
            relPath += fd.name();
            if(fd.isDir()) {
                _ZFP_zfimportDir(ret, impl, pathInfoRoot, relPath);
            }
            else {
                zfauto obj;
                if(_ZFP_zfimportFile(obj, ZFInputForLocal(relPath, pathInfoRoot))) {
                    zfobj<v_zfstring> key;
                    key->zfv = relPath;
                    ret->set(key, obj != zfnull ? obj.toObject() : ZFNull());
                }
            }
        } while(impl.implFindNext(fd));
        impl.implFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfauto, zfimport
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        ) {
    ZFPathInfo abs;
    if(ZFPathInfoFromStringT(abs, path)) {
        if(ZFPathInfoIsDir(abs)) {
            const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(abs.pathType());
            if(impl == zfnull) {
                return zfnull;
            }
            zfobj<ZFMap> ret;
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(abs.pathType(), ""), path);
            return ret;
        }
        else {
            zfauto ret;
            if(_ZFP_zfimportFile(ret, ZFInputForPathInfo(abs))) {
                return ret;
            }
            else {
                return zfnull;
            }
        }
    }
    else if(pathInfo == zfnull) {
        if(ZFResIsDir(path)) {
            const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(ZFPathType_res());
            if(impl == zfnull) {
                return zfnull;
            }
            zfobj<ZFMap> ret;
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(ZFPathType_res(), ""), path);
            return ret;
        }
        else {
            zfauto ret;
            if(_ZFP_zfimportFile(ret, ZFInputForRes(path))) {
                return ret;
            }
            else {
                return zfnull;
            }
        }
    }
    else {
        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo.pathType());
        if(impl == zfnull) {
            return zfnull;
        }
        zfstring pathData;
        if(!impl->implIsDir(pathInfo.pathData())) {
            pathData = impl->implToParent(pathInfo.pathData());
            pathData = impl->implToChild(pathData, path);
        }
        else {
            pathData = impl->implToChild(pathInfo.pathData(), path);
        }
        if(!pathData) {
            return zfnull;
        }
        if(impl->implIsDir(pathData)) {
            zfobj<ZFMap> ret;
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(pathInfo.pathType(), pathData), "");
            return ret;
        }
        else {
            zfauto ret;
            if(_ZFP_zfimportFile(ret, ZFInputForLocal(path, pathInfo))) {
                return ret;
            }
            else {
                return zfnull;
            }
        }
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfauto, zfimportCacheRemove
        , ZFMP_IN(const zfstring &, callbackId)
        ) {
    if(callbackId == zfnull) {
        return zfnull;
    }
    ZFCoreMutexLocker();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    _ZFP_zfimportCacheMapType::iterator it = cacheMap.find(callbackId);
    if(it != cacheMap.end()) {
        zfauto ret = it->second;
        cacheMap.erase(it);
        return ret;
    }
    else {
        return zfnull;
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, zfimportCacheRemoveAll) {
    ZFCoreMutexLock();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(cacheMap.empty()) {
        ZFCoreMutexUnlock();
    }
    else {
        _ZFP_zfimportCacheMapType tmp;
        tmp.swap(cacheMap);
        ZFCoreMutexUnlock();
    }
}

ZF_NAMESPACE_GLOBAL_END

