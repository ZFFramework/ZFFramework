#include "zfimport.h"
#include "ZFHashMap.h"
#include "ZFIO_res.h"
#include "ZFIO_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFImportBegin)
ZFEVENT_GLOBAL_REGISTER(ZFImportEnd)
ZF_NAMESPACE_END(ZFGlobalEvent)

typedef ZFCoreMap<zfstring, zfauto> _ZFP_zfimportCacheMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfimportDataHolder, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfimportDataHolder) {
}
public:
    _ZFP_zfimportCacheMapType cacheMap;
ZF_GLOBAL_INITIALIZER_END(zfimportDataHolder)

static zfauto _ZFP_zfimportFile(
        ZF_IN_OUT _ZFP_zfimportCacheMapType &cacheMap
        , ZF_IN const ZFPathInfo &pathInfo
        ) {
    ZFInput input = ZFInputForPathInfo(pathInfo);
    if(!input) {
        return zfnull;
    }
    zfstring callbackId = input.callbackId();
    if(callbackId) {
        ZFCoreMutexLocker();
        zfiter it = cacheMap.iterFind(callbackId);
        if(it) {
            ZFObject *cache = cacheMap.iterValue(it);
            if(cache == ZFNull()) {
                return zfnull;
            }
            else {
                return cache;
            }
        }

        // mark loading
        cacheMap.set(callbackId, ZFNull());
    }

    zfobj<v_ZFInput> inputHolder;
    inputHolder->zfv = input;
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFImportBegin(), ZFArgs().param0(inputHolder));
    zfauto ret;
    zfbool success = ZFObjectIOLoadT(ret, input);
    ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFImportEnd(), ZFArgs().param0(inputHolder).param1(ret));

    if(callbackId) {
        ZFCoreMutexLocker();
        if(success) {
            cacheMap.set(callbackId, ret);
        }
        else {
            // unmark loading
            cacheMap.remove(callbackId);
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfauto, zfimport
        , ZFMP_IN(const zfstring &, path)
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        ) {
    ZFPathInfo abs;
    if(ZFPathInfoFromStringT(abs, path)) {
        // done
    }
    else if(pathInfo == zfnull) {
        if(ZFResIsDir(path)) {
            abs.pathType(ZFPathType_res());
        }
        else {
            abs.pathType(ZFPathType_file());
        }
        abs.pathData(path);
    }
    else {
        zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
        if(ioImpl == zfnull) {
            return zfnull;
        }
        zfstring pathData;
        if(!ioImpl->ioIsDir(pathInfo.pathData())) {
            ioImpl->ioToParent(pathData, pathInfo.pathData());
            ioImpl->ioToChild(pathData, pathData, path);
        }
        else {
            ioImpl->ioToChild(pathData, pathInfo.pathData(), path);
        }
        if(!pathData) {
            return zfnull;
        }
        abs.pathType(pathInfo.pathType());
        abs.pathData(pathData);
    }

    {
        zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(abs.pathType());
        if(!ioImpl
                || !ioImpl->ioIsExist(abs.pathData())
                ) {
            return zfnull;
        }
        if(!ioImpl->ioIsDir(abs.pathData())) {
            return _ZFP_zfimportFile(ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap, abs);
        }
    }

    zfobj<ZFHashMap> retMap;
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    ZFLISTENER_2(impl
            , zfautoT<ZFHashMap>, retMap
            , _ZFP_zfimportCacheMapType &, cacheMap
            ) {
        const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
        const zfstring &relPath = zfargs.param1().to<v_zfstring *>()->zfv;
        zfauto ret = _ZFP_zfimportFile(cacheMap, pathInfo);
        if(ret) {
            retMap->set(zfobj<v_zfstring>(relPath), ret);
        }
    } ZFLISTENER_END()
    ZFIOForEachFile(abs, impl);
    return retMap;
}

ZFMETHOD_FUNC_DEFINE_1(zfauto, zfimportCacheRemove
        , ZFMP_IN(const zfstring &, callbackId)
        ) {
    if(callbackId == zfnull) {
        return zfnull;
    }
    ZFCoreMutexLocker();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    zfiter it = cacheMap.iterFind(callbackId);
    if(it) {
        zfauto ret = cacheMap.iterValue(it);
        cacheMap.iterRemove(it);
        return ret;
    }
    else {
        return zfnull;
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, zfimportCacheRemoveAll) {
    ZFCoreMutexLock();
    _ZFP_zfimportCacheMapType &cacheMap = ZF_GLOBAL_INITIALIZER_INSTANCE(zfimportDataHolder)->cacheMap;
    if(cacheMap.isEmpty()) {
        ZFCoreMutexUnlock();
    }
    else {
        _ZFP_zfimportCacheMapType tmp;
        tmp.swap(cacheMap);
        ZFCoreMutexUnlock();
    }
}

ZF_NAMESPACE_GLOBAL_END

