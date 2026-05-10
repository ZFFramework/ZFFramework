#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfconv_common(T_To, T_From) \
    ZFCONV_REGISTER(v_##T_To, v_##T_From) { \
        ret = zfobj<v_##T_To>((T_To)(obj.to<v_##T_From *>()->zfv)); \
        return zftrue; \
    }

// ============================================================
ZFCONV_REGISTER(v_zfptr, v_zfstring) {
    ret = zfobj<v_zfptr>((const void *)(obj.to<v_zfstring *>()->zfv.cString()));
    ret->objectTag("zfconv", obj);
    return zftrue;
}
ZFCONV_REGISTER(v_zfstring, v_zfchar) {
    ret = zfobj<v_zfstring>(zfstring(&(obj.to<v_zfchar *>()->zfv), sizeof(zfchar)));
    return zftrue;
}

// ============================================================
_ZFP_zfconv_common(zflong, zfint)

_ZFP_zfconv_common(zfulong, zfuint)
_ZFP_zfconv_common(zfindex, zfuint)

// ============================================================
_ZFP_zfconv_common(zfdouble, zffloat)

// ============================================================
ZFCONV_REGISTER(v_zfchar, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    const zfstring &s = t->valueString();
    if(s.length() != 1) {
        return zffalse;
    }
    ret = zfobj<v_zfchar>(s[0]);
    return zftrue;
}
ZFCONV_REGISTER(v_zfstring, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    ret = zfobj<v_zfstring>(t->valueString().sharedCopy());
    return zftrue;
}

// ============================================================
ZFCONV_REGISTER(v_zfindex, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfindex)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfindex>(v == -1 ? zfindexMax() : (zfindex)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfint, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || v != (zflong)(zfint)v
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfint>((zfint)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfuint, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfuint)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfuint>(v == -1 ? (zfuint)-1 : (zfuint)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zflong, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)) {
        return zffalse;
    }
    ret = zfobj<v_zflong>(v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfulong, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfulong)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfulong>(v == -1 ? (zfulong)-1 : (zfulong)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zffloat, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zfdouble v;
    if(!t->valueFloatT(v)) {
        return zffalse;
    }
    ret = zfobj<v_zffloat>((zffloat)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfdouble, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zfdouble v;
    if(!t->valueFloatT(v)) {
        return zffalse;
    }
    ret = zfobj<v_zfdouble>(v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfbyte, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfbyte)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfbyte>(v == -1 ? (zfbyte)-1 : (zfbyte)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zftimet, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zftimet)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zftimet>(v == -1 ? (zftimet)-1 : (zftimet)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfflags, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfflags)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfflags>(v == -1 ? (zfflags)-1 : (zfflags)v);
    return zftrue;
}
ZFCONV_REGISTER(v_zfidentity, ZFDI_Wrapper) {
    ZFDI_Wrapper *t = obj;
    zflong v;
    if(!t->valueIntT(v)
            || (v != -1 && v != (zflong)(zfidentity)v)
            ) {
        return zffalse;
    }
    ret = zfobj<v_zfidentity>(v == -1 ? zfidentityInvalid() : (zfidentity)v);
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

