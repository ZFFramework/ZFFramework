#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"
#include "ZFDynamicInvoker.h"

#include "ZFCore/ZFSTLWrapper/zfstlvector.h"
#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

// #define _ZFP_ZFClass_DEBUG 1

#if _ZFP_ZFClass_DEBUG
    #include "ZFCore/ZFCoreDef/zfimplLog.h"
    #define _ZFP_ZFClass_invokeTimeLogger(fmt, ...) \
        zfimplInvokeTimeLogger("[ZFCls] " fmt \
                , ##__VA_ARGS__ \
                )
#else
    #define _ZFP_ZFClass_invokeTimeLogger(fmt, ...)
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFClass's global data
zfclassNotPOD _ZFP_ZFClassDataHolder {
public:
    ZFCoreMap classMap; // <classNameFull, ZFClass *>
    ZFCoreMap delayDeleteMap; // <classNameFull, ZFClass *>
    /*
     * delay delete a class
     * ZFClass may be registered by different module,
     * while unloading a module,
     * _ZFP_ZFClassUnregister would be called to unload the class,
     * however, it may be called earlier than object instance's dealloc step,
     * so we cache the ZFClass object,
     * remove from the activating classMap,
     * and delete them all during ZFClassDataHolder's dealloc step
     *
     * this can't resolve the problem perfectly,
     * but did resolve most cases
     */
public:
    _ZFP_ZFClassDataHolder(void) {
        // dummy instance to ensure init order
        ZFSigName dummy(zftext("ZFObject"));
    }
    ~_ZFP_ZFClassDataHolder(void) {
        ZFCoreMap classMapTmp = this->classMap;
        ZFCoreMap delayDeleteMapTmp = this->delayDeleteMap;
        this->classMap = ZFCoreMap();
        this->delayDeleteMap = ZFCoreMap();

        delayDeleteMapTmp.removeAll();
        classMapTmp.removeAll();
    }
};
static _ZFP_ZFClassDataHolder &_ZFP_ZFClassData(void) {
    static _ZFP_ZFClassDataHolder d;
    return d;
}
#define _ZFP_ZFClassMap (_ZFP_ZFClassData().classMap)
#define _ZFP_ZFClassDelayDeleteMap (_ZFP_ZFClassData().delayDeleteMap)

// ============================================================
// _ZFP_ZFClassPrivate
typedef zfstlhashmap<zfstring, zfauto> _ZFP_ZFClassTagMapType;
typedef zfstlhashmap<const ZFProperty *, zfstlhashmap<const ZFClass *, zfbool> > _ZFP_ZFClassPropertyInitStepMapType;

typedef zfimplhashmap<ZFSigName, zfstlvector<const ZFMethod *> > _ZFP_ZFClassMethodMapType;

zfclassNotPOD _ZFP_ZFClassPrivate {
public:
    ZFClass *pimplOwner;
    zfuint refCount;

    zfbool needAutoRegister;
    zfbool needRegisterImplementedInterface;
    zfbool needFinalInit;

    zfbool classIsDynamicRegister;
    zfauto classDynamicRegisterUserData;
    zfstlhashmap<ZFObject *, zfbool> classDynamicRegisterObjectInstanceMap;
    _ZFP_zfAllocCacheCallback objectAllocWithCacheCallback;
    _ZFP_ZFObjectConstructor constructor;
    _ZFP_ZFObjectDestructor destructor;

public:
    ZFCoreArray<const ZFClass *> implementedInterface;
    ZFCoreArray<const ZFClass *> ZFImplementDynamicList; // for ZFImplementDynamicRegister
    _ZFP_ZFClassMethodMapType methodMap; // method of this cls only
    ZFCoreOrderMap propertyMap; // property of this cls only
    /*
     * store all property that has override parent's OnInit step by #ZFPROPERTY_ON_INIT_DECLARE
     * including self and all parent
     *
     * note, only stored when subclass override parent's OnInit,
     * not applied when the property's direct owner declared custom OnInit
     *
     * during owner object's allocation,
     * all of these property would have their getter being called
     * to suit most logic case
     */
    zfstlhashmap<const ZFProperty *, zfbool> propertyAutoInitMap;
    /*
     * see propertyAutoInitMap,
     * used to check whether two class has same property init step
     */
    _ZFP_ZFClassPropertyInitStepMapType propertyInitStepMap;

public:
    _ZFP_ZFClassTagMapType classTagMap;
    _ZFP_ZFClassTagMapType dataCacheMap;

public:
    zfstlhashmap<const ZFClass *, zfbool> allParent; // all parent and interface excluding self
    zfstlhashmap<const ZFClass *, zfbool> allChildren; // all children excluding self

public:
    zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback> interfaceCastMap; // self implemented interface

    // ============================================================
    // instance observer
public:
    zfclassLikePOD InstanceObserverData {
    public:
        ZFListener observer;
        ZFObject *owner;
        ZFLevel observerLevel;
    };
    ZFCoreArray<_ZFP_ZFClassPrivate::InstanceObserverData *> instanceObserver;
    ZFCoreArray<_ZFP_ZFClassPrivate::InstanceObserverData *> instanceObserverCached; // including all parent type's observer

public:
    void _instanceObserverDoAdd(
            ZF_IN_OUT ZFCoreArray<_ZFP_ZFClassPrivate::InstanceObserverData *> &buf
            , ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data
            ) {
        zfindex index = buf.count();
        while(index > 0 && buf[index - 1]->observerLevel > data->observerLevel) {
            --index;
        }
        buf.add(data, index);
    }
    void instanceObserverDoAdd(
            ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data
            , ZF_IN zfbool observeAllChildType
            ) {
        this->_instanceObserverDoAdd(this->instanceObserverCached, data);
        if(!observeAllChildType) {
            return;
        }

        for(zfstlhashmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin(); childIt != this->allChildren.end(); ++childIt) {
            if(!childIt->first->classIsInternalPrivate()) {
                this->_instanceObserverDoAdd(childIt->first->d->instanceObserverCached, data);
            }
        }
    }
    void instanceObserverDoRemove(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data) {
        const ZFClass *cls = this->pimplOwner;
        zfstlhashmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin();
        do {
            for(zfindex i = 0; i < cls->d->instanceObserverCached.count(); ++i) {
                if(cls->d->instanceObserverCached[i] == data) {
                    cls->d->instanceObserverCached.remove(i);
                    break;
                }
            }

            if(childIt != this->allChildren.end()) {
                cls = childIt->first;
                ++childIt;
            }
            else {
                break;
            }
        } while(zftrue);
    }

public:
    ZFObject *objectConstruct(void) {
        if(this->constructor) {
            ZFObject *obj = this->constructor();
            if(this->pimplOwner->classIsDynamicRegister()) {
                obj->_ZFP_ZFObject_classDynamic = this->pimplOwner;
                this->classDynamicRegisterObjectInstanceMap[obj] = zftrue;
            }
            return obj;
        }
        else {
            return zfnull;
        }
    }

public:
    _ZFP_ZFClassPrivate(void)
    : pimplOwner(zfnull)
    , refCount(1)
    , needAutoRegister(zftrue)
    , needRegisterImplementedInterface(zftrue)
    , needFinalInit(zftrue)
    , classIsDynamicRegister(zffalse)
    , classDynamicRegisterUserData()
    , classDynamicRegisterObjectInstanceMap()
    , constructor(zfnull)
    , destructor(zfnull)
    , implementedInterface()
    , ZFImplementDynamicList()
    , methodMap()
    , propertyMap()
    , propertyAutoInitMap()
    , propertyInitStepMap()
    , classTagMap()
    , dataCacheMap()
    , allParent()
    , allChildren()
    , interfaceCastMap()
    , instanceObserver()
    , instanceObserverCached()
    , parentClassCache()
    , parentTypeCache()
    , ZFImplementDynamicCache()
    , interfaceCastCache()
    , methodMapCache()
    , propertyMapCache()
    {
    }

public:
    static void classInitFinish(ZF_IN ZFClass *cls);

    // ============================================================
    // caches
public:
    zfstlhashmap<const ZFClass *, zfbool> parentClassCache; // for classIsTypeOf(class), all parent and ZFImplementDynamicList including self
    zfstlhashmap<const ZFClass *, zfbool> parentTypeCache; // for classIsTypeOf(interface), all parent and interface and ZFImplementDynamicList, including self
    zfstlhashmap<const ZFClass *, zfbool> ZFImplementDynamicCache; /// for ZFImplementDynamicRegister, all ZFImplementDynamicList of parent
    zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback> interfaceCastCache; // including all parent

    _ZFP_ZFClassMethodMapType methodMapCache; // method of this cls and all parent, order ensured from self > parent > parent interface
    ZFCoreOrderMap propertyMapCache; // property of this cls and all parent

    static void classParentCacheUpdate(ZF_IN const ZFClass *cls);
    static void methodAndPropertyCacheUpdate(ZF_IN const ZFClass *cls);
    static void methodCacheRemove(ZF_IN const ZFClass *cls, ZF_IN const ZFMethod *method);
    static void methodCacheRemoveAction(ZF_IN const ZFClass *cls, ZF_IN const ZFMethod *method);
    static void propertyCacheRemove(ZF_IN const ZFClass *cls, ZF_IN const ZFProperty *zfproperty);
    static void propertyCacheRemoveAction(ZF_IN const ZFClass *cls, ZF_IN const ZFProperty *zfproperty);
};

