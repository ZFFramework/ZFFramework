#include "ZFDynamicRegisterUtil.h"

#include "ZFSTLWrapper/zfstlmap.h"
#include "ZFSTLWrapper/zfstldeque.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
static zfstlmap<zfstring, ZFDynamic> &_ZFP_ZFDynamicRegTagMap(void) {
    static zfstlmap<zfstring, ZFDynamic> m;
    return m;
}

// ============================================================
zfclassPOD _ZFP_ZFDynamicImplement {
public:
    const ZFClass *cls;
    const ZFClass *clsToImplement;
};
zfclassPOD _ZFP_ZFDynamicPropLifeCycle {
public:
    const ZFProperty *property;
    const ZFClass *ownerClassOrNull;
    ZFPropertyLifeCycle lifeCycle;
};
zfclassLikePOD _ZFP_ZFDynamicRegScopeInfo {
public:
    typedef enum {
        ScopeType_NS,
        ScopeType_class,
        ScopeType_enum,
    } ScopeType;
public:
    zfclassLikePOD EnumInfo {
    public:
        zfstring enumClassName;
        zfuint enumDefault;
        zfbool enumIsFlags;
        zfuint enumValueNext;
        ZFCoreArray<zfuint> enumValues;
        ZFCoreArray<zfstring> enumNames;
    };
    typedef union {
        zfstring *NS; // for NSBegin, full namespace, including nested
        const ZFClass *cls; // for classBegin
        EnumInfo *enumInfo; // for enumBegin
    } D;
public:
    ScopeType scopeType;
    D d;
public:
    const zfchar *scopeNS(void) {
        switch(this->scopeType) {
            case ScopeType_NS:
                return *(d.NS);
            case ScopeType_class:
                return d.cls->classNameFull();
            default:
                return zfnull;
        }
    }
public:
    _ZFP_ZFDynamicRegScopeInfo(ScopeType scopeType)
    : scopeType(scopeType)
    {
        if(scopeType == ScopeType_enum) {
            d.enumInfo = zfnew(EnumInfo);
        }
        else {
            zfmemset(&d, 0, sizeof(D));
        }
    }
    ~_ZFP_ZFDynamicRegScopeInfo(void) {
        switch(scopeType) {
            case ScopeType_NS:
                zfdelete(d.NS);
                break;
            case ScopeType_class:
                break;
            case ScopeType_enum:
                zfdelete(d.enumInfo);
                break;
            default:
                break;
        }
    }
};
zfclassNotPOD _ZFP_ZFDynamicPrivate {
public:
    // global
    zfuint refCount;
    zfbool errorOccurred;
    zfstring regTag;

    // scope
    ZFCoreArray<_ZFP_ZFDynamicRegScopeInfo *> scopeList;

    // state
    ZFCoreArray<const ZFClass *> allClass;
    ZFCoreArray<_ZFP_ZFDynamicImplement> allImplement;
    ZFCoreArray<const ZFClass *> allEnum;
    ZFCoreArray<const ZFMethod *> allMethod;
    ZFCoreArray<const ZFProperty *> allProperty;
    ZFCoreArray<_ZFP_ZFDynamicPropLifeCycle> allPropertyLifeCycle;
    ZFCoreArray<zfidentity> allEvent;
public:
    _ZFP_ZFDynamicPrivate(void)
    : refCount(1)
    , errorOccurred(zffalse)
    , regTag()
    , scopeList()
    , allClass()
    , allImplement()
    , allEnum()
    , allMethod()
    , allProperty()
    , allPropertyLifeCycle()
    , allEvent()
    {
        this->attachGlobal();
    }
    ~_ZFP_ZFDynamicPrivate(void) {
        this->scopeCheck_all();
        while(!this->scopeList.isEmpty()) {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.removeLastAndGet();
            zfdelete(scope);
        }
    }
public:
    void error(ZF_IN const zfstring &errorHint) {
        this->errorOccurred = zftrue;
        if(!ZFDynamic::errorCallbacks().isEmpty()) {
            zfstring s;
            s += errorHint;
            s += "\n";

            for(zfindex i = 0; i < ZFDynamic::errorCallbacks().count(); ++i) {
                ZFDynamic::errorCallbacks()[i].execute(s.cString(), s.length() * sizeof(zfchar));
            }
        }
    }
public:
    zfbool scopeCheck_all(void) {
        if(this->scopeList.isEmpty()) {
            return zftrue;
        }
        else {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.getLast();
            switch(scope->scopeType) {
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS:
                    this->error("have you forgot NSEnd?");
                    return zffalse;
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_class:
                    this->error("have you forgot classEnd?");
                    return zffalse;
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum:
                    this->error("have you forgot enumEnd?");
                    return zffalse;
                default:
                    return zftrue;
            }
        }
    }
    zfbool scopeCheck_NS(void) {
        if(this->scopeList.isEmpty()) {
            return zftrue;
        }
        else {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.getLast();
            switch(scope->scopeType) {
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_class:
                    this->error("have you forgot classEnd?");
                    return zffalse;
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum:
                    this->error("have you forgot enumEnd?");
                    return zffalse;
                default:
                    return zftrue;
            }
        }
    }
    zfbool scopeCheck_class(void) {
        if(this->scopeList.isEmpty()) {
            return zftrue;
        }
        else {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.getLast();
            switch(scope->scopeType) {
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum:
                    this->error("have you forgot enumEnd?");
                    return zffalse;
                default:
                    return zftrue;
            }
        }
    }
    zfbool scopeCheck_enum(void) {
        if(this->scopeList.isEmpty()) {
            return zftrue;
        }
        else {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.getLast();
            switch(scope->scopeType) {
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_class:
                    this->error("have you forgot classEnd?");
                    return zffalse;
                case _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum:
                    this->error("have you forgot enumEnd?");
                    return zffalse;
                default:
                    return zftrue;
            }
        }
    }
public:
    void removeAll(void) {
        if(!this->allEvent.isEmpty()) {
            ZFCoreArray<zfidentity> allEvent = this->allEvent;
            this->allEvent = ZFCoreArray<zfidentity>();
            for(zfindex i = allEvent.count() - 1; i != zfindexMax(); --i) {
                ZFEventDynamicUnregister(allEvent[i]);
            }
        }

        if(!this->allMethod.isEmpty()) {
            ZFCoreArray<const ZFMethod *> allMethod = this->allMethod;
            this->allMethod = ZFCoreArray<const ZFMethod *>();
            for(zfindex i = allMethod.count() - 1; i != zfindexMax(); --i) {
                ZFMethodDynamicUnregister(allMethod[i]);
            }
        }

        if(!this->allPropertyLifeCycle.isEmpty()) {
            ZFCoreArray<_ZFP_ZFDynamicPropLifeCycle> allPropertyLifeCycle = this->allPropertyLifeCycle;
            this->allPropertyLifeCycle = ZFCoreArray<_ZFP_ZFDynamicPropLifeCycle>();
            for(zfindex i = allPropertyLifeCycle.count() - 1; i != zfindexMax(); --i) {
                _ZFP_ZFDynamicPropLifeCycle const &item = allPropertyLifeCycle[i];
                ZFPropertyDynamicUnregisterLifeCycle(item.property, item.ownerClassOrNull, item.lifeCycle);
            }
        }

        if(!this->allProperty.isEmpty()) {
            ZFCoreArray<const ZFProperty *> allProperty = this->allProperty;
            this->allProperty = ZFCoreArray<const ZFProperty *>();
            for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i) {
                ZFPropertyDynamicUnregister(allProperty[i]);
            }
        }

        if(!this->allEnum.isEmpty()) {
            ZFCoreArray<const ZFClass *> allEnum = this->allEnum;
            this->allEnum = ZFCoreArray<const ZFClass *>();
            for(zfindex i = allEnum.count() - 1; i != zfindexMax(); --i) {
                ZFEnumDynamicUnregister(allEnum[i]);
            }
        }

        if(!this->allImplement.isEmpty()) {
            ZFCoreArray<_ZFP_ZFDynamicImplement> allImplement = this->allImplement;
            this->allImplement = ZFCoreArray<_ZFP_ZFDynamicImplement>();
            for(zfindex i = allImplement.count() - 1; i != zfindexMax(); --i) {
                ZFImplementDynamicUnregister(allImplement[i].cls, allImplement[i].clsToImplement);
            }
        }

        if(!this->allClass.isEmpty()) {
            ZFCoreArray<const ZFClass *> allClass = this->allClass;
            this->allClass = ZFCoreArray<const ZFClass *>();
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i) {
                ZFClassDynamicUnregister(allClass[i]);
            }
        }

        this->errorOccurred = zffalse;
        if(this->regTag) {
            _ZFP_ZFDynamicRegTagMap().erase(this->regTag);
            this->regTag.removeAll();
        }

        while(!this->scopeList.isEmpty()) {
            _ZFP_ZFDynamicRegScopeInfo *scope = this->scopeList.removeLastAndGet();
            zfdelete(scope);
        }
    }
