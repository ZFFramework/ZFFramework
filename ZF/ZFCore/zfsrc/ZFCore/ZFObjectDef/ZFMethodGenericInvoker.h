/**
 * @file ZFMethodGenericInvoker.h
 * @brief generic invoker declare for #ZFMethod::methodGenericInvoker
 */

#ifndef _ZFI_ZFMethodGenericInvoker_h_
#define _ZFI_ZFMethodGenericInvoker_h_

#include "ZFTypeIdFwd.h"
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
typedef zfbool (*ZFMethodGenericInvoker)(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfany const &invokerObject
        , ZF_IN const ZFMethod *invokerMethod
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        );

/**
 * @brief util macro to expand params for #ZFMethodGenericInvoker
 */
#define ZFMETHOD_GENERIC_INVOKER_PARAMS \
    ZF_OUT zfauto &ret \
    , ZF_OUT_OPT zfstring *errorHint \
    , ZF_IN zfany const &invokerObject \
    , ZF_IN const ZFMethod *invokerMethod \
    , ZF_IN zfindex paramCount \
    , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]

extern ZFLIB_ZFCore ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef;
/**
 * @brief mark the object as default param
 */
#define ZFMethodGenericInvokerDefaultParam() ((ZFObject * const &)_ZFP_ZFMethodGenericInvokerDefaultParamRef)

extern ZFLIB_ZFCore zfauto _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;
/**
 * @brief holder of #ZFMethodGenericInvokerDefaultParam
 */
#define ZFMethodGenericInvokerDefaultParamHolder() ((zfauto const &)_ZFP_ZFMethodGenericInvokerDefaultParamHolderRef)

/**
 * @brief util method for #ZFMethodGenericInvoker to check whether all param type match
 *
 * NOTE: when impl by plain #ZFMethodGenericInvoker,
 * impl take full responsibility to check whether param types matches,
 * and must return false to indicate the param type mismatch,
 * and supply necessary error hint\n
 * to make things easier, you may use this method to check all params are desired type
 */
extern ZFLIB_ZFCore zfbool ZFMethodGenericInvokerParamsCheck(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_IN zfindex paramCountMin
        , ZF_IN_OPT const ZFClass *paramType0 = zfnull
        , ZF_IN_OPT const ZFClass *paramType1 = zfnull
        , ZF_IN_OPT const ZFClass *paramType2 = zfnull
        , ZF_IN_OPT const ZFClass *paramType3 = zfnull
        , ZF_IN_OPT const ZFClass *paramType4 = zfnull
        , ZF_IN_OPT const ZFClass *paramType5 = zfnull
        , ZF_IN_OPT const ZFClass *paramType6 = zfnull
        , ZF_IN_OPT const ZFClass *paramType7 = zfnull
        );
/**
 * @brief see #ZFMethodGenericInvokerParamsCheck
 */
extern ZFLIB_ZFCore zfbool ZFMethodGenericInvokerParamsCheckWithMethod(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        , ZF_IN const ZFMethod *method
        );