void _ZFP_ZFClassPrivate::classInitFinish(ZF_IN ZFClass *cls) {
    { // copy parent's interface cast datas
        if(cls->classParent() != zfnull) {
            zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback> &m = cls->classParent()->d->interfaceCastMap;
            for(zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback>::iterator it = m.begin(); it != m.end(); ++it) {
                cls->d->interfaceCastMap[it->first] = it->second;
            }
        }
    }

    { // all parent and children
        ZFCoreQueuePOD<const ZFClass *> clsToCheck;
        clsToCheck.add(cls);
        do {
            const ZFClass *clsTmp = clsToCheck.take();
            if(cls->d->allParent.find(clsTmp) == cls->d->allParent.end()) {
                if(clsTmp != cls) {
                    cls->d->allParent[clsTmp] = zftrue;
                    clsTmp->d->allChildren[cls] = zftrue;
                }

                if(clsTmp->classParent() != zfnull) {
                    clsToCheck.add(clsTmp->classParent());
                }
                clsToCheck.addFrom(clsTmp->d->implementedInterface);
            }
        } while(!clsToCheck.isEmpty());
    }

    { // property init step
        for(zfstlhashmap<const ZFClass *, zfbool>::iterator itParent = cls->d->allParent.begin();
                itParent != cls->d->allParent.end();
                ++itParent
                ) {
            cls->d->propertyAutoInitMap.insert(itParent->first->d->propertyAutoInitMap.begin(), itParent->first->d->propertyAutoInitMap.end());

            for(_ZFP_ZFClassPropertyInitStepMapType::iterator itProperty = itParent->first->d->propertyInitStepMap.begin();
                    itProperty != itParent->first->d->propertyInitStepMap.end();
                    ++itProperty
                    ) {
                cls->d->propertyInitStepMap[itProperty->first].insert(itProperty->second.begin(), itProperty->second.end());
            }
        }
    }

    { // copy all parent's instance observer to self
        for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = cls->d->allParent.begin(); it != cls->d->allParent.end(); ++it) {
            ZFCoreArray<_ZFP_ZFClassPrivate::InstanceObserverData *> &parentInstanceObserver = it->first->d->instanceObserver;
            for(zfindex i = 0; i < parentInstanceObserver.count(); ++i) {
                cls->d->instanceObserverCached.add(parentInstanceObserver[i]);
            }
        }
    }

    // cache
    classParentCacheUpdate(cls);
}

void _ZFP_ZFClassPrivate::classParentCacheUpdate(ZF_IN const ZFClass *cls) {
    zfstlhashmap<const ZFClass *, zfbool> &parentClassCache = cls->d->parentClassCache;
    zfstlhashmap<const ZFClass *, zfbool> &parentTypeCache = cls->d->parentTypeCache;
    zfstlhashmap<const ZFClass *, zfbool> &ZFImplementDynamicCache = cls->d->ZFImplementDynamicCache;
    zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback> &interfaceCastCache = cls->d->interfaceCastCache;
    parentClassCache.clear();
    parentTypeCache.clear();
    ZFImplementDynamicCache.clear();
    interfaceCastCache.clear();

    if(!cls->classIsInterface()) {
        const ZFClass *t = cls;
        do {
            parentClassCache[t] = zftrue;
            t = t->classParent();
        } while(t != zfnull);
    }

    ZFCoreQueuePOD<const ZFClass *> toCheck;
    toCheck.add(cls);
    do {
        const ZFClass *t = toCheck.take();
        if(t->classParent() != zfnull) {
            toCheck.add(t->classParent());
        }
        toCheck.addFrom(t->d->implementedInterface);

        for(zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback>::iterator itCast = t->d->interfaceCastMap.begin(); itCast != t->d->interfaceCastMap.end(); ++itCast) {
            interfaceCastCache[itCast->first] = itCast->second;
        }

        parentTypeCache[t] = zftrue;
        parentClassCache[t] = zftrue;

        for(zfindex i = 0; i < t->d->ZFImplementDynamicList.count(); ++i) {
            const ZFClass *dyn = t->d->ZFImplementDynamicList[i];
            parentClassCache[dyn] = zftrue;
            parentTypeCache[dyn] = zftrue;
            ZFImplementDynamicCache[dyn] = zftrue;
        }
    } while(!toCheck.isEmpty());
}

void _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(ZF_IN const ZFClass *cls) {
    if(!cls->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate
            || ZFFrameworkStateCheck() == ZFFrameworkStateCleanupRunning
            || ZFFrameworkStateCheck() == ZFFrameworkStateNotAvailable
            ) {
        return;
    }
    ZFCoreMutexLocker();
    if(!cls->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate) {
        return;
    }
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zffalse;
    cls->_ZFP_ZFClass_autoRegister();
    _ZFP_ZFClassMethodMapType &methodMapCache = cls->d->methodMapCache;
    ZFCoreOrderMap &propertyMapCache = cls->d->propertyMapCache;
    methodMapCache.clear();
    propertyMapCache.removeAll();

    ZFCoreQueuePOD<const ZFClass *> toCheck;
    toCheck.add(cls);
    do {
        const ZFClass *t = toCheck.take();
        if(t->classParent() != zfnull) {
            toCheck.add(t->classParent());
        }
        toCheck.addFrom(t->d->implementedInterface);
        toCheck.addFrom(t->d->ZFImplementDynamicList);

        for(_ZFP_ZFClassMethodMapType::iterator it = t->d->methodMap.begin(); it != t->d->methodMap.end(); ++it) {
            zfstlvector<const ZFMethod *> &methodList = methodMapCache[it->first];
            for(zfstlsize i = 0; i < it->second.size(); ++i) {
                const ZFMethod *m = it->second[i];
                zfbool exist = zffalse;
                for(zfstlsize j = 0; j < methodList.size(); ++j) {
                    if(methodList[j] == m) {
                        exist = zftrue;
                        break;
                    }
                }
                if(!exist) {
                    methodList.push_back(m);
                }
            }
        }
        for(zfiter it = t->d->propertyMap.iter(); it; ++it) {
            propertyMapCache.set(t->d->propertyMap.iterKey(it), *(t->d->propertyMap.iterValue(it)));
        }
    } while(!toCheck.isEmpty());
}

