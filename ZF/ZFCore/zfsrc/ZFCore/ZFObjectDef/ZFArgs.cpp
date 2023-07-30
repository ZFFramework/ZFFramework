#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFArgs::~ZFArgs(void) {
    zfRelease(d.result);
}

ZFObject *const &ZFArgs::result(void) const {
    return d.result;
}
ZFArgs const &ZFArgs::result(ZF_IN ZFObject * const &result) const {
    if(this->_ZFP_result != zfnull) {
        zfRetainChange(*(this->_ZFP_result), result);
    }
    return *this;
}
ZFArgs &ZFArgs::result(ZF_IN ZFObject * const &result) {
    if(this->_ZFP_result != zfnull) {
        zfRetainChange(*(this->_ZFP_result), result);
    }
    return *this;
}

ZFArgs &ZFArgs::resultEnable(ZF_IN zfbool enable) {
    this->_ZFP_result = enable ? &(d.result) : zfnull;
    if(!enable) {
        zfRetainChange(d.result, zfnull);
    }
    return *this;
}
zfbool ZFArgs::resultEnabled(void) const {
    return (this->_ZFP_result != zfnull);
}

void ZFArgs::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfstringAppend(ret, "ZFArgs(%p)", this);
    const zfchar *eventName = ZFIdMapNameForId(this->eventId());
    if(eventName != zfnull) {
        ret += ", event: ";
        ret += eventName;
    }
    if(this->sender() != zfnull) {
        ret += ", sender: ";
        ZFObjectInfoT(ret, this->sender());
    }
    if(this->param0() != zfnull) {
        ret += ", param0: ";
        ZFObjectInfoT(ret, this->param0());
    }
    if(this->param1() != zfnull) {
        ret += ", param1: ";
        ZFObjectInfoT(ret, this->param1());
    }
    if(this->userData() != zfnull) {
        ret += ", userData: ";
        ZFObjectInfoT(ret, this->userData());
    }
    if(this->result() != zfnull) {
        ret += ", result: ";
        ZFObjectInfoT(ret, this->result());
    }
    if(this->eventFiltered()) {
        ret += ", filtered: ";
        ret += ZFTOKEN_zfbool_zftrue;
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_GLOBAL_END

