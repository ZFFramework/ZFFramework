/**
 * @file ZFImpl_{ZFTT_R_proj_name}.h
 * @brief global header for {ZFTT_R_proj_name} module
 */

#ifndef _ZFI_ZFImpl_{ZFTT_R_proj_name}_h_
#define _ZFI_ZFImpl_{ZFTT_R_proj_name}_h_

#include "ZFImpl/ZFImpl_ZF_impl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief used to export symbols */
#ifndef ZFLIB_{ZFTT_R_proj_name}
    #define ZFLIB_{ZFTT_R_proj_name} ZF_ENV_IMPORT
#endif
// export a dummy class to ensure the lib really "contains" something
zfclassNotPOD ZFLIB_{ZFTT_R_proj_name} _ZFP_ZFLIB_{ZFTT_R_proj_name} {};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_{ZFTT_R_proj_name}_h_

