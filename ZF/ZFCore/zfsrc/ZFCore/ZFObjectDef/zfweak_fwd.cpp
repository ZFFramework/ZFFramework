#include "zfweak_fwd.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(_ZFP_I_zfweak)

void _ZFP_I_zfweak::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += " ";
    if(this->_ZFP_obj != zfnull) {
        this->_ZFP_obj->objectInfoT(ret);
    }
    else {
        ret += ZFTOKEN_zfnull;
    }
}

zfidentity _ZFP_I_zfweak::objectHashImpl(void) {
    return ZFObjectHash(this->_ZFP_obj);
}
ZFCompareResult _ZFP_I_zfweak::objectCompareImpl(ZF_IN ZFObject *anotherObj) {
    zfself *another = zfcast(zfself *, anotherObj);
    if(anotherObj != zfnull && another == zfnull) {return ZFCompareUncomparable;}

    return ZFObjectCompare(this->_ZFP_obj, another ? another->_ZFP_obj : zfnull);
}

ZF_NAMESPACE_GLOBAL_END

