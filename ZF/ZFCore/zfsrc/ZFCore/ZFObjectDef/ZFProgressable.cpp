#include "ZFProgressable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFProgressable)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFProgressable, zfbool, progressUpdate
        , ZFMP_IN(ZFProgressable *, from)
        , ZFMP_IN(ZFProgressable *, to)
        , ZFMP_IN(zffloat, progress)
        )

ZF_NAMESPACE_GLOBAL_END
#endif

