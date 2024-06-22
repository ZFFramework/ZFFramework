#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_ZFArgsPrivate {
public:
    zfuint refCount;
    zfbool eventFiltered;
    zfidentity eventId;
    zfany sender;
    zfany param0;
    zfany param1;
    zfany result;
public:
    _ZFP_ZFArgsPrivate(void)
    : refCount(1)
    , eventFiltered(zffalse)
    , eventId(zfidentityInvalid())
    , sender()
    , param0()
    , param1()
    , result()
    {
    }
};

ZFArgs &ZFArgs::eventId(ZF_IN zfidentity const &v) {
    d->eventId = v;
    return *this;
}
zfidentity const &ZFArgs::eventId(void) const {
    return d->eventId;
}
ZFArgs &ZFArgs::sender(ZF_IN zfany const &v) {
    d->sender = v;
    return *this;
}
zfany const &ZFArgs::sender(void) const {
    return d->sender;
}
ZFArgs &ZFArgs::param0(ZF_IN zfany const &v) {
    d->param0 = v;
    return *this;
}
zfany const &ZFArgs::param0(void) const {
    return d->param0;
}
ZFArgs &ZFArgs::param1(ZF_IN zfany const &v) {
    d->param1 = v;
    return *this;
}
zfany const &ZFArgs::param1(void) const {
    return d->param1;
}

zfany const &ZFArgs::result(void) const {
    return d->result;
}
ZFArgs const &ZFArgs::result(ZF_IN zfany const &result) const {
    zfRetainChange(d->result, result);
    return *this;
}
ZFArgs &ZFArgs::result(ZF_IN zfany const &result) {
    zfRetainChange(d->result, result);
    return *this;
}

ZFArgs const &ZFArgs::eventFiltered(ZF_IN zfbool eventFiltered) const {
    d->eventFiltered = eventFiltered;
    return *this;
}
ZFArgs &ZFArgs::eventFiltered(ZF_IN zfbool eventFiltered) {
    d->eventFiltered = eventFiltered;
    return *this;
}
zfbool ZFArgs::eventFiltered(void) const {
    return d->eventFiltered;
}

ZFArgs::ZFArgs(void) {
    d = zfpoolNew(_ZFP_ZFArgsPrivate);
    zfmemset(d, 0, sizeof(_ZFP_ZFArgsPrivate));
    d->refCount = 1;
    d->eventId = zfidentityInvalid();
}
ZFArgs::ZFArgs(ZF_IN const ZFArgs &ref) {
    d = ref.d;
    ++(d->refCount);
}

ZFArgs::~ZFArgs(void) {
    if(d->refCount == 1) {
        zfRelease(d->result);
        zfpoolDelete(d);
    }
    else {
        --(d->refCount);
    }
}
ZFArgs &ZFArgs::operator = (ZF_IN const ZFArgs &ref) {
    if(d != ref.d) {
        _ZFP_ZFArgsPrivate *dTmp = d;
        d = ref.d;
        ++(d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfRelease(dTmp->result);
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
zfbool ZFArgs::operator == (ZF_IN const ZFArgs &ref) const {
    return d == ref.d;
}

void ZFArgs::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += "ZFArgs";
    const zfchar *eventName = ZFEventNameForId(this->eventId());
    if(eventName != zfnull) {
        ret += " event:";
        ret += eventName;
    }
    if(this->sender() != zfnull) {
        ret += " sender:";
        ZFObjectInfoT(ret, this->sender());
    }
    if(this->param0() != zfnull) {
        ret += " param0:";
        ZFObjectInfoT(ret, this->param0());
    }
    if(this->param1() != zfnull) {
        ret += " param1:";
        ZFObjectInfoT(ret, this->param1());
    }
    if(this->result() != zfnull) {
        ret += " result:";
        ZFObjectInfoT(ret, this->result());
    }
    if(this->eventFiltered()) {
        ret += " filtered:";
        ret += ZFTOKEN_zfbool_zftrue;
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_GLOBAL_END

