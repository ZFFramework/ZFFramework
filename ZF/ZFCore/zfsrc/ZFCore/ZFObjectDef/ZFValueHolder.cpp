#include "ZFValueHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFValueHolder
ZFOBJECT_REGISTER(ZFValueHolder)

void ZFValueHolder::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, _value);
}
ZFCompareResult ZFValueHolder::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->_value == another->_value
            && this->_type == another->_type
            ) {
        return ZFCompareEqual;
    }
    else {
        return ZFCompareUncomparable;
    }
}

ZF_NAMESPACE_GLOBAL_END