public:
    void attachGlobal(void);
};

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFDynamicDataHolder) {
}
public:
    ZFCoreArray<_ZFP_ZFDynamicPrivate *> allImpl;
public:
    void removeAll(void) {
        if(!this->allImpl.isEmpty()) {
            ZFCoreMutexLocker();
            ZFCoreArray<_ZFP_ZFDynamicPrivate *> allImpl = this->allImpl;
            this->allImpl = ZFCoreArray<_ZFP_ZFDynamicPrivate *>();
            for(zfindex i = allImpl.count() - 1; i != zfindexMax(); --i) {
                _ZFP_ZFDynamicPrivate *impl = allImpl[i];
                impl->removeAll();
                if(impl->refCount == 1) {
                    zfdelete(impl);
                }
            }
        }
    }
ZF_STATIC_INITIALIZER_END(ZFDynamicDataHolder)

void _ZFP_ZFDynamicPrivate::attachGlobal(void) {
    ZFCoreMutexLocker();
    ++this->refCount;
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allImpl.add(this);
}

// ============================================================
ZFDynamic::ZFDynamic(void)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
}
ZFDynamic::ZFDynamic(ZF_IN const zfstring &regTag)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
    this->regTag(regTag);
}
ZFDynamic::ZFDynamic(ZF_IN const ZFDynamic &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFDynamic::~ZFDynamic(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFDynamic &ZFDynamic::operator = (ZF_IN const ZFDynamic &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFDynamic::operator == (ZF_IN const ZFDynamic &ref) const {
    return (d == ref.d);
}

void ZFDynamic::exportTag(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OPT zfbool exportScope /* = zffalse */
        , ZF_IN_OPT zfbool exportInternal /* = zffalse */
        ) {
    if(!output) {
        return;
    }

    ZFCoreArray<const ZFClass *> allClass;
    ZFClassGetAllT(allClass);

    ZFCoreArray<const ZFMethod *> allMethod;
    ZFMethodGetAllT(allMethod);

    ZFCoreArray<const ZFTypeInfo *> allTypeId;
    ZFTypeInfoGetAllT(allTypeId);

    ZFCoreArray<zfstring> allNamespace;
    ZFNamespaceGetAllT(allNamespace);

    zfstlmap<zfstring, zfbool> tags;
    const zfchar *zfpFix = "_ZFP_";
    zfindex zfpFixLen = zfslen(zfpFix);
    const zfchar *zfpiFix = "_ZFP_I_";
    zfindex zfpiFixLen = zfslen(zfpiFix);

    for(zfindex i = 0; i < allClass.count(); ++i) {
        const ZFClass *t = allClass[i];
        if(t->classIsInternalPrivate() || (!exportInternal && t->classIsInternal())) {
            continue;
        }
        if(exportScope && t->classNamespace()) {
            tags[t->classNameFull()] = zftrue;
        }
        else {
            tags[t->className()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allMethod.count(); ++i) {
        const ZFMethod *t = allMethod[i];
        if(t->isInternalPrivate() || (!exportInternal && t->isInternal())) {
            continue;
        }
        if(exportScope) {
            if(t->ownerClass() != zfnull) {
                zfstring tag;
                tag += t->ownerClass()->classNameFull();
                tag += ".";
                tag += t->methodName();
                tags[tag] = zftrue;
            }
            else if(t->methodNamespace()) {
                zfstring tag;
                tag += t->methodNamespace();
                tag += ".";
                tag += t->methodName();
                tags[tag] = zftrue;
            }
            else {
                tags[t->methodName()] = zftrue;
            }
        }
        else {
            tags[t->methodName()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allTypeId.count(); ++i) {
        const ZFTypeInfo *t = allTypeId[i];
        if(t->typeIdClass() == zfnull || t->typeIdClass()->classIsInternalPrivate() || (!exportInternal && t->typeIdClass()->classIsInternal())) {
            continue;
        }
        if(zfsncmp(t->typeId(), zfpiFix, zfpiFixLen) == 0 || (!exportInternal && zfsncmp(t->typeId(), zfpFix, zfpFixLen) == 0)) {
            continue;
        }
        tags[t->typeId()] = zftrue;
    }
    for(zfindex i = 0; i < allNamespace.count(); ++i) {
        tags[allNamespace[i]] = zftrue;
    }

    for(zfstlmap<zfstring, zfbool>::iterator it = tags.begin(); it != tags.end(); ++it) {
        output << it->first << "\n";
    }
}

ZFDynamic &ZFDynamic::regTag(ZF_IN const zfstring &regTag) {
    zfstlmap<zfstring, ZFDynamic> &m = _ZFP_ZFDynamicRegTagMap();
    if(!d->regTag.isEmpty()) {
        m.erase(d->regTag);
    }

    if(!regTag) {
        d->regTag.removeAll();
        return *this;
    }

    zfstlmap<zfstring, ZFDynamic>::iterator it = m.find(regTag);
    if(it != m.end() && it->second != *this) {
        // remove may cause unexpect dealloc, retain once
        ZFDynamic holder = it->second;
        holder.removeAll();
    }
    d->regTag = regTag;
    m[regTag] = *this;
    return *this;
}
const zfstring &ZFDynamic::regTag(void) const {
    return d->regTag;
}

void ZFDynamic::removeAll(void) {
    ZFCoreMutexLocker();
    d->removeAll();
}

const ZFCoreArray<const ZFClass *> &ZFDynamic::allClass(void) const {
    return d->allClass;
}
const ZFCoreArray<const ZFClass *> &ZFDynamic::allEnum(void) const {
    return d->allEnum;
}
const ZFCoreArray<const ZFMethod *> &ZFDynamic::allMethod(void) const {
    return d->allMethod;
}
const ZFCoreArray<const ZFProperty *> &ZFDynamic::allProperty(void) const {
    return d->allProperty;
}
const ZFCoreArray<zfidentity> &ZFDynamic::allEvent(void) const {
    return d->allEvent;
}

ZFDynamic &ZFDynamic::classBegin(
        ZF_IN const zfstring &classNameFull
        , ZF_IN const zfstring &parentClassNameFull
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        ) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_class()) {return *this;}
    if(!parentClassNameFull) {
        return this->classBegin(classNameFull, ZFObject::ClassData(), classDynamicRegisterUserData);
    }
    else {
        const ZFClass *classParent = ZFClass::classForName(parentClassNameFull);
        if(classParent != zfnull) {
            return this->classBegin(classNameFull, classParent, classDynamicRegisterUserData);
        }

        _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
        if(scope != zfnull && scope->scopeNS() != zfnull) {
            classParent = ZFClass::classForName(parentClassNameFull, scope->scopeNS());
            if(classParent != zfnull) {
                return this->classBegin(classNameFull, classParent, classDynamicRegisterUserData);
            }
        }

        d->error(zfstr("no such classParent: %s", parentClassNameFull));
        return *this;
    }
}
ZFDynamic &ZFDynamic::classBegin(
        ZF_IN const zfstring &classNameFull
        , ZF_IN_OPT const ZFClass *classParent /* = ZFObject::ClassData() */
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        ) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_class()) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scopePrev = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    const ZFClass *cls = ZFClass::classForName(classNameFull, scopePrev ? scopePrev->scopeNS() : zfnull);
    if(cls != zfnull) {
        _ZFP_ZFDynamicRegScopeInfo *scope = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_class);
        d->scopeList.add(scope);
        scope->d.cls = cls;
    }
    else {
        zfstring errorHint;
        const ZFClass *dynClass = ZFClassDynamicRegister(
            scopePrev && scopePrev->scopeNS() ? zfstr("%s.%s", scopePrev->scopeNS(), classNameFull) : classNameFull,
            classParent, classDynamicRegisterUserData, &errorHint);
        if(dynClass == zfnull) {
            d->error(zfstr("unable to register class: %s, reason: %s"
                        , classNameFull
                        , errorHint
                        ));
        }
        else {
            d->allClass.add(dynClass);

            _ZFP_ZFDynamicRegScopeInfo *scopeNew = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_class);
            d->scopeList.add(scopeNew);
            scopeNew->d.cls = dynClass;
        }
    }
    return *this;
}
ZFDynamic &ZFDynamic::classBegin(ZF_IN const ZFClass *cls) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_class()) {return *this;}
    if(cls == zfnull) {
        d->error("null class");
    }
    else {
        _ZFP_ZFDynamicRegScopeInfo *scopeNew = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_class);
        d->scopeList.add(scopeNew);
        scopeNew->scopeType = _ZFP_ZFDynamicRegScopeInfo::ScopeType_class;
        scopeNew->d.cls = cls;
    }
    return *this;
}
ZFDynamic &ZFDynamic::classEnd(void) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("no paired classBegin");
    }
    else {
        d->scopeList.removeLast();
        zfdelete(scope);
    }
    return *this;
}

