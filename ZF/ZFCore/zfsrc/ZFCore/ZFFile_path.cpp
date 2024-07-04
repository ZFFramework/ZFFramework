#include "ZFFile.h"

#include "protocol/ZFProtocolZFFileCwd.h"
#include "protocol/ZFProtocolZFPath.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cwd
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForCwd) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull) {
        return impl->pathForCwd();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathForCwdChange
        , ZFMP_IN(const zfstring &, path)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull) {
        return impl->pathForCwdChange(path);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForCwdOrModule) {
    zfstring ret = ZFPathForCwd();
    if(ret != zfnull) {
        return ret;
    }
    else {
        return ZFPathForModule();
    }
}

// ============================================================
// module
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForModule) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModule();
}
ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForModuleFile) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModuleFile();
}

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForSettingOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForSetting) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForSetting
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForSettingOnChange())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForSettingOnChange(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting(path);
    }
}

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForStorageOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForStorage) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorage
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForStorageOnChange())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForStorageOnChange(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage(path);
    }
}

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForStorageSharedOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForStorageShared) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorageShared
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForStorageSharedOnChange())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForStorageSharedOnChange(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared(path);
    }
}

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForCacheOnChange)
ZFEVENT_GLOBAL_REGISTER(ZFPathForCacheBeforeClear)
ZFEVENT_GLOBAL_REGISTER(ZFPathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFPathForCache) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForCache();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForCache
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForCacheOnChange())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForCache());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForCache(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForCacheOnChange(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForCache(path);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFPathForCacheClear) {
    if(ZFPROTOCOL_IS_AVAILABLE(ZFPath)) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForCacheBeforeClear());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForCacheClear();
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFPathForCacheAfterClear());
    }
}

ZF_NAMESPACE_GLOBAL_END

