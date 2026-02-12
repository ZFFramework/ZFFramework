/**
 * @file ZFAppSkin.h
 * @brief helper for quickly update app skin
 */

#ifndef _ZFI_ZFAppSkin_h_
#define _ZFI_ZFAppSkin_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief skin info for #ZFAppSkinList
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

protected:
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret += this->skinId();
    }
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
 *             <zfstring prop="skinName_<skinId>" value="<skinName>" />
 *             <zfstring prop="skinName_your_skin_id" value="YourSkinName" />
 *             ...
 *         </ZFStyleList>
 *       @endcode
 * -# #ZFStyleLoadAsync would be called to load the skin files
 *
 * typical usage:
 * @code
 *   // list available skin
 *   zfautoT<ZFArray> skinList = ZFAppSkinList();
 *
 *   // get current skin, empty for default
 *   zfstring cur = ZFAppSkin();
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
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFArray>, ZFAppSkinList
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )
/**
 * @brief see #ZFAppSkinList
 *
 * empty skinId would load previous #ZFAppSkin stored in #ZFState\n
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppSkinLoad
        , ZFMP_IN_OPT(const zfstring &, skinId, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )
/** @brief see #ZFAppSkinList */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfstring, ZFAppSkin)

// ============================================================
/**
 * @brief util to create a task for #ZFAppSkinLoad
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppSkinLoadTask
        , ZFMP_IN_OPT(const zfstring &, skinId, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppSkin_h_

