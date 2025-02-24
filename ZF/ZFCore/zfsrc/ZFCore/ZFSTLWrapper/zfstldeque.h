/**
 * @file zfstldeque.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstldeque_h_
#define _ZFI_zfstldeque_h_

#include "zfstl_define.h"
#include <deque>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstldeque
    #define zfstldeque std::deque
#endif

#endif // #ifndef _ZFI_zfstldeque_h_

