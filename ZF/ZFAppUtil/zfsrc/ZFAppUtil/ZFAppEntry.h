/**
 * @file ZFAppEntry.h
 * @brief helper for quickly create app template
 */

#ifndef _ZFI_ZFAppEntry_h_
#define _ZFI_ZFAppEntry_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief helper for quickly create app template
 *
 * usage:
 * @code
 *   zfobj<ZFAppEntry>()->start();
 * @endcode
 *
 * what it does:
 * -# resolve #ZFAppEntry::res and #ZFAppEntry::resPack immediately
 * -# notify #ZFAppEntry::E_OnLoadState
 * -# load #ZFState
 * -# notify #ZFAppEntry::E_OnLoadLang
 * -# load #ZFAppLangLoad
 * -# notify #ZFAppEntry::E_OnLoadSkin
 * -# load #ZFAppSkinLoad
 * -# load custom #task
 * -# notify #ZFAppEntry::E_OnLoadEntry
 * -# load main entry, by this order,
 *   all of them can return `zftrue` to prevent further load task of this group:
 *   -# `res!!debug.lua`
 *   -# `res!!zf.lua`
 *   -# `res!!main.lua`
 * -# notify #ZFAppEntry::E_OnLoadStop
 *
 * all of load event's param0 is a #zfwrap that impl can set to #ZFTask
 * to perform additional load task
 */
zfclass ZFLIB_ZFAppUtil ZFAppEntry : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAppEntry, ZFObject)

public:
    /** @brief see #ZFAppEntry */
    ZFEVENT(OnLoadState)
    /** @brief see #ZFAppEntry */
    ZFEVENT(OnLoadLang)
    /** @brief see #ZFAppEntry */
    ZFEVENT(OnLoadSkin)
    /** @brief see #ZFAppEntry */
    ZFEVENT(OnLoadEntry)
    /** @brief see #ZFAppEntry */
    ZFEVENT(OnLoadStop)

public:
    /** @brief load event helper */
    ZFMETHOD_DECLARE_1(void, onLoadState
            , ZFMP_IN(const ZFListener &, callback)
            )
    /** @brief load event helper */
    ZFMETHOD_DECLARE_1(void, onLoadLang
            , ZFMP_IN(const ZFListener &, callback)
            )
    /** @brief load event helper */
    ZFMETHOD_DECLARE_1(void, onLoadSkin
            , ZFMP_IN(const ZFListener &, callback)
            )
    /** @brief load event helper */
    ZFMETHOD_DECLARE_1(void, onLoadEntry
            , ZFMP_IN(const ZFListener &, callback)
            )
    /** @brief load event helper */
    ZFMETHOD_DECLARE_1(void, onLoadStop
            , ZFMP_IN(const ZFListener &, callback)
            )

    // ============================================================
public:
    /**
     * @brief the encrypt key for encrypting #ZFState
     */
    ZFPROPERTY_ASSIGN(zfstring, stateEncryptKey, "ZFFramework")
    /**
     * @brief list of contents to entry
     */
    ZFPROPERTY_ASSIGN(ZFCoreArray<ZFPathInfo>, entryList, ZFCoreArrayCreate(ZFPathInfo
                , ZFPathInfo(ZFPathType_res(), "debug.lua")
                , ZFPathInfo(ZFPathType_res(), "zf.lua")
                , ZFPathInfo(ZFPathType_res(), "main.lua")
                ))

    // ============================================================
public:
    /**
     * @brief add additional res by #ZFResExtPathAdd
     */
    ZFMETHOD_DECLARE_1(void, res
            , ZFMP_IN(const ZFPathInfo &, pathInfo)
            )
    /**
     * @brief add additional res by #ZFAppRes::pathInfoForPackage
     */
    ZFMETHOD_DECLARE_2(void, resPack
            , ZFMP_IN(const ZFPathInfo &, packagePathInfo)
            , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
            )
    /**
     * @brief add additional res by #ZFAppRes::start
     */
    ZFMETHOD_DECLARE_2(void, resPack
            , ZFMP_IN(const ZFListener &, packageGetter)
            , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
            )
    /**
     * @brief add additional res by #ZFAppRes::start
     */
    ZFMETHOD_DECLARE_3(void, resPack
            , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageInfoList)
            , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
            , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
            )

public:
    /** @brief empty to use default */
    ZFPROPERTY_ASSIGN(ZFPathInfo, langPath)
    /** @brief empty to use default */
    ZFPROPERTY_ASSIGN(ZFPathInfo, skinPath)

    // ============================================================
public:
    /**
     * @brief util to run custom task, just before #E_OnLoadEntry
     */
    ZFMETHOD_DECLARE_1(void, task
            , ZFMP_IN(ZFTask *, task)
            )

    // ============================================================
public:
    /**
     * @brief see #ZFAppEntry
     */
    ZFMETHOD_DECLARE_1(void, start
            , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
            )
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppEntry_h_

