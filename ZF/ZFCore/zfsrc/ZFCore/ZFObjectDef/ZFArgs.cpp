#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFArgs::~ZFArgs(void)
{
    zfRelease(this->_ZFP_resultHolder);
}

ZFObject *const &ZFArgs::result(void) const
{
    return this->_ZFP_resultHolder;
}
ZFArgs const &ZFArgs::result(ZF_IN ZFObject * const &result) const
{
    if(this->_ZFP_result != zfnull)
    {
        zfRetainChange(*(this->_ZFP_result), result);
    }
    return *this;
}

ZFArgs const &ZFArgs::resultEnable(ZF_IN zfbool enable)
{
    this->_ZFP_result = enable ? &(this->_ZFP_resultHolder) : zfnull;
    if(!enable)
    {
        zfRetainChange(this->_ZFP_resultHolder, zfnull);
    }
    return *this;
}
zfbool ZFArgs::resultEnabled(void) const
{
    return (this->_ZFP_result != zfnull);
}

void ZFArgs::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfstringAppend(ret, "ZFArgs(%p)", this);
    const zfchar *eventName = ZFIdMapNameForId(this->eventId());
    if(eventName != zfnull)
    {
        ret += ", event: ";
        ret += eventName;
    }
    if(this->sender() != zfnull)
    {
        ret += ", sender: ";
        ZFObjectInfoT(ret, this->sender());
    }
    if(this->param0() != zfnull)
    {
        ret += ", param0: ";
        ZFObjectInfoT(ret, this->param0());
    }
    if(this->param1() != zfnull)
    {
        ret += ", param1: ";
        ZFObjectInfoT(ret, this->param1());
    }
    if(this->result() != zfnull)
    {
        ret += ", result: ";
        ZFObjectInfoT(ret, this->result());
    }
    if(this->eventFiltered())
    {
        ret += ", filtered: ";
        ret += ZFTOKEN_zfbool_zftrue;
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_GLOBAL_END

