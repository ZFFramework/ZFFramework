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
zfclassNotPOD _ZFP_ZFDynamicPrivate {
public:
    // global
    zfuint refCount;
    zfbool errorOccurred;
    zfstring regTag;

    // scope
    const ZFClass *cls;
    zfstring methodNamespace;
    zfstring enumClassName;

    // enumBegin
    zfuint enumDefault;
    zfbool enumIsFlags;
    zfuint enumValueNext;
    ZFCoreArrayPOD<zfuint> enumValues;
    ZFCoreArray<zfstring> enumNames;

    // state
    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFCoreArrayPOD<_ZFP_ZFDynamicImplement> allImplement;
    ZFCoreArrayPOD<const ZFClass *> allEnum;
    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
    ZFCoreArrayPOD<_ZFP_ZFDynamicPropLifeCycle> allPropertyLifeCycle;
    ZFCoreArrayPOD<zfidentity> allEvent;
public:
    _ZFP_ZFDynamicPrivate(void)
    : refCount(1)
    , errorOccurred(zffalse)
    , regTag()
    , cls(zfnull)
    , methodNamespace()
    , enumClassName()
    , enumDefault(ZFEnumInvalid())
    , enumIsFlags(zffalse)
    , enumValueNext(0)
    , enumValues()
    , enumNames()
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
        this->scopeBeginCheck();
    }
public:
    void error(ZF_IN const zfchar *errorHint) {
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
    zfbool scopeBeginCheck(void) {
        if(this->cls != zfnull) {
            this->error("have you forgot classEnd?");
            return zffalse;
        }
        else if(!this->methodNamespace.isEmpty()) {
            this->error("have you forgot NSEnd?");
            return zffalse;
        }
        else if(!this->enumClassName.isEmpty()) {
            this->error("have you forgot enumEnd?");
            return zffalse;
        }
        else {
            return zftrue;
        }
    }
public:
    void removeAll(void) {
        if(!this->allEvent.isEmpty()) {
            ZFCoreArrayPOD<zfidentity> allEvent = this->allEvent;
            this->allEvent = ZFCoreArrayPOD<zfidentity>();
            for(zfindex i = allEvent.count() - 1; i != zfindexMax(); --i) {
                ZFIdMapDynamicUnregister(allEvent[i]);
            }
        }

        if(!this->allMethod.isEmpty()) {
            ZFCoreArrayPOD<const ZFMethod *> allMethod = this->allMethod;
            this->allMethod = ZFCoreArrayPOD<const ZFMethod *>();
            for(zfindex i = allMethod.count() - 1; i != zfindexMax(); --i) {
                ZFMethodDynamicUnregister(allMethod[i]);
            }
        }

        if(!this->allPropertyLifeCycle.isEmpty()) {
            ZFCoreArrayPOD<_ZFP_ZFDynamicPropLifeCycle> allPropertyLifeCycle = this->allPropertyLifeCycle;
            this->allPropertyLifeCycle = ZFCoreArrayPOD<_ZFP_ZFDynamicPropLifeCycle>();
            for(zfindex i = allPropertyLifeCycle.count() - 1; i != zfindexMax(); --i) {
                _ZFP_ZFDynamicPropLifeCycle const &item = allPropertyLifeCycle[i];
                ZFPropertyDynamicUnregisterLifeCycle(item.property, item.ownerClassOrNull, item.lifeCycle);
            }
        }

        if(!this->allProperty.isEmpty()) {
            ZFCoreArrayPOD<const ZFProperty *> allProperty = this->allProperty;
            this->allProperty = ZFCoreArrayPOD<const ZFProperty *>();
            for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i) {
                ZFPropertyDynamicUnregister(allProperty[i]);
            }
        }

        if(!this->allEnum.isEmpty()) {
            ZFCoreArrayPOD<const ZFClass *> allEnum = this->allEnum;
            this->allEnum = ZFCoreArrayPOD<const ZFClass *>();
            for(zfindex i = allEnum.count() - 1; i != zfindexMax(); --i) {
                ZFEnumDynamicUnregister(allEnum[i]);
            }
        }

        if(!this->allImplement.isEmpty()) {
            ZFCoreArrayPOD<_ZFP_ZFDynamicImplement> allImplement = this->allImplement;
            this->allImplement = ZFCoreArrayPOD<_ZFP_ZFDynamicImplement>();
            for(zfindex i = allImplement.count() - 1; i != zfindexMax(); --i) {
                ZFImplementDynamicUnregister(allImplement[i].cls, allImplement[i].clsToImplement);
            }
        }

        if(!this->allClass.isEmpty()) {
            ZFCoreArrayPOD<const ZFClass *> allClass = this->allClass;
            this->allClass = ZFCoreArrayPOD<const ZFClass *>();
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i) {
                ZFClassDynamicUnregister(allClass[i]);
            }
        }

        this->errorOccurred = zffalse;
        if(!this->regTag.isEmpty()) {
            _ZFP_ZFDynamicRegTagMap().erase(this->regTag);
            this->regTag.removeAll();
        }
        this->cls = zfnull;
        this->enumClassName.removeAll();
        this->enumDefault = ZFEnumInvalid();
        this->enumIsFlags = zffalse;
        this->enumValueNext = 0;
        this->enumValues.removeAll();
        this->enumNames.removeAll();
    }
