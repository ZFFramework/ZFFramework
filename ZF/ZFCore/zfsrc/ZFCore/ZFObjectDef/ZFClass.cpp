#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"
#include "ZFDynamicInvoker.h"

#include "ZFCore/ZFSTLWrapper/zfstlvector.h"
#include "ZFCore/ZFSTLWrapper/zfstldeque.h"
#include "ZFCore/ZFSTLWrapper/zfstlmap.h"

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
        ZFSigName dummy("ZFObject");
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
static const ZFClass *_ZFP_ZFClassDummy[1] = {0};
static _ZFP_ZFObjectToInterfaceCastCallback _ZFP_ZFClassInterfaceCastCallbackDummy[1] = {0};

typedef zfstlmap<zfstring, zfauto> _ZFP_ZFClassTagMapType;
typedef zfstlmap<const ZFProperty *, zfstlmap<const ZFClass *, zfbool> > _ZFP_ZFClassPropertyInitStepMapType;

typedef zfstlmap<ZFSigName, zfstlvector<const ZFMethod *> > _ZFP_ZFClassMethodMapType;
typedef zfstlmap<ZFSigName, const ZFProperty *> _ZFP_ZFClassPropertyMapType;

zfclassNotPOD _ZFP_ZFClassPrivate {
public:
    zfuint refCount;
    /*
     * here's a memo for ZFCoreLibDestroyFlag:
     *
     * ZFFramework is designed as plugin type framework,
     * which can be load dynamically
     *
     * it's all right for most case if use
     * ZF_GLOBAL_INITIALIZER_INIT/ZF_STATIC_INITIALIZER_INIT,
     * however, for core meta-object types (class map, method map, etc),
     * it require additional register and unregister steps
     * to ensure initialize and destroy order
     *
     * here's a typical case of the questions mentioned above:
     *
     * * libraries that depends on ZFFramework needs to be load and unload at runtime
     *
     *   at this case, we need some static holder object, to unregister class map from ZFFramework,
     *   during the library being unloaded
     *
     * * ZFFramework would be unloaded before app's destruction,
     *   this is typical if under Qt using QLibrary
     *
     *   at this case, simply use static holder object would cause crash during app's destruction,
     *   since ZFFramework already unloaded when the holder object try to unregister class map
     */
    ZFCoreArray<zfbool *> ZFCoreLibDestroyFlag;
    ZFClass *pimplOwner;
    zfbool classIsDynamicRegister;
    zfauto classDynamicRegisterUserData;
    zfstlmap<ZFObject *, zfbool> classDynamicRegisterObjectInstanceMap;
    _ZFP_zfAllocCacheCallback objectAllocWithCacheCallback;
    _ZFP_ZFObjectConstructor constructor;
    _ZFP_ZFObjectDestructor destructor;

public:
    zfbool needAutoRegister;
    zfbool needRegisterImplementedInterface;
    zfbool needFinalInit;
    ZFCoreArray<const ZFClass *> implementedInterface;
    _ZFP_ZFClassMethodMapType methodMap; // method of this cls only
    ZFCoreArray<const ZFMethod *> methodList; // method of this cls only
    _ZFP_ZFClassPropertyMapType propertyMap; // property of this cls only
    ZFCoreArray<const ZFProperty *> propertyList; // property of this cls only
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
    zfstlmap<const ZFProperty *, zfbool> propertyAutoInitMap;
    /*
     * see propertyAutoInitMap,
     * used to check whether two class has same property init step
     */
    _ZFP_ZFClassPropertyInitStepMapType propertyInitStepMap;

public:
    _ZFP_ZFClassTagMapType classTagMap;

public:
    zfstlmap<zfstring, zfbool> classDataChangeAutoRemoveTagList;

public:
    zfstlmap<const ZFClass *, zfbool> allParent; // all parent and interface excluding self
    zfstlmap<const ZFClass *, zfbool> allChildren; // all children excluding self

public:
    const ZFClass **interfaceCastListCache; // all parent implemented interface, for interface cast
    _ZFP_ZFObjectToInterfaceCastCallback *interfaceCastCallbackListCache;

    // ============================================================
    // instance observer
public:
    zfclassLikePOD InstanceObserverData {
    public:
        ZFListener observer;
        ZFObject *owner;
        ZFLevel observerLevel;
    };
    zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > instanceObserver;
    zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> instanceObserverCached;

public:
    void _instanceObserverDoAdd(
            ZF_IN_OUT zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> &buf
            , ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data
            ) {
        zfstlsize index = buf.size();
        while(index > 0 && buf[index - 1]->observerLevel > data->observerLevel) {
            --index;
        }
        buf.insert(buf.begin() + index, data);
    }
    void instanceObserverDoAdd(
            ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data
            , ZF_IN zfbool observeAllChildType
            ) {
        this->_instanceObserverDoAdd(this->instanceObserverCached, data);
        if(!observeAllChildType) {
            return;
        }

        for(zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin(); childIt != this->allChildren.end(); ++childIt) {
            if(!childIt->first->classIsInternalPrivate()) {
                this->_instanceObserverDoAdd(childIt->first->d->instanceObserverCached, data);
            }
        }
    }
    void instanceObserverDoRemove(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data) {
        const ZFClass *cls = this->pimplOwner;
        zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin();
        do {
            for(zfstlsize i = 0; i < cls->d->instanceObserverCached.size(); ++i) {
                if(cls->d->instanceObserverCached[i] == data) {
                    cls->d->instanceObserverCached.erase(cls->d->instanceObserverCached.begin() + i);
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
    : refCount(1)
    , ZFCoreLibDestroyFlag()
    , pimplOwner(zfnull)
    , classIsDynamicRegister(zffalse)
    , classDynamicRegisterUserData()
    , classDynamicRegisterObjectInstanceMap()
    , constructor(zfnull)
    , destructor(zfnull)
    , needAutoRegister(zftrue)
    , needRegisterImplementedInterface(zftrue)
    , needFinalInit(zftrue)
    , implementedInterface()
    , methodMap()
    , methodList()
    , propertyMap()
    , propertyList()
    , propertyAutoInitMap()
    , propertyInitStepMap()
    , classTagMap()
    , classDataChangeAutoRemoveTagList()
    , allParent()
    , allChildren()
    , interfaceCastListCache(zfnull)
    , interfaceCastCallbackListCache(zfnull)
    , instanceObserver()
    , instanceObserverCached()
    , parentClassCache()
    , parentTypeCache()
    , methodAndPropertyCacheNeedUpdate(zftrue)
    , methodMapCache()
    , propertyMapCache()
    {
    }
    ~_ZFP_ZFClassPrivate(void) {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i) {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
        if(this->interfaceCastListCache != _ZFP_ZFClassDummy) {
            zffree(this->interfaceCastListCache);
            this->interfaceCastListCache = zfnull;
        }
        if(this->interfaceCastCallbackListCache != _ZFP_ZFClassInterfaceCastCallbackDummy) {
            zffree(this->interfaceCastCallbackListCache);
            this->interfaceCastCallbackListCache = zfnull;
        }
    }

public:
    static void classInitFinish(ZF_IN ZFClass *cls);

    // ============================================================
    // caches
public:
    ZFCoreArray<const ZFClass *> parentClassCache; // for classIsTypeOf(class), ensured end with a null element for performance
    ZFCoreArray<const ZFClass *> parentTypeCache; // for classIsTypeOf(interface), ensured end with a null element for performance

    zfbool methodAndPropertyCacheNeedUpdate;
    _ZFP_ZFClassMethodMapType methodMapCache; // method of this cls and all parent, order ensured from self > parent > parent interface
    _ZFP_ZFClassPropertyMapType propertyMapCache; // property of this cls and all parent

    static void classParentCacheUpdate(ZF_IN const ZFClass *cls);
    static void methodAndPropertyCacheUpdate(ZF_IN const ZFClass *cls);
};

void _ZFP_ZFClassPrivate::classInitFinish(ZF_IN ZFClass *cls) {
    { // copy parent's interface cast datas
        ZFCoreArray<const ZFClass *> parentInterfaceList;
        if(cls->classParent() != zfnull) {
            for(const ZFClass **p = cls->classParent()->d->interfaceCastListCache; *p != zfnull; ++p) {
                if(parentInterfaceList.find(*p, ZFComparerCheckEqual) == zfindexMax()) {
                    parentInterfaceList.add(*p);
                }
            }
        }
        if(cls->d->interfaceCastListCache != zfnull) {
            for(const ZFClass **p = cls->d->interfaceCastListCache; *p != zfnull; ++p) {
                if(parentInterfaceList.find(*p, ZFComparerCheckEqual) == zfindexMax()) {
                    parentInterfaceList.add(*p);
                }
            }
        }
        if(parentInterfaceList.isEmpty()) {
            cls->d->interfaceCastListCache = _ZFP_ZFClassDummy;
        }
        else {
            cls->d->interfaceCastListCache = (const ZFClass **)zfrealloc(
                cls->d->interfaceCastListCache != _ZFP_ZFClassDummy ? cls->d->interfaceCastListCache : zfnull
                , sizeof(const ZFClass *) * (parentInterfaceList.count() + 1));
            for(zfindex i = 0, iEnd = parentInterfaceList.count(); i < iEnd; ++i) {
                cls->d->interfaceCastListCache[i] = parentInterfaceList[i];
            }
            cls->d->interfaceCastListCache[parentInterfaceList.count()] = zfnull;
        }

        ZFCoreArray<_ZFP_ZFObjectToInterfaceCastCallback> parentInterfaceCastList;
        if(cls->classParent() != zfnull) {
            for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->classParent()->d->interfaceCastCallbackListCache; *p != zfnull; ++p) {
                if(parentInterfaceCastList.find(*p, ZFComparerCheckEqual) == zfindexMax()) {
                    parentInterfaceCastList.add(*p);
                }
            }
        }
        if(cls->d->interfaceCastCallbackListCache != zfnull) {
            for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->d->interfaceCastCallbackListCache; *p != zfnull; ++p) {
                if(parentInterfaceCastList.find(*p, ZFComparerCheckEqual) == zfindexMax()) {
                    parentInterfaceCastList.add(*p);
                }
            }
        }

        if(parentInterfaceCastList.isEmpty()) {
            cls->d->interfaceCastCallbackListCache = _ZFP_ZFClassInterfaceCastCallbackDummy;
        }
        else {
            cls->d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
                cls->d->interfaceCastCallbackListCache != _ZFP_ZFClassInterfaceCastCallbackDummy ? cls->d->interfaceCastCallbackListCache : zfnull
                , sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (parentInterfaceCastList.count() + 1));
            for(zfindex i = 0, iEnd = parentInterfaceCastList.count(); i < iEnd; ++i) {
                cls->d->interfaceCastCallbackListCache[i] = parentInterfaceCastList[i];
            }
            cls->d->interfaceCastCallbackListCache[parentInterfaceCastList.count()] = zfnull;
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
        for(zfstlmap<const ZFClass *, zfbool>::iterator itParent = cls->d->allParent.begin();
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
        for(zfstlmap<const ZFClass *, zfbool>::iterator it = cls->d->allParent.begin(); it != cls->d->allParent.end(); ++it) {
            zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > &parentInstanceObserver = it->first->d->instanceObserver;
            for(zfstlsize i = 0; i < parentInstanceObserver.size(); ++i) {
                cls->d->instanceObserverCached.push_back(parentInstanceObserver[i]);
            }
        }
    }

    // cache
    classParentCacheUpdate(cls);
}

void _ZFP_ZFClassPrivate::classParentCacheUpdate(ZF_IN const ZFClass *cls) {
    ZFCoreArray<const ZFClass *> &parentClassCache = cls->d->parentClassCache;
    ZFCoreArray<const ZFClass *> parentTypeCache = cls->d->parentTypeCache;
    parentClassCache.removeAll();
    parentTypeCache.removeAll();

    if(!cls->classIsInterface()) {
        const ZFClass *t = cls;
        do {
            parentClassCache.add(t);
            t = t->classParent();
        } while(t != zfnull);
    }
    parentClassCache.add(zfnull);

    {
        ZFCoreQueuePOD<const ZFClass *> toCheck;
        toCheck.add(cls);
        do {
            const ZFClass *t = toCheck.take();
            if(t->classParent() != zfnull) {
                toCheck.add(t->classParent());
            }
            toCheck.addFrom(t->d->implementedInterface);

            if(parentTypeCache.find(t) == zfindexMax()) {
                parentTypeCache.add(t);
            }
        } while(!toCheck.isEmpty());
    }
    parentTypeCache.add(zfnull);
}

void _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(ZF_IN const ZFClass *cls) {
    if(!cls->d->methodAndPropertyCacheNeedUpdate) {
        return;
    }
    zfCoreMutexLocker();
    if(!cls->d->methodAndPropertyCacheNeedUpdate) {
        return;
    }
    cls->d->methodAndPropertyCacheNeedUpdate = zffalse;
    _ZFP_ZFClassMethodMapType &methodMapCache = cls->d->methodMapCache;
    _ZFP_ZFClassPropertyMapType &propertyMapCache = cls->d->propertyMapCache;
    methodMapCache.clear();
    propertyMapCache.clear();

    ZFCoreQueuePOD<const ZFClass *> toCheck;
    toCheck.add(cls);
    do {
        const ZFClass *t = toCheck.take();
        if(t->classParent() != zfnull) {
            toCheck.add(t->classParent());
        }
        toCheck.addFrom(t->d->implementedInterface);

        for(_ZFP_ZFClassMethodMapType::iterator it = t->d->methodMap.begin(); it != t->d->methodMap.end(); ++it) {
            zfstlvector<const ZFMethod *> &methodList = methodMapCache[it->first];
            methodList.insert(methodList.end(), it->second.begin(), it->second.end());
        }
        for(_ZFP_ZFClassPropertyMapType::iterator it = t->d->propertyMap.begin(); it != t->d->propertyMap.end(); ++it) {
            propertyMapCache[it->first] = it->second;
        }
    } while(!toCheck.isEmpty());
}

// ============================================================
// clear class tag both in ZFLevelZFFrameworkEssential and ZFLevelZFFrameworkHigh
static void _ZFP_ZFClass_classTagClear(void) {
    ZFCoreArray<const ZFClass *> allClass;
    _ZFP_ZFClassMap.allValueT(allClass);
    for(zfindex i = 0; i < allClass.count(); ++i) {
        allClass.get(i)->classTagRemoveAll();
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
const ZFClass *ZFClass::classForName(ZF_IN const zfchar *className) {
    if(className == zfnull) {
        return zfnull;
    }
    else {
        return _ZFP_ZFClassMap.get<const ZFClass *>(ZFNamespaceSkipGlobal(className));
    }
}
const ZFClass *ZFClass::classForName(
        ZF_IN const zfchar *className
        , ZF_IN const zfchar *classNamespace
        ) {
    classNamespace = ZFNamespaceSkipGlobal(classNamespace);
    if(!zfstringIsEmpty(classNamespace)) {
        zfstring classNameFull;
        classNameFull += classNamespace;
        classNameFull += ZFNamespaceSeparator();
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

    _ZFP_ZFClassPrivate::InstanceObserverData *data = zfnew(_ZFP_ZFClassPrivate::InstanceObserverData);
    data->observer = observer;
    data->owner = owner;
    data->observerLevel = observerLevel;
    d->instanceObserver.push_back(ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *>(data));
    d->instanceObserverDoAdd(data, observeAllChildType);
}
void ZFClass::instanceObserverRemove(ZF_IN const ZFListener &observer) const {
    zfCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->observer == observer) {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            break;
        }
    }
}
void ZFClass::instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const {
    zfCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->owner == owner) {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            --i;
        }
    }
}
void ZFClass::instanceObserverRemoveAll(void) const {
    zfCoreMutexLocker();
    while(!d->instanceObserver.empty()) {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[0];
        d->instanceObserverDoRemove(data);
        d->instanceObserver.erase(d->instanceObserver.begin());
    }
}
void ZFClass::_ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const {
    if(!d->instanceObserverCached.empty()) {
        ZFArgs zfargs;
        zfargs
            .eventFilterEnable(zftrue)
            .eventId(ZFObject::EventObjectAfterAlloc())
            .sender(obj)
            ;
        for(zfstlsize i = 0; i < d->instanceObserverCached.size() && !zfargs.eventFiltered(); ++i) {
            _ZFP_ZFClassPrivate::InstanceObserverData &data = *(d->instanceObserverCached[i]);
            data.observer.execute(zfargs);
        }
    }
}

// ============================================================
// class data change observer
void ZFClass::_ZFP_ZFClass_classDataChangeNotify(void) const {
    if(!d->classDataChangeAutoRemoveTagList.empty()) {
        zfstlmap<zfstring, zfbool> t = d->classDataChangeAutoRemoveTagList;
        for(zfstlmap<zfstring, zfbool>::iterator it = t.begin(); it != t.end(); ++it) {
            this->classTagRemove(it->first);
        }
    }
}
void ZFClass::classDataChangeAutoRemoveTagAdd(ZF_IN const zfchar *tag) const {
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList[tag] = zftrue;
}
void ZFClass::classDataChangeAutoRemoveTagRemove(ZF_IN const zfchar *tag) const {
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList.erase(tag);
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
    if(this->implementedInterfaceCount() > 0) {
        ret += '<';
        for(zfindex i = 0; i < this->implementedInterfaceCount(); ++i) {
            if(i != 0) {
                ret += ", ";
            }
            ret += this->implementedInterfaceAt(i)->classNameFull();
        }
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

        if(cls->implementedInterfaceCount() > 0) {
            zfbool first = zftrue;
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
                if(cls->implementedInterfaceAt(i) == ZFInterface::ClassData()) {
                    continue;
                }
                if(first) {
                    first = zffalse;
                    ret += '<';
                }
                else {
                    ret += ", ";
                }
                cls->implementedInterfaceAt(i)->objectInfoOfInheritTreeT(ret);
            }
            if(!first) {
                ret += '>';
            }
        }

        cls = cls->classParent();
    }
}

zfbool ZFClass::classIsTypeOf(ZF_IN const ZFClass *cls) const {
    const ZFCoreArray<const ZFClass *> &p = (cls->classIsInterface() ? d->parentTypeCache : d->parentClassCache);
    for(zfindex i = 0, iEnd = p.count(); i < iEnd; ++i) {
        if(p[i] == cls) {
            return zftrue;
        }
    }
    return zffalse;
}

zfbool ZFClass::classIsDynamicRegister(void) const {
    return d->classIsDynamicRegister;
}
zfany ZFClass::classDynamicRegisterUserData(void) const {
    return d->classDynamicRegisterUserData;
}

zfauto ZFClass::newInstance(void) const {
    zfCoreMutexLocker();
    ZFObject *obj = zfnull;
    if(d->objectAllocWithCacheCallback) {
        obj = d->objectAllocWithCacheCallback();
    }
    else {
        obj = d->objectConstruct();
        if(obj != zfnull) {
            obj->objectOnInit();
            obj->_ZFP_ZFObjectCheckOnInit();
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
        , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
        , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
        ) const {
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = ZFMETHOD_MAX_PARAM;
    do {
        if(param0 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 0; break;} else {paramList[0].zfunsafe_assign(param0);}
        if(param1 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 1; break;} else {paramList[1].zfunsafe_assign(param1);}
        if(param2 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 2; break;} else {paramList[2].zfunsafe_assign(param2);}
        if(param3 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 3; break;} else {paramList[3].zfunsafe_assign(param3);}
        if(param4 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 4; break;} else {paramList[4].zfunsafe_assign(param4);}
        if(param5 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 5; break;} else {paramList[5].zfunsafe_assign(param5);}
        if(param6 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 6; break;} else {paramList[6].zfunsafe_assign(param6);}
        if(param7 == ZFMethodGenericInvokerDefaultParam()) {paramCount = 7; break;} else {paramList[7].zfunsafe_assign(param7);}
    } while(zffalse);
    zfCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_alloc(ret, zfnull, this, paramCount, paramList, zftrue)) {
        return ret;
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
    zfCoreMutexLock();
    zfauto paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = zfmMin((zfindex)ZFMETHOD_MAX_PARAM, params.count());
    for(zfindex i = 0; i < paramCount; ++i) {
        paramList[i].zfunsafe_assign(params[i]);
    }
    for(zfindex i = paramCount; i < ZFMETHOD_MAX_PARAM; ++i) {
        paramList[i].zfunsafe_assign(ZFMethodGenericInvokerDefaultParam());
    }
    zfCoreMutexUnlock();
    zfauto ret;
    if(ZFDI_alloc(ret, errorHint, this, paramCount, paramList, zftrue)) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

void *ZFClass::newInstanceGenericBegin(void) const {
    zfCoreMutexLocker();
    return d->objectConstruct();
}
zfbool ZFClass::newInstanceGenericCheck(
        ZF_IN void *&token
        , ZF_IN const ZFMethod *objectOnInitMethod
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */) const {
    if(objectOnInitMethod == zfnull
            // || !this->classIsTypeOf(objectOnInitMethod->methodOwnerClass())
            // || !zfstringIsEqual(objectOnInitMethod->methodName(), "objectOnInit")
            ) {
        return zffalse;
    }
    ZFObject *obj = (ZFObject *)token;
    zfauto methodRetDummy;
    zfbool ret = objectOnInitMethod->methodGenericInvoker()(methodRetDummy, errorHint, obj, objectOnInitMethod, paramCount, paramList);
    if(!ret && obj->d) {
        zfCoreMutexLocker();
        // since objectOnInit already called,
        // we must ensure init and destroy the object,
        // then recreate the token
        obj->_ZFP_ZFObjectCheckOnInit();
        zfunsafe_zfRelease(obj);
        token = d->objectConstruct();
    }
    else if(ret && obj->d && obj->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed) != zfnull) {
        zfCoreMutexLocker();
        ret = zffalse;
        if(errorHint != zfnull) {
            v_zfstring *error = obj->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed);
            if(error != zfnull) {
                *errorHint += error->zfv;
            }
        }
        // remove for safety
        obj->objectTagRemove(ZFObjectTagKeyword_newInstanceGenericFailed);

        // since objectOnInit already called,
        // we must ensure init and destroy the object,
        // then recreate the token
        obj->_ZFP_ZFObjectCheckOnInit();
        zfunsafe_zfRelease(obj);
        token = d->objectConstruct();
    }
    return ret;
}
zfauto ZFClass::newInstanceGenericEnd(
        ZF_IN void *&token
        , ZF_IN zfbool objectOnInitMethodInvokeSuccess
        ) const {
    ZFObject *obj = (ZFObject *)token;
    if(objectOnInitMethodInvokeSuccess) {
        obj->_ZFP_ZFObjectCheckOnInit();
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

// ============================================================
// ZFMethod
zfindex ZFClass::methodCount(void) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    return d->methodList.count();
}
const ZFMethod *ZFClass::methodAt(ZF_IN zfindex index) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    if(index >= d->methodList.count()) {
        zfCoreCriticalIndexOutOfRange(index, this->methodCount());
        return zfnull;
    }
    return d->methodList[index];
}

void ZFClass::methodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(_ZFP_ZFClassMethodMapType::iterator it = d->methodMapCache.begin(); it != d->methodMapCache.end(); ++it) {
        ret.addFrom(&it->second[0], (zfindex)it->second.size());
    }
}

/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFClass::methodForNameIgnoreParent(
        ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *methodParamTypeId0
        , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamTypeIdIsMatch(
                            methodParamTypeId0
                            , methodParamTypeId1
                            , methodParamTypeId2
                            , methodParamTypeId3
                            , methodParamTypeId4
                            , methodParamTypeId5
                            , methodParamTypeId6
                            , methodParamTypeId7
                            )) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForNameIgnoreParent(ZF_IN const zfchar *methodName) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> const &l = itName->second;
            if(l.size() > 1) {
                for(zfstlsize i = 0; i < l.size(); ++i) {
                    if(l[i]->methodParamCountMin() == 0) {
                        return l[i];
                    }
                }
            }
            return l[0];
        }
    }
    return zfnull;
}
void ZFClass::methodForNameIgnoreParentGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfchar *methodName
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
        ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *methodParamTypeId0
        , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
        , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
        ) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(methodName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamTypeIdIsMatch(
                            methodParamTypeId0
                            , methodParamTypeId1
                            , methodParamTypeId2
                            , methodParamTypeId3
                            , methodParamTypeId4
                            , methodParamTypeId5
                            , methodParamTypeId6
                            , methodParamTypeId7
                            )) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForName(ZF_IN const zfchar *methodName) const {
    if(methodName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(methodName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> const &l = itName->second;
            if(l.size() > 1) {
                for(zfstlsize i = 0; i < l.size(); ++i) {
                    if(l[i]->methodParamCountMin() == 0) {
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
        , ZF_IN const zfchar *methodName
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
    return d->propertyList.count();
}
const ZFProperty *ZFClass::propertyAt(ZF_IN zfindex index) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    if(index >= d->propertyList.count()) {
        zfCoreCriticalIndexOutOfRange(index, this->propertyCount());
        return zfnull;
    }
    return d->propertyList.get(index);
}

void ZFClass::propertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const {
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    for(_ZFP_ZFClassPropertyMapType::iterator it = d->propertyMapCache.begin(); it != d->propertyMapCache.end(); ++it) {
        ret.add(it->second);
    }
}

const ZFProperty *ZFClass::propertyForNameIgnoreParent(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        if(propertyName != zfnull) {
            _ZFP_ZFClassPropertyMapType::iterator it = d->propertyMap.find(propertyName);
            if(it != d->propertyMap.end()) {
                return it->second;
            }
        }
    }
    return zfnull;
}
const ZFProperty *ZFClass::propertyForName(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        if(propertyName != zfnull) {
            _ZFP_ZFClassPropertyMapType::iterator it = d->propertyMapCache.find(propertyName);
            if(it != d->propertyMapCache.end()) {
                return it->second;
            }
        }
    }
    return zfnull;
}

const ZFMethod *ZFClass::propertySetterForNameIgnoreParent(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 1) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertySetterForName(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(propertyName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 1) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForNameIgnoreParent(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 0) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForName(const zfchar *propertyName) const {
    if(propertyName != zfnull) {
        _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
        _ZFP_ZFClassMethodMapType::iterator itName = d->methodMapCache.find(propertyName);
        if(itName != d->methodMapCache.end()) {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i) {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 0) {
                    return m;
                }
            }
        }
    }
    return zfnull;
}

zfbool ZFClass::propertyHasOverrideInitStep(void) const {
    return !(d->propertyInitStepMap.empty());
}
zfbool ZFClass::propertyHasOverrideInitStep(ZF_IN const ZFProperty *property) const {
    return (d->propertyInitStepMap.find(property) != d->propertyInitStepMap.end());
}

// ============================================================
// class instance methods
void ZFClass::classTag(
        ZF_IN const zfchar *key
        , ZF_IN ZFObject *tag
        ) const {
    if(key == zfnull) {
        return;
    }
    if(tag != zfnull && ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) != ZFFrameworkStateAvailable) {
        // must not store any new tags during cleanup
        abort();
    }
    zfCoreMutexLocker();
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
zfany ZFClass::classTag(ZF_IN const zfchar *key) const {
    if(key != zfnull) {
        zfCoreMutexLocker();
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
zfauto ZFClass::classTagRemoveAndGet(ZF_IN const zfchar *key) const {
    if(key != zfnull) {
        zfCoreMutexLocker();
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
, _ZFP_ZFClass_implListNeedInit(zftrue)
, _ZFP_ZFClass_classCanAllocPublic(zftrue)
, _ZFP_ZFClass_classAliasTo()
{
    d = zfnew(_ZFP_ZFClassPrivate);
    d->pimplOwner = this;
}
ZFClass::~ZFClass(void) {
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
        ZF_IN zfbool *ZFCoreLibDestroyFlag
        , ZF_IN const zfchar *classNamespace
        , ZF_IN const zfchar *className
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
    zfCoreMutexLocker();
    if(outer != zfnull) {
        classNamespace = outer->classNameFull();
    }

    // method data holder is required during _ZFP_ZFClassUnregister,
    // access here to ensure init order
    _ZFP_ZFMethodDataHolderInit();

    classNamespace = ZFNamespaceSkipGlobal(classNamespace);
    zfstring classNameFull;
    if(classNamespace != zfnull) {
        classNameFull += classNamespace;
        classNameFull += ZFNamespaceSeparator();
    }
    classNameFull += className;

    ZFCorePointerBase *d = _ZFP_ZFClassMap.get(classNameFull);
    ZFClass *cls = zfnull;
    zfbool needFinalInit = zffalse;
    if(d != zfnull) {
        cls = d->pointerValueT<ZFClass *>();
        if(cls->classIsInterface() != classIsInterface || cls->classParent() != parent) {
            zfCoreCriticalMessageTrim("[ZFClass] register a class that already registered: %s", className);
            return zfnull;
        }
        ++(cls->d->refCount);
    }
    else {
        needFinalInit = zftrue;
        cls = zfnew(ZFClass);
        _ZFP_ZFClassMap.set(classNameFull, ZFCorePointerForObject<ZFClass *>(cls));

        if(ZFCoreLibDestroyFlag) {
            cls->d->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);
        }

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

        cls->_ZFP_ZFClass_classNamespace = classNamespace;
        cls->_ZFP_ZFClass_className = className;
        cls->_ZFP_ZFClass_classNameFull = classNameFull;

        cls->_ZFP_ZFClass_classParent = parent;
        cls->_ZFP_ZFClass_classIsAbstract = (constructor == zfnull);
        cls->_ZFP_ZFClass_classIsInterface = classIsInterface;
        cls->_ZFP_ZFClass_classCanAllocPublic = (classCanAllocPublic && (constructor != zfnull));

        // internal
        static const zfchar *_ZFP_ = "_ZFP_";
        static zfindex _ZFP_len = zfslen(_ZFP_);
        static const zfchar *_ZFP_I_ = "_ZFP_I_";
        static zfindex _ZFP_I_len = zfslen(_ZFP_I_);
        if(zfsncmp(className, _ZFP_I_, _ZFP_I_len) == 0) {
            cls->_ZFP_ZFClass_classIsInternal = zftrue;
            cls->_ZFP_ZFClass_classIsInternalPrivate = zftrue;
        }
        else if(zfsncmp(className, _ZFP_, _ZFP_len) == 0) {
            cls->_ZFP_ZFClass_classIsInternal = zftrue;
            cls->_ZFP_ZFClass_classIsInternalPrivate = zffalse;
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
                return invokerMethod->methodOwnerClass();
            }, cls, public, ZFMethodTypeStatic,
            const ZFClass *, "ClassData");

        // notify
        _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, cls, zfnull, zfnull);
    }

    return cls;
}
void ZFClass::_ZFP_ZFClassUnregister(
        ZF_IN zfbool *ZFCoreLibDestroyFlag
        , ZF_IN const ZFClass *cls
        ) {
    if(ZFCoreLibDestroyFlag && *ZFCoreLibDestroyFlag) {
        return;
    }
    zfCoreMutexLocker();

    if(cls->classIsDynamicRegister()) {
        // remove all dynamic registered class info,
        // the existing object instance would fallback to parent class
        for(zfstlmap<ZFObject *, zfbool>::iterator it = cls->d->classDynamicRegisterObjectInstanceMap.begin(); it != cls->d->classDynamicRegisterObjectInstanceMap.end(); ++it) {
            it->first->_ZFP_ZFObject_classDynamic = zfnull;
        }
        cls->d->classDynamicRegisterObjectInstanceMap.clear();
    }

    zfiterator itClass = _ZFP_ZFClassMap.iterFind(cls->classNameFull());
    if(!_ZFP_ZFClassMap.iterValid(itClass)) {
        zfCoreCriticalShouldNotGoHere();
        return;
    }

    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeDetach, cls, zfnull, zfnull);

    _ZFP_ZFClassPrivate *d = cls->d;
    d->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
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

    ZFMethodUserUnregister(cls->methodForNameIgnoreParent("ClassData"));

    d->classDynamicRegisterUserData = zfnull;
    cls->classTagRemoveAll();
    cls->instanceObserverRemoveAll();

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->allParent.erase(cls);
        if(it->first->classParent() == cls) {
            it->first->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classParent = cls->classParent();
        }
    }
    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allParent.begin(); it != d->allParent.end(); ++it) {
        it->first->d->allChildren.erase(cls);
    }
}

