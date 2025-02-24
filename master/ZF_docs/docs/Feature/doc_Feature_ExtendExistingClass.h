/**
 * @page DocTag_Feature_ExtendExistingClass Extend existing cpp class
 *
 * similar to category of Object-C, it's possible to extend existing cpp class in ZFFramework:
 * @code
 *   zfclass ExistClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(ExistClass, ZFObject)
 *       ZFMETHOD_INLINE_0(void, funcInExistClass) {
 *           ZFLogTrim("funcInExistClass: %s, cast to AttachClass: %s", this, zfcast(AttachClass *, this));
 *       }
 *   };
 *
 *   zfclass AttachClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(AttachClass, ZFObject)
 *       ZFMETHOD_INLINE_0(void, funcInAttachClass) {
 *           ZFLogTrim("funcInAttachClass: %s, cast to ExistClass: %s", this, zfcast(ExistClass *, this));
 *       }
 *   };
 *
 *   // extend ExistClass by AttachClass
 *   ZFCLASS_EXTEND(MyRegSig, ExistClass, AttachClass)
 *   // or
 *   ZF_GLOBAL_INITIALIZER_INIT(MyRegSig) {
 *       ZFImplementDynamicRegister(ExistClass::ClassData(), AttachClass::ClassData());
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyRegSig) {
 *       ZFImplementDynamicUnregister(ExistClass::ClassData(), AttachClass::ClassData());
 *   }
 *   ZF_GLOBAL_INITIALIZER_END(MyRegSig)
 *
 *   // now the ExistClass would have AttachClass's method
 *   zfobj<ExistClass> obj;
 *   obj->funcInExistClass();
 *   obj->to<AttachClass *>()->funcInAttachClass();
 *
 *   // this is useful in script
 *   ZFLuaExecute(
 *       "ZFImplementDynamicRegister(ExistClass.ClassData(), AttachClass.ClassData())\n"
 *       "\n"
 *       "local obj = ExistClass()\n"
 *       "obj:funcInExistClass()\n"
 *       "obj:funcInAttachClass()\n"
 *       );
 * @endcode
 */

