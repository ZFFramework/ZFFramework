#include "ZFCoreGlobalInitializer.h"
#include "ZFCoreMutex.h"
#include "ZFCoreArray.h"
#include "ZFCoreValue.h"
#include "zfstr.h"

#include "../ZFSTLWrapper/zfstlordermap.h"

// #define _ZFP_ZFCoreGlobalInitializer_DEBUG 1

#if _ZFP_ZFCoreGlobalInitializer_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFCoreGlobalInitializer_log(fmt, ...) \
        zfimplLog("%s [ZFGI] %s", zfimplTime(), zfstr(fmt, ##__VA_ARGS__).cString())

    #define _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFGI] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFCoreGlobalInitializer_log(fmt, ...)
    #define _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCoreArray<ZFFrameworkStateUpdateCallback> &_ZFP_ZFFrameworkInitFinishCallbacks(void) {
    static ZFCoreArray<ZFFrameworkStateUpdateCallback> d;
    return d;
}
ZFCoreArray<ZFFrameworkStateUpdateCallback> &_ZFP_ZFFrameworkCleanupPrepareCallbacks(void) {
    static ZFCoreArray<ZFFrameworkStateUpdateCallback> d;
    return d;
}

zfclassNotPOD _ZFP_GI_Data {
public:
    zfuint refCount;
    ZFFrameworkState state;
    zfstring name;
    ZFLevel level;
    void *instance;
    _ZFP_GI_Constructor constructor;
    _ZFP_GI_Destructor destructor;

public:
    _ZFP_GI_Data(void)
    : refCount(1)
    , state(ZFFrameworkStateNotAvailable)
    , name()
    , level(ZFLevelAppLow)
    , instance(zfnull)
    , constructor(zfnull)
    , destructor(zfnull)
    {
    }
    ~_ZFP_GI_Data(void) {
        if(this->instance != zfnull) {
            _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("destroy %s", this->name.cString());
            this->destructor(this->instance);
        }
    }
};
typedef zfstlordermap<zfstring, ZFCoreValue<_ZFP_GI_Data> > _ZFP_GI_DataMap;

static void _ZFP_GI_instanceInit(_ZFP_GI_DataMap &m) {
    if(!m.empty()) {
        // array may be changed during init step, copy it first
        _ZFP_GI_DataMap tmp = m;
        for(_ZFP_GI_DataMap::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            _ZFP_GI_Data &data = it->second.value();
            if(data.instance == zfnull) {
                _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("create %s", data.name.cString());
                data.instance = data.constructor();
            }
        }
    }
}
static void _ZFP_GI_instanceDealloc(_ZFP_GI_DataMap &m) {
    zfbool hasDataToClean = zffalse;
    do {
        hasDataToClean = zffalse;
        if(!m.empty()) {
            _ZFP_GI_DataMap::iterator it = m.end();
            --it;
            for( ; it != m.end(); --it) {
                _ZFP_GI_Data &data = it->second.value();
                if(data.instance != zfnull) {
                    hasDataToClean = zftrue;
                    void *tmp = data.instance;
                    data.instance = zfnull;
                    _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("destroy %s", data.name.cString());
                    data.destructor(tmp);
                }
            }
        }
    } while(hasDataToClean);
}

zfclassNotPOD _ZFP_GI_DataContainer {
public:
    ZFFrameworkState state;

    ZFFrameworkState stateZFFrameworkStatic;
    _ZFP_GI_DataMap dataMapZFFrameworkStatic;

    ZFFrameworkState stateZFFrameworkEssential;
    _ZFP_GI_DataMap dataMapZFFrameworkEssential;

    ZFFrameworkState stateZFFrameworkHigh;
    _ZFP_GI_DataMap dataMapZFFrameworkHigh;

    ZFFrameworkState stateZFFrameworkNormal;
    _ZFP_GI_DataMap dataMapZFFrameworkNormal;

    ZFFrameworkState stateZFFrameworkLow;
    _ZFP_GI_DataMap dataMapZFFrameworkLow;


    ZFFrameworkState stateAppEssential;
    _ZFP_GI_DataMap dataMapAppEssential;

    ZFFrameworkState stateAppHigh;
    _ZFP_GI_DataMap dataMapAppHigh;

    ZFFrameworkState stateAppNormal;
    _ZFP_GI_DataMap dataMapAppNormal;

    ZFFrameworkState stateAppLow;
    _ZFP_GI_DataMap dataMapAppLow;


    ZFFrameworkState stateZFFrameworkPostLow;
    _ZFP_GI_DataMap dataMapZFFrameworkPostLow;

    ZFFrameworkState stateZFFrameworkPostNormal;
    _ZFP_GI_DataMap dataMapZFFrameworkPostNormal;

    ZFFrameworkState stateZFFrameworkPostHigh;
    _ZFP_GI_DataMap dataMapZFFrameworkPostHigh;

    ZFFrameworkState stateZFFrameworkPostEssential;
    _ZFP_GI_DataMap dataMapZFFrameworkPostEssential;

    ZFFrameworkState stateZFFrameworkPostStatic;
    _ZFP_GI_DataMap dataMapZFFrameworkPostStatic;

public:
    _ZFP_GI_DataMap &dataMapForLevel(ZF_IN ZFLevel level) {
        switch(level) {
            case ZFLevelZFFrameworkStatic:
                return this->dataMapZFFrameworkStatic;
            case ZFLevelZFFrameworkEssential:
                return this->dataMapZFFrameworkEssential;
            case ZFLevelZFFrameworkHigh:
                return this->dataMapZFFrameworkHigh;
            case ZFLevelZFFrameworkNormal:
                return this->dataMapZFFrameworkNormal;
            case ZFLevelZFFrameworkLow:
                return this->dataMapZFFrameworkLow;

            case ZFLevelAppEssential:
                return this->dataMapAppEssential;
            case ZFLevelAppHigh:
                return this->dataMapAppHigh;
            case ZFLevelAppNormal:
                return this->dataMapAppNormal;
            case ZFLevelAppLow:
                return this->dataMapAppLow;

            case ZFLevelZFFrameworkPostLow:
                return this->dataMapZFFrameworkPostLow;
            case ZFLevelZFFrameworkPostNormal:
                return this->dataMapZFFrameworkPostNormal;
            case ZFLevelZFFrameworkPostHigh:
                return this->dataMapZFFrameworkPostHigh;
            case ZFLevelZFFrameworkPostEssential:
                return this->dataMapZFFrameworkPostEssential;
            case ZFLevelZFFrameworkPostStatic:
                return this->dataMapZFFrameworkPostStatic;

            default:
                ZFCoreCriticalShouldNotGoHere();
                return this->dataMapAppLow;
        }
    }

public:
    _ZFP_GI_DataContainer(void)
    : state(ZFFrameworkStateNotAvailable)

    , stateZFFrameworkStatic(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkStatic()
    , stateZFFrameworkEssential(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkEssential()
    , stateZFFrameworkHigh(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkHigh()
    , stateZFFrameworkNormal(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkNormal()
    , stateZFFrameworkLow(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkLow()

    , stateAppEssential(ZFFrameworkStateNotAvailable)
    , dataMapAppEssential()
    , stateAppHigh(ZFFrameworkStateNotAvailable)
    , dataMapAppHigh()
    , stateAppNormal(ZFFrameworkStateNotAvailable)
    , dataMapAppNormal()
    , stateAppLow(ZFFrameworkStateNotAvailable)
    , dataMapAppLow()

    , stateZFFrameworkPostLow(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkPostLow()
    , stateZFFrameworkPostNormal(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkPostNormal()
    , stateZFFrameworkPostHigh(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkPostHigh()
    , stateZFFrameworkPostEssential(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkPostEssential()
    , stateZFFrameworkPostStatic(ZFFrameworkStateNotAvailable)
    , dataMapZFFrameworkPostStatic()
    {
    }
};
static _ZFP_GI_DataContainer &_ZFP_GI_dataContainerInstance_(void) {
    static _ZFP_GI_DataContainer _instance;
    return _instance;
}
#define _ZFP_GI_dataContainerInstance (_ZFP_GI_dataContainerInstance_())

zfclassLikePOD _ZFP_ZFFrameworkAutoCleanupHolder {
public:
    ~_ZFP_ZFFrameworkAutoCleanupHolder(void) {
        ZFFrameworkCleanup();
    }
};
void ZFFrameworkInit(void) {
    _ZFP_ZFCoreGlobalInitializer_log("ZFFrameworkInit begin");
    zfbool mutexAvailable = ZFCoreMutexImplAvailable();
    if(mutexAvailable) {
        ZFCoreMutexLock();
    }

    _ZFP_GI_DataContainer &d = _ZFP_GI_dataContainerInstance;
    if(d.state == ZFFrameworkStateNotAvailable) {
        d.state = ZFFrameworkStateInitRunning;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkStatic");
        d.stateZFFrameworkStatic = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkStatic);
        d.stateZFFrameworkStatic = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkEssential");
        d.stateZFFrameworkEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkHigh");
        d.stateZFFrameworkHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkNormal");
        d.stateZFFrameworkNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkLow");
        d.stateZFFrameworkLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppEssential");
        d.stateAppEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapAppEssential);
        d.stateAppEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppHigh");
        d.stateAppHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapAppHigh);
        d.stateAppHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppNormal");
        d.stateAppNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapAppNormal);
        d.stateAppNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppLow");
        d.stateAppLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapAppLow);
        d.stateAppLow = ZFFrameworkStateAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPost");
        d.stateZFFrameworkPostLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostNormal");
        d.stateZFFrameworkPostNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostHigh");
        d.stateZFFrameworkPostHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostEssential");
        d.stateZFFrameworkPostEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostStatic");
        d.stateZFFrameworkPostStatic = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostStatic);
        d.stateZFFrameworkPostStatic = ZFFrameworkStateAvailable;


        d.state = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init notify callback");
        ZFCoreArray<ZFFrameworkStateUpdateCallback> &m = ZFFrameworkInitFinishCallbacks;
        for(zfindex i = 0; i < m.count(); ++i) {
            m[i]();
        }
    }
    else {
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkStatic);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkEssential);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkHigh);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkNormal);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkLow);

        _ZFP_GI_instanceInit(d.dataMapAppEssential);
        _ZFP_GI_instanceInit(d.dataMapAppHigh);
        _ZFP_GI_instanceInit(d.dataMapAppNormal);
        _ZFP_GI_instanceInit(d.dataMapAppLow);

        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostLow);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostNormal);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostHigh);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostEssential);
        _ZFP_GI_instanceInit(d.dataMapZFFrameworkPostStatic);
    }

    if(mutexAvailable) {
        ZFCoreMutexUnlock();
    }
    static _ZFP_ZFFrameworkAutoCleanupHolder _holder;
    _ZFP_ZFCoreGlobalInitializer_log("ZFFrameworkInit end");
}
void ZFFrameworkCleanup(void) {
    _ZFP_ZFCoreGlobalInitializer_log("ZFFrameworkCleanup begin");
    _ZFP_GI_DataContainer &d = _ZFP_GI_dataContainerInstance;
    if(d.state == ZFFrameworkStateAvailable) {
        d.state = ZFFrameworkStateCleanupRunning;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup notify callback");
        ZFCoreArray<ZFFrameworkStateUpdateCallback> &m = ZFFrameworkCleanupPrepareCallbacks;
        for(zfindex i = 0; i < m.count(); ++i) {
            m[i]();
        }

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostStatic");
        d.stateZFFrameworkPostStatic = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkPostStatic);
        d.stateZFFrameworkPostStatic = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostEssential");
        d.stateZFFrameworkPostEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostHigh");
        d.stateZFFrameworkPostHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostNormal");
        d.stateZFFrameworkPostNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostLow");
        d.stateZFFrameworkPostLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateNotAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppLow");
        d.stateAppLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapAppLow);
        d.stateAppLow = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppNormal");
        d.stateAppNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapAppNormal);
        d.stateAppNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppHigh");
        d.stateAppHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapAppHigh);
        d.stateAppHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppEssential");
        d.stateAppEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapAppEssential);
        d.stateAppEssential = ZFFrameworkStateNotAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkLow");
        d.stateZFFrameworkLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkNormal");
        d.stateZFFrameworkNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkHigh");
        d.stateZFFrameworkHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkEssential");
        d.stateZFFrameworkEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkStatic");
        d.stateZFFrameworkStatic = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataMapZFFrameworkStatic);
        d.stateZFFrameworkStatic = ZFFrameworkStateNotAvailable;

        d.state = ZFFrameworkStateNotAvailable;
    }
    _ZFP_ZFCoreGlobalInitializer_log("ZFFrameworkCleanup end");
}

