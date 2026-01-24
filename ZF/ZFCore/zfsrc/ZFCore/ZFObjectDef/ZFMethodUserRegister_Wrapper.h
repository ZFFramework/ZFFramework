/**
 * @file ZFMethodUserRegister_Wrapper.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodUserRegister_Wrapper_h_
#define _ZFI_ZFMethodUserRegister_Wrapper_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Wrapper>
inline T_Wrapper &_ZFP_mUR_zfv(T_Wrapper &zfv) {
    return zfv;
};
template<typename T_Wrapper>
inline T_Wrapper const &_ZFP_mUR_zfv(T_Wrapper const &zfv) {
    return zfv;
};
template<typename T_Wrapper>
inline T_Wrapper &_ZFP_mUR_zfv(T_Wrapper *zfv) {
    return *zfv;
};
template<typename T_Wrapper>
inline T_Wrapper const &_ZFP_mUR_zfv(const T_Wrapper *zfv) {
    return *zfv;
};

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperVarReadonly(getterMethod, WrapperClass, VarType, VarName) \
    ZFMethodUserRegister_0(getterMethod, { \
            return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName); \
        }, WrapperClass::ClassData(), VarType const &, zftext(#VarName))
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperVar(setterMethod, getterMethod, WrapperClass, VarType, VarName) \
    ZFMethodUserRegister_1(setterMethod, { \
            _ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName = param0; \
        }, WrapperClass::ClassData(), void, zftext(#VarName) \
        , ZFMP_IN(VarType const &, param0) \
        ); \
    ZFMethodUserRegister_0(getterMethod, { \
            return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName); \
        }, WrapperClass::ClassData(), VarType const &, zftext(#VarName))

/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(WrapperClass, VarType, VarName) \
    ZF_STATIC_REGISTER_INIT(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserRegister_0(getterMethod, { \
                return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName); \
            }, WrapperClass::ClassData(), VarType const &, zftext(#VarName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserUnregister(WrapperClass::ClassData()->methodForNameIgnoreParent(zftext(#VarName), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdUR_##WrapperClass##_##VarName)
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(WrapperClass, VarType, VarName) \
    ZF_STATIC_REGISTER_INIT(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                _ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName = param0; \
            }, WrapperClass::ClassData(), void, zftext(#VarName) \
            , ZFMP_IN(VarType const &, param0) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName); \
            }, WrapperClass::ClassData(), VarType const &, zftext(#VarName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserUnregister(WrapperClass::ClassData()->methodForNameIgnoreParent(zftext(#VarName), ZFTypeId<VarType>::TypeId())); \
        ZFMethodUserUnregister(WrapperClass::ClassData()->methodForNameIgnoreParent(zftext(#VarName), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdUR_##WrapperClass##_##VarName)

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperSetterGetter(setterMethod, getterMethod, WrapperClass, VarType, VarName) \
    ZFMethodUserRegister_1(setterMethod, { \
            _ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName(param0); \
        }, WrapperClass::ClassData(), void, zftext(#VarName) \
        , ZFMP_IN(VarType const &, param0) \
        ); \
    ZFMethodUserRegister_0(getterMethod, { \
            return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName()); \
        }, WrapperClass::ClassData(), VarType const &, zftext(#VarName))

/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(WrapperClass, VarType, VarName) \
    ZF_STATIC_REGISTER_INIT(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                _ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName(param0); \
            }, WrapperClass::ClassData(), void, zftext(#VarName) \
            , ZFMP_IN(VarType const &, param0) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return (VarType const &)(_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName()); \
            }, WrapperClass::ClassData(), VarType const &, zftext(#VarName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(MtdUR_##WrapperClass##_##VarName) { \
        ZFMethodUserUnregister(WrapperClass::ClassData()->methodForNameIgnoreParent(zftext(#VarName), ZFTypeId<VarType>::TypeId())); \
        ZFMethodUserUnregister(WrapperClass::ClassData()->methodForNameIgnoreParent(zftext(#VarName), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdUR_##WrapperClass##_##VarName)

// ============================================================
#define _ZFP_ZFMethodUserRegisterForWrapperFunc(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMethodUserRegisterForWrapperFunc_(__VA_ARGS__))
#define _ZFP_ZFMethodUserRegisterForWrapperFunc_(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, { \
            return (ReturnType)_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).methodName( \
                    ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                ); \
        }, WrapperClass::ClassData(), \
        public, ZFMethodTypeVirtual, \
        ReturnType, zftext(#methodName) \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    )
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_(WrapperClass, \
        RegSig, \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
                return (ReturnType)_ZFP_mUR_zfv(invokerObject->to<WrapperClass *>()->zfv).methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, WrapperClass, \
            public, ZFMethodTypeVirtual, RegSig, \
            ReturnType, methodName \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        )

// ============================================================
#define _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMethodUserRegisterForWrapperFuncStatic_(__VA_ARGS__))
#define _ZFP_ZFMethodUserRegisterForWrapperFuncStatic_(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, { \
            return (ReturnType)OriginalClass::methodName( \
                    ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                ); \
        }, WrapperClass::ClassData(), \
        public, ZFMethodTypeStatic, \
        ReturnType, zftext(#methodName) \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    )
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_(OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
                return (ReturnType)OriginalClass::methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, WrapperClass, \
            public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
            ReturnType, methodName \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        )

// ============================================================
// 0 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_0(resultMethod, WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_0(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_0(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 1 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_1(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_1(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 2 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_2(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_2(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 3 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_3(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_3(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_3(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 4 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_4(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_4(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_4(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 5 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_5(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_5(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_5(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 6 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_6(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_6(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_6(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 7 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_7(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_7(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_7(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_7(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
// 8 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFunc_8(resultMethod, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFunc(resultMethod, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForWrapperFuncStatic_8(resultMethod, OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodUserRegisterForWrapperFuncStatic(resultMethod, OriginalClass, WrapperClass, \
        ReturnType, methodName \
        , _ZFP_mP_EXPAND(ZFMP_0) \
        , _ZFP_mP_EXPAND(ZFMP_1) \
        , _ZFP_mP_EXPAND(ZFMP_2) \
        , _ZFP_mP_EXPAND(ZFMP_3) \
        , _ZFP_mP_EXPAND(ZFMP_4) \
        , _ZFP_mP_EXPAND(ZFMP_5) \
        , _ZFP_mP_EXPAND(ZFMP_6) \
        , _ZFP_mP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_8(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
        ReturnType, methodName \
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
#endif // #ifndef _ZFI_ZFMethodUserRegister_Wrapper_h_

