#include "ZFTypeId_spec.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_zfvAccessFinishDelayCb &_ZFP_zfvAccessFinishDelayImpl(void) {
    static _ZFP_zfvAccessFinishDelayCb d = zfnull;
    return d;
}

zfauto _ZFP_genericAccessFinishWrap(ZF_IN const zfauto &ret, ZF_IN void *v, ZF_IN void (*type)(ZF_IN void *)) {
    zfobj<ZFValueHolder> holder(v, type);
    if(ret) {
        ret->objectTag(zfstr("_ZFP_GAFW_%s", (void *)type), holder);
        return ret;
    }
    else {
        return holder;
    }
}

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const zfchar *, ZFTypeId_none)

// ============================================================
// void
typedef void _ZFP_PropTypeW_void;
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