void _ZFP_ZFClassPrivate::methodCacheRemove(ZF_IN const ZFClass *cls, ZF_IN const ZFMethod *method) {
    methodCacheRemoveAction(cls, method);
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator childIt = cls->d->allChildren.begin(); childIt != cls->d->allChildren.end(); ++childIt) {
        methodCacheRemoveAction(childIt->first, method);
    }
}
void _ZFP_ZFClassPrivate::methodCacheRemoveAction(ZF_IN const ZFClass *cls, ZF_IN const ZFMethod *method) {
    _ZFP_ZFClassMethodMapType::iterator itName = cls->d->methodMapCache.find(method->methodName());
    if(itName != cls->d->methodMapCache.end()) {
        zfstlvector<const ZFMethod *> &l = itName->second;
        for(zfstlvector<const ZFMethod *>::iterator itList = l.begin(); itList != l.end(); ++itList) {
            if(method == *itList) {
                l.erase(itList);
                if(l.empty()) {
                    cls->d->methodMapCache.erase(itName);
                }
                break;
            }
        }
    }
}
void _ZFP_ZFClassPrivate::propertyCacheRemove(ZF_IN const ZFClass *cls, ZF_IN const ZFProperty *zfproperty) {
    propertyCacheRemoveAction(cls, zfproperty);
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator childIt = cls->d->allChildren.begin(); childIt != cls->d->allChildren.end(); ++childIt) {
        propertyCacheRemoveAction(childIt->first, zfproperty);
    }
}
void _ZFP_ZFClassPrivate::propertyCacheRemoveAction(ZF_IN const ZFClass *cls, ZF_IN const ZFProperty *zfproperty) {
    cls->d->propertyMapCache.remove(zfproperty->propertyName());
}

// ============================================================
// clear class tag both in ZFLevelZFFrameworkEssential and ZFLevelZFFrameworkHigh
static void _ZFP_ZFClass_classTagClear(void) {
    ZFCoreArray<const ZFClass *> allClass;
    _ZFP_ZFClassMap.allValueT(allClass);
    for(zfindex i = 0; i < allClass.count(); ++i) {
        allClass.get(i)->classTagRemoveAll();
        allClass.get(i)->dataCacheRemoveAll();
    }
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelEssential, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelEssential) {
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelEssential)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelHigh, ZFLevelZFFrameworkHigh) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelHigh) {
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelHigh)

// ============================================================
// static methods
const ZFClass *ZFClass::classForName(ZF_IN const zfstring &className) {
    if(className == zfnull) {
        return zfnull;
    }
    else {
        return _ZFP_ZFClassMap.get<const ZFClass *>(ZFNamespaceSkipGlobal(className));
    }
}
const ZFClass *ZFClass::classForName(
        ZF_IN const zfstring &className
        , ZF_IN const zfstring &classNamespace
        ) {
    const zfchar *classNamespaceTmp = ZFNamespaceSkipGlobal(classNamespace);
    if(classNamespaceTmp) {
        zfstring classNameFull;
        classNameFull += classNamespaceTmp;
        classNameFull += ".";
        classNameFull += className;
        return _ZFP_ZFClassMap.get<const ZFClass *>(classNameFull);
    }
    else {
        return _ZFP_ZFClassMap.get<const ZFClass *>(ZFNamespaceSkipGlobal(className));
    }
}

// ============================================================
// instance observer
void ZFClass::instanceObserverAdd(
        ZF_IN const ZFListener &observer
        , ZF_IN_OPT ZFObject *owner /* = zfnull */
        , ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */
        , ZF_IN_OPT zfbool observeAllChildType /* = zftrue */
        ) const {
    if(this->classIsInternalPrivate()) {
        return;
    }

    ZFCoreMutexLocker();
    _ZFP_ZFClassPrivate::InstanceObserverData *data = zfpoolNew(_ZFP_ZFClassPrivate::InstanceObserverData);
    data->observer = observer;
    data->owner = owner;
    data->observerLevel = observerLevel;
    d->instanceObserver.add(data);
    d->instanceObserverDoAdd(data, observeAllChildType);
}
void ZFClass::instanceObserverRemove(ZF_IN const ZFListener &observer) const {
    ZFCoreMutexLocker();
    for(zfindex i = 0; i < d->instanceObserver.count(); ++i) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->observer == observer) {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.remove(i);
            zfpoolDelete(data);
            break;
        }
    }
}
void ZFClass::instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const {
    ZFCoreMutexLocker();
    for(zfindex i = 0; i < d->instanceObserver.count(); ++i) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->owner == owner) {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.remove(i);
            --i;
            zfpoolDelete(data);
        }
    }
}
void ZFClass::instanceObserverRemoveAll(void) const {
    ZFCoreMutexLocker();
    while(!d->instanceObserver.isEmpty()) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver.getLast();
        d->instanceObserverDoRemove(data);
        d->instanceObserver.removeLast();
        zfpoolDelete(data);
    }
}
void ZFClass::_ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const {
    if(!d->instanceObserverCached.isEmpty()) {
        ZFArgs zfargs;
        zfargs
            .eventId(ZFObject::E_ObjectAfterAlloc())
            .sender(obj)
            ;
        for(zfindex i = 0; i < d->instanceObserverCached.count() && !zfargs.eventFiltered(); ++i) {
            _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserverCached[i];
            data->observer.execute(zfargs);
        }
    }
}

// ============================================================
// class info
static void _ZFP_ZFClassInfo(
        ZF_IN_OUT zfstring &s
        , ZF_IN const ZFClass *cls
        ) {
    if(cls->classIsAbstract() && !cls->classIsInterface()) {
        s += "(abstract)";
    }
    s += cls->classNameFull();
}
void ZFClass::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    _ZFP_ZFClassInfo(ret, this);
    zfbool first = zftrue;
    for(zfindex i = 0; i < this->implementedInterfaceCount(); ++i) {
        if(first) {
            first = zffalse;
            ret += '<';
        }
        else {
            ret += ", ";
        }
        ret += this->implementedInterfaceAt(i)->classNameFull();
    }
    for(zfindex i = 0; i < this->dynamicInterfaceCount(); ++i) {
        if(first) {
            first = zffalse;
            ret += '<';
        }
        else {
            ret += ", ";
        }
        ret += this->dynamicInterfaceAt(i)->classNameFull();
    }
    if(!first) {
        ret += '>';
    }
}
void ZFClass::objectInfoOfInheritTreeT(ZF_IN_OUT zfstring &ret) const {
    const ZFClass *cls = this;
    while(cls != zfnull) {
        if(cls != this) {
            if(cls == ZFInterface::ClassData()) {
                break;
            }
            ret += " : ";
        }

        _ZFP_ZFClassInfo(ret, cls);

        zfbool first = zftrue;
        for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
            if(first) {
                first = zffalse;
                ret += '<';
            }
            else {
                ret += ", ";
            }
            cls->implementedInterfaceAt(i)->objectInfoOfInheritTreeT(ret);
        }
        for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
            if(first) {
                first = zffalse;
                ret += '<';
            }
            else {
                ret += ", ";
            }
            ret += cls->dynamicInterfaceAt(i)->classNameFull();
        }
        if(!first) {
            ret += '>';
        }

        cls = cls->classParent();
    }
}

zfbool ZFClass::classIsTypeOf(ZF_IN const ZFClass *cls) const {
    if(this == cls) {
        return zftrue;
    }
    else {
        zfstlhashmap<const ZFClass *, zfbool> &p = (cls->classIsInterface() ? d->parentTypeCache : d->parentClassCache);
        return p.find(cls) != p.end();
    }
}

