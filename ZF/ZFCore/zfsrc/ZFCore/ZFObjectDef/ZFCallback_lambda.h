/**
 * @file ZFCallback_lambda.h
 * @brief header for ZFCallback
 */

#ifndef _ZFI_ZFCallback_lambda_h_
#define _ZFI_ZFCallback_lambda_h_

#include "ZFCallback.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFLAMBDA
 */
#define ZFLAMBDA_END() \
    _ZFP_ZFLAMBDA_END()

/**
 * @brief create a callback that simulate lambda function,
 *   which can capture params outside of scope
 *
 * usage:
 * @code
 *   CaptureParam0 capture0;
 *   CaptureParam1 capture1;
 *   CaptureParam2 capture2;
 *
 *   ZFLAMBDA_3(myCallback
 *           , CaptureParam0, capture0
 *           , CaptureParam1 &, capture1
 *           , CaptureParam2 const &, capture2
 *           , ReturnType
 *           , ZFMP_IN(ParamType0, param0)
 *           , ZFMP_IN_OPT(ParamType1, param1, zfnull)
 *           ) {
 *       capture1 = param1;
 *   } ZFLAMBDA_END()
 *
 *   ZFLogTrim() << capture0 << " " << capture1 << " " << capture2;
 *   myCallback.executeExact<ReturnType, ParamType0, ParamType1>(p0, p1);
 * @endcode
 */
