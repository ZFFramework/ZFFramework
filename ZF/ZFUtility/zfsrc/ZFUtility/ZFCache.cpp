#include "ZFCache.h"

#include "ZFCore/ZFSTLWrapper/zfstllist.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCacheDataHolder, ZFLevelZFFrameworkStatic) {
    this->cacheTrimListener = ZFCallbackForFunc(zfself::cacheTrim);
}
public:
    ZFListener cacheTrimListener;
    zfstlhashmap<ZFCache *, zfbool> attachedObject;
public:
    static void cacheTrim(ZF_IN const ZFArgs &zfargs) {
        const zfstlhashmap<ZFCache *, zfbool> &attachedObject = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->attachedObject;
        for(zfstlhashmap<ZFCache *, zfbool>::const_iterator it = attachedObject.begin(); it != attachedObject.end(); ++it) {
            ZFCache *holder = it->first;
            holder->cacheTrim((zft_zfindex)(holder->cacheMaxSize() * holder->cacheTrimThreshold()));
        }
    }
    void cacheTrimListenerSetup(
            ZF_IN ZFCache *objectCache
            , ZF_IN zfbool autoTrim
            ) {
        ZFCoreMutexLocker();

        if(autoTrim) {
            this->attachedObject[objectCache] = zftrue;
        }
        else {
            this->attachedObject.erase(objectCache);
        }

        if(autoTrim && this->attachedObject.size() == 1) {
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::E_AppOnMemoryLow(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0) {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::E_AppOnMemoryLow(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFCacheDataHolder)

// ============================================================
zfclassFwd _ZFP_ZFCacheData;
typedef zfstllist<_ZFP_ZFCacheData *> _ZFP_ZFCacheList;
typedef zfstlhashmap<zfstring, _ZFP_ZFCacheList> _ZFP_ZFCacheMap;
zfclassNotPOD _ZFP_ZFCacheData {
public:
    ZFObject *cacheValue; // retained only when added to cacheList
    ZFListener cacheOnDeallocListener; // used to detect and move from aliveList to cacheList
    _ZFP_ZFCacheMap::iterator cacheMapIt; // it of _ZFP_ZFCachePrivate::cacheMap
    _ZFP_ZFCacheList::iterator cacheMapListIt; // it of _ZFP_ZFCachePrivate::cacheMap[cacheKey]
    _ZFP_ZFCacheList::iterator cacheListIt; // it of _ZFP_ZFCachePrivate::cacheList, invalid if still used by app
    _ZFP_ZFCacheList::iterator aliveListIt; // it of _ZFP_ZFCachePrivate::aliveList, invalid if not used by app
};
zfclassNotPOD _ZFP_ZFCachePrivate {
public:
    _ZFP_ZFCacheMap cacheMap; // all including used and not used by app
    _ZFP_ZFCacheList cacheList; // not used by app, new cache added or moved to tail
    _ZFP_ZFCacheList aliveList; // still used by app
};

// ============================================================
ZFOBJECT_REGISTER(ZFCache)

ZFEVENT_REGISTER(ZFCache, CacheOnAdd)
ZFEVENT_REGISTER(ZFCache, CacheOnRemove)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFCache, zfindex, cacheMaxSize) {
    this->cacheTrim(this->cacheMaxSize());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFCache, zfbool, cacheTrimWhenReceiveMemoryWarning) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(
        this, this->cacheTrimWhenReceiveMemoryWarning());
}

ZFMETHOD_DEFINE_2(ZFCache, void, cacheAdd
        , ZFMP_IN(const zfstring &, cacheKey)
        , ZFMP_IN(ZFObject *, cacheValue)
        ) {
    if(cacheKey == zfnull || cacheValue == zfnull) {
        return;
    }

    {
        zfsynchronize(this);
        _ZFP_ZFCacheData *cacheData = zfpoolNew(_ZFP_ZFCacheData);
        cacheData->cacheValue = cacheValue;
        ZFLISTENER_2(cacheOnDealloc
                , _ZFP_ZFCachePrivate *, d
                , _ZFP_ZFCacheData *, cacheData
                ) {
            d->aliveList.erase(cacheData->aliveListIt);
            cacheData->aliveListIt = d->aliveList.end();

            if(ZFFrameworkStateCheck() == ZFFrameworkStateCleanupRunning) {
                return;
            }

            d->cacheList.push_back(cacheData);
            --(cacheData->cacheListIt = d->cacheList.end());

            zfRetain(cacheData->cacheValue);
            // would be removed by retain in E_ObjectBeforeDealloc
            // cacheData->cacheValue->observerRemove(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
        } ZFLISTENER_END()
        cacheData->cacheOnDeallocListener = cacheOnDealloc;

        cacheData->cacheMapIt = d->cacheMap.insert(zfstlpair<zfstring, _ZFP_ZFCacheList>(cacheKey, _ZFP_ZFCacheList())).first;
        _ZFP_ZFCacheList &cacheList = cacheData->cacheMapIt->second;

        cacheList.push_back(cacheData);
        --(cacheData->cacheMapListIt = cacheList.end());

        cacheData->cacheListIt = d->cacheList.end();

        d->aliveList.push_back(cacheData);
        --(cacheData->aliveListIt = d->aliveList.end());

        cacheValue->observerAdd(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
    }

    this->cacheOnAdd(cacheValue);
    this->cacheTrim(this->cacheMaxSize());
}
ZFMETHOD_DEFINE_1(ZFCache, zfauto, cacheGet
        , ZFMP_IN(const zfstring &, cacheKey)
        ) {
    if(cacheKey == zfnull) {
        return zfnull;
    }
    zfsynchronize(this);
    _ZFP_ZFCacheMap::iterator cacheMapIt = d->cacheMap.find(cacheKey);
    if(cacheMapIt == d->cacheMap.end() || cacheMapIt->second.empty()) {
        return zfnull;
    }
    _ZFP_ZFCacheData *cacheData = *(cacheMapIt->second.begin());
    zfauto ret = cacheData->cacheValue;

    if(cacheData->aliveListIt != d->aliveList.end()) {
        cacheData->cacheValue->observerRemove(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
        d->aliveList.erase(cacheData->aliveListIt);
    }
    if(cacheData->cacheListIt != d->cacheList.end()) {
        d->cacheList.erase(cacheData->cacheListIt);
        zfRelease(cacheData->cacheValue);
    }
    cacheData->cacheMapIt->second.erase(cacheData->cacheMapListIt);
    zfpoolDelete(cacheData);

    this->cacheOnRemove(ret);
    return ret;
}
ZFMETHOD_DEFINE_1(ZFCache, zfauto, cacheCheck
        , ZFMP_IN(const zfstring &, cacheKey)
        ) {
    if(cacheKey == zfnull) {
        return zfnull;
    }
    zfsynchronize(this);
    _ZFP_ZFCacheMap::iterator cacheMapIt = d->cacheMap.find(cacheKey);
    if(cacheMapIt == d->cacheMap.end() || cacheMapIt->second.empty()) {
        return zfnull;
    }
    _ZFP_ZFCacheData *cacheData = *(cacheMapIt->second.begin());
    zfauto ret = cacheData->cacheValue;

    // move to aliveList
    if(cacheData->aliveListIt == d->aliveList.end()) {
        d->cacheList.erase(cacheData->cacheListIt);
        cacheData->cacheListIt = d->cacheList.end();
        zfRelease(cacheData->cacheValue);

        d->aliveList.push_back(cacheData);
        --(cacheData->aliveListIt = d->aliveList.end());

        cacheData->cacheValue->observerAdd(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
    }

    return ret;
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheRemove
        , ZFMP_IN(const zfstring &, cacheKey)
        ) {
    _ZFP_ZFCacheMap::iterator cacheMapIt = d->cacheMap.find(cacheKey);
    if(cacheMapIt == d->cacheMap.end() || cacheMapIt->second.empty()) {
        return;
    }
    zfsynchronize(this);
    do {
        _ZFP_ZFCacheList &cacheList = cacheMapIt->second;
        _ZFP_ZFCacheData *cacheData = *(cacheList.begin());
        ZFObject *toRelease = zfnull;

        cacheList.erase(cacheData->cacheMapListIt);
        if(cacheData->cacheListIt != d->cacheList.end()) {
            d->cacheList.erase(cacheData->cacheListIt);
            toRelease = cacheData->cacheValue;
        }
        if(cacheData->aliveListIt != d->aliveList.end()) {
            d->aliveList.erase(cacheData->aliveListIt);
            cacheData->cacheValue->observerRemove(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
        }

        this->cacheOnRemove(cacheData->cacheValue);
        zfRelease(toRelease);
        zfpoolDelete(cacheData);
    } while(!cacheMapIt->second.empty());
}

ZFMETHOD_DEFINE_0(ZFCache, void, cacheRemoveAll) {
    if(d->cacheList.empty()) {
        return;
    }
    zfsynchronize(this);
    do {
        for(_ZFP_ZFCacheMap::iterator cacheMapIt = d->cacheMap.begin(); cacheMapIt != d->cacheMap.end(); ++cacheMapIt) {
            while(!cacheMapIt->second.empty()) {
                _ZFP_ZFCacheList &cacheList = cacheMapIt->second;
                _ZFP_ZFCacheData *cacheData = *(cacheList.begin());
                ZFObject *toRelease = zfnull;

                cacheList.erase(cacheData->cacheMapListIt);
                if(cacheData->cacheListIt != d->cacheList.end()) {
                    d->cacheList.erase(cacheData->cacheListIt);
                    toRelease = cacheData->cacheValue;
                }
                if(cacheData->aliveListIt != d->aliveList.end()) {
                    d->aliveList.erase(cacheData->aliveListIt);
                    cacheData->cacheValue->observerRemove(ZFObject::E_ObjectBeforeDealloc(), cacheData->cacheOnDeallocListener);
                }

                this->cacheOnRemove(cacheData->cacheValue);
                zfRelease(toRelease);
                zfpoolDelete(cacheData);
            }
        }
    } while(!d->cacheList.empty());
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheTrim
        , ZFMP_IN(zfindex, size)
        ) {
    if(d->cacheList.size() <= (zfstlsize)size) {
        return;
    }
    zfsynchronize(this);
    do {
        _ZFP_ZFCacheData *cacheData = *(d->cacheList.begin());
        cacheData->cacheMapIt->second.erase(cacheData->cacheMapListIt);
        d->cacheList.erase(cacheData->cacheListIt);
        zfRelease(cacheData->cacheValue);
        zfpoolDelete(cacheData);
    } while(d->cacheList.size() > (zfstlsize)size);
}

ZFMETHOD_DEFINE_2(ZFCache, void, cacheGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, cacheKeyList)
        , ZFMP_IN_OUT(ZFCoreArray<zfauto> &, cacheValueList)
        ) {
    for(_ZFP_ZFCacheMap::iterator cacheMapIt = d->cacheMap.begin(); cacheMapIt != d->cacheMap.end(); ++cacheMapIt) {
        const zfstring &cacheKey = cacheMapIt->first;
        _ZFP_ZFCacheList &cacheList = cacheMapIt->second;
        for(_ZFP_ZFCacheList::iterator cacheListIt = cacheList.begin(); cacheListIt != cacheList.end(); ++cacheListIt) {
            cacheKeyList.add(cacheKey);
            cacheValueList.add((*cacheListIt)->cacheValue);
        }
    }
}

void ZFCache::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFCachePrivate);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
}
void ZFCache::objectOnDealloc(void) {
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(this, zffalse);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFCache::objectOnDeallocPrepare(void) {
    this->cacheRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

