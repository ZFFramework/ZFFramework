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
    #if ZF_ENV_ZFSTL_ALLOCATOR_ENABLE
        #define zfstllist zft_list

        /** @cond ZFPrivateDoc */
        template<typename T>
        zfclassLikePOD zft_list : zfextend std::list<T, zfstlallocator<T> > {
        protected:
            typedef zft_list<T> zfself;
            typedef std::list<T, zfstlallocator<T> > zfsuper;
        public:
            zft_list(void) : zfsuper() {}
            zft_list(zfself const &ref) : zfsuper(ref) {}
            template<typename Iter>
            zft_list(Iter first, Iter last) : zfsuper(first, last) {}
            zft_list(zfstlsize n, T const &v = T()) : zfsuper(n, v) {}
        };
        /** @endcond */
    #else
        #define zfstllist std::list
    #endif
#endif

#endif // #ifndef _ZFI_zfstllist_h_