public:
    void attachGlobal(void);
};

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFDynamicDataHolder) {
}
public:
    ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *> allImpl;
public:
    void removeAll(void) {
        if(!this->allImpl.isEmpty()) {
            zfCoreMutexLocker();
            ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *> allImpl = this->allImpl;
            this->allImpl = ZFCoreArrayPOD<_ZFP_ZFDynamicPrivate *>();
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
    zfCoreMutexLocker();
    ++this->refCount;
    ZF_STATIC_INITIALIZER_INSTANCE(ZFDynamicDataHolder)->allImpl.add(this);
}

// ============================================================
ZFDynamic::ZFDynamic(void)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
}
ZFDynamic::ZFDynamic(ZF_IN const zfchar *regTag)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
    this->regTag(regTag);
}
ZFDynamic::ZFDynamic(ZF_IN ZFObject *regTag)
: d(zfnew(_ZFP_ZFDynamicPrivate))
{
    if(regTag != zfnull) {
        this->regTag(regTag->objectInfo());
    }
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

    ZFCoreArrayPOD<const ZFClass *> allClass;
    ZFClassGetAllT(allClass);

    ZFCoreArrayPOD<const ZFMethod *> allMethod;
    ZFMethodGetAllT(allMethod);

    ZFCoreArrayPOD<const ZFTypeInfo *> allTypeId;
    ZFTypeInfoGetAllT(allTypeId);

    ZFCoreArrayPOD<const zfchar *> allNamespace;
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
        if(exportScope && !zfstringIsEmpty(t->classNamespace())) {
            tags[t->classNameFull()] = zftrue;
        }
        else {
            tags[t->className()] = zftrue;
        }
    }
    for(zfindex i = 0; i < allMethod.count(); ++i) {
        const ZFMethod *t = allMethod[i];
        if(t->methodIsInternalPrivate() || (!exportInternal && t->methodIsInternal())) {
            continue;
        }
        if(exportScope) {
            if(t->methodOwnerClass() != zfnull) {
                zfstring tag;
                tag += t->methodOwnerClass()->classNameFull();
                tag += ".";
                tag += t->methodName();
                tags[tag] = zftrue;
            }
            else if(!zfstringIsEmpty(t->methodNamespace())) {
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

ZFDynamic &ZFDynamic::regTag(ZF_IN const zfchar *regTag) {
    zfstlmap<zfstring, ZFDynamic> &m = _ZFP_ZFDynamicRegTagMap();
    if(!d->regTag.isEmpty()) {
        m.erase(d->regTag);
    }

    if(zfstringIsEmpty(regTag)) {
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
const zfchar *ZFDynamic::regTag(void) const {
    return (d->regTag.isEmpty() ? zfnull : d->regTag.cString());
}
ZFDynamic &ZFDynamic::regTag(ZF_IN ZFObject *regTag) {
    if(regTag != zfnull) {
        this->regTag(regTag->objectInfo());
    }
    else {
        this->regTag("");
    }
    return *this;
}

void ZFDynamic::removeAll(void) {
    zfCoreMutexLocker();
    d->removeAll();
}

const ZFCoreArrayPOD<const ZFClass *> &ZFDynamic::allClass(void) const {
    return d->allClass;
}
const ZFCoreArrayPOD<const ZFClass *> &ZFDynamic::allEnum(void) const {
    return d->allEnum;
}
const ZFCoreArrayPOD<const ZFMethod *> &ZFDynamic::allMethod(void) const {
    return d->allMethod;
}
const ZFCoreArrayPOD<const ZFProperty *> &ZFDynamic::allProperty(void) const {
    return d->allProperty;
}
const ZFCoreArrayPOD<zfidentity> &ZFDynamic::allEvent(void) const {
    return d->allEvent;
}

ZFDynamic &ZFDynamic::classBegin(
        ZF_IN const zfchar *classNameFull
        , ZF_IN_OPT const ZFClass *classParent /* = ZFObject::ClassData() */
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        ) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->cls = ZFClass::classForName(classNameFull);
    if(d->cls == zfnull) {
        zfstring errorHint;
        const ZFClass *dynClass = ZFClassDynamicRegister(
            classNameFull, classParent, classDynamicRegisterUserData, &errorHint);
        if(dynClass == zfnull) {
            d->error(zfstr("unable to register class: %s, reason: %s"
                        , classNameFull
                        , errorHint
                        ));
        }
        else {
            d->allClass.add(dynClass);
            d->cls = dynClass;
        }
    }
    return *this;
}
ZFDynamic &ZFDynamic::classBegin(
        ZF_IN const zfchar *classNameFull
        , ZF_IN const zfchar *parentClassNameFull
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */
        ) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfstringIsEmpty(parentClassNameFull)) {
        return this->classBegin(classNameFull, ZFObject::ClassData(), classDynamicRegisterUserData);
    }
    else {
        const ZFClass *classParent = ZFClass::classForName(parentClassNameFull);
        if(classParent == zfnull) {
            d->error(zfstr("no such classParent: %s", parentClassNameFull));
            return *this;
        }
        else {
            return this->classBegin(classNameFull, classParent, classDynamicRegisterUserData);
        }
    }
}
ZFDynamic &ZFDynamic::classBegin(ZF_IN const ZFClass *cls) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->cls = cls;
    if(d->cls == zfnull) {
        d->error("null class");
    }
    return *this;
}
ZFDynamic &ZFDynamic::classEnd(void) {
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull) {
        d->error("no paired classBegin");
    }
    d->cls = zfnull;
    return *this;
}