ZFDynamic &ZFDynamic::classImplement(ZF_IN const ZFClass *clsToImplement) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("no paired classBegin");
    }
    else {
        ZFImplementDynamicRegister(scope->d.cls, clsToImplement);
    }
    return *this;
}

ZFDynamic &ZFDynamic::classCanAllocPublic(ZF_IN zfbool value) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
    }
    else {
        if(scope->d.cls->classIsDynamicRegister()) {
            scope->d.cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classCanAllocPublic = value;
        }
    }
    return *this;
}

// ============================================================
// onEvent()
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicClassEventDataHolder, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicClassEventDataHolder) {
    if(this->classOnUpdateListener) {
        ZFClassDataUpdateObserver().observerRemove(
            ZFGlobalEvent::EventClassDataUpdate(),
            this->classOnUpdateListener);
    }
}
zfstlmap<const ZFClass *, zfstlmap<zfidentity, zfstldeque<ZFListener> > > classEventMap;
ZFListener classOnUpdateListener;
void classOnUpdateCheckAttach(void) {
    if(!this->classOnUpdateListener) {
        this->classOnUpdateListener = ZFCallbackForFunc(zfself::classOnUpdate);
        ZFClassDataUpdateObserver().observerAdd(
            ZFGlobalEvent::EventClassDataUpdate(),
            this->classOnUpdateListener);
    }
}
static void classOnUpdate(ZF_IN const ZFArgs &zfargs) {
    ZFClassDataUpdateData const &data = zfargs.param0().to<v_ZFClassDataUpdateData *>()->zfv;
    if(data.changeType == ZFClassDataUpdateTypeDetach && data.changedClass != zfnull) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicClassEventDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicClassEventDataHolder);
        d->classEventMap.erase(data.changedClass);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicClassEventDataHolder)

