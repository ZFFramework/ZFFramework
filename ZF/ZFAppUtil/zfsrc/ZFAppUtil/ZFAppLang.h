/**
 * @file ZFAppLang.h
 * @brief helper for quickly update app lang
 */

#ifndef _ZFI_ZFAppLang_h_
#define _ZFI_ZFAppLang_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief lang info for #ZFAppLangList
 */
zfclass ZFLIB_ZFAppUtil ZFAppLangData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAppLangData, ZFObject)
public:
    /** @brief langId according to child dir name of lang dir */
    ZFPROPERTY_ASSIGN(zfstring, langId)
    /** @brief lang name for display, same as #langId if not available */
    ZFMETHOD_DECLARE_0(zfstring, langName)

public:
    /** @brief list of items to load */
    zfobj<ZFArray> resList; // v_ZFPathInfo

protected:
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret += this->langId();
    }
};

/**
 * @brief get a list of supported lang
 *
 * how it works:
 * -# you should prepare your lang files in `lang` dir of #ZFPathType_res
 * -# the lang files' file name should match this rule:
 *   -  `<langId>.*` file,
 *     e.g. `lang/your_lang_id.xml` or `lang/your_lang_id.xxx.xml`
 *   -  `<langId>.*` dir,
 *     e.g. `lang/your_lang_id/` or `lang/your_lang_id.xxx/`
 *   -  specially:
 *     -  `lang/default.*` file or `lang/default.*` dir would be loaded
 *       before loading each lang
 *     -  `lang/config.*` file would be loaded to detect each lang names for display,
 *       with this format:
 *       @code
 *         <ZFStyleList>
 *             <zfstring prop="langName_<langId>" value="<langName>" />
 *             <zfstring prop="langName_your_lang_id" value="YourLangName" />
 *             ...
 *         </ZFStyleList>
 *       @endcode
 * -# #ZFStyleLoadAsync would be called to load the lang files
 *
 * typical usage:
 * @code
 *   // list available lang
 *   zfautoT<ZFArray> langList = ZFAppLangList();
 *
 *   // get current lang, empty for default
 *   zfstring cur = ZFAppLang();
 *
 *   // let user to choose,
 *   // or fallback to use default
 *   zfstring desired = xxx;
 *
 *   // change lang, empty for default lang
 *   // the desired lang would be saved to #ZFState,
 *   // and would be applied when next time app launch with #ZFAppLangInit
 *   ZFAppLangLoad(desired);
 *
 *   // use the lang as #ZFStyleable::propStyle
 *   ZFUITextView *v = xxx;
 *   v->propStyle("text", "some_style_key");
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFArray>, ZFAppLangList
        , ZFMP_IN_OPT(const ZFPathInfo &, base, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )
/**
 * @brief see #ZFAppLangList
 *
 * empty langId would load previous #ZFAppLang stored in #ZFState\n
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppLangLoad
        , ZFMP_IN_OPT(const zfstring &, langId, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(const ZFPathInfo &, base, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )
/** @brief see #ZFAppLangList */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfstring, ZFAppLang)

// ============================================================
/**
 * @brief util to create a task for #ZFAppLangLoad
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppLangLoadTask
        , ZFMP_IN_OPT(const zfstring &, langId, zfnull)
        , ZFMP_IN_OPT(const ZFPathInfo &, base, zfnull)
        , ZFMP_IN_OPT(zfbool, enableCache, zftrue)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppLang_h_

