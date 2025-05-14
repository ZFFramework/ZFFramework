/**
 * @file ZFCoreEnvDef.h
 * @brief used to define the env used by ZFFramework
 */
#ifndef _ZFI_ZFCoreEnvDef_h_
#define _ZFI_ZFCoreEnvDef_h_

#include "ZFCoreEnvConfig.h"

// ============================================================
/** @brief used to export symbols */
#ifndef ZF_ENV_EXPORT
    #ifdef _WIN32
        #define ZF_ENV_EXPORT __declspec(dllexport)
    #else
        #define ZF_ENV_EXPORT __attribute__((visibility("default")))
    #endif
#endif
/** @brief used to import symbols */
#ifndef ZF_ENV_IMPORT
    #ifdef _WIN32
        #define ZF_ENV_IMPORT __declspec(dllimport)
    #else
        #define ZF_ENV_IMPORT
    #endif
#endif

/** @brief used to export symbols */
#ifndef ZFLIB_ZFCore
    #define ZFLIB_ZFCore ZF_ENV_IMPORT
#endif

/** @brief used to export symbols */
#ifndef ZFLIB_APP
    #define ZFLIB_APP ZF_ENV_EXPORT
#endif

/**
 * @def ZF_ENV_EXTERN_C_BEGIN
 * @brief extern "C" wrapper
 * @def ZF_ENV_EXTERN_C_END
 * @brief extern "C" wrapper
 */
#if defined(__cplusplus) || defined(c_plusplus)
    #define ZF_ENV_EXTERN_C_BEGIN extern "C" {
    #define ZF_ENV_EXTERN_C_END }
#else
    #define ZF_ENV_EXTERN_C_BEGIN
    #define ZF_ENV_EXTERN_C_END
#endif

// ============================================================
/**
 * @brief dummy macro to show the code is env sensitive
 */
#define ZF_ENV_SENSITIVE(hint)

// ============================================================
/**
 * @brief whether use C++11
 *
 * add -DZF_ENV_LAMBDA=1 to compiler to override
 * @warning must be explicitly declared by compiler,
 *   also, all libs must be compiled with same config,
 *   otherwise, unexpected behavior may happen,
 *   if you don't know what to do,
 *   just leave it off
 */
#ifndef ZF_ENV_CPP11
    #define ZF_ENV_CPP11 0
#endif

// ============================================================
/**
 * @brief true to enable lambda (which require C++11), auto detect by default
 *
 * add -DZF_ENV_LAMBDA=1 to compiler to override
 */
#ifndef ZF_ENV_LAMBDA
    #if defined(__cplusplus) && (__cplusplus >= 201103L)
        #define ZF_ENV_LAMBDA 1
    #else
        #define ZF_ENV_LAMBDA 0
    #endif
#endif

// ============================================================
/**
 * @brief whether in debug mode
 *
 * add -DZF_ENV_DEBUG=1 to compiler to make this macro take effect
 */
#ifndef ZF_ENV_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
        #define ZF_ENV_DEBUG 1
    #else
        #define ZF_ENV_DEBUG 0
    #endif
#endif

#endif // #ifndef _ZFI_ZFCoreEnvDef_h_

