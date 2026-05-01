/**
 * @file zfstl_define.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_define_h_
#define _ZFI_zfstl_define_h_

#include "../ZFCoreDef.h"

/**
 * @brief size_t wrapper
 *
 * take good care of that, zfindex is not ensured same as size_t,
 * so it is not ensured that zfindexMax() is equal to string::npos,
 * e.g.:
 * @code
 *   zfindex n = string::npos;
 *   zfbool b0 = (n == string::npos); // zftrue
 *   zfbool b1 = (zfindexMax() == string::npos); // not ensured
 *
 *   size_t t = 0;
 *   zfindex t2 = t - 1;
 *   zfbool b2 = (t2 == zfindexMax()); // not ensured
 * @endcode
 * use explicit conversion is recommended
 */
#ifndef zfstlsize
    #define zfstlsize size_t
#endif

// ============================================================
/** @cond ZFPrivateDoc */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlLess {
public:
    inline zfbool operator () (const zfchar * const &k1, const zfchar * const &k2) const {
        return (zfscmp(k1, k2) < 0);
    }
};
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlHash {
public:
    zfstlsize operator () (const zfchar * const &v) const {
        return (zfstlsize)zfidentityCalcString(v);
    }
};
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlEqual {
public:
    inline zfbool operator () (const zfchar * const &k1, const zfchar * const &k2) const {
        return (zfscmp(k1, k2) == 0);
    }
};

zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlLess {
public:
    inline zfbool operator () (zfchar * const &k1, zfchar * const &k2) const {
        return (zfscmp(k1, k2) < 0);
    }
};
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlHash {
public:
    zfstlsize operator () (zfchar * const &v) const {
        return (zfstlsize)zfidentityCalcString(v);
    }
};
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlEqual {
public:
    inline zfbool operator () (zfchar * const &k1, zfchar * const &k2) const {
        return (zfscmp(k1, k2) == 0);
    }
};

template<typename T_POD>
zfclassNotPOD ZFLIB_ZFCore zfpod_zfstlLess {
public:
    inline zfbool operator () (T_POD const &k1, T_POD const &k2) const {
        return zfcmpPOD(k1, k2) < 0;
    }
};
template<typename T_POD>
zfclassNotPOD zfpod_zfstlHash {
public:
    zfstlsize operator () (T_POD const &v) const {
        return (zfstlsize)zfidentityCalc(v);
    }
};
template<typename T_POD>
zfclassNotPOD zfpod_zfstlEqual {
public:
    inline zfbool operator () (T_POD const &k1, T_POD const &k2) const {
        return zfcmpPOD(k1, k2) == 0;
    }
};
/** @endcond */

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T>
zfclassNotPOD zfstlallocator {
public:
    typedef T value_type;
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T & reference;
    typedef const T & const_reference;
    typedef zfstlsize size_type;
    typedef zfstlsize difference_type;

    template<typename U>
    struct rebind {
        typedef zfstlallocator<U> other;
    };

    zfstlallocator() {}
    template<typename U>
    zfstlallocator(const zfstlallocator<U> &) {}

    pointer allocate(size_type n, const void* hint = 0) {
        return (pointer)zfpoolMalloc(n * sizeof(T));
    }

    void deallocate(pointer p, size_type) {
        zfpoolFree(p);
    }

    void construct(pointer p, const T &val) {
        zfnewPlacement(p, T, val);
    }

    void destroy(pointer p) {
        zfdeletePlacement(p);
    }

    size_type max_size() const {
        return size_type(-1) / sizeof(T);
    }
};
template<typename T, typename U>
zfbool operator == (const zfstlallocator<T> &, const zfstlallocator<U> &) {
    return zftrue;
}
template<typename T, typename U>
zfbool operator != (const zfstlallocator<T> &, const zfstlallocator<U> &) {
    return zffalse;
}
/** @endcond */

#endif // #ifndef _ZFI_zfstl_define_h_

