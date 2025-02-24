#include "ZFCoreGlobalInitializer.h"
#include "ZFCoreMutex.h"
#include "ZFCorePointer.h"
#include "ZFCoreArray.h"
#include "ZFCoreMap.h"
#include "ZFCoreStringConvert.h"

// #define _ZFP_ZFCoreGlobalInitializer_DEBUG 1

#if _ZFP_ZFCoreGlobalInitializer_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFCoreGlobalInitializer_log(fmt, ...) \
        zfimplLog("%s [ZFGI] " fmt, zfimplTime(), ##__VA_ARGS__)

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

static void _ZFP_GI_instanceInit(ZFCoreArray<_ZFP_GI_Data *> &list) {
    if(!list.isEmpty()) {
        // array may be changed during init step, copy it first
        ZFCoreArray<_ZFP_GI_Data *> tmp;
        tmp.copyFrom(list);
        for(zfindex i = 0; i < tmp.count(); ++i) {
            _ZFP_GI_Data *data = tmp.get(i);
            if(data->instance == zfnull) {
                _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("create %s", data->name.cString());
                data->instance = data->constructor();
            }
        }
    }
}
static void _ZFP_GI_instanceDealloc(ZFCoreArray<_ZFP_GI_Data *> &list) {
    zfbool hasDataToClean = zftrue;
    do {
        hasDataToClean = zffalse;
        for(zfindex i = list.count() - 1; i != zfindexMax(); --i) {
            _ZFP_GI_Data *data = list.get(i);
            if(data->instance != zfnull) {
                hasDataToClean = zftrue;
                void *tmp = data->instance;
                data->instance = zfnull;
                _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("destroy %s", data->name.cString());
                data->destructor(tmp);
            }
        }
    } while(hasDataToClean);
}

zfclassNotPOD _ZFP_GI_DataContainer {
public:
    ZFFrameworkState state;

    ZFFrameworkState stateZFFrameworkStatic;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkStatic;
    ZFCoreMap dataMapLevelZFFrameworkStatic; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkEssential;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkEssential;
    ZFCoreMap dataMapLevelZFFrameworkEssential; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkHigh;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkHigh;
    ZFCoreMap dataMapLevelZFFrameworkHigh; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkNormal;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkNormal;
    ZFCoreMap dataMapLevelZFFrameworkNormal; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkLow;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkLow;
    ZFCoreMap dataMapLevelZFFrameworkLow; // _ZFP_GI_Data *


    ZFFrameworkState stateAppEssential;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelAppEssential;
    ZFCoreMap dataMapLevelAppEssential; // _ZFP_GI_Data *

    ZFFrameworkState stateAppHigh;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelAppHigh;
    ZFCoreMap dataMapLevelAppHigh; // _ZFP_GI_Data *

    ZFFrameworkState stateAppNormal;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelAppNormal;
    ZFCoreMap dataMapLevelAppNormal; // _ZFP_GI_Data *

    ZFFrameworkState stateAppLow;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelAppLow;
    ZFCoreMap dataMapLevelAppLow; // _ZFP_GI_Data *


    ZFFrameworkState stateZFFrameworkPostLow;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkPostLow;
    ZFCoreMap dataMapLevelZFFrameworkPostLow; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkPostNormal;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkPostNormal;
    ZFCoreMap dataMapLevelZFFrameworkPostNormal; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkPostHigh;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkPostHigh;
    ZFCoreMap dataMapLevelZFFrameworkPostHigh; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkPostEssential;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkPostEssential;
    ZFCoreMap dataMapLevelZFFrameworkPostEssential; // _ZFP_GI_Data *

    ZFFrameworkState stateZFFrameworkPostStatic;
    ZFCoreArray<_ZFP_GI_Data *> dataLevelZFFrameworkPostStatic;
    ZFCoreMap dataMapLevelZFFrameworkPostStatic; // _ZFP_GI_Data *

public:
    ZFCoreArray<_ZFP_GI_Data *> &dataListForLevel(ZF_IN ZFLevel level) {
        switch(level) {
            case ZFLevelZFFrameworkStatic:
                return this->dataLevelZFFrameworkStatic;
            case ZFLevelZFFrameworkEssential:
                return this->dataLevelZFFrameworkEssential;
            case ZFLevelZFFrameworkHigh:
                return this->dataLevelZFFrameworkHigh;
            case ZFLevelZFFrameworkNormal:
                return this->dataLevelZFFrameworkNormal;
            case ZFLevelZFFrameworkLow:
                return this->dataLevelZFFrameworkLow;

            case ZFLevelAppEssential:
                return this->dataLevelAppEssential;
            case ZFLevelAppHigh:
                return this->dataLevelAppHigh;
            case ZFLevelAppNormal:
                return this->dataLevelAppNormal;
            case ZFLevelAppLow:
                return this->dataLevelAppLow;

            case ZFLevelZFFrameworkPostLow:
                return this->dataLevelZFFrameworkPostLow;
            case ZFLevelZFFrameworkPostNormal:
                return this->dataLevelZFFrameworkPostNormal;
            case ZFLevelZFFrameworkPostHigh:
                return this->dataLevelZFFrameworkPostHigh;
            case ZFLevelZFFrameworkPostEssential:
                return this->dataLevelZFFrameworkPostEssential;
            case ZFLevelZFFrameworkPostStatic:
                return this->dataLevelZFFrameworkPostStatic;

            default:
                ZFCoreCriticalShouldNotGoHere();
                return this->dataLevelAppLow;
        }
    }
    ZFCoreMap &dataMapForLevel(ZF_IN ZFLevel level) {
        switch(level) {
            case ZFLevelZFFrameworkStatic:
                return this->dataMapLevelZFFrameworkStatic;
            case ZFLevelZFFrameworkEssential:
                return this->dataMapLevelZFFrameworkEssential;
            case ZFLevelZFFrameworkHigh:
                return this->dataMapLevelZFFrameworkHigh;
            case ZFLevelZFFrameworkNormal:
                return this->dataMapLevelZFFrameworkNormal;
            case ZFLevelZFFrameworkLow:
                return this->dataMapLevelZFFrameworkLow;

            case ZFLevelAppEssential:
                return this->dataMapLevelAppEssential;
            case ZFLevelAppHigh:
                return this->dataMapLevelAppHigh;
            case ZFLevelAppNormal:
                return this->dataMapLevelAppNormal;
            case ZFLevelAppLow:
                return this->dataMapLevelAppLow;

            case ZFLevelZFFrameworkPostLow:
                return this->dataMapLevelZFFrameworkPostLow;
            case ZFLevelZFFrameworkPostNormal:
                return this->dataMapLevelZFFrameworkPostNormal;
            case ZFLevelZFFrameworkPostHigh:
                return this->dataMapLevelZFFrameworkPostHigh;
            case ZFLevelZFFrameworkPostEssential:
                return this->dataMapLevelZFFrameworkPostEssential;
            case ZFLevelZFFrameworkPostStatic:
                return this->dataMapLevelZFFrameworkPostStatic;

            default:
                ZFCoreCriticalShouldNotGoHere();
                return this->dataMapLevelAppLow;
        }
    }

public:
    _ZFP_GI_DataContainer(void)
    : state(ZFFrameworkStateNotAvailable)

    , stateZFFrameworkStatic(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkStatic()
    , stateZFFrameworkEssential(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkEssential()
    , stateZFFrameworkHigh(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkHigh()
    , stateZFFrameworkNormal(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkNormal()
    , stateZFFrameworkLow(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkLow()

    , stateAppEssential(ZFFrameworkStateNotAvailable)
    , dataLevelAppEssential()
    , stateAppHigh(ZFFrameworkStateNotAvailable)
    , dataLevelAppHigh()
    , stateAppNormal(ZFFrameworkStateNotAvailable)
    , dataLevelAppNormal()
    , stateAppLow(ZFFrameworkStateNotAvailable)
    , dataLevelAppLow()

    , stateZFFrameworkPostLow(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostLow()
    , stateZFFrameworkPostNormal(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostNormal()
    , stateZFFrameworkPostHigh(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostHigh()
    , stateZFFrameworkPostEssential(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostEssential()
    , stateZFFrameworkPostStatic(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostStatic()
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
    static _ZFP_ZFFrameworkAutoCleanupHolder _holder;
    zfbool mutexAvailable = ZFCoreMutexImplAvailable();
    if(mutexAvailable) {
        ZFCoreMutexLock();
    }

    _ZFP_GI_DataContainer &d = _ZFP_GI_dataContainerInstance;
    if(d.state == ZFFrameworkStateNotAvailable) {
        d.state = ZFFrameworkStateInitRunning;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkStatic");
        d.stateZFFrameworkStatic = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkStatic);
        d.stateZFFrameworkStatic = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkEssential");
        d.stateZFFrameworkEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkHigh");
        d.stateZFFrameworkHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkNormal");
        d.stateZFFrameworkNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkLow");
        d.stateZFFrameworkLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppEssential");
        d.stateAppEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelAppEssential);
        d.stateAppEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppHigh");
        d.stateAppHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelAppHigh);
        d.stateAppHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppNormal");
        d.stateAppNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelAppNormal);
        d.stateAppNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelAppLow");
        d.stateAppLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelAppLow);
        d.stateAppLow = ZFFrameworkStateAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPost");
        d.stateZFFrameworkPostLow = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostNormal");
        d.stateZFFrameworkPostNormal = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostHigh");
        d.stateZFFrameworkPostHigh = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostEssential");
        d.stateZFFrameworkPostEssential = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init ZFLevelZFFrameworkPostStatic");
        d.stateZFFrameworkPostStatic = ZFFrameworkStateInitRunning;
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostStatic);
        d.stateZFFrameworkPostStatic = ZFFrameworkStateAvailable;


        d.state = ZFFrameworkStateAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("init notify callback");
        ZFCoreArray<ZFFrameworkStateUpdateCallback> &m = ZFFrameworkInitFinishCallbacks;
        for(zfindex i = 0; i < m.count(); ++i) {
            m[i]();
        }
    }
    else {
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkStatic);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkEssential);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkHigh);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkNormal);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkLow);

        _ZFP_GI_instanceInit(d.dataLevelAppEssential);
        _ZFP_GI_instanceInit(d.dataLevelAppHigh);
        _ZFP_GI_instanceInit(d.dataLevelAppNormal);
        _ZFP_GI_instanceInit(d.dataLevelAppLow);

        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostLow);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostNormal);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostHigh);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostEssential);
        _ZFP_GI_instanceInit(d.dataLevelZFFrameworkPostStatic);
    }

    if(mutexAvailable) {
        ZFCoreMutexUnlock();
    }
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
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkPostStatic);
        d.stateZFFrameworkPostStatic = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostEssential");
        d.stateZFFrameworkPostEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostHigh");
        d.stateZFFrameworkPostHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostNormal");
        d.stateZFFrameworkPostNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkPostLow");
        d.stateZFFrameworkPostLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateNotAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppLow");
        d.stateAppLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelAppLow);
        d.stateAppLow = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppNormal");
        d.stateAppNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelAppNormal);
        d.stateAppNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppHigh");
        d.stateAppHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelAppHigh);
        d.stateAppHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelAppEssential");
        d.stateAppEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelAppEssential);
        d.stateAppEssential = ZFFrameworkStateNotAvailable;


        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkLow");
        d.stateZFFrameworkLow = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkNormal");
        d.stateZFFrameworkNormal = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkHigh");
        d.stateZFFrameworkHigh = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkEssential");
        d.stateZFFrameworkEssential = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateNotAvailable;

        _ZFP_ZFCoreGlobalInitializer_log("cleanup ZFLevelZFFrameworkStatic");
        d.stateZFFrameworkStatic = ZFFrameworkStateCleanupRunning;
        _ZFP_GI_instanceDealloc(d.dataLevelZFFrameworkStatic);
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
    ZFCoreArray<_ZFP_GI_Data *> &dataList = holder.dataListForLevel(level);
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);

    _ZFP_GI_Data *data = dataMap.get<_ZFP_GI_Data *>(name);
    if(data != zfnull) {
        ZFCoreAssert(level == data->level);
        ++(data->refCount);
    }
    else {
        data = zfpoolNew(_ZFP_GI_Data);
        data->name = name;
        data->level = level;
        data->constructor = constructor;
        data->destructor = destructor;

        dataList.add(data);
        dataMap.set(name, ZFCorePointerForPoolObject<_ZFP_GI_Data *>(data));
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
    ZFCoreArray<_ZFP_GI_Data *> &dataList = holder.dataListForLevel(level);
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);

    zfiter it = dataMap.iterFind(name);
    if(!it) {
        ZFCoreCriticalShouldNotGoHere();
        return;
    }
    _ZFP_GI_Data *data = dataMap.iterValue<_ZFP_GI_Data *>(it);
    --(data->refCount);
    if(data->refCount == 0) {
        for(zfindex i = 0; i < dataList.count(); ++i) {
            if(dataList[i] == data) {
                dataList.remove(i);
                break;
            }
        }
        dataMap.iterRemove(it);
    }
}