void ZFFrameworkAssertInit(void) {
    ZFCoreAssertWithMessage(ZFFrameworkStateCheck() == ZFFrameworkStateAvailable, "ZFFramework hasn't been initialized");
}
ZFFrameworkState ZFFrameworkStateCheck(void) {
    return _ZFP_GI_dataContainerInstance.state;
}
ZFFrameworkState ZFFrameworkStateCheck(ZF_IN ZFLevel level) {
    switch(level) {
        case ZFLevelZFFrameworkStatic:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkStatic;
        case ZFLevelZFFrameworkEssential:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkEssential;
        case ZFLevelZFFrameworkHigh:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkHigh;
        case ZFLevelZFFrameworkNormal:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkNormal;
        case ZFLevelZFFrameworkLow:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkLow;

        case ZFLevelAppEssential:
            return _ZFP_GI_dataContainerInstance.stateAppEssential;
        case ZFLevelAppHigh:
            return _ZFP_GI_dataContainerInstance.stateAppHigh;
        case ZFLevelAppNormal:
            return _ZFP_GI_dataContainerInstance.stateAppNormal;
        case ZFLevelAppLow:
            return _ZFP_GI_dataContainerInstance.stateAppLow;

        case ZFLevelZFFrameworkPostLow:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkPostLow;
        case ZFLevelZFFrameworkPostNormal:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkPostNormal;
        case ZFLevelZFFrameworkPostHigh:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkPostHigh;
        case ZFLevelZFFrameworkPostEssential:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkPostEssential;
        case ZFLevelZFFrameworkPostStatic:
            return _ZFP_GI_dataContainerInstance.stateZFFrameworkPostStatic;

        default:
            ZFCoreCriticalShouldNotGoHere();
            return _ZFP_GI_dataContainerInstance.stateAppLow;
    }
}

