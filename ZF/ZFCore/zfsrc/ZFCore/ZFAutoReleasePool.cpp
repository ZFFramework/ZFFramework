#include "ZFAutoReleasePool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFAutoReleasePool)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFAutoReleasePool, instance, ZFLevelZFFrameworkHigh)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAutoReleasePoolCleanup, ZFLevelZFFrameworkNormal) {
    ZFAutoReleasePool::instance()->poolDrain();
}
ZF_GLOBAL_INITIALIZER_END(ZFAutoReleasePoolCleanup)

void ZFAutoReleasePool::objectOnDeallocPrepare(void) {
    this->poolDrain();
    zfsuper::objectOnDeallocPrepare();
}
void ZFAutoReleasePool::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ret += ", count: ";
    zfsFromIntT(ret, d.count());
}

ZF_NAMESPACE_GLOBAL_END

