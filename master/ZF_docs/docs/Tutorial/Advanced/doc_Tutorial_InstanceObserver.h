/**
 * @page DocTag_Tutorial_InstanceObserver Instance observer
 *
 * ZFClass supply methods to observe event that any of specified class has created:
 * @code
 *   ZFLISTENER(objCreated) {
 *       zfLogT() << "MyClass created:" << zfargs.sender();
 *   } ZFLISTENER_END()
 *   MyClass::ClassData()->instanceObserverAdd(objCreated);
 *
 *   zfblockedAlloc(MyClass); // would cause objCreated being called
 *   zfblockedAlloc(SomeTypeOfMyClass); // would cause objCreated being called
 *   zfblockedAlloc(NotTypeOfMyClass); // would not cause objCreated being called
 * @endcode
 */

