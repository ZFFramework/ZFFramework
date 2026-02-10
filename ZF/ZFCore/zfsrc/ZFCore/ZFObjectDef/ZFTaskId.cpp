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
ZFOBJECT_ON_INIT_USER_REGISTER_1({
    ZFTaskIdBasic *v = invokerObject;
    v->stopImpl(stopImpl);
}, ZFTaskIdBasic
, ZFMP_IN(const ZFListener &, stopImpl)
)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTaskIdBasic, void, stopImpl
        , ZFMP_IN(const ZFListener &, v)
        )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFTaskIdBasic, const ZFListener &, stopImpl)

ZF_NAMESPACE_GLOBAL_END