static void _ZFP_GI_notifyInstanceCreated(ZF_IN const _ZFP_GI_Data *data);
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
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);

    _ZFP_GI_Data *data = dataMap.get<_ZFP_GI_Data *>(name);
    if(data == zfnull) {
        ZFCoreCriticalShouldNotGoHere();
        return &dummy;
    }

    if(data->instance == zfnull) {
        _ZFP_ZFCoreGlobalInitializer_invokeTimeLogger("create %s", data->name.cString());
        data->instance = data->constructor();
        _ZFP_GI_notifyInstanceCreated(data);
    }

    return &(data->instance);
}

static const _ZFP_GI_Data *_ZFP_GI_dependencyCheck(ZFCoreArray<_ZFP_GI_Data *> &data) {
    for(zfindex i = 0; i < data.count(); ++i) {
        if(data[i]->instance == zfnull) {
            return data[i];
        }
    }
    return zfnull;
}
void _ZFP_GI_notifyInstanceCreated(ZF_IN const _ZFP_GI_Data *data) {
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkStatic) == ZFFrameworkStateNotAvailable) {
        ZFCoreCriticalMessageTrim(
                "ZFGlobalInitializer %s accessed before ZFFrameworkInit"
            , data->name);
        return;
    }

    _ZFP_GI_DataContainer &d = _ZFP_GI_dataContainerInstance;
    // check higher level initialized?
    const _ZFP_GI_Data *dependency = zfnull;
    do {
        if(data->level > ZFLevelZFFrameworkStatic) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkStatic)) != zfnull) {break;}
            if(data->level > ZFLevelZFFrameworkEssential) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkEssential)) != zfnull) {break;}
                if(data->level > ZFLevelZFFrameworkHigh) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkHigh)) != zfnull) {break;}
                    if(data->level > ZFLevelZFFrameworkNormal) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkNormal)) != zfnull) {break;}
                        if(data->level > ZFLevelZFFrameworkLow) {
                            if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkLow)) != zfnull) {break;}
                        }
                    }
                }
            }
        }
        if(data->level > ZFLevelAppEssential) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelAppEssential)) != zfnull) {break;}
            if(data->level > ZFLevelAppHigh) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelAppHigh)) != zfnull) {break;}
                if(data->level > ZFLevelAppNormal) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelAppNormal)) != zfnull) {break;}
                    if(data->level > ZFLevelAppLow) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelAppLow)) != zfnull) {break;}
                    }
                }
            }
        }
        if(data->level > ZFLevelZFFrameworkPostLow) {
            if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkPostLow)) != zfnull) {break;}
            if(data->level > ZFLevelZFFrameworkPostNormal) {
                if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkPostNormal)) != zfnull) {break;}
                if(data->level > ZFLevelZFFrameworkPostHigh) {
                    if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkPostHigh)) != zfnull) {break;}
                    if(data->level > ZFLevelZFFrameworkPostEssential) {
                        if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkPostEssential)) != zfnull) {break;}
                        if(data->level > ZFLevelZFFrameworkPostStatic) {
                            if((dependency = _ZFP_GI_dependencyCheck(d.dataLevelZFFrameworkPostStatic)) != zfnull) {break;}
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
            , data->name, dependency->name);
        return;
    }

    // reorder in same level
    ZFCoreArray<_ZFP_GI_Data *> &dataList = d.dataListForLevel(data->level);
    zfindex prevNull = zfindexMax();
    zfindex self = 0;
    for(zfindex i = 0; i < dataList.count(); ++i) {
        if(dataList[i] == data) {
            self = i;
        }
        else if(prevNull == zfindexMax() && dataList[i]->instance == zfnull) {
            prevNull = i;
        }
    }
    if(prevNull < self) {
        dataList.move(self, prevNull);
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

