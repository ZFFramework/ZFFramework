/**
 * @page DocTag_Tutorial_Observer Observer
 *
 * with ZFCallback available,
 * it's easy to use observer mechanism
 * @code
 *   zfclass MyObject : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *
 *       ZFEVENT(MyEvent)
 *
 *   public:
 *       void test(void) {
 *           zfobj<ZFObject> myParam0;
 *           zfobj<ZFObject> myParam1;
 *           this->observerNotify(
 *               MyObject::E_MyEvent(), // event id to notify
 *               myParam0, // param0
 *               myParam1); // param1
 *       }
 *   };
 *   static void testListener1(ZF_IN const ZFArgs &zfargs) {
 *       ZFLog()
 *           << zfargs.sender() // should be MyObject
 *           << zfargs.param0() // should be myParam0 passed from observerNotify
 *           << zfargs.param1() // should be myParam1 passed from observerNotify
 *           ;
 *   }
 *   static void testListener2(
 *           ZF_IN const ZFArgs &zfargs
 *           , ZF_IN v_zfstring *myUserData
 *           ) {
 *       ZFLog()
 *           << zfargs.sender() // should be MyObject
 *           << zfargs.param0() // should be myParam0 passed from observerNotify
 *           << zfargs.param1() // should be myParam1 passed from observerNotify
 *           << myUserData // should be myUserData passed from observer
 *           ;
 *   }
 * @endcode
 * @code
 *   // the object to test observer
 *   zfobj<MyObject> obj;
 *   // the object to test user data passing
 *   zfobj<v_zfstring> myUserData("myUserData");
 *
 *   // we use ZFListener as observer callback type,
 *   // which takes two param as ZFArgs and ZFObject,
 *   // to create it,
 *   // use ZFCallbackForXXX series same as ZFCallback
 *   ZFListener listener1 = ZFCallbackForFunc(testListener1);
 *
 *   // add an observer
 *   obj->observerAdd(MyObject::E_MyEvent(), listener1);
 *
 *   // optionally, you may declare a lambda-like listener for short
 *   // or, for extra param passing
 *   ZFLISTENER_1(listener2
 *           , zfautoT<v_zfstring>, myUserData
 *           ) {
 *       testListener2(zfargs, myUserData);
 *   } ZFLISTENER_END()
 *   obj->observerAdd(MyObject::E_MyEvent(), listener2);
 *
 *   // finlly we will test to notify the event
 *   obj->test();
 *
 *   // or, you may notify it manually
 *   // but it's your responsibility to ensure the passed params suit for the listener
 *   zfobj<ZFObject> myParam0;
 *   zfobj<ZFObject> myParam1;
 *   obj->observerNotify(MyObject::E_MyEvent(), myParam0, myParam1);
 * @endcode
 */

