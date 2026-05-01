/**
 * @file zfstlvector.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstlvector_h_
#define _ZFI_zfstlvector_h_

#include "zfstl_define.h"
#include <vector>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlvector
    #define zfstlvector zft_vector

    /** @cond ZFPrivateDoc */
    template<typename T>
    zfclassLikePOD zft_vector : zfextend std::vector<T, zfstlallocator<T> > {
    protected:
        typedef zft_vector<T> zfself;
        typedef std::vector<T, zfstlallocator<T> > zfsuper;
    public:
        zft_vector(void) : zfsuper() {}
        zft_vector(zfself const &ref) : zfsuper(ref) {}
        template<typename Iter>
        zft_vector(Iter first, Iter last) : zfsuper(first, last) {}
        zft_vector(zfself::size_type n, const zfself::value_type &v = zfself::value_type()) : zfsuper(n, v) {}
    };
    /** @endcond */
#endif

#endif // #ifndef _ZFI_zfstlvector_h_

