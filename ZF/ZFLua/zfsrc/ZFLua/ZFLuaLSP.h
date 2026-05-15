/**
 * @file ZFLuaLSP.h
 * @brief util to export a lua file for LSP
 */

#ifndef _ZFI_ZFLuaLSP_h_
#define _ZFI_ZFLuaLSP_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief a list of local path to map
 *
 * #ZFSourceCodeMap::Enable must be set to use
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFLua, ZFCoreArray<zfstring>, ZFLuaLSPGenSourceRootList)
/**
 * @brief when use #ZFLuaLSPGenSourceRootList to map, whether to check file exists
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFLua, zfbool, ZFLuaLSPGenSourceRootCheckExist)

// ============================================================
/**
 * @brief util to export a lua file for LSP (Language Server Protocol)
 *
 * include the generated lua file for LSP plugins,
 * then your LSP plugin should be able to perform semantic completion\n
 * recommended LSP
 * -  lua-language-server : https://github.com/LuaLS/lua-language-server
 * -  EmmyLua : https://github.com/EmmyLua/EmmyLua-LanguageServer
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaLSPGen
        , ZFMP_IN(const ZFOutput &, output)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaLSP_h_

