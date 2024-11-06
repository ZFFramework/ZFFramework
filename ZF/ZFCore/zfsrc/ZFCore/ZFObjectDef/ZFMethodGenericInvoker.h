/**
 * @file ZFMethodGenericInvoker.h
 * @brief generic invoker declare for #ZFMethod::methodGenericInvoker
 */

#ifndef _ZFI_ZFMethodGenericInvoker_h_
#define _ZFI_ZFMethodGenericInvoker_h_

#include "ZFTypeIdFwd.h"
#include "ZFArgs.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief max param supported by ZFMethod
 */
#define ZFMETHOD_MAX_PARAM 8

// ============================================================
zfclassFwd ZFMethod;
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief generic invoker for advanced reflection, see #ZFMethod::methodGenericInvoker
 */
typedef void (*ZFMethodGenericInvoker)(ZF_IN_OUT const ZFArgs &zfargs);

extern ZFLIB_ZFCore ZFObject *_ZFP_ZFMP_DEF;
/**
 * @brief mark the object as default param
 */
#define ZFMP_DEF() ((ZFObject * const &)_ZFP_ZFMP_DEF)

/**
 * @brief util method for #ZFMethodGenericInvoker to check whether all param type match
 *
 * NOTE: when impl by plain #ZFMethodGenericInvoker,
 * impl take full responsibility to check whether param types matches,
 * and must return false to indicate the param type mismatch,
 * and supply necessary error hint\n
 * to make things easier, you may use this method to check all params are desired type
 */
extern ZFLIB_ZFCore zfbool ZFMethodGenericInvokerParamsCheck(ZF_IN_OUT const ZFArgs &zfargs);

// ============================================================
template<typename T_Dummy, int n>
zfclassNotPOD _ZFP_MtdGICk { // check whether type registered
};
template<typename T_Dummy>
zfclassNotPOD _ZFP_MtdGICk<T_Dummy, 1> {
public:
    typedef zfindex TypeNotRegisteredBy_ZFTYPEID;
};
extern ZFLIB_ZFCore zfbool _ZFP_MtdGIParamCheck(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN zfbool zfvAccessAvailable
        , ZF_IN zfindex paramIndex
        , ZF_IN const zfchar *paramType
        );
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(N, ParamType) \
    typedef ParamType _T##N; \
    typedef zftTraits<ParamType>::TrNoRef _TR##N; \
    typedef _ZFP_MtdGICk< \
            _TR##N, \
            ZFTypeId<_TR##N>::TypeIdRegistered \
        >::TypeNotRegisteredBy_ZFTYPEID _Ck##N;
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(N, DefaultExpandOrEmpty, ParamType) \
    _ZFP_MtdGIParamCheck( \
            zfargs \
            , ZFTypeId<_TR##N>::Value<_T##N>::zfvAccessAvailable(zfargs.paramAt(N)) \
            , (_Ck##N)N \
            , #ParamType \
            )
template<typename T_Type, typename T_Access>
zfclassNotPOD _ZFP_MtdGIPA { // Param Access
public:
    zfauto &obj;
public:
    explicit _ZFP_MtdGIPA(ZF_IN_OUT zfauto &obj) : obj(obj) {}
    T_Access a(ZF_IN_OPT ZFObject *pDef = zfnull) {
        if(this->obj == ZFMP_DEF()) {
            this->obj = pDef;
        }
        return ZFTypeId<T_Type>::template Value<T_Access>::zfvAccess(this->obj);
    }
public:
    ~_ZFP_MtdGIPA(void) {
        ZFTypeId<T_Type>::template Value<T_Access>::zfvAccessFinish(this->obj);
    }
};
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(N, DefaultExpandOrEmpty, ParamType) \
    _ZFP_MtdGIPA<_TR##N, _T##N>(zfargs.paramAt(N)).a(DefaultExpandOrEmpty(pDef##N()))
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(N, DefaultExpandOrEmpty, ParamType, DefaultValueFix) \
    DefaultExpandOrEmpty( \
        static zfauto pDef##N(void) { \
            zftValue<zftTraits<ParamType>::TrNoRef> paramDefault; \
            zfauto ret; \
            ZFTypeId<zftTraits<ParamType>::TrNoRef>::ValueStore( \
                ret, \
                (paramDefault.zfv DefaultValueFix())); \
            return ret; \
        } \
        static void pDefGI##N(ZF_IN const ZFArgs &zfargs) { \
            zfargs.result(pDef##N()); \
        } \
    )

