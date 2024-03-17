#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFStyleKeyHolder {
public:
    zfchar *styleKey;
    zfstlmap<zfstring, zfstring> stylePropertyKeyMap;
    ZFListener styleOnChangeListener;
    ZFListener stylePropertyOnChangeListener;
public:
    static void styleOnChange(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFStyleable *owner
            );
    static void stylePropertyOnChange(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFStyleable *owner
            );
};

// ============================================================
void _ZFP_ZFStyleKeyHolder::styleOnChange(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN ZFStyleable *owner
        ) {
    zfCoreMutexLocker();
    zfauto style = ZFStyleGet(owner->styleKey());
    if(style != zfnull) {
        zfCoreAssertWithMessageTrim(style->classData()->classIsTypeOf(owner->classData())
            || owner->classData()->classIsTypeOf(style->classData()),
            "[ZFStyle] applying style from incorrect type, to object: %s, from style: %s",
            owner->toObject()->objectInfoOfInstance(),
            style->objectInfoOfInstance());

        owner->styleableCopyFrom(style);
    }
    else {
        owner->styleKey(zfnull);
    }
}
void ZFStyleable::styleKey(ZF_IN const zfchar *styleKey) {
    zfCoreMutexLocker();
    if(styleKey == zfnull) {
        if(_ZFP_styleKey != zfnull) {
            zfsChange(_ZFP_styleKey->styleKey, styleKey);
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                _ZFP_styleKey->styleOnChangeListener);
        }
    }
    else {
        if(_ZFP_styleKey == zfnull) {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        if(_ZFP_styleKey->styleKey == zfnull) {
            if(!_ZFP_styleKey->styleOnChangeListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(styleOnChange
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::styleOnChange(zfargs, owner);
                } ZFLISTENER_END()
                _ZFP_styleKey->styleOnChangeListener = styleOnChange;
            }
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                _ZFP_styleKey->styleOnChangeListener);
        }
        zfsChange(_ZFP_styleKey->styleKey, styleKey);
        ZFArgs zfargs;
        _ZFP_ZFStyleKeyHolder::styleOnChange(zfargs, this);

        if(_ZFP_styleKey->styleKey == zfnull) {
            v_zfstring *param0 = zfunsafe_zfAlloc(v_zfstring);
            v_zfstring *param1 = zfunsafe_zfAlloc(v_zfstring);
            param1->zfv = styleKey;
            ZFGlobalObserver().observerNotifyWithSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zfunsafe_zfRelease(param0);
            zfunsafe_zfRelease(param1);
        }
    }
}
const zfchar *ZFStyleable::styleKey(void) {
    if(_ZFP_styleKey) {
        return _ZFP_styleKey->styleKey;
    }
    else {
        return zfnull;
    }
}

// ============================================================
static zfbool _ZFP_ZFStylePropertyCopy(
        ZF_IN ZFObject *propertyOwner
        , ZF_IN const zfchar *propertyName
        , ZF_IN const zfchar *styleKey
        ) {
    zfauto styleValue = ZFStyleGet(styleKey);
    if(styleValue == zfnull) {
        return zffalse;
    }
    const ZFMethod *getterMethod = propertyOwner->classData()->propertyGetterForName(propertyName);
    if(getterMethod == zfnull || !getterMethod->methodIsPublic()) {
        return zffalse;
    }
    const ZFMethod *setterMethod = propertyOwner->classData()->propertySetterForName(propertyName);
    if(setterMethod == zfnull || !setterMethod->methodIsPublic()) {
        ZFStyleable *styleable = getterMethod->methodGenericInvoke(propertyOwner);
        if(styleable == zfnull) {
            return zffalse;
        }
        styleable->styleableCopyFrom(styleValue);
        return zftrue;
    }
    else {
        zfauto retDummy;
        zfauto paramDummy[ZFMETHOD_MAX_PARAM];
        paramDummy[0] = styleValue;
        return setterMethod->methodGenericInvoker()(setterMethod, propertyOwner, zfnull, retDummy, 1, paramDummy);
    }
}
void _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN ZFStyleable *owner
        ) {
    zfCoreMutexLocker();
    zfstlmap<zfstring, zfstring> &m = owner->_ZFP_styleKey->stylePropertyKeyMap;
    for(zfstlmap<zfstring, zfstring>::iterator it = m.begin(); it != m.end(); ++it) {
        _ZFP_ZFStylePropertyCopy(owner->toObject(), it->first, it->second);
    }
}
void ZFStyleable::styleKeyForProperty(
        ZF_IN const zfchar *propertyName
        , ZF_IN const zfchar *styleKey
        ) {
    if(propertyName == zfnull) {
        return;
    }
    zfCoreMutexLocker();
    if(styleKey == zfnull) {
        if(_ZFP_styleKey != zfnull) {
            _ZFP_styleKey->stylePropertyKeyMap.erase(propertyName);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    _ZFP_styleKey->stylePropertyOnChangeListener);
            }
        }
    }
    else {
        if(_ZFP_styleKey == zfnull) {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        zfbool oldEmpty = _ZFP_styleKey->stylePropertyKeyMap.empty();
        _ZFP_styleKey->stylePropertyKeyMap[propertyName] = styleKey;
        if(oldEmpty) {
            if(!_ZFP_styleKey->stylePropertyOnChangeListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(stylePropertyOnChange
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(zfargs, owner);
                } ZFLISTENER_END()
                _ZFP_styleKey->stylePropertyOnChangeListener = stylePropertyOnChange;
            }
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                _ZFP_styleKey->stylePropertyOnChangeListener);
        }
        if(!_ZFP_ZFStylePropertyCopy(this->toObject(), propertyName, styleKey)) {
            _ZFP_styleKey->stylePropertyKeyMap.erase(propertyName);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    _ZFP_styleKey->stylePropertyOnChangeListener);
            }
            v_zfstring *param0 = zfunsafe_zfAlloc(v_zfstring);
            param0->zfv = propertyName;
            v_zfstring *param1 = zfunsafe_zfAlloc(v_zfstring);
            param1->zfv = styleKey;
            ZFGlobalObserver().observerNotifyWithSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zfunsafe_zfRelease(param0);
            zfunsafe_zfRelease(param1);
        }
    }
}
const zfchar *ZFStyleable::styleKeyForProperty(ZF_IN const zfchar *propertyName) {
    if(propertyName == zfnull) {
        return zfnull;
    }
    zfCoreMutexLocker();
    if(_ZFP_styleKey != zfnull) {
        zfstlmap<zfstring, zfstring>::iterator it = _ZFP_styleKey->stylePropertyKeyMap.find(propertyName);
        if(it != _ZFP_styleKey->stylePropertyKeyMap.end()) {
            return it->second;
        }
        else {
            return zfnull;
        }
    }
    else {
        return zfnull;
    }
}

ZFINTERFACE_ON_DEALLOC_DEFINE(ZFStyleable) {
    if(_ZFP_styleKey) {
        if(_ZFP_styleKey->styleKey != zfnull) {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                _ZFP_styleKey->styleOnChangeListener);
            zffree(_ZFP_styleKey->styleKey);
            _ZFP_styleKey->styleKey = zfnull;
        }
        if(!_ZFP_styleKey->stylePropertyKeyMap.empty()) {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                _ZFP_styleKey->stylePropertyOnChangeListener);
        }
        zfpoolDelete(_ZFP_styleKey);
    }
}

ZF_NAMESPACE_GLOBAL_END

