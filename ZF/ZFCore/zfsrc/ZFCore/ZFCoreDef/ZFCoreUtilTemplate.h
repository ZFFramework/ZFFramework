/**
 * @file ZFCoreUtilTemplate.h
 * @brief template utils
 */

#ifndef _ZFI_ZFCoreUtilTemplate_h_
#define _ZFI_ZFCoreUtilTemplate_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zftValue
/**
 * @brief util wrapper class to hold a type
 */
template<typename T_Type>
zfclassLikePOD zftValue {
public:
    /** @brief the wrapped value */
    T_Type zfv;
public:
    /** @cond ZFPrivateDoc */
    zftValue(void) : zfv() {}
    zftValue(ZF_IN T_Type const &ref) : zfv(ref) {}
    zftValue(ZF_IN zftValue<T_Type> const &ref) : zfv(ref.zfv) {}
    template<typename T_Ref> zftValue(ZF_IN T_Ref const &ref) : zfv(ref) {}
    inline zftValue<T_Type> &operator = (ZF_IN T_Type const &ref) {zfv = ref; return *this;}
    inline zftValue<T_Type> &operator = (ZF_IN zftValue<T_Type> const &ref) {zfv = ref.zfv; return *this;}
    template<typename T_Ref> inline zftValue<T_Type> &operator = (ZF_IN T_Ref const &ref) {zfv = ref; return *this;}
    inline operator T_Type const & (void) const {return zfv;}
    inline operator T_Type & (void) {return zfv;}
    /** @endcond */
};

// ============================================================
/**
 * @brief util macro to expand type
 *
 * @code
 *   #define MY_MACRO(P0, P1) xxx
 *
 *   MY_MACRO(int, int) // OK
 *   MY_MACRO(zfstlmap<int, int>, int) // not work
 *   MY_MACRO(zftType(zfstlmap<int, int>), int) // OK
 * @endcode
 */
#define zftType(...) \
    _ZFP_zftType<void(__VA_ARGS__)>::Type
template<typename T>
zfclassNotPOD _ZFP_zftType;
template<typename T_Ret, typename T_Type>
zfclassNotPOD _ZFP_zftType<T_Ret (T_Type)> {
public:
    typedef T_Type Type;
};

// ============================================================
// zftTraits
/** @brief see #zftTraits::ModifierName */
typedef enum {
    zftTraitsModifier_N, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_R, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_P, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_PR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_PCR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CP, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CPR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CPCR, /**< @brief see #zftTraits::ModifierName */
} zftTraitsModifier;
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_N "N"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_R "R"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CR "CR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_P "P"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_PR "PR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_PCR "PCR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CP "CP"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CPR "CPR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CPCR "CPCR"
/**
 * @brief type traits similar to STL's traits
 *
 * @note this is a walkaround to reduce dependency of STL,
 *   no rvalue reference support
 */
template<typename T_Type>
zfclassNotPOD zftTraits {
public:
    enum {
        TrIsPtr = 0, /** @brief true if the type is pointer type */
        TrIsRef = 0, /** @brief true if the type is reference type */
        TrModifier = (int)zftTraitsModifier_N, /** @brief the modifier type */
    };
    typedef T_Type                  TrType;         /**< @brief normal type */
    typedef T_Type &                TrRef;          /**< @brief reference type */
    typedef T_Type const &          TrConstRef;     /**< @brief const reference type */
    typedef T_Type *                TrPtr;          /**< @brief pointer type */
    typedef const T_Type *          TrConstPtr;     /**< @brief const pointer type */
    typedef T_Type                  TrNoRef;        /**< @brief remove reference */
    /**
     * @brief type modifier name
     *
     *   -  'N' : Type
     *   -  'R' : Type &
     *   -  'CR' : Type const &
     *   -  'P' : Type *
     *   -  'PR' : Type *&
     *   -  'PCR' : Type * const &
     *   -  'CP' : const Type *
     *   -  'CPR' : const Type *&
     *   -  'CPCR' : const Type * const &
     */
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_N;}
};
/** @cond ZFPrivateDoc */
/*
 * here is a list
 *   Type
 *   Type &
 *   Type const &
 *   Type *
 *   Type *&
 *   Type * const &
 *   const Type *
 *   const Type *&
 *   const Type * const &
 *
 *   void
 *   void *
 *   const void *
 *   void *&
 *   void * const &
 *   const void *&
 *   const void * const &
 */
template<typename T_Type>
zfclassNotPOD zftTraits<T_Type &> {
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_R,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_R;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<T_Type const &> {
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CR;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<T_Type *> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_P,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_P;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<T_Type *&> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PR;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<T_Type * const &> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PCR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PCR;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<const T_Type *> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_CP,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CP;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<const T_Type *&> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPR;}
};
template<typename T_Type>
zfclassNotPOD zftTraits<const T_Type * const &> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPCR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPCR;}
};

