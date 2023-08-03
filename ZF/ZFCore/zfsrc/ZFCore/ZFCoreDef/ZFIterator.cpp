#include "ZFIterator.h"
#include "zfstr.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const zfiterator _ZFP_zfiteratorInvalid;

void zfiterator::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "it<%s>", this->data());
}

ZF_NAMESPACE_GLOBAL_END

