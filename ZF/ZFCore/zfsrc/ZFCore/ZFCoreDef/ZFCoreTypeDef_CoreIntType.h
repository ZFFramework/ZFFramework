/**
 * @file ZFCoreTypeDef_CoreIntType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_
#define _ZFI_ZFCoreTypeDef_CoreIntType_h_

#include "ZFCoreTypeDef_CorePrimitiveType.h"
#include <cfloat>

/**
 * @brief whether we have stdint
 */
#ifndef ZF_ENV_STDINT
    #define ZF_ENV_STDINT 0
#endif

#if ZF_ENV_STDINT
    #include <cstdint>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @typedef zft_zfint8
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint8
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint16
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint16
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint32
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint32
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * */
#if ZF_ENV_STDINT
    typedef int8_t zft_zfint8;
    typedef uint8_t zft_zfuint8;

    typedef int16_t zft_zfint16;
    typedef uint16_t zft_zfuint16;

    typedef int32_t zft_zfint32;
    typedef uint32_t zft_zfuint32;

    typedef int64_t zft_zfint64;
    typedef uint64_t zft_zfuint64;
#else
    #ifndef zft_zfint8
        typedef signed char zft_zfint8;
    #endif
    #ifndef zft_zfuint8
        typedef unsigned char zft_zfuint8;
    #endif

    #ifndef zft_zfint16
        typedef signed short int zft_zfint16;
    #endif
    #ifndef zft_zfuint16
        typedef unsigned short int zft_zfuint16;
    #endif

    #ifndef zft_zfint32
        typedef signed int zft_zfint32;
    #endif
    #ifndef zft_zfuint32
        typedef unsigned int zft_zfuint32;
    #endif

    #ifndef zft_zfint64
        typedef signed long long int zft_zfint64;
    #endif
    #ifndef zft_zfuint64
        typedef unsigned long long int zft_zfuint64;
    #endif
#endif // #if ZF_ENV_STDINT

// ============================================================
/**
 * @brief see #ZFT_INT_STRONG
 */
#define ZFT_INT_WEAK(T, D) \
    typedef T D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D;

// ============================================================
/**
 * @brief declare a strong wrapper type for integer types, similar to boost's BOOST_STRONG_TYPEDEF
 */
#define ZFT_INT_STRONG(T, D) _ZFP_ZFT_INT_STRONG(T, D)
#define _ZFP_ZFT_INT_STRONG(T, D) \
    /** @cond ZFPrivateDoc */ \
    class _zft_##D { \
    protected: \
        typedef _zft_##D D; \
        T t; \
    public: \
        _zft_##D(void) : t() {} \
        _zft_##D(const D &t_) : t(t_.t) {} \
        _zft_##D(const T t_) : t(t_) {} \
        _zft_##D(const int t_) : t((T)t_) {} \
        template<typename T2> explicit _zft_##D(const T2 t_) : t((T)t_) {} \
        inline D &operator = (const D &t_) {t = t_.t; return *this;} \
        inline D &operator = (const int &t_) {t = (T)t_; return *this;} \
        template<typename T2> inline D &operator = (const T2 &t_) {t = t_; return *this;} \
        inline operator const T & (void) const {return t;} \
        inline operator T & (void) {return t;} \
        inline bool operator ! (void) const {return (t == 0);} \
        inline bool operator == (const D &t_) const {return (t == t_.t);} \
        inline bool operator != (const D &t_) const {return (t != t_.t);} \
        inline bool operator <  (const D &t_) const {return (t <  t_.t);} \
        inline bool operator <= (const D &t_) const {return (t <= t_.t);} \
        inline bool operator >  (const D &t_) const {return (t >  t_.t);} \
        inline bool operator >= (const D &t_) const {return (t >= t_.t);} \
        inline bool operator == (const int &t_) const {return (t == (T)t_);} \
        inline bool operator != (const int &t_) const {return (t != (T)t_);} \
        inline bool operator <  (const int &t_) const {return (t <  (T)t_);} \
        inline bool operator <= (const int &t_) const {return (t <= (T)t_);} \
        inline bool operator >  (const int &t_) const {return (t >  (T)t_);} \
        inline bool operator >= (const int &t_) const {return (t >= (T)t_);} \
        template<typename T2> inline bool operator == (const T2 &t_) const {return (t == t_);} \
        template<typename T2> inline bool operator != (const T2 &t_) const {return (t != t_);} \
        template<typename T2> inline bool operator <  (const T2 &t_) const {return (t <  t_);} \
        template<typename T2> inline bool operator <= (const T2 &t_) const {return (t <= t_);} \
        template<typename T2> inline bool operator >  (const T2 &t_) const {return (t >  t_);} \
        template<typename T2> inline bool operator >= (const T2 &t_) const {return (t >= t_);} \
        inline D & operator ++ (void) {t += 1; return *this;} \
        inline D operator ++ (int) {D t_(t); t += 1; return t_;} \
        inline D & operator -- (void) {t -= 1; return *this;} \
        inline D operator -- (int) {D t_(t); t -= 1; return t_;} \
    }; \
    /** @endcond */ \
    typedef _zft_##D D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D;

