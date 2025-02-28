/**
 * @file ZFCallbackSerializable.h
 * @brief serialization for ZFCallback
 */

#ifndef _ZFI_ZFCallbackSerializable_h_
#define _ZFI_ZFCallbackSerializable_h_

#include "ZFIOCallback.h"
#include "ZFSerializable.h"
#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   // for null callback
 *   <null />
 *
 *   // for #ZFCallbackForMethod
 *   <ZFCallback method="methodSig" />
 *
 *   // for #ZFCallbackForMemberMethod
 *   // not supported
 *
 *   // for #ZFCallbackForFunc
 *   // not supported
 * @endcode
 * \n
 * ADVANCED:\n
 * you may also supply your own serialize logic by registering
 * with #ZFCALLBACK_SERIALIZE_TYPE_DEFINE,
 * serializable data:
 * @code
 *   <ZFCallback callbackType="yourType" customAttr="customValue" >
 *       // custom datas
 *   </ZFCallback>
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFCallback, ZFCallback)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFCallback, ZFCallback)
ZFOUTPUT_TYPE(ZFCallback, {v.objectInfoT(s);})

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_method "method"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_callbackType "callbackType"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_callbackData "callbackData"

// ============================================================
ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_REG(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_REG(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_REG(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFInput, ZFInput)
ZFTYPEID_ALIAS_REG(ZFLIB_ZFCore, ZFCallback, ZFCallback, ZFInput, ZFInput)

// ============================================================
// custom serialize logic
typedef zfbool (*_ZFP_ZFCallbackSerializeCustomCallback)(
        ZF_IN_OUT ZFCallback &ret
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        );
extern ZFLIB_ZFCore void _ZFP_ZFCallbackSerializeCustomTypeRegister(
        ZF_IN const zfstring &customType
        , ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback
        );
extern ZFLIB_ZFCore void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfstring &customType);
extern ZFLIB_ZFCore _ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfstring &customType);

/**
 * @brief see #ZFTypeId_ZFCallback
 *
 * usage:
 * @code
 *   // in cpp file
 *   ZFCALLBACK_SERIALIZE_TYPE_DEFINE(registerSig, yourTypeName) {
 *       // serialize callback from data
 *       // proto type:
 *       //   zfbool action(
 *       //           ZF_IN_OUT ZFCallback &ret
 *       //           , ZF_IN const ZFSerializableData &serializableData
 *       //           , ZF_OUT_OPT zfstring *outErrorHint
 *       //           , ZF_OUT_OPT ZFSerializableData *outErrorPos
 *       //           );
 *       return zftrue;
 *   }
 * @endcode
 */
#define ZFCALLBACK_SERIALIZE_TYPE_DEFINE(registerSig, type) \
    static zfbool _ZFP_ZFCallbackSerializeCustom_##registerSig(ZF_IN_OUT ZFCallback &ret, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCallbackSerializeCustomTypeReg_##registerSig, ZFLevelZFFrameworkNormal) { \
        _ZFP_ZFCallbackSerializeCustomTypeRegister(type, _ZFP_ZFCallbackSerializeCustom_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFCallbackSerializeCustomTypeReg_##registerSig) { \
        _ZFP_ZFCallbackSerializeCustomTypeUnregister(type); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFCallbackSerializeCustomTypeReg_##registerSig) \
    static zfbool _ZFP_ZFCallbackSerializeCustom_##registerSig(ZF_IN_OUT ZFCallback &ret, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackSerializable_h_

