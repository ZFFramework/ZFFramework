/**
 * @file ZFAppRes.h
 * @brief util to manager app res
 */

#ifndef _ZFI_ZFAppRes_h_
#define _ZFI_ZFAppRes_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to load resource from remote
 *
 * typical usage:
 * @code
 *   ZFAppRes::start([](const ZFArgs &zfargs) {
 *           zfautoT<ZFAppRes> o = zfargs.sender();
 *
 *           // request your server API to obtain download url and password
 *           // can be async task
 *           ...
 *
 *           // notifyFinish must be called exactly once
 *           o->notifyFinish(srcList, pwd);
 *       }, "my_module_name");
 * @endcode
 *
 * how it works:
 * -  preparing the package
 *   -  package should be built by #ZFAppRes::build
 * -  using the package
 *   -  call #ZFAppRes::start to start the resource module,
 *     which would use local cache if available,
 *     and use remote one for next app launch
 *   -  within the packageGetter,
 *     usually you should request your own server's API
 *     to obtain package download url,
 *     which is a list containing all download mirrors,
 *     all of the download mirrors would be tried one by one,
 *     until one download successfully
 *   -  finally, the package would be used as #ZFResExtPathAdd
 */
zfclass ZFLIB_ZFAppUtil ZFAppRes : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFAppRes, ZFObject)

public:
    /**
     * @brief build package with sourceDir
     */
    ZFMETHOD_DECLARE_STATIC_3(void, build
            , ZFMP_OUT(const ZFPathInfo &, resultPackage)
            , ZFMP_IN(const ZFPathInfo &, sourceDir)
            , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
            )

public:
    /**
     * @brief start and attach this res moudle
     *
     * see #ZFAppRes for more info\n
     * when moduleName is empty, "default" is used instead
     */
    ZFMETHOD_DECLARE_STATIC_2(void, start
            , ZFMP_IN(const ZFListener &, packageGetter)
            , ZFMP_IN_OPT(const zfstring &, moduleName, zfnull)
            )

public:
    /** @brief see #ZFAppRes */
    ZFMETHOD_DECLARE_2(void, notifyFinish
            , ZFMP_IN(const ZFCoreArray<ZFPathInfo> &, packageSrc)
            , ZFMP_IN_OPT(const zfstring &, packagePwd, zfnull)
            )

public:
    zfstring _ZFP_ZFAppRes_moduleName;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAppRes_h_

