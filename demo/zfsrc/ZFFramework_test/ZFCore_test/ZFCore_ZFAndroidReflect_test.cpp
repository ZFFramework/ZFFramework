#include "ZFCore_test.h"

#include "ZFImpl/ZFImpl_env.h"
#if ZF_ENV_sys_Android
#include "ZFImpl/sys_Android/ZFImpl_sys_Android_ZFAndroidReflect.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFAndroidReflect_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFAndroidReflect_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("registering test class");
        this->testCaseOutputSeparator();
        if(ZFAndroidReflect_registerClass("com.ZFFramework.Android.ZFFramework_test.ZFAndroidReflectTest") == zfnull) {
            this->testCaseStop();
            return;
        }
        ZFAndroidReflect_registerClassContents("com.ZFFramework.Android.ZFFramework_test.ZFAndroidReflectTest");

        this->testCaseOutput("invoke constructors");
        this->testCaseOutputSeparator();
        zfauto obj = ZFInvoke("ZFAndroidReflectTest");
        ZFInvoke("ZFAndroidReflectTest", zflineAlloc(v_zfint, 1));
        ZFInvoke("ZFAndroidReflectTest", zflineAlloc(v_zfstring, "2"));
        ZFInvoke("ZFAndroidReflectTest", obj);
        ZFInvoke("ZFAndroidReflectTest", ZFInvoke("ZFAndroidReflectTest.TestIntArr"));
        ZFInvoke("ZFAndroidReflectTest", ZFInvoke("ZFAndroidReflectTest.TestStringArr"));
        ZFInvoke("ZFAndroidReflectTest", ZFInvoke("ZFAndroidReflectTest.TestUnknownObject"));

        zfstring errorHint;
        obj = ZFInvoke("ZFAndroidReflectTest", ZFCoreArrayCreate(zfauto, zflineAlloc(v_zfint, 1)), zfnull, &errorHint);
        this->testCaseOutput("invoke methods");
        this->testCaseOutputSeparator();
        zfLogTrim() << obj->invoke("test");
        zfLogTrim() << obj->invoke("testInt");
        zfLogTrim() << obj->invoke("testInteger");
        zfLogTrim() << obj->invoke("testString");
        zfLogTrim() << obj->invoke("testSelf");
        zfLogTrim() << obj->invoke("testIntArr");
        zfLogTrim() << obj->invoke("testStringArr");
        zfLogTrim() << obj->invoke("testUnknownObject");

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFAndroidReflect_test)

ZF_NAMESPACE_GLOBAL_END
#endif

