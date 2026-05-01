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
        zft_list(zfself::size_type n, const zfself::value_type &v = zfself::value_type()) : zfsuper(n, v) {}
    };
    /** @endcond */
#endif

#endif // #ifndef _ZFI_zfstllist_h_

