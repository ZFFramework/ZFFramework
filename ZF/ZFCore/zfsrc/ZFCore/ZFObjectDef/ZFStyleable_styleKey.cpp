#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFStyleKeyHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFStyleKeyHolder, ZFObject)
public:
    zfstring styleKey;
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
}
void ZFStyleable::styleKey(ZF_IN const zfstring &styleKey) {
    zfCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(styleKey == zfnull) {
        if(holder != zfnull) {
            holder->styleKey = styleKey;
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                holder->styleOnChangeListener);
        }
    }
    else {
        if(holder == zfnull) {
            zfobj<_ZFP_ZFStyleKeyHolder> holderTmp;
            holder = holderTmp;
            this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className(), holder);
        }
        if(holder->styleKey == zfnull) {
            if(!holder->styleOnChangeListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(styleOnChange
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::styleOnChange(zfargs, owner);
                } ZFLISTENER_END()
                holder->styleOnChangeListener = styleOnChange;
            }
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                holder->styleOnChangeListener);
        }
        holder->styleKey = styleKey;
        ZFArgs zfargs;
        _ZFP_ZFStyleKeyHolder::styleOnChange(zfargs, this);

        if(holder->styleKey == zfnull) {
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
const zfstring &ZFStyleable::styleKey(void) {
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    return holder ? holder->styleKey : zfstring::Empty();
}

// ============================================================
static zfbool _ZFP_ZFStylePropertyCopy(
        ZF_IN ZFObject *propertyOwner
        , ZF_IN const zfstring &propertyName
        , ZF_IN const zfstring &styleKey
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
        ZFStyleable *styleable = getterMethod->methodInvoke(propertyOwner);
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
        return setterMethod->methodGenericInvoker()(retDummy, zfnull, propertyOwner, setterMethod, 1, paramDummy);
    }
}
void _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN ZFStyleable *owner
        ) {
    zfCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = owner->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(holder != zfnull) {
        zfstlmap<zfstring, zfstring> &m = holder->stylePropertyKeyMap;
        for(zfstlmap<zfstring, zfstring>::iterator it = m.begin(); it != m.end(); ++it) {
            _ZFP_ZFStylePropertyCopy(owner->toObject(), it->first, it->second);
        }
    }
}
void ZFStyleable::styleKeyForProperty(
        ZF_IN const zfstring &propertyName
        , ZF_IN const zfstring &styleKey
        ) {
    if(propertyName == zfnull) {
        return;
    }
    zfCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(styleKey == zfnull) {
        if(holder != zfnull) {
            holder->stylePropertyKeyMap.erase(propertyName);
            if(holder->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    holder->stylePropertyOnChangeListener);
            }
        }
    }
    else {
        if(holder == zfnull) {
            zfobj<_ZFP_ZFStyleKeyHolder> holderTmp;
            holder = holderTmp;
            this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className(), holder);
        }
        zfbool oldEmpty = holder->stylePropertyKeyMap.empty();
        holder->stylePropertyKeyMap[propertyName] = styleKey;
        if(oldEmpty) {
            if(!holder->stylePropertyOnChangeListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(stylePropertyOnChange
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(zfargs, owner);
                } ZFLISTENER_END()
                holder->stylePropertyOnChangeListener = stylePropertyOnChange;
            }
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                holder->stylePropertyOnChangeListener);
        }
        if(!_ZFP_ZFStylePropertyCopy(this->toObject(), propertyName, styleKey)) {
            holder->stylePropertyKeyMap.erase(propertyName);
            if(holder->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    holder->stylePropertyOnChangeListener);
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
const zfstring &ZFStyleable::styleKeyForProperty(ZF_IN const zfstring &propertyName) {
    if(propertyName == zfnull) {
        return zfstring::Empty();
    }
    zfCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(holder != zfnull) {
        zfstlmap<zfstring, zfstring>::iterator it = holder->stylePropertyKeyMap.find(propertyName);
        if(it != holder->stylePropertyKeyMap.end()) {
            return it->second;
        }
        else {
            return zfstring::Empty();
        }
    }
    else {
        return zfstring::Empty();
    }
}

ZFINTERFACE_ON_DEALLOC_DEFINE(ZFStyleable) {
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(holder) {
        if(holder->styleKey != zfnull) {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                holder->styleOnChangeListener);
            holder->styleKey = zfnull;
        }
        if(!holder->stylePropertyKeyMap.empty()) {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                holder->stylePropertyOnChangeListener);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