zfbool ZFClass::classIsDynamicRegister(void) const {
    return d->classIsDynamicRegister;
}
zfany ZFClass::classDynamicRegisterUserData(void) const {
    return d->classDynamicRegisterUserData;
}

zfauto ZFClass::newInstance(void) const {
    ZFCoreMutexLocker();
    ZFObject *obj = zfnull;
    if(d->objectAllocWithCacheCallback) {
        obj = d->objectAllocWithCacheCallback();
    }
    else {
        obj = d->objectConstruct();
        if(obj != zfnull) {
            obj->_ZFP_ZFObject_objectOnInit();
            obj->_ZFP_ZFObject_objectOnInitFinish();
        }
    }
    zfauto ret;
    ret.zfunsafe_assign(obj);
    zfunsafe_zfRelease(obj);
    return ret;
}
zfauto ZFClass::_ZFP_ZFClass_newInstance(ZF_IN _ZFP_ZFObjectPrivate *dObj) const {
    ZFCoreMutexLocker();
    ZFObject *obj = zfnull;
    if(d->objectAllocWithCacheCallback) {
        obj = d->objectAllocWithCacheCallback();
    }
    else {
        obj = d->objectConstruct();
        if(obj != zfnull) {
            obj->d = dObj;
            obj->_ZFP_ZFObject_objectOnInit();
            obj->_ZFP_ZFObject_objectOnInitFinish();
        }
    }
    zfauto ret;
    ret.zfunsafe_assign(obj);
    zfunsafe_zfRelease(obj);
    return ret;
}

/* ZFMETHOD_MAX_PARAM */
zfauto ZFClass::newInstance(
        ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMP_DEF() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMP_DEF() */
        ) const {
    ZFArgs zfargs;
    zfargs
        .paramInit(
                param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
                )
        ;
    ZFDI_alloc(zfargs, this);
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        return zfnull;
    }
}
zfauto ZFClass::newInstanceDetail(
        ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success /* = zfnull */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) const {
    ZFArgs zfargs;
    zfargs
        .paramInit(params)
        .ignoreErrorEvent(errorHint != zfnull)
        ;
    ZFDI_alloc(zfargs, this);
    if(success != zfnull) {
        *success = zfargs.success();
    }
    if(zfargs.success()) {
        return zfargs.result();
    }
    else {
        if(errorHint != zfnull) {
            *errorHint += zfargs.errorHint();
        }
        return zfnull;
    }
}

void *ZFClass::newInstanceGenericBegin(void) const {
    return d->objectConstruct();
}
zfbool ZFClass::newInstanceGenericCheck(
        ZF_IN void *&token
        , ZF_IN const ZFMethod *objectOnInitMethod
        , ZF_IN_OUT const ZFArgs &zfargs
        ) const {
    if(objectOnInitMethod == zfnull
            // || !this->classIsTypeOf(objectOnInitMethod->ownerClass())
            // || !zfstringIsEqual(objectOnInitMethod->methodName(), "objectOnInit")
            ) {
        return zffalse;
    }

    ZFObject *obj = (ZFObject *)token;
    obj->_ZFP_ZFObject_objectOnInit();

    zfargs._ZFP_ZFArgs_removeConst()
        .sender(obj)
        .ownerMethod(objectOnInitMethod)
        .success(zftrue)
        .errorHint(zfnull)
        ;
    objectOnInitMethod->methodGenericInvoker()(zfargs);
    zfbool success = zfargs.success();

    if(obj->d) {
        if(success && obj->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed) != zfnull) {
            success = zffalse;
            if(!zfargs.ignoreError()) {
                v_zfstring *error = obj->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed);
                if(error != zfnull) {
                    zfargs.errorHint(error->zfv);
                }
            }
            // remove for safety
            obj->objectTagRemove(ZFObjectTagKeyword_newInstanceGenericFailed);
        }

        if(!success) {
            // since objectOnInit already called,
            // we must ensure init and destroy the object,
            // then recreate the token
            obj->_ZFP_ZFObject_objectOnInitFinish();
            zfunsafe_zfRelease(obj);
            token = d->objectConstruct();
        }
    }
    return success;
}
zfauto ZFClass::newInstanceGenericEnd(
        ZF_IN void *&token
        , ZF_IN zfbool objectOnInitMethodInvokeSuccess
        ) const {
    ZFObject *obj = (ZFObject *)token;
    if(objectOnInitMethodInvokeSuccess) {
        obj->_ZFP_ZFObject_objectOnInitFinish();
        zfunsafe_zfblockedRelease(obj);
        return obj;
    }
    else {
        d->destructor(obj);
        return zfnull;
    }
}

zfindex ZFClass::implementedInterfaceCount(void) const {
    return d->implementedInterface.count();
}
const ZFClass *ZFClass::implementedInterfaceAt(ZF_IN zfindex index) const {
    return d->implementedInterface.get(index);
}
zfindex ZFClass::dynamicInterfaceCount(void) const {
    return d->ZFImplementDynamicList.count();
}
const ZFClass *ZFClass::dynamicInterfaceAt(ZF_IN zfindex index) const {
    return d->ZFImplementDynamicList.get(index);
}
zfbool ZFClass::dynamicImplementOf(ZF_IN const ZFClass *parent) const {
    return d->ZFImplementDynamicCache.find(parent) != d->ZFImplementDynamicCache.end();
}

ZFCoreArray<const ZFClass *> ZFClass::childGetAll(void) const {
    ZFCoreArray<const ZFClass *> ret;
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        ret.add(it->first);
    }
    return ret;
}
ZFCoreArray<const ZFClass *> ZFClass::parentGetAll(void) const {
    ZFCoreArray<const ZFClass *> ret;
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->parentTypeCache.begin(); it != d->parentTypeCache.end(); ++it) {
        if(it->first != this) {
            ret.add(it->first);
        }
    }
    return ret;
}

// ============================================================
// ZFMethod
zfindex ZFClass::methodCount(void) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    zfindex ret = 0;
    for(zfiter it = d->methodMap.iter(); it; ++it) {
        zfstlvector<const ZFMethod *> const &l = d->methodMap.iterValue(it);
        ret += (zfindex)l.size();
    }
    return ret;
}
zfclassNotPOD _ZFP_ZFClassMethodIter : zfextend zfiter::Impl {
public:
    _ZFP_ZFClassPrivate *owner;
    zfiter mapIt;
    zfstlsize listIndex;
public:
    zfoverride
    virtual zfbool valid(void) {
        return this->mapIt;
    }
    zfoverride
    virtual void next(void) {
        ++(this->listIndex);
        if(this->listIndex >= owner->methodMap.iterValue(this->mapIt).size()) {
            this->listIndex = 0;
            ++(this->mapIt);
        }
    }
    zfoverride
    virtual Impl *copy(void) {
        _ZFP_ZFClassMethodIter *ret = zfpoolNew(_ZFP_ZFClassMethodIter);
        ret->owner = this->owner;
        ret->mapIt = this->mapIt;
        ret->listIndex = this->listIndex;
        return ret;
    }
    zfoverride
    virtual void destroy(void) {
        zfpoolDelete(this);
    }
    zfoverride
    virtual zfbool isEqual(ZF_IN Impl *d) {
        _ZFP_ZFClassMethodIter *t = (_ZFP_ZFClassMethodIter *)d;
        return owner == t->owner
            && this->mapIt == t->mapIt
            && this->listIndex == t->listIndex
            ;
    }
};
zfiter ZFClass::methodIter(void) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    _ZFP_ZFClassMethodIter *impl = zfpoolNew(_ZFP_ZFClassMethodIter);
    impl->owner = d;
    impl->mapIt = d->methodMap.iter();
    impl->listIndex = 0;
    return zfiter(impl);
}
const ZFMethod *ZFClass::methodIterValue(ZF_IN const zfiter &it) const {
    _ZFP_ZFClassMethodIter *impl = it.impl<_ZFP_ZFClassMethodIter *>();
    return d->methodMap.iterValue(impl->mapIt)[impl->listIndex];
}

