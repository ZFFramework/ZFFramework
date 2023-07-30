#include "ZFFile.h"

#include "protocol/ZFProtocolZFFileCwd.h"
#include "protocol/ZFProtocolZFPath.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cwd
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForCwd) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull) {
        return impl->pathForCwd();
    }
    else {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathForCwdChange
        , ZFMP_IN(const zfchar *, path)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull) {
        return impl->pathForCwdChange(path);
    }
    else {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForCwdOrModule) {
    const zfchar *ret = ZFPathForCwd();
    if(ret != zfnull) {
        return ret;
    }
    else {
        return ZFPathForModule();
    }
}

// ============================================================
// module
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForModule) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModule();
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForModuleFile) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModuleFile();
}

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForSettingOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForSetting) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForSetting
        , ZFMP_IN_OPT(const zfchar *, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForSettingOnChange())) {
        zfblockedAlloc(v_zfstring, old, ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting());
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
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForStorageOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForStorage) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorage
        , ZFMP_IN_OPT(const zfchar *, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForStorageOnChange())) {
        zfblockedAlloc(v_zfstring, old, ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage());
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
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForStorageSharedOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForStorageShared) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorageShared
        , ZFMP_IN_OPT(const zfchar *, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForStorageSharedOnChange())) {
        zfblockedAlloc(v_zfstring, old, ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared());
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
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForCacheOnChange)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForCacheBeforeClear)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFPathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFPathForCache) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForCache();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForCache
        , ZFMP_IN_OPT(const zfchar *, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventZFPathForCacheOnChange())) {
        zfblockedAlloc(v_zfstring, old, ZFPROTOCOL_ACCESS(ZFPath)->pathForCache());
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

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPathForCacheAutoClear, ZFLevelZFFrameworkLow) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPathForCacheAutoClear) {
    ZFPathForCacheClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFPathForCacheAutoClear)

ZF_NAMESPACE_GLOBAL_END

