#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFStyleKeyHolder : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFStyleKeyHolder, ZFObject)
public:
    zfstring styleKey;
    zfstlhashmap<zfstring, zfstring> stylePropertyKeyMap;
    ZFListener styleOnUpdateListener;
    ZFListener stylePropertyOnUpdateListener;
public:
    static void styleOnUpdate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFStyleable *owner
            );
    static void stylePropertyOnUpdate(
            ZF_IN const ZFArgs &zfargs
            , ZF_IN ZFStyleable *owner
            );
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        if(this->styleOnUpdateListener) {
            ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), this->styleOnUpdateListener);
            this->styleOnUpdateListener = zfnull;
        }
        if(this->stylePropertyOnUpdateListener) {
            ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), this->stylePropertyOnUpdateListener);
            this->stylePropertyOnUpdateListener = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }
};

// ============================================================
void _ZFP_ZFStyleKeyHolder::styleOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN ZFStyleable *owner
        ) {
    ZFCoreMutexLocker();
    zfauto style = ZFStyleGet(owner->styleKey());
    if(style != zfnull) {
        ZFCoreAssertWithMessageTrim(style->classData()->classIsTypeOf(owner->classData())
            || owner->classData()->classIsTypeOf(style->classData()),
            "applying style from incorrect type, to object: %s, from style: %s",
            owner->toObject()->objectInfoOfInstance(),
            style->objectInfoOfInstance());

        owner->styleableCopyFrom(style);
    }
}
void ZFStyleable::styleKey(ZF_IN const zfstring &styleKey) {
    ZFCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(styleKey == zfnull) {
        if(holder != zfnull) {
            holder->styleKey = zfnull;
            ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->styleOnUpdateListener);
            holder->styleOnUpdateListener = zfnull;
        }
    }
    else {
        if(holder == zfnull) {
            zfobj<_ZFP_ZFStyleKeyHolder> holderTmp;
            holder = holderTmp;
            this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className(), holder);
        }
        if(holder->styleKey == zfnull) {
            if(!holder->styleOnUpdateListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(styleOnUpdate
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::styleOnUpdate(zfargs, owner);
                } ZFLISTENER_END()
                holder->styleOnUpdateListener = styleOnUpdate;
            }
            ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->styleOnUpdateListener);
        }
        holder->styleKey = styleKey;
        ZFArgs zfargs;
        _ZFP_ZFStyleKeyHolder::styleOnUpdate(zfargs, this);

        if(holder->styleKey == zfnull) {
            ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->styleOnUpdateListener);
            holder->styleOnUpdateListener = zfnull;

            ZFGlobalObserver().observerNotifyWithSender(
                    this->toObject()
                    , ZFGlobalEvent::E_ZFStyleOnInvalid()
                    , zfobj<v_zfstring>()
                    , zfobj<v_zfstring>(styleKey)
                    );
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
    if(getterMethod == zfnull || !getterMethod->isPublic()) {
        return zffalse;
    }
    const ZFMethod *setterMethod = propertyOwner->classData()->propertySetterForName(propertyName);
    if(setterMethod == zfnull || !setterMethod->isPublic()) {
        ZFStyleable *styleable = getterMethod->methodInvoke(propertyOwner);
        if(styleable == zfnull) {
            return zffalse;
        }
        styleable->styleableCopyFrom(styleValue);
        return zftrue;
    }
    else {
        ZFArgs zfargs;
        zfargs
            .sender(propertyOwner)
            .ownerMethod(setterMethod)
            .paramInit()
            .param0(styleValue)
            ;
        setterMethod->methodGenericInvoker()(zfargs);
        return zfargs.success();
    }
}
void _ZFP_ZFStyleKeyHolder::stylePropertyOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN ZFStyleable *owner
        ) {
    ZFCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = owner->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(holder != zfnull) {
        zfstlhashmap<zfstring, zfstring> &m = holder->stylePropertyKeyMap;
        for(zfstlhashmap<zfstring, zfstring>::iterator it = m.begin(); it != m.end(); ++it) {
            _ZFP_ZFStylePropertyCopy(owner->toObject(), it->first, it->second);
        }
    }
}
void ZFStyleable::propStyle(
        ZF_IN const zfstring &propertyName
        , ZF_IN const zfstring &styleKey
        ) {
    if(propertyName == zfnull) {
        return;
    }
    ZFCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(styleKey == zfnull) {
        if(holder != zfnull) {
            holder->stylePropertyKeyMap.erase(propertyName);
            if(holder->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->stylePropertyOnUpdateListener);
                holder->stylePropertyOnUpdateListener = zfnull;
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
            if(!holder->stylePropertyOnUpdateListener) {
                ZFStyleable *owner = this;
                ZFLISTENER_1(stylePropertyOnUpdate
                        , ZFStyleable *, owner
                        ) {
                    _ZFP_ZFStyleKeyHolder::stylePropertyOnUpdate(zfargs, owner);
                } ZFLISTENER_END()
                holder->stylePropertyOnUpdateListener = stylePropertyOnUpdate;
            }
            ZFGlobalObserver().observerAdd(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->stylePropertyOnUpdateListener);
        }
        if(!_ZFP_ZFStylePropertyCopy(this->toObject(), propertyName, styleKey)) {
            holder->stylePropertyKeyMap.erase(propertyName);
            if(holder->stylePropertyKeyMap.empty()) {
                ZFGlobalObserver().observerRemove(ZFGlobalEvent::E_ZFStyleOnUpdate(), holder->stylePropertyOnUpdateListener);
                holder->stylePropertyOnUpdateListener = zfnull;
            }
            ZFGlobalObserver().observerNotifyWithSender(
                    this->toObject()
                    , ZFGlobalEvent::E_ZFStyleOnInvalid()
                    , zfobj<v_zfstring>(propertyName)
                    , zfobj<v_zfstring>(styleKey)
                    );
        }
    }
}
const zfstring &ZFStyleable::propStyle(ZF_IN const zfstring &propertyName) {
    if(propertyName == zfnull) {
        return zfstring::Empty();
    }
    ZFCoreMutexLocker();
    _ZFP_ZFStyleKeyHolder *holder = this->toObject()->objectTag(_ZFP_ZFStyleKeyHolder::ClassData()->className());
    if(holder != zfnull) {
        zfstlhashmap<zfstring, zfstring>::iterator it = holder->stylePropertyKeyMap.find(propertyName);
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

ZF_NAMESPACE_GLOBAL_END

