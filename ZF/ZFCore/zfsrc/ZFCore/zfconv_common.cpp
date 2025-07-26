#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfconv_common(T_To, T_From) \
    ZFCONV_REG(v_##T_To, v_##T_From) { \
        ret = zfobj<v_##T_To>((T_To)obj->to<v_##T_From *>()->zfv); \
        ret->objectTag("zfconv", obj); \
        return zftrue; \
    }

// ============================================================
ZFCONV_REG(v_zfptr, v_zfstring) {
    ret = zfobj<v_zfptr>((const void *)obj->to<v_zfstring *>()->zfv.cString());
    ret->objectTag("zfconv", obj);
    return zftrue;
}
ZFCONV_REG(v_zfstring, v_zfchar) {
    ret = zfobj<v_zfstring>(zfstring(&(obj->to<v_zfchar *>()->zfv), sizeof(zfchar)));
    ret->objectTag("zfconv", obj);
    return zftrue;
}

// ============================================================
_ZFP_zfconv_common(zflong, zfint)
_ZFP_zfconv_common(zfulong, zfuint)
_ZFP_zfconv_common(zfindex, zfuint)

// ============================================================
_ZFP_zfconv_common(zfdouble, zffloat)

ZF_NAMESPACE_GLOBAL_END

