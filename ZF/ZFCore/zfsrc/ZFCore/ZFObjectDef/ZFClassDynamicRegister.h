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
        ZF_IN const zfchar *classNameFull
        , ZF_IN_OPT const ZFClass *parent = zfnull
        , ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief unregister class that registered by #ZFClassDynamicRegister
 */
extern ZFLIB_ZFCore void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls);

// ============================================================
/**
 * @brief util to dynamic register methods and properties from another class
 *
 * example for lua:
 * @code
 *   ZFDynamic()
 *       :classBegin('MyInterface')
 *       :property('zfstring', 'myProp')
 *       :classEnd();
 *
 *   ZFDynamic()
 *       :classBegin('MyClass')
 *       :classImplement('MyInterface')
 *       :propertyOnInt('myProp', function(zfargs)
 *           -- OK, override property init step
 *       end)
 *       :classEnd();
 *
 *   local obj = MyClass()
 *
 *   -- OK, myProp implemented from MyInterface
 *   zfLog('%s', obj:myProp())
 *
 *   -- false, only property and mehtod are copied, class has no actual relationship
 *   zfLog('%s', obj:classData():classIsTypeOf(MyInterface.ClassData()))
 *
 *   -- false, property and method are "copied", not "inherited"
 *   zfLog('%s', obj:classData():methodForName('myProp') == MyInterface.ClassData():methodForName('myProp'))
 * @endcode
 */
extern ZFLIB_ZFCore void ZFImplementDynamicRegister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        , ZF_OUT_OPT ZFCoreArray<const ZFMethod *> *methodRegistered = zfnull
        , ZF_OUT_OPT ZFCoreArray<const ZFProperty *> *propertyRegistered = zfnull
        );
/**
 * @brief unregister contents that registered by #ZFImplementDynamicRegister
 */
extern ZFLIB_ZFCore void ZFImplementDynamicUnregister(
        ZF_IN const ZFClass *cls
        , ZF_IN const ZFClass *clsToImplement
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassDynamicRegister_h_

