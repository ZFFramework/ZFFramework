#include "ZFObjectHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFObjectHolder)

void ZFObjectHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    if(this->_ZFP_objectHolded != zfnull) {
        this->_ZFP_objectHolded->objectInfoT(ret);
    }
    else {
        ret += ZFTOKEN_zfnull;
    }
}

zfidentity ZFObjectHolder::objectHash(void) {
    return ZFObjectHash(this->_ZFP_objectHolded.toObject());
}
ZFCompareResult ZFObjectHolder::objectCompare(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFObjectCompare(this->_ZFP_objectHolded.toObject(), another->_ZFP_objectHolded.toObject());
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObjectHolder, void, objectHolded
        , ZFMP_IN(ZFObject *, obj)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObjectHolder, ZFAny, objectHolded)

ZF_NAMESPACE_GLOBAL_END
#endif