void ZFClass::_ZFP_ZFClass_autoRegister(void) const {
    zfCoreMutexLocker();
    if(d->needAutoRegister) {
        d->needAutoRegister = zffalse;

        // create dummy instance to ensure static init of the object would take effect
        // including method and property register
        if(d->constructor != zfnull) {
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

    ZFCoreArray<const ZFClass *> clsList;
    ZFCoreArray<_ZFP_ZFObjectToInterfaceCastCallback> callbackList;
    {
        va_list vaList;
        va_start(vaList, callback);
        const ZFClass *clsTmp = cls;
        _ZFP_ZFObjectToInterfaceCastCallback callbackTmp = callback;
        do {
            if(clsTmp != ZFInterface::ClassData()) {
                d->implementedInterface.add(clsTmp);
                clsList.add(clsTmp);
                callbackList.add(callbackTmp);
            }
            clsTmp = va_arg(vaList, const ZFClass *);
            if(clsTmp == zfnull) {
                break;
            }
            callbackTmp = va_arg(vaList, _ZFP_ZFObjectToInterfaceCastCallback);
        } while(zftrue);
        va_end(vaList);
    }

    if(clsList.isEmpty()) {
        d->interfaceCastListCache = _ZFP_ZFClassDummy;
        d->interfaceCastCallbackListCache = _ZFP_ZFClassInterfaceCastCallbackDummy;
    }
    else {
        d->interfaceCastListCache = (const ZFClass **)zfrealloc(
            d->interfaceCastListCache != _ZFP_ZFClassDummy ? d->interfaceCastListCache : zfnull
            , sizeof(const ZFClass *) * (clsList.count() + 1));
        for(zfindex i = 0, iEnd = clsList.count(); i < iEnd; ++i) {
            d->interfaceCastListCache[i] = clsList[i];
        }
        d->interfaceCastListCache[clsList.count()] = zfnull;

        d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
            d->interfaceCastCallbackListCache != _ZFP_ZFClassInterfaceCastCallbackDummy ? d->interfaceCastCallbackListCache : zfnull
            , sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (callbackList.count() + 1));
        for(zfindex i = 0, iEnd = callbackList.count(); i < iEnd; ++i) {
            d->interfaceCastCallbackListCache[i] = callbackList[i];
        }
        d->interfaceCastCallbackListCache[callbackList.count()] = zfnull;
    }
}
ZFInterface *ZFClass::_ZFP_ZFClass_interfaceCast(
        ZF_IN ZFObject * const &obj
        , ZF_IN const ZFClass *interfaceClass
        ) const {
    if(obj) {
        const ZFClass **parentInterfaceListTmp = d->interfaceCastListCache;
        _ZFP_ZFObjectToInterfaceCastCallback *parentInterfaceCastListTmp = d->interfaceCastCallbackListCache;
        while(*parentInterfaceListTmp) {
            if(*parentInterfaceListTmp == interfaceClass) {
                return (*parentInterfaceCastListTmp)(obj);
            }
            ++parentInterfaceListTmp;
            ++parentInterfaceCastListTmp;
        }
    }
    return zfnull;
}

void ZFClass::_ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const {
    d->destructor(obj);
}

void ZFClass::_ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method) const {
    zfstlvector<const ZFMethod *> &methodList = d->methodMap[method->methodName()];
    methodList.push_back(method);
    d->methodList.add(method);
    d->methodAndPropertyCacheNeedUpdate = zftrue;
}
void ZFClass::_ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method) const {
    _ZFP_ZFClassMethodMapType::iterator itName = d->methodMap.find(method->methodName());
    if(itName != d->methodMap.end()) {
        zfstlvector<const ZFMethod *> &l = itName->second;
        for(zfstlsize i = 0; i < l.size(); ++i) {
            if(method == l[i]) {
                d->methodAndPropertyCacheNeedUpdate = zftrue;
                l.erase(l.begin() + i);
                if(l.empty()) {
                    d->methodMap.erase(itName);
                    break;
                }
                d->methodList.removeElement(method);
            }
        }
    }
}

