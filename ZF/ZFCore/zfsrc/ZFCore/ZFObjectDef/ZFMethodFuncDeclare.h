/**
 * @file ZFMethodFuncDeclare.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodFuncDeclare_h_
#define _ZFI_ZFMethodFuncDeclare_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFMethodFuncRegister(ZF_IN const ZFMethod *method);
extern ZFLIB_ZFCore void _ZFP_ZFMethodFuncUnregister(ZF_IN const ZFMethod *method);

/**
 * @brief get function type method for name
 *
 * note this method would only find function type method declared by ZFMETHOD_FUNC_DECLARE_xxx series,
 * use #ZFClass::methodForName to find class static member method,
 * or use #ZFMethodForName for short
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodFuncForName(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        );
/**
 * @brief see #ZFMethodFuncForName
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodFuncForName(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
        );
/**
 * @brief see #ZFMethodFuncForName
 */
extern ZFLIB_ZFCore void ZFMethodFuncForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        );
/**
 * @brief see #ZFMethodFuncForName
 */
inline ZFCoreArray<const ZFMethod *> ZFMethodFuncForNameGetAll(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        ) {
    ZFCoreArray<const ZFMethod *> ret;
    ZFMethodFuncForNameGetAllT(ret, methodNamespace, methodName);
    return ret;
}

/** @brief see #ZFMethodFuncGetAll */
extern ZFLIB_ZFCore void ZFMethodFuncGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret);
/**
 * @brief get all function type method currently registered, for debug use only
 */
inline ZFCoreArray<const ZFMethod *> ZFMethodFuncGetAll(void) {
    ZFCoreArray<const ZFMethod *> ret;
    ZFMethodFuncGetAllT(ret);
    return ret;
}

// ============================================================
#define _ZFP_ZFMethodFuncAccess(MethodNamespace, MethodName) \
    _ZFP_ZFMethodFuncAccess_(MethodNamespace, MethodName)