ZFDynamic &ZFDynamic::classImplement(ZF_IN const ZFClass *clsToImplement) {
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull) {
        d->error("no paired classBegin");
    }
    ZFImplementDynamicRegister(d->cls, clsToImplement);
    return *this;
}

ZFDynamic &ZFDynamic::classCanAllocPublic(ZF_IN zfbool value) {
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    if(d->cls->classIsDynamicRegister()) {
        d->cls->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_classCanAllocPublic = value;
    }
    return *this;
}

// ============================================================
// on()
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFDynamicClassEventDataHolder, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFDynamicClassEventDataHolder) {
    if(this->classOnChangeListener) {
        ZFClassDataChangeObserver().observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            this->classOnChangeListener);
    }
}
zfstlmap<const ZFClass *, zfstlmap<zfidentity, zfstldeque<ZFListener> > > classEventMap;
ZFListener classOnChangeListener;
void classOnChangeCheckAttach(void) {
    if(!this->classOnChangeListener) {
        this->classOnChangeListener = ZFCallbackForFunc(zfself::classOnChange);
        ZFClassDataChangeObserver().observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            this->classOnChangeListener);
    }
}
static void classOnChange(ZF_IN const ZFArgs &zfargs) {
    const ZFClassDataChangeData &data = zfargs.param0()->to<v_ZFClassDataChangeData *>()->zfv;
    if(data.changeType == ZFClassDataChangeTypeDetach && data.changedClass != zfnull) {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicClassEventDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicClassEventDataHolder);
        d->classEventMap.erase(data.changedClass);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFDynamicClassEventDataHolder)

ZFDynamic &ZFDynamic::on(
        ZF_IN zfidentity eventId
        , ZF_IN const ZFListener &callback
        , ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */
        ) {
    if(d->errorOccurred) {return *this;}
    if(d->cls == zfnull) {
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

    ZF_GLOBAL_INITIALIZER_CLASS(ZFDynamicClassEventDataHolder) *g = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFDynamicClassEventDataHolder);
    zfstlmap<const ZFClass *, zfstlmap<zfidentity, zfstldeque<ZFListener> > >::iterator it = g->classEventMap.find(d->cls);
    if(it == g->classEventMap.end()) {
        g->classEventMap[d->cls][eventId].push_back(callback);

        const ZFClass *cls = d->cls;
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
        d->cls->instanceObserverAdd(instanceOnCreate, zfnull, ZFLevelZFFrameworkNormal);
    }
    else {
        it->second[eventId].push_back(callback);
    }
    g->classOnChangeCheckAttach();
    return *this;
}

