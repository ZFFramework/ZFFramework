/**
 * @page DocTag_Feature_ExtendExistingClass Extend existing class
 *
 * similar to category of Object-C, it's possible to extend existing cpp class in ZFFramework:
 * @code
 *   zfclass ExistClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(ExistClass, ZFObject, ZFCore_ZFImplementDynamicRegister_test)
 *       ZFMETHOD_INLINE_0(void, funcInExistClass) {
 *           zfLogTrim("funcInExistClass: %s, cast to AttachClass: %s", this, zfcast(AttachClass *, this));
 *       }
 *   };
 *
 *   zfclass AttachClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(AttachClass, ZFObject, ZFCore_ZFImplementDynamicRegister_test)
 *       ZFMETHOD_INLINE_0(void, funcInAttachClass) {
 *           zfLogTrim("funcInAttachClass: %s, cast to ExistClass: %s", this, zfcast(ExistClass *, this));
 *       }
 *   };
 *
 *   // extend ExistClass by AttachClass
 *   ZFCLASS_EXTEND(ExistClass, AttachClass)
 *   // or
 *   ZFImplementDynamicRegister(ExistClass::ClassData(), AttachClass::ClassData());
 *
 *   // now the ExistClass would have AttachClass's method
 *   zfobj<ExistClass> obj;
 *   obj->funcInExistClass();
 *   obj->to<AttachClass *>()->funcInAttachClass();
 *
 *   // this is useful in script
 *   ZFLuaExecute(
 *       "local obj = ExistClass()\n"
 *       "obj:funcInExistClass()\n"
 *       "obj:funcInAttachClass()\n"
 *       );
 * @endcode
 *
 * as you can see, we are able to register class/method/property in script language\n
 * further more, the registered contents can be combined
 * with all other features in C++ world:
 * @code
 *   zfauto obj = ZFClass::classForName("MyClass")->newInstance();
 *   obj->invoke("myProp", zfobj<v_zfstring>("myValue"));
 *   ZFObjectToXml(ZFOutputDefault(), obj);
 * @endcode
 */

