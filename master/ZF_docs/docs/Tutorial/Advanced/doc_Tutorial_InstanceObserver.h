/**
 * @page DocTag_Tutorial_InstanceObserver Instance observer
 *
 * ZFClass supply methods to observe event that any of specified class has created:
 * @code
 *   ZFLISTENER(objCreated) {
 *       ZFLog() << "MyClass created: " << zfargs.sender();
 *   } ZFLISTENER_END()
 *   MyClass::ClassData()->instanceObserverAdd(objCreated);
 *
 *   zfobj<MyClass> p0; // would cause objCreated being called
 *   zfobj<SomeTypeOfMyClass> p1; // would cause objCreated being called
 *   zfobj<NotTypeOfMyClass> p2; // would not cause objCreated being called
 * @endcode
 */