// ============================================================
/** @brief float epsilon */
#define zffloatEpsilon FLT_EPSILON
/** @brief double epsilon */
#define zfdoubleEpsilon DBL_EPSILON

/** @brief template version of #zffloatEpsilon */
template<typename T_zffloat>
class zffloatEpsilonT {
public:
    /** @brief template version of #zffloatEpsilon */
    static inline T_zffloat v(void) {return zffloatEpsilon;}
};

// ============================================================
/**
 * @brief declare a strong wrapper type for float types, similar to boost's BOOST_STRONG_TYPEDEF
 */
#define ZFT_FLOAT_STRONG(T, D, epsilon) _ZFP_ZFT_FLOAT_STRONG(T, D, epsilon)
#define _ZFP_ZFT_FLOAT_STRONG(T, D, epsilon) \
    /** @cond ZFPrivateDoc */ \
    class _zft_##D { \
    protected: \
        typedef _zft_##D D; \
        T t; \
    public: \
        _zft_##D(void) : t() {} \
        _zft_##D(const D &t_) : t(t_.t) {} \
        _zft_##D(const T &t_) : t(t_) {} \
        _zft_##D(const int &t_) : t((T)t_) {} \
        template<typename T2> explicit _zft_##D(const T2 &t_) : t((T)t_) {} \
        inline D &operator = (const D &t_) {t = t_.t; return *this;} \
        inline D &operator = (const int &t_) {t = (T)t_; return *this;} \
        template<typename T2> inline D &operator = (const T2 &t_) {t = t_; return *this;} \
        inline operator const T & (void) const {return t;} \
        inline operator T & (void) {return t;} \
        inline bool operator ! (void) const {return (t >= -epsilon && t <= epsilon);} \
        inline bool operator == (const D &t_) const {return (t >= t_.t - epsilon && t <= t_.t + epsilon);} \
        inline bool operator != (const D &t_) const {return (t < t_.t - epsilon || t > t_.t + epsilon);} \
        inline bool operator <  (const D &t_) const {return (t < t_.t - epsilon);} \
        inline bool operator <= (const D &t_) const {return (t <= t_.t + epsilon);} \
        inline bool operator >  (const D &t_) const {return (t > t_.t + epsilon);} \
        inline bool operator >= (const D &t_) const {return (t >= t_.t - epsilon);} \
        inline bool operator == (const int &t_) const {return (t >= t_ - epsilon && t <= t_ + epsilon);} \
        inline bool operator != (const int &t_) const {return (t < t_ - epsilon || t > t_ + epsilon);} \
        inline bool operator <  (const int &t_) const {return (t < t_ - epsilon);} \
        inline bool operator <= (const int &t_) const {return (t <= t_ + epsilon);} \
        inline bool operator >  (const int &t_) const {return (t > t_ + epsilon);} \
        inline bool operator >= (const int &t_) const {return (t >= t_ - epsilon);} \
        template<typename T2> inline bool operator == (const T2 &t_) const {return (t >= t_ - epsilon && t <= t_ + epsilon);} \
        template<typename T2> inline bool operator != (const T2 &t_) const {return (t < t_ - epsilon || t > t_ + epsilon);} \
        template<typename T2> inline bool operator <  (const T2 &t_) const {return (t < t_ - epsilon);} \
        template<typename T2> inline bool operator <= (const T2 &t_) const {return (t <= t_ + epsilon);} \
        template<typename T2> inline bool operator >  (const T2 &t_) const {return (t > t_ + epsilon);} \
        template<typename T2> inline bool operator >= (const T2 &t_) const {return (t >= t_ - epsilon);} \
        inline D & operator ++ (void) {t += 1; return *this;} \
        inline D operator ++ (int) {D t_(t); t += 1; return t_;} \
        inline D & operator -- (void) {t -= 1; return *this;} \
        inline D operator -- (int) {D t_(t); t -= 1; return t_;} \
    }; \
    /** @endcond */ \
    typedef _zft_##D D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    class zffloatEpsilonT<D> { \
    public: \
        static inline D v(void) {return epsilon;} \
    }; \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_

