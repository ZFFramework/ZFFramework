/**
 * @file ZFObjectIO.h
 * @brief high level object IO
 */

#ifndef _ZFI_ZFObjectIO_h_
#define _ZFI_ZFObjectIO_h_

#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief high level object IO
 *
 * most object of ZFFramework are seraizliable,
 * and can be stored as different type (such as xml or json),
 * to make app level code more convenient,
 * we introduced this method to load/save serializable object
 * without knowing underlying storage type\n
 * \n
 * typical usage:
 * @code
 *   // serialize object from xml file
 *   zfauto obj = ZFObjectIOLoad(ZFInputForRes("data.xml"));
 *
 *   // load image
 *   zfauto img = ZFObjectIOLoad(ZFInputForRes("image.png"));
 * @endcode
 * \n
 * how it works:
 * -  get #ZFCallback::pathInfo from input/output
 * -  call all impl checker method to check whether it matches impl's type,
 *   until we find an impl can resolve it
 *   (for most case, this is done by checking the file ext from pathInfo's pathData)
 * -  call impl callback to serialize from input or to output
 *
 * \n
 * to add your own type,
 * please refer to #ZFOBJECTIO_DEFINE
 */
extern ZFLIB_ZFCore zfauto ZFObjectIOLoad(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
/** @brief see #ZFObjectIOLoad */
extern ZFLIB_ZFCore zfbool ZFObjectIOLoadT(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
/** @brief see #ZFObjectIOLoad */
extern ZFLIB_ZFCore zfbool ZFObjectIOSave(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );

// ============================================================
/**
 * @brief see #ZFObjectIOLoad
 *
 * usage:
 * @code
 *   ZFOBJECTIO_DEFINE(registerSig, {
 *           // callback to check whether the pathInfo can be used as object IO
 *           // proto type:
 *           //   zfbool checker(ZF_IN const ZFPathInfo &pathInfo);
 *           // pathInfo: ZFCallback::pathInfo
 *       }, {
 *           // callback to load object from input
 *           // proto type:
 *           //   zfbool fromInput(
 *           //           ZF_OUT zfauto &ret
 *           //           , ZF_IN_OUT const ZFInput &input
 *           //           , ZF_OUT_OPT zfstring *outErrorHint = zfnull
 *           //           );
 *       }, {
 *           // callback to save object to output
 *           // proto type:
 *           //   zfbool toOutput(
 *           //           ZF_IN_OUT const ZFOutput &output
 *           //           , ZF_IN zfany const &obj
 *           //           , ZF_OUT_OPT zfstring *outErrorHint = zfnull
 *           //           );
 *       })
 * @endcode
 */
#define ZFOBJECTIO_DEFINE(registerSig, checkerAction, fromInputAction, toOutputAction) \
    ZF_STATIC_REGISTER_INIT(ObjIOReg_##registerSig) { \
        _ZFP_ZFObjectIORegister(zftext(#registerSig), zfself::_ZFP_checker, zfself::_ZFP_fromInput, zfself::_ZFP_toOutput); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ObjIOReg_##registerSig) { \
        _ZFP_ZFObjectIOUnregister(zftext(#registerSig), zfself::_ZFP_checker, zfself::_ZFP_fromInput, zfself::_ZFP_toOutput); \
    } \
    static zfbool _ZFP_checker(ZF_IN const ZFPathInfo &pathInfo) { \
        checkerAction \
    } \
    static zfbool _ZFP_fromInput( \
            ZF_OUT zfauto &ret \
            , ZF_IN_OUT const ZFInput &input \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ) { \
        fromInputAction \
    } \
    static zfbool _ZFP_toOutput( \
            ZF_IN_OUT const ZFOutput &output \
            , ZF_IN zfany const &obj \
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull \
            ) { \
        toOutputAction \
    } \
    ZF_STATIC_REGISTER_END(ObjIOReg_##registerSig)

typedef zfbool (*_ZFP_ZFObjectIOCallback_checker)(ZF_IN const ZFPathInfo &pathInfo);
typedef zfbool (*_ZFP_ZFObjectIOCallback_fromInput)(
        ZF_OUT zfauto &ret
        , ZF_IN_OUT const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        );
typedef zfbool (*_ZFP_ZFObjectIOCallback_toOutput)(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN zfany const &obj
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        );
extern ZFLIB_ZFCore void _ZFP_ZFObjectIORegister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        );
extern ZFLIB_ZFCore void _ZFP_ZFObjectIOUnregister(
        ZF_IN const zfchar *registerSig
        , ZF_IN _ZFP_ZFObjectIOCallback_checker checker
        , ZF_IN _ZFP_ZFObjectIOCallback_fromInput fromInput
        , ZF_IN _ZFP_ZFObjectIOCallback_toOutput toOutput
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_h_

