#include "ZFValueHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFValueHolder
ZFOBJECT_REGISTER(ZFValueHolder)

void ZFValueHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, this->holdedData);
}
zfidentity ZFValueHolder::objectHash(void) {
    return zfidentityHash(
        zfidentityCalcPointer(this->holdedData),
        zfidentityCalcPointer(reinterpret_cast<void *>(this->holderType)));
}
ZFCompareResult ZFValueHolder::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->holdedData == another->holdedData
            && this->holderType == another->holderType
            ) {
        return ZFCompareEqual;
    }
    else {
        return ZFCompareUncomparable;
    }
}

ZF_NAMESPACE_GLOBAL_END

