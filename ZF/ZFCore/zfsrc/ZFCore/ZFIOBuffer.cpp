#include "ZFIOBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFIOBuffer)

ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFInput, input) {
    return this->implInput();
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFOutput, output) {
    return this->implOutput();
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, void, removeAll) {
    this->implRemoveAll();
}

ZF_NAMESPACE_GLOBAL_END