#define _ZFP_ZFMethodFuncAccess_(MethodNamespace, MethodName) \
    ((MethodNamespace::_ZFP_MtdFA_##MethodName)(zfnull))
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccess(MethodName) \
    _ZFP_ZFMethodFuncAccess(ZF_NAMESPACE_GLOBAL_ID, MethodName)

#define _ZFP_ZFMethodFuncAccessDetail(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodFuncAccessDetail_(__VA_ARGS__))
#define _ZFP_ZFMethodFuncAccessDetail_(MethodNamespace, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    (((const ZFMethod * (*)(void (*)( \
           ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
           ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
           ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
           ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
           ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
           ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
           ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
           ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
        )))(MethodNamespace::_ZFP_MtdFA_##MethodName))(zfnull))
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_0(MethodNamespace, MethodName \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_1(MethodNamespace, MethodName \
        , ZFMP_0 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_2(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_3(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_4(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_5(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_6(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_7(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_MtdP_EXPAND(ZFMP_6) \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_8(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
        , ZFMP_7 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_MtdP_EXPAND(ZFMP_6) \
            , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

// ============================================================
#define _ZFP_ZFMethodFuncIsInline isInline
#define _ZFP_ZFMethodFuncNotInline notInline

#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_isInline(ZFLIB_) inline
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_notInline(ZFLIB_) extern ZFLIB_
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix(ZFLIB_, isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_##isInlineOrNot(ZFM_EXPAND(ZFLIB_))

#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_isInline()
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_notInline() ;
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_##isInlineOrNot()

#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_isInline() inline
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_notInline() extern
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_##isInlineOrNot()

#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_isInline() ;
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_notInline()
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_##isInlineOrNot()

#if ZF_DOXYGEN_FLAG
    #define _ZFP_ZFMETHOD_FUNC_NAME(MethodName) MethodName
#else
    #define _ZFP_ZFMETHOD_FUNC_NAME(MethodName) (MethodName)
#endif

// ============================================================
#define _ZFP_ZFMETHOD_FUNC_DECLARE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_DECLARE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_DECLARE_(ZFLIB_, \
        isInlineOrNot, \
        ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    /** @cond ZFPrivateDoc */ \
    extern ZFLIB_ const ZFMethod *_ZFP_MtdFA_##MethodName(void (*)( \
            ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
        )); \
    /** @endcond */ \
    _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix(ZFLIB_, isInlineOrNot) ReturnType _ZFP_ZFMETHOD_FUNC_NAME(MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
        ) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix(isInlineOrNot)

#define _ZFP_ZFMETHOD_FUNC_DEFINE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_DEFINE_( \
        isInlineOrNot, RegSig, \
        ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) ReturnType _ZFP_ZFMETHOD_FUNC_NAME(MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ); \
    zfclassNotPOD _ZFP_MtdFH_##MethodName##_##RegSig { \
    public: \
        _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
                ReturnType \
                , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
            ) \
        static ReturnType methodInvoker( \
                ZF_IN const ZFMethod *invokerMethod, \
                ZF_IN zfany const &invokerObject \
                ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                ) { \
            return (MethodName)( \
                    ParamExpandOrEmpty0(            param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                ); \
        } \
    }; \
    const ZFMethod *_ZFP_MtdFA_##MethodName(void (*)( \
            ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
            )) { \
        static _ZFP_ZFMethodRegisterHolder _methodHolder(zffalse \
                , zffalse \
                , zfnull \
                , reinterpret_cast<ZFFuncAddrType>(&_ZFP_MtdFH_##MethodName##_##RegSig::methodInvoker) \
                , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig) \
                , ZFMethodTypeStatic \
                , zfnull \
                , ZFMethodAccessTypePublic \
                , ZF_NAMESPACE_CURRENT() \
                , zftext(#MethodName) \
                , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
                , _ZFP_ZFMethodMP() \
                ParamExpandOrEmpty0(.add( \
                        ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                        , zftext(#param0) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty0, 0) \
                        )) \
                ParamExpandOrEmpty1(.add( \
                        ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                        , zftext(#param1) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty1, 1) \
                        )) \
                ParamExpandOrEmpty2(.add( \
                        ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                        , zftext(#param2) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty2, 2) \
                        )) \
                ParamExpandOrEmpty3(.add( \
                        ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                        , zftext(#param3) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty3, 3) \
                        )) \
                ParamExpandOrEmpty4(.add( \
                        ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                        , zftext(#param4) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty4, 4) \
                        )) \
                ParamExpandOrEmpty5(.add( \
                        ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                        , zftext(#param5) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty5, 5) \
                        )) \
                ParamExpandOrEmpty6(.add( \
                        ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                        , zftext(#param6) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty6, 6) \
                        )) \
                ParamExpandOrEmpty7(.add( \
                        ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                        , zftext(#param7) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty7, 7) \
                        )) \
            ); \
        return _methodHolder.method; \
    } \
    ZF_STATIC_REGISTER_INIT(MtdFR_##MethodName##_##RegSig) { \
        (((const ZFMethod * (*)(void (*)( \
               ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
               ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
               ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
               ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
               ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
               ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
               ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
               ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
            )))(_ZFP_MtdFA_##MethodName))(zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdFR_##MethodName##_##RegSig) \
    _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) ReturnType _ZFP_ZFMETHOD_FUNC_NAME(MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(isInlineOrNot)

// ============================================================
// 0 param method declare
/**
 * @brief declare function type of #ZFMethod
 *
 * usage:
 * @code
 *   // in header file
 *   ZFMETHOD_FUNC_DECLARE_0(ZFLIB_APP, void, myFunc)
 *
 *   // in source file
 *   ZFMETHOD_FUNC_DEFINE_0(void, myFunc) {
 *       // func body
 *   }
 * @endcode
 *
 * similar to #ZFMETHOD_INLINE_0, but declared in global scope instead of class scope,
 * declared method can be reflected by #ZFMethodFuncForName,
 * and can be invoked by by #ZFMethod::executeExact with null as first param\n
 * function type method support at most one level namespace
 */
#define ZFMETHOD_FUNC_DECLARE_0(ZFLIB_, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_0(ZFLIB_, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_0(ZFLIB_, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 1 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_1(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_1(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_1(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 2 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_2(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_2(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_2(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 3 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_3(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_3(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_3(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 4 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_4(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_4(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_4(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 5 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_5(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_5(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_5(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_5(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 6 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_6(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_6(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_6(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_6(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 7 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_7(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_7(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_7(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_7(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 8 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_8(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_8(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncNotInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_8(ZFLIB_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_8(ZFLIB_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE(ZFLIB_, \
        _ZFP_ZFMethodFuncIsInline, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

/* ZFMETHOD_MAX_PARAM */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodFuncDeclare_h_

