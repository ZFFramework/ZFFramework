/**
 * @file ZFExport.h
 * @brief util to export global variable
 */

#ifndef _ZFI_ZFExport_h_
#define _ZFI_ZFExport_h_

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFEXPORT_VAR_DEFINE(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportV_##Name) { \
        ZFMethodFuncUserRegister_1(setterMethod, { \
                    (Name)(v); \
                }, ZF_NAMESPACE_CURRENT(), \
                void, zftext(#Name) \
                , ZFMP_IN(Type const &, v) \
                ); \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                    return (Name)(); \
                }, ZF_NAMESPACE_CURRENT(), \
                Type &, zftext(#Name) \
                ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportV_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(ZF_NAMESPACE_CURRENT(), zftext(#Name), ZFTypeId<Type>::TypeId())); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(ZF_NAMESPACE_CURRENT(), zftext(#Name), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(ExportV_##Name)
#define _ZFP_ZFEXPORT_VAR_DEFINE_READONLY(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportV_##Name) { \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                    return (Name)(); \
                }, ZF_NAMESPACE_CURRENT(), \
                Type const &, zftext(#Name) \
                ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportV_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName(ZF_NAMESPACE_CURRENT(), zftext(#Name), zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(ExportV_##Name)

// ============================================================
/**
 * @brief util to export global variable
 *
 * the variable are exported as #ZFMETHOD_FUNC_DECLARE_0\n
 * usage:
 * @code
 *   // in header file
 *   ZFEXPORT_VAR_DECLARE(ZFLIB_APP, YourType, yourName)
 *
 *   // in source file
 *   ZFEXPORT_VAR_DEFINE(YourType, yourName, yourInitValue)
 *
 *   // access
 *   YourType &v = yourName();
 *   // modify
 *   yourName(v);
 * @endcode
 * \n
 * for readonly var, use #ZFEXPORT_VAR_READONLY_DECLARE\n
 * to alias existing var, use #ZFEXPORT_VAR_ALIAS_DECLARE\n
 * for existing raw var, use #ZFEXPORT_VAR_VALUEREF_DECLARE\n
 */
#define ZFEXPORT_VAR_DECLARE(ZFLIB_, Type, Name) \
    /** @cond ZFPrivateDoc */ \
    extern ZFLIB_ Type _ZFP_v_##Name; \
    /** @endcond */ \
    inline Type &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return _ZFP_v_##Name; \
    } \
    /** @brief see @ref Name */ \
    inline void _ZFP_ZFMETHOD_FUNC_NAME(Name)(ZF_IN Type const &v) { \
        _ZFP_v_##Name = v; \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_DEFINE(Type, Name, initValue) \
    Type _ZFP_v_##Name = (initValue); \
    _ZFP_ZFEXPORT_VAR_DEFINE(Type, Name)

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_ALIAS_DECLARE(ZFLIB_, Type, Name, AliasName) \
    inline Type &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return (AliasName)(); \
    } \
    /** @brief see @ref Name */ \
    inline void _ZFP_ZFMETHOD_FUNC_NAME(Name)(ZF_IN Type const &v) { \
        AliasName(v); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_ALIAS_DEFINE(Type, Name) \
    _ZFP_ZFEXPORT_VAR_DEFINE(Type, Name)

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_VALUEREF_DECLARE(ZFLIB_, Type, Name, ValueRef) \
    inline Type &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return ValueRef; \
    } \
    /** @brief see @ref Name */ \
    inline void _ZFP_ZFMETHOD_FUNC_NAME(Name)(ZF_IN Type const &v) { \
        ValueRef = v; \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_VALUEREF_DEFINE(Type, Name) \
    _ZFP_ZFEXPORT_VAR_DEFINE(Type, Name)

// ============================================================
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_, Type, Name) \
    /** @cond ZFPrivateDoc */ \
    extern ZFLIB_ Type _ZFP_v_##Name; \
    /** @endcond */ \
    inline Type const &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return _ZFP_v_##Name; \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_DEFINE(Type, Name, initValue) \
    Type _ZFP_v_##Name = (initValue); \
    _ZFP_ZFEXPORT_VAR_DEFINE_READONLY(Type, Name)

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_ALIAS_DECLARE(ZFLIB_, Type, Name, AliasName) \
    inline Type const &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return (AliasName)(); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_ALIAS_DEFINE(Type, Name) \
    _ZFP_ZFEXPORT_VAR_DEFINE_READONLY(Type, Name)

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_, Type, Name, ValueRef) \
    inline Type const &_ZFP_ZFMETHOD_FUNC_NAME(Name)(void) { \
        return ValueRef; \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(Type, Name) \
    _ZFP_ZFEXPORT_VAR_DEFINE_READONLY(Type, Name)

// ============================================================
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_USER_REGISTER(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportVar_##Name) { \
        ZFMethodFuncUserRegister_1(setterMethod, { \
                Name = v; \
            }, ZF_NAMESPACE_CURRENT(), void, zftext(#Name) \
            , ZFMP_IN(Type const &, v) \
            ); \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                return Name; \
            }, ZF_NAMESPACE_CURRENT(), Type const &, zftext(#Name) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportVar_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , ZFTypeId<Type>::TypeId() \
                )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , zfnull \
                )); \
    } \
    ZF_STATIC_REGISTER_END(ExportVar_##Name)
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_USER_REGISTER(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportVar_##Name) { \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                return Name; \
            }, ZF_NAMESPACE_CURRENT(), Type const &, zftext(#Name) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportVar_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , zfnull \
                )); \
    } \
    ZF_STATIC_REGISTER_END(ExportVar_##Name)

// ============================================================
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_USER_REGISTER_FOR_FUNC(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportVar_##Name) { \
        ZFMethodFuncUserRegister_1(setterMethod, { \
                Name(v); \
            }, ZF_NAMESPACE_CURRENT(), void, zftext(#Name) \
            , ZFMP_IN(Type const &, v) \
            ); \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                return Name(); \
            }, ZF_NAMESPACE_CURRENT(), Type const &, zftext(#Name) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportVar_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , ZFTypeId<Type>::TypeId() \
                )); \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , zfnull \
                )); \
    } \
    ZF_STATIC_REGISTER_END(ExportVar_##Name)
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_USER_REGISTER_FOR_FUNC(Type, Name) \
    ZF_STATIC_REGISTER_INIT(ExportVar_##Name) { \
        ZFMethodFuncUserRegister_0(getterMethod, { \
                return Name(); \
            }, ZF_NAMESPACE_CURRENT(), Type const &, zftext(#Name) \
            ); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportVar_##Name) { \
        ZFMethodFuncUserUnregister(ZFMethodFuncForName( \
                    ZF_NAMESPACE_CURRENT() \
                    , zftext(#Name) \
                    , zfnull \
                )); \
    } \
    ZF_STATIC_REGISTER_END(ExportVar_##Name)

// ============================================================
/**
 * @brief util macro to export raw enum
 *
 * for normal enum, use #ZFENUM_BEGIN is recommended\n
 * this macro is used to export low level enum values by #ZFMETHOD_FUNC_USER_REGISTER_0,
 * with this proto type:
 * @code
 *   YourEnumType YourRawEnumValueName(void);
 * @endcode
 */
#define ZFEXPORT_RAW_ENUM_DEFINE(EnumName, enumValues, ...) \
    _ZFP_ZFEXPORT_RAW_ENUM_DEFINE(EnumName, enumValues, ##__VA_ARGS__)

#define _ZFP_ZFEXPORT_RAW_ENUM_DEFINE_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFEXPORT_RAW_ENUM_DEFINE(...) \
    _ZFP_ZFEXPORT_RAW_ENUM_DEFINE_EXPAND(_ZFP_ZFEXPORT_RAW_ENUM_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFEXPORT_RAW_ENUM_DEFINE_(EnumName, enumValues, ...) \
    ZF_STATIC_REGISTER_INIT(ExportEnum_##EnumName) { \
        typedef EnumName _EnumName; \
        ZFM_FIX_PARAM(_ZFP_ZFEXPORT_RAW_ENUM_DEFINE_ITEM_EXPAND, ZFM_EMPTY, enumValues, ##__VA_ARGS__) \
    } \
    ZF_STATIC_REGISTER_DESTROY(ExportEnum_##EnumName) { \
        for(zfindex i = 0; i < m.count(); ++i) { \
            ZFMethodFuncUserUnregister(m[i]); \
        } \
    } \
    ZFCoreArray<const ZFMethod *> m; \
    ZF_STATIC_REGISTER_END(ExportEnum_##EnumName)
#define _ZFP_ZFEXPORT_RAW_ENUM_DEFINE_ITEM_EXPAND(v) \
    { \
        ZFMethodFuncUserRegister_0(resultMethod, { \
                return v; \
            }, ZF_NAMESPACE_CURRENT(), _EnumName, zftext(#v)); \
        m.add(resultMethod); \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFExport_h_

