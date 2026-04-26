#include "ZFTaskId.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFTaskId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTaskId, void, stop)

void ZFTaskId::stop(void) {
}

// ============================================================
ZFOBJECT_REGISTER(ZFTaskIdDummy)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTaskIdDummy, void, stopped
        , ZFMP_IN(const zfbool &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTaskIdDummy, const zfbool &, stopped)

// ============================================================
ZFOBJECT_REGISTER(ZFTaskIdBasic)
ZFOBJECT_ON_INIT_USER_REGISTER_1(ZFTaskIdBasic
    , ZFMP_IN(const ZFListener &, stopImpl)
    ) {
    ZFTaskIdBasic *v = invokerObject;
    v->stopImpl(stopImpl);
}
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTaskIdBasic, void, stopImpl
        , ZFMP_IN(const ZFListener &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTaskIdBasic, const ZFListener &, stopImpl)

// ============================================================
ZFOBJECT_REGISTER(ZFTaskIdWrapper)
ZFOBJECT_ON_INIT_USER_REGISTER_1(ZFTaskIdWrapper
    , ZFMP_IN(const zfautoT<ZFTaskId> &, impl)
    ) {
    ZFTaskIdWrapper *v = invokerObject;
    v->impl(impl);
}
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTaskIdWrapper, void, impl
        , ZFMP_IN(const zfautoT<ZFTaskId> &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTaskIdWrapper, const zfautoT<ZFTaskId> &, impl)

ZF_NAMESPACE_GLOBAL_END