ZFDynamic &ZFDynamic::NSBegin(ZF_IN_OPT const zfchar *methodNamespace /* = ZF_NAMESPACE_GLOBAL_NAME */) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    if(zfstringIsEmpty(methodNamespace) || zfstringIsEqual(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME)) {
        d->methodNamespace = ZF_NAMESPACE_GLOBAL_NAME;
    }
    else {
        d->methodNamespace = methodNamespace;
    }
    return *this;
}
ZFDynamic &ZFDynamic::NSEnd(void) {
    if(d->errorOccurred) {return *this;}
    if(d->methodNamespace.isEmpty()) {
        d->error("no paired NSBegin");
    }
    d->methodNamespace.removeAll();
    return *this;
}

ZFDynamic &ZFDynamic::enumBegin(ZF_IN const zfchar *enumClassName) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->enumClassName = enumClassName;
    d->enumIsFlags = zffalse;
    return *this;
}
ZFDynamic &ZFDynamic::enumBeginFlags(ZF_IN const zfchar *enumClassName) {
    if(d->errorOccurred) {return *this;}
    if(!d->scopeBeginCheck()) {return *this;}
    d->enumClassName = enumClassName;
    d->enumIsFlags = zftrue;
    return *this;
}
ZFDynamic &ZFDynamic::enumValue(
        ZF_IN const zfchar *enumName
        , ZF_IN_OPT zfuint enumValue /* = ZFEnumInvalid() */
        ) {
    if(d->errorOccurred) {return *this;}
    if(d->enumClassName.isEmpty()) {
        d->error("have you forgot enumBegin?");
        return *this;
    }
    if(enumValue == ZFEnumInvalid()) {
        enumValue = d->enumValueNext;
        ++(d->enumValueNext);
    }
    else {
        if(d->enumValueNext <= enumValue) {
            d->enumValueNext = enumValue + 1;
        }
    }
    d->enumValues.add(enumValue);
    d->enumNames.add(enumName);
    return *this;
}
ZFDynamic &ZFDynamic::enumEnd(ZF_IN_OPT zfuint enumDefault /* = ZFEnumInvalid() */) {
    if(d->errorOccurred) {return *this;}
    if(d->enumClassName.isEmpty()) {
        d->error("have you forgot enumBegin?");
        return *this;
    }

    zfstring errorHint;
    const ZFClass *enumClass = ZFEnumDynamicRegister(
        d->enumClassName,
        d->enumValues,
        d->enumNames,
        d->enumDefault,
        d->enumIsFlags,
        &errorHint);
    if(enumClass == zfnull) {
        d->error(zfstr("unable to register enum, reason: %s", errorHint));
        return *this;
    }
    d->allEnum.add(enumClass);
    d->enumClassName.removeAll();
    d->enumDefault = ZFEnumInvalid();
    d->enumIsFlags = zffalse;
    d->enumValueNext = 0;
    d->enumValues.removeAll();
    d->enumNames.removeAll();
    return *this;
}

static zfbool _ZFP_ZFDynamicEventGI(ZFMETHOD_GENERIC_INVOKER_PARAMS) {
    ret = invokerMethod->methodDynamicRegisterUserData();
    return zftrue;
}
ZFDynamic &ZFDynamic::event(ZF_IN const zfchar *eventName) {
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty()) {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(zfstringIsEmpty(eventName)) {
        d->error("empty event name");
        return *this;
    }
    zfstring idName;
    if(d->cls != zfnull) {
        idName += d->cls->classNameFull();
    }
    else {
        if(d->methodNamespace.isEmpty()) {
            idName += ZF_NAMESPACE_GLOBAL_NAME;
        }
        else {
            idName += d->methodNamespace;
        }
    }
    idName += "::Event";
    idName += eventName;
    zfidentity idValue = ZFIdMapIdForName(idName);
    if(idValue != zfidentityInvalid()) {
        d->error(zfstr("%s already exists", idName));
        return *this;
    }
    idValue = ZFIdMapDynamicRegister(idName);
    d->allEvent.add(idValue);

    zfblockedAlloc(v_zfidentity, t);
    t->zfv = idValue;
    const ZFMethod *method = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFDynamicEventGI)
            .methodDynamicRegisterUserData(t)
            .methodOwnerClass(d->cls)
            .methodNamespace(d->methodNamespace)
            .methodName(zfstr("Event%s", eventName))
            .methodReturnTypeId(ZFTypeId_zfidentity())
        );
    zfCoreAssert(method != zfnull);
    d->allMethod.add(method);

    return *this;
}

