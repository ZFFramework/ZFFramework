/**
 * @file ZFComparer.h
 * @brief common comparer for ZFFramework
 */

#ifndef _ZFI_ZFComparer_h_
#define _ZFI_ZFComparer_h_

#include "ZFCoreUtilTemplate.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief comparer holder fo #ZFComparerDefault,
 *   you may specialize this class to supply your custom type's comparation,
 *   or use #ZFCOMPARER_DEFAULT_DECLARE for short
 */
template<typename T_Comparable0, typename T_Comparable1,
         typename TypeFix0 = void, typename TypeFix1 = void,
         typename T_ReservedFix0 = void, typename T_ReservedFix1 = void>
zfclassNotPOD ZFComparerDefaultHolder {
public:
    /**
     * @brief default comparer
     *
     * same as #ZFComparerCheckEqual by default,
     * use #ZFCOMPARER_DEFAULT_DECLARE to implement custom compare logic
     */
    static ZFCompareResult comparer(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        return (v0 == v1) ? ZFCompareEqual : ZFCompareUncomparable;
    }
};
template<typename T_Comparable0, typename T_Comparable1>
inline ZFCompareResult _ZFP_ZFComparerDefault(
        ZF_IN T_Comparable0 const &v0
        , ZF_IN T_Comparable1 const &v1
        ) {
    return ZFComparerDefaultHolder<T_Comparable0, T_Comparable1>::comparer(v0, v1);
}
#if 1 /* ZFTAG_TRICKS: tricks to solve NULL compare for cpp03 */
    template<typename T_Comparable>
    inline ZFCompareResult _ZFP_ZFComparerDefault(
            ZF_IN T_Comparable const &v0
            , ZF_IN zfnullT const &v1
            ) {
        return (v0 == zfnull) ? ZFCompareEqual : ZFCompareUncomparable;
    }
    template<typename T_Comparable>
    inline ZFCompareResult _ZFP_ZFComparerDefault(
            ZF_IN zfnullT const &v0
            , ZF_IN T_Comparable const &v1
            ) {
        return (zfnull == v1) ? ZFCompareEqual : ZFCompareUncomparable;
    }
    inline ZFCompareResult _ZFP_ZFComparerDefault(
            ZF_IN zfnullT const &v0
            , ZF_IN zfnullT const &v1
            ) {
        return ZFComparerDefaultHolder<zfnullT, zfnullT>::comparer(v0, v1);
    }
#endif
/**
 * @brief default comparer for common types, see #ZFComparer
 *
 * by default, compare by operator ==,
 * you may use #ZFCOMPARER_DEFAULT_DECLARE to supply your own type's compare method
 */
#define ZFComparerDefault _ZFP_ZFComparerDefault

/**
 * @brief declare a template specialization for #ZFComparerDefault
 *
 * usage:
 * @code
 *   ZFCOMPARER_DEFAULT_DECLARE(Type0, Type1, {
 *           return yourCompareMethod(v0, v1);
 *       })
 * @endcode
 * @note all subclass of the declared type would be specialized by this method,
 *   if you don't want it, use #ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT instead
 */
#define ZFCOMPARER_DEFAULT_DECLARE(T_Comparable0, T_Comparable1, compareAction) \
    /** @cond ZFPrivateDoc */ \
    template<typename T0, typename T1> \
    zfclassNotPOD ZFComparerDefaultHolder<T0, T1 \
        , typename zftEnableIf<zftIsTypeOf<T0, T_Comparable0>::Value>::Value \
        , typename zftEnableIf<zftIsTypeOf<T1, T_Comparable1>::Value>::Value \
        > \
    { \
    public: \
        static ZFCompareResult comparer( \
                ZF_IN T0 const &v0 \
                , ZF_IN T1 const &v1 \
                ) { \
            compareAction \
        } \
    }; \
    /** @endcond */

/**
 * @brief util macro to alias compare action see #ZFCOMPARER_DEFAULT_DECLARE
 *
 * usage:
 * @code
 *   ZFCOMPARER_DEFAULT_DECLARE_ALIAS(T_Comparable0, T_Comparable1, ZFComparerCheckEqual)
 *   ZFCOMPARER_DEFAULT_DECLARE_ALIAS(T_Comparable2, T_Comparable3, ZFComparerNumeric)
 * @endcode
 */
#define ZFCOMPARER_DEFAULT_DECLARE_ALIAS(T_Comparable0, T_Comparable1, compareAction) \
    ZFCOMPARER_DEFAULT_DECLARE(T_Comparable0, T_Comparable1, {return compareAction(v0, v1);})

/**
 * @brief see #ZFCOMPARER_DEFAULT_DECLARE
 */
