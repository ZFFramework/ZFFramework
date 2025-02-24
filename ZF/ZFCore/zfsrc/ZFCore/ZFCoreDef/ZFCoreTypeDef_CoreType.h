/**
 * @file ZFCoreTypeDef_CoreType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreType_h_
#define _ZFI_ZFCoreTypeDef_CoreType_h_

#include "ZFCoreTypeDef_CoreIntType.h"
#include <cstdarg>
#include <cstdio>
#include <new>

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * here's a list of all basic type (ensured differs from each other)
 * * zfbool
 * * zfindex
 * * zfint
 * * zfuint
 * * zflong
 * * zfulong
 * * zffloat
 * * zfdouble
 * * zfbyte
 * * zftimet
 * * zfflags
 * * zfidentity
 */

// ============================================================
/** @cond ZFPrivateDoc */
class ZFLIB_ZFCore zft_zfnullT {
public:
    inline operator void *(void) const {
        return NULL;
    }
    inline operator const void *(void) const {
        return NULL;
    }
    template<typename T>
    inline operator T *(void) const {
        return NULL;
    }
    template<class C, typename T>
    inline operator T C::*(void) const {
        return NULL;
    }
private:
    void operator &(void) const;
};
extern ZFLIB_ZFCore const zft_zfnullT zft_zfnull;
/** @endcond */

/**
 * @def zfnullT
 * @brief type for zfnull, can be used for function overload
 * @def zfnull
 * @brief same as NULL, defined for future use
 */
#if 1 && defined(__cplusplus) && (__cplusplus >= 201103L) // c++11
    #ifndef zfnullT
        #define zfnullT nullptr_t
    #endif
    #ifndef zfnull
        #define zfnull nullptr
    #endif
#elif 1
    #ifndef zfnullT
        #define zfnullT zft_zfnullT
    #endif
    #ifndef zfnull
        #define zfnull zft_zfnull
    #endif
#else // backward capacity
    #ifndef zfnullT
        #define zfnullT int
    #endif
    #ifndef zfnull
        #define zfnull 0
    #endif
#endif

// ============================================================
/**
 * @brief bool type
 */
ZFT_INT_WEAK(_ZFT_t_zfbool, zfbool)
/**
 * @brief bool true type
 */
#define zftrue _ZFT_t_zftrue
/**
 * @brief bool false type
 */
#define zffalse _ZFT_t_zffalse

// ============================================================
/**
 * @brief similar to size_t, used for index and size only
 *
 * we use these number types in ZFFramework:
 * -  zfint, zfuint: normal integer types, size depends on environment
 * -  zflong, zfulong: long integer types, at least 64 bit
 * -  zfindex: integer types used for index and size
 * -  zffloat/zfdouble: most widely used float type in ZFFramework, size depends on environment
 * -  zfbyte: for byte types
 * -  zftimet: for time types
 * -  zfflags: for flag types
 * -  zfidentity: for identity types
 *
 *
 * @warning take good care of that, zfindex is not ensured same as size_t,
 *   so it is not ensured that zfindexMax() is equal to string::npos,
 *   e.g.:
 *   @code
 *     zfindex n = std::string::npos;
 *     zfbool b0 = (n == std::string::npos); // true
 *     zfbool b1 = (zfindexMax() == std::string::npos); // not ensured
 *   @endcode
 * @note all integer types are ensured differs from each other (by #ZFT_INT_STRONG if necessary),
 *   so that it's convenient to achieve template specialization\n
 *   however, as the cost, some math operators may have strange behaviors,
 *   use static_cast if necessary\n
 *   we ensure these types are primitive:
 *   -  zfbool
 *   -  zfchar
 *   -  zfbyte
 *   -  zfint
 *   -  zfuint
 *   -  zfindex
 *   -  zffloat
 *
 *   and only primitive types are safe to be passed to va_arg\n
 *   \n
 *   for types wrapped by #ZFT_INT_STRONG,
 *   you may use zft_XXX to access original primitive type
 */
ZFT_INT_WEAK(_ZFT_t_zfindex, zfindex)
/**
 * @brief (zfindex)-1, indicate a max index value, see #zfindex
 * @see zfuint
 */
#define zfindexMax() ((zfindex)-1)

// ============================================================
/**
 * @brief same as int, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_t_zfint, zfint)
/**
 * @brief same as unsigned int, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_t_zfuint, zfuint)
/**
 * @brief long, see #zfindex
 */
ZFT_INT_STRONG(zft_zfint64, zflong)
/**
 * @brief unsigned long, see #zfindex
 */
ZFT_INT_STRONG(zft_zfuint64, zfulong)

// ============================================================
/**
 * @brief same as float, see #zfindex
 */
ZFT_FLOAT_STRONG(_ZFT_t_zffloat, zffloat, zffloatEpsilon)

/**
 * @brief same as double, see #zfindex
 */
ZFT_FLOAT_STRONG(double, zfdouble, zfdoubleEpsilon)

// ============================================================
/**
 * @brief 8-bit unsigned value, see #zfindex
 */
ZFT_INT_WEAK(_ZFT_t_zfbyte, zfbyte)

// ============================================================
/**
 * @brief time unit, ensured at least 64 bit, ensured signed
 *
 * time unit of zftimet is not required,
 * but usually use miliseconds in ZFFramework
 */
ZFT_INT_STRONG(zft_zfint64, zftimet)
/** @brief invalid time value */
#define zftimetInvalid() ((zftimet)-1)

// ============================================================
/**
 * @brief used to hold flags, ensured 32 bit, ensured unsigned
 */
ZFT_INT_STRONG(zft_zfuint32, zfflags)

// ============================================================
/**
 * @brief identity type, ensured at least 32 bit, ensured unsigned
 * @note typically, an identity type has these usage:
 *   -  use as task id, the actual value is not important,
 *     and should not be serialized
 *   -  use as event id, the actual value may change during app launch,
 *     while event name would be stable
 *
 *   because of the above reasons,
 *   #zfidentity would always be serialized by the name of #ZFIdMapNameForId
 */
ZFT_INT_STRONG(zft_zfuint32, zfidentity)
/**
 * @brief an invalid id value, ensured ((zfidentity)-1)
 */
#define zfidentityInvalid() ((zfidentity)-1)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreType_h_

