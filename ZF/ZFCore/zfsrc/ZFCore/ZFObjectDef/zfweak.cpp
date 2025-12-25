#include "zfweak.h"
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

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(zfweak, zfweak)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        v_zfweak *owner = invokerObject;
        v_zfweak *v = zfcast(v_zfweak *, obj);
        if(v) {
            owner->zfv = v->zfv;
        }
        else {
            owner->zfv.set(obj);
        }
    }, v_zfweak
    , ZFMP_IN(ZFObject *, obj)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfweak, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfweak, zfany, get)
ZFMETHOD_USER_REGISTER_1({
    v_zfweak *owner = invokerObject;
    v_zfweak *v = zfcast(v_zfweak *, obj);
    if(v) {
        owner->zfv = v->zfv;
    }
    else {
        owner->zfv.set(obj);
    }
}, v_zfweak, void, set
, ZFMP_IN(ZFObject *, obj)
)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfweak, zfany, toObject)

ZF_NAMESPACE_GLOBAL_END