void ZFClass::_ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty) const {
#if 0
    _ZFP_ZFClassPrivate::methodAndPropertyCacheUpdate(this);
    const ZFProperty *existProp = this->propertyForName(zfproperty->propertyName());
#else
    const ZFProperty *existProp = zfnull;
    {
        ZFCoreQueuePOD<const ZFClass *> toCheck;
        toCheck.add(this);
        do {
            const ZFClass *t = toCheck.take();
            if(t->classParent() != zfnull) {
                toCheck.add(t->classParent());
            }
            toCheck.addFrom(t->d->implementedInterface);

            _ZFP_ZFClassPropertyMapType::iterator it = t->d->propertyMap.find(zfproperty->propertyName());
            if(it != t->d->propertyMap.end()) {
                existProp = it->second;
                break;
            }
        } while(!toCheck.isEmpty());
    }
#endif
    zfCoreAssertWithMessageTrim(existProp == zfnull,
        "class %s already has property named %s",
        this->className(),
        zfproperty->propertyName());

    d->propertyMap[zfproperty->propertyName()] = zfproperty;
    d->propertyList.add(zfproperty);
    d->methodAndPropertyCacheNeedUpdate = zftrue;
}
void ZFClass::_ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty) const {
    d->propertyMap.erase(zfproperty->propertyName());
    d->propertyList.removeElement(zfproperty);
    d->methodAndPropertyCacheNeedUpdate = zftrue;
}

