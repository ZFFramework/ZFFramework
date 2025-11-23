#include "ZFProtocol.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFProtocolLevel)
ZFENUM_DEFINE(ZFProtocolInstanceState)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFProtocol, ZFProtocol *)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProtocol, ZFProtocolInstanceState, protocolInstanceState)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProtocol, const zfchar *, protocolName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProtocol, const zfchar *, protocolImplName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProtocol, ZFProtocolLevel, protocolImplLevel)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProtocol, const zfchar *, protocolImplPlatformHint)

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFProtocolDataHolder) {
}
ZFCoreMap implDataMap; // _ZFP_ZFProtocolData *
ZF_STATIC_INITIALIZER_END(ZFProtocolDataHolder)
#define _ZFP_ZFProtocolDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFProtocolDataHolder)->implDataMap)

// ============================================================
// cleanup to ensure ZF's cleanup step queued before global static cleanup,
// which may break other framework's cleanup order,
// not reliable, but may solve most cases
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFProtocolImplCleanup_protocolOnDeallocPrepare, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFProtocolImplCleanup_protocolOnDeallocPrepare) {
    ZFCoreMap &implDataMap = _ZFP_ZFProtocolDataMap;
    ZFCoreArray<_ZFP_ZFProtocolData *> allValue;
    implDataMap.allValueT(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i) {
        _ZFP_ZFProtocolData *dataHolder = allValue[i];
        if(dataHolder->implInstance != zfnull) {
            dataHolder->implInstance->_ZFP_ZFProtocol_protocolInstanceState = v_ZFProtocolInstanceState::e_OnDeallocPrepare;
            dataHolder->implInstance->protocolOnDeallocPrepare();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFProtocolImplCleanup_protocolOnDeallocPrepare)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFProtocolImplCleanup_protocolOnDealloc, ZFLevelZFFrameworkStatic) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFProtocolImplCleanup_protocolOnDealloc) {
    ZFCoreMap &implDataMap = _ZFP_ZFProtocolDataMap;
    ZFCoreArray<_ZFP_ZFProtocolData *> allValue;
    implDataMap.allValueT(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i) {
        _ZFP_ZFProtocolData *dataHolder = allValue[i];
        if(dataHolder->implInstance != zfnull) {
            dataHolder->implInstance->_ZFP_ZFProtocol_protocolInstanceState = v_ZFProtocolInstanceState::e_OnDealloc;
            dataHolder->implInstance->protocolOnDealloc();
            dataHolder->implInstance->_ZFP_ZFProtocol_protocolInstanceState = v_ZFProtocolInstanceState::e_OnInit;
            dataHolder->implCleanupCallback(dataHolder);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFProtocolImplCleanup_protocolOnDealloc)

// ============================================================
_ZFP_ZFProtocolData &_ZFP_ZFProtocolImplDataRegister(
        ZF_IN const zfchar *protocolName
        , ZF_IN _ZFP_ZFProtocolTryAccessCallback implTryAccessCallback
        , ZF_IN zfbool protocolOptional
        ) {
    _ZFP_ZFProtocolData *dataHolder = _ZFP_ZFProtocolDataMap.get<_ZFP_ZFProtocolData *>(protocolName);
    if(dataHolder == zfnull) {
        dataHolder = zfpoolNew(_ZFP_ZFProtocolData);
        dataHolder->protocolName = protocolName;
        dataHolder->protocolOptional = protocolOptional;
        dataHolder->implTryAccessCallback = implTryAccessCallback;
        dataHolder->implConstructor = zfnull;
        dataHolder->implLevel = v_ZFProtocolLevel::e_Default;
        dataHolder->implCleanupCallback = zfnull;
        dataHolder->implInstance = zfnull;
        _ZFP_ZFProtocolDataMap.set(protocolName, ZFCorePointerForPoolObject<_ZFP_ZFProtocolData *>(dataHolder));
    }

    return *dataHolder;
}
void _ZFP_ZFProtocolImplDataUnregister(const zfchar *protocolName) {
}
void _ZFP_ZFProtocolImplAccess(void) {
    // access to ensure init order
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkStatic) == ZFFrameworkStateInitRunning) {
        (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFProtocolImplCleanup_protocolOnDealloc);
    }
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) == ZFFrameworkStateInitRunning) {
        (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFProtocolImplCleanup_protocolOnDeallocPrepare);
    }
}
void _ZFP_ZFProtocolImplFail(
        ZF_IN const zfchar *protocolName
        , ZF_IN const zfchar *protocolImplName
        , ZF_IN const zfchar *mismatchProtocolName
        , ZF_IN const zfchar *desiredImplPlatformHint
        , ZF_IN ZFProtocol *mismatchImpl
        ) {
    zfstring err;
    zfstringAppend(err
            , "ZFProtocol (%s)%s depends on (%s)\"%s\""
            , protocolName
            , protocolImplName
            , mismatchProtocolName
            , desiredImplPlatformHint
            );
    if(mismatchImpl == zfnull) {
        zfstringAppend(err, " but it's not implemented");
    }
    else {
        zfstringAppend(err, " but it's implementation \"%s\" mismatch",
                mismatchImpl->protocolImplPlatformHint());
    }
    ZFCoreCriticalMessageTrim("%s", err);
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(ZFProtocol *, ZFProtocolTryAccess
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN_OPT(const zfchar *, desiredImpl, zfnull)
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFProtocolData *data = _ZFP_ZFProtocolDataMap.get<_ZFP_ZFProtocolData *>(name);
    if(data != zfnull) {
        if(data->implInstance == zfnull) {
            data->implTryAccessCallback();
        }
        if(data->implInstance != zfnull
                && (zfstringIsEmpty(desiredImpl) || zfstringIsEqual(desiredImpl, data->implInstance->protocolImplPlatformHint()))
                ) {
            return data->implInstance;
        }
    }
    return zfnull;
}
ZFMETHOD_FUNC_DEFINE_2(ZFProtocol *, ZFProtocolAccess
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN_OPT(const zfchar *, desiredImpl, zfnull)
        ) {
    ZFProtocol *ret = ZFProtocolTryAccess(name, desiredImpl);
    if(ret == zfnull) {
        ZFProtocol *mismatchImpl = ZFProtocolTryAccess(name);
        if(mismatchImpl == zfnull) {
            ZFCoreCriticalMessageTrim("ZFProtocol (%s)%s mismatch with current impl \"%s\""
                    , name
                    , desiredImpl
                    , mismatchImpl->protocolImplPlatformHint()
                    );
        }
        else {
            ZFCoreCriticalMessageTrim("ZFProtocol (%s)%s not registered"
                    , name
                    , desiredImpl
                    );
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFProtocolIsAvailable
        , ZFMP_IN(const zfchar *, name)
        , ZFMP_IN_OPT(const zfchar *, desiredImpl, zfnull)
        ) {
    return (ZFProtocolTryAccess(name, desiredImpl) != zfnull);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFProtocolImplInfo, ZFProtocolImplInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFProtocolImplInfo, zfstring, protocolName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFProtocolImplInfo, zfbool, protocolOptional)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(v_ZFProtocolImplInfo, ZFProtocol *, protocolImpl)

void ZFProtocolImplInfo::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += this->protocolName;
    if(this->protocolOptional) {
        ret += " (optional)";
    }
    if(this->protocolImpl != zfnull) {
        zfstringAppend(ret, " %s(%s)"
                , this->protocolImpl->protocolImplName()
                , this->protocolImpl->protocolImplLevel()
                );
        if(!zfstringIsEmpty(this->protocolImpl->protocolImplPlatformHint())) {
            ret += " ";
            ret += this->protocolImpl->protocolImplPlatformHint();
        }
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

static void _ZFP_ZFProtocolImplInfoCopy(
        ZF_IN_OUT ZFProtocolImplInfo &data
        , ZF_IN _ZFP_ZFProtocolData &implData
        ) {
    data.protocolName = implData.protocolName;
    data.protocolOptional = implData.protocolOptional;
    data.protocolImpl = implData.implTryAccessCallback();
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFProtocolImplInfo>, ZFProtocolImplInfoGetAll) {
    ZFCoreArray<ZFProtocolImplInfo> ret;

    ZFCoreArray<_ZFP_ZFProtocolData *> allValue;
    _ZFP_ZFProtocolDataMap.allValueT(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i) {
        ZFProtocolImplInfo data;
        _ZFP_ZFProtocolImplInfoCopy(data, *(allValue[i]));
        ret.add(data);
    }

    return ret;
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFProtocolImplInfo>, ZFProtocolImplInfoGetAllImplemented) {
    ZFCoreArray<ZFProtocolImplInfo> ret;

    ZFCoreArray<_ZFP_ZFProtocolData *> allValue;
    _ZFP_ZFProtocolDataMap.allValueT(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i) {
        ZFProtocolImplInfo data;
        _ZFP_ZFProtocolData &implData = *(allValue[i]);
        if(implData.implTryAccessCallback() != zfnull) {
            _ZFP_ZFProtocolImplInfoCopy(data, implData);
            ret.add(data);
        }
    }

    return ret;
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<ZFProtocolImplInfo>, ZFProtocolImplInfoGetAllNotImplemented) {
    ZFCoreArray<ZFProtocolImplInfo> ret;

    ZFCoreArray<_ZFP_ZFProtocolData *> allValue;
    _ZFP_ZFProtocolDataMap.allValueT(allValue);
    for(zfindex i = 0; i < allValue.count(); ++i) {
        ZFProtocolImplInfo data;
        _ZFP_ZFProtocolData &implData = *(allValue[i]);
        if(implData.implTryAccessCallback() == zfnull) {
            _ZFP_ZFProtocolImplInfoCopy(data, implData);
            ret.add(data);
        }
    }

    return ret;
}
static ZFCompareResult _ZFP_ZFProtocolImplInfoPrint_sortComparer(
        ZF_IN const ZFProtocolImplInfo &e0
        , ZF_IN const ZFProtocolImplInfo &e1
        ) {
    zfint t = e0.protocolName.compare(e1.protocolName);
    if(t < 0) {
        return ZFCompareSmaller;
    }
    else if(t > 0) {
        return ZFCompareGreater;
    }
    else {
        return ZFCompareEqual;
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFProtocolImplInfoPrint
        , ZFMP_IN(const ZFProtocolImplInfo &, data)
        , ZFMP_IN_OPT(const ZFOutput &, callback, ZFOutputDefault())
        ) {
    callback.execute(data.protocolName.cString());
    if(data.protocolOptional) {
        callback.execute(" (optional)");
    }
    if(data.protocolImpl != zfnull) {
        callback.execute(" ");
        callback.execute(data.protocolImpl->protocolImplName());
        callback.execute("(");
        callback.execute(ZFProtocolLevelToString(data.protocolImpl->protocolImplLevel()).cString());
        callback.execute(")");
        if(!zfstringIsEmpty(data.protocolImpl->protocolImplPlatformHint())) {
            callback.execute(" ");
            callback.execute(data.protocolImpl->protocolImplPlatformHint());
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFProtocolImplInfoPrint
        , ZFMP_IN_OPT(const ZFOutput &, callback, ZFOutputDefault())
        ) {
    if(!callback) {
        return;
    }

    ZFCoreArray<ZFProtocolImplInfo> notImplemented = ZFProtocolImplInfoGetAllNotImplemented();
    ZFCoreArray<ZFProtocolImplInfo> implemented = ZFProtocolImplInfoGetAllImplemented();
    if(!notImplemented.isEmpty()) {
        notImplemented.sort(0, zfindexMax(), _ZFP_ZFProtocolImplInfoPrint_sortComparer);

        callback.execute("not implemented:\n");
        for(zfindex i = 0; i < notImplemented.count(); ++i) {
            const ZFProtocolImplInfo &data = notImplemented[i];
            callback.execute("  ");
            ZFProtocolImplInfoPrint(data, callback);
            callback.execute("\n");
        }
    }
    if(!implemented.isEmpty()) {
        implemented.sort(0, zfindexMax(), _ZFP_ZFProtocolImplInfoPrint_sortComparer);

        callback.execute("implemented:\n");
        for(zfindex i = 0; i < implemented.count(); ++i) {
            const ZFProtocolImplInfo &data = implemented[i];
            callback.execute("  ");
            ZFProtocolImplInfoPrint(data, callback);
            callback.execute("\n");
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

