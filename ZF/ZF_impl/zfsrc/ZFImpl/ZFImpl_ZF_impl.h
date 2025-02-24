/**
 * @file ZFImpl_ZF_impl.h
 * @brief global header for ZF_impl module
 */

#ifndef _ZFI_ZFImpl_ZF_impl_h_
#define _ZFI_ZFImpl_ZF_impl_h_

#include "ZFCore/ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_ZF_impl
    #define ZFLIB_ZF_impl ZF_ENV_IMPORT
#endif
// export a dummy class to ensure the lib really "contains" something
zfclassNotPOD ZFLIB_ZF_impl _ZFP_ZFLIB_ZF_impl {};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_ZF_impl_h_

