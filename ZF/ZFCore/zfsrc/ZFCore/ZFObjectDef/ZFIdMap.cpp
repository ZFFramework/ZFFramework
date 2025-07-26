#include "ZFIdMap.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
_ZFP_ZFIdMapHolder::_ZFP_ZFIdMapHolder(
        ZF_IN const zfstring &idName
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN const zfstring &ownerNamespace
        , ZF_IN const zfstring &methodName
)
: idValue(_ZFP_ZFIdMapRegister(idName))
{
    zfclassNotPOD _ZFP_IdMap_GI {
    public:
        static void GI(ZF_IN_OUT const ZFArgs &zfargs) {
            zfargs.result(zfargs.ownerMethod()->dynamicRegisterUserData());
        }
    };
    ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .ownerClass(ownerClass)
            .methodNamespace(ownerNamespace)
            .methodType(ZFMethodTypeStatic)
            .returnTypeId(ZFTypeId_zfidentity())
            .methodName(methodName)
            .methodGenericInvoker(_ZFP_IdMap_GI::GI)
            .dynamicRegisterUserData(zfobj<v_zfidentity>(*idValue))
            .zfunsafe_disableChecker(zftrue)
            );
}
_ZFP_ZFIdMapHolder::~_ZFP_ZFIdMapHolder(void) {
    zfstring idName = ZFIdMapNameForId(*idValue);
    if(idName != zfnull) {
        zfindex dotPos = zfstringFindReversely(idName, ".");
        const ZFMethod *method = dotPos != zfindexMax()
            ? ZFMethodForName(zfstring(idName, dotPos), idName + dotPos + 1)
            : ZFMethodForName(zfnull, idName)
            ;
        if(method != zfnull) {
            ZFMethodDynamicUnregister(method);
        }
    }
    _ZFP_ZFIdMapUnregister(*idValue);
}

// ============================================================
zfclassLikePOD _ZFP_ZFIdMapData {
public:
    zfuint refCount;
    zfidentity idValue;
    zfstring idName;
    zfbool isDynamicRegister;

public:
    _ZFP_ZFIdMapData(void)
    : refCount(1)
    , idValue(zfidentityInvalid())
    , idName()
    , isDynamicRegister(zffalse)
    {
    }
};
typedef zfstlhashmap<zfidentity, _ZFP_ZFIdMapData *> _ZFP_ZFIdMapDataIdMapType;
typedef zfstlhashmap<zfstring, _ZFP_ZFIdMapData *> _ZFP_ZFIdMapDataNameMapType;

zfclassLikePOD _ZFP_ZFIdMapModuleData {
public:
    _ZFP_ZFIdMapDataIdMapType dataIdMap;
    _ZFP_ZFIdMapDataNameMapType dataNameMap;
    ZFIdentityGenerator idValueGenerator;
};

static _ZFP_ZFIdMapModuleData &_ZFP_ZFIdMapModuleDataRef(void) {
    static _ZFP_ZFIdMapModuleData d;
    return d;
}

