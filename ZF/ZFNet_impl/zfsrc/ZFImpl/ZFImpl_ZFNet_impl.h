/**
 * @file ZFImpl_ZFNet_impl.h
 * @brief global header for ZFNet_impl module
 */

#ifndef _ZFI_ZFImpl_ZFNet_impl_h_
#define _ZFI_ZFImpl_ZFNet_impl_h_

#include "ZFImpl/ZFImpl_ZF_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_ZFNet_impl
    #define ZFLIB_ZFNet_impl ZF_ENV_IMPORT
#endif
// export a dummy class to ensure the lib really "contains" something
zfclassNotPOD ZFLIB_ZFNet_impl _ZFP_ZFLIB_ZFNet_impl {};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_ZFNet_impl_h_

