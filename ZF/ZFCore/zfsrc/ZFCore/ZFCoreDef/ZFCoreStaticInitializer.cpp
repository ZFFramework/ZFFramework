#include "ZFCoreStaticInitializer.h"
#include "ZFCoreArray.h"
#include "ZFCoreMutex.h"
#include "ZFCorePointer.h"

// #define _ZFP_ZFCoreStaticInitializer_DEBUG 1

#if _ZFP_ZFCoreStaticInitializer_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFCoreStaticInitializer_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFSI] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFCoreStaticInitializer_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_SI_ItemData {
public:
    zfuint refCount;
    zfstring name;
    void *instance;
    _ZFP_SI_Destructor destructor;
public:
    _ZFP_SI_ItemData(void)
    : refCount(1)
    , name()
    , instance(zfnull)
    , destructor(zfnull)
    {
    }
    ~_ZFP_SI_ItemData(void) {
        if(this->instance != zfnull) {
            _ZFP_ZFCoreStaticInitializer_invokeTimeLogger("destroy: %s", name.cString());
            this->destructor(this->instance);
        }
    }
};

zfclassLikePOD _ZFP_SI_Data {
public:
    ZFCoreArray<_ZFP_SI_ItemData *> datas;
public:
    ~_ZFP_SI_Data(void) {
        ZFCoreArray<_ZFP_SI_ItemData *> tmp = datas;
        datas = ZFCoreArray<_ZFP_SI_ItemData *>();
        for(zfindex i = 0; i < tmp.count(); ++i) {
            zfdelete(tmp[i]);
        }
    }
public:
    void *instanceAccess(
            ZF_IN const zfstring &name
            , ZF_IN _ZFP_SI_Constructor constructor
            , ZF_IN _ZFP_SI_Destructor destructor
            ) {
        _ZFP_ZFCoreStaticInitializer_invokeTimeLogger("create: %s", name.cString());
        ZFCoreMutexLocker();
        for(zfindex i = 0; i < this->datas.count(); ++i) {
            if(this->datas[i]->name.compare(name) == 0) {
                ++(this->datas[i]->refCount);
                return this->datas[i]->instance;
            }
        }
        _ZFP_SI_ItemData *item = zfnew(_ZFP_SI_ItemData);
        item->name = name;
        item->instance = constructor();
        item->destructor = destructor;
        this->datas.add(item);
        return item->instance;
    }
    void instanceCleanup(ZF_IN void *instance) {
        ZFCoreMutexLocker();
        for(zfindex i = 0; i < this->datas.count(); ++i) {
            if(this->datas[i]->instance == instance) {
                --(this->datas[i]->refCount);
                if(this->datas[i]->refCount == 0) {
                    _ZFP_SI_ItemData *t = this->datas[i];
                    this->datas.remove(i);
                    zfdelete(t);
                    return;
                }
            }
        }
    }
};

static _ZFP_SI_Data &_ZFP_SI_data(void) {
    static _ZFP_SI_Data d;
    return d;
}

_ZFP_SI_Holder::_ZFP_SI_Holder(
        ZF_IN const zfstring &name
        , ZF_IN _ZFP_SI_Constructor constructor
        , ZF_IN _ZFP_SI_Destructor destructor
        )
: instance(_ZFP_SI_data().instanceAccess(name, constructor, destructor))
{
}
_ZFP_SI_Holder::~_ZFP_SI_Holder(void) {
    _ZFP_SI_data().instanceCleanup(this->instance);
}

ZF_NAMESPACE_GLOBAL_END

