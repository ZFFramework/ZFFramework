/**
 * @file ZFMethodSerializable.h
 * @brief serialization for ZFMethod
 */

#ifndef _ZFI_ZFMethodSerializable_h_
#define _ZFI_ZFMethodSerializable_h_

#include "ZFSerializable.h"
#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethod value="methodSig" />
 * @endcode
 *
 * valid method signature:
 * -  "OwnerClass.methodName" :
 *   for class member type method
 * -  "OwnerClass.methodName:paramTypeId0:paramTypeId1" :
 *   for method with params
 * -  "methodName" :
 *   for function type method with default name space
 * -  ".methodName" :
 *   for function type method with default name space
 * -  "MethodNamespace0.MethodNamespace1.methodName" :
 *   for function type method with custom name space
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFMethod, const ZFMethod *)
ZFOUTPUT_TYPE(ZFMethod, {v.objectInfoT(s);})

/* ZFMETHOD_MAX_PARAM */
/**
 * @brief parse method from method sig
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodFromSig(
        ZF_IN const zfstring &classOrNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT const zfchar *paramTypeId0 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
        );
/**
 * @brief parse method from method sig
 *
 * methodSigPos must be successfully decoded by #ZFMethodSigSplit
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodFromSig(
        ZF_IN const zfstring &methodSig
        , ZF_IN const ZFCoreArray<ZFIndexRange> &methodSigPos);

/**
 * @brief split method sig
 *
 * result ensured to contain (ZFMETHOD_MAX_PARAM + 2) element if success:
 * -  pos[0] : range of OwnerClass or MethodNamespace, 0 length for global namespace
 * -  pos[1] : range of methodName, ensured not empty
 * -  pos[2] : range of paramTypeId0, 0 length if none
 * -  pos[3] : range of paramTypeId1, 0 length if none
 * -  pos[4] : range of paramTypeId2, 0 length if none
 * -  pos[5] : range of paramTypeId3, 0 length if none
 * -  pos[6] : range of paramTypeId4, 0 length if none
 * -  pos[7] : range of paramTypeId5, 0 length if none
 * -  pos[8] : range of paramTypeId6, 0 length if none
 * -  pos[9] : range of paramTypeId7, 0 length if none
 */
extern ZFLIB_ZFCore zfbool ZFMethodSigSplit(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfstring &src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodSerializable_h_

