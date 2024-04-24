#include "zfimport.h"
#include "ZFMap.h"
#include "ZFFile.h"
#include "ZFPathType_res.h"

#include "ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ImportBegin)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ImportEnd)
ZF_NAMESPACE_END(ZFGlobalEvent)

typedef zfstlmap<zfstring, zfauto> _ZFP_zfimportCacheMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfimportDataHolder, ZFLevelZFFrameworkNormal) {
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
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(input.callbackId() != zfnull) {
        zfCoreMutexLocker();
        _ZFP_zfimportCacheMapType::iterator it = cacheMap.find(input.callbackId());
        if(it != cacheMap.end()) {
            ret = it->second;
            return zftrue;
        }
    }

    zfobj<v_ZFInput> inputHolder;
    inputHolder->zfv = input;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventImportBegin(), inputHolder);
    zfbool success = ZFObjectIOLoadT(ret, input);
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventImportEnd(), inputHolder, ret);

    if(success && input.callbackId() != zfnull) {
        zfCoreMutexLocker();
        cacheMap[input.callbackId()] = ret;
    }

    return success;
}
static void _ZFP_zfimportDir(
        ZF_IN_OUT ZFMap *ret
        , ZF_IN const ZFPathInfoImpl &impl
        , ZF_IN const ZFPathInfo &pathInfoRoot
        , ZF_IN const zfchar *path
        ) {
    zfstring pathData;
    if(!impl.callbackToChild(pathInfoRoot.pathData, pathData, path)) {
        return;
    }
    ZFFileFindData fd;
    if(impl.callbackFindFirst(fd, pathData)) {
        do {
            zfstring relPath;
            zfstringAppend(relPath, "%s/%s", path, fd.fileName());
            if(fd.fileIsDir()) {
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
        } while(impl.callbackFindNext(fd));
        impl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfauto, zfimport
        , ZFMP_IN(const zfchar *, path)
        , ZFMP_IN_OPT(const ZFPathInfo *, pathInfo, zfnull)
        ) {
    zfstring pathFormated;
    if(!ZFPathFormat(pathFormated, path)) {
        return zfnull;
    }

    if(pathInfo == zfnull || pathInfo->isEmpty()) {
        if(ZFResIsDir(pathFormated)) {
            const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(ZFPathType_res());
            if(impl == zfnull) {
                return zfnull;
            }
            zfobj<ZFMap> ret;
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(ZFPathType_res(), ""), pathFormated);
            return ret;
        }
        else {
            zfauto ret;
            if(_ZFP_zfimportFile(ret, ZFInputForRes(pathFormated))) {
                return ret;
            }
            else {
                return zfnull;
            }
        }
    }
    else {
        const ZFPathInfoImpl *impl = ZFPathInfoImplForPathType(pathInfo->pathType);
        if(impl == zfnull) {
            return zfnull;
        }
        zfstring pathData;
        if(!impl->callbackIsDir(pathInfo->pathData)) {
            impl->callbackToParent(pathInfo->pathData, pathData);
            if(!impl->callbackToChild(pathData, pathData, pathFormated)) {
                return zfnull;
            }
        }
        else {
            if(!impl->callbackToChild(pathInfo->pathData, pathData, pathFormated)) {
                return zfnull;
            }
        }
        if(impl->callbackIsDir(pathData)) {
            zfobj<ZFMap> ret;
            _ZFP_zfimportDir(ret, *impl, ZFPathInfo(pathInfo->pathType, pathData), "");
            return ret;
        }
        else {
            zfauto ret;
            if(_ZFP_zfimportFile(ret, ZFInputForLocal(pathFormated, pathInfo))) {
                return ret;
            }
            else {
                return zfnull;
            }
        }
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfauto, zfimportCacheRemove
        , ZFMP_IN(const zfchar *, callbackId)
        ) {
    if(callbackId == zfnull) {
        return zfnull;
    }
    zfCoreMutexLocker();
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
    zfCoreMutexLock();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(cacheMap.empty()) {
        zfCoreMutexUnlock();
    }
    else {
        _ZFP_zfimportCacheMapType tmp;
        tmp.swap(cacheMap);
        zfCoreMutexUnlock();
    }
}

ZF_NAMESPACE_GLOBAL_END

