/**
 * @file ZFMethodUserRegister.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodUserRegister_h_
#define _ZFI_ZFMethodUserRegister_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
extern ZFLIB_ZFCore void ZFMethodUserUnregister(ZF_IN const ZFMethod *method);

// ============================================================
#define _ZFP_ZFMethodUserRegisterParamExpand( \
        GenericInvokerOwner, methodInvoker, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    ( \
        zftrue \
        , zffalse \
        , zfnull \
        , reinterpret_cast<ZFFuncAddrType>(methodInvoker) \
        , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(GenericInvokerOwner) \
        , ZFMethodType_ \
        , ownerClass \
        , _ZFP_ZFMethod_initClassMemberType_accessType(PublicOrProtectedOrPrivate) \
        , zfnull \
        , methodNameString \
        , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
        , _ZFP_ZFMethodMP() \
        ParamExpandOrEmpty0(.add( \
                ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                , zftext(#param0) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty0, 0) \
                )) \
        ParamExpandOrEmpty1(.add( \
                ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                , zftext(#param1) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty1, 1) \
                )) \
        ParamExpandOrEmpty2(.add( \
                ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                , zftext(#param2) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty2, 2) \
                )) \
        ParamExpandOrEmpty3(.add( \
                ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                , zftext(#param3) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty3, 3) \
                )) \
        ParamExpandOrEmpty4(.add( \
                ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                , zftext(#param4) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty4, 4) \
                )) \
        ParamExpandOrEmpty5(.add( \
                ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                , zftext(#param5) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty5, 5) \
                )) \
        ParamExpandOrEmpty6(.add( \
                ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                , zftext(#param6) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty6, 6) \
                )) \
        ParamExpandOrEmpty7(.add( \
                ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                , zftext(#param7) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty7, 7) \
                )) \
    )

#define _ZFP_ZFMethodUserRegister(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMethodUserRegister_(__VA_ARGS__))
#define _ZFP_ZFMethodUserRegister_(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    const ZFMethod *resultMethod = zfnull; \
    { \
        zfclassNotPOD _ZFP_mUR { \
        public: \
            static ReturnType I( \
                    ZF_IN const ZFMethod *invokerMethod \
                    , ZF_IN zfany const &invokerObject \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                    ) { \
                methodInvokerAction \
            } \
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
        resultMethod = _ZFP_ZFMethodRegister _ZFP_ZFMethodUserRegisterParamExpand( \
                _ZFP_mUR, _ZFP_mUR::I, ownerClass, \
                PublicOrProtectedOrPrivate, ZFMethodType_, \
                ReturnType, methodNameString \
                , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
            ); \
    } \
    ZFUNUSED(resultMethod)

// ============================================================
#define _ZFP_ZFMETHOD_USER_REGISTER(...) \
    _ZFP_mD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_( \
        ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
    zfclassNotPOD _ZFP_mURIvk_##ownerClassSig##_##methodName##_##RegSig { \
    public: \
        static ReturnType I( \
                ZF_IN const ZFMethod *invokerMethod \
                , ZF_IN zfany const &invokerObject \
                ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                ); \
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
    static _ZFP_ZFMethodRegisterHolder _ZFP_mURH_##ownerClassSig##_##methodName##_##RegSig \
        _ZFP_ZFMethodUserRegisterParamExpand( \
            _ZFP_mURIvk_##ownerClassSig##_##methodName##_##RegSig, \
            _ZFP_mURIvk_##ownerClassSig##_##methodName##_##RegSig::I, \
            ownerClassSig::ClassData(), \
            PublicOrProtectedOrPrivate, ZFMethodType_, \
            ReturnType, zftext(#methodName) \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        ); \
    ReturnType _ZFP_mURIvk_##ownerClassSig##_##methodName##_##RegSig::I( \
            ZF_IN const ZFMethod *invokerMethod \
            , ZF_IN zfany const &invokerObject \
            ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
            )

// ============================================================
// 0 param
/**
 * @brief register a custom method to existing class, for advanced reflection use only
 *
 * example:
 * @code
 *   // register it, using ZF_GLOBAL_INITIALIZER_INIT is recommended
 *   ZF_GLOBAL_INITIALIZER_INIT(MyMethodRegister) {
 *       ZFMethodUserRegisterDetail_2(resultMethod, {
 *               return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *           }, MyClassToAttachTheMethod::ClassData(),
 *           public, ZFMethodTypeStatic,
 *           ReturnType, "myMethodName"
 *           , ZFMP_IN(ParamType0, param0)
 *           , ZFMP_IN(ParamType1, param1));
 *       _method = resultMethod;
 *       ZFLogTrim() << resultMethod;
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyMethodRegister) {
 *       ZFMethodUserUnregister(_method);
 *   }
 *   const ZFMethod *_method;
 *   ZF_GLOBAL_INITIALIZER_END(MyMethodRegister)
 *
 *   // or, you may use this macro for short, at cpp files only
 *   ZFMETHOD_USER_REGISTER_DETAIL_2(MyClassToAttachTheMethod::ClassData(),
 *       public, ZFMethodTypeStatic, ZF_CALLER_LINE,
 *       ReturnType, "myMethodName"
 *       , ZFMP_IN(ParamType0, param0)
 *       , ZFMP_IN(ParamType1, param1)
 *       ) {
 *       return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *   }
 * @endcode
 *
 * note:
 * -  if there is already a method exist with same method name and method id:
 *   -  if registered to same class, register would fail
 *   -  if registered to different class in inherit tree, child class's one would replace parent class
 * -  the registered method must be unregister manually at proper time,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommeded
 * -  once registered, the method can be reflected by #ZFClass::methodForName,
 *   but there's no way to invoke the method by object directly,
 *   it can only be invoked by #ZFMethod::executeExact
 *
 * @see ZFMethodDynamicRegister
 */
#define ZFMethodUserRegister_0(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_0(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_0(ownerClassSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_0(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_1(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_1(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_1(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_1(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_2(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_2(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_2(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_2(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_3(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_3(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_3(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_3(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_4(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_4(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_4(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_4(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_5(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_5(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_5(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_5(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_6(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_6(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_6(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_6(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_7(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_7(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_7(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_7(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#define ZFMethodUserRegister_8(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
        ReturnType, methodNameString \
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
#define ZFMethodUserRegisterDetail_8(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
        ReturnType, methodNameString \
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
#define ZFMETHOD_USER_REGISTER_8(ownerClassSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
#define ZFMETHOD_USER_REGISTER_DETAIL_8(ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER(ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
#endif // #ifndef _ZFI_ZFMethodUserRegister_h_

#include "ZFMethodUserRegister_ZFObject.h"
#include "ZFMethodUserRegister_Wrapper.h"

