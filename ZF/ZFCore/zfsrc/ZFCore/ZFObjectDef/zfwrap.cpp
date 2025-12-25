#include "zfwrap.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(zfwrap, zfwrap)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        v_zfwrap *owner = invokerObject;
        v_zfwrap *v = zfcast(v_zfwrap *, obj);
        if(v) {
            owner->zfv = v->zfv;
        }
        else {
            owner->zfv.set(obj);
        }
    }, v_zfwrap
    , ZFMP_IN(ZFObject *, obj)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfwrap, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfwrap, zfany, get)
ZFMETHOD_USER_REGISTER_1({
    v_zfwrap *owner = invokerObject;
    v_zfwrap *v = zfcast(v_zfwrap *, obj);
    if(v) {
        owner->zfv = v->zfv;
    }
    else {
        owner->zfv.set(obj);
    }
}, v_zfwrap, void, set
, ZFMP_IN(ZFObject *, obj)
)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfwrap, zfany, toObject)

zfauto *_ZFP_zfwrapAccess(ZF_IN const zfauto &obj) {
    v_zfwrap *t = obj;
    if(t) {
        return &(t->zfv);
    }
    else {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

