/**
 * @file ZFCoreEnvConfig.h
 * @brief global definition
 *
 * this file would be included by all files before any other files,
 * so put things here ONLY if necessary
 */
#ifndef _ZFI_ZFCoreEnvConfig_h_
#define _ZFI_ZFCoreEnvConfig_h_

/* ZFTAG_ADD_MODULE */
/**
 * @def ZF_ENV_ZFVERSION_MAIN
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION_SUB
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION_MINOR
 * @brief see #ZF_ENV_ZFVERSION
 * @def ZF_ENV_ZFVERSION
 * @brief version of ZFFramework, e.g. '0.1.0'
 */
#define ZF_ENV_ZFVERSION_MAIN()       0
#define ZF_ENV_ZFVERSION_SUB()        10
#define ZF_ENV_ZFVERSION_MINOR()      0

#define ZF_ENV_ZFVERSION() _ZFP_ZF_ENV_ZFVERSION(ZF_ENV_ZFVERSION_MAIN(), ZF_ENV_ZFVERSION_SUB(), ZF_ENV_ZFVERSION_MINOR())
#define _ZFP_ZF_ENV_ZFVERSION(vMain, vSub, vMinor) ZFM_TOSTRING(vMain) "." ZFM_TOSTRING(vSub) "." ZFM_TOSTRING(vMinor)

#if 0
// ZFTAG_ADD_MODULE
/*
 * to make doc with Doxygen, you should
 * specify INPUT and INCLUDE_PATH to the "zfsrc" dir of each module,
 * and:
 * ============================================================ */
FULL_PATH_NAMES        = NO
AUTOLINK_SUPPORT       = YES
EXTRACT_PRIVATE        = NO
EXTRACT_LOCAL_CLASSES  = NO
EXTRACT_LOCAL_METHODS  = NO
CASE_SENSE_NAMES       = NO
SORT_MEMBER_DOCS       = NO
MAX_INITIALIZER_LINES  = 0
RECURSIVE              = YES
USE_MDFILE_AS_MAINPAGE = ../../../README.md
HTML_FOOTER            = doc_footer.html
HTML_TIMESTAMP         = YES
GENERATE_LATEX         = NO
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = YES
EXPAND_ONLY_PREDEF     = NO
SKIP_FUNCTION_MACROS   = YES

INPUT                  =
INPUT                  += ../../../README.md
INPUT                  += ../docs
INPUT                  += ../../../ZF/ZFAlgorithm/zfsrc
INPUT                  += ../../../ZF/ZFAlgorithm_impl/zfsrc
INPUT                  += ../../../ZF/ZFCore/zfsrc
INPUT                  += ../../../ZF/ZFCore_impl/zfsrc
INPUT                  += ../../../ZF/ZFLua/zfsrc
INPUT                  += ../../../ZF/ZFLua_impl/zfsrc
INPUT                  += ../../../ZF/ZFNet/zfsrc
INPUT                  += ../../../ZF/ZFNet_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIExt/zfsrc
INPUT                  += ../../../ZF/ZFUIKit/zfsrc
INPUT                  += ../../../ZF/ZFUIKit_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit/zfsrc
INPUT                  += ../../../ZF/ZFUIWebKit_impl/zfsrc
INPUT                  += ../../../ZF/ZFUIWidget/zfsrc
INPUT                  += ../../../ZF/ZFUtility/zfsrc
INPUT                  += ../../../ZF/ZF_impl/zfsrc

FILE_PATTERNS          = *.h *.hpp

EXCLUDE_PATTERNS      += */*.java
EXCLUDE_PATTERNS      += */_repo/*
EXCLUDE_PATTERNS      += */_tmp/*
EXCLUDE_PATTERNS      += */_impl/*

EXCLUDE_SYMBOLS       += _ZFP_*
EXCLUDE_SYMBOLS       += _ZFI_*
EXCLUDE_SYMBOLS       += _ZFT_*
EXCLUDE_SYMBOLS       += *zfstl_impl*

INCLUDE_PATH           =
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFAlgorithm_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFCore_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFLua/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFLua_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFNet/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFNet_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIExt/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIKit_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWebKit_impl/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUIWidget/zfsrc
INCLUDE_PATH           += ../../../ZF/ZFUtility/zfsrc
INCLUDE_PATH           += ../../../ZF/ZF_impl/zfsrc

PREDEFINED            += ZF_DOXYGEN_FLAG
PREDEFINED            += ZF_NAMESPACE_GLOBAL_BEGIN:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_END:=
PREDEFINED            += ZF_NAMESPACE_GLOBAL_USE:=
PREDEFINED            += _ZFP_ZFIMPLEMENT_DECLARE(...):=
PREDEFINED            += ZFIMPLEMENT_DECLARE(...):=
PREDEFINED            += ZF_ENV_EXPORT:=
PREDEFINED            += ZF_ENV_IMPORT:=
/* ============================================================ */
#endif

// ignore MSVC warnings
#if defined(_MSC_VER)
    // reference type warnings
    #pragma warning(disable:4180)

    // internal template without ZF_ENV_EXPORT
    #pragma warning(disable:4251)

    // placement new for POD with no-param-constructor:
    //   new addr Type();
    #pragma warning(disable:4345)

    // array member init
    #pragma warning(disable:4351)

    // unsafe APIs
    #pragma warning(disable:4996)
#endif

#endif // #ifndef _ZFI_ZFCoreEnvConfig_h_