#define ZFCOMPARER_DEFAULT_DECLARE_EXPLICIT(T_Comparable0, T_Comparable1, compareAction) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFComparerDefaultHolder<T_Comparable0, T_Comparable1> { \
    public: \
        static ZFCompareResult comparer( \
                ZF_IN T_Comparable0 const &v0 \
                , ZF_IN T_Comparable1 const &v1 \
                ) { \
            compareAction \
        } \
    }; \
    /** @endcond */

// ============================================================
template<typename T0, typename T1>
inline ZFCompareResult _ZFP_ZFComparerNumeric(
        ZF_IN T0 const &v0
        , ZF_IN T1 const &v1
        ) {
    if(v0 == v1) {
        return ZFCompareEqual;
    }
    else if(v0 < v1) {
        return ZFCompareSmaller;
    }
    else if(v0 > v1) {
        return ZFCompareGreater;
    }
    else {
        return ZFCompareUncomparable;
    }
}
/**
 * @brief numeric comparer compare by operator ==, < and >, see #ZFComparer
 */
#define ZFComparerNumeric _ZFP_ZFComparerNumeric

// ============================================================
template<typename T_Comparable0, typename T_Comparable1>
inline ZFCompareResult _ZFP_ZFComparerCheckEqual(
        ZF_IN T_Comparable0 const &v0
        , ZF_IN T_Comparable1 const &v1
        ) {
    return ((v0 == v1) ? ZFCompareEqual : ZFCompareUncomparable);
}
/**
 * @brief compare by operator == only, see #ZFComparer
 *
 * useful for some non-POD types that can only be compared with operator ==
 */
#define ZFComparerCheckEqual _ZFP_ZFComparerCheckEqual

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerForPOD(
        ZF_IN T_Comparable const &v0
        , ZF_IN T_Comparable const &v1
        ) {
    return ((zfmemcmp(&v0, &v1, sizeof(T_Comparable)) == 0) ? ZFCompareEqual : ZFCompareUncomparable);
}
/**
 * @brief default comparer for POD types, compare by memory, see #ZFComparer
 */
#define ZFComparerForPOD _ZFP_ZFComparerForPOD

template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerDummy(
        ZF_IN T_Comparable const &v0
        , ZF_IN T_Comparable const &v1
        ) {
    return ZFCompareUncomparable;
}
/**
 * @brief dummy comparer that always return ZFCompareUncomparable, see #ZFComparer
 */
#define ZFComparerDummy _ZFP_ZFComparerDummy

// ============================================================
ZFCOMPARER_DEFAULT_DECLARE(const zfchar *, const zfchar *, {
        const zfchar *v0Tmp = ((v0 == zfnull) ? "" : v0);
        const zfchar *v1Tmp = ((v1 == zfnull) ? "" : v1);
        zfint result = zfscmp(v0Tmp, v1Tmp);
        if(result < 0) {
            return ZFCompareSmaller;
        }
        else if(result > 0) {
            return ZFCompareGreater;
        }
        else {
            return ZFCompareEqual;
        }
    })
ZFCOMPARER_DEFAULT_DECLARE(zfstring, zfstring, {
        zfint result = v0.compare(v1);
        if(result < 0) {
            return ZFCompareSmaller;
        }
        else if(result > 0) {
            return ZFCompareGreater;
        }
        else {
            return ZFCompareEqual;
        }
    })
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfindex, zfindex, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfint, zfint, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfuint, zfuint, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zffloat, zffloat, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfdouble, zfdouble, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zflongdouble, zflongdouble, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zftimet, zftimet, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfflags, zfflags, ZFComparerNumeric)
ZFCOMPARER_DEFAULT_DECLARE_ALIAS(zfidentity, zfidentity, ZFComparerNumeric)

// ============================================================
/**
 * @brief util method to compare two POD type
 */
template<typename T_Element>
inline zfbool zfcmpPODTheSame(
        ZF_IN T_Element const &v0
        , ZF_IN T_Element const &v1
        ) {
    return (zfmemcmp(&v0, &v1, sizeof(T_Element)) == 0);
}

/**
 * @brief utile macro to declare a operator == comparer of POD type
 */
#define ZFCORE_POD_COMPARER_DECLARE(Type) \
    /** @brief compare by value */ \
    inline zfbool Type##IsEqual( \
            ZF_IN Type const &v0 \
            , ZF_IN Type const &v1 \
            ) { \
        return zfcmpPODTheSame(v0, v1); \
    } \
    /** @cond ZFPrivateDoc */ \
    inline zfbool operator == ( \
            ZF_IN Type const &v0 \
            , ZF_IN Type const &v1 \
            ) { \
        return zfcmpPODTheSame(v0, v1); \
    } \
    inline zfbool operator != ( \
            ZF_IN Type const &v0 \
            , ZF_IN Type const &v1 \
            ) { \
        return !zfcmpPODTheSame(v0, v1); \
    } \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFComparer_h_

