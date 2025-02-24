#include "zfweak.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

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