#define ZFLAMBDA(name \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN_0(name \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_0(name \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN_0(name \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_1(name \
    , CaptureParam0, capture0 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_2(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_3(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_4(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_5(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_6(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_7(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_8(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_9(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_10(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_11(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_12(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_13(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_14(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_15(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_16(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_17(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_18(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_19(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_20(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_21(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_22(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_23(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_24(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_25(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_26(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_27(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_28(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , CaptureParam27, capture27 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam27, capture27 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_29(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , CaptureParam27, capture27 \
    , CaptureParam28, capture28 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam27, capture27 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam28, capture28 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_30(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , CaptureParam27, capture27 \
    , CaptureParam28, capture28 \
    , CaptureParam29, capture29 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam27, capture27 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam28, capture28 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam29, capture29 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_31(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , CaptureParam27, capture27 \
    , CaptureParam28, capture28 \
    , CaptureParam29, capture29 \
    , CaptureParam30, capture30 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam27, capture27 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam28, capture28 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam29, capture29 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam30, capture30 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA */
#define ZFLAMBDA_32(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    , CaptureParam9, capture9 \
    , CaptureParam10, capture10 \
    , CaptureParam11, capture11 \
    , CaptureParam12, capture12 \
    , CaptureParam13, capture13 \
    , CaptureParam14, capture14 \
    , CaptureParam15, capture15 \
    , CaptureParam16, capture16 \
    , CaptureParam17, capture17 \
    , CaptureParam18, capture18 \
    , CaptureParam19, capture19 \
    , CaptureParam20, capture20 \
    , CaptureParam21, capture21 \
    , CaptureParam22, capture22 \
    , CaptureParam23, capture23 \
    , CaptureParam24, capture24 \
    , CaptureParam25, capture25 \
    , CaptureParam26, capture26 \
    , CaptureParam27, capture27 \
    , CaptureParam28, capture28 \
    , CaptureParam29, capture29 \
    , CaptureParam30, capture30 \
    , CaptureParam31, capture31 \
    , ReturnType, ...) \
    _ZFP_ZFLAMBDA_BEGIN(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam8, capture8 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam9, capture9 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam10, capture10 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam11, capture11 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam12, capture12 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam13, capture13 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam14, capture14 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam15, capture15 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam16, capture16 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam17, capture17 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam18, capture18 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam19, capture19 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam20, capture20 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam21, capture21 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam22, capture22 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam23, capture23 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam24, capture24 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam25, capture25 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam26, capture26 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam27, capture27 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam28, capture28 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam29, capture29 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam30, capture30 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam31, capture31 \
        , ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

#define _ZFP_ZFLAMBDA_N_VA_EXPAND(...) , ##__VA_ARGS__

// ============================================================
#define _ZFP_ZFLambdaCapture_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLambdaCapture_EMPTY(...)

template<typename T_Type>
zfclassNotPOD _ZFP_ZFLAMBDA_TR {
public:
    typedef T_Type &T;
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFLAMBDA_TR<T_Type &> {
public:
    typedef T_Type & T;
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFLAMBDA_TR<T_Type const &> {
public:
    typedef T_Type const & T;
};

#define _ZFP_ZFLAMBDA_BEGIN_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLAMBDA_BEGIN(...) \
    _ZFP_ZFLAMBDA_BEGIN_EXPAND(_ZFP_ZFLAMBDA_BEGIN_(__VA_ARGS__))
#define _ZFP_ZFLAMBDA_BEGIN_(name \
        , CaptureExpandOrEmpty0, CaptureType0, capture0 \
        , CaptureExpandOrEmpty1, CaptureType1, capture1 \
        , CaptureExpandOrEmpty2, CaptureType2, capture2 \
        , CaptureExpandOrEmpty3, CaptureType3, capture3 \
        , CaptureExpandOrEmpty4, CaptureType4, capture4 \
        , CaptureExpandOrEmpty5, CaptureType5, capture5 \
        , CaptureExpandOrEmpty6, CaptureType6, capture6 \
        , CaptureExpandOrEmpty7, CaptureType7, capture7 \
        , ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        , ... \
    ) \
    ZFCallback name; \
    { \
        zfclassNotPOD _ZFP_Lbd_D_##name { \
        public: \
            CaptureExpandOrEmpty0(CaptureType0 capture0;) \
            CaptureExpandOrEmpty1(CaptureType1 capture1;) \
            CaptureExpandOrEmpty2(CaptureType2 capture2;) \
            CaptureExpandOrEmpty3(CaptureType3 capture3;) \
            CaptureExpandOrEmpty4(CaptureType4 capture4;) \
            CaptureExpandOrEmpty5(CaptureType5 capture5;) \
            CaptureExpandOrEmpty6(CaptureType6 capture6;) \
            CaptureExpandOrEmpty7(CaptureType7 capture7;) \
        public: \
            _ZFP_Lbd_D_##name( \
                CaptureExpandOrEmpty0(ZFM_EMPTY() CaptureType0 capture0) \
                CaptureExpandOrEmpty1(ZFM_COMMA() CaptureType1 capture1) \
                CaptureExpandOrEmpty2(ZFM_COMMA() CaptureType2 capture2) \
                CaptureExpandOrEmpty3(ZFM_COMMA() CaptureType3 capture3) \
                CaptureExpandOrEmpty4(ZFM_COMMA() CaptureType4 capture4) \
                CaptureExpandOrEmpty5(ZFM_COMMA() CaptureType5 capture5) \
                CaptureExpandOrEmpty6(ZFM_COMMA() CaptureType6 capture6) \
                CaptureExpandOrEmpty7(ZFM_COMMA() CaptureType7 capture7) \
            ) \
            CaptureExpandOrEmpty0(          : capture0(capture0)) \
            CaptureExpandOrEmpty1(ZFM_COMMA() capture1(capture1)) \
            CaptureExpandOrEmpty2(ZFM_COMMA() capture2(capture2)) \
            CaptureExpandOrEmpty3(ZFM_COMMA() capture3(capture3)) \
            CaptureExpandOrEmpty4(ZFM_COMMA() capture4(capture4)) \
            CaptureExpandOrEmpty5(ZFM_COMMA() capture5(capture5)) \
            CaptureExpandOrEmpty6(ZFM_COMMA() capture6(capture6)) \
            CaptureExpandOrEmpty7(ZFM_COMMA() capture7(capture7)) \
            { \
            } \
        public: \
            static void _ZFP_d(ZF_IN void *impl) { \
                delete (_ZFP_Lbd_D_##name *)impl; \
            } \
        }; \
        ZFCallback _ZFP_Lbd_v = ZFCallback::_ZFP_ZFCallbackCreateLambda( \
            new _ZFP_Lbd_D_##name( \
                CaptureExpandOrEmpty0(ZFM_EMPTY() capture0) \
                CaptureExpandOrEmpty1(ZFM_COMMA() capture1) \
                CaptureExpandOrEmpty2(ZFM_COMMA() capture2) \
                CaptureExpandOrEmpty3(ZFM_COMMA() capture3) \
                CaptureExpandOrEmpty4(ZFM_COMMA() capture4) \
                CaptureExpandOrEmpty5(ZFM_COMMA() capture5) \
                CaptureExpandOrEmpty6(ZFM_COMMA() capture6) \
                CaptureExpandOrEmpty7(ZFM_COMMA() capture7) \
            ), \
            _ZFP_Lbd_D_##name::_ZFP_d, \
            zfnull); \
        name = _ZFP_Lbd_v; \
        zfclassNotPOD _ZFP_Lbd_P_##name { \
        public: \
            _ZFP_Lbd_P_##name(ZF_IN_OUT ZFCallback &cb) { \
                cb._ZFP_ZFCallback_callbackLambdaInvoker((ZFFuncAddrType)_ZFP_Lbd_P_##name::_ZFP_i); \
            } \
        public: \
            static ReturnType _ZFP_i(_ZFP_Lbd_D_##name *_ZFP_d \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                    ) { \
                CaptureExpandOrEmpty0(_ZFP_ZFLAMBDA_TR<CaptureType0>::T capture0 = _ZFP_d->capture0;) \
                CaptureExpandOrEmpty1(_ZFP_ZFLAMBDA_TR<CaptureType1>::T capture1 = _ZFP_d->capture1;) \
                CaptureExpandOrEmpty2(_ZFP_ZFLAMBDA_TR<CaptureType2>::T capture2 = _ZFP_d->capture2;) \
                CaptureExpandOrEmpty3(_ZFP_ZFLAMBDA_TR<CaptureType3>::T capture3 = _ZFP_d->capture3;) \
                CaptureExpandOrEmpty4(_ZFP_ZFLAMBDA_TR<CaptureType4>::T capture4 = _ZFP_d->capture4;) \
                CaptureExpandOrEmpty5(_ZFP_ZFLAMBDA_TR<CaptureType5>::T capture5 = _ZFP_d->capture5;) \
                CaptureExpandOrEmpty6(_ZFP_ZFLAMBDA_TR<CaptureType6>::T capture6 = _ZFP_d->capture6;) \
                CaptureExpandOrEmpty7(_ZFP_ZFLAMBDA_TR<CaptureType7>::T capture7 = _ZFP_d->capture7;) \
                {
#define _ZFP_ZFLAMBDA_END() \
                } \
            } \
        } _ZFP_Lbd_p(_ZFP_Lbd_v); \
    }

// ============================================================
#define _ZFP_ZFLAMBDA_BEGIN_0_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLAMBDA_BEGIN_0(...) \
    _ZFP_ZFLAMBDA_BEGIN_0_EXPAND(_ZFP_ZFLAMBDA_BEGIN_0_(__VA_ARGS__))
#define _ZFP_ZFLAMBDA_BEGIN_0_(name \
        , ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        , ... \
    ) \
    ZFCallback name; \
    { \
        ZFCallback _ZFP_Lbd_v = ZFCallback::_ZFP_ZFCallbackCreateLambda( \
            zfnull, \
            zfnull, \
            zfnull); \
        name = _ZFP_Lbd_v; \
        zfclassNotPOD _ZFP_Lbd_P_##name { \
        public: \
            _ZFP_Lbd_P_##name(ZF_IN_OUT ZFCallback &cb) { \
                cb._ZFP_ZFCallback_callbackLambdaInvoker((ZFFuncAddrType)_ZFP_Lbd_P_##name::_ZFP_i); \
            } \
        public: \
            static ReturnType _ZFP_i(void * \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                    ) { \
                {

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallback_lambda_h_

