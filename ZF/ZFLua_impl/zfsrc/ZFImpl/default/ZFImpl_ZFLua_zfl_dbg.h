/**
 * @file ZFImpl_ZFLua_zfl_dbg.h
 * @brief path info impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_zfl_dbg_h_
#define _ZFI_ZFImpl_ZFLua_zfl_dbg_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief debug port for lua, 30000 by default
 *
 * possible values:
 * -  `0` : read command from `stdin`
 * -  `> 0` : start a local http server with specified port, read command from client request
 *
 * @code
 *   // from other client, curl POST with debugger.lua's command as body contents, e.g.:
 *   curl -X POST --data 'p myVar' 'http://localhost:30000'
 * @endcode
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFLua_impl, zfuint, ZFLuaDebugPort)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_zfl_dbg_h_

