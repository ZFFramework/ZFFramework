#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

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

static ZFCoreArray<const ZFProperty *> _ZFP_ZFStyleable_propList(ZF_IN const ZFClass *cls) {
    v_ZFCoreArray *holder = cls->dataCache("_ZFP_ZFStyleable_propList");
    if(holder) {
        return *(const ZFCoreArray<const ZFProperty *> *)holder->zfv;
    }
    ZFCoreArray<const ZFProperty *> ret = cls->propertyGetAll();
    for(zfindex i = ret.count() - 1; i != zfindexMax(); --i) {
        const ZFProperty *prop = ret[i];
        if(zffalse
                || !prop->getterMethod()->isPublic()
                || prop->propertyName()[0] == '_'
                ) {
            ret.remove(i);
        }
        else if(!prop->setterMethod()->isPublic() && !(
                    prop->isRetainProperty()
                    && (zffalse
                        || prop->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData())
                        || prop->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData())
                        )
                        )) {
            ret.remove(i);
        }
    }
    zfobj<v_ZFCoreArray> holderTmp(ret);
    cls->dataCache("_ZFP_ZFStyleable_propList", holderTmp);
    return ret;
}

// ============================================================
void ZFStyleable::styleableCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    if(anotherStyleable != zfnull && anotherStyleable != this->toObject()) {
        this->styleableOnCopyFrom(anotherStyleable);
    }
}
void ZFStyleable::styleablePropertyGetAllT(
        ZF_OUT ZFCoreArray<const ZFProperty *> &ret
        ) {
    ret.addFrom(_ZFP_ZFStyleable_propList(this->classData()));
}
ZFCoreArray<const ZFProperty *> ZFStyleable::styleablePropertyGetAll(void) {
    ZFCoreArray<const ZFProperty *> ret;
    this->styleablePropertyGetAllT(ret);
    return ret;
}

void ZFStyleable::styleableOnCopyPropertyFrom(
        ZF_IN ZFObject *anotherStyleable
        , ZF_IN const ZFProperty *property
        ) {
    ZFStyleable *ref = zfcast(ZFStyleable *, anotherStyleable);
    this->propStyle(property->propertyName(), ref->propStyle(property->propertyName()));

    if(property->setterMethod()->isPublic()) {
        zfbool copied = zffalse;
        if(property->isRetainProperty()) {
            if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData())) {
                copied = zftrue;
                ZFStyleable *v0 = property->getterMethod()->methodInvoke(this->toObject());
                ZFObject *v1 = property->getterMethod()->methodInvoke(anotherStyleable);
                if(v0) {
                    if(v1) {
                        v0->styleableCopyFrom(v1);
                    }
                    else {
                        property->setterMethod()->methodInvoke(this->toObject(), zfnull);
                    }
                }
                else {
                    if(v1) {
                        zfautoT<ZFStyleable> v0Tmp = v1->classData()->newInstance();
                        v0Tmp->styleableCopyFrom(v1);
                    }
                }
            }
            else if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData())) {
                copied = zftrue;
                ZFCopyable *v0 = property->getterMethod()->methodInvoke(this->toObject());
                ZFObject *v1 = property->getterMethod()->methodInvoke(anotherStyleable);
                if(v0) {
                    if(v1) {
                        v0->copyFrom(v1);
                    }
                    else {
                        property->setterMethod()->methodInvoke(this->toObject(), zfnull);
                    }
                }
                else {
                    if(v1) {
                        property->setterMethod()->methodInvoke(this->toObject(), zfcast(ZFCopyable *, v1)->copy());
                    }
                }
            }
        }
        if(!copied) {
            ZFPropertyCopy(property, this->toObject(), anotherStyleable);
        }
    }
    else if(property->isRetainProperty()) {
        if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData())) {
            ZFStyleable *v0 = property->getterMethod()->methodInvoke(this->toObject());
            ZFObject *v1 = property->getterMethod()->methodInvoke(anotherStyleable);
            if(v0 && v1) {
                v0->styleableCopyFrom(v1);
            }
        }
        else if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData())) {
            ZFCopyable *v0 = property->getterMethod()->methodInvoke(this->toObject());
            ZFObject *v1 = property->getterMethod()->methodInvoke(anotherStyleable);
            if(v0 && v1) {
                v0->copyFrom(v1);
            }
        }
    }
}

