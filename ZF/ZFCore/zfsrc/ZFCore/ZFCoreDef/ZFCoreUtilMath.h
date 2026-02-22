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
    return ((n0 < 0) ? (T_Number)(0 - n0) : n0);
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
 * @brief return a random float in range [0, 1]
 */
extern ZFLIB_ZFCore zffloat zfmRandFloat(void);
/**
 * @brief max value for #zfmRand
 */
extern ZFLIB_ZFCore zfint zfmRandMax(void);
/**
 * @brief return a random integer in range [0, zfmRandMax]
 */
extern ZFLIB_ZFCore zfint zfmRand(void);
/**
 * @brief return a random number range [0, range]
 */
template<typename T_int>
T_int zfmRand(ZF_IN T_int const &range) {
    return (T_int)(range * zfmRandFloat());
}
/**
 * @brief return a random number range [start, end]
 */
template<typename T_int>
T_int zfmRand(
        ZF_IN T_int const &start
        , ZF_IN T_int const &end
        ) {
    return start + (T_int)((end - start) * zfmRandFloat());
}

// ============================================================
template<typename T_Element, typename T_Holder, typename T_Comparer>
zfindex _ZFP_zfmSort(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN T_Comparer const &comparer
        , ZF_IN zfbool ascending
        ) {
    ZFCompareResult cmpToken = (ascending ? ZFCompareGreater : ZFCompareSmaller);
    T_Element pivot = (T_Element)holder[left];
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
template<typename T_Element, typename T_Holder, typename T_Comparer>
zfbool zfmSort(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN T_Comparer const &comparer
        ) {
    if(left < right) {
        zfindex mid = _ZFP_zfmSort<T_Element>(holder, left, right, comparer, zftrue);
        if(mid == zfindexMax()) {
            return zffalse;
        }
        if(mid > 0) {
            if(!zfmSort<T_Element>(holder, left, mid - 1, comparer)) {
                return zffalse;
            }
        }
        if(!zfmSort<T_Element>(holder, mid + 1, right, comparer)) {
            return zffalse;
        }
    }
    return zftrue;
}
/**
 * @brief sort with custom comparer in range [left, right], holder must support operator []
 */
template<typename T_Element, typename T_Holder, typename T_Comparer>
zfbool zfmSortReversely(
        ZF_IN T_Holder &holder
        , ZF_IN zfindex left
        , ZF_IN zfindex right
        , ZF_IN T_Comparer const &comparer
        ) {
    if(left < right) {
        zfindex mid = _ZFP_zfmSort<T_Element>(holder, left, right, comparer, zffalse);
        if(mid == zfindexMax()) {
            return zffalse;
        }
        if(mid > 0) {
            if(!zfmSortReversely<T_Element>(holder, left, mid - 1, comparer)) {
                return zffalse;
            }
        }
        if(!zfmSortReversely<T_Element>(holder, mid + 1, right, comparer)) {
            return zffalse;
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilMath_h_

