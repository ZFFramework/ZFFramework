/**
 * @file ZFAppSkin.h
 * @brief helper for quickly update app skin
 */

#ifndef _ZFI_ZFAppSkin_h_
#define _ZFI_ZFAppSkin_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief skin info for #ZFAppSkinDataList
 */
zfclass ZFLIB_ZFAppUtil ZFAppSkinData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAppSkinData, ZFObject)
public:
    /** @brief skinId according to child dir name of skin dir */
    ZFPROPERTY_ASSIGN(zfstring, skinId)
    /** @brief skin name for display, same as #skinId if not available */
    ZFMETHOD_DECLARE_0(zfstring, skinName)

public:
    /** @brief list of items to load */
    zfobj<ZFArray> resList; // v_ZFPathInfo
};

/**
 * @brief get a list of supported skin
 *
 * how it works:
 * -# you should prepare your skin files in `skin` dir of #ZFPathType_res
 * -# the skin files' file name should match this rule:
 *   -  `<skinId>.*` file,
 *     e.g. `skin/your_skin_id.xml` or `skin/your_skin_id.xxx.xml`
 *   -  `<skinId>.*` dir,
 *     e.g. `skin/your_skin_id/` or `skin/your_skin_id.xxx/`
 *   -  specially:
 *     -  `skin/default.*` file or `skin/default.*` dir would be loaded
 *       before loading each skin
 *     -  `skin/config.*` file would be loaded to detect each skin names for display,
 *       with this format:
 *       @code
 *         <ZFStyleList>
 *             <zfstring prop="skin_<skinId>" value="<skinName>" />
 *             <zfstring prop="skin_your_skin_id" value="YourSkinName" />
 *             ...
 *         </ZFStyleList>
 *       @endcode
 * -# #ZFStyleLoadAsync would be called to load the skin files
 *
 * typical usage:
 * @code
 *   // init and load default skin
 *   ZFAppSkinInit();
 *
 *   // list available skin
 *   zfautoT<ZFArray> skinList = ZFAppSkinDataList();
 *
 *   // get current skin, empty for default
 *   zfstring cur = ZFAppSkinCur();
 *
 *   // let user to choose,
 *   // or fallback to use default
 *   zfstring desired = xxx;
 *
 *   // change skin, empty for default skin
 *   // the desired skin would be saved to #ZFState,
 *   // and would be applied when next time app launch with #ZFAppSkinInit
 *   ZFAppSkinLoad(desired);
 *
 *   // use the skin as #ZFStyleable::propStyle
 *   ZFUITextView *v = xxx;
 *   v->propStyle("text", "some_style_key");
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfautoT<ZFArray>, ZFAppSkinDataList)
/**
 * @brief see #ZFAppSkinDataList
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppSkinInit
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )
/**
 * @brief see #ZFAppSkinDataList
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppSkinLoad
        , ZFMP_IN(const zfstring &, skinId)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )
/** @brief see #ZFAppSkinDataList */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, const zfstring &, ZFAppSkinCur)

// ============================================================
/**
 * @brief util to create a task for #ZFAppSkinInit
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppSkinInitTask)
/**
 * @brief util to create a task for #ZFAppSkinLoad
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppSkinLoadTask
        , ZFMP_IN(const zfstring &, langId)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppSkin_h_

