/**
 * @file ZFProtocolZFRegExp.h
 * @brief protocol for ZFRegExp
 */

#ifndef _ZFI_ZFProtocolZFRegExp_h_
#define _ZFI_ZFProtocolZFRegExp_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFRegExp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFRegExp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFAlgorithm, ZFRegExp)
public:
    /**
     * @brief create a native reg exp
     */
    virtual void *nativeRegExpCreate(ZF_IN ZFRegExp *regExp) zfpurevirtual;
    /**
     * @brief destroy a native reg exp
     */
    virtual void nativeRegExpDestroy(
            ZF_IN ZFRegExp *regExp
            , ZF_IN void *nativeRegExp
            ) zfpurevirtual;

    /** @brief see ZFRegExp::pattern */
    virtual void pattern(
            ZF_IN ZFRegExp *regExp
            , ZF_IN const zfchar *pattern
            , ZF_IN_OPT ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault()
            ) zfpurevirtual;

    /** @brief see ZFRegExp::namedGroupIndexForName */
    virtual zfindex namedGroupIndexForName(
            ZF_IN ZFRegExp *regExp
            , ZF_IN const zfchar *name
            ) zfpurevirtual;

    /** @brief see ZFRegExp::find */
    virtual void find(
            ZF_IN ZFRegExp *regExp
            , ZF_OUT ZFRegExpResult &result
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLength = zfindexMax()
            ) zfpurevirtual;

    /** @brief see ZFRegExp::replace */
    virtual void replace(
            ZF_IN ZFRegExp *regExp
            , ZF_OUT zfstring &ret
            , ZF_OUT ZFRegExpResult &result
            , ZF_IN const zfchar *replacePattern
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLength = zfindexMax()
            , ZF_IN_OPT zfindex maxReplaceCount = zfindexMax()
            ) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFRegExp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFRegExp_h_

