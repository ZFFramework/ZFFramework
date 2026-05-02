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
    #if ZF_ENV_ZFSTL_ALLOCATOR_ENABLE
        #define zfstldeque zft_deque

        /** @cond ZFPrivateDoc */
        template<typename T>
        zfclassLikePOD zft_deque : zfextend std::deque<T, zfstlallocator<T> > {
        protected:
            typedef zft_deque<T> zfself;
            typedef std::deque<T, zfstlallocator<T> > zfsuper;
        public:
            zft_deque(void) : zfsuper() {}
            zft_deque(zfself const &ref) : zfsuper(ref) {}
            template<typename Iter>
            zft_deque(Iter first, Iter last) : zfsuper(first, last) {}
            zft_deque(zfstlsize n, T const &v = T()) : zfsuper(n, v) {}
        };
        /** @endcond */
    #else
        #define zfstldeque std::deque
    #endif
#endif

#endif // #ifndef _ZFI_zfstldeque_h_