// ============================================================
static void **_ZFP_GI_instanceAccess(
        ZF_IN const zfchar *name
        , ZF_IN ZFLevel level
        );
static void _ZFP_GI_dataRegister(
        ZF_IN const zfchar *name
        , ZF_IN ZFLevel level
        , ZF_IN _ZFP_GI_Constructor constructor
        , ZF_IN _ZFP_GI_Destructor destructor
        ) {
    _ZFP_GI_DataContainer &holder = _ZFP_GI_dataContainerInstance;
    _ZFP_GI_DataMap &dataMap = holder.dataMapForLevel(level);

    _ZFP_GI_Data *data = zfnull;
    {
        _ZFP_GI_DataMap::iterator it = dataMap.find(name);
        if(it != dataMap.end()) {
            data = &(it->second.value());
            ZFCoreAssert(level == data->level);
            ++(data->refCount);
        }
        else {
            data = &(dataMap[name].value());
            data->name = name;
            data->level = level;
            data->constructor = constructor;
            data->destructor = destructor;
        }
    }

    switch(ZFFrameworkStateCheck(level)) {
        case ZFFrameworkStateNotAvailable:
            // this is the normal routine
            break;
        case ZFFrameworkStateInitRunning:
            // static register during init processing,
            // may unable to detect dependency
            ZFCoreCriticalMessageTrim(
                "ZFGlobalInitializer %s attached during init processing, which is not allowed",
                name);
            break;
        case ZFFrameworkStateAvailable:
            // registered after init finish, manually load it
            _ZFP_GI_instanceAccess(name, level);
            break;
        case ZFFrameworkStateCleanupRunning:
            // static register during cleanup processing,
            // may cause wrong cleanup order
            ZFCoreCriticalMessageTrim(
                "ZFGlobalInitializer %s attached during cleanup processing, which is not allowed",
                name);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}
static void _ZFP_GI_dataUnregister(
        ZF_IN const zfchar *name
        , ZF_IN ZFLevel level
        ) {
    _ZFP_GI_DataContainer &holder = _ZFP_GI_dataContainerInstance;
    _ZFP_GI_DataMap &dataMap = holder.dataMapForLevel(level);

    _ZFP_GI_DataMap::iterator it = dataMap.find(name);
    if(it == dataMap.end()) {
        ZFCoreCriticalShouldNotGoHere();
        return;
    }
    _ZFP_GI_Data &data = it->second.value();
    --(data.refCount);
    if(data.refCount == 0) {
        dataMap.erase(it);
    }
}

static void _ZFP_GI_notifyInstanceCreated(ZF_IN const _ZFP_GI_Data &data);
static void **_ZFP_GI_instanceAccess(
        ZF_IN const zfchar *name
        , ZF_IN ZFLevel level
        ) {
    static void *dummy = zfnull;
    ZFCoreMutexLocker();

    if(ZFFrameworkStateCheck(level) == ZFFrameworkStateCleanupRunning) {
        ZFCoreCriticalMessageTrim(
            "try to reenter global initializer during ZFFrameworkCleanup, name: %s, "
            "typically due to invalid global initializer dependency",
            name);
        return &dummy;
    }

    _ZFP_GI_DataContainer &holder = _ZFP_GI_dataContainerInstance;
    _ZFP_GI_DataMap &dataMap = holder.dataMapForLevel(level);

    _ZFP_GI_DataMap::iterator it = dataMap.find(name);
    if(it == dataMap.end()) {
        ZFCoreCriticalShouldNotGoHere();
        return &dummy;
    }

    _ZFP_GI_Data &data = it->second.value();
    if(data.instance == zfnull) {
        _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("create %s", data.name.cString());
        data.instance = data.constructor();
        _ZFP_GI_notifyInstanceCreated(data);
    }

    return &(data.instance);
}

static const _ZFP_GI_Data *_ZFP_GI_dependencyCheck(_ZFP_GI_DataMap &data) {
    for(_ZFP_GI_DataMap::iterator it = data.begin(); it != data.end(); ++it) {
        if(it->second.value().instance == zfnull) {
            return &(it->second.value());
        }
    }
    return zfnull;
}
void _ZFP_GI_notifyInstanceCreated(ZF_IN const _ZFP_GI_Data &data) {
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkStatic) == ZFFrameworkStateNotAvailable) {
        ZFCoreCriticalMessageTrim(
                "ZFGlobalInitializer %s accessed before ZFFrameworkInit"
            , data.name);
        return;
    }

    _ZFP_GI_DataContainer &d = _ZFP_GI_dataContainerInstance;
    // check higher level initialized?
    const _ZFP_GI_Data *dependency = zfnull;
    do {
        if(data.level > ZFLevelZFFrameworkStatic) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkStatic)) != zfnull) {break;}
            if(data.level > ZFLevelZFFrameworkEssential) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkEssential)) != zfnull) {break;}
                if(data.level > ZFLevelZFFrameworkHigh) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkHigh)) != zfnull) {break;}
                    if(data.level > ZFLevelZFFrameworkNormal) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkNormal)) != zfnull) {break;}
                        if(data.level > ZFLevelZFFrameworkLow) {
                            if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkLow)) != zfnull) {break;}
                        }
                    }
                }
            }
        }
        if(data.level > ZFLevelAppEssential) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataMapAppEssential)) != zfnull) {break;}
            if(data.level > ZFLevelAppHigh) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataMapAppHigh)) != zfnull) {break;}
                if(data.level > ZFLevelAppNormal) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataMapAppNormal)) != zfnull) {break;}
                    if(data.level > ZFLevelAppLow) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataMapAppLow)) != zfnull) {break;}
                    }
                }
            }
        }
        if(data.level > ZFLevelZFFrameworkPostLow) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkPostLow)) != zfnull) {break;}
            if(data.level > ZFLevelZFFrameworkPostNormal) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkPostNormal)) != zfnull) {break;}
                if(data.level > ZFLevelZFFrameworkPostHigh) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkPostHigh)) != zfnull) {break;}
                    if(data.level > ZFLevelZFFrameworkPostEssential) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkPostEssential)) != zfnull) {break;}
                        if(data.level > ZFLevelZFFrameworkPostStatic) {
                            if((dependency = _ZFP_GI_dependencyCheck(d.dataMapZFFrameworkPostStatic)) != zfnull) {break;}
                        }
                    }
                }
            }
        }
    } while(zffalse);
    if(dependency != zfnull) {
        // dependency hasn't initialized
        ZFCoreCriticalMessageTrim(
                "ZFGlobalInitializer %s depends on or level lower than %s"
                ", while it hasn't been initialized or already deallocated"
                ", typically because of invalid dependency or invalid access"
            , data.name, dependency->name);
        return;
    }

    // reorder in same level, move self before first null instance
    _ZFP_GI_DataMap &dataMap = d.dataMapForLevel(data.level);
    _ZFP_GI_DataMap::iterator prevNull = dataMap.end();
    _ZFP_GI_DataMap::iterator selfPos = dataMap.end();
    for(_ZFP_GI_DataMap::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
        if(&(it->second.value()) == &data) {
            selfPos = it;
            if(prevNull != dataMap.end()) {
                break;
            }
        }
        else if(prevNull == dataMap.end() && it->second.value().instance == zfnull) {
            if(selfPos != dataMap.end()) {
                break;
            }
            prevNull = it;
        }
    }
    if(selfPos != dataMap.end() && prevNull != dataMap.end()) {
        dataMap.move(selfPos, prevNull);
    }
}

