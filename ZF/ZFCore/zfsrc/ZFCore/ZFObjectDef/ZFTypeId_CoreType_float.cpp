#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// utils
#define _ZFP_ZFTYPEID_DEFINE_float(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            if(!zfsToFloatT(v, src, srcLen)) { \
                if(errorHint) { \
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen)); \
                } \
                return zffalse; \
            } \
            return zftrue; \
        }, { \
            return zfsFromFloatT(s, v); \
        })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_float(zffloat, zffloat)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zffloat, zffloat)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_float(zfdouble, zfdouble)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfdouble, zfdouble)

ZF_NAMESPACE_GLOBAL_END

