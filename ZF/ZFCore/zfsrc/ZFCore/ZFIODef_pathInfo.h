/**
 * @file ZFIODef_pathInfo.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIODef_pathInfo_h_
#define _ZFI_ZFIODef_pathInfo_h_

#include "ZFIODef_fwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFIOIsExist
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFIOIsDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFIOToFileName
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIOToChild
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childName)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFIOToParent
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFIOPathCreate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIORemove
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(zfbool, isRecursive, zftrue)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFIOMove
        , ZFMP_IN(const zfstring &, pathDataTo)
        , ZFMP_IN(const ZFPathInfo &, pathInfoFrom)
        , ZFMP_IN_OPT(zfbool, isForce, zftrue)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zftimet, ZFIOModTime
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFIOModTime
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(zftimet, time)
        )

/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfautoT<ZFIOToken>, ZFIOOpen
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(ZFIOOpenOptionFlags, flags)
        , ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue)
        )

// ============================================================
// utils
/**
 * @brief return local path info for current context
 *
 * this method is designed for script only,
 * when called from cpp side,
 * it always return a dummy path info
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFPathInfo, ZFLocalPathInfo
        , ZFMP_IN_OPT(const zfstring &, localPath, zfnull)
        )
/**
 * @brief util to make a child path info relative to existing one
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(ZFPathInfo &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        )
/**
 * @brief util to make a child path info relative to existing one
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoForLocalT
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        )
/**
 * @brief see #ZFPathInfoForLocalT
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFPathInfo, ZFPathInfoForLocal
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        )

// ============================================================
// ZFInputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForPathInfo"
 *       pathInfo="ZFPathInfo" // required, the path info
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForPathInfo "ZFInputForPathInfo"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFIO_DECLARE
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/**
 * @brief see #ZFInputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallback::callbackSerializeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFInputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )

/** @brief see #ZFInputForPathInfo */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFInput, ZFInputForIOToken
        , ZFMP_IN(ZFIOToken *, token)
        )
/** @brief see #ZFInputForPathInfo */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFInputForIOTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(ZFIOToken *, token)
        )

// ============================================================
// ZFOutputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFOutputForPathInfo"
 *       pathInfo="ZFPathInfo" // required, the path info
 *       flags="Write" // optional, open option
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForPathInfo "ZFOutputForPathInfo"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFIO_DECLARE
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForPathInfo
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )
/**
 * @brief see #ZFOutputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallback::callbackSerializeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFOutputForPathInfoT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )

/** @brief see #ZFOutputForPathInfo */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, ZFOutput, ZFOutputForIOToken
        , ZFMP_IN(ZFIOToken *, token)
        )
/** @brief see #ZFOutputForPathInfo */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFOutputForIOTokenT
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(ZFIOToken *, token)
        )

// ============================================================
// ZFInputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFInputForLocal"
 *       localPath="zfstring" // required, the local path
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFInputForLocal "ZFInputForLocal"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const zfstring &)localPath: local file path to use
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFInputForFile
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFIO_DECLARE
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFInput, ZFInputForLocal
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/**
 * @brief see #ZFInputForLocal
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFInputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )

// ============================================================
// ZFOutputForLocal
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback
 *       callbackType="ZFOutputForLocal"
 *       localPath="zfstring" // required, the local path
 *       flags="Write" // optional, open option
 *   >
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForLocal "ZFOutputForLocal"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const zfstring &)localPath: local file path to use
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo, when not valid,
 *   this method behaves same as #ZFOutputForFile
 *
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFIO_DECLARE
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, ZFOutput, ZFOutputForLocal
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )
/**
 * @brief see #ZFOutputForLocal
 */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFCore, zfbool, ZFOutputForLocalT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const zfstring &, localPath)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )

// ============================================================
/**
 * @brief util to mark a ZFInput serializable with specified pathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFInputMarkSerializable
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        )
/**
 * @brief util to mark a ZFOutput serializable with specified pathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void, ZFOutputMarkSerializable
        , ZFMP_IN_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Write)
        )

// ============================================================
/**
 * @brief util for impl to parse chained path data
 *
 * chained path info format: 'pathType1:pathType2:pathData2|pathData1'\n
 * or, if recursively chained: 'pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1'\n
 * note if the chained path info contains the special '|' char, it must be escaped by '%7c',
 * or use #ZFCoreDataEncode with #ZFPathInfoChainCharMap\n
 * \n
 * typical case:
 * @code
 *   // if original path info is:
 *   pathType1:pathType2:pathType3:pathData3%7cpathData2|pathData1
 *   // the pathDataOrig should be:
 *             pathType2:pathType3:pathData3%7cpathData2|pathData1
 *
 *   // after decode, chainPathInfo should be:
 *             pathType2
 *                       pathType3:pathData3|pathData2
 *   // pathData should be:
 *                                                       pathData1
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoChainDecode
        , ZFMP_OUT(ZFPathInfo &, chainPathInfo)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFPathInfoChainDecodeString
        , ZFMP_OUT(zfstring &, chainPathInfoString)
        , ZFMP_IN_OUT(zfstring &, pathData)
        , ZFMP_IN(const zfchar *, pathDataOrig)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, void, ZFPathInfoChainEncodeT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        )
/** @brief see #ZFPathInfoChainDecode */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfstring, ZFPathInfoChainEncode
        , ZFMP_IN(const ZFPathInfo &, chainPathInfo)
        , ZFMP_IN(const zfchar *, pathData)
        )

extern ZFLIB_ZFCore const zfchar _ZFP_ZFPathInfoChainCharMap[256];
/**
 * @brief see #ZFPathInfoChainDecode
 *
 * keep all printable chars except:
 * -  `%`
 * -  `|`
 */
#define ZFPathInfoChainCharMap() _ZFP_ZFPathInfoChainCharMap

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIODef_pathInfo_h_

