/**
 * @file ZFPathInfo.h
 * @brief path related info storage for impl
 */

#ifndef _ZFI_ZFPathInfo_h_
#define _ZFI_ZFPathInfo_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief path related info storage for impl,
 *   actual meaning of the info depends on impl
 *
 * recommend to use #ZFPATHTYPE_DECLARE/#ZFPATHTYPE_DEFINE for each of your path type,
 * and additionally use #ZFPATHTYPE_FILEIO_REGISTER to register your own IO type\n
 * \n
 * path info may describe any abstract files, including:
 * -  normal file
 * -  res file, which may be bundled in executable
 * -  url based file, which may be stored in net driver
 * -  compressed or encrypted file
 *
 * all file IO are abstracted as these operation:
 * -  #ZFPATHTYPE_FILEIO_REGISTER : supply low level file IO
 * -  #ZFInputForPathInfo / #ZFOutputForPathInfo :
 *   supply high level data IO
 *
 * with all the things above, app can achieve IO operations without knowing
 * the underlying file types
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFPathInfo {
public:
    zfstring pathType; /**< @brief path type, normal file or res file, etc */
    zfstring pathData; /**< @brief path value, such as local relative path or http url, etc */

public:
    /** @cond ZFPrivateDoc */
    ZFPathInfo(void) : pathType(), pathData() {}
    ZFPathInfo(ZF_IN const zfnullT &dummy) : pathType(), pathData() {}
    ZFPathInfo(ZF_IN const ZFPathInfo &pathInfo) : pathType(pathInfo.pathType), pathData(pathInfo.pathData) {}
    ZFPathInfo(ZF_IN const zfchar *pathInfoString);
    ZFPathInfo(ZF_IN const zfstring &pathType, ZF_IN const zfstring &pathData) : pathType(pathType), pathData(pathData) {}
    ZFPathInfo &operator = (ZF_IN const ZFPathInfo &pathInfo) {this->pathType = pathInfo.pathType; this->pathData = pathInfo.pathData; return *this;}
    ZFPathInfo &operator = (ZF_IN const zfnullT &dummy) {this->pathType = zfnull; this->pathData = zfnull; return *this;}
    zfbool operator == (ZF_IN const ZFPathInfo &pathInfo) const {return (this->pathType == pathInfo.pathType && this->pathData == pathInfo.pathData);}
    zfbool operator != (ZF_IN const ZFPathInfo &pathInfo) const {return !this->operator == (pathInfo);}
    zfbool operator == (ZF_IN const zfnullT &dummy) const {return (this->pathType == zfnull && this->pathData == zfnull);}
    zfbool operator != (ZF_IN const zfnullT &dummy) const {return !this->operator == (dummy);}
    operator zfbool (void) const {return !this->isEmpty();}
    /** @endcond */

public:
    /** @brief true if #pathType and #pathData is both empty */
    zfbool isEmpty(void) const {return this->pathType.isEmpty() && this->pathData.isEmpty();}
    /** @brief util to remove #pathType and #pathData */
    void removeAll(void) {this->pathType.removeAll(); this->pathData.removeAll();}
};

/**
 * @brief see #ZFPathInfo
 */
#define ZFPATHTYPE_DECLARE(ZFLIB_, pathType) \
    ZFMETHOD_FUNC_INLINE_DECLARE_0(ZFLIB_, const zfstring &, ZFPathType_##pathType) { \
        static zfstring ret = zftext(ZFM_TOSTRING(pathType)); \
        return ret; \
    }

/**
 * @brief see #ZFPathInfo
 */
#define ZFPATHTYPE_DEFINE(pathType) \
    ZFMETHOD_FUNC_INLINE_DEFINE_0(const zfstring &, ZFPathType_##pathType)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFPathInfo_h_

