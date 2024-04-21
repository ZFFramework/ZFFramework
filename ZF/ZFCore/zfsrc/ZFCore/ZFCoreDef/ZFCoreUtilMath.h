/**
 * @file ZFCoreUtilMath.h
 * @brief math utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilMath_h_
#define _ZFI_ZFCoreUtilMath_h_

#include "ZFComparer.h"

ZF_NAMESPACE_GLOBAL_BEGIN
// ============================================================
/**
 * @brief well known MAX
 */
template<typename T_Number>
inline T_Number const &zfmMax(
        ZF_IN T_Number const &n0
        , ZF_IN T_Number const &n1
        ) {
    return ((n0 > n1) ? n0 : n1);
}
/**
 * @brief well known MIN
 */
template<typename T_Number>
inline T_Number const &zfmMin(
        ZF_IN T_Number const &n0
        , ZF_IN T_Number const &n1
        ) {
    return ((n0 < n1) ? n0 : n1);
}
/**
 * @brief well known ABS
 */
template<typename T_Number>
inline T_Number zfmAbs(ZF_IN T_Number const &n0) {
    return ((n0 < 0) ? (T_Number)(-n0) : n0);
}
/**
 * @brief return n if in range [nMin, nMax], return edge value otherwise
 *
 * return value in range [nMin, infinite) if nMax < mMin
 */
template<typename T_Number>
inline T_Number zfmApplyRange(
        ZF_IN T_Number const &n
        , ZF_IN T_Number const &nMin
        , ZF_IN T_Number const &nMax
        ) {
    return ((n <= nMin)
        ? nMin
        : ((n <= nMax)
           ? n
           : ((nMax >= nMin)
              ? nMax
              : n
              )
           )
        );
}
/**
 * @brief return value at progress in range [start, end], progress must in range [0, 1]
 */
template<typename T_Number>
inline T_Number zfmApplyProgress(
        ZF_IN T_Number const &start
        , ZF_IN T_Number const &end
        , ZF_IN zffloat progress
        ) {
    return (T_Number)(start + (end - start) * progress);
}

// ============================================================
/** @brief float epsilon */
#define zffloatEpsilon ((zffloat)1.192092896e-07)
/** @brief double epsilon */
#define zfdoubleEpsilon ((zfdouble)2.2204460492503131e-016)
/** @brief longdouble epsilon */
#define zflongdoubleEpsilon ((zflongdouble)2.2204460492503131e-016)

/** @brief template version of #zffloatEpsilon */
template<typename T_zffloat>
zfclassNotPOD zffloatEpsilonT {
public:
    /** @brief template version of #zffloatEpsilon */
    static inline T_zffloat v(void) {return zffloatEpsilon;}
};
/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD zffloatEpsilonT<zffloat> {
public:
    static inline zffloat v(void) {return zffloatEpsilon;}
};
template<>
zfclassNotPOD zffloatEpsilonT<zfdouble> {
public:
    static inline zfdouble v(void) {return zfdoubleEpsilon;}
};
template<>
zfclassNotPOD zffloatEpsilonT<zflongdouble> {
public:
    static inline zflongdouble v(void) {return zflongdoubleEpsilon;}
};
/** @endcond */

// round float
/** @brief util method to round up/down or round a float value */
template<typename T_zffloat>
inline T_zffloat zfmRoundUp(ZF_IN const T_zffloat &v) {
    return (T_zffloat)((v >= 0)
            ? (zfint)(v + 1 - zffloatEpsilonT<T_zffloat>::v())
            : (zfint)v
        );
}
/** @brief util method to round up/down or round a float value */
template<typename T_zffloat>
inline T_zffloat zfmRoundDown(ZF_IN const T_zffloat &v) {
    return (T_zffloat)((v >= 0)
            ? (zfint)v
            : (zfint)(v - 1 + zffloatEpsilonT<T_zffloat>::v())
        );
}
/** @brief util method to round up/down or round a float value */
template<typename T_zffloat>
inline T_zffloat zfmRound(ZF_IN const T_zffloat &v) {
    return (T_zffloat)(zfint)((v >= 0) ? (v + 0.5f) : (v - 0.5f));
}

// ============================================================
/**
 * @brief return a random number range [0, max),
 *   max depends on implementation and is ensured at least 32767
 */
extern ZFLIB_ZFCore zfuint zfmRand(void);
/**
 * @brief return a random number range [0, range) or 0 if range is 0 or negative
 */
template<typename T_int>
T_int zfmRand(ZF_IN T_int range) {
    return ((range <= 0) ? 0 : ((T_int)(zfmRand()) % range));
}
/**
 * @brief return a random number range [start, end) or 0 if range invalid
 */
template<typename T_int>
T_int zfmRand(
        ZF_IN T_int start
        , ZF_IN T_int end
        ) {
    return ((start < end) ? (start + zfmRand(end - start)) : 0);
}

// ============================================================
template<typename T_Element, typename T_Holder>
zfindex _ZFP_zfmSort(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN typename ZFComparer<T_Element>::Comparer comparer
        , ZF_IN zfbool ascending
        ) {
    ZFCompareResult cmpToken = (ascending ? ZFCompareGreater : ZFCompareSmaller);
    T_Element pivot = holder[left];
    ZFCompareResult cmpTmp = ZFCompareUncomparable;
    while(left < right) {
        while(left < right) {
            cmpTmp = comparer(pivot, holder[right]);
            if(cmpTmp == ZFCompareUncomparable) {
                holder[left] = pivot;
                return zfindexMax();
            }
            else if(cmpTmp == cmpToken) {
                break;
            }
            --right;
        }
        holder[left] = holder[right];
        while(left < right) {
            cmpTmp = comparer(holder[left], pivot);
            if(cmpTmp == ZFCompareUncomparable) {
                holder[right] = pivot;
                return zfindexMax();
            }
            else if(cmpTmp == cmpToken) {
                break;
            }
            ++left;
        }
        holder[right] = holder[left];
    }
    holder[left] = pivot;
    return left;
}
/**
 * @brief sort with custom comparer in range [left, right], holder must support operator []
 */
template<typename T_Element, typename T_Holder>
void zfmSort(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
        ) {
    if(left < right) {
        zfindex mid = _ZFP_zfmSort<T_Element>(holder, left, right, comparer, zftrue);
        if(mid > 0) {
            zfmSort<T_Element>(holder, left, mid - 1, comparer);
        }
        zfmSort<T_Element>(holder, mid + 1, right, comparer);
    }
}
/**
 * @brief sort with custom comparer in range [left, right], holder must support operator []
 */
template<typename T_Element, typename T_Holder>
void zfmSortReversely(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault
        ) {
    if(left < right) {
        zfindex mid = _ZFP_zfmSort<T_Element>(holder, left, right, comparer, zffalse);
        if(mid > 0) {
            zfmSortReversely<T_Element>(holder, left, mid - 1, comparer);
        }
        zfmSortReversely<T_Element>(holder, mid + 1, right, comparer);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilMath_h_

