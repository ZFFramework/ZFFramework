#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_gc_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFLua_gc_test_Object, ZFObject)

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
ZFOBJECT_REGISTER(_ZFP_ZFLua_gc_test_Object)

zfclass ZFLua_gc_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFLua_gc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);

        ZFLogTrim() << "normal begin";
        ZFLuaExecute(
                "local obj = _ZFP_ZFLua_gc_test_Object();\n"
            );
        ZFLogTrim() << "normal gc begin";
        ZFLuaGCImmediately();
        ZFLogTrim() << "normal gc end";
        ZFLogTrim() << "normal end";

        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "note:";
        ZFLogTrim() << "    if compiled without cpp exception,";
        ZFLogTrim() << "    cpp object would leak when lua exception occurred";
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "lua exception begin";
        #if ZF_ENV_ZFLUA_USE_EXCEPTION
            ZFLuaExecute(
                    "local obj = _ZFP_ZFLua_gc_test_Object();\n"
                    "ZFObject.notExistFunc(obj);\n"
                );
            ZFLogTrim() << "lua exception gc begin";
            ZFLuaGCImmediately();
            ZFLogTrim() << "lua exception gc end";
        #else
            ZFLogTrim() << "lua exception disabled";
        #endif
        ZFLogTrim() << "lua exception end";

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFLua_gc_test)

ZF_NAMESPACE_GLOBAL_END

