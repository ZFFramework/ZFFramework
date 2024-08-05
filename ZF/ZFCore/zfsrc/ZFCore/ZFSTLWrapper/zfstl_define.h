/**
 * @file zfstl_define.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_define_h_
#define _ZFI_zfstl_define_h_

#include "../ZFCoreDef.h"

/**
 * @brief namespace wrapper for std
 */
#ifndef zfstl
    #define zfstl std
#endif

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
/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (const zfchar * const &k1, const zfchar * const &k2) const {
        return (zfscmp(k1, k2) < 0);
    }
    /** @endcond */
};
/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (zfchar * const &k1, zfchar * const &k2) const {
        return (zfscmp(k1, k2) < 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (const zfstring &k1, const zfstring &k2) const {
        return (k1.compare(k2) < 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (ZFSigName const &k1, ZFSigName const &k2) const {
        return (k1.compare(k2) < 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
template<typename T_Pointer>
zfclassNotPOD ZFLIB_ZFCore zfpointer_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (T_Pointer const &k1, T_Pointer const &k2) const {
        return (const zfbyte *)k1 < (const zfbyte *)k2;
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
template<typename T_POD>
zfclassNotPOD ZFLIB_ZFCore zfpod_zfstlLess {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (T_POD const &k1, T_POD const &k2) const {
        return zfcmpPOD(k1, k2) < 0;
    }
    /** @endcond */
};

// ============================================================
/**
 * @brief key hasher by string value hash, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (const zfchar * const &v) const {
        return (zfstlsize)zfidentityCalcString(v);
    }
    /** @endcond */
};
/**
 * @brief key hasher by string value hash, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (zfchar * const &v) const {
        return (zfstlsize)zfidentityCalcString(v);
    }
    /** @endcond */
};

/**
 * @brief key hasher by string value hash, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (zfstring const &v) const {
        return (zfstlsize)zfidentityCalcString(v.cString());
    }
    /** @endcond */
};

/**
 * @brief key hasher by string value hash, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (ZFSigName const &v) const {
        return (zfstlsize)zfidentityCalcString(v.cString());
    }
    /** @endcond */
};

/**
 * @brief key hasher by pointer value hash, used in STL
 */
template<typename T_Pointer>
zfclassNotPOD zfpointer_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (T_Pointer const &v) const {
        return (zfstlsize)zfidentityCalcPointer((const void *)v);
    }
    /** @endcond */
};

/**
 * @brief key hasher by pointer value hash, used in STL
 */
template<typename T_POD>
zfclassNotPOD zfpod_zfstlHash {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (T_POD const &v) const {
        return (zfstlsize)zfidentityCalcPOD(v);
    }
    /** @endcond */
};

// ============================================================
/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (const zfchar * const &k1, const zfchar * const &k2) const {
        return (zfscmp(k1, k2) == 0);
    }
    /** @endcond */
};
/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (zfchar * const &k1, zfchar * const &k2) const {
        return (zfscmp(k1, k2) == 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (const zfstring &k1, const zfstring &k2) const {
        return (k1.compare(k2) == 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (const ZFSigName &k1, const ZFSigName &k2) const {
        return (k1.compare(k2) == 0);
    }
    /** @endcond */
};

/**
 * @brief key comparer by pointer value comparation, used in STL
 */
template<typename T_Pointer>
zfclassNotPOD zfpointer_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (T_Pointer const &k1, T_Pointer const &k2) const {
        return (k1 == k2);
    }
    /** @endcond */
};

/**
 * @brief key comparer by pointer value comparation, used in STL
 */
template<typename T_POD>
zfclassNotPOD zfpod_zfstlEqual {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (T_POD const &k1, T_POD const &k2) const {
        return zfcmpPOD(k1, k2) == 0;
    }
    /** @endcond */
};

#endif // #ifndef _ZFI_zfstl_define_h_

