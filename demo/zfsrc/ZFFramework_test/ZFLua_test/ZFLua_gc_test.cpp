#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_gc_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFLua_gc_test_Object, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfLog() << (void *)this;
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfLog() << (void *)this;
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void)
    {
        zfsuper::objectOnRetain();
        zfLog() << (void *)this << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void)
    {
        zfsuper::objectOnRelease();
        zfLog() << (void *)this << this->objectRetainCount();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFLua_gc_test_Object)

zfclass ZFLua_gc_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_gc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);

        zfLogTrim() << "normal begin";
        ZFLuaExecute(
                "local obj = _ZFP_ZFLua_gc_test_Object();\n"
            );
        zfLogTrim() << "normal gc begin";
        ZFLuaGCImmediately();
        zfLogTrim() << "normal gc end";
        zfLogTrim() << "normal end";

        zfLogTrim() << "============================================================";
        zfLogTrim() << "note:";
        zfLogTrim() << "    if compiled without cpp exception,";
        zfLogTrim() << "    cpp object would leak when lua exception occurred";
        zfLogTrim() << "============================================================";
        zfLogTrim() << "lua exception begin";
        #if ZF_ENV_ZFLUA_USE_EXCEPTION
            ZFLuaExecute(
                    "local obj = _ZFP_ZFLua_gc_test_Object();\n"
                    "ZFObject.notExistFunc(obj);\n"
                );
            zfLogTrim() << "lua exception gc begin";
            ZFLuaGCImmediately();
            zfLogTrim() << "lua exception gc end";
        #else
            zfLogTrim() << "lua exception disabled";
        #endif
        zfLogTrim() << "lua exception end";

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_gc_test)

ZF_NAMESPACE_GLOBAL_END