ZFDynamic &ZFDynamic::onEvent(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &callback
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(eventId == zfidentityInvalid()) {
        d->error("invalid eventId");
        return *this;
    }
    if(!callback) {
        d->error("invalid callback");
        return *this;
    }
    const ZFClass *cls = scope->d.cls;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicClassEventDataHolder) *g = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicClassEventDataHolder);
    zfstlmap<const ZFClass *, zfstlmap<zfidentity, zfstldeque<ZFListener> > >::iterator it = g->classEventMap.find(cls);
    if(it == g->classEventMap.end()) {
        g->classEventMap[cls][eventId].push_back(callback);

        ZFLISTENER_2(instanceOnCreate
                , const ZFClass *, cls
                , ZFLevel, level
                ) {
            ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicClassEventDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicClassEventDataHolder);
            zfstlmap<const ZFClass *, zfstlmap<zfidentity, zfstldeque<ZFListener> > >::iterator itClass = d->classEventMap.find(cls);
            if(itClass == d->classEventMap.end()) {
                return;
            }
            ZFObject *obj = zfargs.sender();
            zfstlmap<zfidentity, zfstldeque<ZFListener> > &eventMap = itClass->second;
            for(zfstlmap<zfidentity, zfstldeque<ZFListener> >::iterator itEvent = eventMap.begin(); itEvent != eventMap.end(); ++itEvent) {
                for(zfstlsize i = 0; i < itEvent->second.size(); ++i) {
                    obj->observerAdd(itEvent->first, itEvent->second[i], level);
                }
            }
        } ZFLISTENER_END()
        cls->instanceObserverAdd(instanceOnCreate, zfnull, ZFLevelZFFrameworkNormal);
    }
    else {
        it->second[eventId].push_back(callback);
    }
    g->classOnUpdateCheckAttach();
    return *this;
}