void ZFClass::methodGetAllIgnoreParentT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(_ZFP_ZFClassMethodMapType::iterator it = d->methodMap.begin(); it != d->methodMap.end(); ++it) {
        ret.addFrom(&it->second[0], (zfindex)it->second.size());
    }
}

void ZFClass::methodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(_ZFP_ZFClassMethodMapType::iterator it = d->methodMapCache.begin(); it != d->methodMapCache.end(); ++it) {
        ret.addFrom(&it->second[0], (zfindex)it->second.size());
    }
}

/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFClass::methodForNameIgnoreParent(
        ZF_IN const zfstring &methodName
        , ZF_IN const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramTypeIdIsMatch(
                            paramTypeId0
                            , paramTypeId1
                            , paramTypeId2
                            , paramTypeId3
                            , paramTypeId4
                            , paramTypeId5
                            , paramTypeId6
                            , paramTypeId7
                            )) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForNameIgnoreParent(ZF_IN const zfstring &methodName) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> const &l = itName->second;
            if(l.size() > 1) {
                for(zfstlsize i = 0; i < l.size(); ++i) {
                    if(l[i]->paramCountMin() == 0) {
                        return l[i];
                    }
                }
            }
            return l[0];
        }
    }
    return zfnull;
}
void ZFClass::methodForNameGetAllIgnoreParentT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfstring &methodName
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end()) {
            ret.addFrom(&(itName->second[0]), (zfindex)itName->second.size());
        }
    }
}
const ZFMethod *ZFClass::methodForName(
        ZF_IN const zfstring &methodName
        , ZF_IN const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *paramTypeId7 /* = zfnull */
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(methodName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramTypeIdIsMatch(
                            paramTypeId0
                            , paramTypeId1
                            , paramTypeId2
                            , paramTypeId3
                            , paramTypeId4
                            , paramTypeId5
                            , paramTypeId6
                            , paramTypeId7
                            )) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForName(ZF_IN const zfstring &methodName) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(methodName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> const &l = itName->second;
            if(l.size() > 1) {
                for(zfstlsize i = 0; i < l.size(); ++i) {
                    if(l[i]->paramCountMin() == 0) {
                        return l[i];
                    }
                }
            }
            return l[0];
        }
    }
    return zfnull;
}
void ZFClass::methodForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfstring &methodName
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(methodName);
        if(itName != d->methodMapCache.end()) {
            ret.addFrom(&(itName->second[0]), (zfindex)itName->second.size());
        }
    }
}

// ============================================================
// ZFProperty
zfindex ZFClass::propertyCount(void) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    return d->propertyMap.count();
}
zfiter ZFClass::propertyIter(void) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    return d->propertyMap.iter();
}
const ZFProperty *ZFClass::propertyIterValue(ZF_IN const zfiter &it) const {
    return d->propertyMap.iterValue<const ZFProperty *>(it);
}

void ZFClass::propertyGetAllIgnoreParentT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(zfiter it = d->propertyMap.iter(); it; ++it) {
        ret.add(d->propertyMap.iterValue<const ZFProperty *>(it));
    }
}

void ZFClass::propertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(zfiter it = d->propertyMapCache.iter(); it; ++it) {
        ret.add(d->propertyMapCache.iterValue<const ZFProperty *>(it));
    }
}

const ZFProperty *ZFClass::propertyForNameIgnoreParent(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        zfiter it = d->propertyMap.iterFind(propertyName);
        if(it) {
            return d->propertyMap.iterValue<const ZFProperty *>(it);
        }
    }
    return zfnull;
}
const ZFProperty *ZFClass::propertyForName(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        if(propertyName != zfnull) {
            zfiter it = d->propertyMapCache.iterFind(propertyName);
            if(it) {
                return d->propertyMapCache.iterValue<const ZFProperty *>(it);
            }
        }
    }
    return zfnull;
}

const ZFMethod *ZFClass::propertySetterForNameIgnoreParent(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramCountMin() == 1) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertySetterForName(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(propertyName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramCountMin() == 1) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForNameIgnoreParent(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramCountMin() == 0) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForName(ZF_IN const zfstring &propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(propertyName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->paramCountMin() == 0) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}

zfbool ZFClass::propertyInitStepExist(void) const {
    return !(d->propertyInitStepMap.empty());
}
zfbool ZFClass::propertyInitStepExist(ZF_IN const ZFProperty *property) const {
    return (d->propertyInitStepMap.find(property) != d->propertyInitStepMap.end());
}
zfbool ZFClass::propertyInitStepIsEqual(
        ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *refClass
        ) const {
    if(this == refClass) {
        return zftrue;
    }
    _ZFP_ZFClassPropertyInitStepMapType::iterator data = d->propertyInitStepMap.find(property);
    _ZFP_ZFClassPropertyInitStepMapType::iterator dataRef = refClass->d->propertyInitStepMap.find(property);
    if(data == d->propertyInitStepMap.end()) {
        return (dataRef == refClass->d->propertyInitStepMap.end());
    }
    if(dataRef == refClass->d->propertyInitStepMap.end()) {
        return (data == d->propertyInitStepMap.end());
    }

    if(data->second.size() != dataRef->second.size()) {
        return zffalse;
    }
    if(data->second.size() == 0) {
        return zftrue;
    }
    if(this->classIsTypeOf(refClass) || refClass->classIsTypeOf(this)) {
        return zftrue;
    }

    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = data->second.begin(), itRef = dataRef->second.begin();
            it != data->second.end();
            ++it, ++itRef
            ) {
        if(*it != *itRef) {
            return zffalse;
        }
    }
    return zftrue;
}

