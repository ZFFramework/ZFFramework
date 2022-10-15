/**
 * @file ZFSerializableDataSerializableConverter.h
 * @brief convert #ZFSerializableData from and to string
 */

#ifndef _ZFI_ZFSerializableDataSerializableConverter_h_
#define _ZFI_ZFSerializableDataSerializableConverter_h_

#include "ZFSerializableData.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataFromZfsd(ZF_OUT ZFSerializableData &serializableData,
                                                      ZF_IN const ZFInput &input,
                                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull);
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore ZFSerializableData ZFSerializableDataFromZfsd(ZF_IN const ZFInput &input,
                                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull);

/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataToZfsd(ZF_IN_OUT const ZFOutput &output,
                                                    ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                    ZF_IN_OPT zfbool prettyPrint = zftrue);

// ============================================================
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataFromZfsd(ZF_OUT ZFSerializableData &serializableData,
                                                      ZF_IN const zfchar *encodedData,
                                                      ZF_IN_OPT zfindex encodedDataLen = zfindexMax(),
                                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull);
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore ZFSerializableData ZFSerializableDataFromZfsd(ZF_IN const zfchar *encodedData,
                                                                  ZF_IN_OPT zfindex encodedDataLen = zfindexMax(),
                                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull);

/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataToZfsd(ZF_OUT zfstring &result,
                                                    ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                    ZF_IN_OPT zfbool prettyPrint = zftrue);
/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfstring ZFSerializableDataToZfsd(ZF_IN const ZFSerializableData &serializableData,
                                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                      ZF_IN_OPT zfbool prettyPrint = zftrue);

// ============================================================
/**
 * @brief util method to parse object from input with zfsd format
 */
extern ZFLIB_ZFCore zfbool ZFObjectFromZfsd(ZF_OUT zfautoObject &ret,
                                            ZF_IN const ZFInput &input,
                                            ZF_OUT_OPT zfstring *outErrorHint = zfnull);
/** @brief see #ZFObjectFromZfsd */
extern ZFLIB_ZFCore zfautoObject ZFObjectFromZfsd(ZF_IN const ZFInput &input,
                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull);

/**
 * @brief util method to convert serializable object to zfsd format
 */
extern ZFLIB_ZFCore zfbool ZFObjectToZfsd(ZF_IN_OUT const ZFOutput &output,
                                          ZF_IN ZFObject *obj,
                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                          ZF_IN_OPT zfbool prettyPrint = zftrue);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataSerializableConverter_h_

