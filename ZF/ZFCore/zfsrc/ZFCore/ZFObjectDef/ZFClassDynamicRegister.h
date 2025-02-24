/**
 * @file ZFClassDynamicRegister.h
 * @brief dynamic register a ZFClass
 */

#ifndef _ZFI_ZFClassDynamicRegister_h_
#define _ZFI_ZFClassDynamicRegister_h_

#include "ZFClass.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dynamic register a class
 *
 * parent class must be object type that can be allocated\n
 * return null if parent class not valid
 * or class name already exists
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 * @note it's ensured safe to unregister class even if
 *   allocated object instance still exists,
 *   at this case ZFObject::classData would return parent class
 *   after the class unregistered
 */
extern ZFLIB_ZFCore const ZFClass *ZFClassDynamicRegister(
        ZF_IN const zfstring &classNameFull
        , ZF_IN_OPT const ZFClass *parent = zfnull
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief unregister class that registered by #ZFClassDynamicRegister
 */
extern ZFLIB_ZFCore void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls);

// ============================================================
/** @brief see #ZFCLASS_EXTEND */
extern ZFLIB_ZFCore zfbool ZFImplementDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        );
/** @brief see #ZFCLASS_EXTEND */
extern ZFLIB_ZFCore void ZFImplementDynamicUnregister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        );

/**
 * @brief advanced dynamic implement
 *
 * usage:
 * @code
 *   zfclass ExistClass : zfextend ZFObject {...};
 *   zfclass AttachClass : zfextend ZFObject {...};
 *
 *   // register
 *   // ZFImplementDynamicRegister(ExistClass::ClassData(), AttachClass::ClassData());
 *
 *   // or use the util macro for short
 *   ZFCLASS_EXTEND(ExistClass, AttachClass)
 *
 *   // create object
 *   zfobj<ExistClass> obj;
 *
 *   // access as AttachClass
 *   AttachClass *p = obj; // OK
 *
 *   // call AttachClass's method
 *   obj->invoke("methodInAttachClass"); // OK
 * @endcode
 * this is useful to extend existing class, simplar to category of Object-C
 */
#define ZFCLASS_EXTEND(ExistClass, AttachClass) \
    ZFCLASS_EXTEND_DETAIL(ExistClass, AttachClass, ExistClass##_##AttachClass)

/**
 * @brief #ZFCLASS_EXTEND with custom regSig
 */
#define ZFCLASS_EXTEND_DETAIL(ExistClass, AttachClass, regSig) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(DynImpl_##regSig, ZFLevelZFFrameworkStatic) { \
        ZFImplementDynamicRegister(ExistClass::ClassData(), AttachClass::ClassData()); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(DynImpl_##regSig) { \
        ZFImplementDynamicUnregister(ExistClass::ClassData(), AttachClass::ClassData()); \
    } \
    ZF_GLOBAL_INITIALIZER_END(DynImpl_##regSig)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassDynamicRegister_h_

