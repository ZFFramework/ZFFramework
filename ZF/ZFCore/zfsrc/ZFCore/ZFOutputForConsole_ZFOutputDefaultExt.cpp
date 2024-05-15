#include "ZFOutputForConsole.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputForConsole_ZFOutputDefaultExt, ZFLevelZFFrameworkEssential) {
    saved = ZFOutputDefault();
    ZFOutputDefault(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputForConsole_ZFOutputDefaultExt) {
    ZFOutputDefault(saved);
}
ZFOutput saved;
ZF_GLOBAL_INITIALIZER_END(ZFOutputForConsole_ZFOutputDefaultExt)

ZF_NAMESPACE_GLOBAL_END

