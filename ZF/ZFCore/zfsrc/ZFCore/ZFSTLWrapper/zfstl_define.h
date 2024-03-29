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
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlComparer {
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
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlComparer {
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
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlComparer {
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
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlComparer {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (ZFSigName const &k1, ZFSigName const &k2) const {
        return (k1.compare(k2) < 0);
    }
    /** @endcond */
};

// ============================================================
/**
 * @brief key hasher by string value hash, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlHasher {
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
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlHasher {
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
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlHasher {
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
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlHasher {
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
zfclassNotPOD zfpointer_zfstlHasher {
public:
    /** @cond ZFPrivateDoc */
    zfstlsize operator () (T_Pointer const &v) const {
        return (zfstlsize)zfidentityCalcPointer((const void *)v);
    }
    /** @endcond */
};

// ============================================================
/**
 * @brief key comparer by string comparation, used in STL
 */
zfclassNotPOD ZFLIB_ZFCore zfcharConst_zfstlHashComparer {
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
zfclassNotPOD ZFLIB_ZFCore zfchar_zfstlHashComparer {
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
zfclassNotPOD ZFLIB_ZFCore zfstring_zfstlHashComparer {
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
zfclassNotPOD ZFLIB_ZFCore ZFSigName_zfstlHashComparer {
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
zfclassNotPOD zfpointer_zfstlHashComparer {
public:
    /** @cond ZFPrivateDoc */
    inline zfbool operator () (T_Pointer const &k1, T_Pointer const &k2) const {
        return (k1 == k2);
    }
    /** @endcond */
};

#endif // #ifndef _ZFI_zfstl_define_h_