ZFDynamic &ZFDynamic::method(
        ZF_IN const zfchar *methodReturnTypeId
        , ZF_IN const zfchar *methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        ) {
    ZFMethodDynamicRegisterParam p;
    p.methodReturnTypeId(methodReturnTypeId);
    p.methodName(methodName);
    p.methodImpl(methodImpl);
    for(zfindex i = 0; i < methodParam.methodParamCount(); ++i) {
        if(methodParam.methodParamDefaultValueAt(i) == ZFMethodGenericInvokerDefaultParam()) {
            p.methodParamAdd(
                methodParam.methodParamTypeIdAt(i),
                zfnull,
                methodParam.methodParamNameAt(i));
        }
        else {
            p.methodParamAddWithDefault(
                methodParam.methodParamTypeIdAt(i),
                zfnull,
                methodParam.methodParamNameAt(i),
                methodParam.methodParamDefaultValueAt(i));
        }
    }
    if(d->cls == zfnull) {
        p.methodNamespace(d->methodNamespace);
    }
    else {
        p.methodOwnerClass(d->cls);
    }
    return this->method(p);
}

ZFDynamic &ZFDynamic::method(ZF_IN const ZFMethodDynamicRegisterParam &param) {
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty()) {
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

static zfautoObject _ZFP_ZFDynamicPropertyInit(ZF_IN const ZFProperty *property) {
    ZFCopyable *copyable = property->propertyDynamicRegisterUserData()->toAny();
    if(copyable != zfnull) {
        return copyable->copy();
    }
    ZFStyleable *styleable = property->propertyDynamicRegisterUserData()->toAny();
    if(styleable != zfnull) {
        zfautoObject ret = styleable->classData()->newInstance();
        ret.to<ZFStyleable *>()->styleableCopyFrom(styleable);
        return ret;
    }
    return zfnull;
}
ZFDynamic &ZFDynamic::property(
        ZF_IN const zfchar *propertyTypeId
        , ZF_IN const zfchar *propertyName
        , ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    const ZFClass *cls = ZFClass::classForName(propertyTypeId);
    if(cls != zfnull) {
        return this->property(cls, propertyName, propertyInitValue, setterPrivilegeType, getterPrivilegeType);
    }
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClass(d->cls);
    param.propertyTypeId(propertyTypeId);
    param.propertyClassOfRetainProperty(ZFClass::classForName(propertyTypeId));
    param.propertyName(propertyName);
    param.propertySetterType(setterPrivilegeType);
    param.propertyGetterType(getterPrivilegeType);
    if(propertyInitValue != zfnull) {
        ZFTypeIdWrapper *propertyInitValueWrapper = ZFCastZFObject(ZFTypeIdWrapper *, propertyInitValue);
        if(propertyInitValueWrapper == zfnull) {
            d->error(zfstr("assign property's type must be %s: %s"
                        , ZFTypeIdWrapper::ClassData()->classNameFull()
                        , propertyInitValue
                        ));
            return *this;
        }
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
        param.propertyDynamicRegisterUserData(propertyInitValue);
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(
        ZF_IN const ZFClass *propertyClassOfRetainProperty
        , ZF_IN const zfchar *propertyName
        , ZF_IN_OPT ZFObject *propertyInitValue /* = zfnull */
        , ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        , ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType /* = ZFMethodPrivilegeTypePublic */
        ) {
    if(d->errorOccurred) {return *this;}
    if(propertyClassOfRetainProperty == zfnull) {
        d->error("propertyClassOfRetainProperty not set");
        return *this;
    }
    ZFPropertyDynamicRegisterParam param;
    param.propertyOwnerClass(d->cls);
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
        if(ZFCastZFObject(ZFCopyable *, propertyInitValue) == zfnull
                && ZFCastZFObject(ZFStyleable *, propertyInitValue) == zfnull
                ) {
            d->error(zfstr("init value %s is not type of %s or %s"
                        , propertyInitValue
                        , ZFCopyable::ClassData()->classNameFull()
                        , ZFStyleable::ClassData()->classNameFull()
                        ));
            return *this;
        }
        param.propertyInitValueCallback(_ZFP_ZFDynamicPropertyInit);
        param.propertyDynamicRegisterUserData(propertyInitValue);
    }
    return this->property(param);
}
ZFDynamic &ZFDynamic::property(ZF_IN const ZFPropertyDynamicRegisterParam &param) {
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty()) {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(d->cls == zfnull) {
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
        ZF_IN const zfchar *propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnInit, callback);
}
ZFDynamic &ZFDynamic::propertyOnVerify(
        ZF_IN const zfchar *propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnVerify, callback);
}
ZFDynamic &ZFDynamic::propertyOnAttach(
        ZF_IN const zfchar *propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnAttach, callback);
}
ZFDynamic &ZFDynamic::propertyOnDetach(
        ZF_IN const zfchar *propertyName
        , ZF_IN const ZFListener &callback
        ) {
    return this->propertyLifeCycle(propertyName, ZFPropertyLifeCycleOnDetach, callback);
}

