/**
 * @file ZFSerializableDataSerializableConverter.h
 * @brief convert #ZFSerializableData from and to string
 */

#ifndef _ZFI_ZFSerializableDataSerializableConverter_h_
#define _ZFI_ZFSerializableDataSerializableConverter_h_

#include "ZFSerializableData.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief token used to output zfsd file
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFSDOutputToken {
public:
    /**
     * @brief whether pretty output, true by default
     */
    zfbool prettyPrint;

public:
    /** @cond ZFPrivateDoc */
    ZFSDOutputToken(void)
    : prettyPrint(zftrue)
    {
    }
    zfbool operator == (ZF_IN ZFSDOutputToken const &ref) const {
        return zftrue
            && this->prettyPrint == ref.prettyPrint
            ;
    }
    zfbool operator != (ZF_IN ZFSDOutputToken const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
/** @brief default output token */
extern ZFLIB_ZFCore ZFSDOutputToken &ZFSDOutputTokenDefault(void);
/** @brief default output token */
extern ZFLIB_ZFCore void ZFSDOutputTokenDefault(ZF_IN const ZFSDOutputToken &v);

/** @brief trim output token */
extern ZFLIB_ZFCore ZFSDOutputToken &ZFSDOutputTokenTrim(void);
/** @brief trim output token */
extern ZFLIB_ZFCore void ZFSDOutputTokenTrim(ZF_IN const ZFSDOutputToken &v);

/** @brief detailed output token */
extern ZFLIB_ZFCore ZFSDOutputToken &ZFSDOutputTokenDetail(void);
/** @brief detailed output token */
extern ZFLIB_ZFCore void ZFSDOutputTokenDetail(ZF_IN const ZFSDOutputToken &v);

// ============================================================
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataFromZFSD(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore ZFSerializableData ZFSerializableDataFromZFSD(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );

/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataToZFSD(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_IN_OPT const ZFSDOutputToken &token = ZFSDOutputTokenDefault()
        );

// ============================================================
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataFromZFSD(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const zfchar *encodedData
        , ZF_IN_OPT zfindex encodedDataLen = zfindexMax()
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
/**
 * @brief convert serializable data from string
 */
extern ZFLIB_ZFCore ZFSerializableData ZFSerializableDataFromZFSD(
        ZF_IN const zfchar *encodedData
        , ZF_IN_OPT zfindex encodedDataLen = zfindexMax()
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );

/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfbool ZFSerializableDataToZFSD(
        ZF_OUT zfstring &result
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_IN_OPT const ZFSDOutputToken &token = ZFSDOutputTokenDefault()
        );
/**
 * @brief convert serializable data to string
 */
extern ZFLIB_ZFCore zfstring ZFSerializableDataToZFSD(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_IN_OPT const ZFSDOutputToken &token = ZFSDOutputTokenDefault()
        );

// ============================================================
/**
 * @brief util method to parse object from input with zfsd format
 */
extern ZFLIB_ZFCore zfbool ZFObjectFromZFSD(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
/** @brief see #ZFObjectFromZFSD */
extern ZFLIB_ZFCore zfauto ZFObjectFromZFSD(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );

/**
 * @brief util method to convert serializable object to zfsd format
 */
extern ZFLIB_ZFCore zfbool ZFObjectToZFSD(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        , ZF_IN_OPT const ZFSDOutputToken &token = ZFSDOutputTokenDefault()
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataSerializableConverter_h_

