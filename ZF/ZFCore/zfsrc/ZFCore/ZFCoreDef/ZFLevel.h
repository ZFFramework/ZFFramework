/**
 * @file ZFLevel.h
 * @brief level for logic which depends on order
 */
#ifndef _ZFI_ZFLevel_h_
#define _ZFI_ZFLevel_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief level for logic which depends on order
 *
 * higher priority ensured to have smaller value
 */
typedef enum {
    ZFLevelZFFrameworkStatic        = 0, /**< @brief ZFFramework static, for non-ZFObject-related meta data or global instance */
    ZFLevelZFFrameworkEssential     = 1, /**< @brief ZFFramework essential, for ZFObject-related meta data, or essential global instance */
    ZFLevelZFFrameworkHigh          = 2, /**< @brief ZFFramework high, for global instance */
    ZFLevelZFFrameworkNormal        = 3, /**< @brief ZFFramework, for logic initialization */
    ZFLevelZFFrameworkLow           = 4, /**< @brief ZFFramework low */

    ZFLevelAppEssential             = 5, /**< @brief app essential */
    ZFLevelAppHigh                  = 6, /**< @brief app high */
    ZFLevelAppNormal                = 7, /**< @brief app */
    ZFLevelAppLow                   = 8, /**< @brief app low */

    ZFLevelZFFrameworkPostLow       = 9, /**< @brief ZFFramework post low */
    ZFLevelZFFrameworkPostNormal    = 10, /**< @brief ZFFramework post normal */
    ZFLevelZFFrameworkPostHigh      = 11, /**< @brief ZFFramework post high */
    ZFLevelZFFrameworkPostEssential = 12, /**< @brief ZFFramework post essential */
    ZFLevelZFFrameworkPostStatic    = 13, /**< @brief ZFFramework post static */
} ZFLevel;

/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkStatic "LevelZFFrameworkStatic"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkEssential "LevelZFFrameworkEssential"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkHigh "LevelZFFrameworkHigh"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkNormal "LevelZFFrameworkNormal"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkLow "LevelZFFrameworkLow"

/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppEssential "LevelAppEssential"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppHigh "LevelAppHigh"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppNormal "LevelAppNormal"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelAppLow "LevelAppLow"

/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostLow "LevelZFFrameworkPostLow"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostNormal "LevelZFFrameworkPostNormal"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostHigh "LevelZFFrameworkPostHigh"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostEssential "LevelZFFrameworkPostEssential"
/** @brief string tokens */
#define ZFTOKEN_ZFLevelZFFrameworkPostStatic "LevelZFFrameworkPostStatic"

ZFOUTPUT_TYPE(ZFLevel, {
    switch(v) {
        case ZFLevelZFFrameworkStatic:
            s += ZFTOKEN_ZFLevelZFFrameworkStatic;
            break;
        case ZFLevelZFFrameworkEssential:
            s += ZFTOKEN_ZFLevelZFFrameworkEssential;
            break;
        case ZFLevelZFFrameworkHigh:
            s += ZFTOKEN_ZFLevelZFFrameworkHigh;
            break;
        case ZFLevelZFFrameworkNormal:
            s += ZFTOKEN_ZFLevelZFFrameworkNormal;
            break;
        case ZFLevelZFFrameworkLow:
            s += ZFTOKEN_ZFLevelZFFrameworkLow;
            break;
        case ZFLevelAppEssential:
            s += ZFTOKEN_ZFLevelAppEssential;
            break;
        case ZFLevelAppHigh:
            s += ZFTOKEN_ZFLevelAppHigh;
            break;
        case ZFLevelAppNormal:
            s += ZFTOKEN_ZFLevelAppNormal;
            break;
        case ZFLevelAppLow:
            s += ZFTOKEN_ZFLevelAppLow;
            break;
        case ZFLevelZFFrameworkPostLow:
            s += ZFTOKEN_ZFLevelZFFrameworkPostLow;
            break;
        case ZFLevelZFFrameworkPostNormal:
            s += ZFTOKEN_ZFLevelZFFrameworkPostNormal;
            break;
        case ZFLevelZFFrameworkPostHigh:
            s += ZFTOKEN_ZFLevelZFFrameworkPostHigh;
            break;
        case ZFLevelZFFrameworkPostEssential:
            s += ZFTOKEN_ZFLevelZFFrameworkPostEssential;
            break;
        case ZFLevelZFFrameworkPostStatic:
            s += ZFTOKEN_ZFLevelZFFrameworkPostStatic;
            break;
        default:
            break;
    }
})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLevel_h_

