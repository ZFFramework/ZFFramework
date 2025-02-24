/**
 * @file ZFImpl_ZFLua_impl.h
 * @brief global header for ZFLua_impl module
 */

#ifndef _ZFI_ZFImpl_ZFLua_impl_h_
#define _ZFI_ZFImpl_ZFLua_impl_h_

#include "ZFImpl/ZFImpl_ZF_impl.h"
#include "_repo/lua/ELuna.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_ZFLua_impl
    #define ZFLIB_ZFLua_impl ZF_ENV_IMPORT
#endif
// export a dummy class to ensure the lib really "contains" something
zfclassNotPOD ZFLIB_ZFLua_impl _ZFP_ZFLIB_ZFLua_impl {};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_ZFLua_impl_h_

