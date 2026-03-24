#include "ZFValue.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFValue
ZFOBJECT_REGISTER(ZFValue)

void ZFValue::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, _value);
}
ZFCompareResult ZFValue::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
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

