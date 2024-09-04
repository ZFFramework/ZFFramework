#include "ZFOutputForConsole.h"
#include "ZFImplOutput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForConsole, ZFCallbackSerializeType_ZFOutputForConsole) {
    ret = ZFOutputForConsole();
    serializableData.resolveMark();
    return ret;
}

// ============================================================
// ZFOutputForConsole
static zfindex _ZFP_ZFOutputForConsoleFunction(
        ZF_IN const void *s
        , ZF_IN zfindex count
        ) {
    if(count == zfindexMax()) {
        ZFImplOutput((const zfchar *)s, zfindexMax());
        return zfslen((const zfchar *)s) * sizeof(zfchar);
    }
    else {
        ZFImplOutput((const zfchar *)s, count / sizeof(zfchar));
        return count;
    }
}
static ZFOutput _ZFP_ZFOutputForConsole_create(void) {
    ZFOutput ret = ZFCallbackForFunc(_ZFP_ZFOutputForConsoleFunction);
    ret.callbackSerializeType(ZFCallbackSerializeType_ZFOutputForConsole);
    ret.callbackSerializeData(ZFSerializableData());
    return ret;
}

ZFEXPORT_VAR_DEFINE(ZFOutput, ZFOutputForConsole, _ZFP_ZFOutputForConsole_create())

ZF_NAMESPACE_GLOBAL_END

