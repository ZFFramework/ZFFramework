#include "ZFAni.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfautoT<ZFAniForTimer>, ZFAni
        , ZFMP_IN(const ZFListener &, aniImpl)
        ) {
    if(!aniImpl) {
        return zfnull;
    }
    zfobj<ZFAniForImpl> ani;
    ani->aniImpl(aniImpl);
    return ani;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFAniForTimer>, ZFAni
        , ZFMP_IN(const zfstring &, propertyName)
        , ZFMP_IN(ZFObject *, from)
        , ZFMP_IN(ZFObject *, to)
        ) {
    zfobj<ZFAniForGeneric> ani;
    ani->name(propertyName);
    ani->fromValue(from);
    ani->toValue(to);
    return ani;
}

// ============================================================
zfclassNotPOD _ZFP_ZFAniForGenericPrivate {
public:
    zfbool needUpdate;
private:
    zfauto _target;
    const ZFMethod *_setterMethod;
    const ZFMethod *_getterMethod;
    zfauto _valueSaved;
    zfautoT<ZFProgressable> _valueHolder;
    zfauto _from;
    zfauto _to;
    const ZFTypeInfo *_typeInfo; // null for retain property, non-null for assignable property
public:
    _ZFP_ZFAniForGenericPrivate(void)
    : needUpdate(zftrue)
    , _target(zfnull)
    , _setterMethod(zfnull)
    , _getterMethod(zfnull)
    , _valueSaved()
    , _valueHolder()
    , _from(zfnull)
    , _to(zfnull)
    , _typeInfo(zfnull)
    {
    }
public:
    zfbool checkSetup(
            ZF_IN ZFObject *target
            , ZF_IN const zfstring &name
            , ZF_IN ZFObject *from
            , ZF_IN ZFObject *to
            ) {
        if(!this->needUpdate) {
            return zftrue;
        }
        if(!this->_prepare(target, name)) {
            return zffalse;
        }
        zfstring typeId = _getterMethod->returnTypeId();
        const ZFClass *typeClass = ZFClass::classForName(typeId);
        if(typeClass == zfnull) {
            const ZFTypeInfo *typeInfo = (typeClass == zfnull ? ZFTypeInfoForName(typeId) : zfnull);
            if(typeInfo == zfnull || typeInfo->typeIdClass() == zfnull || typeInfo->typeIdClass()->classIsAbstract()) {
                return zffalse;
            }
            typeClass = typeInfo->typeIdClass();
        }
        return zftrue
            && this->_convert(_from, from, typeClass)
            && this->_convert(_to, to, typeClass)
            && this->_setup()
            ;
    }
    void update(ZF_IN zffloat progress) {
        if(_from == zfnull) {
            _from = _getterMethod->methodInvoke(_target);
            if(_from == zfnull) {
                return;
            }
            if(_from->classData()->classIsTypeOf(ZFCopyable::ClassData())) {
                _from = _from.to<ZFCopyable *>()->copy();
            }
        }
        if(_to == zfnull) {
            _to = _getterMethod->methodInvoke(_target);
            if(_to == zfnull) {
                return;
            }
            if(_to->classData()->classIsTypeOf(ZFCopyable::ClassData())) {
                _to = _to.to<ZFCopyable *>()->copy();
            }
        }

        if(_typeInfo != zfnull) {
            if(_valueHolder != zfnull && _valueHolder->progressUpdate(_from, _to, progress)) {
                _setterMethod->methodInvoke(_target, _valueHolder);
            }
        }
        else {
            zfauto valueHolder = _getterMethod->methodInvoke(_target);
            ZFProgressable *value = valueHolder;
            if(value != zfnull) {
                value->progressUpdate(_from, _to, progress);
            }
        }
    }
    void valueSavedUpdate(void) {
        if(_typeInfo == zfnull) {
            _valueSaved = zfnull;
            return;
        }
        _valueSaved = _getterMethod->methodInvoke(_target);
    }
    void valueSavedRestore(void) {
        if(_valueSaved == zfnull || _typeInfo == zfnull) {
            return;
        }
        _setterMethod->methodInvoke(_target, _valueSaved);
    }
private:
    zfbool _prepare(
            ZF_IN ZFObject *target
            , ZF_IN const zfstring &name
            ) {
        if(target == zfnull) {
            return zffalse;
        }
        const ZFProperty *prop = target->classData()->propertyForName(name);
        if(prop != zfnull) {
            _setterMethod = prop->setterMethod();
            _getterMethod = prop->getterMethod();
        }
        else {
            _setterMethod = target->classData()->propertySetterForName(name);
            _getterMethod = target->classData()->propertyGetterForName(name);
        }
        if(_setterMethod != zfnull && !_setterMethod->isPublic()) {
            _setterMethod = zfnull;
        }
        if(_getterMethod != zfnull && _getterMethod->isPrivate()) {
            _getterMethod = zfnull;
        }
        _target = target;
        return (_getterMethod != zfnull);
    }
    static zfbool _convert(
            ZF_OUT zfauto &dst
            , ZF_IN ZFObject *src
            , ZF_IN const ZFClass *typeClass
            ) {
        if(src == zfnull
                || src->classData() == ZFObject::ClassData()
                || src->classData()->classIsTypeOf(typeClass)
                ) {
            dst = src;
            return zftrue;
        }
        const zfchar *s = zfnull;
        v_zfstring *tmp = zfcast(v_zfstring *, src);
        if(tmp != zfnull) {
            s = tmp->zfv;
        }
        else {
            ZFDI_Wrapper *tmp2 = zfcast(ZFDI_Wrapper *, src);
            if(tmp2 != zfnull) {
                s = tmp2->zfv();
            }
        }
        if(s == zfnull) {
            return zffalse;
        }
        dst = typeClass->newInstance();
        if(!zfstringIsEmpty(s)) {
            if(typeClass->classIsTypeOf(ZFTypeIdWrapper::ClassData())) {
                if(!dst.to<ZFTypeIdWrapper *>()->zfvFromString(s)) {
                    return zffalse;
                }
            }
            else {
                if(!typeClass->classIsTypeOf(ZFSerializable::ClassData())
                        || !dst.to<ZFSerializable *>()->serializeFromString(s)
                        ) {
                    return zffalse;
                }
            }
        }
        return zftrue;
    }
    zfbool _setup(void) {
        if(_from == zfnull && _to == zfnull) {
            return zffalse;
        }
        if(_getterMethod->methodInvoke(_target) == zfnull) {
            return zffalse;
        }

        if(_setterMethod == zfnull) { // ani by getter only
            if(ZFClass::classForName(_getterMethod->returnTypeId()) == zfnull
                    || zfcast(ZFProgressable *, _getterMethod->methodInvoke(_target)) == zfnull
                    ) { // ani by getter only valid for retain property
                return zffalse;
            }
        }
        else { // ani by getter>change>setter
            _typeInfo = ZFTypeInfoForName(_getterMethod->returnTypeId());
            if(_typeInfo == zfnull
                || _typeInfo->typeIdClass() == zfnull
                || _typeInfo->typeIdClass()->classIsAbstract()
                || !_typeInfo->typeIdClass()->classIsTypeOf(ZFProgressable::ClassData())
            ) {
                return zffalse;
            }
            _valueHolder = _typeInfo->typeIdClass()->newInstance();
        }
        return zftrue;
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForImpl)

ZFOBJECT_REGISTER(ZFAniForGeneric)
ZFPROPERTY_ON_ATTACH_DEFINE(ZFAniForGeneric, zfstring, name) {d->needUpdate = true;}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFAniForGeneric, zfany, fromValue) {d->needUpdate = true;}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFAniForGeneric, zfany, toValue) {d->needUpdate = true;}

void ZFAniForGeneric::aniImplTargetOnUpdate(ZF_IN ZFObject *targetOld) {
    zfsuper::aniImplTargetOnUpdate(targetOld);
    d->needUpdate = zftrue;
}
zfbool ZFAniForGeneric::aniImplCheckValid(void) {
    if(!zfsuper::aniImplCheckValid()) {return zffalse;}
    return d->checkSetup(this->target(), this->name(), this->fromValue(), this->toValue());
}
void ZFAniForGeneric::aniImplStart(void) {
    d->valueSavedUpdate();
    zfsuper::aniImplStart();
}
void ZFAniForGeneric::aniImplStop(void) {
    zfsuper::aniImplStop();
    d->valueSavedRestore();
}
void ZFAniForGeneric::aniTimerOnUpdate(ZF_IN zffloat progress) {
    d->update(progress);
}
void ZFAniForGeneric::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForGenericPrivate);
}
void ZFAniForGeneric::objectOnDealloc(void) {
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

