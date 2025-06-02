/**
 * @file ZFCoreTypeDef_OtherType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_OtherType_h_
#define _ZFI_ZFCoreTypeDef_OtherType_h_

#include "ZFCoreTypeDef_StringType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief string tokens */
#define ZFTOKEN_zfnull "null"

/** @brief string tokens */
#define ZFTOKEN_zfbool_zftrue "true"
/** @brief string tokens */
#define ZFTOKEN_zfbool_zffalse "false"

// ============================================================
/**
 * @brief compare result of two ZFObjects
 * @see ZFObject::compare
 */
typedef enum {
    ZFCompareUncomparable = -2, /**< can't be compared */
    ZFCompareSmaller = -1, /**< left < right */
    ZFCompareEqual = 0, /**< left == right */
    ZFCompareGreater = 1, /**< left > right */
} ZFCompareResult;
/** @brief string tokens */
#define ZFTOKEN_ZFCompareUncomparable "Uncomparable"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareSmaller "Smaller"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareEqual "Equal"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareGreater "Greater"

// ============================================================
/**
 * @brief seek position similar to SEEK_SET of FILE operation
 */
typedef enum {
    ZFSeekPosBegin,
    ZFSeekPosCur,
    ZFSeekPosCurReversely,
    ZFSeekPosEnd,
} ZFSeekPos;
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosBegin "SeekBegin"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosCur "SeekCur"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosCurReversely "SeekCurReversely"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosEnd "SeekEnd"

// ============================================================
/**
 * @brief POD data to hold index range
 */
zfclassPOD ZFLIB_ZFCore ZFIndexRange {
public:
    zfindex start; /**< @brief start */
    zfindex count; /**< @brief count */
};
extern ZFLIB_ZFCore const ZFIndexRange _ZFP_ZFIndexRangeZero;
extern ZFLIB_ZFCore const ZFIndexRange _ZFP_ZFIndexRangeMax;
/**
 * @brief zero index range
 */
#define ZFIndexRangeZero() _ZFP_ZFIndexRangeZero
/**
 * @brief max index range (0, zfindexMax())
 */
#define ZFIndexRangeMax() _ZFP_ZFIndexRangeMax
/**
 * @brief make a ZFIndexRange
 */
inline ZFIndexRange ZFIndexRangeCreate(
        ZF_IN zfindex start
        , ZF_IN zfindex count
        ) {
    ZFIndexRange ret = {start, count};
    return ret;
}
/**
 * @brief true if contain index in range
 */
inline zfbool ZFIndexRangeContain(
        ZF_IN const ZFIndexRange &range
        , ZF_IN zfindex index
        ) {
    return (range.count > 0 && index >= range.start && index < range.start + range.count);
}

// ============================================================
/**
 * @brief comparar for template types
 *
 * for common types (that support operation "==", "<", ">", such as pointers, built-in types)
 * you may use #ZFComparerDefault:
 * @code
 *   ZFComparerDefault<YourType>
 * @endcode
 * for custom types you may declare your own comparar:
 * @code
 *   ZFCompareResult YourCompareFunction(
 *           ZF_IN YourType const &e0
 *           , ZF_IN YourType const &e1
 *           ) {
 *       // do your compare
 *   }
 * @endcode
 * and pass the comparar as:
 * @code
 *   YourCompareFunction
 * @endcode
 */
template<typename T_Comparable, typename T_Comparable2 = T_Comparable>
zfclassNotPOD ZFComparer {
public:
    /**
     * @brief see #ZFComparer
     */
    typedef ZFCompareResult (*Comparer)(
            ZF_IN T_Comparable const &e0
            , ZF_IN T_Comparable2 const &e1
            );
};

// ============================================================
/**
 * @brief string if the content info not available, see #zftToStringT
 */
#define ZFTOKEN_zftToStringNotAvailable "NA"

/**
 * @brief proto type for obtain object info, see #zftToStringT
 */
template<typename T_Type>
zfclassNotPOD ZFCoreInfoGetter {
public:
    /**
     * @brief proto type for obtain object info, see #zftToStringT
     */
    typedef void (*InfoGetter)(
            ZF_IN_OUT zfstring &ret
            , ZF_IN T_Type const &v
            );
};

template<typename T_Type, typename T_Fix = void>
zfclassNotPOD _ZFP_zftToStringFallback {
public:
    static inline void a(
            ZF_IN_OUT zfstring &s
            , ZF_IN T_Type const &v
            ) {
        s += ZFTOKEN_zftToStringNotAvailable;
    }
};

/**
 * @brief util function to obtain object info
 *
 * for your custom type, you must use #ZFOUTPUT_TYPE to specialize your type
 * before accessing this method,
 * or supply your own specialization manually with this proto type:
 * @code
 *   void zftToStringT(
 *           ZF_IN_OUT zfstring &s
 *           , ZF_IN YourType const &v
 *           );
 * @endcode
 */
template<typename T_Type>
inline void zftToStringT(
        ZF_IN_OUT zfstring &s
        , ZF_IN T_Type const &v
        ) {
    _ZFP_zftToStringFallback<T_Type>::a(s, v);
}
/** @brief see #zftToStringT */
template<typename T_Type>
inline zfstring zftToString(ZF_IN T_Type const &v) {
    zfstring s;
    zftToStringT(s, v);
    return s;
}

/**
 * @brief declare your custom type conversion to string,
 *   convenient for debug
 *
 * proto type:
 * @code
 *   void zftToStringT(
 *           ZF_IN_OUT zfstring &s
 *           , ZF_IN YourType const &v
 *           );
 * @endcode
 * usage:
 * @code
 *   ZFOUTPUT_TYPE(YourType, {s += YourConverter(v);})
 * @endcode
 *
 * once declared, you may output your type to #ZFOutput by:
 * @code
 *   output << yourObject;
 * @endcode
 * or, use the declared method:
 * @code
 *   zftToStringT(s, v);
 * @endcode
 */
#define ZFOUTPUT_TYPE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    inline void zftToStringT( \
            ZF_IN_OUT zfstring &s \
            , T_Type const &v \
            ) { \
        outputAction \
    } \
    /** @endcond */
/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFOUTPUT_TYPE_DECLARE(ZFLIB_APP, YourType)
 *   // in source file
 *   ZFOUTPUT_TYPE_DEFINE(YourType, {doYourStuff();})
 *
 *   // or, the inline version
 *   ZFOUTPUT_TYPE(YourType, {doYourStuff();})
 * @endcode
 */
#define ZFOUTPUT_TYPE_DECLARE(ZFLIB_, T_Type) \
    /** @cond ZFPrivateDoc */ \
    extern ZFLIB_ void zftToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ); \
    /** @endcond */
/** @brief see #ZFOUTPUT_TYPE_DECLARE */
#define ZFOUTPUT_TYPE_DEFINE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    void zftToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ) { \
        outputAction \
    } \
    /** @endcond */
/** @brief see #ZFOUTPUT_TYPE */
#define ZFOUTPUT_TYPE_TEMPLATE(T_typenameList, T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    template<T_typenameList> \
    inline void zftToStringT( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ) { \
        outputAction \
    } \
    /** @endcond */

// ============================================================
/**
 * @brief dummy function address type
 */
typedef void (*ZFFuncAddrType)(void);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_OtherType_h_