// ============================================================
template<typename T_Dummy, int n>
zfclassNotPOD _ZFP_MtdGICk { // check whether type registered
};
template<typename T_Dummy>
zfclassNotPOD _ZFP_MtdGICk<T_Dummy, 1> {
public:
    typedef int TypeMustRegisterByZFPROPERTY;
};
extern ZFLIB_ZFCore zfbool _ZFP_MtdGIParamCheck(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN zfbool zfvAccessAvailable
        , ZF_IN const ZFMethod *invokerMethod
        , ZF_IN zfindex paramIndex
        , ZF_IN const zfchar *paramType
        , ZF_IN ZFObject *param
        );
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(N, ParamType) \
    typedef ParamType _T##N; \
    typedef zftTraits<ParamType>::TrNoRef _TR##N; \
    typedef _ZFP_MtdGICk< \
            _TR##N, \
            ZFTypeId<_TR##N>::TypeIdRegistered \
        >::TypeMustRegisterByZFPROPERTY _Ck##N;
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    _ZFP_MtdGIParamCheck( \
        errorHint, \
        ZFTypeId<_TR##N>::Value<_T##N>::zfvAccessAvailable(param), \
        invokerMethod, \
        (_Ck##N)N, \
        ZFM_TOSTRING(ParamType), \
        param)
template<typename T_Type, typename T_Access>
zfclassNotPOD _ZFP_MtdGIPA { // Param Access
public:
    zfauto &obj;
public:
    explicit _ZFP_MtdGIPA(ZF_IN_OUT zfauto &obj) : obj(obj) {}
    T_Access a(ZF_IN_OPT ZFObject *pDef = zfnull) {
        if(this->obj == ZFMethodGenericInvokerDefaultParam()) {
            this->obj = pDef;
        }
        return ZFTypeId<T_Type>::template Value<T_Access>::zfvAccess(this->obj);
    }
public:
    ~_ZFP_MtdGIPA(void) {
        ZFTypeId<T_Type>::template Value<T_Access>::zfvAccessFinish(this->obj);
    }
};
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    _ZFP_MtdGIPA<_TR##N, _T##N>(param).a(DefaultExpandOrEmpty(pDef##N(invokerMethod, N)))
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(N, DefaultExpandOrEmpty, ParamType, DefaultValueFix) \
    DefaultExpandOrEmpty( \
        static zfauto pDef##N( \
                ZF_IN const ZFMethod *invokerMethod \
                , ZF_IN zfindex index \
                ) { \
            zftValue<zftTraits<ParamType>::TrNoRef> paramDefault; \
            zfauto ret; \
            ZFTypeId<zftTraits<ParamType>::TrNoRef>::ValueStore( \
                ret, \
                (paramDefault.zfv DefaultValueFix())); \
            ZFTypeIdWrapper::markConst(ret); \
            return ret; \
        } \
    )

