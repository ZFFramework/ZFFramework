#include "ZFTypeId_spec.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const zfchar *, ZFTypeId_none)

// ============================================================
// void
_ZFP_ZFTYPEID_ID_DATA_REGISTER(void, void)

// ============================================================
// zfauto
ZFOUTPUT_TYPE_DEFINE(zfauto, {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        v.toObject()->objectInfoT(s);
    }
})

// ============================================================
// ZFAny
ZFOUTPUT_TYPE_DEFINE(ZFAny, {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        v.toObject()->objectInfoT(s);
    }
})

ZF_NAMESPACE_GLOBAL_END

