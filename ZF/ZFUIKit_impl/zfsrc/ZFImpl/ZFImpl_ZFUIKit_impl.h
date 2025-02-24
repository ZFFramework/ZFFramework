/**
 * @file ZFImpl_ZFUIKit_impl.h
 * @brief global header for ZFUIKit_impl module
 */

#ifndef _ZFI_ZFImpl_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_ZFUIKit_impl_h_

#include "ZFImpl/ZFImpl_ZF_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_ZFUIKit_impl
    #define ZFLIB_ZFUIKit_impl ZF_ENV_IMPORT
#endif
// export a dummy class to ensure the lib really "contains" something
zfclassNotPOD ZFLIB_ZFUIKit_impl _ZFP_ZFLIB_ZFUIKit_impl {};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_ZFUIKit_impl_h_

