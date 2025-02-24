/**
 * @page DocTag_Tutorial_UsingInterface Using interface
 *
 * similar to Interface in Java and protocol in Object-C,
 * ZFFramework has ZFInterface for interface types\n
 * to declare a ZFInterface:
 * @code
 *   zfinterface MyInterface : zfextend ZFInterface {
 *       ZFINTERFACE_DECLARE(MyInterface, ZFInterface)
 *   };
 *
 *   // in cpp files
 *   ZFOBJECT_REGISTER(MyInterface)
 * @endcode
 * -  use zfinterface to show it's an ZFInterface type
 * -  due to some limitatation,
 *   all interface type must be inherited from ZFInterface
 *   by "zfextend ZFInterface"
 * -  use ZFINTERFACE_DECLARE for necessary macro expansion
 * -  similar to ZFObject,
 *   use ZFOBJECT_REGISTER to register your interface
 *   if you want it to be reflectable
 *
 * \n
 * once declared, an interface can be implemented by other object
 * @code
 *   zfclass MyObject : zfextend ParentObject
 *                    , zfimplement MyInterface,
 *                    , zfimplement AnotherInterface
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ParentObject)
 *       ZFIMPLEMENT_DECLARE(MyInterface, AnotherInterface)
 *   };
 * @endcode
 * -  use zfimplement to implement an interface
 * -  use ZFIMPLEMENT_DECLARE for necessary macro expansion,
 *   an object can implement more than one interfaces
 */