ZFDynamic &ZFDynamic::propertyLifeCycle(
        ZF_IN const zfchar *propertyName
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        , ZF_IN const ZFListener &callback
        ) {
    if(d->errorOccurred) {return *this;}
    if(!d->enumClassName.isEmpty()) {
        d->error("can not be called within enumBegin");
        return *this;
    }
    if(d->cls == zfnull) {
        d->error("have you forgot classBegin?");
        return *this;
    }
    _ZFP_ZFDynamicPropLifeCycle item;
    item.property = d->cls->propertyForName(propertyName);
    item.ownerClassOrNull = d->cls;
    item.lifeCycle = lifeCycle;
    if(item.property == zfnull) {
        d->error(zfstr("no property %s for class %s", propertyName, d->cls->className()));
        return *this;
    }
    zfstring errorHint;
    if(!ZFPropertyDynamicRegisterLifeCycle(item.property, d->cls, lifeCycle, callback)) {
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
        invokerObject->to<v_ZFDynamic *>()->zfv.regTag(regTag);
    }, v_ZFDynamic
    , ZFMP_IN(const zfchar *, regTag)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFDynamic *>()->zfv.regTag(regTag);
    }, v_ZFDynamic
    , ZFMP_IN(ZFObject *, regTag)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, regTag
        , ZFMP_IN(const zfchar *, regTag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, regTag
        , ZFMP_IN(ZFObject *, regTag)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const zfchar *, regTag)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFClass *> &, allClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFClass *> &, allEnum)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFMethod *> &, allMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<const ZFProperty *> &, allProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, const ZFCoreArrayPOD<zfidentity> &, allEvent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin
        , ZFMP_IN(const zfchar *, classNameFull)
        , ZFMP_IN_OPT(const ZFClass *, classParent, ZFObject::ClassData())
        , ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, classBegin
        , ZFMP_IN(const zfchar *, classNameFull)
        , ZFMP_IN(const zfchar *, parentClassNameFull)
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFDynamic, ZFDynamic &, on
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
        , ZFMP_IN_OPT(const zfchar *, methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFDynamic &, NSEnd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumBegin
        , ZFMP_IN(const zfchar *, enumClassName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumBeginFlags
        , ZFMP_IN(const zfchar *, enumClassName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, enumValue
        , ZFMP_IN(const zfchar *, enumName)
        , ZFMP_IN_OPT(zfuint, enumValue, ZFEnumInvalid())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, enumEnd
        , ZFMP_IN_OPT(zfuint, enumDefault, ZFEnumInvalid())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, event
        , ZFMP_IN(const zfchar *, eventName)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFDynamic, ZFDynamic &, method
        , ZFMP_IN(const zfchar *, methodReturnTypeId)
        , ZFMP_IN(const zfchar *, methodName)
        , ZFMP_IN(const ZFMP &, methodParam)
        , ZFMP_IN(const ZFListener &, methodImpl)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, method
        , ZFMP_IN(const ZFMethodDynamicRegisterParam &, param)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const zfchar *, propertyTypeId)
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty)
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN_OPT(ZFObject *, propertyInitValue, zfnull)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, setterPrivilegeType, ZFMethodPrivilegeTypePublic)
        , ZFMP_IN_OPT(ZFMethodPrivilegeType, getterPrivilegeType, ZFMethodPrivilegeTypePublic)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFDynamic, ZFDynamic &, property
        , ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnInit
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnVerify
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnAttach
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFDynamic, ZFDynamic &, propertyOnDetach
        , ZFMP_IN(const zfchar *, propertyName)
        , ZFMP_IN(const ZFListener &, callback)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFDynamic, ZFCoreArray<ZFOutput> &, errorCallbacks)