// ============================================================
zfclassNotPOD _ZFP_GI_RegPrivate {
public:
    const zfchar *name;
    ZFLevel level;
    void **instance;
public:
    _ZFP_GI_RegPrivate(
            ZF_IN const zfchar *name
            , ZF_IN ZFLevel level
            )
    : name(name)
    , level(level)
    , instance(zfnull)
    {
    }
};
_ZFP_GI_Reg::_ZFP_GI_Reg(
        ZF_IN const zfchar *name
        , ZF_IN ZFLevel level
        , ZF_IN _ZFP_GI_Constructor constructor
        , ZF_IN _ZFP_GI_Destructor destructor
        )
: d(zfpoolNew(_ZFP_GI_RegPrivate, name, level))
{
    _ZFP_GI_dataRegister(name, level, constructor, destructor);
}
_ZFP_GI_Reg::~_ZFP_GI_Reg(void) {
    _ZFP_GI_dataUnregister(d->name, d->level);
    zfpoolDelete(d);
}
void *_ZFP_GI_Reg::instanceAccess(void) {
    if(d->instance == zfnull || *(d->instance) == zfnull) {
        d->instance = _ZFP_GI_instanceAccess(d->name, d->level);
    }
    return *(d->instance);
}

ZF_NAMESPACE_GLOBAL_END