// ============================================================
extern ZFLIB_ZFCore void _ZFP_MtdGIRetError(
        ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnValueInfo
        );

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGIA { // Access with raw invoker
public:
    typedef T_ReturnType (*Ivk)(
            ZF_IN const ZFMethod *invokerMethod
            , ZF_IN zfany const &invokerObject
            , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
            );
public:
    static zfbool A(
            ZF_IN Ivk invoke
            , ZF_IN const ZFMethod *invokerMethod
            , ZF_IN zfany const &invokerObject
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_OUT_OPT zfauto &ret
            , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
            ) {
        T_ReturnType retTmp = invoke(invokerMethod, invokerObject, paramList);
        zfCoreMutexLocker();
        typedef typename zftTraits<T_ReturnType>::TrNoRef T_ReturnTypeTmp;
        if(ZFTypeId<T_ReturnTypeTmp>::ValueStore(ret, retTmp)) {
            _ZFP_ZFTypeIdWrapperMarkConstCheck<T_ReturnType>::a(ret);
            return zftrue;
        }
        else {
            zfstring info;
            zftToString(info, retTmp);
            _ZFP_MtdGIRetError(errorHint,
                ZFTypeId<T_ReturnTypeTmp>::TypeId(),
                info);
            return zffalse;
        }
    }
};
template<>
zfclassNotPOD _ZFP_MtdGIA<void> {
public:
    typedef void (*Ivk)(
            ZF_IN const ZFMethod *invokerMethod
            , ZF_IN zfany const &invokerObject
            , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
            );
public:
    static zfbool A(
            ZF_IN Ivk invoke
            , ZF_IN const ZFMethod *invokerMethod
            , ZF_IN zfany const &invokerObject
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_OUT_OPT zfauto &ret
            , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
            ) {
        invoke(invokerMethod, invokerObject, paramList);
        return zftrue;
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
        static zfbool GI(ZFMETHOD_GENERIC_INVOKER_PARAMS) { \
            ParamExpandOrEmpty0( \
                if( \
                                           !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, paramList[0]) \
                    ParamExpandOrEmpty1(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, paramList[1])) \
                    ParamExpandOrEmpty2(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, paramList[2])) \
                    ParamExpandOrEmpty3(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, paramList[3])) \
                    ParamExpandOrEmpty4(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, paramList[4])) \
                    ParamExpandOrEmpty5(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, paramList[5])) \
                    ParamExpandOrEmpty6(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, paramList[6])) \
                    ParamExpandOrEmpty7(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, paramList[7])) \
                    ) { \
                    return zffalse; \
                } \
            ) \
            return _ZFP_MtdGIA<ReturnType>::A(I, invokerMethod, invokerObject, errorHint, ret, paramList); \
        } \
    private: \
        static ReturnType I( \
                ZF_IN const ZFMethod *invokerMethod \
                , ZF_IN zfany const &invokerObject \
                , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM] \
                ) { \
            return invokerMethod->_ZFP_execute<ReturnType \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                >(invokerObject \
                    ParamExpandOrEmpty0(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, paramList[0])) \
                    ParamExpandOrEmpty1(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, paramList[1])) \
                    ParamExpandOrEmpty2(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, paramList[2])) \
                    ParamExpandOrEmpty3(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, paramList[3])) \
                    ParamExpandOrEmpty4(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, paramList[4])) \
                    ParamExpandOrEmpty5(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, paramList[5])) \
                    ParamExpandOrEmpty6(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, paramList[6])) \
                    ParamExpandOrEmpty7(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, paramList[7])) \
                ); \
        } \
    public:
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(owner) \
    owner::GI
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(owner, DefaultExpandOrEmpty, N) \
    (NULL DefaultExpandOrEmpty(ZFM_EMPTY(), owner::pDef##N))

// ============================================================
extern ZFLIB_ZFCore zfbool _ZFP_ZFMethodGenericInvoke(ZFMETHOD_GENERIC_INVOKER_PARAMS);
extern ZFLIB_ZFCore void _ZFP_ZFMethodGenericInvokeError(
        ZF_IN const ZFMethod *method
        , ZF_IN ZFObject *obj
        , ZF_IN zfint pos
        , ZF_IN_OPT const zfchar *errorHint = zfnull
        );

#define _ZFP_ZFMethodGenericInvoke_REPEAT1(N) \
        typedef typename zftTraits<Type##N>::TrNoRef _Type##N; \
        if(!ZFTypeId<_Type##N>::ValueStore(_p[N], param##N)) { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, N); \
        } \
        _ZFP_ZFTypeIdWrapperMarkConstCheck<Type##N>::a(_p[N]);
#define _ZFP_ZFMethodGenericInvoke_REPEAT2(N) \
        _ZFP_MtdGII_P<Type##N, zftIsZFObject(typename zftTraits<Type##N>::TrType)>::p(param##N, _p[N]);

#define _ZFP_ZFMethodGenericInvoke_DECLARE(N) \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType _ZFP_MtdGII( \
            ZF_IN const ZFMethod *method \
            , ZF_IN zfany const &obj \
            ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA) \
            ) { \
        zfauto _p[ZFMETHOD_MAX_PARAM] = { \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
            ZFMethodGenericInvokerDefaultParamHolder(), \
        }; \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT1, ZFM_EMPTY, ZFM_EMPTY) \
        zfauto _ret; \
        zfstring errorHint; \
        if(!_ZFP_ZFMethodGenericInvoke(_ret, &errorHint, obj, method, N, _p)) { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, -1, errorHint); \
        } \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT2, ZFM_EMPTY, ZFM_EMPTY) \
        return _ZFP_MtdGII_R<T_ReturnType>::r(method, obj, _ret); \
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
    static T_ReturnType r(
            ZF_IN const ZFMethod *method
            , ZF_IN ZFObject *obj
            , ZF_IN_OUT zfauto &ret
            ) {
        typedef typename zftTraits<T_ReturnType>::TrNoRef _T_ReturnType;
        if(!ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccessAvailable(ret)) {
            _ZFP_ZFMethodGenericInvokeError(method, obj, -2);
        }
        return ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccess(ret);
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_R<void> {
public:
    static void r(
            ZF_IN const ZFMethod *method
            , ZF_IN ZFObject *obj
            , ZF_IN_OUT zfauto &ret
            ) {
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

