#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfanyT<ZFStyleable> ZFStyleable::defaultStyle(void) {
    const ZFMethod *method = this->classData()->methodForName("DefaultStyle");
    if(method != zfnull) {
        return method->methodInvoke(zfnull);
    }
    else {
        return zfnull;
    }
}

// ============================================================
zfclass _ZFP_I_ZFStyleable_PropertyTypeHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFStyleable_PropertyTypeHolder, ZFObject)

public:
    ZFCoreArray<const ZFProperty *> normalProperty;
    ZFCoreArray<const ZFProperty *> styleableProperty;
    ZFCoreArray<const ZFProperty *> copyableProperty;
};

// ============================================================
void ZFStyleable::styleableCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    if(anotherStyleable != zfnull && anotherStyleable != this->toObject()) {
        this->styleableOnCopyFrom(anotherStyleable);
    }
}
void ZFStyleable::styleablePropertyTypeGetAll(
        ZF_OUT ZFCoreArray<const ZFProperty *> &normalProperty
        , ZF_OUT ZFCoreArray<const ZFProperty *> &styleableProperty
        , ZF_OUT ZFCoreArray<const ZFProperty *> &copyableProperty
        ) {
    _ZFP_I_ZFStyleable_PropertyTypeHolder *holder = this->_ZFP_ZFStyleable_getPropertyTypeHolder();
    normalProperty.addFrom(holder->normalProperty);
    styleableProperty.addFrom(holder->styleableProperty);
    copyableProperty.addFrom(holder->copyableProperty);
}
zfstring ZFStyleable::styleablePropertyTypeInfo(void) {
    _ZFP_I_ZFStyleable_PropertyTypeHolder *holder = this->_ZFP_ZFStyleable_getPropertyTypeHolder();

    zfstring ret;
    ret += "normal:\n";
    for(zfindex i = 0; i < holder->normalProperty.count(); ++i) {
        const ZFProperty *p = holder->normalProperty[i];
        zfstringAppend(ret, "    (%s) %s::%s", p->propertyTypeName(), p->ownerClass()->className(), p->propertyName());
    }
    ret += "styleable:\n";
    for(zfindex i = 0; i < holder->styleableProperty.count(); ++i) {
        const ZFProperty *p = holder->styleableProperty[i];
        zfstringAppend(ret, "    (%s) %s::%s", p->propertyTypeName(), p->ownerClass()->className(), p->propertyName());
    }
    ret += "copyable:\n";
    for(zfindex i = 0; i < holder->copyableProperty.count(); ++i) {
        const ZFProperty *p = holder->copyableProperty[i];
        zfstringAppend(ret, "    (%s) %s::%s", p->propertyTypeName(), p->ownerClass()->className(), p->propertyName());
    }
    return ret;
}
ZFStyleable::PropertyType ZFStyleable::styleableOnCheckPropertyType(ZF_IN const ZFProperty *property) {
    if(property->isRetainProperty()
            && property->setterMethod()->isPrivate()
            && !property->getterMethod()->isPrivate()
            ) {
        if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData())) {
            return ZFStyleable::PropertyTypeStyleable;
        }
        else if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData())) {
            return ZFStyleable::PropertyTypeCopyable;
        }
    }
    if(property->setterMethod()->isPrivate()
            || property->getterMethod()->isPrivate()
            ) {
        return ZFStyleable::PropertyTypeNotStyleable;
    }
    return ZFStyleable::PropertyTypeNormal;
}

