#include "ZFIODef_path.h"

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
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFPathForCwd
        , ZFMP_IN(const zfstring &, path)
        ) {
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull) {
        return impl->pathForCwd(path);
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
ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForModule) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModule();
}
ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForModuleFile) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForModuleFile();
}

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForSettingOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForSetting) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForSetting
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::E_ZFPathForSettingOnUpdate())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFPathForSettingOnUpdate(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForSetting(path);
    }
}

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForStorageOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForStorage) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorage
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::E_ZFPathForStorageOnUpdate())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFPathForStorageOnUpdate(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorage(path);
    }
}

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForStorageSharedOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForStorageShared) {
    return ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForStorageShared
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::E_ZFPathForStorageSharedOnUpdate())) {
        zfobj<v_zfstring> old(ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared());
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared(path);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFPathForStorageSharedOnUpdate(), old);
    }
    else {
        ZFPROTOCOL_ACCESS(ZFPath)->pathForStorageShared(path);
    }
}

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFPathForCacheOnUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_STATIC_INITIALIZER_INIT(ZFPathForCache) {
}
ZF_STATIC_INITIALIZER_DESTROY(ZFPathForCache) {
}
public:
    const zfstring &get(void) {
        if(_realPath) {
            return _realPath;
        }
        zfstring path = ZFPROTOCOL_ACCESS(ZFPath)->pathForCache();
        ZFCoreAssert(path);
        _update(path);
        return _realPath;
    }
    void set(ZF_IN const zfstring &path) {
        if(_rootPath == path) {
            return;
        }
        ZFPROTOCOL_ACCESS(ZFPath)->pathForCache(path);
        _update(path);
    }
private:
    zfstring _rootPath;
    zfstring _realPath; // _rootPath/randName
    void *_lockToken;
private:
    static void _rootPathCleanup(ZF_IN const zfstring &rootPath) {
        ZFIOFindData fd;
        if(ZFFileFindFirst(fd, rootPath)) {
            do {
                if(fd.name() && fd.isDir()) {
                    zfstring lockPath = zfstr("%s/%s/.ZF.lock", rootPath, fd.name());
                    void *lockToken = ZFFileOpen(lockPath, v_ZFIOOpenOption::e_Write);
                    if(lockToken) {
                        ZFFileClose(lockToken);
                        ZFFileRemove(zfstr("%s/%s", rootPath, fd.name()));
                    }
                }
            } while(ZFFileFindNext(fd));
            ZFFileFindClose(fd);
        }
    }
    void _update(ZF_IN const zfstring &path) {
        zfstring old = _realPath;
        if(_rootPath) {
            if(_lockToken) {
                ZFFileClose(_lockToken);
                _lockToken = zfnull;
            }
            _rootPathCleanup(_rootPath);
        }
        _rootPath = path;
        _rootPathCleanup(_rootPath);
        do {
            _realPath = zfstr("%s/%x"
                    , _rootPath
                    , zfidentityHash(zfidentityCalc(zfmRand()), zfidentityCalc(this))
                    );
            _lockToken = ZFFileOpen(zfstr("%s/.ZF.lock", _realPath), v_ZFIOOpenOption::e_Write);
        } while(!_lockToken);
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFPathForCacheOnUpdate(), zfobj<v_zfstring>(old));
    }
ZF_STATIC_INITIALIZER_END(ZFPathForCache)

ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathForCache) {
    return ZF_STATIC_INITIALIZER_INSTANCE(ZFPathForCache)->get();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFPathForCache
        , ZFMP_IN_OPT(const zfstring &, path, zfnull)
        ) {
    ZF_STATIC_INITIALIZER_INSTANCE(ZFPathForCache)->set(path);
}

ZF_NAMESPACE_GLOBAL_END

