#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFIdMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// utils
#define _ZFP_ZFTYPEID_DEFINE_int_allow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            if(!zfsToIntT(v, src, srcLen, 10, zftrue)) { \
                if(errorHint) { \
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen)); \
                } \
                return zffalse; \
            } \
            return zftrue; \
        }, { \
            return zfsFromIntT(s, v); \
        })
#define _ZFP_ZFTYPEID_DEFINE_int_disallow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            if(src != zfnull && zfsncmp(src, "-1", 2) == 0) { \
                v = (Type)-1; \
                return zftrue; \
            } \
            else { \
                if(!zfsToIntT(v, src, srcLen, 10, zffalse)) { \
                    if(errorHint) { \
                        zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen)); \
                    } \
                    return zffalse; \
                } \
                return zftrue; \
            } \
        }, { \
            if(v == ((Type)-1)) { \
                s += "-1"; \
                return zftrue; \
            } \
            else { \
                return zfsFromIntT(s, v); \
            } \
        })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zfint, zfint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfint, zfint)
// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfuint, zfuint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfuint, zfuint)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zflong, zflong)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zflong, zflong)
// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfulong, zfulong)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfulong, zfulong)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfindex, zfindex)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfindex, zfindex)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zftimet, zftimet)
ZFTYPEID_PROGRESS_DEFINE(zftimet, zftimet, {
        ret = (zftimet)(from + (zftimet)((to - from) * progress));
    })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zfidentity, zfidentity)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfflags, zfflags)

ZF_NAMESPACE_GLOBAL_END