ZFDynamic &ZFDynamic::NSBegin(ZF_IN const zfstring &methodNamespace) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_NS()) {return *this;}
    const zfchar *methodNamespaceTmp = ZFNamespaceSkipGlobal(methodNamespace);
    if(!methodNamespaceTmp) {
        d->error("empty namespace or NSBegin on global namespace");
        return *this;
    }
    _ZFP_ZFDynamicRegScopeInfo *scopePrev = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    _ZFP_ZFDynamicRegScopeInfo *scope = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS);
    d->scopeList.add(scope);
    if(scopePrev != zfnull) {
        scope->d.NS = zfnew(zfstring);
        zfstring &tmp = *(scope->d.NS);
        tmp += scope->scopeNS();
        tmp += ".";
        tmp += methodNamespaceTmp;
    }
    else {
        scope->d.NS = zfnew(zfstring, methodNamespaceTmp);
    }
    return *this;
}
ZFDynamic &ZFDynamic::NSEnd(void) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS) {
        d->error("no paired NSBegin");
    }
    else {
        d->scopeList.removeLast();
        zfdelete(scope);
    }
    return *this;
}

ZFDynamic &ZFDynamic::enumBegin(ZF_IN const zfstring &enumClassName) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_enum()) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scopePrev = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    _ZFP_ZFDynamicRegScopeInfo *scope = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS);
    d->scopeList.add(scope);
    if(scopePrev != zfnull) {
        scope->d.enumInfo->enumClassName += scopePrev->scopeNS();
        scope->d.enumInfo->enumClassName += ".";
    }
    scope->d.enumInfo->enumClassName += enumClassName;
    scope->d.enumInfo->enumIsFlags = zffalse;
    return *this;
}
ZFDynamic &ZFDynamic::enumBeginFlags(ZF_IN const zfstring &enumClassName) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeCheck_enum()) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scopePrev = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    _ZFP_ZFDynamicRegScopeInfo *scope = zfnew(_ZFP_ZFDynamicRegScopeInfo, _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS);
    d->scopeList.add(scope);
    if(scopePrev != zfnull) {
        scope->d.enumInfo->enumClassName += scopePrev->scopeNS();
        scope->d.enumInfo->enumClassName += ".";
    }
    scope->d.enumInfo->enumClassName += enumClassName;
    scope->d.enumInfo->enumIsFlags = zftrue;
    return *this;
}
ZFDynamic &ZFDynamic::enumValue(
        ZF_IN const zfstring &enumName
        , ZF_IN_OPT zfuint enumValue /* = ZFEnumInvalid() */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum) {
        d->error("have you forgot enumBegin?");
        return *this;
    }
    if(enumValue == ZFEnumInvalid()) {
        enumValue = scope->d.enumInfo->enumValueNext;
        ++(scope->d.enumInfo->enumValueNext);
    }
    else {
        if(scope->d.enumInfo->enumValueNext <= enumValue) {
            scope->d.enumInfo->enumValueNext = enumValue + 1;
        }
    }
    scope->d.enumInfo->enumValues.add(enumValue);
    scope->d.enumInfo->enumNames.add(enumName);
    return *this;
}
ZFDynamic &ZFDynamic::enumEnd(ZF_IN_OPT zfuint enumDefault /* = ZFEnumInvalid() */) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum) {
        d->error("have you forgot enumBegin?");
        return *this;
    }
    d->scopeList.removeLast();
    zfblockedDelete(scope);

    zfstring errorHint;
    const ZFClass *enumClass = ZFEnumDynamicRegister(
        scope->d.enumInfo->enumClassName,
        scope->d.enumInfo->enumValues,
        scope->d.enumInfo->enumNames,
        scope->d.enumInfo->enumDefault,
        scope->d.enumInfo->enumIsFlags,
        &errorHint);
    if(enumClass == zfnull) {
        d->error(zfstr("unable to register enum, reason: %s", errorHint));
        return *this;
    }
    d->allEnum.add(enumClass);
    return *this;
}

static zfbool _ZFP_ZFDynamicEventGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 0
                )) {
        return zffalse;
    }
    ret = invokerMethod->dynamicRegisterUserData();
    return zftrue;
}
ZFDynamic &ZFDynamic::event(ZF_IN const zfstring &eventName) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope != zfnull && scope->scopeType == _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum) {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(!eventName) {
        d->error("empty event name");
        return *this;
    }
    zfstring idName;
    const ZFClass *cls = zfnull;
    zfstring NS;
    if(scope == zfnull) {
        NS = zftext("ZFGlobalEvent");
        idName += NS;
    }
    else if(scope->scopeType == _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS) {
        idName += *(scope->d.NS);
        NS = *(scope->d.NS);
    }
    else if(scope->scopeType == _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        idName += scope->d.cls->classNameFull();
        cls = scope->d.cls;
    }
    idName += ".Event";
    idName += eventName;
    zfidentity idValue = ZFEventIdForName(idName);
    if(idValue != zfidentityInvalid()) {
        d->error(zfstr("%s already exists", idName));
        return *this;
    }
    idValue = ZFEventDynamicRegister(idName);
    d->allEvent.add(idValue);

    zfobj<v_zfidentity> t;
    t->zfv = idValue;
    const ZFMethod *method = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFDynamicEventGI)
            .dynamicRegisterUserData(t)
            .ownerClass(cls)
            .methodNamespace(NS)
            .methodName(zfstr("Event%s", eventName))
            .returnTypeId(ZFTypeId_zfidentity())
        );
    ZFCoreAssert(method != zfnull);
    d->allMethod.add(method);

    return *this;
}

