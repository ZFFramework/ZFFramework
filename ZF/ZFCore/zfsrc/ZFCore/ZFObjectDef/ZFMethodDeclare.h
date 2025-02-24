/**
 * @file ZFMethodDeclare.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodDeclare_h_
#define _ZFI_ZFMethodDeclare_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFMethodAccess(OwnerClass, MethodName) \
    (OwnerClass::_ZFP_Mtd_##MethodName(zfnull))
/** @brief see #ZFMethod */
#define ZFMethodAccess(OwnerClass, MethodName) \
    _ZFP_ZFMethodAccess(OwnerClass, MethodName)

#define _ZFP_ZFMethodAccessDetail(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodAccessDetail_(__VA_ARGS__))
#define _ZFP_ZFMethodAccessDetail_(OwnerClass, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    (OwnerClass::_ZFP_Mtd_##MethodName((void (*)( \
           ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
           ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
           ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
           ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
           ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
           ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
           ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
           ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
       ))zfnull))
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_0(OwnerClass, MethodName \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_1(OwnerClass, MethodName \
        , ZFMP_0 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_2(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_3(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_4(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_5(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_6(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_7(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_MtdP_EXPAND(ZFMP_6) \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMethodAccessDetail_8(OwnerClass, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
        , ZFMP_7 \
    ) \
    _ZFP_ZFMethodAccessDetail(OwnerClass, MethodName \
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
#define _ZFP_ZFMethod_AutoRegister_notAutoRegister(MethodName, RegSig \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    )
#define _ZFP_ZFMethod_AutoRegister(autoRegisterOrNot, MethodName, RegSig \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMethod_AutoRegister_##autoRegisterOrNot(MethodName, RegSig \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        )

#define _ZFP_ZFMethod_initClassMemberType_privilege_public ZFMethodAccessTypePublic
#define _ZFP_ZFMethod_initClassMemberType_privilege_protected ZFMethodAccessTypeProtected
#define _ZFP_ZFMethod_initClassMemberType_privilege_private ZFMethodAccessTypePrivate
#define _ZFP_ZFMethod_initClassMemberType_privilege(privilege) _ZFP_ZFMethod_initClassMemberType_privilege_##privilege

// ============================================================
#define _ZFP_ZFMethod_AutoRegister_isAutoRegister(MethodName, RegSig \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    zfclassNotPOD _ZFP_MtdReg_##MethodName##_##RegSig { \
    public: \
        _ZFP_MtdReg_##MethodName##_##RegSig(void) { \
            (void)zfself::_ZFP_Mtd_##MethodName((void (*)( \
                   ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
                   ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                   ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                   ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                   ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                   ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                   ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                   ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
               ))zfnull); \
        } \
    }; \
    _ZFP_MtdReg_##MethodName##_##RegSig _ZFP_MtdRegI_##MethodName##_##RegSig; \
    friend zfclassFwd _ZFP_MtdReg_##MethodName##_##RegSig;
#define _ZFP_ZFMETHOD_INLINE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_INLINE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_INLINE_( \
        autoRegisterOrNot, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
        /** @cond ZFPrivateDoc */ /* required to make Doxygen doc placed at right pos */ \
    private: \
        _ZFP_ZFMethod_AutoRegister(autoRegisterOrNot, MethodName, RegSig \
                , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
            ) \
        zfclassNotPOD _ZFP_MtdH_##MethodName##_##RegSig { \
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
        }; \
    private: \
        static ReturnType _ZFP_MtdI_##MethodName( \
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
            return zfcast(zfself *, invokerObject)->MethodName( \
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
    public: \
        static const ZFMethod *_ZFP_Mtd_##MethodName(void (*)( \
                ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                )) /* tricks to support overload method */ { \
            static _ZFP_ZFMethodRegisterHolder _methodHolder(zffalse \
                    , zffalse \
                    , zfnull \
                    , reinterpret_cast<ZFFuncAddrType>((ReturnType (*)( \
                            ZF_IN const ZFMethod *invokerMethod, \
                            ZF_IN zfany const &invokerObject \
                            ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                            ))&zfself::_ZFP_MtdI_##MethodName) \
                    , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(_ZFP_MtdH_##MethodName##_##RegSig) \
                    , ZFMethodType_ \
                    , zfself::ClassData() \
                    , _ZFP_ZFMethod_initClassMemberType_privilege(PublicOrProtectedOrPrivate) \
                    , zfnull \
                    , zftext(#MethodName) \
                    , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
                    , _ZFP_ZFMethodMP() \
                    ParamExpandOrEmpty0(.add( \
                            ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                            , zftext(#param0) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty0, 0) \
                            )) \
                    ParamExpandOrEmpty1(.add( \
                            ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                            , zftext(#param1) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty1, 1) \
                            )) \
                    ParamExpandOrEmpty2(.add( \
                            ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                            , zftext(#param2) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty2, 2) \
                            )) \
                    ParamExpandOrEmpty3(.add( \
                            ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                            , zftext(#param3) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty3, 3) \
                            )) \
                    ParamExpandOrEmpty4(.add( \
                            ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                            , zftext(#param4) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty4, 4) \
                            )) \
                    ParamExpandOrEmpty5(.add( \
                            ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                            , zftext(#param5) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty5, 5) \
                            )) \
                    ParamExpandOrEmpty6(.add( \
                            ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                            , zftext(#param6) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty6, 6) \
                            )) \
                    ParamExpandOrEmpty7(.add( \
                            ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                            , zftext(#param7) \
                            , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##MethodName##_##RegSig, DefaultExpandOrEmpty7, 7) \
                            )) \
                ); \
            return _methodHolder.method; \
        } \
        /** @endcond */ \
    PublicOrProtectedOrPrivate: \
        /** @cond ZFPrivateDoc */ \
        _ZFP_ZFMethodTypeChain(ZFMethodType_, \
            zffinal inline zfself *c_##MethodName( \
                    ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
                ) { \
                this->MethodName( \
                        ParamExpandOrEmpty0(            param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
                return this; \
            } \
        ) \
        /** @endcond */ \
        _ZFP_ZFMethodType(ZFMethodType_) ReturnType MethodName( \
                ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
            )

// ============================================================
#define _ZFP_ZFMETHOD_DECLARE_isPureVirtual() = 0
#define _ZFP_ZFMETHOD_DECLARE_notPureVirtual()

#define _ZFP_ZFMETHOD_DECLARE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_DECLARE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_DECLARE_( \
        PublicOrProtectedOrPrivate, ZFMethodType_, pureVirtualOrNot, \
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
        /** @cond ZFPrivateDoc */ /* required to make Doxygen doc placed at right pos */ \
    private: \
        static ReturnType _ZFP_MtdI_##MethodName( \
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
            return zfcast(zfself *, invokerObject)->MethodName( \
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
        static inline ZFMethodAccessType _ZFP_MtdS_P_##MethodName(void (*)( \
                ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                )) { \
            return _ZFP_ZFMethod_initClassMemberType_privilege(PublicOrProtectedOrPrivate); \
        } \
        static inline ZFMethodType _ZFP_MtdS_T_##MethodName(void (*)( \
                ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                )) { \
            return ZFMethodType_; \
        } \
    public: \
        static const ZFMethod *_ZFP_Mtd_##MethodName(void (*)( \
                ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
            )); /* tricks to support overload method */ \
        /** @endcond */ \
    PublicOrProtectedOrPrivate: \
        /** @cond ZFPrivateDoc */ \
        _ZFP_ZFMethodTypeChain(ZFMethodType_, \
            zffinal inline zfself *c_##MethodName( \
                    ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
                ) { \
                this->MethodName( \
                        ParamExpandOrEmpty0(            param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
                return this; \
            } \
        ) \
        /** @endcond */ \
        _ZFP_ZFMethodType(ZFMethodType_) ReturnType MethodName( \
                ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
            ) pureVirtualOrNot();

#define _ZFP_ZFMETHOD_DEFINE_isPureVirtual(...)
#define _ZFP_ZFMETHOD_DEFINE_notPureVirtual(...) __VA_ARGS__

#define _ZFP_ZFMETHOD_DEFINE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_DEFINE_(OwnerClass, pureVirtualOrNot, RegSig, \
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
    zfclassNotPOD _ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig { \
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
    }; \
    const ZFMethod *OwnerClass::_ZFP_Mtd_##MethodName(void (*_dummy)( \
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
                , reinterpret_cast<ZFFuncAddrType>((ReturnType (*)( \
                        ZF_IN const ZFMethod *invokerMethod, \
                        ZF_IN zfany const &invokerObject \
                        ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                        ))&zfself::_ZFP_MtdI_##MethodName) \
                , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig) \
                , zfself::_ZFP_MtdS_T_##MethodName(_dummy) \
                , zfself::ClassData() \
                , zfself::_ZFP_MtdS_P_##MethodName(_dummy) \
                , zfnull \
                , zftext(#MethodName) \
                , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
                , _ZFP_ZFMethodMP() \
                ParamExpandOrEmpty0(.add( \
                        ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                        , zftext(#param0) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty0, 0) \
                        )) \
                ParamExpandOrEmpty1(.add( \
                        ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                        , zftext(#param1) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty1, 1) \
                        )) \
                ParamExpandOrEmpty2(.add( \
                        ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                        , zftext(#param2) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty2, 2) \
                        )) \
                ParamExpandOrEmpty3(.add( \
                        ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                        , zftext(#param3) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty3, 3) \
                        )) \
                ParamExpandOrEmpty4(.add( \
                        ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                        , zftext(#param4) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty4, 4) \
                        )) \
                ParamExpandOrEmpty5(.add( \
                        ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                        , zftext(#param5) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty5, 5) \
                        )) \
                ParamExpandOrEmpty6(.add( \
                        ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                        , zftext(#param6) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty6, 6) \
                        )) \
                ParamExpandOrEmpty7(.add( \
                        ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                        , zftext(#param7) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdH_##OwnerClass##_##MethodName##_##RegSig, DefaultExpandOrEmpty7, 7) \
                        )) \
            ); \
        return _methodHolder.method; \
    } \
    ZF_STATIC_REGISTER_INIT(MtdD_##OwnerClass##_##MethodName##_##RegSig) { \
        (void)OwnerClass::_ZFP_Mtd_##MethodName((void (*)( \
               ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
               ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
               ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
               ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
               ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
               ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
               ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
               ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
           ))zfnull); \
    } \
    ZF_STATIC_REGISTER_END(MtdD_##OwnerClass##_##MethodName##_##RegSig) \
    pureVirtualOrNot( \
        ReturnType OwnerClass::MethodName( \
                ParamExpandOrEmpty0(            ParamType0 param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
            ) \
        )

// ============================================================
// 0 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_0( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_0( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_0(OwnerClass, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_0(OwnerClass, RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_1( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_1( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_1(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_1(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_2( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_2( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_2(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_2(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_3( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_3( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_3(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_3(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_4( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_4( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_4(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_4(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_5( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_5( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_5(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_5(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_6( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_6( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_6(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_6(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_7( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_7( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_7(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_7(OwnerClass, RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_STATIC_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeStatic, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_PROTECTED_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        protected, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DECLARE_DETAIL_8( \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
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
    _ZFP_ZFMETHOD_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodType_, _ZFP_ZFMETHOD_DECLARE_notPureVirtual, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_STATIC_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_PROTECTED_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_INLINE_DETAIL_8( \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
    _ZFP_ZFMETHOD_INLINE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_8(OwnerClass, \
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
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethod */
#define ZFMETHOD_DEFINE_DETAIL_8(OwnerClass, RegSig, \
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
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_notPureVirtual, RegSig, \
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
#endif // #ifndef _ZFI_ZFMethodDeclare_h_

