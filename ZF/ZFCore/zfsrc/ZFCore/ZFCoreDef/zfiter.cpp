#include "zfiter.h"
#include "zfstr.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void zfiter::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "it<%s>", (const void *)this->impl());
}

ZF_NAMESPACE_GLOBAL_END

