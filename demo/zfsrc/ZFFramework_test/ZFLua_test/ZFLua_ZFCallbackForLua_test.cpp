#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_ZFCallbackForLua_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFLua_ZFCallbackForLua_test_Object, ZFObject)

    ZFEVENT(Test)

public:
    ZFMETHOD_INLINE_0(void, notifyTest) {
        this->observerNotify(zfself::E_Test(), zfobj<v_zfint>(1111), zfobj<v_zfint>(2222));
    }

protected:
    zfoverride
    virtual void objectOnInit(void) {
        ZFLog() << (void *)this;
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        ZFLog() << (void *)this;
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void) {
        zfsuper::objectOnRetain();
        ZFLog() << (void *)this << " " << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void) {
        zfsuper::objectOnRelease();
        ZFLog() << (void *)this << " " << this->objectRetainCount();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFLua_ZFCallbackForLua_test_Object)
ZFEVENT_REGISTER(_ZFP_ZFLua_ZFCallbackForLua_test_Object, Test)

zfclass ZFLua_ZFCallbackForLua_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFLua_ZFCallbackForLua_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("run lua code");
            ZFLuaExecute(
                    "local obj = zfAlloc('_ZFP_ZFLua_ZFCallbackForLua_test_Object')\n"
                    "local callback = function(zfargs)"
                    "        print(zfargs)\n"
                    "    end\n"
                    "obj:observerAdd(_ZFP_ZFLua_ZFCallbackForLua_test_Object.E_Test(), callback)\n"
                    "obj:notifyTest()\n"
                );
        }

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("lua gc");
            ZFLuaGC();
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFLua_ZFCallbackForLua_test)

ZF_NAMESPACE_GLOBAL_END