void ZFClass::_ZFP_ZFClass_propertyAutoInitRegister(ZF_IN const ZFProperty *property) const {
    if(property->propertyOwnerClass() == this) {
        return;
    }

    d->propertyAutoInitMap[property] = zftrue;

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->propertyAutoInitMap[property] = zftrue;
    }
}
void ZFClass::_ZFP_ZFClass_propertyAutoInitAction(ZF_IN ZFObject *owner) const {
    // access getter to ensure property value created
    for(zfstlmap<const ZFProperty *, zfbool>::iterator it = d->propertyAutoInitMap.begin(); it != d->propertyAutoInitMap.end(); ++it) {
        const ZFProperty *property = it->first;
        if(property->_ZFP_ZFProperty_callbackEnsureInit) {
            property->_ZFP_ZFProperty_callbackEnsureInit(property, owner);
        }
    }
}
void ZFClass::_ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property) const {
    if(property->propertyOwnerClass() == this) {
        return;
    }

    d->propertyInitStepMap[property][this] = zftrue;

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it) {
        it->first->d->propertyInitStepMap[property][this] = zftrue;
    }
}
zfbool ZFClass::_ZFP_ZFClass_propertyInitStepIsTheSame(
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

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = data->second.begin(), itRef = dataRef->second.begin();
            it != data->second.end();
            ++it, ++itRef
            ) {
        if(it->first != itRef->first) {
            return zffalse;
        }
    }
    return zftrue;
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
        ZF_IN const zfchar *classNamespace
        , ZF_IN const zfchar *className
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
: ZFCoreLibDestroyFlag(zffalse)
, cls(zfnull)
{
    cls = ZFClass::_ZFP_ZFClassRegister(
        &ZFCoreLibDestroyFlag,
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
    ZFClass::_ZFP_ZFClassUnregister(&ZFCoreLibDestroyFlag, this->cls);
}

// ============================================================
void ZFClassGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret
        , ZF_IN_OPT const ZFFilterForZFClass *classFilter /* = zfnull */
        ) {
    zfCoreMutexLocker();
    if(classFilter == zfnull) {
        _ZFP_ZFClassMap.allValueT(ret);
    }
    else {
        const ZFCoreMap &m = _ZFP_ZFClassMap;
        for(zfiterator it = m.iter(); m.iterValid(it); m.iterNext(it)) {
            ZFClass *v = m.iterValue<ZFClass *>(it);
            if(classFilter->filterCheckActive(v)) {
                ret.add(v);
            }
        }
    }
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ClassDataChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFObserver &_ZFP_ZFClassDataChangeObserverRef(void) {
    static ZFObserver d;
    return d;
}
void _ZFP_ZFClassDataChangeNotify(
        ZF_IN ZFClassDataChangeType changeType
        , ZF_IN const ZFClass *changedClass
        , ZF_IN const ZFProperty *changedProperty
        , ZF_IN const ZFMethod *changedMethod
        , ZF_IN_OPT const zfchar *name /* = zfnull */
        ) {
    zfCoreMutexLocker();
    (void)ZFClassDataChangeObserver(); // ensure init order
    (void)ZFGlobalObserver(); // ensure init order
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkLow) == ZFFrameworkStateAvailable) {
        if(changedProperty != zfnull) {
            changedProperty->propertyOwnerClass()->_ZFP_ZFClass_classDataChangeNotify();
        }
        else if(changedMethod != zfnull) {
            if(changedMethod->methodOwnerClass() != zfnull) {
                changedMethod->methodOwnerClass()->_ZFP_ZFClass_classDataChangeNotify();
            }
        }

        v_ZFClassDataChangeData *holder = zfunsafe_zfAlloc(v_ZFClassDataChangeData);
        holder->zfv.changeType = changeType;
        holder->zfv.changedClass = changedClass;
        holder->zfv.changedProperty = changedProperty;
        holder->zfv.changedMethod = changedMethod;
        holder->zfv.name = name;
        ZFClassDataChangeObserver().observerNotify(ZFGlobalEvent::EventClassDataChange(), holder);
        zfunsafe_zfRelease(holder);
    }
}