void ZFStyleable::styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
    ZFStyleable *ref = zfcast(ZFStyleable *, anotherStyleable);
    if(ref->styleKey()) {
        this->styleKey(ref->styleKey());
    }

    ZFCoreArray<const ZFProperty *> propList = _ZFP_ZFStyleable_propList(this->classData());
    const ZFClass *thisCls = this->classData();
    const ZFClass *anotherCls = anotherStyleable->classData();
    ZFObject *anotherStyleableObject = anotherStyleable->toObject();;
    const ZFProperty *property = zfnull;

    for(zfindex i = propList.count() - 1; i != zfindexMax(); --i) {
        property = propList[i];
        if(!anotherCls->classIsTypeOf(property->ownerClass())) {
            continue;
        }
        if(!thisCls->propertyInitStepIsEqual(property, anotherCls)
                || property->callbackIsValueAccessed(property, anotherStyleableObject)
                ) {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property);
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
typedef zfstlhashmap<zfstring, zfauto> _ZFP_ZFStyleHolderMapType;
static _ZFP_ZFStyleHolderMapType &_ZFP_ZFStyleHolder(void) {
    static _ZFP_ZFStyleHolderMapType d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleCleanup, ZFLevelZFFrameworkNormal) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleCleanup) {
    _ZFP_ZFStyleHolder().clear();
}
ZF_GLOBAL_INITIALIZER_END(ZFStyleCleanup)

typedef zfstlhashmap<zfstring, _ZFP_ZFStyleDecoder> _ZFP_ZFStyleDecoderMapType;
static _ZFP_ZFStyleDecoderMapType &_ZFP_ZFStyleDecoderMap(void) {
    static _ZFP_ZFStyleDecoderMapType d;
    return d;
}
void _ZFP_ZFStyleDecoderRegister(
        ZF_IN const zfstring &registerSig
        , ZF_IN _ZFP_ZFStyleDecoder decoder
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFStyleDecoderMapType &m = _ZFP_ZFStyleDecoderMap();
    ZFCoreAssert(registerSig != zfnull && decoder != zfnull);
    ZFCoreAssertWithMessageTrim(m.find(registerSig) == m.end(),
        "[ZFSTYLE_DECODER_DEFINE] %s already registered",
        registerSig);
    m[registerSig] = decoder;
}
void _ZFP_ZFStyleDecoderUnregister(ZF_IN const zfstring &registerSig) {
    ZFCoreMutexLocker();
    _ZFP_ZFStyleDecoderMapType &m = _ZFP_ZFStyleDecoderMap();
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
    _ZFP_ZFStyleDecoderMapType &m = _ZFP_ZFStyleDecoderMap();
    for(_ZFP_ZFStyleDecoderMapType::iterator it = m.begin(); it != m.end(); ++it) {
        if(it->second(ret, styleKey)) {
            return ret;
        }
    }

    _ZFP_ZFStyleHolderMapType &d = _ZFP_ZFStyleHolder();
    _ZFP_ZFStyleHolderMapType::iterator it = d.find(styleKey);
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
    _ZFP_ZFStyleHolderMapType &d = _ZFP_ZFStyleHolder();
    for(_ZFP_ZFStyleHolderMapType::iterator it = d.begin(); it != d.end(); ++it) {
        styleKey.add(it->first);
        styleValue.add(it->second);
    }
}
void ZFStyleRemoveAll(void) {
    ZFCoreMutexLock();
    _ZFP_ZFStyleHolderMapType d;
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
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleablePropertyGetAllT
        , ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, ZFCoreArray<const ZFProperty *>, styleablePropertyGetAll)
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