void ZFStyleable::styleableOnCopyPropertyFrom(
        ZF_IN ZFObject *anotherStyleable
        , ZF_IN const ZFProperty *property
        , ZF_IN ZFStyleable::PropertyType propertyType
        ) {
    if(property && this->propStyle(property->propertyName()) == zfnull) {
        this->propStyle(property->propertyName(), zfcast(ZFStyleable *, anotherStyleable)->propStyle(property->propertyName()));
    }
    switch(propertyType) {
        case ZFStyleable::PropertyTypeNormal: {
            ZFPropertyCopy(property, this->toObject(), anotherStyleable->toObject());
        }
            break;
        case ZFStyleable::PropertyTypeStyleable: {
            ZFStyleable *selfPropertyValue = property->getterMethod()->methodInvoke(this->toObject());
            ZFObject *anotherPropertyValue = property->getterMethod()->methodInvoke(anotherStyleable->toObject());
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull) {
                selfPropertyValue->styleableCopyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeCopyable: {
            ZFCopyable *selfPropertyValue = property->getterMethod()->methodInvoke(this->toObject());
            ZFObject *anotherPropertyValue = property->getterMethod()->methodInvoke(anotherStyleable->toObject());
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull) {
                selfPropertyValue->copyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeNotStyleable:
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}

_ZFP_I_ZFStyleable_PropertyTypeHolder *ZFStyleable::_ZFP_ZFStyleable_getPropertyTypeHolder(void) {
    ZFCoreMutexLocker();
    _ZFP_I_ZFStyleable_PropertyTypeHolder *holder = this->classData()->classTag(_ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull());
    if(holder == zfnull) {
        zfobj<_ZFP_I_ZFStyleable_PropertyTypeHolder> holderTmp;

        const ZFCoreArray<const ZFProperty *> allProperty = ZFClassUtil::allProperty(this->classData());
        const ZFProperty *propertyTmp = zfnull;
        for(zfindex i = 0; i < allProperty.count(); ++i) {
            propertyTmp = allProperty[i];
            switch(this->styleableOnCheckPropertyType(propertyTmp)) {
                case ZFStyleable::PropertyTypeNotStyleable:
                    break;
                case ZFStyleable::PropertyTypeNormal:
                    holderTmp->normalProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeStyleable:
                    holderTmp->styleableProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeCopyable:
                    holderTmp->copyableProperty.add(propertyTmp);
                    break;
            }
        }

        holder = holderTmp;
        this->classData()->classTag(
            _ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull(),
            holderTmp);
    }
    return holder;
}

void ZFStyleable::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    if(this->styleKey() == zfnull) {
        this->styleKey(zfcast(ZFStyleable *, anotherStyleable)->styleKey());
    }

    _ZFP_I_ZFStyleable_PropertyTypeHolder *holderTmp = this->_ZFP_ZFStyleable_getPropertyTypeHolder();
    const ZFClass *thisCls = this->classData();
    const ZFClass *anotherCls = anotherStyleable->classData();
    ZFObject *anotherStyleableObject = anotherStyleable->toObject();;
    const ZFProperty *property = zfnull;

    for(zfindex i = holderTmp->normalProperty.count() - 1; i != zfindexMax(); --i) {
        property = holderTmp->normalProperty[i];
        if(!anotherCls->classIsTypeOf(property->ownerClass())) {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsEqual(property, anotherCls)
                || property->callbackIsValueAccessed(property, anotherStyleableObject)
                ) {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeNormal);
        }
    }
    for(zfindex i = holderTmp->styleableProperty.count() - 1; i != zfindexMax(); --i) {
        property = holderTmp->styleableProperty[i];
        if(!anotherCls->classIsTypeOf(property->ownerClass())) {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsEqual(property, anotherCls)
                || property->callbackIsValueAccessed(property, anotherStyleableObject)
                ) {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeStyleable);
        }
    }
    for(zfindex i = holderTmp->copyableProperty.count() - 1; i != zfindexMax(); --i) {
        property = holderTmp->copyableProperty[i];
        if(!anotherCls->classIsTypeOf(property->ownerClass())) {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsEqual(property, anotherCls)
                || property->callbackIsValueAccessed(property, anotherStyleableObject)
                ) {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeCopyable);
        }
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFStyle)

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFStyleableDefaultStyleDataHolder) {
}
ZFCoreMap instanceDataMap; // _ZFP_ZFStyleableDefaultPointerHolder *
ZF_STATIC_INITIALIZER_END(ZFStyleableDefaultStyleDataHolder)

_ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfstring &name) {
    ZFCoreMutexLocker();
    ZFCoreMap &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFStyleableDefaultStyleDataHolder)->instanceDataMap;
    const ZFCorePointerBase *v = m.get(name);
    if(v == zfnull) {
        m.set(name, ZFCorePointerForPoolObject<_ZFP_ZFStyleableDefaultPointerHolder *>(zfpoolNew(_ZFP_ZFStyleableDefaultPointerHolder)));
        v = m.get(name);
    }
    return v->pointerValueT<_ZFP_ZFStyleableDefaultPointerHolder *>();
}

