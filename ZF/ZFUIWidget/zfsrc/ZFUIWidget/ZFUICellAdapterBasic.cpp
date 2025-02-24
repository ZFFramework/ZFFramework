#include "ZFUICellAdapterBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUICellAdapterBasicParam)

ZFOBJECT_REGISTER(ZFUICellAdapterBasic)

ZFEVENT_REGISTER(ZFUICellAdapterBasic, CellCount)
ZFEVENT_REGISTER(ZFUICellAdapterBasic, CellAt)
ZFEVENT_REGISTER(ZFUICellAdapterBasic, CellSizeAt)
ZFEVENT_REGISTER(ZFUICellAdapterBasic, CellCacheOnAccess)

ZF_NAMESPACE_GLOBAL_END