// ============================================================
template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGIA { // Access with raw invoker
public:
    typedef T_ReturnType (*Ivk)(ZF_IN_OUT const ZFArgs &zfargs);
public:
    static void A(
            ZF_IN Ivk invoke
            , ZF_IN_OUT const ZFArgs &zfargs
            ) {
        T_ReturnType retTmp = invoke(zfargs);
        ZFCoreMutexLocker();
        typedef typename zftTraits<T_ReturnType>::TrNoRef T_ReturnTypeTmp;
        zfauto ret;
        if(ZFTypeId<T_ReturnTypeTmp>::ValueStore(ret, retTmp)) {
            zfargs.result(ret);
        }
        else {
            zfargs.success(zffalse);
            if(!zfargs.ignoreError()) {
                zfargs.errorHint(zfstr("unable to convert return value as type \"%s\""
                            , ZFTypeId<T_ReturnTypeTmp>::TypeId()
                            ));
            }
        }
    }
};
template<>
zfclassNotPOD _ZFP_MtdGIA<void> {
public:
    typedef void (*Ivk)(ZF_IN_OUT const ZFArgs &zfargs);
public:
    static void A(
            ZF_IN Ivk invoke
            , ZF_IN_OUT const ZFArgs &zfargs
            ) {
        invoke(zfargs);
    }
};

