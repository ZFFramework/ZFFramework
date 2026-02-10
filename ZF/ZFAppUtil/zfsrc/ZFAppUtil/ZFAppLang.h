/**
 * @file ZFAppLang.h
 * @brief helper for quickly update app language
 */

#ifndef _ZFI_ZFAppLang_h_
#define _ZFI_ZFAppLang_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief language info for #ZFAppLangDataList
 */
zfclass ZFLIB_ZFAppUtil ZFAppLangData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAppLangData, ZFObject)
public:
    /** @brief see #ZFEnvInfo::localeLangId */
    ZFPROPERTY_ASSIGN(zfstring, langId)
    /** @brief language name for display, same as #langId if not available */
    ZFMETHOD_DECLARE_0(zfstring, langName)

public:
    /** @brief list of items to load */
    zfobj<ZFArray> resList; // v_ZFPathInfo
};

/**
 * @brief get a list of supported language
 *
 * how it works:
 * -# you should prepare your language files in `lang` dir of #ZFPathType_res
 * -# the language files' file name should match this rule:
 *   -  `<langId>.*` file,
 *     e.g. `lang/en.xml` or `lang/en.xxx.xml`
 *   -  `<langId>.*` dir,
 *     e.g. `lang/en/` or `lang/en.xxx/`
 *   -  specially:
 *     -  `lang/default.*` file or `lang/default.*` dir would be loaded
 *       before loading each language
 *     -  `lang/config.*` file would be loaded to detect each lang names for display,
 *       with this format:
 *       @code
 *         <ZFStyleList>
 *             <zfstring prop="lang_<langId>" value="<langName>" />
 *             <zfstring prop="lang_en" value="English" />
 *             ...
 *         </ZFStyleList>
 *       @endcode
 * -# #ZFStyleLoadAsync would be called to load the language files
 *
 * typical usage:
 * @code
 *   // init and load default or previous language
 *   ZFAppLangInit();
 *
 *   // list available languages
 *   zfautoT<ZFArray> langList = ZFAppLangDataList();
 *
 *   // get current language, empty for default
 *   zfstring cur = ZFAppLangCur();
 *
 *   // let user to choose,
 *   // or use #ZFEnvInfo::localeLangId to check best match,
 *   // or fallback to use default
 *   zfstring desired = xxx;
 *
 *   // change language, empty for default language
 *   // the desired language would be saved to #ZFState,
 *   // and would be applied when next time app launch with #ZFAppLangInit
 *   ZFAppLangLoad(desired);
 *
 *   // use the language as #ZFStyleable::propStyle
 *   ZFUITextView *v = xxx;
 *   v->propStyle("text", "some_style_key");
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfautoT<ZFArray>, ZFAppLangDataList)
/**
 * @brief see #ZFAppLangDataList
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppLangInit
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )
/**
 * @brief see #ZFAppLangDataList
 *
 * finishCallback's param0 would be a #v_ZFResultType indicates result
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFAppLangLoad
        , ZFMP_IN(const zfstring &, langId)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )
/** @brief see #ZFAppLangDataList */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, const zfstring &, ZFAppLangCur)

// ============================================================
/**
 * @brief util to create a task for #ZFAppLangInit
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppLangInitTask)
/**
 * @brief util to create a task for #ZFAppLangLoad
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFTask>, ZFAppLangLoadTask
        , ZFMP_IN(const zfstring &, langId)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppLang_h_

