#include "ZFProgressable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFProgressable)

zfbool ZFProgressable::progressUpdate(
        ZF_IN ZFProgressable *from
        , ZF_IN ZFProgressable *to
        , ZF_IN zffloat progress
        ) {
    const ZFMethod *m = this->classData()->methodForName("progressOnUpdate");
    if(m) {
        return m->methodInvoke(
                this->toObject()
                , zfcast(ZFObject *, from)
                , zfcast(ZFObject *, to)
                , zfobj<v_zffloat>(progress)
                ).to<v_zfbool *>()->zfv;
    }
    return this->progressOnUpdate(from, to, progress);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFProgressable, zfbool, progressUpdate
        , ZFMP_IN(ZFProgressable *, from)
        , ZFMP_IN(ZFProgressable *, to)
        , ZFMP_IN(zffloat, progress)
        )

ZF_NAMESPACE_GLOBAL_END

