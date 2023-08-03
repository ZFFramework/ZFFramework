#include "ZFTypeId_spec.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// void
_ZFP_ZFTYPEID_ID_DATA_REGISTER(void, void)

// ============================================================
// zfautoObject
ZFOUTPUT_TYPE_DEFINE(zfautoObject, {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        v.toObject()->objectInfoT(s);
    }
})

ZF_NAMESPACE_GLOBAL_END

