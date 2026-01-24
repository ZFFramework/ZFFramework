/**
 * @file ZFMethodInterfaceDeclare.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodInterfaceDeclare_h_
#define _ZFI_ZFMethodInterfaceDeclare_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFMETHOD_IMPL_isInline()
#define _ZFP_ZFMETHOD_IMPL_notInline() ;

#define _ZFP_ZFMETHOD_IMPL_DECLARE(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMETHOD_IMPL_DECLARE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_IMPL_DECLARE_( \
        inlineOrNot \
        , ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    public: \
        virtual ReturnType MethodName( \
                ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
            ) inlineOrNot()

#define _ZFP_ZFMETHOD_IMPL_DEFINE(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMETHOD_IMPL_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_IMPL_DEFINE_( \
        OwnerClass, ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
        ReturnType OwnerClass::MethodName( \
                ParamExpandOrEmpty0(            ParamType0 param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
            )

// ============================================================
// 0 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
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
#define ZFMETHOD_INTERFACE_DEFINE_0(OwnerClass, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_IMPL_DECLARE_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
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
#define ZFMETHOD_IMPL_INLINE_0(ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
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
#define ZFMETHOD_IMPL_DEFINE_0(OwnerClass, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
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
#define ZFMETHOD_INTERFACE_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_1(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_1(ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_1(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
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
#define ZFMETHOD_INTERFACE_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_2(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_2(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_2(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
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
#define ZFMETHOD_INTERFACE_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_3(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_3(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_3(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 4 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_4(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_4(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_4(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 5 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_5(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_5(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_5(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 6 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_6(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_6(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_6(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 7 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DECLARE( \
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_7(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_7(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_7(OwnerClass, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 8 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_8(ReturnType, MethodName \
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
        public, ZFMethodTypeVirtual, _ZFP_ZFMETHOD_DECLARE_isPureVirtual, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_INTERFACE_DEFINE_8(OwnerClass, \
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
    _ZFP_ZFMETHOD_DEFINE(OwnerClass, _ZFP_ZFMETHOD_DEFINE_isPureVirtual, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DECLARE_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_notInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_INLINE_8(ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_IMPL_DECLARE( \
        _ZFP_ZFMETHOD_IMPL_isInline, \
        ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_IMPL_DEFINE_8(OwnerClass, \
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
    _ZFP_ZFMETHOD_IMPL_DEFINE(OwnerClass, ReturnType, MethodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )

/* ZFMETHOD_MAX_PARAM */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodInterfaceDeclare_h_

