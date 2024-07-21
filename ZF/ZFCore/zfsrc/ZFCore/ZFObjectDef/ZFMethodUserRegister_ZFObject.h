/**
 * @file ZFMethodUserRegister_ZFObject.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodUserRegister_ZFObject_h_
#define _ZFI_ZFMethodUserRegister_ZFObject_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectVarReadonly(getterMethod, ownerClassSig, VarType, VarName) \
    ZFMethodUserRegister_0(getterMethod, { \
            return (VarType const &)(invokerObject->to<ownerClassSig *>()->VarName); \
        }, ownerClassSig::ClassData(), VarType const &, zftext(#VarName))
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectVar(setterMethod, getterMethod, ownerClassSig, VarType, VarName) \
    ZFMethodUserRegister_1(setterMethod, { \
            invokerObject->to<ownerClassSig *>()->VarName = param0; \
        }, ownerClassSig::ClassData(), void, zftext(#VarName) \
        , ZFMP_IN(VarType const &, param0) \
        ); \
    ZFMethodUserRegister_0(getterMethod, { \
            return (VarType const &)(invokerObject->to<ownerClassSig *>()->VarName); \
        }, ownerClassSig::ClassData(), VarType const &, zftext(#VarName))

/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_READONLY(ownerClassSig, VarType, VarName) \
    ZF_STATIC_REGISTER_INIT(MtdUR_##ownerClassSig##_##VarName) { \
        ZFMethodUserRegister_0(getterMethod, { \
                return (VarType const &)(invokerObject->to<ownerClassSig *>()->VarName); \
            }, ownerClassSig::ClassData(), VarType const &, zftext(#VarName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(MtdUR_##ownerClassSig##_##VarName) { \
        ZFMethodUserUnregister(ownerClassSig::ClassData()->methodForNameIgnoreParent(zftext(#VarName), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdUR_##ownerClassSig##_##VarName)
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ownerClassSig, VarType, VarName) \
    ZF_STATIC_REGISTER_INIT(MtdUR_##ownerClassSig##_##VarName) { \
        ZFMethodUserRegister_1(setterMethod, { \
                invokerObject->to<ownerClassSig *>()->VarName = param0; \
            }, ownerClassSig::ClassData(), void, zftext(#VarName) \
            , ZFMP_IN(VarType const &, param0) \
            ); \
        ZFMethodUserRegister_0(getterMethod, { \
                return (VarType const &)(invokerObject->to<ownerClassSig *>()->VarName); \
            }, ownerClassSig::ClassData(), VarType const &, zftext(#VarName)); \
    } \
    ZF_STATIC_REGISTER_DESTROY(MtdUR_##ownerClassSig##_##VarName) { \
        ZFMethodUserUnregister(ownerClassSig::ClassData()->methodForNameIgnoreParent(zftext(#VarName), ZFTypeId<VarType>::TypeId())); \
        ZFMethodUserUnregister(ownerClassSig::ClassData()->methodForNameIgnoreParent(zftext(#VarName), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdUR_##ownerClassSig##_##VarName)

// ============================================================
#define _ZFP_ZFMethodUserRegisterForZFObjectFunc(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodUserRegisterForZFObjectFunc_(__VA_ARGS__))
#define _ZFP_ZFMethodUserRegisterForZFObjectFunc_( \
        resultMethod, ownerClassSig, \
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
            return (ReturnType)zfcast(ownerClassSig *, invokerObject)->methodName( \
                    ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                ); \
        }, ownerClass, \
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

// ============================================================
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_( \
        ownerClassSig, \
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
                return (ReturnType)zfcast(ownerClassSig *, invokerObject)->methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, ownerClassSig, \
            public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMethodUserRegisterForZFObjectFunc_0(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
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
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ownerClassSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
#define ZFMethodUserRegisterForZFObjectFunc_1(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 2 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_2(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 3 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_3(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 4 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_4(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 5 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_5(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_5(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 6 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_6(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_6(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 7 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_7(resultMethod, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_7(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
// 8 param
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterForZFObjectFunc_8(resultMethod, ownerClassSig, \
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
    _ZFP_ZFMethodUserRegisterForZFObjectFunc(resultMethod, ownerClassSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ownerClassSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
        ReturnType, methodName \
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
#endif // #ifndef _ZFI_ZFMethodUserRegister_ZFObject_h_