// ============================================================
void ZFClassAlias(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *aliasName
        ) {
    zfCoreMutexLocker();

    if(cls == zfnull || zfstringIsEmpty(aliasName)
        || cls->classAliasTo().find(aliasName) != zfindexMax()
        || ZFClass::classForName(aliasName, cls->classNamespace()) != zfnull
    ) {
        return;
    }
    zfstring aliasNameFull = cls->classNamespace();
    if(!aliasNameFull.isEmpty()) {
        aliasNameFull += ZFNamespaceSeparator();
    }
    aliasNameFull += aliasName;
    _ZFP_ZFClassMap.set(aliasNameFull, ZFCorePointerForPointerRef<const ZFClass *>(cls));
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classAliasTo.add(aliasName);

    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeClassAliasAttach, cls, zfnull, zfnull, aliasName);
}
void ZFClassAliasRemove(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *aliasName
        ) {
    zfCoreMutexLocker();
    if(cls == zfnull || zfstringIsEmpty(aliasName)) {
        return;
    }
    zfindex index = cls->classAliasTo().find(aliasName);
    if(index == zfindexMax()) {
        return;
    }

    zfstring aliasNameFull = cls->classNamespace();
    if(!aliasNameFull.isEmpty()) {
        aliasNameFull += ZFNamespaceSeparator();
    }
    aliasNameFull += aliasName;
    _ZFP_ZFClassMap.remove(aliasNameFull);
    cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classAliasTo.remove(index);

    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeClassAliasDetach, cls, zfnull, zfnull, aliasName);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFClassGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret)
        , ZFMP_IN_OPT(const ZFFilterForZFClass *, classFilter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArray<const ZFClass *>, ZFClassGetAll
        , ZFMP_IN_OPT(const ZFFilterForZFClass *, classFilter, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFClassAlias
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const zfchar *, aliasName)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFClassAliasRemove
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN(const zfchar *, aliasName)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