// ============================================================
// class instance methods
void ZFClass::classTag(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) const {
    if(key == zfnull) {
        return;
    }
    if(tag != zfnull && ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) != ZFFrameworkStateAvailable) {
        // must not store any new tags during cleanup
        abort();
    }
    ZFCoreMutexLocker();
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    _ZFP_ZFClassTagMapType::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag != zfnull) {
            m[key] = tag;
        }
    }
    else {
        ZFObject *obj = zfunsafe_zfRetain(it->second);
        if(tag == zfnull) {
            m.erase(it);
        }
        else {
            it->second.zfunsafe_assign(tag);
        }
        zfunsafe_zfRelease(obj);
    }
}
zfany ZFClass::classTag(ZF_IN const zfstring &key) const {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_ZFClassTagMapType::iterator it = d->classTagMap.find(key);
        if(it != d->classTagMap.end()) {
            return it->second;
        }
    }
    return zfnull;
}
void ZFClass::classTagGetAllKeyValue(
        ZF_IN_OUT ZFCoreArray<zfstring> &allKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
        ) const {
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFClassTagMapType::iterator it = m.begin(); it != m.end(); ++it) {
        allKey.add(it->first);
        allValue.add(it->second);
    }
}
zfauto ZFClass::classTagRemoveAndGet(ZF_IN const zfstring &key) const {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_ZFClassTagMapType &m = d->classTagMap;
        _ZFP_ZFClassTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            zfauto ret;
            ret.zfunsafe_assign(it->second);
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFClass::classTagRemoveAll(void) const {
    if(!d->classTagMap.empty()) {
        _ZFP_ZFClassTagMapType tmp;
        tmp.swap(d->classTagMap);
    }
}

// ============================================================
void ZFClass::dataCache(
        ZF_IN const zfstring &key
        , ZF_IN ZFObject *tag
        ) const {
    if(key == zfnull) {
        return;
    }
    if(tag != zfnull && ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) != ZFFrameworkStateAvailable) {
        // must not store any new tags during cleanup
        abort();
    }
    ZFCoreMutexLocker();
    _ZFP_ZFClassTagMapType &m = d->dataCacheMap;
    _ZFP_ZFClassTagMapType::iterator it = m.find(key);
    if(it == m.end()) {
        if(tag != zfnull) {
            m[key] = tag;
        }
    }
    else {
        ZFObject *obj = zfunsafe_zfRetain(it->second);
        if(tag == zfnull) {
            m.erase(it);
        }
        else {
            it->second.zfunsafe_assign(tag);
        }
        zfunsafe_zfRelease(obj);
    }
}
zfany ZFClass::dataCache(ZF_IN const zfstring &key) const {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_ZFClassTagMapType::iterator it = d->dataCacheMap.find(key);
        if(it != d->dataCacheMap.end()) {
            return it->second;
        }
    }
    return zfnull;
}
void ZFClass::dataCacheGetAllKeyValue(
        ZF_IN_OUT ZFCoreArray<zfstring> &allKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
        ) const {
    _ZFP_ZFClassTagMapType &m = d->dataCacheMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFClassTagMapType::iterator it = m.begin(); it != m.end(); ++it) {
        allKey.add(it->first);
        allValue.add(it->second);
    }
}
zfauto ZFClass::dataCacheRemoveAndGet(ZF_IN const zfstring &key) const {
    if(key != zfnull) {
        ZFCoreMutexLocker();
        _ZFP_ZFClassTagMapType &m = d->dataCacheMap;
        _ZFP_ZFClassTagMapType::iterator it = m.find(key);
        if(it != m.end()) {
            zfauto ret;
            ret.zfunsafe_assign(it->second);
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFClass::dataCacheRemoveAll(void) const {
    if(!d->dataCacheMap.empty()) {
        _ZFP_ZFClassTagMapType tmp;
        tmp.swap(d->dataCacheMap);
    }
}

// ============================================================
// private
ZFClass::ZFClass(void)
: d(zfnull)
, _ZFP_ZFClass_classParent(zfnull)
, _ZFP_ZFClass_classNamespace(zfnull)
, _ZFP_ZFClass_className(zfnull)
, _ZFP_ZFClass_classNameFull(zfnull)
, _ZFP_ZFClass_classIsAbstract(zffalse)
, _ZFP_ZFClass_classIsInterface(zffalse)
, _ZFP_ZFClass_classIsInternal(zffalse)
, _ZFP_ZFClass_classIsInternalPrivate(zffalse)
, _ZFP_ZFClass_methodAndPropertyCacheNeedUpdate(zffalse)
, _ZFP_ZFClass_implListNeedInit(zftrue)
, _ZFP_ZFClass_classCanAllocPublic(zftrue)
, _ZFP_ZFClass_classAliasTo()
{
    d = zfnew(_ZFP_ZFClassPrivate);
    d->pimplOwner = this;
}
ZFClass::~ZFClass(void) {
    this->instanceObserverRemoveAll();

    if(!d->dataCacheMap.empty()) {
        // class data caches must be removed before destroying a ZFClass
        abort();
    }
    if(!d->classTagMap.empty()) {
        // class tags must be removed before destroying a ZFClass
        abort();
    }

    zfdelete(d);
    d = zfnull;

    // registered by ZFSigName, no need to free
    // this->_ZFP_ZFClass_classNamespace;
    // this->_ZFP_ZFClass_className;
    // this->_ZFP_ZFClass_classNameFull;
}

ZFClass *ZFClass::_ZFP_ZFClassRegister(
        ZF_IN const zfstring &classNamespace
        , ZF_IN const zfstring &className
        , ZF_IN const ZFClass *parent
        , ZF_IN const ZFClass *outer
        , ZF_IN zfbool classCanAllocPublic
        , ZF_IN _ZFP_zfAllocCacheCallback objectAllocWithCacheCallback
        , ZF_IN _ZFP_ZFObjectConstructor constructor
        , ZF_IN _ZFP_ZFObjectDestructor destructor
        , ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback
        , ZF_IN zfbool classIsInterface
        , ZF_IN zfbool classIsDynamicRegister
        , ZF_IN ZFObject *classDynamicRegisterUserData
        ) {
    _ZFP_ZFClass_invokeTimeLogger("reg: %s", className.cString());
    ZFCoreMutexLocker();
    const zfchar *classNamespaceTmp = classNamespace;
    if(outer != zfnull) {
        classNamespaceTmp = outer->classNameFull();
    }

    zfstring classNameTmp = className;
    if(zfsncmp(classNameTmp, "v_", 2) == 0) {
        classNameTmp.remove(0, 2);
    }

    // method data holder is required during _ZFP_ZFClassUnregister,
    // access here to ensure init order
    _ZFP_ZFMethodDataHolderInit();

    classNamespaceTmp = ZFNamespaceSkipGlobal(classNamespaceTmp);
    zfstring classNameFull;
    if(classNamespaceTmp != zfnull) {
        classNameFull += classNamespaceTmp;
        classNameFull += ".";
        classNameFull += classNameTmp;
    }
    else {
        classNameFull = classNameTmp;
    }

    const ZFCorePointerBase *d = _ZFP_ZFClassMap.get(classNameFull);
    ZFClass *cls = zfnull;
    zfbool needFinalInit = zffalse;
    if(d != zfnull) {
        cls = d->pointerValueT<ZFClass *>();
        if(cls->classIsInterface() != classIsInterface || cls->classParent() != parent) {
            ZFCoreCriticalMessageTrim("[ZFClass] register a class that already registered: %s", classNameTmp);
            return zfnull;
        }
        ++(cls->d->refCount);
    }
    else {
        needFinalInit = zftrue;
        cls = zfnew(ZFClass);
        _ZFP_ZFClassMap.set(classNameFull, ZFCorePointerForObject<ZFClass *>(cls));

        cls->d->classIsDynamicRegister = classIsDynamicRegister;
        cls->d->classDynamicRegisterUserData = classDynamicRegisterUserData;
        if(objectAllocWithCacheCallback == ZFObject::_ZFP_zfAllocCacheIvk) {
            cls->d->objectAllocWithCacheCallback = zfnull;
        }
        else {
            cls->d->objectAllocWithCacheCallback = objectAllocWithCacheCallback;
        }
        cls->d->constructor = constructor;
        cls->d->destructor = destructor;

        cls->_ZFP_ZFClass_classNamespace = classNamespaceTmp;
        cls->_ZFP_ZFClass_className = classNameTmp;
        cls->_ZFP_ZFClass_classNameFull = classNameFull;

        if(parent != zfnull && parent != ZFInterface::ClassData()) {
            cls->_ZFP_ZFClass_classParent = parent;
        }
        else {
            cls->_ZFP_ZFClass_classParent = zfnull;
        }
        cls->_ZFP_ZFClass_classIsAbstract = (constructor == zfnull);
        cls->_ZFP_ZFClass_classIsInterface = classIsInterface;
        cls->_ZFP_ZFClass_classCanAllocPublic = (classCanAllocPublic && (constructor != zfnull));

        // internal
        //   _ZFP_xxx
        //   Outer._ZFP_xxx
        zfindex p = zfstringFind(classNameTmp, "_ZFP_");
        if(p != zfindexMax() && (p == 0 || classNameTmp[p - 1] == '.')) {
            cls->_ZFP_ZFClass_classIsInternal = zftrue;
            // _ZFP_I_
            cls->_ZFP_ZFClass_classIsInternalPrivate = (classNameTmp[p + 5] == 'I' && classNameTmp[p + 6] == '_');
        }
        else {
            cls->_ZFP_ZFClass_classIsInternal = zffalse;
            cls->_ZFP_ZFClass_classIsInternalPrivate = zffalse;
        }
    }

    if(checkInitImplListCallback) {
        checkInitImplListCallback(cls);
    }

    if(needFinalInit) {
        _ZFP_ZFClassPrivate::classInitFinish(cls);

        // ClassData() registered here instead of ZFOBJECT_REGISTER,
        // to reduce output executable size and runtime memory usage,
        // unregistered during class unregister
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return invokerMethod->ownerClass();
            }, cls, public, ZFMethodTypeStatic,
            const ZFClass *, zftext("ClassData"));

        // notify
        _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeAttach, cls, zfnull, zfnull);
    }

    return cls;
}
void ZFClass::_ZFP_ZFClassUnregister(ZF_IN const ZFClass *cls) {
    _ZFP_ZFClass_invokeTimeLogger("unreg: %s", cls->className().cString());
    ZFCoreMutexLocker();

    if(cls->classIsDynamicRegister()) {
        // remove all dynamic registered class info,
        // the existing object instance would fallback to parent class
        for(zfstlhashmap<ZFObject *, zfbool>::iterator it = cls->d->classDynamicRegisterObjectInstanceMap.begin(); it != cls->d->classDynamicRegisterObjectInstanceMap.end(); ++it) {
            it->first->_ZFP_ZFObject_classDynamic = zfnull;
        }
        cls->d->classDynamicRegisterObjectInstanceMap.clear();
    }

    zfiter itClass = _ZFP_ZFClassMap.iterFind(cls->classNameFull());
    if(!itClass) {
        ZFCoreCriticalShouldNotGoHere();
        return;
    }

    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeDetach, cls, zfnull, zfnull);

    _ZFP_ZFClassPrivate *d = cls->d;
    --(d->refCount);
    if(d->refCount != 0) {
        return;
    }

    while(!cls->classAliasTo().isEmpty()) {
        ZFClassAliasRemove(cls, cls->classAliasTo().getLast());
    }

    _ZFP_ZFClassDelayDeleteMap.set(cls->classNameFull(),
        *_ZFP_ZFClassMap.iterValue(itClass));
    _ZFP_ZFClassMap.iterRemove(itClass);

    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zffalse;
    ZFMethodUserUnregister(cls->methodForNameIgnoreParent("ClassData"));

    d->classDynamicRegisterUserData = zfnull;
    cls->dataCacheRemoveAll();
    cls->classTagRemoveAll();
    cls->instanceObserverRemoveAll();

    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->allParent.erase(cls);
        if(it->first->classParent() == cls) {
            it->first->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classParent = cls->classParent();
        }
    }
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allParent.begin(); it != d->allParent.end(); ++it) {
        it->first->d->allChildren.erase(cls);
    }
}