const zfidentity *_ZFP_ZFIdMapRegister(
        ZF_IN const zfstring &idName
        , ZF_IN_OPT zfbool isDynamicRegister /* = zffalse */
        ) {
    if(!idName) {
        ZFCoreCriticalMessageTrim("[ZFIdMapDynamicRegister] empty name");
    }

    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapData *data = zfnull;
    _ZFP_ZFIdMapDataNameMapType::iterator itName = dataNameMap.find(idName);
    if(itName != dataNameMap.end()) {
        data = itName->second;
    }
    if(data != zfnull) {
        if(isDynamicRegister) {
            ZFCoreCriticalMessageTrim("[ZFIdMapDynamicRegister] already registered: %s", idName);
        }
        ++(data->refCount);
    }
    else {
        data = zfnew(_ZFP_ZFIdMapData);
        data->idValue = moduleData.idValueGenerator.idAcquire();
        data->idName = idName;
        data->isDynamicRegister = isDynamicRegister;

        dataIdMap[data->idValue] = data;
        dataNameMap[data->idName] = data;
    }
    return &(data->idValue);
}
void _ZFP_ZFIdMapUnregister(
        ZF_IN zfidentity idValue
        , ZF_IN_OPT zfbool isDynamicRegister /* = zffalse */
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataIdMapType::iterator it = dataIdMap.find(idValue);
    if(it == dataIdMap.end()) {
        if(!isDynamicRegister) {
            ZFCoreCriticalShouldNotGoHere();
        }
        return;
    }
    _ZFP_ZFIdMapData *data = it->second;
    if(!data->isDynamicRegister && isDynamicRegister) {
        ZFCoreCriticalMessageTrim(
            "[ZFIdMapDynamicUnregister] unregister %s(%s) which is not dynamic registered",
            data->idValue,
            data->idName);
    }
    --(data->refCount);
    if(data->refCount == 0) {
        dataIdMap.erase(data->idValue);
        dataNameMap.erase(data->idName);
        zfdelete(data);
    }
}
zfstring ZFIdMapNameForId(ZF_IN zfidentity idValue) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;

    _ZFP_ZFIdMapDataIdMapType::const_iterator it = dataIdMap.find(idValue);
    if(it != dataIdMap.end()) {
        return it->second->idName;
    }
    return zfnull;
}
zfidentity ZFIdMapIdForName(ZF_IN const zfstring &idName) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataNameMapType::const_iterator it = dataNameMap.find(idName);
    if(it != dataNameMap.end()) {
        return it->second->idValue;
    }
    return zfidentityInvalid();
}
void ZFIdMapGetAll(
        ZF_IN_OUT ZFCoreArray<zfidentity> &idValues
        , ZF_IN_OUT ZFCoreArray<zfstring> &idNames
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    idValues.capacity(idValues.count() + moduleData.dataIdMap.size());
    idNames.capacity(idNames.count() + moduleData.dataIdMap.size());
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = moduleData.dataIdMap.begin(); it != moduleData.dataIdMap.end(); ++it) {
        idValues.add(it->second->idValue);
        idNames.add(it->second->idName);
    }
}
void ZFIdMapGetAllValueT(ZF_IN_OUT ZFCoreArray<zfidentity> &idValues) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    idValues.capacity(idValues.count() + moduleData.dataIdMap.size());
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = moduleData.dataIdMap.begin(); it != moduleData.dataIdMap.end(); ++it) {
        idValues.add(it->second->idValue);
    }
}
void ZFIdMapGetAllNameT(ZF_IN_OUT ZFCoreArray<zfstring> &idNames) {
    ZFCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    idNames.capacity(idNames.count() + moduleData.dataIdMap.size());
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = moduleData.dataIdMap.begin(); it != moduleData.dataIdMap.end(); ++it) {
        idNames.add(it->second->idName);
    }
}

zfidentity ZFIdMapDynamicRegister(ZF_IN const zfstring &idName) {
    return *_ZFP_ZFIdMapRegister(idName, zftrue);
}
void ZFIdMapDynamicUnregister(ZF_IN zfidentity idValue) {
    _ZFP_ZFIdMapUnregister(idValue, zftrue);
}

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFIdMapNameForId
        , ZFMP_IN(zfidentity, idValue)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFIdMapIdForName
        , ZFMP_IN(const zfstring &, idName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFIdMapGetAll
        , ZFMP_IN_OUT(ZFCoreArray<zfidentity> &, idValues)
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, idNames)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFIdMapGetAllValueT
        , ZFMP_IN_OUT(ZFCoreArray<zfidentity> &, idValues)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<zfidentity>, ZFIdMapGetAllValue)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFIdMapGetAllNameT
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, idNames)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArray<zfstring>, ZFIdMapGetAllName)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFIdMapDynamicRegister
        , ZFMP_IN(const zfstring &, idName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFIdMapDynamicUnregister
        , ZFMP_IN(zfidentity, idValue)
        )

ZF_NAMESPACE_GLOBAL_END

