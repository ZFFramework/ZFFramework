#include "ZFPathInfo.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPathInfo::ZFPathInfo(ZF_IN const zfchar *pathInfoString) {
    ZFPathInfoFromStringT(*this, pathInfoString);
}

ZF_NAMESPACE_GLOBAL_END