void ZFClass::_ZFP_ZFClass_autoRegister(void) const {
    if(d->needAutoRegister) {
        d->needAutoRegister = zffalse;

        // create dummy instance to ensure static init of the object would take effect
        // including method and property register
        if(d->constructor != zfnull) {
            _ZFP_ZFClass_invokeTimeLogger("autoReg: %s", this->className().cString());
            d->destructor(d->constructor());
        }
    }
}

zfbool ZFClass::_ZFP_ZFClass_interfaceNeedRegister(void) {
    return d->needRegisterImplementedInterface;
}
void ZFClass::_ZFP_ZFClass_interfaceRegister(
        ZF_IN zfint dummy
        , ZF_IN const ZFClass *cls
        , ZF_IN _ZFP_ZFObjectToInterfaceCastCallback callback
        , ...
        ) {
    d->needRegisterImplementedInterface = zffalse;

    va_list vaList;
    va_start(vaList, callback);
    const ZFClass *clsTmp = cls;
    _ZFP_ZFObjectToInterfaceCastCallback callbackTmp = callback;
    do {
        if(clsTmp != ZFInterface::ClassData()) {
            d->implementedInterface.add(clsTmp);
            d->interfaceCastMap[clsTmp] = callbackTmp;

            while(clsTmp->classParent() != zfnull) {
                clsTmp = clsTmp->classParent();
                if(clsTmp != zfnull && clsTmp != ZFInterface::ClassData() && clsTmp->classIsInterface()) {
                    d->interfaceCastMap[clsTmp] = callbackTmp;
                }
            }
        }
        clsTmp = va_arg(vaList, const ZFClass *);
        if(clsTmp == zfnull) {
            break;
        }
        callbackTmp = va_arg(vaList, _ZFP_ZFObjectToInterfaceCastCallback);
    } while(zftrue);
    va_end(vaList);
}
ZFInterface *ZFClass::_ZFP_ZFClass_interfaceCast(
        ZF_IN ZFObject *obj
        , ZF_IN const ZFClass *interfaceClass
        ) const {
    zfstlhashmap<const ZFClass *, _ZFP_ZFObjectToInterfaceCastCallback>::iterator it = d->interfaceCastCache.find(interfaceClass);
    if(it != d->interfaceCastCache.end()) {
        return it->second(obj);
    }
    else {
        return zfnull;
    }
}

ZFObject *ZFClass::_ZFP_ZFClass_objectCast(
        ZF_IN ZFObject *obj
        , ZF_IN const ZFClass *objectClass
        ) const {
    obj = obj->_ZFP_ZFObject_ZFImplementDynamicOwnerOrSelf();
    _ZFP_ZFClassPrivate *d = obj->classData()->d;
    if(d->ZFImplementDynamicCache.find(objectClass) != d->ZFImplementDynamicCache.end()) {
        return obj->_ZFP_ZFObject_ZFImplementDynamicHolder(objectClass);
    }
    else if(d->parentClassCache.find(objectClass) != d->parentClassCache.end()) {
        return obj;
    }
    else {
        return zfnull;
    }
}

zfbool ZFClass::_ZFP_ZFClass_ZFImplementDynamicRegister(ZF_IN const ZFClass *clsToImplement) const {
    if(this->classIsTypeOf(clsToImplement)) {
        ZFCoreLogTrim("[ZFImplementDynamicRegister] cls %s already type of %s", this, clsToImplement);
        return zffalse;
    }
    if(this->classIsAbstract()) {
        ZFCoreLogTrim("[ZFImplementDynamicRegister] cls must not abstract: %s", this);
        return zffalse;
    }
    if(clsToImplement->classIsAbstract()) {
        ZFCoreLogTrim("[ZFImplementDynamicRegister] clsToImplement must not abstract: %s", clsToImplement);
        return zffalse;
    }
    d->ZFImplementDynamicList.add(clsToImplement);

    _ZFP_ZFClassPrivate::classParentCacheUpdate(this);
    this->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zftrue;
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        _ZFP_ZFClassPrivate::classParentCacheUpdate(it->first);
        it->first->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zftrue;
    }
    this->dataCacheRemoveAll();
    return zftrue;
}
void ZFClass::_ZFP_ZFClass_ZFImplementDynamicUnregister(ZF_IN const ZFClass *clsToImplement) const {
    zfindex index = d->ZFImplementDynamicList.find(clsToImplement);
    if(index == zfindexMax()) {
        return;
    }
    d->ZFImplementDynamicList.remove(index);

    _ZFP_ZFClassPrivate::classParentCacheUpdate(this);
    this->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zftrue;
    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        _ZFP_ZFClassPrivate::classParentCacheUpdate(it->first);
        it->first->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zftrue;
    }
    this->dataCacheRemoveAll();
}

