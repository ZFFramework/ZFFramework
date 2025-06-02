/**
 * @file ZFPathInfo.h
 * @brief path related info storage for impl
 */

#ifndef _ZFI_ZFPathInfo_h_
#define _ZFI_ZFPathInfo_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFPathInfoPrivate;
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
    /** @brief path type, normal file or res file, etc */
    ZFPathInfo &pathType(ZF_IN const zfstring &pathType);
    /** @brief path type, normal file or res file, etc */
    const zfstring &pathType(void) const;

    /** @brief path value, such as local relative path or http url, etc */
    ZFPathInfo &pathData(ZF_IN const zfstring &pathData);
    /** @brief path value, such as local relative path or http url, etc */
    const zfstring &pathData(void) const;

public:
    /** @brief true if #pathType and #pathData is both empty */
    zfbool isEmpty(void) const;
    /** @brief util to remove #pathType and #pathData */
    void removeAll(void);

public:
    /** @cond ZFPrivateDoc */
    ZFPathInfo(void);
    ZFPathInfo(ZF_IN const zfnullT &dummy);
    ZFPathInfo(ZF_IN const ZFPathInfo &ref);
    ZFPathInfo(ZF_IN const zfchar *pathInfoString);
    ZFPathInfo(ZF_IN const zfstring &pathType, ZF_IN const zfstring &pathData);
    ~ZFPathInfo(void);
    ZFPathInfo &operator = (ZF_IN const ZFPathInfo &ref);
    ZFPathInfo &operator = (ZF_IN const zfnullT &dummy);
    zfbool operator == (ZF_IN const ZFPathInfo &ref) const;
    zfbool operator != (ZF_IN const ZFPathInfo &ref) const {return !this->operator == (ref);}
    zfbool operator == (ZF_IN const zfnullT &dummy) const {return this->isEmpty();}
    zfbool operator != (ZF_IN const zfnullT &dummy) const {return !this->isEmpty();}
    inline operator zfbool (void) const {return !this->isEmpty();}
    /** @endcond */
private:
    _ZFP_ZFPathInfoPrivate *d;
};

ZFHASH_DECLARE(ZFPathInfo, {return zfidentityHash(zfidentityCalcString(v.pathType()), zfidentityCalcString(v.pathData()));})

/* ZFTAG_TRICKS: tricks to make ZFMap<ZFPathInfo, xxx> works */
inline zfint _ZFP_ZFPathInfoCompare(ZF_IN const ZFPathInfo &v0, ZF_IN const ZFPathInfo &v1) {
    zfint t = v0.pathType().compare(v1.pathType());
    if(t < 0) {
        return -1;
    }
    else if(t > 0) {
        return 1;
    }
    else {
        t = v0.pathData().compare(v1.pathData());
        if(t < 0) {
            return -1;
        }
        else if(t > 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
inline zfbool operator < (ZF_IN const ZFPathInfo &v0, ZF_IN const ZFPathInfo &v1) {return _ZFP_ZFPathInfoCompare(v0, v1) < 0;}
inline zfbool operator <= (ZF_IN const ZFPathInfo &v0, ZF_IN const ZFPathInfo &v1) {return _ZFP_ZFPathInfoCompare(v0, v1) <= 0;}
inline zfbool operator > (ZF_IN const ZFPathInfo &v0, ZF_IN const ZFPathInfo &v1) {return _ZFP_ZFPathInfoCompare(v0, v1) > 0;}
inline zfbool operator >= (ZF_IN const ZFPathInfo &v0, ZF_IN const ZFPathInfo &v1) {return _ZFP_ZFPathInfoCompare(v0, v1) >= 0;}

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

