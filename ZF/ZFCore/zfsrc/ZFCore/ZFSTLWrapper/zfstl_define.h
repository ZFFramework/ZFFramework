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
        return (zfstlsize)zfidentityCalcPOD(v);
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

#endif // #ifndef _ZFI_zfstl_define_h_

