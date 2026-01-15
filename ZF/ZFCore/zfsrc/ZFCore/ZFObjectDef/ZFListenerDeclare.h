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

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclare_h_

