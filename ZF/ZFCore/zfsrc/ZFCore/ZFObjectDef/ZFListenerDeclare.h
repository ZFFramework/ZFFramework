/**
 * @file ZFListenerDeclare.h
 * @brief util macros to declare #ZFListener as #ZFCallback
 */

#ifndef _ZFI_ZFListenerDeclare_h_
#define _ZFI_ZFListenerDeclare_h_

#include "ZFCallback_lambda.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFLISTENER
 */
#define ZFLISTENER_END() \
    ZFLAMBDA_END()

#define _ZFP_ZFLISTENER_EXPAND(...) __VA_ARGS__
/**
 * @brief util to declare a #ZFListener locally
 *
 * usage:
 * @code
 *   // proto type:
 *   // void listenerName(ZF_IN const ZFArgs &zfargs);
 *   ZFLISTENER(yourListener) {
 *       // your code
 *   } ZFLISTENER_END()
 *   obj->observerAdd(eventId, yourListener);
 * @endcode
 *
 * you may also capture params outside of scope, see #ZFLAMBDA for more info
 * @code
 *   zfstring text0 = "text0";
 *   zfstring text1 = "text1";
 *   // proto type:
 *   // void listenerName(ZF_IN const ZFArgs &zfargs);
 *   ZFLISTENER_2(yourListener
 *           , zfstring, text0
 *           , zfstring &, text1
 *           ) {
 *       // your code
 *       text0 = "text_0";
 *       text1 = "text_1";
 *   } ZFLISTENER_END()
 *   ZFLogTrim() << text0 << " " << text1;
 *   yourListener.execute();
 *   ZFLogTrim() << text0 << " " << text1;
 * @endcode
 */
#define ZFLISTENER(name \
    ) \
    ZFLAMBDA(name \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_0(name \
    ) \
    ZFLAMBDA_0(name \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_1(name \
    , CaptureParam0, capture0 \
    ) \
    ZFLAMBDA_1(name \
        , CaptureParam0, capture0 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_2(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    ) \
    ZFLAMBDA_2(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_3(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    ) \
    ZFLAMBDA_3(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_4(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    ) \
    ZFLAMBDA_4(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_5(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    ) \
    ZFLAMBDA_5(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_6(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    ) \
    ZFLAMBDA_6(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_7(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    ) \
    ZFLAMBDA_7(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_8(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    ) \
    ZFLAMBDA_8(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , CaptureParam7, capture7 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_9(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , CaptureParam8, capture8 \
    ) \
    ZFLAMBDA_9(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , CaptureParam7, capture7 \
        , CaptureParam8, capture8 \
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_10(name \
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
    ) \
    ZFLAMBDA_10(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_11(name \
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
    ) \
    ZFLAMBDA_11(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_12(name \
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
    ) \
    ZFLAMBDA_12(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_13(name \
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
    ) \
    ZFLAMBDA_13(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_14(name \
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
    ) \
    ZFLAMBDA_14(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_15(name \
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
    ) \
    ZFLAMBDA_15(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_16(name \
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
    ) \
    ZFLAMBDA_16(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_17(name \
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
    ) \
    ZFLAMBDA_17(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_18(name \
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
    ) \
    ZFLAMBDA_18(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_19(name \
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
    ) \
    ZFLAMBDA_19(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_20(name \
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
    ) \
    ZFLAMBDA_20(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_21(name \
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
    ) \
    ZFLAMBDA_21(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_22(name \
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
    ) \
    ZFLAMBDA_22(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_23(name \
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
    ) \
    ZFLAMBDA_23(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_24(name \
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
    ) \
    ZFLAMBDA_24(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_25(name \
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
    ) \
    ZFLAMBDA_25(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_26(name \
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
    ) \
    ZFLAMBDA_26(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_27(name \
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
    ) \
    ZFLAMBDA_27(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_28(name \
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
    ) \
    ZFLAMBDA_28(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_29(name \
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
    ) \
    ZFLAMBDA_29(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_30(name \
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
    ) \
    ZFLAMBDA_30(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_31(name \
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
    ) \
    ZFLAMBDA_31(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

/** @brief see #ZFLISTENER */
#define ZFLISTENER_32(name \
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
    ) \
    ZFLAMBDA_32(name \
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
        , void \
        , _ZFP_ZFLISTENER_EXPAND(ZFMP_IN(const ZFArgs &, zfargs)) \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclare_h_

