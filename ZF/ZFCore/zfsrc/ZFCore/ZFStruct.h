/**
 * @file ZFStruct.h
 * @brief abstract struct data util
 */

#ifndef _ZFI_ZFStruct_h_
#define _ZFI_ZFStruct_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief abstract struct data util
 *
 * designed for script and for convenient, by default:
 * -  ZFObject::objectCompare / ZFObject::objectCompareValue
 *   would be compared by comparing all property
 * -  ZFObject::objectInfo would print by #ZFObjectPrettyInfoT
 * -  can be construct from string, serialize from/to string,
 *   by serialize all property in declared order
 *
 * typical usage:
 * @code
 *   ZFDynamic()
 *       :classBegin('MyData', 'ZFStruct')
 *       :property('zfint', 'value1')
 *       :property('zfint', 'value2')
 *       ...
 *       :classEnd()
 *
 *   local v = MyData('(1,2)')
 *   --                 ^ ^
 *   --                 | value2
 *   --                 value1
 * @endcode
 */
zfclass ZFLIB_ZFCore ZFStruct : zfextend ZFStyle, zfimplement ZFProgressable {
    ZFOBJECT_DECLARE(ZFStruct, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFProgressable)

    /** @brief construct from string, see #ZFStruct */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const zfchar *, src)
            , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
            )

protected:
    zfoverride
    virtual zfidentity objectHashImpl(void);
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj);
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual zfbool serializableOnSerializeFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToString(
            ZF_IN_OUT zfstring &ret
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStruct_h_

