/**
 * @file ZFDebugServer.h
 * @brief debug server
 */

#ifndef _ZFI_ZFDebugServer_h_
#define _ZFI_ZFDebugServer_h_

#include "ZFAppUtilDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to start a debug http server that can receive and run lua code from remote
 *
 * typical usage:
 * @code
 *   // start the debug server
 *   ZFDebugServer(32100)
 *
 *   // for the client, post lua code to run on server
 *   ZFDebugClient([](const ZFArgs &zfargs) {
 *           ZFHttpResponse *recv = zfargs.param0();
 *           ZFLogTrim("%s", recv->body());
 *       }
 *       , "ZFUIViewTreePrint(ZFUISysWindow.mainWindow():rootView())"
 *       , "http://localhost:32100"
 *       );
 *
 *   // or, use plain curl
 *   curl -X POST -H "Content-Type:application/json;charset=UTF-8" \
 *       --data '{"run": "ZFUIViewTreePrint(ZFUISysWindow.mainWindow():rootView())"}' \
 *       "http://localhost:32100"
 *
 *   // response data:
 *   {
 *       "errno" : "0",
 *       "error" : "success",
 *       "result" : "result from code",
 *       "output" : "output from code",
 *   }
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAppUtil, zfautoT<ZFHttpServer>, ZFDebugServer
        , ZFMP_IN(zfuint, port)
        )

/** @brief see #ZFDebugServer */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFDebugClient
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN(const zfstring &, luaCode)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        )

/** @brief see #ZFDebugServer */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAppUtil, zfautoT<ZFTaskId>, ZFDebugClient
        , ZFMP_IN(const zfstring &, url)
        , ZFMP_IN(const ZFInput &, luaCode)
        , ZFMP_IN_OPT(const ZFListener &, callback, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDebugServer_h_

