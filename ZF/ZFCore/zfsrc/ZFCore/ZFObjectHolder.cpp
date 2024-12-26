#include "ZFObjectHolder.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFObjectHolder)

ZFOBJECT_ON_INIT_DEFINE_1(ZFObjectHolder, ZFMP_IN(ZFObject *, v)) {
    this->objectOnInit();
    this->value(v);
}

ZF_NAMESPACE_GLOBAL_END