template<>
zfclassNotPOD zftTraits<void> {
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_N,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void                    TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void                    TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_N;}
};
template<>
zfclassNotPOD zftTraits<const void> {
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_N,
    };
    typedef const void              TrType;
    typedef const void              TrRef;
    typedef const void              TrConstRef;
    typedef const void *            TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void              TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_N;}
};
template<>
zfclassNotPOD zftTraits<void *> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_P,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_P;}
};
template<>
zfclassNotPOD zftTraits<const void *> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_CP,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef const void * const &    TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CP;}
};
template<>
zfclassNotPOD zftTraits<void *&> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PR;}
};
template<>
zfclassNotPOD zftTraits<void * const &> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PCR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PCR;}
};
template<>
zfclassNotPOD zftTraits<const void *&> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPR;}
};
template<>
zfclassNotPOD zftTraits<const void * const &> {
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPCR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPCR;}
};
/** @endcond */

// ============================================================
/** @brief std::enable_if wrapper */
template<int cond = 0, typename T_Fix = void>
zfclassNotPOD zftEnableIf {
};
/** @brief std::enable_if wrapper */
template<>
zfclassNotPOD zftEnableIf<1> {
public:
    /** @brief std::enable_if wrapper */
    typedef void Value;
};

// ============================================================
/** @brief std::is_same wrapper */
template<typename T0, typename T1>
zfclassNotPOD zftIsSame {
public:
    enum {
        /** @brief std::is_same wrapper */
        Value = 0
    };
};
/** @brief std::is_same wrapper */
template<typename T>
zfclassNotPOD zftIsSame<T, T> {
public:
    enum {
        /** @brief std::is_same wrapper */
        Value = 1
    };
};

// ============================================================
/** @brief std::is_class wrapper */
template<typename T>
zfclassNotPOD zftIsClass {
private:
    template<typename U>
    static char _test(int U::*);
    template<typename U>
    static int _test(...);
public:
    enum {
        /** @brief std::is_class wrapper */
        Value = ((sizeof(_test<T>(0)) == sizeof(char)) ? 1 : 0)
    };
};

// ============================================================
/** @brief std::is_base_of wrapper */
template<typename TChild, typename TBase,
    bool _IsClass = (zftIsClass<TChild>::Value && zftIsClass<TBase>::Value)>
zfclassNotPOD zftIsTypeOf {
private:
    template<typename T_Dummy>
    static char _test(TChild const &, T_Dummy);
    static int _test(TBase const &, int);
    zfclassNotPOD _Conv {
    public:
        operator TChild const & (void);
        operator TBase const & (void) const;
    };
public:
    enum {
        /** @brief std::is_base_of wrapper */
        Value = ((sizeof(_test(_Conv(), 0)) == sizeof(char)) ? 1 : 0)
    };
};
/** @brief std::is_base_of wrapper */
template<typename TChild, typename TBase>
zfclassNotPOD zftIsTypeOf<TChild, TBase, false> {
public:
    enum {
        /** @brief std::is_base_of wrapper */
        Value = zftIsSame<TChild, TBase>::Value
    };
};
/** @brief std::is_base_of wrapper */
template<typename T>
zfclassNotPOD zftIsTypeOf<T, T, true> {
public:
    enum {
        /** @brief std::is_base_of wrapper */
        Value = 1
    };
};

// ============================================================
/** @brief std::is_pod wrapper */
template<typename T_Type>
zfclassNotPOD zftIsPOD {
public:
    enum {
        /** @brief std::is_pod wrapper */
        Value = !zftIsClass<T_Type>::Value,
    };
};
/** @brief explicitly declare as POD */
#define ZFCORE_POD_DECLARE(Type) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD zftIsPOD<Type> { \
    public: \
        enum { \
            Value = 1, \
        }; \
    }; \
    /** @endcond */
/** @brief explicitly declare as POD */
#define ZFCORE_POD_DECLARE_TEMPLATE(T_Type, Type) \
    /** @cond ZFPrivateDoc */ \
    template<T_Type> \
    zfclassNotPOD zftIsPOD<Type > { \
    public: \
        enum { \
            Value = 1, \
        }; \
    }; \
    /** @endcond */

ZFCORE_POD_DECLARE(zflong)
ZFCORE_POD_DECLARE(zfulong)
ZFCORE_POD_DECLARE(zfdouble)
ZFCORE_POD_DECLARE(zftimet)
ZFCORE_POD_DECLARE(zfflags)
ZFCORE_POD_DECLARE(zfidentity)
ZFCORE_POD_DECLARE(ZFIndexRange)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilTemplate_h_