void ZFClass::_ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const {
    d->destructor(obj);
}

void ZFClass::_ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method) const {
    zfstlvector<const ZFMethod *> &methodList = d->methodMap[method->methodName()];
    methodList.push_back(method);
    this->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodAndPropertyCacheNeedUpdate = zftrue;
}
void ZFClass::_ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method) const {
    _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(method->methodName());
    if(itName != d->methodMap.end()) {
        zfstlvector<const ZFMethod *> &l = itName->second;
        for(zfstlvector<const ZFMethod *>::iterator itList = l.begin(); itList != l.end(); ++itList) {
            if(method == *itList) {
                l.erase(itList);
                if(l.empty()) {
                    d->methodMap.erase(itName);
                }
                break;
            }
        }
        _ZFP_ZFClassPrivate::methodCacheRemove(this, method);
    }
}

void ZFClass::_ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty) const {
    d->propertyMap.set(zfproperty->propertyName(), ZFCorePointerForPointerRef<const ZFProperty *>(zfproperty));
}
void ZFClass::_ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty) const {
    d->propertyMap.remove(zfproperty->propertyName());
    d->propertyMapCache.remove(zfproperty->propertyName());
    _ZFP_ZFClassPrivate::propertyCacheRemove(this, zfproperty);
}

void ZFClass::_ZFP_ZFClass_propertyAutoInitRegister(ZF_IN const ZFProperty *property) const {
    if(property->ownerClass() == this) {
        return;
    }

    d->propertyAutoInitMap[property] = zftrue;

    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->propertyAutoInitMap[property] = zftrue;
    }
}
void ZFClass::_ZFP_ZFClass_propertyAutoInitAction(ZF_IN ZFObject *owner) const {
    d->needAutoRegister = zffalse;

    // access getter to ensure property value created
    for(zfstlhashmap<const ZFProperty *, zfbool>::iterator it = d->propertyAutoInitMap.begin(); it != d->propertyAutoInitMap.end(); ++it) {
        const ZFProperty *property = it->first;
        if(property->_ZFP_ZFProperty_callbackEnsureInit) {
            property->_ZFP_ZFProperty_callbackEnsureInit(property, owner);
        }
    }
}
void ZFClass::_ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property) const {
    if(property->ownerClass() == this) {
        return;
    }

    d->propertyInitStepMap[property][this] = zftrue;

    for(zfstlhashmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->propertyInitStepMap[property][this] = zftrue;
    }
}

_ZFP_zfAllocCacheCallback ZFClass::_ZFP_objectAllocWithCacheCallback(void) const {
    return d->objectAllocWithCacheCallback;
}
_ZFP_ZFObjectConstructor ZFClass::_ZFP_objectConstructor(void) const {
    return d->constructor;
}
_ZFP_ZFObjectDestructor ZFClass::_ZFP_objectDestructor(void) const {
    return d->destructor;
}
void ZFClass::_ZFP_classDynamicRegisterObjectInstanceDetach(ZF_IN ZFObject *obj) const {
    d->classDynamicRegisterObjectInstanceMap.erase(obj);
}

// ============================================================
_ZFP_ZFClassRegisterHolder::_ZFP_ZFClassRegisterHolder(
        ZF_IN const zfstring &classNamespace
        , ZF_IN const zfstring &className
        , ZF_IN const ZFClass *parent
        , ZF_IN const ZFClass *outer
        , ZF_IN zfbool classCanAllocPublic
        , ZF_IN _ZFP_zfAllocCacheCallback objectAllocWithCacheCallback
        , ZF_IN _ZFP_ZFObjectConstructor constructor
        , ZF_IN _ZFP_ZFObjectDestructor destructor
        , ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback
        , ZF_IN_OPT zfbool classIsInterface /* = zffalse */
        , ZF_IN_OPT zfbool classIsDynamicRegister /* = zffalse */
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        )
: cls(zfnull)
{
    cls = ZFClass::_ZFP_ZFClassRegister(
        classNamespace,
        className,
        parent,
        outer,
        classCanAllocPublic,
        objectAllocWithCacheCallback,
        constructor,
        destructor,
        checkInitImplListCallback,
        classIsInterface,
        classIsDynamicRegister,
        classDynamicRegisterUserData);
}
_ZFP_ZFClassRegisterHolder::~_ZFP_ZFClassRegisterHolder(void) {
    ZFClass::_ZFP_ZFClassUnregister(this->cls);
}

// ============================================================
void ZFClassGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFClassMap.allValueT(ret);
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ClassDataUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFObserver &_ZFP_ZFClassDataUpdateObserverRef(void) {
    static ZFObserver d;
    return d;
}
void _ZFP_ZFClassDataUpdateNotify(
        ZF_IN ZFClassDataUpdateType changeType
        , ZF_IN const ZFClass *changedClass
        , ZF_IN const ZFProperty *changedProperty
        , ZF_IN const ZFMethod *changedMethod
        , ZF_IN_OPT const zfstring &name /* = zfnull */
        ) {
    (void)ZFClassDataUpdateObserver(); // ensure init order
    (void)ZFGlobalObserver(); // ensure init order
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkPostStatic) == ZFFrameworkStateAvailable) {
        _ZFP_ZFClass_invokeTimeLogger("cls change notify: %s %s %s %s"
                , ZFClassDataUpdateTypeToString(changeType).cString()
                , changedClass ? changedClass->className().cString() : ZFTOKEN_zfnull
                , changedProperty ? changedProperty->propertyName().cString() : ZFTOKEN_zfnull
                , changedMethod ? changedMethod->objectInfo().cString() : ZFTOKEN_zfnull
                );
        if(changedProperty != zfnull) {
            changedProperty->ownerClass()->dataCacheRemoveAll();
        }
        else if(changedMethod != zfnull) {
            if(changedMethod->ownerClass() != zfnull) {
                changedMethod->ownerClass()->dataCacheRemoveAll();
            }
        }

        v_ZFClassDataUpdateData *holder = zfunsafe_zfAlloc(v_ZFClassDataUpdateData);
        holder->zfv.changeType = changeType;
        holder->zfv.changedClass = changedClass;
        holder->zfv.changedProperty = changedProperty;
        holder->zfv.changedMethod = changedMethod;
        holder->zfv.name = name;
        ZFClassDataUpdateObserver().observerNotify(ZFGlobalEvent::E_ClassDataUpdate(), holder);
        zfunsafe_zfRelease(holder);
    }
}

// ============================================================
void ZFClassAlias(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfstring &aliasNameFull
        ) {
    ZFCoreMutexLocker();

    if(cls == zfnull || !aliasNameFull
        || cls->classAliasTo().find(aliasNameFull) != zfindexMax()
        || ZFClass::classForName(aliasNameFull) != zfnull
    ) {
        return;
    }
    _ZFP_ZFClassMap.set(aliasNameFull, ZFCorePointerForPointerRef<const ZFClass *>(cls));
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classAliasTo.add(aliasNameFull);

    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeClassAliasAttach, cls, zfnull, zfnull, aliasNameFull);
}
void ZFClassAliasRemove(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfstring &aliasNameFull
        ) {
    ZFCoreMutexLocker();
    if(cls == zfnull || !aliasNameFull) {
        return;
    }
    zfindex index = cls->classAliasTo().find(aliasNameFull);
    if(index == zfindexMax()) {
        return;
    }

    _ZFP_ZFClassMap.remove(aliasNameFull);
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classAliasTo.remove(index);

    _ZFP_ZFClassDataUpdateNotify(ZFClassDataUpdateTypeClassAliasDetach, cls, zfnull, zfnull, aliasNameFull);
}

ZF_NAMESPACE_GLOBAL_END

