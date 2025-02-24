/**
 * @file zfstllist.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstllist_h_
#define _ZFI_zfstllist_h_

#include "zfstl_define.h"
#include <list>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstllist
    #define zfstllist std::list
#endif

#endif // #ifndef _ZFI_zfstllist_h_

