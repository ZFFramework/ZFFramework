#include "ZFTypeId_spec.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const zfchar *, ZFTypeId_none)

// ============================================================
// void
_ZFP_ZFTYPEID_ID_DATA_REGISTER(void, void)

// ============================================================
// const void * / void *
ZFTYPEID_ACCESS_ONLY_DEFINE(zfptr, const void *)
ZFTYPEID_ALIAS_DEFINE(zfptr, const void *, zfptrW, void *)

// ============================================================
// zfauto
ZFOUTPUT_TYPE_DEFINE(zfauto, {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        v->objectInfoT(s);
    }
})

// ============================================================
// zfany
ZFOUTPUT_TYPE_DEFINE(zfany, {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        v->objectInfoT(s);
    }
})

ZF_NAMESPACE_GLOBAL_END