ZFDynamic &ZFDynamic::method(
        ZF_IN const zfstring &returnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType /* = ZFMethodTypeVirtual */
        , ZF_IN_OPT ZFMethodPrivilegeType methodPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    ZFMethodDynamicRegisterParam p;
    p.returnTypeId(returnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    p.methodType(methodType);
    p.methodPrivilegeType(methodPrivilegeType);
    for(zfindex i = 0; i < methodParam.paramCount(); ++i) {
        p.methodParam(
            methodParam.paramTypeIdAt(i),
            methodParam.paramNameAt(i),
            methodParam.paramDefaultValueCallbackAt(i));
    }
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope != zfnull) {
        switch(scope->scopeType) {
            case _ZFP_ZFDynamicRegScopeInfo::ScopeType_NS:
                p.methodNamespace(*(scope->d.NS));
                break;
            case _ZFP_ZFDynamicRegScopeInfo::ScopeType_class:
                p.ownerClass(scope->d.cls);
                break;
            case _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum:
            default:
                break;
        }
    }
    return this->method(p);
}

ZFDynamic &ZFDynamic::method(ZF_IN const ZFMethodDynamicRegisterParam &param) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope != zfnull && scope->scopeType == _ZFP_ZFDynamicRegScopeInfo::ScopeType_enum) {
        d->error("can not be called within enumBegin");
        return *this;
    }
    zfstring errorHint;
    const ZFMethod *dynMethod = ZFMethodDynamicRegister(param, &errorHint);
    if(dynMethod == zfnull) {
        d->error(zfstr("unable to register method, reason: %s", errorHint));
    }
    else {
        d->allMethod.add(dynMethod);
    }
    return *this;
}

static zfbool _ZFP_ZFDynamicSingletonGI_getter(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 0
                )) {
        return zffalse;
    }
    ret = invokerMethod->ownerClass()->classTag("_ZFP_ZFDynamicSingleton");
    if(!ret) {
        ret = invokerMethod->ownerClass()->newInstance();
        invokerMethod->ownerClass()->classTag("_ZFP_ZFDynamicSingleton", ret);
    }
    return zftrue;
}
static zfbool _ZFP_ZFDynamicSingletonGI_setter(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    if(!ZFMethodGenericInvokerParamsCheck(errorHint, paramCount, paramList
                , 1
                , invokerMethod->ownerClass()
                )) {
        return zffalse;
    }
    invokerMethod->ownerClass()->classTag("_ZFP_ZFDynamicSingleton"
            , paramList[0]
            );
    return zftrue;
}
ZFDynamic &ZFDynamic::singleton(ZF_IN_OPT const zfstring &methodName /* = zftext("instance") */) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("can only be called within classBegin");
        return *this;
    }
    const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFDynamicSingletonGI_getter)
            .ownerClass(scope->d.cls)
            .methodName(methodName)
            .returnTypeId(scope->d.cls->classNameFull())
            );
    if(getterMethod == zfnull) {
        d->error("unable to register singleton getter");
        return *this;
    }
    const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFDynamicSingletonGI_setter)
            .ownerClass(scope->d.cls)
            .methodName(methodName)
            .methodParam(scope->d.cls->classNameFull())
            );
    if(setterMethod == zfnull) {
        ZFMethodDynamicUnregister(getterMethod);
        d->error("unable to register singleton setter");
        return *this;
    }
    d->allMethod.add(getterMethod);
    d->allMethod.add(setterMethod);
    return *this;
}

