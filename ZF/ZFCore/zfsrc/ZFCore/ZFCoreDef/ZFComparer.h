/**
 * @file ZFComparer.h
 * @brief common comparer for ZFFramework
 */

#ifndef _ZFI_ZFComparer_h_
#define _ZFI_ZFComparer_h_

#include "ZFCoreUtilTemplate.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
namespace _ZFP_ZFComparer {
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
#endif

    class No { bool b[2]; };
    template<typename T, typename Arg> No operator == (T, Arg);
    template<typename T, typename Arg> No operator < (T, Arg);

    bool _ck(...);
    No &_ck(const No &);

    template<typename T> class _remove_pointer {public: typedef T Type;};
    template<typename T> class _remove_pointer<T *> {public: typedef T Type;};

    template <class _Tp> struct _is_reference        {public: enum {Value = 0}; };
    template <class _Tp> struct _is_reference<_Tp&>  {public: enum {Value = 1}; };
    #if defined(__cplusplus) && (__cplusplus >= 201103L)
    template <class _Tp> struct _is_reference<_Tp&&> {public: enum {Value = 1}; };
    #endif

    template <class _Tp> struct _is_const            {public: enum {Value = 0}; };
    template <class _Tp> struct _is_const<_Tp const> {public: enum {Value = 1}; };

    template <class _Tp> struct _is_function {public: enum {Value = !(_is_reference<_Tp>::Value || _is_const<const _Tp>::Value) ? 1 : 0 };};

    template <typename T, typename Arg = T>
    class HasEqual {
    public:
        enum {
            Value = (sizeof(_ck(*(T *)(0) == *(Arg *)(0))) != sizeof(No)) ? 1 : 0,
        };
    };
    template <typename T, typename Arg = T, int isFunc =
        (_is_function<typename _remove_pointer<T>::Type>::Value
         || _is_function<typename _remove_pointer<Arg>::Type>::Value) ? 1 : 0
        >
    class HasSmaller {
    public:
        enum {
            Value = (sizeof(_ck(*(T *)(0) < *(Arg *)(0))) != sizeof(No)) ? 1 : 0,
        };
    };
    template <typename T, typename Arg>
    class HasSmaller<T, Arg, 1> {
    public:
        enum {
            Value = 0,
        };
    };
    template <typename T, int isFunc>
    class HasSmaller<T, zfnullT, isFunc> {
    public:
        enum {
            Value = 0,
        };
    };
    template <typename T, int isFunc>
    class HasSmaller<zfnullT, T, isFunc> {
    public:
        enum {
            Value = 0,
        };
    };
    template <int isFunc>
    class HasSmaller<zfnullT, zfnullT, isFunc> {
    public:
        enum {
            Value = zftIsSame<zfnullT, int>::Value,
        };
    };

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
}

// ============================================================
template<typename T_Comparable>
inline ZFCompareResult _ZFP_ZFComparerForPOD(
        ZF_IN T_Comparable const &v0
        , ZF_IN T_Comparable const &v1
        ) {
    zfint result = zfmemcmp(&v0, &v1, sizeof(T_Comparable));
    if(result < 0) {
        return ZFCompareSmaller;
    }
    else if(result > 0) {
        return ZFCompareGreater;
    }
    else {
        return ZFCompareEqual;
    }
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
template<typename T_Comparable0, typename T_Comparable1, int hasEqual, int hasSmaller>
zfclassNotPOD _ZFP_ZFComparerDefaultImpl {
public:
    static inline ZFCompareResult a(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        return ZFCompareUncomparable;
    }
};
template<typename T_Comparable0, typename T_Comparable1>
zfclassNotPOD _ZFP_ZFComparerDefaultImpl<T_Comparable0, T_Comparable1, 1, 1> {
public:
    static inline ZFCompareResult a(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        if(v0 == v1) {
            return ZFCompareEqual;
        }
        else if(v0 < v1) {
            return ZFCompareSmaller;
        }
        else if(v1 < v0) {
            return ZFCompareGreater;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
};
template<typename T_Comparable0, typename T_Comparable1>
zfclassNotPOD _ZFP_ZFComparerDefaultImpl<T_Comparable0, T_Comparable1, 1, 0> {
public:
    static inline ZFCompareResult a(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        if(v0 == v1) {
            return ZFCompareEqual;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
};
template<typename T_Comparable0, typename T_Comparable1>
zfclassNotPOD _ZFP_ZFComparerDefaultImpl<T_Comparable0, T_Comparable1, 0, 1> {
public:
    static inline ZFCompareResult a(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        if(v0 < v1) {
            return ZFCompareSmaller;
        }
        else if(v1 < v0) {
            return ZFCompareGreater;
        }
        else {
            return ZFCompareUncomparable;
        }
    }
};
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
     * by default, use `operator==` or `operator<` if available,
     * use #ZFCOMPARER_DEFAULT_DECLARE to implement custom compare logic
     */
    static inline ZFCompareResult comparer(
            ZF_IN T_Comparable0 const &v0
            , ZF_IN T_Comparable1 const &v1
            ) {
        return _ZFP_ZFComparerDefaultImpl<T_Comparable0, T_Comparable1
            , _ZFP_ZFComparer::HasEqual<T_Comparable0, T_Comparable1>::Value ? 1 : 0
            , _ZFP_ZFComparer::HasSmaller<T_Comparable0, T_Comparable1>::Value ? 1 : 0
            >::a(v0, v1);
    }
};
template<typename T_Comparable0, typename T_Comparable1>
inline ZFCompareResult _ZFP_ZFComparerDefault(
        ZF_IN T_Comparable0 const &v0
        , ZF_IN T_Comparable1 const &v1
        ) {
    return ZFComparerDefaultHolder<T_Comparable0, T_Comparable1>::comparer(v0, v1);
}
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
    return (v1 == zfnull) ? ZFCompareEqual : ZFCompareUncomparable;
}
inline ZFCompareResult _ZFP_ZFComparerDefault(
        ZF_IN zfnullT const &v0
        , ZF_IN zfnullT const &v1
        ) {
    return ZFComparerDefaultHolder<zfnullT, zfnullT>::comparer(v0, v1);
}
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
 *   ZFCOMPARER_DEFAULT_DECLARE_ALIAS(T_Comparable0, T_Comparable1, OtherComparr)
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

// ============================================================
/**
 * @brief util method to compare two POD type
 */
template<typename T_Element>
inline zfint zfcmpPOD(
        ZF_IN T_Element const &v0
        , ZF_IN T_Element const &v1
        ) {
    return zfmemcmp(&v0, &v1, sizeof(T_Element));
}

/**
 * @brief utile macro to declare a operator == comparer of POD type
 */
#define ZFCORE_POD_COMPARER_DECLARE(Type) \
    /** @cond ZFPrivateDoc */ \
    inline zfbool operator == ( \
            ZF_IN Type const &v0 \
            , ZF_IN Type const &v1 \
            ) { \
        return zfcmpPOD(v0, v1) == 0; \
    } \
    inline zfbool operator != ( \
            ZF_IN Type const &v0 \
            , ZF_IN Type const &v1 \
            ) { \
        return zfcmpPOD(v0, v1) != 0; \
    } \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFComparer_h_