// ============================================================
zfclassNotPOD _ZFP_ZFMPPrivate {
public:
    zfuint refCount;
    zfindex methodParamCount;
    zfstring methodParamTypeId[ZFMETHOD_MAX_PARAM];
    zfstring methodParamName[ZFMETHOD_MAX_PARAM];
    zfautoObject methodParamDefaultValue[ZFMETHOD_MAX_PARAM];
public:
    _ZFP_ZFMPPrivate(void)
    : refCount(1)
    , methodParamCount(0)
    , methodParamTypeId()
    , methodParamName()
    , methodParamDefaultValue()
    {
    }
};

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMP, ZFMP)

ZFMP &ZFMP::mp(
        ZF_IN const zfchar *methodParamTypeId
        , ZF_IN_OPT const zfchar *methodParamName /* = zfnull */
        , ZF_IN_OPT ZFObject *methodParamDefaultValue /* = ZFMethodGenericInvokerDefaultParam() */
        ) {
    zfCoreAssert(d->methodParamCount <= ZFMETHOD_MAX_PARAM);
    d->methodParamTypeId[d->methodParamCount] = methodParamTypeId;
    d->methodParamName[d->methodParamCount] = methodParamName;
    d->methodParamDefaultValue[d->methodParamCount] = methodParamDefaultValue;
    ++(d->methodParamCount);
    return *this;
}

zfindex ZFMP::methodParamCount(void) const {
    return d->methodParamCount;
}
const zfchar *ZFMP::methodParamTypeIdAt(ZF_IN zfindex index) const {
    return d->methodParamTypeId[index];
}
const zfchar *ZFMP::methodParamNameAt(ZF_IN zfindex index) const {
    return d->methodParamName[index];
}
ZFObject *ZFMP::methodParamDefaultValueAt(ZF_IN zfindex index) const {
    return d->methodParamDefaultValue[index];
}

ZFMP::ZFMP(void)
: d(zfnew(_ZFP_ZFMPPrivate))
{
}
ZFMP::ZFMP(ZF_IN const ZFMP &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFMP::~ZFMP(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFMP &ZFMP::operator = (ZF_IN const ZFMP &ref) {
    _ZFP_ZFMPPrivate *dTmp = d;
    d = ref.d;
    ++(d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0) {
        zfdelete(dTmp);
    }
    return *this;
}

void ZFMP::methodParamListInfoT(ZF_IN_OUT zfstring &ret) const {
    for(zfindex i = 0; i < d->methodParamCount; ++i) {
        if(i > 0) {
            ret += ", ";
        }
        ret += d->methodParamTypeId[i];
        ret += " ";
        if(d->methodParamName[i].isEmpty()) {
            zfstringAppend(ret, "p%s", i);
        }
        else {
            ret += d->methodParamName[i];
        }
        if(d->methodParamDefaultValue[i] != ZFMethodGenericInvokerDefaultParam()) {
            ret += " = ";
            ZFObjectInfoT(ret, d->methodParamDefaultValue[i]);
        }
    }
}

void ZFMP::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "ZFMP(";
    this->methodParamListInfoT(ret);
    ret += ")";
}

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMP, ZFMP &, mp
        , ZFMP_IN(const zfchar *, methodParamTypeId)
        , ZFMP_IN_OPT(const zfchar *, methodParamName, zfnull)
        , ZFMP_IN_OPT(ZFObject *, methodParamDefaultValue, ZFMethodGenericInvokerDefaultParam())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMP, zfindex, methodParamCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfchar *, methodParamTypeIdAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, const zfchar *, methodParamNameAt
        , ZFMP_IN(zfindex, index)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMP, ZFObject *, methodParamDefaultValueAt
        , ZFMP_IN(zfindex, index)
        )

ZF_NAMESPACE_GLOBAL_END