static zfauto _ZFP_ZFDynamicPropertyInit(ZF_IN const ZFProperty *property) {
    ZFCopyable *copyable = property->dynamicRegisterUserData();
    if(copyable != zfnull) {
        return copyable->copy();
    }
    ZFStyleable *styleable = property->dynamicRegisterUserData();
    if(styleable != zfnull) {
        zfauto ret = styleable->classData()->newInstance();
        ret.to<ZFStyleable *>()->styleableCopyFrom(styleable);
        return ret;
    }
    return zfnull;
}
ZFDynamic &ZFDynamic::property(
        ZF_IN const zfstring &propertyTypeId
        , ZF_IN const zfstring &propertyName
        , ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    const ZFClass *cls = ZFClass::classForName(propertyTypeId);
    if(cls != zfnull && ZFTypeInfoForName(propertyTypeId) == zfnull) {
        return this->property(cls, propertyName, propertyInitValue, setterPrivilegeType, getterPrivilegeType);
    }
    ZFPropertyDynamicRegisterParam param;
    param.ownerClass(scope->d.cls);
    param.propertyTypeId(propertyTypeId);
    param.propertyName(propertyName);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull) {
        ZFTypeIdWrapper *propertyInitValueWrapper = zfcast(ZFTypeIdWrapper *, propertyInitValue);
        if(propertyInitValueWrapper == zfnull) {
            d->error(zfstr("assign property's type must be %s: %s"
                        , ZFTypeIdWrapper::ClassData()->classNameFull()
                        , propertyInitValue
                        ));
            return *this;
        }
        if(propertyInitValue->classData()->classIsTypeOf(cls)) {
            param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
            param.dynamicRegisterUserData(propertyInitValue);
        }
        else {
            // try to convert by construct new value
            zfauto wrap;
            zfstring errorHint;
            zfauto paramList[ZFMETHOD_MAX_PARAM];
            paramList[0] = propertyInitValue;
            if(!ZFDI_alloc(wrap, &errorHint, cls, 1, paramList)
                    || !wrap
                    ) {
                d->error(zfstr("invalid init value \"%s\" for assign property: (%s)%s"
                            , propertyInitValue
                            , cls->classNameFull()
                            , propertyName
                            ));
                return *this;
            }
            param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
            param.dynamicRegisterUserData(wrap);
        }
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(
        ZF_IN const ZFClass *propertyClassOfRetainProperty
        , ZF_IN const zfstring &propertyName
        , ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(propertyClassOfRetainProperty == zfnull) {
        d->error("propertyClassOfRetainProperty not set");
        return *this;
    }
    ZFPropertyDynamicRegisterParam param;
    param.ownerClass(scope->d.cls);
    param.propertyTypeId(propertyClassOfRetainProperty->classNameFull());
    param.propertyName(propertyName);
    param.propertyClassOfRetainProperty(propertyClassOfRetainProperty);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull) {
        if(!propertyInitValue->classData()->classIsTypeOf(propertyClassOfRetainProperty)) {
            d->error(zfstr("init value %s is not type of %s"
                        , propertyInitValue
                        , propertyClassOfRetainProperty->classNameFull()
                        ));
            return *this;
        }
        if(zfcast(ZFCopyable *, propertyInitValue) == zfnull
                && zfcast(ZFStyleable *, propertyInitValue) == zfnull
                ) {
            d->error(zfstr("init value %s is not type of %s or %s, use propertyWithInit instead"
                        , propertyInitValue
                        , ZFCopyable::ClassData()->classNameFull()
                        , ZFStyleable::ClassData()->classNameFull()
                        ));
            return *this;
        }
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
        param.dynamicRegisterUserData(propertyInitValue);
    }
    return this->property(param);
}
static zfauto _ZFP_ZFDynamicPropertyInitGetter(ZF_IN const ZFProperty *property) {
    v_ZFListener *impl = property->dynamicRegisterUserData();
    if(impl) {
        ZFListener l = impl->zfv;
        ZFArgs zfargs;
        zfargs.sender(zfobj<v_ZFProperty>(property));
        l.execute(zfargs);
        return zfargs.result();
    }
    return zfnull;
}
ZFDynamic &ZFDynamic::propertyWithInit(
        ZF_IN const zfstring &propertyTypeId
        , ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &propertyInitValue
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    const ZFClass *cls = ZFClass::classForName(propertyTypeId);
    if(cls != zfnull && ZFTypeInfoForName(propertyTypeId) == zfnull) {
        return this->propertyWithInit(cls, propertyName, propertyInitValue, setterPrivilegeType, getterPrivilegeType);
    }
    ZFPropertyDynamicRegisterParam param;
    param.ownerClass(scope->d.cls);
    param.propertyTypeId(propertyTypeId);
    param.propertyName(propertyName);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull) {
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInitGetter);
        param.dynamicRegisterUserData(zfobj<v_ZFListener>(propertyInitValue));
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::propertyWithInit(
        ZF_IN const ZFClass *propertyClassOfRetainProperty
        , ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &propertyInitValue
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(propertyClassOfRetainProperty == zfnull) {
        d->error("propertyClassOfRetainProperty not set");
        return *this;
    }
    ZFPropertyDynamicRegisterParam param;
    param.ownerClass(scope->d.cls);
    param.propertyTypeId(propertyClassOfRetainProperty->classNameFull());
    param.propertyName(propertyName);
    param.propertyClassOfRetainProperty(propertyClassOfRetainProperty);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull) {
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInitGetter);
        param.dynamicRegisterUserData(zfobj<v_ZFListener>(propertyInitValue));
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFPropertyDynamicRegisterParam &param) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    zfstring errorHint;
    const ZFProperty *dynProperty = ZFPropertyDynamicRegister(param, &errorHint);
    if(dynProperty == zfnull) {
        d->error(zfstr("unable to register property, reason: %s", errorHint));
    }
    else {
        d->allProperty.add(dynProperty);
    }
    return *this;
}

ZFDynamic &ZFDynamic::propertyOnInit(
        ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnInit, callback);
}
ZFDynamic &ZFDynamic::propertyOnVerify(
        ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnVerify, callback);
}
ZFDynamic &ZFDynamic::propertyOnAttach(
        ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnAttach, callback);
}
ZFDynamic &ZFDynamic::propertyOnDetach(
        ZF_IN const zfstring &propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnDetach, callback);
}