// ============================================================
zfclass _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData, ZFObject)

public:
    ZFCoreArray<ZFObject *> styles;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleDefaultApplyAutoCopyDataHolder, ZFLevelZFFrameworkEssential) {
    this->styleOnDeallocListener = ZFCallbackForFunc(zfself::styleOnDealloc);
    this->defaultStyleOnUpdateListener = ZFCallbackForFunc(zfself::defaultStyleOnUpdate);
}
public:
    ZFListener styleOnDeallocListener;
    ZFListener defaultStyleOnUpdateListener;
public:
    static void styleOnDealloc(ZF_IN const ZFArgs &zfargs) {
        ZFCoreMutexLocker();

        ZFObject *defaultStyle = zfargs.sender()->to<ZFStyleable *>()->defaultStyle();
        ZFCoreAssert(defaultStyle != zfnull);
        _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle
            ->objectTag(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
        ZFCoreAssert(taskData != zfnull);

        taskData->styles.removeElement(zfargs.sender());

        if(taskData->styles.isEmpty()) {
            defaultStyle->objectTagRemove(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            defaultStyle->observerRemove(ZFObject::E_ObjectPropertyValueOnUpdate(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnUpdateListener);
        }
    }
    static void defaultStyleOnUpdate(ZF_IN const ZFArgs &zfargs) {
        ZFCoreMutexLocker();

        const ZFProperty *property = zfargs.param0().to<v_ZFProperty *>()->zfv;
        ZFObject *defaultStyle = zfargs.sender();
        _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle
            ->objectTag(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());

        ZFCoreArray<ZFObject *> styles;
        styles.copyFrom(taskData->styles);
        for(zfindex i = 0; i < styles.count(); ++i) {
            ZFPropertyCopy(property, styles[i], zfargs.sender());
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFStyleDefaultApplyAutoCopyDataHolder)

void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style) {
    ZFCoreMutexLocker();
    if(style != zfnull && !style->styleableIsDefaultStyle()) {
        ZFObject *defaultStyle = style->defaultStyle();
        if(defaultStyle != zfnull) {
            _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle
                ->objectTag(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            if(taskData == zfnull) {
                taskData = zfAlloc(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData);
                defaultStyle->objectTag(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull(), taskData);
                zfRelease(taskData);

                defaultStyle->observerAdd(ZFObject::E_ObjectPropertyValueOnUpdate(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnUpdateListener);
            }
            taskData->styles.add(style->toObject());
            style->toObject()->observerAdd(ZFObject::E_ObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->styleOnDeallocListener);
        }
    }
}

// ============================================================
// style holder
static zfbool _ZFP_ZFStyleUpdateFlag = zffalse;
static zfstlmap<zfstring, zfauto> &_ZFP_ZFStyleHolder(void) {
    static zfstlmap<zfstring, zfauto> d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleCleanup, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleCleanup) {
    _ZFP_ZFStyleHolder().clear();
}
ZF_GLOBAL_INITIALIZER_END(ZFStyleCleanup)

static zfstlmap<zfstring, _ZFP_ZFStyleDecoder> &_ZFP_ZFStyleDecoderMap(void) {
    static zfstlmap<zfstring, _ZFP_ZFStyleDecoder> d;
    return d;
}
void _ZFP_ZFStyleDecoderRegister(
        ZF_IN const zfstring &registerSig
        , ZF_IN _ZFP_ZFStyleDecoder decoder
        ) {
    ZFCoreMutexLocker();
    zfstlmap<zfstring, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    ZFCoreAssert(registerSig != zfnull && decoder != zfnull);
    ZFCoreAssertWithMessageTrim(m.find(registerSig) == m.end(),
        "[ZFSTYLE_DECODER_DEFINE] %s already registered",
        registerSig);
    m[registerSig] = decoder;
}
void _ZFP_ZFStyleDecoderUnregister(ZF_IN const zfstring &registerSig) {
    ZFCoreMutexLocker();
    zfstlmap<zfstring, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    m.erase(registerSig);
}

void ZFStyleSet(
        ZF_IN const zfstring &styleKey
        , ZF_IN ZFStyleable *styleValue
        ) {
    if(styleKey && styleValue) {
        ZFCoreMutexLock();
        _ZFP_ZFStyleUpdateFlag = zftrue;
        _ZFP_ZFStyleHolder()[styleKey] = styleValue;
        ZFCoreMutexUnlock();
    }
}
zfauto ZFStyleGet(ZF_IN const zfstring &styleKey) {
    if(styleKey == zfnull) {
        return zfnull;
    }

    ZFCoreMutexLocker();
    zfauto ret;
    zfstlmap<zfstring, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    for(zfstlmap<zfstring, _ZFP_ZFStyleDecoder>::iterator it = m.begin(); it != m.end(); ++it) {
        if(it->second(ret, styleKey)) {
            return ret;
        }
    }

    zfstlmap<zfstring, zfauto> &d = _ZFP_ZFStyleHolder();
    zfstlmap<zfstring, zfauto>::iterator it = d.find(styleKey);
    if(it != d.end()) {
        return it->second;
    }
    else {
        return zfnull;
    }
}
void ZFStyleGetAll(
        ZF_IN_OUT ZFCoreArray<zfstring> &styleKey
        , ZF_IN_OUT ZFCoreArray<ZFStyleable *> &styleValue
        ) {
    ZFCoreMutexLocker();
    zfstlmap<zfstring, zfauto> &d = _ZFP_ZFStyleHolder();
    for(zfstlmap<zfstring, zfauto>::iterator it = d.begin(); it != d.end(); ++it) {
        styleKey.add(it->first);
        styleValue.add(it->second);
    }
}
void ZFStyleRemoveAll(void) {
    ZFCoreMutexLock();
    zfstlmap<zfstring, zfauto> d;
    _ZFP_ZFStyleUpdateFlag = zftrue;
    d.swap(_ZFP_ZFStyleHolder());
    ZFCoreMutexUnlock();
}

static zfint _ZFP_ZFStyleUpdateBeginFlag = 0;
void ZFStyleUpdateBegin() {
    ZFCoreMutexLock();
    ++_ZFP_ZFStyleUpdateBeginFlag;
    if(_ZFP_ZFStyleUpdateBeginFlag == 1) {
        _ZFP_ZFStyleUpdateFlag = zffalse;
    }
    ZFCoreMutexUnlock();
}
void ZFStyleUpdateEnd() {
    ZFCoreAssertWithMessageTrim(_ZFP_ZFStyleUpdateBeginFlag != 0,
        "ZFStyleUpdateBegin/ZFStyleUpdateEnd not paired");

    ZFCoreMutexLock();
    --_ZFP_ZFStyleUpdateBeginFlag;
    zfbool needNotify = (_ZFP_ZFStyleUpdateBeginFlag == 0 && _ZFP_ZFStyleUpdateFlag);
    ZFCoreMutexUnlock();

    if(needNotify) {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::E_ZFStyleOnUpdate());
    }
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFEVENT_GLOBAL_REGISTER(ZFStyleOnUpdate)
ZFEVENT_GLOBAL_REGISTER(ZFStyleOnInvalid)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, zfanyT<ZFStyleable>, defaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleableCopyFrom
        , ZFMP_IN(ZFObject *, anotherStyleable)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, zfbool, styleableIsDefaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFStyleable, void, styleablePropertyTypeGetAll
        , ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, normalProperty)
        , ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, styleableProperty)
        , ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, copyableProperty)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, zfstring, styleablePropertyTypeInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleKey
        , ZFMP_IN(const zfstring &, styleKey)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, const zfstring &, styleKey)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFStyleable, void, propStyle
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(const zfstring &, styleKey)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, const zfstring &, propStyle
        , ZFMP_IN(const zfstring &, propertyName)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFStyleDefaultApplyAutoCopy
        , ZFMP_IN(ZFStyleable *, styleValue)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleSet
        , ZFMP_IN(const zfstring &, styleKey)
        , ZFMP_IN(ZFStyleable *, styleValue)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfauto, ZFStyleGet
        , ZFMP_IN(const zfstring &, styleKey)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleGetAll
        , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, styleKey)
        , ZFMP_IN_OUT(ZFCoreArray<ZFStyleable *> &, styleValue)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleRemoveAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleUpdateBegin)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleUpdateEnd)

ZF_NAMESPACE_GLOBAL_END
#endif

