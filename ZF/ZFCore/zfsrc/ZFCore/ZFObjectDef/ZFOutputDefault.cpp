#include "ZFOutputDefault.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfindex _ZFP_ZFOutputDefaultImpl(
        ZF_IN const void *src
        , ZF_IN_OPT zfindex count = zfindexMax()
        ) {
    // try to print to std output
    fprintf(stderr, "%s", (const zfchar *)src);
    return count;
}

static ZFOutput _ZFP_ZFOutputDefault = ZFCallbackForFunc(_ZFP_ZFOutputDefaultImpl);

const ZFOutput &ZFOutputDefault(void) {
    return _ZFP_ZFOutputDefault;
}
void ZFOutputDefault(ZF_IN const ZFOutput &v) {
    _ZFP_ZFOutputDefault = v;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const ZFOutput &, ZFOutputDefault)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFOutputDefault
        , ZFMP_IN(const ZFOutput &, v)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