// ============================================================
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
        ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0_, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1_, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2_, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3_, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4_, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5_, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6_, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7_, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    private: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(0, ParamType0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(1, ParamType1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(2, ParamType2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(3, ParamType3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(4, ParamType4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(5, ParamType5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(6, ParamType6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(7, ParamType7)) \
    public: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(0, DefaultExpandOrEmpty0, ParamType0, DefaultValueFix0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(1, DefaultExpandOrEmpty1, ParamType1, DefaultValueFix1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(2, DefaultExpandOrEmpty2, ParamType2, DefaultValueFix2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(3, DefaultExpandOrEmpty3, ParamType3, DefaultValueFix3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(4, DefaultExpandOrEmpty4, ParamType4, DefaultValueFix4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(5, DefaultExpandOrEmpty5, ParamType5, DefaultValueFix5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(6, DefaultExpandOrEmpty6, ParamType6, DefaultValueFix6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(7, DefaultExpandOrEmpty7, ParamType7, DefaultValueFix7)) \
    public: \
        static void GI(ZF_IN_OUT const ZFArgs &zfargs) { \
            ParamExpandOrEmpty0( \
                if( \
                                           !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(0, DefaultExpandOrEmpty0, ParamType0) \
                    ParamExpandOrEmpty1(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(1, DefaultExpandOrEmpty1, ParamType1)) \
                    ParamExpandOrEmpty2(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(2, DefaultExpandOrEmpty2, ParamType2)) \
                    ParamExpandOrEmpty3(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(3, DefaultExpandOrEmpty3, ParamType3)) \
                    ParamExpandOrEmpty4(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(4, DefaultExpandOrEmpty4, ParamType4)) \
                    ParamExpandOrEmpty5(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(5, DefaultExpandOrEmpty5, ParamType5)) \
                    ParamExpandOrEmpty6(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(6, DefaultExpandOrEmpty6, ParamType6)) \
                    ParamExpandOrEmpty7(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(7, DefaultExpandOrEmpty7, ParamType7)) \
                    ) { \
                    return; \
                } \
            ) \
            _ZFP_MtdGIA<ReturnType>::A(I, zfargs); \
        } \
    private: \
        static ReturnType I(ZF_IN_OUT const ZFArgs &zfargs) { \
            return zfargs.ownerMethod()->_ZFP_execute<ReturnType \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                >(zfargs.sender() \
                    ParamExpandOrEmpty0(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(0, DefaultExpandOrEmpty0, ParamType0)) \
                    ParamExpandOrEmpty1(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(1, DefaultExpandOrEmpty1, ParamType1)) \
                    ParamExpandOrEmpty2(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(2, DefaultExpandOrEmpty2, ParamType2)) \
                    ParamExpandOrEmpty3(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(3, DefaultExpandOrEmpty3, ParamType3)) \
                    ParamExpandOrEmpty4(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(4, DefaultExpandOrEmpty4, ParamType4)) \
                    ParamExpandOrEmpty5(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(5, DefaultExpandOrEmpty5, ParamType5)) \
                    ParamExpandOrEmpty6(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(6, DefaultExpandOrEmpty6, ParamType6)) \
                    ParamExpandOrEmpty7(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(7, DefaultExpandOrEmpty7, ParamType7)) \
                ); \
        } \
    public:
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(owner) \
    owner::GI
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(owner, DefaultExpandOrEmpty, N) \
    ZFListener(DefaultExpandOrEmpty(ZFCallbackForFunc(owner::pDefGI##N)))

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFMethodGenericInvoke(ZF_IN_OUT const ZFArgs &zfargs);
extern ZFLIB_ZFCore void _ZFP_ZFMethodGenericInvokeError(
        ZF_IN zfint pos
        , ZF_IN const ZFArgs &zfargs
        );

#define _ZFP_ZFMethodGenericInvoke_REPEAT1(N) \
        { \
            zfauto tmp; \
            typedef typename zftTraits<Type##N>::TrNoRef _Type##N; \
            if(!ZFTypeId<_Type##N>::ValueStore(tmp, param##N)) { \
                _ZFP_ZFMethodGenericInvokeError(N, zfargs); \
            } \
            else { \
                zfargs.param(N, tmp); \
            } \
        }
#define _ZFP_ZFMethodGenericInvoke_REPEAT2(N) \
        _ZFP_MtdGII_P<Type##N, zftIsZFObject(typename zftTraits<Type##N>::TrType)>::p(param##N, zfargs.paramAt(N));

#define _ZFP_ZFMethodGenericInvoke_DECLARE(N) \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType _ZFP_MtdGII( \
            ZF_IN const ZFMethod *method \
            , ZF_IN zfany const &obj \
            ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA) \
            ) { \
        ZFArgs zfargs; \
        zfargs \
            .sender(obj) \
            .ownerMethod(method) \
            .paramInit() \
            ; \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT1, ZFM_EMPTY, ZFM_EMPTY) \
        _ZFP_ZFMethodGenericInvoke(zfargs); \
        if(!zfargs.success()) { \
            _ZFP_ZFMethodGenericInvokeError(-1, zfargs); \
        } \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT2, ZFM_EMPTY, ZFM_EMPTY) \
        return _ZFP_MtdGII_R<T_ReturnType>::r(zfargs); \
    }

template<typename T_ParamType, int isZFObject>
zfclassNotPOD _ZFP_MtdGII_P; // Param access
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType, 0> {
public:
    static inline void p(
            ZF_IN_OUT T_ParamType p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType, 1> {
public:
    static inline void p(
            ZF_IN_OUT T_ParamType p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType const &, 0> {
public:
    static inline void p(
            ZF_IN_OUT T_ParamType const &p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<const T_ParamType *, 0> {
public:
    static inline void p(
            ZF_IN_OUT const T_ParamType *p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_P<const void *, 0> {
public:
    static inline void p(
            ZF_IN_OUT const void *p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_P<void *, 0> {
public:
    static inline void p(
            ZF_IN_OUT void *p
            , ZF_IN_OUT zfauto &h
            ) {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType &, 0> {
public:
    static void p(
            ZF_IN_OUT T_ParamType &p
            , ZF_IN_OUT zfauto &h
            ) {
        typedef typename zftTraits<T_ParamType &>::TrNoRef _Type;
        p = ZFTypeId<_Type>::template Value<T_ParamType &>::zfvAccess(h);
        ZFTypeId<_Type>::template Value<T_ParamType &>::zfvAccessFinish(h);
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType *, 0> {
public:
    static void p(
            ZF_IN_OUT T_ParamType *p
            , ZF_IN_OUT zfauto &h
            ) {
        typedef typename zftTraits<T_ParamType *>::TrNoRef _Type;
        if(p) {
            *p = ZFTypeId<_Type>::template Value<T_ParamType const &>::zfvAccess(h);
            ZFTypeId<_Type>::template Value<T_ParamType const &>::zfvAccessFinish(h);
        }
    }
};

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGII_R { // Return access
public:
    static T_ReturnType r(ZF_IN const ZFArgs &zfargs) {
        typedef typename zftTraits<T_ReturnType>::TrNoRef _T_ReturnType;
        if(!ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccessAvailable(zfargs.result())) {
            _ZFP_ZFMethodGenericInvokeError(-2, zfargs);
        }
        return ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccess(zfargs.result());
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_R<void> {
public:
    static void r(ZF_IN const ZFArgs &zfargs) {
    }
};

_ZFP_ZFMethodGenericInvoke_DECLARE(0)
_ZFP_ZFMethodGenericInvoke_DECLARE(1)
_ZFP_ZFMethodGenericInvoke_DECLARE(2)
_ZFP_ZFMethodGenericInvoke_DECLARE(3)
_ZFP_ZFMethodGenericInvoke_DECLARE(4)
_ZFP_ZFMethodGenericInvoke_DECLARE(5)
_ZFP_ZFMethodGenericInvoke_DECLARE(6)
_ZFP_ZFMethodGenericInvoke_DECLARE(7)
_ZFP_ZFMethodGenericInvoke_DECLARE(8)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodGenericInvoker_h_