ZFDynamic &ZFDynamic::propertyLifeCycle(
        ZF_IN const zfstring &propertyName
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        , ZF_IN const ZFListener &callback
        ) {
    if(d->errorOccurred) {return *this;}
    _ZFP_ZFDynamicRegScopeInfo *scope = d->scopeList.isEmpty() ? zfnull : d->scopeList.getLast();
    if(scope == zfnull || scope->scopeType != _ZFP_ZFDynamicRegScopeInfo::ScopeType_class) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    _ZFP_ZFDynamicPropLifeCycle item;
    item.property = scope->d.cls->propertyForName(propertyName);
    item.ownerClassOrNull = scope->d.cls;
    item.lifeCycle = lifeCycle;
    if(item.property == zfnull) {
        d->error(zfstr("no property %s for class %s", propertyName, scope->d.cls->className()));
        return *this;
    }
    zfstring errorHint;
    if(!ZFPropertyDynamicRegisterLifeCycle(item.property, scope->d.cls, lifeCycle, callback)) {
        d->error(zfstr("%s", errorHint));
        return *this;
    }
    d->allPropertyLifeCycle.add(item);
    return *this;
}

ZFCoreArray<ZFOutput> &ZFDynamic::errorCallbacks(void) {
    static ZFCoreArray<ZFOutput> d;
    return d;
}

void ZFDynamic::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFDynamic";

    if(!d->regTag.isEmpty()) {
        ret += " regTag:";
        ret += d->regTag;
    }

    if(!d->allClass.isEmpty()) {
        ret += " allClass:";
        d->allClass.objectInfoT(ret);
    }

    if(!d->allEnum.isEmpty()) {
        ret += " allEnum:";
        d->allEnum.objectInfoT(ret);
    }

    if(!d->allMethod.isEmpty()) {
        ret += " allMethod:";
        d->allMethod.objectInfoT(ret);
    }

    if(!d->allProperty.isEmpty()) {
        ret += " allProperty:";
        d->allProperty.objectInfoT(ret);
    }

    if(!d->allEvent.isEmpty()) {
        ret += " allEvent:";
        d->allEvent.objectInfoT(ret);
    }

    ret += ">";
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicErrorCallbacks, ZFLevelZFFrameworkNormal) {
    ZFDynamic::errorCallbacks().add(ZFOutputDefault());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicErrorCallbacks) {
    ZFDynamic::errorCallbacks().removeAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicErrorCallbacks)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFDynamicRemoveAll) {
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->removeAll();
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicRemoveAllAutoNotify, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicRemoveAllAutoNotify) {
    ZFDynamicRemoveAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicRemoveAllAutoNotify)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFDynamic, ZFDynamic)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_3(ZFDynamic, v_ZFDynamic, void, exportTag
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN_OPT(zfbool, exportScope, zffalse)
        , ZFMP_IN_OPT(zfbool, exportInternal, zffalse)
        )
ZFOBJECT_ON_INIT_USER_REGISTER_1({
        ZFDynamic &v = invokerObject.to<v_ZFDynamic *>()->zfv;
        v.regTag(regTag);
    }, v_ZFDynamic
    , ZFMP_IN(const zfstring &, regTag)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, regTag
        , ZFMP_IN(const zfstring &, regTag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const zfstring &, regTag)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArray<const ZFClass *> &, allClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArray<const ZFClass *> &, allEnum)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArray<const ZFMethod *> &, allMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArray<const ZFProperty *> &, allProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArray<zfidentity> &, allEvent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin
        , ZFMP_IN(const zfstring &, classNameFull)
        , ZFMP_IN(const zfstring &, parentClassNameFull)
        , ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin
        , ZFMP_IN(const zfstring &, classNameFull)
        , ZFMP_IN_OPT(const ZFClass *, classParent, ZFObject::ClassData())
        , ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, classBegin
        , ZFMP_IN(const ZFClass *, cls)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, classImplement
        , ZFMP_IN(const ZFClass *, clsToImplement)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, classEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, classCanAllocPublic
        , ZFMP_IN(zfbool, value)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, onEvent
        , ZFMP_IN(zfidentity, eventId)
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(ZFLevel, level, ZFLevelAppNormal)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, onInit
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, onDealloc
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, NSBegin
        , ZFMP_IN(const zfstring &, methodNamespace)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, NSEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumBegin
        , ZFMP_IN(const zfstring &, enumClassName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumBeginFlags
        , ZFMP_IN(const zfstring &, enumClassName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, enumValue
        , ZFMP_IN(const zfstring &, enumName)
        , ZFMP_IN_OPT(zfuint, enumValue, ZFEnumInvalid())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumEnd
        , ZFMP_IN_OPT(zfuint, enumDefault, ZFEnumInvalid())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, event
        , ZFMP_IN(const zfstring &, eventName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(v_ZFDynamic, ZFDynamic &, method
        , ZFMP_IN(const zfstring &, returnTypeId)
        , ZFMP_IN(const zfstring &, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        , ZFMP_IN_OPT(ZFMethodType, methodType, ZFMethodTypeVirtual)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, methodPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, method
        , ZFMP_IN(const ZFMethodDynamicRegisterParam &, param)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, singleton
        , ZFMP_IN_OPT(const zfstring &, methodName, zftext("instance"))
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const zfstring &, propertyTypeId)
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty)
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, propertyWithInit
        , ZFMP_IN(const zfstring &, propertyTypeId)
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, propertyInitValue)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, propertyWithInit
        , ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty)
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, propertyInitValue)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnInit
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnVerify
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnAttach
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnDetach
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFCoreArray<ZFOutput> &, errorCallbacks)

ZF_NAMESPACE_GLOBAL_END

